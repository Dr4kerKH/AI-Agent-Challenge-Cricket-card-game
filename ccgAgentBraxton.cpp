// YOUR NAME: Sonpaorong Muchhim
//
// CS 4318, spring 2024
// Agent Challenge B: Cricket card game

//    nice bash ccgBuild.bash
//    nice ./ccgRunSim
#include "ccg.h"
namespace
{
   // If you need to define any new types or functions, put them here in
   // this unnamed namespace.  But no variables allowed!
}

int ccgAgentBraxton(Hand hand, Card lastBowledCard, bool isBatting, const MatchState &match)
{
	Card card;
	int bestCard, diffSuit, smallcard, suitFrequency, count, bestQuality = INT_MIN, whichCard;
	int quality[numCardsPerHand] = {0};
	int suitCount[5] = {0};
	int numCount[numCardsPerSuit] = {0};
	
	bestCard = diffSuit = smallcard = suitFrequency = count = 0;
	
	// Finding each suits count
	for (whichCard = 0; whichCard < numCardsPerHand; whichCard += 1) {
		card = hand.getCard(whichCard);
		suitCount[card.getSuit()] += 1;
		numCount[card.getNumber()-1] += 1;
	}
	// Getting the most suit and its highest count on hand
	for (int x = 0; x < 5; x++) {
		if (suitCount[x] > count) {
			count = suitCount[x];
			suitFrequency = x;
		}
	}
	// Add quality for card with the most same suit
	if (count > 2) {
		for (whichCard = 0; whichCard < numCardsPerHand; whichCard++) {
			card = hand.getCard(whichCard);
			if (card.getSuit() == suitFrequency)
				quality[whichCard] += 182;
		}
	}
	// Action: Batting
	if (isBatting) {
		
		int highNum = hand.getCard(0).getNumber();
		int lastCard = lastBowledCard.getNumber();
		int lastSuit = lastBowledCard.getSuit();
		// Search for highest number that match lastBowledCard suit
		for (int i = 0; i < numCardsPerHand; i++){
			card = hand.getCard(i);
			if (card.getSuit() == lastSuit && card.getNumber() > lastCard) {
				highNum = hand.getCard(i).getNumber();
				if (card.getNumber() > highNum)
					highNum = card.getNumber();
			}
		}
		// Condition 1: if the bestCard that match lastSuit still less than lastCard
		// Favour playing small card of the same suit, prevent using highCard and dont get run
		if (highNum <= lastCard) {
			for (whichCard = 0; whichCard < numCardsPerHand; whichCard += 1) {
				card = hand.getCard(whichCard);
				if (card.getSuit() == lastSuit)
					quality[whichCard] += (numCardsPerSuit - card.getNumber()) * 480;
			}	
		}
		
		// Adjusting quality upon favour criterions
		for (whichCard = 0; whichCard < numCardsPerHand; whichCard += 1) {
			
			card = hand.getCard(whichCard);
			int myCard = card.getNumber();
			int mySuit = card.getSuit();
			// Special Cases:
			quality[whichCard] += (lastCard - myCard) * 135;
			quality[whichCard] += numRuns(myCard - lastCard)* 45;
			
			// Criterion 1: Favour playing same suit
			if (mySuit == lastSuit)
				quality[whichCard] += 450;
			// Criterion 2: Favour playing same suit && bigger than lastBowledCard
			if (mySuit == lastSuit && myCard > lastCard)
				quality[whichCard] += 290;
			// Criterion 3: Add-on condition 2, if bigger than 3, ++Favour
			if (mySuit == lastSuit && myCard > lastCard + 3)
				quality[whichCard] += 320;
			// Criterion 4: Conter condition 1, --Favour
			if (mySuit != lastSuit) {
				quality[whichCard] -= 660;
				++diffSuit; // ++Count for not matching suit
			}
			// Criterion 5: Add-on condition 4, if suit not match && less than, --Favour
			if (mySuit != lastSuit && myCard < lastCard) {
				quality[whichCard] -= 660;
				smallcard++;
			}
		}
		// If nothing match the last suit, pick the best of the worst
		if (diffSuit == 6) {
			for (whichCard = 0; whichCard < numCardsPerHand; whichCard += 1) {
				card = hand.getCard(whichCard);
				int numDiff = card.getNumber() - lastBowledCard.getNumber();
				// ++Favour conditions
				if (numDiff == 0)
					quality[whichCard] += 760;
				if (numDiff >= 0 && numDiff < 2)
					quality[whichCard] += 820;
				if (numDiff < 0)
					quality[whichCard] -= 1250;
				if (numDiff > 3)
					quality[whichCard] -= 375;
			}
		}
		// If no matter whichCard == lose wicket
		// waste lowest card && suit == most suit
		if (smallcard == 6 && diffSuit == 6) {
			for (whichCard = 0; whichCard < numCardsPerHand; whichCard += 1) {
				card = hand.getCard(whichCard);
				quality[whichCard] = (numCardsPerSuit - card.getNumber()) * 230;
				if (card.getSuit() == suitFrequency)
					quality[whichCard] += 500;
			}
		}
	}
	// Action: Bowling
	else {
		// Search for most repeated card number > 3
		int mostNum;
		for (int i = 0; i < numCardsPerSuit; i++){
			if (numCount[i] > 3)
				mostNum = i + 1;
		}
		for (whichCard = 0; whichCard < numCardsPerHand; whichCard += 1) {
			
			card = hand.getCard(whichCard);
			int myCard = card.getNumber();
			int mySuit = card.getSuit();
			int lastCard = lastBowledCard.getNumber();
			int lastSuit = lastBowledCard.getSuit();
			
			// Conditions Prevention: qaulity++ for most number, preventing the 'wide' 'bye' conditions
			// when there are too much of the same card
			if (myCard == mostNum)
				quality[whichCard] += 175;
			if (mySuit == lastSuit && myCard == mostNum)
				quality[whichCard] += 250;
			if (myCard < lastCard && myCard == mostNum)
				quality[whichCard] += 350;
			
			// Base Criterion 1: The smaller the card, the better accordingly
			quality[whichCard] += (numCardsPerSuit - myCard) * 12;
			// Base Criterion 2: Same suit, the better accordingly
			if (mySuit == lastSuit)
				quality[whichCard] += (numCardsPerSuit - myCard) * 18;
			
			// Criterion 1: Favour playing smaller than last, ++Favour
			if (myCard < lastCard) 
				quality[whichCard] += 3;		//********************************************************************************
			// Criterion 2: Favour playing same suit, ++Favour
			if (mySuit == lastSuit)
				quality[whichCard] += 300;
			// Criterion 3: Punish the "Wide" condition, --Favour
			if (myCard == lastCard)
				quality[whichCard] -= 300;
			// Criterion 4: Punish the "Bye" condition, --Favour
			if (myCard == lastCard + 1)
				quality[whichCard] -= 500;
		}
	}
	// Picking the best quality card
	for (whichCard = 0; whichCard < numCardsPerHand; whichCard += 1) {
		if (quality[whichCard] > bestQuality) {
			bestCard = whichCard;
			bestQuality = quality[whichCard];
		}
	}
	return bestCard;
}

/*
--------------------------------------------------------------------------------
>> Approach Analysis:

- Most Frequent Suit: identifies and prioritizes the most suit than has more
than 2 cards on hand by adding quality points.
- Batting Logic: favour conditions playing cards have same suit as last bowled 
card, higher numbers, while also deduct quality point from unfavour conditions.
- No Matching Suit: special case when having no match suit, add quality points
according to card number different to last bowled card.
- Bowling Logic: prioritizes playing small card, same suits while also penalizes
certain criterions such as "Wide" condition and "Bye" condition.
- Quality Picking: after weighting all the criterions, runs through all the cards
quality point and pick the best card to play with.
--------------------------------------------------------------------------------
>> Strengths:
 
- Seperate bowling & batting: considers both batting and bowling scenarios separately,
adapting its strategy accordingly.
- Frequent Suit: it takes into account the frequency of suits and adjusts its 
decision-making based on the most frequent suit.
- Batting: playing highcard, prioritizes the minimize wicket lost.
- Bowling: playing lowcard, same suit, to bait out the same suit card, thus when the
big card from the same suit is boweled, gurentee wicket lost.
--------------------------------------------------------------------------------
>> Weakness:

- Complex Logic: due to complex and redundant conditions for qaulity points, some
game dynamic might be the soft spot for this agent or potentially suboptimal.
--------------------------------------------------------------------------------
>> Expectations:

- Fairly Good: likely to perform well when the game situation aligns with its 
strategic considerations, especially when it can take advantage of frequent suits
 and other specific card conditions.
- Adaptability: handling of special cases, such as when no card matches the last 
suit, adds a layer of adaptability.
- Performance: should fairly good for official matchs againt other agents, although
the expectation might be somewhere in middle.
--------------------------------------------------------------------------------
>> Testing Optimality:

- Multiple Agents Testing: serveral agents have been created to test with this agent,
and, performance-wise, this agent during great with some matchs win rate at 100%.
- Aticulate Quality Point: tweaking, adjusting the given quality point base on 
criterions by going through handreds run for all these quality point adjusted.
--------------------------------------------------------------------------------
*/



