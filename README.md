# CARIBBEAN STUD POKER RTP SIMULATOR

This is simulator of Caribbean Stud Poker to obtain RTP (Return To Player) value.
Note that this may be still work in progress (or maybe I already lost interest).

**TODO:** 
1) There is a lot of room for improvement in terms of speed (note that to get somewhat stable RTP we may need 100s of millions of simulations). First is the sort() algorithm. I believe that we only need to sort arrays of length 5 and 2 for now, so custom implementation would benefit a lot. Then the count() number of occurrences of some value in array. Same reasoning as before. If we always count on the same array length we could be faster with custom implementation. Next is deck shuffling for each game. We only need 10 random unique cards for each game. Maybe 10 random unique could be faster than shuffling the entire 52 card deck.
EDIT: some of the above has been done, see **Speed Notes** at the end

### Why & How
As part of my work I have to independently check various things. Recently I had to check the RTP for this poker variant. The odds for this game can be found on various web-sites. I was using WizardOfOdds (WoO from now on) as reference as it seems to be reasonably "*trusted*" in this industry [https://wizardofodds.com/games/caribbean-stud-poker/]. The 1st simulator was done (due to complicated reasons) mainly in my free time, than this "*port to C*" was done fully in my time, so I decided to share the code. Maybe someone will have to do something similar in the future.
Once I made my simulator (1st version was in python2) I was getting RTP value that was higher than what I expected. This made me lost 2 days of searching for mistake in my code. In the end I have discovered that I was misinterpreting the  info on WoO. The devil is in the details. 
I was expecting "*House Edge*" value (House Edge is just 100% - RTP) which was incorrect. WoO web-site lists the "*House Edge*" of 5.224% (following perfect strategy) and "*Element of Risk*" of 2.555%.
Note that the House Edge is calculated as expected player win vs **INITIAL** bet ratio. But in this game the player has to  decide to raise or fold. If (s)he raises, the bet consequently increases. RTP is defined as total player wins vs total  player bets ratio over all possible game outcomes. So RTP is what the WoO defines as "*Element of Risk*". [see https://wizardofodds.com/gambling/glossary/]

Note that the RTP depends on player strategy of when (s)he decides to raise or to fold.
In this game 1 card deck of 52 cards is used. Player and bank each get 5 cards. Player sees all of his cards, but only 1 bank card, other 4 bank cards are covered. Based on this info the player has to decide if to raise (by placing RAISE bet = 2x ANTE bet) or to fold.
This is where the strategy is hidden. Player has only limited info about game and has to calculate what gives him the best probabilities of winning.

### The Rules
- Player makes an ante bet.
- The player and the bank (=dealer) get five cards each. Player sees all his cards, but only 1 bank card, other 4 bank cards are face-down.
- Player must fold or raise.
- If player folds (s)he looses the ante bet.
- If player raises then he must make a raise bet (equal to twice the ante bet ammount).
- The bank other 4 cards will be shown at this point.
- The bank must have an ACE and a KING or higher to qualify. If the bank does not qualify the player will win 1:1 on his ante bet and the raise bet will be returned. Standard poker ranks apply here (HIGHCARD, PAIR, TWO_PAIRS, TRIS etc).
- If the dealer qualifies and beats the player, both ante and raise bets will be lost.
- If the dealer qualifies and loses to the player, then the ante will pay 1:1 and the raise will pay according to the paytable.
- If the player and dealer tie, both ante and raise bets will be returned.

Note that the Caribbean Stud Poker usually has also JackPot side bet or something similar. This side bet is not implemented here as it is considered "*sucker*" bet, since it will decrease overall RTP value.

This program uses the Monte-Carlo method to obtain RTP. Basically we just mix card deck using RNG and then play the game. The strategy when to raise is implemented in `decide_if_raise()` function in `src/poker.c`. If you need to test some other strategy it should be simple enough to implement it here.
The paytable is defined in `src/poker.h` and is using what is known as standard U.S. Paytable.
If any additional statistics are needed this are easily implemented in `play()` function in `src/poker.c`. For example statistics of different hand ranks (% of PAIR, TWO_PAIRS, TRIS etc.), expected StandardDeviation of RTP etc. In such case it may be the easiest just to expand `game_result` struct (defined in `src/poker.h`) to include the desired info. Like this the additional info in `game_result` will be  accessible in `main()`.

There are some commented "debug" printf() left in the code. Note that I am not a good C programmer and this SW was made in few days. This is not a good-practice example of how SW is to be made. The 1st python version of this simulator was done in less than 2 days and it was made with the assumption that the function inputs are correct (because speed is important and checks would slow the code). 

### Compilation (Linux)
Just clone this repository, move to src/ directory and run make command in terminal.
I have not tested this on Windows OS, but it should not be a problem to compile, since I am using only basic includes.

### Running
Run the compiled binary with number of simulations as 1st argument. For example to run 10 million simulations:
`./StudPokerSim 10000000`

### Speed Notes
On ThinkPad T490s laptop I get an average of >1 million games per second.
This is after the few optimizations that have been made:
- better checking for straight with ACE as the lowest card
- made sort5, sort2, count5, count2 functions to be used on arrays of length 5 and 2 (instead of generic sort and count)

Before the above mentioned changes I was getting average ~105s for 100 million simulations using blind strategy, after the changes it has dropped to ~90s. The biggest single contributor to this has been the straight check optimization.


