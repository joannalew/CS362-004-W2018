/* -----------------------------------------------------------------------
* Joanna Lew
* Assignment 3
* Testing Council Room card effect from dominion.c
* Include the following lines in your makefile:
*
* cardtest3: cardtest3.c dominion.o rngs.o
*      gcc -o cardtest3 -g  cardtest3.c dominion.o rngs.o $(CFLAGS)
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


// Council Room effect: +4 cards, +1 buy, each other player +1 card
int checkCouncilRoomCard(int p, struct gameState *post, int handPos){
    int totalFailed = 0;
    
    int i, r;
    struct gameState pre;
    memcpy (&pre, post, sizeof(struct gameState));
    
    // do cardEffect on POST gamestate
    r = cardEffect(council_room, 0, 0, 0, post, handPos, 0);
    
    if (r != 0){
        printf("Error: Council Room cardEffect() failed\n");
        totalFailed++;
    }
    
    // do "manual" Council Room effect on PRE gamestate
    
    // draw four cards
    for (i = 0; i < 4; i++){
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
    }
    
    // +1 buy
    pre.numBuys++;
    
    // each other player draws a card
    for (i = 0; i < pre.numPlayers; i++){
        if (i != p){
            // if there's a card in deck
            // put top card of deck in last position of hand
            if (pre.deckCount[i] > 0) {
                pre.handCount[i]++;
                pre.hand[i][pre.handCount[i]-1] = pre.deck[i][pre.deckCount[i]-1];
                pre.deckCount[i]--;
            }
            // if there's no card in deck, put the discard pile as deck
            // put top card of deck in last position of hand
            else if (pre.discardCount[i] > 0) {
                memcpy(pre.deck[i], post->deck[i], sizeof(int) * pre.discardCount[i]);
                memcpy(pre.discard[i], post->discard[i], sizeof(int)*pre.discardCount[i]);
                pre.hand[i][post->handCount[i]-1] = post->hand[i][post->handCount[i]-1];
                pre.handCount[i]++;
                pre.deckCount[i] = pre.discardCount[i]-1;
                pre.discardCount[i] = 0;
            }
        }
    }
    
    // discard Council Room card
    // add Council Room to played cards pile
    pre.playedCards[pre.playedCardCount] = pre.hand[p][handPos];
    pre.playedCardCount++;
    
    // set value to -1
    pre.hand[p][handPos] = -1;
    
    // if Council Room was last card in hand
    // or if Council Room was the only card in hand
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
        printf("Error: Council Room effect incorrect\n");
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
    
    // change first card in hand to Council Room
    printf("TESTING Council Room card effect\n");
    G.hand[p][0] = council_room;
    
    if (checkCouncilRoomCard(p, &G, 0) == 0){
        printf("All tests passed!\n");
    }
    
    return 0;
}
