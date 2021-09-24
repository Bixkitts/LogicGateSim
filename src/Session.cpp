#include "Session.h"
#include "Chip.h"

#define GET_BIT(x, n) ((((x)[(n) / 8]) & (0x1 << ((n) % 8))) != 0)


void Session::AddChip(Chip* chip)
{
	chips.Push(chip);
	ChipCount++;
}
void Session::RunProgram(char* program, int length, int chipID, uint64_t wIndex[], uint8_t busSize, int frequency)
{
	for (int i = 0; i < length; i++)	//core loop, this is our program, our signal that changes each timestep. each iteration is a change in signal and in timestep.
	{
		
		int bInputbits = i % frequency; 	//compiler division optimisation add flag -o1
		int pos1 = i / frequency;

		if((bInputbits) == 1)
		{
			for(int j =0; j < busSize; j++)//Each bit inputted into the system
			{
				uint64_t pos2 = j + (busSize * pos1);
				bool bit = GET_BIT(program, pos2);
				chips[chipID] -> Impulse(wIndex[j], bit);
				std::cout << "Impulse bit:" << bit << "\n";
			}
		}

		for(int j = 0; j < ChipCount; j++)
		{
			threads.push_back(std::thread(&Session::ProcessChip, this, j));
		}
		for(std::thread &t : threads)
		{
			if (t.joinable())
			{
				t.join();
			}
		}
		
	//	threads.clear();
		AdvanceTimestep();
	}

}
void Session::ProcessChip(int i) 	//this right here is going to be run on a separate thread for each chip
{
		chips[i] -> SyncTimestep();
		chips[i] -> ProcessWireque();
		chips[i] -> ProcessGateque();
		chips[i] -> gateque[timestep].ClearData();
		chips[i] -> wireque[timestep].ClearData();
}

void Session::AdvanceTimestep()
{
	globaltimestep++;
	if(timestep == 7)
	{
		timestep = 0;
		return;
	}
	timestep ++;
}
