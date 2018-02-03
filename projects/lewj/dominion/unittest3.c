/* -----------------------------------------------------------------------
 * Joanna Lew
 * Assignment 3
 * Testing fullDeckCount() from dominion.c
 * Include the following lines in your makefile:
 *
 * unittest3: unittest3.c dominion.o rngs.o
 *      gcc -o unittest3 -g  unittest3.c dominion.o rngs.o $(CFLAGS)
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
    int i, j, p, r;
    int handCount, deckCount, discardCount;
    int maxHand = 5, maxDeck = 30, maxDiscard = 30;
    int numPlayer = 2;
    int total = 0;
    int seed = 1000;
    
    int numCards = 10;
    int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
    struct gameState G;
    
    printf ("TESTING fullDeckCount():\n");
    
    memset(&G, 23, sizeof(struct gameState));           // clear the game state
    r = initializeGame(numPlayer, k, seed, &G);         // initialize a new game
    
    
    
    // test kingdom cards not in starting deck
#if (NOISY_TEST == 1)
    printf("Kingdom cards NOT in starting deck\n");
#endif
    
    for (i = 0; i < numCards; i++){
        p = G.whoseTurn;
        assert(fullDeckCount(p, k[i], &G) == 0);
    }
    
    // change each card in full deck to available kingdom cards
    // for each kingdom card available
    for (int i = 0; i < numCards; i++){
        total = 0;
        
        j = 0;
        // change each card in hand
        for (handCount = 0; handCount <= maxHand; handCount++){
            G.handCount[p] = handCount;
            G.hand[p][j++] = k[i];
            total = handCount;
#if (NOISY_TEST == 1)
            printf("(Card %d) fullDeckCount = %d, expected %d\n", k[i], fullDeckCount(p, k[i], &G), total);
#endif
            assert(fullDeckCount(p, k[i], &G) == total);
        }
        
        j = 0;
        // change each card in deck
        for (deckCount = 0; deckCount <= maxDeck; deckCount++){
            G.deckCount[p] = deckCount;
                G.deck[p][j++] = k[i];
            total = maxHand + deckCount;
#if (NOISY_TEST == 1)
            printf("(Card %d) fullDeckCount = %d, expected %d\n", k[i], fullDeckCount(p, k[i], &G), total);
#endif
            assert(fullDeckCount(p, k[i], &G) == total);
        }
        
        j = 0;
        // change each card in discard
        for (discardCount = 0; discardCount <= maxDiscard; discardCount++){
            G.discardCount[p] = discardCount;
                G.discard[p][j++] = k[i];
            total = maxHand + maxDeck + discardCount;
#if (NOISY_TEST == 1)
            printf("(Card %d) fullDeckCount = %d, expected %d\n", k[i], fullDeckCount(p, k[i], &G), total);
#endif
            assert(fullDeckCount(p, k[i], &G) == total);
        }
        
    }
    
    // test that non-available kingdom card not in deck
    // (current card range: 7-16)
#if (NOISY_TEST == 1)
    printf("(Card %d) fullDeckCount = %d, expected %d\n", 20, fullDeckCount(p, k[i], &G), 0);
#endif
    assert(fullDeckCount(p, 20, &G) == 0);
    
    
    
    printf("All tests passed!\n");

    return 0;
}

