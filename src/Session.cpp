#include "Session.h"
#include "Chip.h"
#include "Output.h"

#define GET_BIT(x, n) ((((x)[(n) / 8]) & (0x80 >> ((n) % 8))) != 0)


void Session::AddChip(Chip* chip)
{
	std::cout<<"\nChip added...";
	chips.Push(chip);
	ChipCount++;
}

void Session::CatChip(Chip* chip)
{
	std::cout<<"\nChip Catted...";
	chipCat.Push(chip);

}

Chip* Session::SearchCat(std::string s)
{
 	for(int x = 0; x < chipCat.size; x++)
	{
		if(s == chipCat[x]->name)
		{
			return chipCat[x];
		}
	}
	std::cout<<"\nERROR, chip '" << s << "' not found. \nIt needs to be included.\n";
 	exit(EXIT_FAILURE);

}

void Session::RunProgram(char* program, int length, int chipID, uint64_t wIndex[], uint8_t busSize, int frequency)
{
	std::cout << "\nRunning Program...";
	for(int i = 0; i<chips.size; i++)
	{
		chips[i]->ChipInit();

	}
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
		
		ProcessOutputs();	
		//threads.clear();
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

void Session::ProcessOutputs()
{
	int s = outputs.size;
	for(int i = 0; i < s; i++)
	{
		outputs[i]->doMyThing();
	}
}

void Session::AddOutput(Output* o)
{
	outputs.Push(o);
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
