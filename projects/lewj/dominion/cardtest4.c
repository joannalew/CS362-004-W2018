/* -----------------------------------------------------------------------
* Joanna Lew
* Assignment 3
* Testing Adventurer card effect from dominion.c
* Include the following lines in your makefile:
*
* cardtest4: cardtest4.c dominion.o rngs.o
*      gcc -o cardtest4 -g  cardtest4.c dominion.o rngs.o $(CFLAGS)
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


// Adventurer effect: draw cards until 2 treasure; keep treasure, discard rest
int checkAdventurerCard(int p, struct gameState *post, int handPos){
    int totalFailed = 0;
    
    int r;
    int drawntreasure = 0;
    int z = 0;                      // counter for temp hand
    int cardDrawn;
    int tempHand[MAX_HAND];
    
    struct gameState pre;
    memcpy (&pre, post, sizeof(struct gameState));
    
    // do cardEffect on POST gamestate
    r = cardEffect(adventurer, 0, 0, 0, post, handPos, 0);
    
    if (r != 0){
        printf("Error: Adventurer cardEffect() failed\n");
        totalFailed++;
    }
    

    // do "manual" Adventurer effect on PRE gamestate
    
    // draw cards until two treasures found, or run through entire deck
    while (drawntreasure < 2){
        // if deck and discard are empty
        if (pre.deckCount[p] < 1 && pre.discardCount[p] < 1)
            break;
        
        // if deck is empty, put discard as deck
        if (pre.deckCount[p] < 1 && pre.discardCount[p] > 0){
            memcpy(pre.deck[p], post->deck[p], sizeof(int) * pre.discardCount[p]);
            memcpy(pre.discard[p], post->discard[p], sizeof(int)*pre.discardCount[p]);
            pre.deckCount[p] = pre.discardCount[p];
            pre.discardCount[p] = 0;
        }
        else{
            break;           // already shuffled and run out of cards -> end
        }
    
        // if there's a card in deck
        if (pre.deckCount[p] > 0) {
            // put top card of deck in hand
            pre.handCount[p]++;
            pre.hand[p][pre.handCount[p]-1] = pre.deck[p][pre.deckCount[p]-1];
            pre.deckCount[p]--;
            
            // if treasure, keep in hand; else, put in tempHand to discard
            cardDrawn = pre.hand[p][pre.handCount[p]-1];
            if (cardDrawn == copper || cardDrawn == silver || cardDrawn == gold)
                drawntreasure++;
            else{
                tempHand[z] = cardDrawn;
                pre.handCount[p]--;
                z++;
            }
        }
    }
    
    // discard temp hand (non-treasure cards)
    while (z - 1 >= 0){
        pre.discard[p][pre.discardCount[p]++] = tempHand[z-1];
        z = z - 1;
    }
    
    // discard Adventurer card
    // add Adventurer to played cards pile
    pre.playedCards[pre.playedCardCount] = pre.hand[p][handPos];
    pre.playedCardCount++;
    
    // set value to -1
    pre.hand[p][handPos] = -1;
    
    // if Adventurer was last card in hand
    // or if Adventurer was the only card in hand
    // reduce number of cards in hand
    if (handPos == (pre.handCount[p] - 1)){
        pre.handCount[p]--;
    }
    else if (pre.handCount[p] == 1){
        pre.handCount[p]--;
    }
    // otherwise, replace discarded card with last card in hand
    // set last card to -1, and reduce number of cards in hand
    else {
        pre.hand[p][handPos] = pre.hand[p][(pre.handCount[p] - 1)];
        pre.hand[p][pre.handCount[p] - 1] = -1;
        pre.handCount[p]--;
    }
    
    // compare cardEffect and manual implementation
    if (memcmp(&pre, post, sizeof(struct gameState)) != 0){
        printf("Error: Adventurer effect incorrect\n");
        totalFailed++;
    }
    
    return totalFailed;
}

// Adventurer effect: draw cards until 2 treasure; keep treasure, discard rest
int checkAdventurerCard2(int p, struct gameState *post, int handPos, int numTreasure){
    int totalFailed = 0;
    
    int r;
    
    struct gameState pre;
    memcpy (&pre, post, sizeof(struct gameState));
    
    // do cardEffect on POST gamestate
    r = cardEffect(adventurer, 0, 0, 0, post, handPos, 0);
    
    if (r != 0){
        printf("Error: Adventurer cardEffect() failed\n");
        totalFailed++;
    }
    
    if (post->handCount[p] != pre.handCount[p] + numTreasure){
        printf("Cards in hand: %d, expected: %d\n", post->handCount[p], pre.handCount[p] + numTreasure);
        totalFailed++;
    }
    
    return totalFailed;
}


int main () {
    int r, p;
    int maxHand = 5, maxDeck = 20, maxDiscard = 20;
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
    
    // replace hand, deck, and discard with all Curses
    memset(G.hand[p], 0, sizeof(int) * maxHand);
    memset(G.deck[p], 0, sizeof(int) * maxDeck);
    memset(G.discard[p], 0, sizeof(int) * maxDiscard);
    
    // change first card in hand to Adventurer
    printf("TESTING Adventurer card effect\n");
    
    printf("No treasure cards\n");
    G.hand[p][0] = adventurer;
    if (checkAdventurerCard2(p, &G, 0, 0) == 0){
        printf("All tests passed!\n");
    }
    
    memset(&G, 23, sizeof(struct gameState));           // clear the game state
    r = initializeGame(numPlayer, k, seed, &G);         // initialize a new game
    p = G.whoseTurn;
    
    G.handCount[p] = maxHand;
    G.deckCount[p] = maxDeck;
    G.discardCount[p] = maxDiscard;
    
    // replace hand, deck, and discard with all Curses
    memset(G.hand[p], 0, sizeof(int) * maxHand);
    memset(G.deck[p], 0, sizeof(int) * maxDeck);
    memset(G.discard[p], 0, sizeof(int) * maxDiscard);
    
    printf("One treasure card\n");
    G.hand[p][0] = adventurer;
    G.deck[p][0] = copper;
    if (checkAdventurerCard2(p, &G, 0, 1) == 0){
        printf("All tests passed!\n");
    }
    
    memset(&G, 23, sizeof(struct gameState));           // clear the game state
    r = initializeGame(numPlayer, k, seed, &G);         // initialize a new game
    p = G.whoseTurn;
    
    G.handCount[p] = maxHand;
    G.deckCount[p] = maxDeck;
    G.discardCount[p] = maxDiscard;
    
    // replace hand, deck, and discard with all Curses
    memset(G.hand[p], 0, sizeof(int) * maxHand);
    memset(G.deck[p], 0, sizeof(int) * maxDeck);
    memset(G.discard[p], 0, sizeof(int) * maxDiscard);
    
    printf("Two treasure cards\n");
    G.hand[p][0] = adventurer;
    G.deck[p][0] = copper;
    G.deck[p][1] = copper;
    if (checkAdventurerCard2(p, &G, 0, 2) == 0){
        printf("All tests passed!\n");
    }
    
    return 0;
}
