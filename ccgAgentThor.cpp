#include "ccg.h"

namespace
{
   // If you need to define any new types or functions, put them here in
   // this unnamed namespace.  But no variables allowed!
}

int ccgAgentThor(Hand hand, Card lastBowledCard, bool isBatting, const MatchState &match)
{
	Card card;
	int bestCard, diffSuit, suitFrequency, count, bestQuality = INT_MIN, whichCard;
	int quality[numCardsPerHand] = {0};
	int suitCount[5] = {0};
	int numCount[numCardsPerSuit] = {0};
	
	bestCard = diffSuit = suitFrequency = count = 0;
	
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
				quality[whichCard] += 330;
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
					quality[whichCard] += (numCardsPerSuit - card.getNumber()) * 330;
			}	
		}
		
		// Adjusting quality upon favour criterions
		for (whichCard = 0; whichCard < numCardsPerHand; whichCard += 1) {
			
			card = hand.getCard(whichCard);
			int myCard = card.getNumber();
			int mySuit = card.getSuit();
			// Special Cases:
			quality[whichCard] += (lastCard - myCard) * 24;
			quality[whichCard] += numRuns(myCard - lastCard)* 35;
			
			// Criterion 1: Favour playing same suit
			if (mySuit == lastSuit)
				quality[whichCard] += 200;
			// Criterion 2: Favour playing same suit && bigger than lastBowledCard
			if (mySuit == lastSuit && myCard > lastCard)
				quality[whichCard] += 300;
			// Criterion 3: Add-on condition 2, if bigger than 3, ++Favour
			if (mySuit == lastSuit && myCard > lastCard + 3)
				quality[whichCard] += 400;
			// Criterion 4: Conter condition 1, --Favour
			if (mySuit != lastSuit) {
				quality[whichCard] -= 200;
				++diffSuit; // ++Count for not matching suit
			}
			// Criterion 5: Add-on condition 4, if suit not match && less than, --Favour
			if (mySuit != lastSuit && myCard < lastCard)
				quality[whichCard] -= 200;
		}
		// If nothing match the last suit, pick the best of the worst
		if (diffSuit == 6) {
			for (whichCard = 0; whichCard < numCardsPerHand; whichCard += 1) {
				
				card = hand.getCard(whichCard);
				int numDiff = card.getNumber() - lastBowledCard.getNumber();
				// ++Favour conditions
				if (numDiff > 0)
					quality[whichCard] += 300;
				if (numDiff >= 0 && numDiff < 4)
					quality[whichCard] += 400;
				if (numDiff < 0 && numDiff > -3)
					quality[whichCard] += 100;
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
				quality[whichCard] += 125;
			if (myCard < lastCard && myCard == mostNum)
				quality[whichCard] += 200;
			
			// Base Criterion 1: The smaller the card, the better accordingly
			quality[whichCard] += (numCardsPerSuit - myCard) * 20;
			// Base Criterion 2: Same suit, the better accordingly
			if (mySuit == lastSuit)
				quality[whichCard] += (numCardsPerSuit - myCard) * 10;
			
			// Criterion 1: Favour playing smaller than last, ++Favour
			if (myCard < lastCard) 
				quality[whichCard] += 400;
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

