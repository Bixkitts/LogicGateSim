#ifndef SESSION
#define SESSION

#include <vector>
#include <thread>
#include "BArray.h"
class Chip;
class Wiring;
class Gate;

class Session  
{
public:
	BArray<Chip*> chips;
	std::vector<std::thread> threads;

	uint64_t globaltimestep;
	uint8_t timestep; 			//The timestep is just the globaltimestep modulo 8, but it's more computationally efficient to track them separately.
	int ChipCount;
	uint8_t threadcount;
	std::thread t;
	char* program;


	void AddChip(Chip* chip);	
	void ProcessChip(int i);
	void RunProgram(char* program, int length, int chipID, uint64_t wIndex[], uint8_t busSize, int frequency); 	//calls processing for each gate and wire que on each chip (on separate threads) and then advances the timestep by one.
	void AdvanceTimestep();
	
};
#endif
