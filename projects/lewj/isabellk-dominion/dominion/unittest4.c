/* -----------------------------------------------------------------------
 * Joanna Lew
 * Assignment 3
 * Testing scoreFor() from dominion.c
 * Include the following lines in your makefile:
 *
 * unittest4: unittest4.c dominion.o rngs.o
 *      gcc -o unittest4 -g  unittest4.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "rngs.h"

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 1

int sudoAssert(int test){
    if (test)
        return 0;
    else
        return 1;
}

int checkScoreFor(int card, int cardVal){
    int checker = 0;                        // for pseudo assert statement
    
    int i, p, r;
    int maxHand = 5, maxDeck = 20, maxDiscard = 20;
    int score = 0;
    int numPlayer = 2;
    int seed = 1000;
    int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
    struct gameState G;
    
    memset(&G, 23, sizeof(struct gameState));           // clear the game state
    r = initializeGame(numPlayer, k, seed, &G);         // initialize a new game
    p = G.whoseTurn;
    
    G.handCount[p] = maxHand;
    G.deckCount[p] = maxDeck;
    G.discardCount[p] = maxDiscard;
    
    // score = 0
    // replace hand, deck, and discard with all coppers
    for (i = 0; i < maxHand; i++){
        G.hand[p][i] = copper;
    }
    for (i = 0; i < maxDeck; i++){
        G.deck[p][i] = copper;
    }
    for (i = 0; i < maxDiscard; i++){
        G.discard[p][i] = copper;
    }
    
#if (NOISY_TEST == 1)
    printf("scoreFor: %d, expected %d\n", scoreFor(p, &G), score);
#endif
    checker += sudoAssert((scoreFor(p, &G) == score));
    
    
    // replace garden's card value with proper one
    if (card == gardens)
        cardVal = floor((maxHand + maxDeck + maxDiscard) / 10);
    
    // replace hand with card
    for (i = 0; i < maxHand; i++){
        G.hand[p][i] = card;
        score += cardVal;
#if (NOISY_TEST == 1)
        printf("scoreFor: %d, expected %d\n", scoreFor(p, &G), score);
#endif
        checker += sudoAssert((scoreFor(p, &G) == score));
    }
    
    // replace deck with card
    for (i = 0; i < maxDeck; i++){
        G.deck[p][i] = card;
        score += cardVal;
#if (NOISY_TEST == 1)
        printf("scoreFor: %d, expected %d\n", scoreFor(p, &G), score);
#endif
        checker += sudoAssert((scoreFor(p, &G) == score));
    }
    
    // replace discard with card
    for (i = 0; i < maxDiscard; i++){
        G.discard[p][i] = card;
        score += cardVal;
#if (NOISY_TEST == 1)
        printf("scoreFor: %d, expected %d\n", scoreFor(p, &G), score);
#endif
        checker += sudoAssert((scoreFor(p, &G) == score));
    }
    
    // removing single card from deck, hand, and discard
    G.hand[p][0] = copper;
    score -= cardVal;
#if (NOISY_TEST == 1)
    printf("scoreFor: %d, expected %d\n", scoreFor(p, &G), score);
#endif
    checker += sudoAssert((scoreFor(p, &G) == score));
    
    G.deck[p][0] = copper;
    score -= cardVal;
#if (NOISY_TEST == 1)
    printf("scoreFor: %d, expected %d\n", scoreFor(p, &G), score);
#endif
    checker += sudoAssert((scoreFor(p, &G) == score));
    
    G.discard[p][0] = copper;
    score -= cardVal;
#if (NOISY_TEST == 1)
    printf("scoreFor: %d, expected %d\n", scoreFor(p, &G), score);
#endif
    checker += sudoAssert((scoreFor(p, &G) == score));

    return checker;
}


int main() {
    int totalFailed = 0;
    
    // cards' victory points
    int estateVal = 1;
    int curseVal = -1;
    int duchyVal = 3;
    int provinceVal = 6;
    int greatHallVal = 1;
    
    printf ("TESTING scoreFor():\n");
    
    printf("Testing estate (+1)\n");
    if (checkScoreFor(estate, estateVal) > 0){
        printf("%s\n", "Estate test failed");
        totalFailed++;
    }
    
    printf("Testing great hall (+1)\n");
    if (checkScoreFor(great_hall, greatHallVal) > 0){
        printf("%s\n", "Great Hall test failed");
        totalFailed++;
    }
    
    printf("Testing duchy (+3)\n");
    if (checkScoreFor(duchy, duchyVal) > 0){
        printf("%s\n", "Duchy test failed");
        totalFailed++;
    }
    
    printf("Testing province (+6)\n");
    if (checkScoreFor(province, provinceVal) > 0){
        printf("%s\n", "Province test failed");
        totalFailed++;
    }
    
    printf("Testing curse (-1)\n");
    if (checkScoreFor(curse, curseVal) > 0){
        printf("%s\n", "Curse test failed");
        totalFailed++;
    }
    
    printf("Testing gardens (+cards/10)\n");
    if (checkScoreFor(gardens, 0) > 0){
        printf("%s\n", "Gardens test failed");
        totalFailed++;
    }
    
    if (totalFailed == 0)
        printf("All tests passed!\n");

    return 0;
}

