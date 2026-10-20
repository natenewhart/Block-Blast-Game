// Nate Newhart
// Score.h

// Stores all score constants and variables and updates game score

#pragma once
class Score
{
public:
	Score();

	float GetScore() const;

	void UpdateOnBlockPlaced(int tilesPlaced, int tilesCleared, int handSize); // Hand size AFTER block is placed
	void Reset(); // Reset score and multipliers to zero

private:
	static constexpr float scScoreMultiplierIncrement = 1.f; // How much multiplier increases when you score
	static constexpr float scScorePerPlacedTile       = 10.f;
	static constexpr float scScorePerClearedTile      = 100.f / 8.f;
	static constexpr float scScoreMultiplierMax       = 11.f;

	float mScore;                // Current game score
	float mScoreMultiplier;      // Current score multiplier based on number of lines cleared in a single block placement
	bool  mHasScoredCurrentHand; // Tracks if player has scored at least once during their current block hand
	int   mScoreStreak;          // Number of consecutive block placements that cleared at least one line
};

