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


int main() {
    int i, p, r, handCount;
    int numPlayer;
    int seed = 1000;
    int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
    struct gameState G;
    
    printf ("TESTING numHandCards():\n");
    
    
    for (i = 0; i <= MAX_PLAYERS; i++){                     // test for every possible num of players
        numPlayer = i;
#if (NOISY_TEST == 1)
        printf("\nNumber of Players: %d\n", numPlayer);
#endif
        memset(&G, 23, sizeof(struct gameState));           // clear the game state
        r = initializeGame(numPlayer, k, seed, &G);         // initialize a new game
        
        for (p = 0; p < numPlayer; p++){                                // for each player in game
            for (handCount = 0; handCount < MAX_HAND; handCount++){     // test every possible hand count
                G.handCount[p] = handCount;
                G.whoseTurn = p;
#if (NOISY_TEST == 1)
                printf("(Player %d) G.handCount = %d, expected = %d\n", p, numHandCards(&G), handCount);
#endif
                assert(numHandCards(&G) == handCount);
            }
        }
    }
    
    printf("All tests passed!\n");

    return 0;
}

