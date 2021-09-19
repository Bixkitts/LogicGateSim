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

	uint8_t timestep;
	int ChipCount;
	uint8_t threadcount;

	char* program;


	void AddChip(Chip* chip);	
	void ProcessChip(int i);
	void RunProgram(char* program, int length, int chipID, uint64_t wIndex); 	//calls processing for each gate and wire que on each chip (on separate threads) and then advances the timestep by one.
	void AdvanceTimestep(int t);
	
};
#endif
