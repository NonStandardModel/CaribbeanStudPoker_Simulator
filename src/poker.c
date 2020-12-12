#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "helpers.h"
#include "card.h"
#include "poker.h"

int rank_hand(Card hand[], int values[]) {
	/*NOTE: values must be already sorted array*/
	// we need number of distinct values in hand and number of distinct suits in hand
	// we start with 1 because we have 1 distinct at the moment we have 1 card
	// than we  just cont number of "changes" in array . .note that array is sorted
	int distinct_vals = 1;
	int distinct_suit = 1;
	int suites[5];
	for (int i = 0; i < HAND_NUM_CARDS-1; i++) {
		if (values[i+1] != values[i]) {distinct_vals++;}
		suites[i] = hand[i].suite; // suites for cards 1-4 are copied into array here .. the 5th card is done after
	}
	suites[4] = hand[4].suite; // the missing 5th card suite from above
	sort(suites, 5);
	for (int i = 0; i < HAND_NUM_CARDS-1; i++) {
		if (suites[i+1] != suites[i]) {distinct_suit++;}
	}
/*	display_hand(hand);*/
/*	printf("Distinct values: %d\n", distinct_vals);*/
/*	printf("Distinct suites: %d\n", distinct_suit);*/
	//fflush(stdin);
	// check if we have STRAIGHT . .note that ACE can be 1 or 14
	bool straight, straight_ACE;
	int tmp_vals[5];
	if (distinct_vals == 5) {
		straight = true;
		int tmp = values[0];
		for (int i = 1; i < 5; i++) {
			tmp++;
			if (values[i] != tmp) {straight = false;break;}
		}
		for (int i = 0; i < 5; i++) {
			if (values[i] != ACE) {
				tmp_vals[i] = values[i];
			} else {tmp_vals[i] = 1;}
		}
		sort(tmp_vals, 5);
		straight_ACE = true;
		tmp = tmp_vals[0];
		for (int i = 1; i < 5; i++) {
			tmp++;
			if (tmp_vals[i] != tmp) {straight_ACE = false;break;}
		}
		if (straight || straight_ACE) {straight = true;}
	} else {straight = false;}
	
	// check if we have FLUSH
	bool flush = false;
	if (distinct_suit == 1) {flush = true;}
	
	/*Note that if we have either STRAIGHT or FLUSH we can ignore POKER, TRIS, PAIRS
	  because it is impossible to have 2 or more cards with the same value and still have
	  either FLUSH or STRAIGHT. So we can just determine the exact rank and exit.*/
	if (flush) {
		if (straight) {
			// if straight starts with 10 we have ROYALFLUSH
			if (values[0] == 10) {/*printf("ROYALFLUSH\n");*/return ROYALFLUSH;}
			else {/*printf("STRAIGHTFLUSH\n");*/return STRAIGHTFLUSH;}
		} else {/*printf("FLUSH\n");*/return FLUSH;}
	}
	// we have to check for clear STRAIGHT . .other STRAIGHT variants would already return above
	if (straight) {/*printf("STRAIGHT\n");*/return STRAIGHT;}
	
	// we know that's no STRAIGHT or FLUSH, so 5 distinct values is HIGHCARD
	if (distinct_vals == 5) {/*printf("HIGHCARD\n");*/return HIGHCARD;}
	// with 4 values we must have a PAIR
	if (distinct_vals == 4) {/*printf("PAIR\n");*/return PAIR;}
	// with 3 we could have TRIS or TWO_PAIRS
	if (distinct_vals == 3) {
		// if any value repeats 3 times we know it is TRIS .. otherwise it must be TWO_PAIRS
		// note that for repeat 3 times it is enaugh to do count on first 3 elements !!!
		for (int i = 0;i < 3; i++) {
			if (count(values, values[i], 5) == 3) {/*printf("TRIS\n");*/return TRIS;}
		}
		/*printf("TWO_PAIRS\n")*/;return TWO_PAIRS;
	}
	// with 2 distinct values we can have POKER or FULLHOUSE
	if (distinct_vals == 2) {
		// if any value repeats 4 time we know it is POKER .. otherwise FULLHOUSE
		// note that for repeat 4 times it is enaugh to do count on first 2 elements !!!
		for (int i = 0;i < 2; i++) {
			if (count(values, values[i], 5) == 4) {/*printf("POKER\n");*/return POKER;}
		}
		/*printf("FULLHOUSE\n");*/return FULLHOUSE;
	}
	// if we did not return until here it is invalid . .return -1
	printf("<ERROR> Should not have been here . .end rank_hand!\n");
	printf("Distinct values: %d\n", distinct_vals);
	printf("Distinct suites: %d\n", distinct_suit);
	display_hand(hand);
	fflush(stdin);
	return -1;
}

bool check_qualify(int rank, int values[]) {
	// it is assumed that values are sorted min->max
	if (rank > HIGHCARD) {return true;}
	if (values[4] < ACE || values[3] < KING) {return false;}
	return true;
}

char resolve_helper(int p, int b) {
	if (p > b) {return PLAYER_WIN;}
	if (p < b) {return BANK_WIN;}
	return TIE;
}

char straight_tie_resolve(int pvals[], int bvals[]) {
	// moved to separate function the TIE resolve case when both have STRAIGHT/STRAIGHTFLUSH
	// note that we must not check only lowest or highest value due to ACE possibility to be 14 or 1
	bool player_ACE_straight = (count(pvals, 2, 5) == 1 && count(pvals, ACE, 5) == 1);
	bool bank_ACE_straight = (count(bvals, 2, 5) == 1 && count(bvals, ACE, 5) == 1);
	// if both have "dirty" straight it must be TIE
	if (player_ACE_straight && bank_ACE_straight) {return TIE;}
	// player has dirty straight, bank has clear .. BANK_WIN
	if (player_ACE_straight && !bank_ACE_straight) {return BANK_WIN;}
	// player has clear, bank has dirty straight .. PLAYER_WIN
	if (!player_ACE_straight && bank_ACE_straight) {return PLAYER_WIN;}
	// both have clear straight, so we just check lowest card and return
	return resolve_helper(pvals[0], bvals[0]);
}

char resolve_tie(int rank, int pvals[], int bvals[]) {
	// pvals & bvals must be sorted arrays min->max
	// make checks so that they go from most probable to less probable --> mostly done
	
	// HIGHCARD .. wins higher .. covered together with FLUSH case bellow
	// XXX does it have sense to move it here .. maybe PAIR is more probable
	// because in HIGHCARD case it is rare that bank qualifies ??? XXX
	// PAIR .. wins higher pair, if pair is equal we go through single cards
	if (rank == PAIR) {
		// TODO maybe find better way to do this
		// find pair value and store single cards in another array
		int ppair, bpair, psingles[3], bsingles[3], pidx = 0, bidx = 0;
		for (int i = 0; i < 5; i++) {
			if (count(pvals, pvals[i], 5) == 2) {ppair = pvals[i];}
			if (count(bvals, bvals[i], 5) == 2) {bpair = bvals[i];}
		}
		for (int i = 0; i < 5; i++) {
			if (pvals[i] != ppair) {psingles[pidx] = pvals[i];pidx++;}
			if (bvals[i] != bpair) {bsingles[bidx] = bvals[i];bidx++;}
		}
		// note that singles arrays are sorted by design (since vals was sorted)
		char result = resolve_helper(ppair, bpair);
		if (result != TIE) {return result;}
		for (int i = 0; i < 3; i++) {
			result = resolve_helper(psingles[2-i], bsingles[2-i]);
			if (result != TIE) {return result;}
		}
		return result;
	}
	// TWO_PAIRS .. wins higher..order of importance/checks: high pair, low pair, single card
	// first we search for pairs values and sort it for later easy use
	if (rank == TWO_PAIRS) {
		int ppairs[2] = {0, 0}, bpairs[2] = {0, 0};
		int psingle_card, bsingle_card;
		for (int i = 0; i < 5; i++) {
			if (count(pvals, pvals[i], 5) == 2) {
				if (count(ppairs, pvals[i], 2) == 0) {
					if (ppairs[0] == 0) {ppairs[0] = pvals[i];}
					else {ppairs[1] = pvals[i];}
				} else {psingle_card = pvals[i];}
			}
			if (count(bvals, bvals[i], 5) == 2) {
				if (count(bpairs, bvals[i], 2) == 0) {
					if (bpairs[0] == 0) {bpairs[0] = bvals[i];}
					else {bpairs[1] = bvals[i];}
				} else {bsingle_card = bvals[i];}
			}
		}
		sort(ppairs, 2);
		sort(bpairs, 2);
		// first copare pairs and than single if needed .. note that higher pair is idx 1 so we start with 1
		for (int i = 0; i < 2; i++) {
			char result = resolve_helper(ppairs[1-i], bpairs[1-i]);
			if (result != TIE) {return result;}
		}
		return resolve_helper(psingle_card, bsingle_card);
	}
	// FLUSH or HIGHCARD.. higher card wins, we need to go through all cards if they are equal
	if (rank == FLUSH || rank == HIGHCARD) {
		char result;
		// TODO . .can do only until 4th card and than return 5th card compare .. saves 1 if statement compare
		// Note that array is min->max, so we must start with last values and go backwards
		for (int i = 0; i < 5; i++) {
			result = resolve_helper(pvals[4-i], bvals[4-i]);
			if (result != TIE) {return result;}
		}
		return result;
	}
	// FULLHOUSE or TRIS .. wins higher TRIS .. note that TIE is impossible
	if (rank == FULLHOUSE || rank == TRIS) {
		// need to find tris value, so search for card that repeats 3x in hand
		// we need to check 1st and 2nd card, if neither is 3x repeated it must be the 3rd
		int ptris, btris;
		for (int i = 0; i < 3; i++) {
			// we just do 3 checks for both in the same loop, in average it is ~same as 2 loops with break
			if (count(pvals, pvals[i], HAND_NUM_CARDS) == 3) {ptris = pvals[i];}
			if (count(bvals, bvals[i], HAND_NUM_CARDS) == 3) {btris = bvals[i];}
		}
		if (ptris > btris) {return PLAYER_WIN;}
		else {return BANK_WIN;}
	}
	// POKER .. wins higher .. note that TIE is impossible
	if (rank == POKER) {
		// we need to find poker value, so search which cards repeats 4x in hand
		// check if 1st card, if not than it must be 2nd (since poker is 4 repeated cards and hand has 5 cards)
		int ppoker, bpoker;
		if (count(pvals, pvals[0], HAND_NUM_CARDS) == 4) {ppoker = pvals[0];}
		else {ppoker = pvals[1];}
		if (count(bvals, bvals[0], HAND_NUM_CARDS) == 4) {bpoker = bvals[0];}
		else {bpoker = bvals[1];}
		if (ppoker > bpoker) {return PLAYER_WIN;}
		else {return BANK_WIN;}
	}
	// STRAIGHT or STRAIGHTFLUSH .. wins higher cards
	// But ACE can be 1 or 14, so it was oved to helper function
	if (rank == STRAIGHT || rank == STRAIGHTFLUSH) {return straight_tie_resolve(pvals, bvals);}
	// if its ROYALFLUSH it is TIE (can't resolve ROYALFLUSH)
	if (rank == ROYALFLUSH) {return TIE;}
	
	printf("<ERROR> Should not have been here!\n");
	fflush(stdout);
}

unsigned int get_win(unsigned int bet, int player_rank, int bank_rank, int player_values[], int bank_values[]) {
	char result;
	if (check_qualify(bank_rank, bank_values)) {
		// BANK has qualified
		// first we determine game result .. PLAYER_WIN, TIE, BANK_WIN
		if (player_rank > bank_rank) {result = PLAYER_WIN;}
		if (player_rank < bank_rank) {result = BANK_WIN;}
		// we can use player_rank as rank in resolve_tie() function parameter as both player and bank rank are equal
		if (player_rank == bank_rank) {result = resolve_tie(player_rank, player_values, bank_values);}
		
		// based on result we now determine the win ammount
		if (result == PLAYER_WIN) {
			// return 2xANTE + RAISE (note that RAISE=2*ANTE) multiplied by paytable rank value
/*			printf("Player win.\n\n");*/
			unsigned int win = 2 * bet + (2 * bet) * (1 + PAYTABLE[player_rank]);
			return win;
		}
		if (result == TIE) {
			// in TIE we return all bets, ANTE + RAISE .. so we just return 3*bet
/*			printf("Tie.\n\n");*/
			return 3 * bet;
		}
		// if we're here it was BANK_WIN, so we return 0
/*		printf("Bank win.\n\n");*/
		return 0;
	} else {
		// BANK has NOT qualified..ANTE pays 1:1 and RAISE is returned.. so we return 4*bet
/*		printf("Bank not qualified.\n\n");*/
		return 4 * bet;
	}
	printf("<ERROR> Should not have been here .. get_win end!");
}

bool decide_if_raise(int player_rank, int player_values[], int bank_visible_card) {
	/*
	Few strategies are implemented already bellow. Just un-comment the desired one.
	Optimal strategy is still to be done.
	*/
	//=================================================================================
	// BLIND PLAY STRATEGY . .raise in any case
/*	return true;*/

	//=================================================================================
	// RAISE ON ACE-KING OR BETTER SRATEGY
/*	if (player_rank > HIGHCARD) {return true;}*/
/*	else if (player_values[4] == ACE && player_values[3] == KING) {return true;}*/
/*	else {return false;}*/
	
	// RAISE ON ANY PAIR OR BETTER STRATEGY
	if (player_rank > HIGHCARD) {return true;}
	return false;
	
	//=================================================================================
	// WIZARD STRATEGY [see https://wizardofodds.com/games/caribbean-stud-poker/]
/*	if (player_rank > HIGHCARD) {return true;}*/
/*	else if (player_values[4] < ACE || player_values[3] < KING) {return false;}*/
/*	//elif (bank_visible_card in [2,3,4,5,6,7,8,9,10,11,12] and bank_visible_card in player_values): -> return true*/
/*	else if (bank_visible_card < KING) {*/
/*		for (int j = 0; j < 5; j++) {*/
/*			if (bank_visible_card == player_values[j]) {return true;}*/
/*		}*/
/*		return false;*/
/*	}*/
/*	//elif (bank_visible_card in [13, 14] and (11 in player_values or 12 in player_values)): --> return true*/
/*	else if (bank_visible_card == KING || bank_visible_card == ACE) {*/
/*		for (int j = 0; j < 5; j++) {*/
/*			if (player_values[j] == JACK || player_values[j] == QUEEN) {return true;}*/
/*		}*/
/*		return false;*/
/*	}*/
/*	//elif (bank_visible_card not in player_values and 12 in player_values and bank_visible_card < player_values[1]): --> return true*/
/*	else if (bank_visible_card < player_values[1]) {*/
/*		bool queen_in_player_hand = false;*/
/*		bool bank_visible_not_in_player_hand = true;*/
/*		for (int j = 0; j < 5; j++) {*/
/*			if (player_values[j] == QUEEN) {queen_in_player_hand = true;}*/
/*			if (bank_visible_card == player_values[j]) {bank_visible_not_in_player_hand = false;}*/
/*		}*/
/*		if (bank_visible_not_in_player_hand && queen_in_player_hand) {return true;}*/
/*		else {return false;}*/
/*		*/
/*	}*/
/*	else {return false;}*/

	//=================================================================================
	// TODO perfect strategy as presented [https://wizardofodds.com/games/caribbean-stud-poker/optimal-strategy/]
	
}

GameResult play(Card deck[], unsigned int bet) {
	/* deck is shuffled in main() */
	GameResult _res = {0,0};
	Card player_hand[5] = {0, 0};
	Card bank_hand[5] = {0, 0};
	int player_values[5], bank_values[5];
	int bank_visible_card;
	int player_rank, bank_rank;
	// first 5 cards are players, next 5 are bank (dealer)
	for (int i = 0 ; i < 5; i++) {
		player_hand[i] = deck[i];
		player_values[i] = deck[i].value;
		bank_hand[i] = deck[i+5];
		bank_values[i] = deck[i+5].value;
	}
	// get bank visible card before sorting !!
	bank_visible_card = bank_values[0];
/*	printf("PLAYER\n");*/
	sort(player_values, 5);
	player_rank = rank_hand(player_hand, player_values);
	/*
	TODO --> Done!
	Bank hand sort and ranking should be moved to after RAISE. Like this we avoid
	calling un-needed things in cases where the player folds. It is left here for
	now for debugging purposes.
	*/
/*	printf("\nBANK\n");*/
/*	sort(bank_values, 5);*/
/*	bank_rank = rank_hand(bank_hand, bank_values);*/
/*	printf("\n");*/
	// place ANTE bet
	_res.game_bet = bet;
	// implement desired strategy in decide_if_raise() function above
	if (decide_if_raise(player_rank, player_values, bank_visible_card)) {
		// bank hand sort and rank done here because we do not need them in case player folds
		sort(bank_values, 5);
		bank_rank = rank_hand(bank_hand, bank_values);
		// raise .. RAISE = 2 x ANTE
		_res.game_bet += 2*bet;
		_res.game_win = get_win(bet, player_rank, bank_rank, player_values, bank_values);
	}
/*	printf("Game bet: %d, game win: %d\n", _res.game_bet, _res.game_win);*/
	return _res;
}
















