/* -----------------------------------------------------------------------
 * Joanna Lew
 * Assignment 4
 * Randomly Testing Village card effect from dominion.c
 * Include the following lines in your makefile:
 *
 * randomtestcard2: randomtestcard2.c dominion.o rngs.o
 *      gcc -o randomtestcard2 -g  randomtestcard2.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

#define NOISY_TEST 1

int sudoAssert(int test){
    if (test)
        return 0;
    else
        return 1;
}


// Village effect: +1 card, +2 actions
int checkVillageCard(int p, struct gameState *post, int handPos){
    int totalFailed = 0;
    
    int r;
    struct gameState pre;
    memcpy (&pre, post, sizeof(struct gameState));
    
    // do cardEffect on POST gamestate
    r = cardEffect(village, 0, 0, 0, post, handPos, 0);
    
    if (r != 0){
        printf("Error: Village cardEffect() failed\n");
        totalFailed++;
    }
    
    // do "manual" Village effect on PRE gamestate
    
    // draw a card
    // if there's a card in deck
    // put top card of deck in last position of hand
    if (pre.deckCount[p] > 0) {
        pre.handCount[p]++;
        pre.hand[p][pre.handCount[p]-1] = pre.deck[p][pre.deckCount[p]-1];
        pre.deckCount[p]--;
    }
    // if there's no card in deck, put the discard pile as deck
    // put top card of deck in last position of hand
    else if (pre.discardCount[p] > 0) {
        memcpy(pre.deck[p], post->deck[p], sizeof(int) * pre.discardCount[p]);
        memcpy(pre.discard[p], post->discard[p], sizeof(int)*pre.discardCount[p]);
        pre.hand[p][post->handCount[p]-1] = post->hand[p][post->handCount[p]-1];
        pre.handCount[p]++;
        pre.deckCount[p] = pre.discardCount[p]-1;
        pre.discardCount[p] = 0;
    }
    
    // +2 actions
    pre.numActions = pre.numActions + 2;
    
    // discard Village card
    // add village to played cards pile
    pre.playedCards[pre.playedCardCount] = pre.hand[p][handPos];
    pre.playedCardCount++;
    
    // set value to -1
    pre.hand[p][handPos] = -1;
    
    // if Village was last card in hand
    // or if Vmithy was the only card in hand
    // reduce number of cards in hand
    if (handPos == (pre.handCount[p] - 1)){
        pre.handCount[p]--;
    }
    else if (pre.handCount[p] == 1){
        pre.handCount[p]--;
    }
    // otherwise, replace discarded card with last card in hand
    // set last card to -1 and reduce number of cards in hand
    else {
        pre.hand[p][handPos] = pre.hand[p][(pre.handCount[p] - 1)];
        pre.hand[p][pre.handCount[p] - 1] = -1;
        pre.handCount[p]--;
    }
    
    // compare cardEffect and manual implementation
    if (memcmp(&pre, post, sizeof(struct gameState)) != 0){
        printf("Error: Village effect incorrect\n");
        totalFailed++;
    }
    
    return totalFailed;
}



int checkVillage(){
    int failed = 0;
    int r, p;
    
    // random game state
    int maxHand = rand() % MAX_HAND;
    int maxDeck = rand() % MAX_DECK;
    int maxDiscard = rand() % MAX_DECK;
    int numPlayer = 1 + rand() % MAX_PLAYERS;
    
    printf("players: %d, hand: %d, deck: %d, discard: %d\n", numPlayer, maxHand, maxDeck, maxDiscard);
    
    int seed = 1000;
    int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
    struct gameState G;
    
    // test if valid players (2-4), otherwise initialization fails
    if (numPlayer >= 2 && numPlayer <= MAX_PLAYERS){
        memset(&G, 23, sizeof(struct gameState));           // clear the game state
        r = initializeGame(numPlayer, k, seed, &G);         // initialize a new game
        p = G.whoseTurn;
        
        G.handCount[p] = maxHand;
        G.deckCount[p] = maxDeck;
        G.discardCount[p] = maxDiscard;
    }
    else{
        printf("Not enough players.\n");
        return 0;
    }
    
    // replace hand, deck, and discard with some cards available
    int card = rand() % 10;
    memset(G.hand[p], k[card], sizeof(int) * maxHand);
    
    card = rand() % 10;
    memset(G.deck[p], k[card], sizeof(int) * maxDeck);
    
    card = rand() % 10;
    memset(G.discard[p], k[card], sizeof(int) * maxDiscard);
    
    // change first card in hand to Village
    printf("TESTING Village card effect\n");
    if (maxHand > 0){
        G.hand[p][0] = village;
        
        if (maxDeck >= 1){
            if (checkVillageCard(p, &G, 0) == 0){
                printf("All tests passed!\n");
            }
            else{
                return 1;       // tested & failed
            }
        }
        else{
            printf("Not enough cards in deck.\n");
            return 0;
        }
    }
    else{
        printf("Not enough cards in hand.\n");
        return 0;
    }
    
    return 0;
}



int main () {
    srand(time(NULL));
    int i;
    int numTests = 50000;
    int totalFailed = 0;
    
    for (i = 0; i < numTests; i++){
        printf("Test %d: ", i+1);
        totalFailed += checkVillage();
    }
    
    printf("\n%d/%d Tests Failed\n", totalFailed, numTests);
    return 0;
}
