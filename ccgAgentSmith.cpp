// YOUR NAME: ___
//
// CS 4318, spring 2024
// Agent Challenge B: Cricket card game
//
// Rename this file and the function below.  For example, if your agent name
// is Jones, rename this ccgAgentSmith.cpp file to ccgAgentJones.cpp and the
// ccgAgentSmith function below to ccgAgentJones.  Complete your agent
// function and turn it in on Blackboard.  Random-number generation is not
// allowed; your agent must be entirely deterministic.  Feel free to create
// other agents--each in a separate .cpp file--for yours to play against,
// but turn in only one.  Test your agent(s) with
//
//    nice bash ccgBuild.bash
//
// and then
//
//    nice ./ccgRunSim
//
// Each submitted agent will play each other, batting first and batting
// second an equal number of times, to determine the standings, which will
// be posted soon after the agents are due.

#include "ccg.h"

namespace
{
   // If you need to define any new types or functions, put them here in
   // this unnamed namespace.  But no variables allowed!
}

// Rename and complete this agent function.
int ccgAgentSmith(Hand hand, Card lastBowledCard, bool isBatting, const MatchState &match)
{
   // Your function must end up returning a valid int between 0 and numCardsPerHand - 1.
   // No random-number generation allowed!
   // hand.getCard(0) gives the first card in your hand.
   // hand.getCard(0).getNumber() gives the number of that card, 1 to 11.
   // hand.getCard(0).getSuit() gives the suit of that card.
   // match.getRuns(0) gives the number of runs scored by player A so far.
   // numRuns(d) gives the number of runs scored when the card difference is d.
   // See the definitions of the Hand, Card and MatchState classes for more helpful functions.

   if (isBatting)
   {
      // Replace this return statement with your agent code for when batting.
      return 0;
   }
   else // is bowling
   {
      // Replace this return statement with your agent code for when bowling.
      return 0;
   }
}

/*

 - First, carefully comment your code above to clarify how it works.
 - Here, describe your approach and analyze it.  How exactly did you develop
   and test it?  What are its strengths and weaknesses?  Why do you expect
   it to do well against the other submitted agents?
 - Also make a note here if you talked about the assignment with anyone or
   gave or received any kind of help.

*/
