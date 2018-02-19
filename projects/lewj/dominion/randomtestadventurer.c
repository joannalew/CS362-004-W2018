/* -----------------------------------------------------------------------
 * Joanna Lew
 * Assignment 4
 * Randomly Testing Adventurer card effect from dominion.c
 * Include the following lines in your makefile:
 *
 * randomtestadventurer: randomtestadventurer.c dominion.o rngs.o
 *      gcc -o randomtestadventurer -g  randomtestadventurer.c dominion.o rngs.o $(CFLAGS)
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

int discardIndex(int p, struct gameState *G){
    int i;
    int index = -1;
    int treasures = 0;
    int cardDrawn;
    
    // draw cards from deck until none
    for (i = G->deckCount[p] - 1; i >= 0; i--){
        cardDrawn = G->deck[p][i];
        
        if (cardDrawn == copper || cardDrawn == silver || cardDrawn == gold){
            treasures++;
            if (treasures == 2){
                index = G->deckCount[p] - i;
                return index;
            }
        }
    }
    
    return index;
}


// Adventurer effect: draw cards until 2 treasure; keep treasure, discard rest
int checkAdventurerCard(int p, struct gameState *post, int handPos){
    int totalFailed = 0;
    
    int i, r;
    struct gameState pre;
    memcpy (&pre, post, sizeof(struct gameState));
    
    printf("POST0: hand %d, deck %d, discard %d\n", post->handCount[p], post->deckCount[p], post->discardCount[p]);
    
    // do cardEffect on POST gamestate
    r = cardEffect(adventurer, 0, 0, 0, post, handPos, 0);
    
    printf("POST1: hand %d, deck %d, discard %d\n", post->handCount[p], post->deckCount[p], post->discardCount[p]);
    
    if (r != 0){
        printf("Error: Adventurer cardEffect() failed\n");
        totalFailed++;
    }
    
    
    // do "manual" Adventurer effect on PRE gamestate
    int secondTreasureIndex = discardIndex(p, &pre);
    int deckCount = pre.deckCount[p];
    int cardDrawn;
    
    if (secondTreasureIndex == -1){
        secondTreasureIndex = pre.deckCount[p];
    }
    
    printf("PRE0: hand %d, deck %d, discard %d\n", pre.handCount[p], pre.deckCount[p], pre.discardCount[p]);
    
    for (i = 0; i < secondTreasureIndex; i++){
        cardDrawn = pre.deck[p][deckCount - i - 1];
        
        if (cardDrawn == copper || cardDrawn == silver || cardDrawn == gold){
            pre.hand[p][pre.handCount[p]] = cardDrawn;
            pre.handCount[p]++;
        }
        else{
            pre.discard[p][pre.discardCount[p]] = cardDrawn;
            pre.discardCount[p]++;
        }
        pre.deckCount[p]--;
    }
    
    
    printf("PRE1: hand %d, deck %d, discard %d\n", pre.handCount[p], pre.deckCount[p], pre.discardCount[p]);
    
    if (memcmp(&pre, post, sizeof(struct gameState)) != 0){
        printf("Error: Adventurer effect incorrect\n");
        totalFailed++;
    }
    
    return totalFailed;
}


int checkAdventurer(){
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
    
    
    // random number of treasures at some random indices
    int numTreasures = rand() % 5;
    int randIndex = -1;
    int totalTreasures = 0;
    
    for (int i = 0; i < numTreasures; i++){
        randIndex = rand() % G.deckCount[p];
        card = G.deck[p][randIndex];
        if (card != copper && card != silver && card != gold){
            totalTreasures++;
            G.deck[p][randIndex] = 4 + rand() % 3;          // copper = 4
        }
            
    }
    
    printf("Number of treasures: %d\n", totalTreasures);
    
    // change first card in hand to Adventurer
    printf("TESTING Adventurer card effect\n");
    if (maxHand > 0){
        G.hand[p][0] = adventurer;
        
        if (maxDeck > 0){
            if (checkAdventurerCard(p, &G, 0) == 0){
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
        totalFailed += checkAdventurer();
    }
    
    printf("\n%d/%d Tests Failed\n", totalFailed, numTests);
    return 0;
}
