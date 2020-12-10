#ifndef POKER_H_
#define POKER_H_

#define PLAYER_WIN 1
#define BANK_WIN -1
#define TIE 0

struct game_result{
	unsigned int game_bet;
	unsigned int game_win;
};
typedef struct game_result GameResult;

// poker-hand rank order
enum {HIGHCARD=0, PAIR=1, TWO_PAIRS=2, TRIS=3, STRAIGHT=4, FLUSH=5, FULLHOUSE=6, POKER=7, STRAIGHTFLUSH=8, ROYALFLUSH=9};

// pay-table .. this one is known as standard U.S. paytable for Caribbean Stud Poker
enum Paytable {PAY_HIGHCARD=1, PAY_PAIR=1, PAY_TWO_PAIRS=2, PAY_TRIS=3, PAY_STRAIGHT=4, PAY_FLUSH=5, 
		PAY_FULLHOUSE=7, PAY_POKER=20, PAY_STRAIGHTFLUSH=50, PAY_ROYALFLUSH=100};
static const enum Paytable PAYTABLE[] = {PAY_HIGHCARD, PAY_PAIR, PAY_TWO_PAIRS, PAY_TRIS, PAY_STRAIGHT, PAY_FLUSH, 
		PAY_FULLHOUSE, PAY_POKER, PAY_STRAIGHTFLUSH, PAY_ROYALFLUSH};

int rank_hand(Card hand[], int values[]);

bool check_qualify(int rank, int values[]);

char resolve_helper(int p, int b);

char straight_tie_resolve(int pvals[], int bvals[]);

char resolve_tie(int rank, int pvals[], int bvals[]);

unsigned int get_win(unsigned int bet, int player_rank, int bank_rank, int player_values[], int bank_values[]);

bool decide_if_raise(int player_rank, int player_values[], int bank_visible_card);

GameResult play(Card deck[], unsigned int bet);

#endif
