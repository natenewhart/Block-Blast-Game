#include "BlockData.h"

const Blocks::tSignature Blocks::cSignatures[cNumberOfShapes] =
{
	{}, // Empty

	{ //OneByOne
		{0, 0}
	},
	{ //TwoByOne
		{0, 0},{1, 0}
	},
	{ //ThreeByOne
		{0, 0}, {1, 0}, {2, 0}
	},
	{ //FourByOne
		{0, 0}, {1, 0}, {2, 0}, {3, 0}
	},
	{ //FiveByOne
		{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}
	},

	{ //TwoByTwo
		{0, 1}, {1, 1},
		{0, 0}, {1, 0}
	},
	{ //TwoByThree
		{0, 2}, {1, 2},
		{0, 1}, {1, 1},
		{0, 0}, {1, 0}
	},
	{ //ThreeByThree
		{0, 2}, {1, 2}, {2, 2},
		{0, 1}, {1, 1}, {2, 1},
		{0, 0}, {1, 0}, {2, 0}
	},

	{ //LShapeLarge
		{0, 2},
		{0, 1},
		{0, 0}, {1, 0}, {2, 0}
	},
	{ //LShapeMedium
		{0, 2},
		{0, 1},
		{0, 0}, {1, 0}
	},
	{ //LShapeSmall
		{0, 1},
		{0, 0}, {1, 0}
	},
	{ //TShape
				{1, 1},
		{0, 0}, {1, 0}, {2, 0},
	},
	{ //SShape
				{1, 1}, {2, 1},
		{0, 0}, {1, 0}
	},
	{ //TwoDiagonal
				{1, 1},
		{0, 0}
	},
	{ //ThreeDiagonal
						{2, 2},
				{1, 1},
		{0, 0}
	},
};

const int Blocks::cOrientations[cNumberOfShapes] =
{
	0, // Empty

	1, // OneByOne
	2, // TwoByOne
	2, // ThreeByOne
	2, // FourByOne
	2, // FiveByOne
	1, // TwoByTwo
	2, // TwoByThree
	1, // ThreeByThree
	4, // LShapeLarge
	4, // LShapeMedium
	4, // LShapeSmall
	4, // TShape
	2, // SShape
	2, // TwoDiagonal
	2  // ThreeDiagonal
};

const float Blocks::cWeights[cNumberOfShapes] =
{
	0.0f, // Empty  

	0.1f, // OneByOne
	0.2f, // TwoByOne
	0.4f, // ThreeByOne
	0.5f, // FourByOne
	0.6f, // FiveByOne
	0.5f, // TwoByTwo
	0.6f, // TwoByThree
	0.8f, // ThreeByThree
	0.7f, // LShapeLarge
	0.5f, // LShapeMedium
	0.5f, // LShapeSmall
	0.8f, // TShape
	0.6f, // SShape
	0.3f, // TwoDiagonal
	0.1f  // ThreeDiagonal
};
