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

int sudoAssert(int test){
    if (test)
        return 0;
    else
        return 1;
}

int main() {
    int checker = 0;
    int totalFailed = 0;
    
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
    printf("Testing empty province pile\n");

    checker += sudoAssert((isGameOver(&G) == 1));
    if (checker > 0){
        checker = 0;
        printf("Empty province pile failed\n");
        totalFailed++;
    }
    
    // test for game over if 3 supplies are empty
    // try every combination of empty supply cards
    int x_max = numCards - 3;
    int y_max = x_max + 1;
    int z_max = y_max + 1;
    
    printf("Testing empty supply piles\n");
    for (x = 0; x < x_max; x++){
        for (y = x+1; y < y_max; y++){
            for (z = y+1; z < z_max; z++){
                memset(&G, 23, sizeof(struct gameState));           // clear the game state
                r = initializeGame(numPlayer, k, seed, &G);         // initialize a new game
                
                G.supplyCount[k[x]] = 0;
                G.supplyCount[k[y]] = 0;
                G.supplyCount[k[z]] = 0;
#if (NOISY_TEST == 1)
                printf("Testing empty supply piles: [%d][%d][%d]\n", x, y, z);
#endif
                checker += sudoAssert((isGameOver(&G) == 1));
            }
        }
    }
    
    if (checker > 0){
        checker = 0;
        printf("Empty supply piles failed\n");
        totalFailed++;
    }
    
    // fixed test: new game -> not game over
    memset(&G, 23, sizeof(struct gameState));           // clear the game state
    r = initializeGame(numPlayer, k, seed, &G);         // initialize a new game

    printf("Testing new game is NOT game over\n");
    
    checker += sudoAssert((isGameOver(&G) == 0));
    if (checker > 0){
        checker = 0;
        printf("New game is NOT game over failed\n");
        totalFailed++;
    }
    
    if (totalFailed == 0)
        printf("All tests passed!\n");

    return 0;
}

