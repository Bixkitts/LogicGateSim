#ifndef CHIP
#define CHIP

#include "Libraries.h"
#include "Session.h"
#include "BArray.h"
#include "Gates.h"
#include "Wiring.h"
#include "Bitstream.h"
#include "Output.h"

class Chip
{
public:
    Chip(Session* session); 		//When a chip is constructed, it must be linked to a session which tracks the global timestep and event ques.
	Chip(); 																

	void LinkSession(Session* session);
   // Chip(const Chip&);      		//When a chip is created inside another one,
									//it's logic circuits need to be copied and allocated.                        

    Chip& operator =(Chip& chip);

// these functions do not happen while the circuit runs and are for modifying the circuit to simulate.
// the actual simulation is stopped if any of these are called.
	void ChipInit(); 	//anything that needs to be done on startup before any program is run
    void SpawnComponent(ElectronicObjects type);
	void PushComponent(Wiring* wire);
	void PushComponent(Gate* gate);
    void DeleteComponent(size_t UID, ElectronicObjects type);
    void AttachWiring(uint32_t Index1, uint32_t Index2);    		//Attach wiring to wiring, making them pass on their state to eachother
    void AttachWiring(uint32_t Index1, uint32_t Index2, char pin);  //Attach wiring to a gate!
    void DetachWiring(uint32_t Index1, uint32_t Index2); 
    void DetachWiring(uint32_t wIndex, uint32_t gIndex, char pin);
  
	void MarkInput(Wiring* wire); 	//Mark a wire as being and Input or output wire of this chip.
	void MarkOutput(Wiring* wire); 	//This is semantically useful for interfacing.
	
// Functions that run during the simulation
	void Impulse(uint64_t wire, bool bit); // Sends a bit down a wire that'll cause the chip to generate ques for the coming timesteps.

    void AmmendWireque(Wiring* wire);
    void ProcessWireque();
    void AmmendGateque(Gate* gate);
    void ProcessGateque();

	void SyncTimestep(); 	//Syncs the local timestep to the session one. It is otherwise static.
    Wiring* getWire(uint32_t UID);
    Gate* getGate(uint32_t UID);
	
// The Variables
	int ChipID; 				//Session Chip ID

	Session* LinkedSession; 	//The Session that this chip belongs to as it is being instantiated.

	BArray<Wiring*> Inputs; 	//These arrays point to the wirings that are to serve as inputs and outputs to the chip.
	BArray<Wiring*> Outputs; 	//This distinguishes these wires from the rest and allows easy interfacing.

	BArray<Gate*> garray; 		//Arrays containing pointers to every wire and gate in the chip.
    BArray<Wiring*> warray; 	//Including the input and output wires.
    BArray<Gate*> gateque[8] = {};   //ques of all the gate and wire events, their length should exceed the largest amount of time it could possibly take to traverse one depth level in the logic system.
    BArray<Wiring*> wireque[8] = {}; //It needs two dimensions because a single time step is gonna hold many instructions.

	uint8_t timestep; 				//this is meant to be synched to the Session timestep.
	

};

#endif
