#include "block.h"

const tBlockSignature BLOCK_SIGNATURES[NUMBER_OF_BLOCK_TYPES] =
{
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
		{0, 0}, {1, 0},
		{0, 1}, {1, 1},
		{0, 2}, {1, 2}
	},
	{ //ThreeByThree
		{0, 0}, {1, 0}, {2, 0},
		{0, 1}, {1, 1}, {2, 1},
		{0, 2}, {1, 2}, {2, 2}
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
		{-1, 1}, {0, 1}, {1, 1},
				 {0, 0}
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
