#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "helpers.h"
#include "card.h"
#include "poker.h"

int main(int argc, char **argv) {
	if (argc != 2) { /* Need one argument */ printf("Usage: %s NUM_SIMS\n", argv[0]); return 1;}
	unsigned int SIMS = atoi(argv[1]);
	unsigned int TOT_BET = 0, TOT_WIN = 0, BET = 1;
	Card deck[NUM_CARDS] = {0, 0};
	initialize(deck);
	// seed RNG here instead of in each shuffle function call
	srand(time(NULL));

	GameResult res;
	for (unsigned int sim = 0; sim < SIMS; sim++) {
		shuffle(deck);
/*		shuffle(deck);*/
/*		display(deck);*/
		res = play(deck, BET);
		TOT_BET += res.game_bet;
		TOT_WIN += res.game_win;
	}
	double RTP = (double)TOT_WIN / (double)TOT_BET;
	printf("RTP --> %f\n", 100*RTP);
	
	return 0;
}











