#ifndef SESSION
#define SESSION

#include <string>
#include <vector>
#include <thread>
#include "BArray.h"
#include <map>
class Chip;
class Wiring;
class Gate;
class Output;

class Session  
{
public:
	void AddChip(Chip* chip); 	//This is run by a chip when it is instantiated.
	void ProcessChip(int i);
	void RunProgram(char* program, int length, int chipID, uint64_t wIndex[], uint8_t busSize, int frequency); 	//calls processing for each gate and wire que on 
																												//each chip (on separate threads) and then advances the timestep by one.
 	void ProcessOutputs(); 	//Cycles through each 
	void AdvanceTimestep();
	void AddOutput(Output* o);	

	std::map<std::string, Chip*> chipCat; 	// A catalogue of chips to be copied around as needed
	BArray<Chip*> chips;
	BArray<Output*> outputs;
	std::vector<std::thread> threads;

	uint64_t globaltimestep;
	uint8_t timestep; 			//The timestep is just the globaltimestep modulo 8, but it's more computationally efficient to track them separately.
	int ChipCount;
	uint8_t threadcount;
	std::thread t;
	char* program;
};
#endif
