#include "Session.h"
#include "Chip.h"

void Session::AddChip(Chip* chip)
{
	chips.Push(chip);
	ChipCount++;
}
void Session::RunProgram(char* program, int length, int chipID, uint64_t wIndex)
{
	for (int i = 0; i < length; i++)	//core loop, this is our program, our signal that changes each timestep. each iteration is a change in signal and in timestep.
	{
		bool bit = program[i]-48;
		chips[chipID] -> Impulse(wIndex, bit);
		
		for(int i = 0; i < ChipCount; i++)
		{
			threads.push_back(std::thread(&Session::ProcessChip, this, i));
		}
		for(int i = 0; i < ChipCount; i++)
		{
			if (threads[i].joinable())
			{
				threads[i].join();
				std::cout<<"thread joined\n";
			}
		}
		

		AdvanceTimestep(1);
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

void Session::AdvanceTimestep(int t)
{
timestep += t;
}
