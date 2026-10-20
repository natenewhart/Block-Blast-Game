#include "Score.h"
#include "Config.h"
#include <print>

Score::Score()
	: mScore(0.f)
	, mScoreMultiplier(1.f) 
	, mScoreStreak(0.f)
	, mHasScoredCurrentHand(false)
{}

float Score::GetScore() const
{
	return mScore;
}

void Score::UpdateOnBlockPlaced(int tilesPlaced, int tilesCleared, int handSize)
{
	if (handSize == 2) mHasScoredCurrentHand = false; // Every new hand, rest has scored bool

	// Increment score
	mScore += scScorePerPlacedTile * tilesPlaced; // Increase score by tiles placed
	mScore += (tilesCleared * tilesCleared) / Config::Get().tileMap.width * scScorePerClearedTile * mScoreMultiplier; // tiles squared / tiles per row * points per row * multiplier
	
	// Update multiplier after score is incremented
	if (tilesCleared > 0) // Has scored
	{
		mHasScoredCurrentHand = true;
			
		// Update multiplier
		if (mScoreMultiplier < scScoreMultiplierMax)
			mScoreMultiplier += scScoreMultiplierIncrement + mScoreStreak;

		mScoreStreak++;
	}
	else // No score
	{
		mScoreStreak = 0;

		if (handSize == 0 && !mHasScoredCurrentHand) // If you don't score once through entire hand
			mScoreMultiplier = 1.f;
	}
}

void Score::Reset()
{
	*this = Score();
}
