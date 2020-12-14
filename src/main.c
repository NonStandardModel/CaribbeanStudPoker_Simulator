#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "helpers.h"
#include "card.h"
#include "poker.h"

int main(int argc, char **argv) {
	if (argc != 2) { /* Need one argument */ printf("Usage: %s NUM_SIMS\n", argv[0]); return 1;}
	
	// Get the desired strategy from user and pass it as arg to play() which will pass it to decide_if_raise().
	// To add custom strategy you need to add the code in switch-case in decide_if_raise() function in poker.c .
	char strat[4];
	printf("Select the strategy. (default is 1 - Blind)\n");
	printf("\t1 - Blind play strategy (raise in any case). Expected RTP = 94.464%%\n");
	printf("\t2 - Raise on ACE-KING or better strategy. Expected RTP = 97.328%%\n");
	printf("\t3 - Raise on any PAIR or better strategy. Expected RTP = 97.262%%\n");
	printf("\t4 - Wizards strategy [see https://wizardofodds.com/games/caribbean-stud-poker/]. Expected RTP = 97.446%%\n");
	fgets(strat, sizeof(strat), stdin);
	
	unsigned int SIMS = atoi(argv[1]);
	unsigned int TOT_BET = 0, TOT_WIN = 0, BET = 1;
	Card deck[NUM_CARDS] = {0, 0};
	initialize(deck);
	// seed RNG here instead of in each shuffle function call
	srand(time(NULL));

	GameResult res;
	for (unsigned int sim = 0; sim < SIMS; sim++) {
		shuffle(deck);
/*		display(deck);*/
		res = play(deck, BET, atoi(strat));
		TOT_BET += res.game_bet;
		TOT_WIN += res.game_win;
	}
	double RTP = (double)TOT_WIN / (double)TOT_BET;
	printf("RTP --> %f\n", 100*RTP);
	
	return 0;
}











