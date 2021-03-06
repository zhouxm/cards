#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <set>
#include <map>
#include <cstdlib>
#include <ctime>
#include "CardSet.h"
#include "Card.h"
#include "CardComp.h"

using std::multiset;
using std::ofstream;
using std::ios;
using std::endl;
using std::runtime_error;

CardSet::CardSet(int num, bool partial)
    : num_of_card_set(num), is_partial(partial)
{
    card_set = new multiset<Card> ();

    if (!partial){
        for (int i=0; i< num_of_card_set; i++){
            char val;
            for(val=CARD_VAL_TWO; val<CARD_VAL_JOKER; val++)
            {
                Card sp(SPADE, (card_val_t) val);
                Add_card(sp);
                Card ht(HEART, (card_val_t) val);
                Add_card(ht);
                Card cl(CLUB, (card_val_t) val);
                Add_card(cl);
                Card dm(DIAMOND, (card_val_t) val);
                Add_card(dm);
                //Display();
            }
            Card bj(BJOKER, (card_val_t) val);
            Add_card(bj);
            Card cj(CJOKER, (card_val_t) val);
            Add_card(cj);
        }
    }
}

CardSet::CardSet(int num, const uint8_t* pair_arr, const size_t sz)
    : num_of_card_set(num), is_partial(true)
{
    card_set = new multiset<Card> ();
    for( const uint8_t* ptr = pair_arr; ptr != pair_arr+sz; ptr++){
        //Add exception handling here
        Card cd(*ptr);
        if( !Add_card(cd) )
            throw runtime_error("Invalid pair_arr, more cards than allowed");
    }
}

CardSet::~CardSet()
{
    delete card_set;
}

bool CardSet::Add_card(const Card& card)
{
    if (card_set->count(card) < num_of_card_set){
        card_set->insert(card);
        return true;
    }
    return false;
}

bool CardSet::Del_card(const Card& card)
{
    //We only want to remove one card, so don't use erase(key) directly
    multiset<Card>::iterator iter = card_set->find(card);
    if(iter != card_set->end()) {
        card_set->erase(iter);
        return true;
    }
    return false;
}

bool CardSet::Is_empty() const
{
    return card_set->empty();
}

void CardSet::Display() const
{
    ofstream log_file;
    log_file.open("display.log", ios::app);
    log_file << "===== Beginning =====" << endl;
    multiset<Card>::const_iterator iter = card_set->begin();
    for (; iter!= card_set->end(); ++iter)
        log_file << *iter << endl; 
    log_file << "===== End =====" << endl << endl;
    log_file.close();
}

void CardSet::Set_prime(const Card& pm)
{
    if (pm.Get_prime() != pm.Get_char())
        Card::Set_prime(pm.Get_char());
    multiset<Card>* tmp 
        = new multiset<Card> ();
    tmp->insert(card_set->begin(), card_set->end());
    delete card_set;
    card_set =tmp;
}

static int seed_gen(int i) 
{ 
    std::srand(std::time(0));
    return std::rand()%i; 
}

void CardSet::Get_randomized_vector(vector<Card>& vec)
{
    vec.clear();
    vec.assign(card_set->begin(), card_set->end());
    std::random_shuffle(vec.begin(), vec.end(), seed_gen);
}

int CardSet::Get_point()
{
    multiset<Card>::const_iterator iter = card_set->begin();
    int ret = 0;
    while( iter!= card_set->end() ){
        if(iter->Get_val() == CARD_VAL_FIVE)
            ret += 5;
        else if (iter->Get_val() == CARD_VAL_TEN || iter->Get_val() == CARD_VAL_KING)
            ret += 10;
        ++iter;
    }
    return ret;
}

int CardSet::Get_char_array(uint8_t* parr, size_t len)
{
    multiset<Card>::const_iterator iter = card_set->begin();
    int i = 0;
    while( iter!= card_set->end() ){
        if( i>len )
            break;
        parr[i] = iter->Get_char();
        ++i;
        ++iter;
    }
    return i;
}
