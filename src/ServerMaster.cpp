#include "ServerMaster.h"

ServerMaster::ServerMaster(int nPlayers, int nCardSets)
{
    playerCount = nPlayers;
    cardSetCount = nCardSets;
    remainingCards = new CardSet(cardSetCount);

    usedCards = new CardSet[playerCount];//need to initialize for each player in constructor
    bottomCards = new CardSet(4 * cardSetCount);

    levelGap = LEVEL_GAP_DEFAULT;
    playerScore = new int[playerCount];
    currentRound = 0;
    banker = PLAYER_NONE;
    firstPlayer = PLAYER_1; //set to player_1 for use in dispatch in the first round


    MemSet(playingLevel, 0, 2);
    currentPrime = CARD_INVALID_VAL;
}

ServerMaster::~ServerMaster()
{
    //delete card sets
    delete remainingCards;
    delete usedCards;
    delete bottomCards;

    //kill workers
    //close sockets
}

bool ServerMaster::Init()
{
    bool status = TRUE;
    cout << "How many users? (Default = %d)" << playerCount << endl;
    cin >> playerCount;
    
    cout << "How many set of cards? (Default = %d)" << cardSetCount << endl;
    cin >> cardSetCount;
        
    // May need to init the size of card sets?
    
    //TODO: create sockets.
    
    g_fds = new int [playerCount]; 

    for(int idx = 0; idx < playerCount; idx++)
    {
        g_fds[idx] = accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
        if(-1 != g_fds[idx])
        {
            //For now, we just kill the program itself on error, error handling will be developed later.
            exit -1;
        }
    }

    //listen to each sockets
    //link to workes
    return status;
}

bool ServerMaster::WaitPlayerReady()
{
    //listen to sockets and wait for bind? I don't know how to write this.
}

void ServerMaster::Reset()//reset all data
{
    remainingCards.Clear();
    usedCards.Clear();
    bottomCards.Clear();

    for(int i = 0; i < playerCount; i++)
    {
        usedCards[i].Clear();
    }

    MemSet(playerScore, 0, playerCount);
    currentRound = 0;
    banker = PLAYER_NONE;
    firstPlayer = PLAYER_NONE;
        
    MemeSet(playingLevel, 0, 2);
    currentPrime = INVALID_CARD;
    currentState = SHUFFLE_CARDS;
}

void ServerMaster::Run()
{
    while(1)
    {
        switch(currentState)
        {
            case SHUFFLE_CARDS:
            {
                this->Shuffle();
                currentState = DISPATCHING_CARDS;					
                // Fall through
            }

            case DISPATCHING_CARDS:
            {
                this->DispatchCard();
                currentState = WAITING_PRIME;  
                // Fall through
            }

            case WAITING_PRIME:
            {
                this->WaitForPrime();
                currentState = CHANGE_CARD;
                //Fall through
            }

            case CHANGE_CARD:
            {
                this->ExchangeCard();
                currentState = PLAYING;
                firstPlayer = banker;
                //Fall through
            }

            case PLAYING:
            { 
                /* In playing game state */ 
                /* 
                * server -> client: Hey, you're the one to send card
                * client -> server: OK, here's my cards
                * server -> client: ACK FIXME: Is this necessary? //X1n: This is necessary, I think all card exchange action should be acked.
                */

                this->PlayOneHand();

                //compare cards and record score for this round
                // decide the first_player of the next round
                currentState = RECORD_SCORE;
            }

            case RECORD_SCORE:
            {
                //Do some record
                //Choose first player of next round
                this->RecordScore();
                if(!IsLastHand())
                {
                    currentState = PLAYING;
                    break;
                   
                }
                else
                {
                    currentState = ROUND_END;
                    // Fall through
                }
            }
        
            case ROUND_END:
            {
                this->RoundEnd();
                if((LEVEL_END == playingLevel[0]) || (LEVEL_END == playingLevel[1]))
                {
                    currentState = GAME_END;
                }
                else
                {
                    currentState = SHUFFLE_CARDS;
                }
                break;
            }
                
            case GAME_END:
            {
                this->GameEnd();
                    
                break;
            }
        }
    }
}


void ServerMaster::Shuffle()
{
    // TODO: the content here should be changed, pending the change of CardSet
    remainingCards.RandomizeCardSet(cardSetCount);
}

void ServerMaster::DispatchCard(Worker * workers)
{
    Card cardToDispatch;
    int curPlayer = firstPlayer;
    
    do
    {
        cardToDispatch = remainingCards.GetFirstCard();
        workers[curPlayer].FetchCard(cardToDispatch);
        curPlayer = ((curPlayer + 1) == MAX_PLAYER_COUNT) ? PLAYER_1 : (curPlayer + 1);
    }while(remainingCards.GetCount() > 8);
}

void ServerMaster::WaitForPrime()
{//TODO: need to set banker based on if it is the first round, it should be also implemented in ClaimPrime()
    int curPlayer = firstPlayer;
    int idx;
    
    for(idx = 0; idx < playerCount; idx++)
    {
        workers[curPlayer].NeedPrime();
        curPlayer++;
        if(curPlayer == playerCount)
        {
            curPlayer = PLAYER_1;
        }
    }

    sleep(5*1000);//wait for 5s for player to claim other prime
    //workers who claims prime should wakeup me!!!
    //Note: validation of claimed prime is performed in ClaimPrime() which will be called by worker.
    //      And it will decide whether keep sleep or wakeup
    //      But how long should it be for the second sleep?

    //still nobody claims
    if(CARD_INVALID_VAL == currentPrime)
    {
        currentPrime = bottomCards.GetBiggest();
        //TODO: who is the banker now?
    }
    
    //notify workers
    for(curPlayer = PLAYER_1; curPlayer < playerCount; curPlayer++)
    {
        workers[curPlayer].GetPrime();
        workers[curPlayer].GetBanker();
    }
}

void ServerMaster::ExchangeCard()
{
    //send all the left cards to banker
    //And wait for the banker to send cards back
    Card cardToDispatch;
    for (int idx = 0; idx < 8; idx++)
    {
        cardToDispatch = remainingCards.GetFirstCard();
        workers[banker].FetchCard(cardToDispatch);
    }

    while(8 != bottomCards.GetCardCount())
    {
        // Notify and return
        workers[banker].WaitChangeCard();
        sleep(1000);
    }
    //TODO: wait for banker to return cards to bottomCards, maybe wait for a flag?
}

void ServerMaster::PlayOneHand()
{
    PLAYERNAME curPlayer = firstPlayer;
    
    do
    {
        cardPlayed[curPlayer] = workers[curPlayer].PlayOneCard();
        curPlayer = (curPlayer + 1) % playerCount;
    }while(curPlayer != firstPlayer);
}

void ServerMaster::RecordScore()
{
    Card bigCard = cardPlayed[firstPlayer];
    PLAYERNAME winner = firstPlayer;
    PLAYERNAME player = (firstPlayer + 1) % MAX_PLAYER_COUNT;
    int tempScore = cardPlayed[firstPlayer].GetScore();
    do
    {
        if(bigCard < cardPlayed[player])
        {
            bigCard = cardPlayed[player];
            winner = player;
        }
        tempScore += cardPlayed[player].GetScore();
        player = (player + 1) % MAX_PLAYER_COUNT;
    }while(player != firstPlayer);

    playerScore[winner % 2] += tempScore;
}

void ServerMaster::RoundEnd()
{
    //calculate new level and set new first player
    if ((PLAYER_1 == banker) || (PLAYER_3 == banker))
    {
        if (playerScore[1] >= 80)
        {
            //turn over
            banker++;
            //won't have below issue, need to move to else..
            if (MAX_PLAYER_COUNT == banker)
            {
                banker = PLAYER_1;
            }
        }
        else
        {
            if()
        }
    }
    else
    {
        ...
    }

    // need to clear variables for the next round
    currentPrime = CARD_INVALID_VAL;
    playerScore[0] = playerScore[1] = 0;
}

void ServerMaster::GameEnd()
{
    //notify the worker, Show the game result
    //exit program or start another game by user's choice
    bool playAgain;
    cout << "Wanner play again?" << endl;
    cin >> playAgain;

    if (playAgain)
    {
        this->Reset();
        currentState = SHUFFLE_CARDS;
    }
}

void ServerMaster::GetScore(int* scoreContainer)
{
    for (int idx = PLAYER_1; idx < playerCount; idx++)
    {
        scoreContainer[idx] = playerScore[idx];
    }
}


int ServerMaster::GetCurrentRound()
{
    return currentRound;
}

int ServerMaster::GetPlayingLevel(PLAYERNAME player)
{
    if ((PLAYER_1 == player) || (PLAYER_3 == player))
    {
        return playingLevel[0];
    }
    else
    {
        return playingLevel[1];
    }
}

Card ServerMaster::GetCurrentPrime()
{
    return currentPrime;
}

int ServerMaster::GetCurrentRound()
{
    return currentRound;
}

int ServerMaster::GetBanker(void)
{
    return banker;
}

void ServerMaster::SetBanker(int newBanker)
{
    banker = newBanker;
}

void ServerMaster::SetLevelGap(int gap)
{
    levelGap = gap;
}

bool ServerMaster::IsBanker(int player)
{
    return (player == banker);
}

bool ServerMaster::IsLastHand()
{
    return (cardsInHand[banker]->GetCardCount() == 0);
}

