/* -----------------------------------------------------------------------
 * Joanna Lew
 * Assignment 3
 * Testing isGameOver() from dominion.c
 * Include the following lines in your makefile:
 *
 * unittest2: unittest2.c dominion.o rngs.o
 *      gcc -o unittest2 -g  unittest2.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 1


int main() {
    int x, y, z, r;
    int numPlayer = 4;
    int seed = 1000;
    
    int numCards = 10;
    int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
    struct gameState G;
    
    printf ("TESTING isGameOver():\n");
    
    // fixed test: no more provinces -> end game
    memset(&G, 23, sizeof(struct gameState));           // clear the game state
    r = initializeGame(numPlayer, k, seed, &G);         // initialize a new game
    
    G.supplyCount[province] = 0;
#if (NOISY_TEST == 1)
    printf("testing empty province pile\n");
#endif
    assert(isGameOver(&G) == 1);
    
    // test for game over if 3 supplies are empty
    // try every combination of empty supply cards
    int x_max = numCards - 3;
    int y_max = x_max + 1;
    int z_max = y_max + 1;
    
    for (x = 0; x < x_max; x++){
        for (y = x+1; y < y_max; y++){
            for (z = y+1; z < z_max; z++){
                memset(&G, 23, sizeof(struct gameState));           // clear the game state
                r = initializeGame(numPlayer, k, seed, &G);         // initialize a new game
                
                G.supplyCount[k[x]] = 0;
                G.supplyCount[k[y]] = 0;
                G.supplyCount[k[z]] = 0;
#if (NOISY_TEST == 1)
                printf("testing empty supply piles: [%d][%d][%d]\n", x, y, z);
#endif
                assert(isGameOver(&G) == 1);
            }
        }
    }
    
    // fixed test: new game -> not game over
    memset(&G, 23, sizeof(struct gameState));           // clear the game state
    r = initializeGame(numPlayer, k, seed, &G);         // initialize a new game
#if (NOISY_TEST == 1)
    printf("testing new game is NOT game over\n");
#endif
    assert(isGameOver(&G) == 0);
    
    
    printf("All tests passed!\n");

    return 0;
}

