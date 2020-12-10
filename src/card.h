#ifndef CARD_H_
#define CARD_H_

#define NUM_CARDS 52
#define NUM_VALUE 13
#define NUM_SUITS 4
#define HAND_NUM_CARDS 5
#define ACE 14
#define KING 13
#define QUEEN 12
#define JACK 11


struct card{
	int value;
	int suite;
};
typedef struct card Card;

void initialize(Card deck[]);

void shuffle(Card deck[]);

void display(const Card deck[]);

void display_hand(const Card hand[]);

#endif
