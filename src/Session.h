#ifndef SESSION
#define SESSION

#include "BArray.h"
#include "Gates.h"
#include "Wiring.h"

class Session  // A structure to be instantiated in main and hold some global parameters and variables pertaining to the simulation that is
		// shared across all chips.
{
public:
	BArray<Gate*> gateque[8];
	BArray<Wiring*> wireque[8];

	uint8_t timestep;
	int ChipCount;

	Session()
	{
		timestep = 0;
		ChipCount = 0;
		
	}	
}

#endif
