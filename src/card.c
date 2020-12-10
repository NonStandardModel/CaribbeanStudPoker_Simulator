#include <stdio.h>
#include <stdlib.h>
//#include <time.h> // since seeding (srand) is done in main we do not need time.h here

#include "card.h"

void initialize(Card deck[]) {
	char idx = 0;
	for (char val = 2; val < NUM_VALUE+2; val++) {
		for (char sui = 1; sui < NUM_SUITS+1; sui++) {
			idx = NUM_SUITS*(val - 2) + sui - 1;
			deck[idx].value = val;
			deck[idx].suite = sui;
		}
	}
}

// TODO .. there is <0.5% error from expected RTP to what this simulator returns
// make sure that the shuffling is OK !! Maybe run statistical tests. 
// On the other hand .. all we need is to select 10 random unique cards from deck.
// Maybe unique random selection could be faster than shuffling entire deck ???
void shuffle(Card deck[]) {
	int swp = 0; //index of card to be swapped
	Card temp = {0, 0}; //temp holding place for swap
	//srand(time(NULL)); // seeding done in main to avoid re-seeding at each shuffle
	for (int i = 0; i < NUM_CARDS; i++){
		swp = rand() % NUM_CARDS; 
		temp = deck[i];
		deck[i] = deck[swp];
		deck[swp] = temp;
	}
}

void display(const Card deck[]) {
	for (int i = 0; i < NUM_CARDS; i++) {	
		if (i % 4 == 0) {printf("\n");}
		printf("%d-%d ", deck[i].value, deck[i].suite);
	}
	printf("\n");
}

void display_hand(const Card hand[]) {
	char sui;
	for (int i = 0; i < 5; i++) {
		switch (hand[i].suite) {
			case 1: sui = 'D';break;
			case 2: sui = 'S';break;
			case 3: sui = 'C';break;
			case 4: sui = 'H';break;
			default: sui = 'x';
		}
		printf("%d%c ", hand[i].value, sui);
	}
	printf("\n");
	fflush(stdout);
}



