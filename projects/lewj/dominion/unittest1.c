/* -----------------------------------------------------------------------
 * Joanna Lew
 * Assignment 3
 * Testing numHandCards() from dominion.c
 * Include the following lines in your makefile:
 *
 * unittest1: unittest1.c dominion.o rngs.o
 *      gcc -o unittest1 -g  unittest1.c dominion.o rngs.o $(CFLAGS)
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


int main() {
    int checker = 0;
    int totalFailed = 0;
    
    int i, p, r, handCount;
    int numPlayer;
    int seed = 1000;
    int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
    struct gameState G;
    
    printf ("TESTING numHandCards():\n");
    
    // test for every num of players
    for (i = 0; i <= MAX_PLAYERS; i++){
        numPlayer = i;
        printf("Testing for %d players\n", numPlayer);
        
        memset(&G, 23, sizeof(struct gameState));                       // clear the game state
        r = initializeGame(numPlayer, k, seed, &G);                     // initialize a new game
        
        // for each player in game
        // test every possible hand count [0 ... MAX_HAND)
        for (p = 0; p < numPlayer; p++){
            for (handCount = 0; handCount < MAX_HAND; handCount++){
                G.handCount[p] = handCount;
                G.whoseTurn = p;
#if (NOISY_TEST == 1)
                printf("(Player %d) G.handCount = %d, expected = %d\n", p, numHandCards(&G), handCount);
#endif
                checker += sudoAssert((numHandCards(&G) == handCount));
            }
        }
        
        if (checker > 0){
            printf("Test failed for %d players\n", numPlayer);
            totalFailed++;
        }
    }
    
    if (totalFailed == 0)
        printf("All tests passed!\n");

    return 0;
}

