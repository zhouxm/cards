#include "Card.h"
#include "CardComp.h"
#include "gtest/gtest.h"
#include <iostream>

TEST(CardComp, DefaultConstructor)
{
    CardComp comp;
    EXPECT_EQ(BJOKER, comp.Get_suit());
    EXPECT_EQ(CARD_VAL_JOKER, comp.Get_val());
}

TEST(CardComp, HEARTAsPrime)
{
    Card cjoker(CJOKER, CARD_VAL_JOKER);
    Card bjoker(BJOKER, CARD_VAL_JOKER);
    Card ht_5(HEART, CARD_VAL_FIVE);    
    Card sp_5(SPADE, CARD_VAL_FIVE);
    Card dm_5(DIAMOND, CARD_VAL_FIVE);
    Card cl_5(CLUB, CARD_VAL_FIVE);
    Card ht_a(HEART, CARD_VAL_ACE);
    Card ht_3(HEART, CARD_VAL_THREE);
    Card sp_9(SPADE, CARD_VAL_NINE);
    Card sp_8(SPADE, CARD_VAL_EIGHT);
    Card dm_9(DIAMOND, CARD_VAL_NINE);
    Card dm_7(DIAMOND, CARD_VAL_SEVEN);
    Card cl_k(CLUB, CARD_VAL_KING);
    Card cl_2(CLUB, CARD_VAL_TWO);

    CardComp Less(HEART,CARD_VAL_FIVE);
    EXPECT_EQ(false, Less(bjoker, bjoker));

    EXPECT_EQ(true, Less(bjoker, cjoker));
    EXPECT_EQ(false, Less(cjoker, bjoker));
    EXPECT_EQ(true, Less(ht_5, bjoker));
    EXPECT_EQ(false, Less(bjoker,ht_5));
    EXPECT_EQ(true, Less(sp_5, ht_5));
    EXPECT_EQ(false, Less(ht_5, sp_5));
    EXPECT_EQ(true, Less(dm_5, sp_5));
    EXPECT_EQ(false, Less(sp_5, dm_5));
    EXPECT_EQ(true, Less(cl_5, dm_5));
    EXPECT_EQ(false, Less(dm_5, cl_5));
    EXPECT_EQ(true, Less(ht_a, ht_5));
    EXPECT_EQ(false, Less(ht_5, ht_a));
    EXPECT_EQ(true, Less(ht_3, ht_a));
    EXPECT_EQ(false, Less(ht_a, ht_3));
    EXPECT_EQ(true, Less(sp_9, ht_3));
    EXPECT_EQ(false, Less(ht_3, sp_9));
    EXPECT_EQ(true, Less(sp_8, sp_9));
    EXPECT_EQ(false, Less(sp_9, sp_8));
    EXPECT_EQ(true, Less(dm_7, ht_3));
    EXPECT_EQ(false, Less(ht_3, dm_7));
    EXPECT_EQ(true, Less(dm_7, dm_9));
    EXPECT_EQ(false, Less(dm_9, dm_7));
    EXPECT_EQ(true, Less(cl_k, ht_3));
    EXPECT_EQ(false, Less(ht_3, cl_k));
    EXPECT_EQ(true, Less(cl_2, cl_k));
    EXPECT_EQ(false, Less(cl_k, cl_2));
    EXPECT_EQ(true, Less(dm_9, sp_8));
    EXPECT_EQ(false, Less(sp_8, dm_9));
    EXPECT_EQ(true, Less(cl_k, dm_7));
    EXPECT_EQ(false, Less(dm_7, cl_k));
    EXPECT_EQ(true, Less(cl_k, sp_8));
    EXPECT_EQ(false, Less(sp_8, cl_k));
}

TEST(CardComp, CLUBAsPrimeWithSetOrder)
{
    Card cjoker(CJOKER, CARD_VAL_JOKER);
    Card bjoker(BJOKER, CARD_VAL_JOKER);
    Card ht_5(HEART, CARD_VAL_FIVE);    
    Card sp_5(SPADE, CARD_VAL_FIVE);
    Card dm_5(DIAMOND, CARD_VAL_FIVE);
    Card cl_5(CLUB, CARD_VAL_FIVE);
    Card ht_a(HEART, CARD_VAL_ACE);
    Card ht_3(HEART, CARD_VAL_THREE);
    Card sp_9(SPADE, CARD_VAL_NINE);
    Card sp_8(SPADE, CARD_VAL_EIGHT);
    Card dm_9(DIAMOND, CARD_VAL_NINE);
    Card dm_7(DIAMOND, CARD_VAL_SEVEN);
    Card cl_k(CLUB, CARD_VAL_KING);
    Card cl_2(CLUB, CARD_VAL_TWO);

    bjoker.Set_order(FIRST_PLAYED);
    cl_5.Set_order(FIRST_PLAYED);
    sp_5.Set_order(FIRST_PLAYED);
    dm_9.Set_order(FIRST_PLAYED);

    cl_k.Set_order(SECOND_PLAYED);
    ht_3.Set_order(SECOND_PLAYED);
    dm_5.Set_order(SECOND_PLAYED);

    cjoker.Set_order(THIRD_PLAYED);
    sp_9.Set_order(THIRD_PLAYED);
    dm_7.Set_order(THIRD_PLAYED);

    cl_2.Set_order(FOURTH_PLAYED);
    sp_8.Set_order(FOURTH_PLAYED);
    ht_a.Set_order(FOURTH_PLAYED);
    ht_5.Set_order(FOURTH_PLAYED);

    CardComp Less(CLUB, CARD_VAL_TEN);
    EXPECT_EQ(true, Less(sp_9, dm_9));
    EXPECT_EQ(false, Less(dm_9, sp_9));
    EXPECT_EQ(true, Less(ht_3, dm_9));
    EXPECT_EQ(false, Less(dm_9, ht_3));
    EXPECT_EQ(true, Less(ht_a, dm_9));
    EXPECT_EQ(false, Less(dm_9, ht_a));
    EXPECT_EQ(true, Less(sp_5, sp_9));
    EXPECT_EQ(false, Less(sp_9, sp_5));
    EXPECT_EQ(true, Less(ht_a, dm_5));
    EXPECT_EQ(false, Less(dm_5, ht_a));
    EXPECT_EQ(true, Less(sp_9, dm_5));
    EXPECT_EQ(false, Less(dm_5, sp_9));
    EXPECT_EQ(true, Less(cl_5, cl_k));
    EXPECT_EQ(false, Less(cl_k, cl_5));
    EXPECT_EQ(true, Less(dm_9, sp_5));
    EXPECT_EQ(false, Less(sp_5, dm_9));
    EXPECT_EQ(true, Less(cl_k, bjoker));
    EXPECT_EQ(false, Less(bjoker, cl_k));
    EXPECT_EQ(true, Less(bjoker, cjoker));
    EXPECT_EQ(false, Less(cjoker, bjoker));
} 

TEST(CardComp, NTAsPrime)
{
    Card cjoker(CJOKER, CARD_VAL_JOKER);
    Card bjoker(BJOKER, CARD_VAL_JOKER);
    Card ht_5(HEART, CARD_VAL_FIVE);    
    Card sp_5(SPADE, CARD_VAL_FIVE);
    Card dm_5(DIAMOND, CARD_VAL_FIVE);
    Card cl_5(CLUB, CARD_VAL_FIVE);
    Card ht_a(HEART, CARD_VAL_ACE);
    Card ht_3(HEART, CARD_VAL_THREE);
    Card sp_9(SPADE, CARD_VAL_NINE);
    Card sp_8(SPADE, CARD_VAL_EIGHT);
    Card dm_9(DIAMOND, CARD_VAL_NINE);
    Card dm_7(DIAMOND, CARD_VAL_SEVEN);
    Card cl_k(CLUB, CARD_VAL_KING);
    Card cl_2(CLUB, CARD_VAL_TWO);

    CardComp Less(BJOKER,CARD_VAL_JOKER);
    EXPECT_EQ(false, Less(bjoker, bjoker));

    /* Compare with joker */
    EXPECT_EQ(true, Less(bjoker, cjoker));
    EXPECT_EQ(false, Less(cjoker, bjoker));
    EXPECT_EQ(true, Less(ht_5, bjoker));
    EXPECT_EQ(false, Less(bjoker,ht_5));
    EXPECT_EQ(true, Less(sp_8, bjoker));
    EXPECT_EQ(false, Less(bjoker,sp_8));
    EXPECT_EQ(true, Less(cl_2, cjoker));
    EXPECT_EQ(false, Less(cjoker,cl_2));
    EXPECT_EQ(true, Less(dm_9, cjoker));
    EXPECT_EQ(false, Less(cjoker,dm_9));

    /* Same color, different value */
    EXPECT_EQ(true, Less(ht_5, ht_a));
    EXPECT_EQ(false, Less(ht_a, ht_5));
    EXPECT_EQ(true, Less(ht_3, ht_a));
    EXPECT_EQ(false, Less(ht_a, ht_3));
    EXPECT_EQ(true, Less(sp_8, sp_9));
    EXPECT_EQ(false, Less(sp_9, sp_8));
    EXPECT_EQ(true, Less(dm_7, dm_9));
    EXPECT_EQ(false, Less(dm_9, dm_7));
    EXPECT_EQ(true, Less(cl_2, cl_k));
    EXPECT_EQ(false, Less(cl_k, cl_2));
    
    /* Different color, same value */
    EXPECT_EQ(true, Less(ht_5, sp_5));
    EXPECT_EQ(false, Less(sp_5, ht_5));
    EXPECT_EQ(true, Less(dm_5, sp_5));
    EXPECT_EQ(false, Less(sp_5, dm_5));
    EXPECT_EQ(true, Less(dm_5, cl_5));
    EXPECT_EQ(false, Less(cl_5, dm_5));
    EXPECT_EQ(true, Less(ht_5, cl_5));
    EXPECT_EQ(false, Less(cl_5, ht_5));
    
    /* Different color, different value */
    EXPECT_EQ(true, Less(ht_3, sp_9));
    EXPECT_EQ(false, Less(sp_9, ht_3));    
    EXPECT_EQ(true, Less(dm_7, ht_3));
    EXPECT_EQ(false, Less(ht_3, dm_7));    
    EXPECT_EQ(true, Less(cl_k, ht_3));
    EXPECT_EQ(false, Less(ht_3, cl_k));    
    EXPECT_EQ(true, Less(dm_9, sp_8));
    EXPECT_EQ(false, Less(sp_8, dm_9));
    EXPECT_EQ(true, Less(dm_7, cl_k));
    EXPECT_EQ(false, Less(cl_k, dm_7));
    EXPECT_EQ(true, Less(cl_k, sp_8));
    EXPECT_EQ(false, Less(sp_8, cl_k));
}


TEST(CardComp, NTAsPrimeWithSetOrder)
{
    Card cjoker(CJOKER, CARD_VAL_JOKER);
    Card bjoker(BJOKER, CARD_VAL_JOKER);
    Card ht_6(HEART, CARD_VAL_FIVE);    
    Card sp_6(SPADE, CARD_VAL_FIVE);
    Card dm_6(DIAMOND, CARD_VAL_FIVE);
    Card cl_6(CLUB, CARD_VAL_FIVE);
    Card ht_a(HEART, CARD_VAL_ACE);
    Card ht_3(HEART, CARD_VAL_THREE);
    Card sp_9(SPADE, CARD_VAL_NINE);
    Card sp_8(SPADE, CARD_VAL_EIGHT);
    Card dm_9(DIAMOND, CARD_VAL_NINE);
    Card dm_7(DIAMOND, CARD_VAL_SEVEN);
    Card cl_k(CLUB, CARD_VAL_KING);
    Card cl_2(CLUB, CARD_VAL_TWO);

    bjoker.Set_order(FIRST_PLAYED);
    cl_6.Set_order(FIRST_PLAYED);
    sp_6.Set_order(FIRST_PLAYED);
    dm_9.Set_order(FIRST_PLAYED);

    cl_k.Set_order(SECOND_PLAYED);
    ht_3.Set_order(SECOND_PLAYED);
    dm_6.Set_order(SECOND_PLAYED);

    cjoker.Set_order(THIRD_PLAYED);
    sp_9.Set_order(THIRD_PLAYED);
    dm_7.Set_order(THIRD_PLAYED);

    cl_2.Set_order(FOURTH_PLAYED);
    sp_8.Set_order(FOURTH_PLAYED);
    ht_a.Set_order(FOURTH_PLAYED);
    ht_6.Set_order(FOURTH_PLAYED);

    CardComp Less(BJOKER,CARD_VAL_JOKER);

    /* Different card */
    EXPECT_EQ(true, Less(sp_9, dm_9));
    EXPECT_EQ(false, Less(dm_9, sp_9));
    EXPECT_EQ(true, Less(ht_3, dm_9));
    EXPECT_EQ(false, Less(dm_9, ht_3));
    EXPECT_EQ(true, Less(ht_a, dm_9));
    EXPECT_EQ(false, Less(dm_9, ht_a));
    EXPECT_EQ(true, Less(sp_6, sp_9));
    EXPECT_EQ(false, Less(sp_9, sp_6));
    EXPECT_EQ(true, Less(ht_a, dm_6));
    EXPECT_EQ(false, Less(dm_6, ht_a));
    EXPECT_EQ(true, Less(sp_9, dm_6));
    EXPECT_EQ(false, Less(dm_6, sp_9));
    EXPECT_EQ(true, Less(cl_6, cl_k));
    EXPECT_EQ(false, Less(cl_k, cl_6));
    EXPECT_EQ(true, Less(dm_9, sp_6));
    EXPECT_EQ(false, Less(sp_6, dm_9));
    EXPECT_EQ(true, Less(cl_k, bjoker));
    EXPECT_EQ(false, Less(bjoker, cl_k));
    EXPECT_EQ(true, Less(bjoker, cjoker));
    EXPECT_EQ(false, Less(cjoker, bjoker));

    /* Same card */
}

