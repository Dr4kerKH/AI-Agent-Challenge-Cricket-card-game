#include "ccg.h"
//    nice bash ccgBuild.bash

//    nice ./ccgRunSim
int ccgAgentEXE(Hand hand, Card lastBowledCard, bool isBatting, const MatchState &match)
{
	Card card;
	int bestCard, bestQuality, quality, whichCard;

	if (isBatting)
	{
		// When batting, play the card with the highest quality according to weighted criteria.
		bestQuality = INT_MIN;
		bestCard = 0;
		for (whichCard = 0; whichCard < numCardsPerHand; whichCard += 1)
		{
			card = hand.getCard(whichCard);
			quality = 0;
			// Criterion 1: Favour playing a card that doesn't lose a wicket.
			if (card.getSuit() == lastBowledCard.getSuit())
			{
				quality += 100;
			}
			if (card.getNumber() >= lastBowledCard.getNumber())
			{
				quality += 100; 
			}
			// Create your own batting criteria!
			// Have we found a better card to play?
			if (quality > bestQuality)
			{
				bestQuality = quality;
				bestCard = whichCard;
			}
		}
		return bestCard;
	}
	else // is bowling
	{
		// When bowling, play the card with the highest quality according to weighted criteria.
		bestQuality = INT_MIN;
		bestCard = 0;
			for (whichCard = 0; whichCard < numCardsPerHand; whichCard += 1) {
				card = hand.getCard(whichCard);
				quality = 0;
				// Criterion 1: Favour playing a card with a smaller number than the last bowled card.
				if (card.getNumber() < lastBowledCard.getNumber() )
				{
					quality += 100;
				}
				if (card.getSuit() == lastBowledCard.getSuit() 
					&& card.getNumber() > lastBowledCard.getNumber() + 1) {
					quality += 100;
				}
				if (card.getNumber() > 8)
				{
					quality += 50;
				}
				// Create your own bowling criteria!
				// Have we found a better card to play?
				if (quality > bestQuality)
				{
					bestQuality = quality;
					bestCard = whichCard;
				}
			}
		return bestCard;
	}
}