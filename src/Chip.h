#ifndef SESSION
#define SESSION

#include "Libraries.h"
#include "BArray.h"
#include "Gates.h"
#include "Wiring.h"
#include "Bitstream.h"

class Chip
{
private:
    BArray<Gate*> garray;
    BArray<Wiring*> warray;
    static BArray<Gate*> gateque[8];   //ques of all the gate and wire events, their length should exceed the largest amount of time it could possibly take to traverse one depth level in the logic system.
    static BArray<Wiring*> wireque[8]; //It needs two dimensions because a single time step is gonna hold many instructions.

    Program program;
    bool programloaded = 0;
    static uint8_t timestep = 0;   //The counter that controls what events to process next, this loops around at the same length of the two arrays for gates and wires.
                            //After processing all the events at the current timestep, it erases those intructions to clear space for the next set.
public:

    Chip();
    Chip(const Chip&);      //When a chip is created inside another one,
                            //it's logic circuits need to be copied and allocated.
                            
                          

    Chip& operator =(Chip& chip);

    // these functions do not happen while the circuit runs and are for modifying the circuit to simulate.
    // the actual simulation is stopped if any of these are called.
    void SpawnComponent(ElectronicObjects type);

    void DeleteComponent(size_t UID, ElectronicObjects type);    //Need to decrement all relevant memory addresses in wires!

    void AttachWiring(uint32_t Index1, uint32_t Index2);    //Attach wiring to wiring, making them pass on their state to eachother

    void AttachWiring(uint32_t Index1, uint32_t Index2, char pin);  //Attach wiring to a gate!

    void DetachWiring(uint32_t Index1, uint32_t Index2);  
    
    void DetachWiring(uint32_t wIndex, uint32_t gIndex, char pin);
  
    void SaveSession();     //writes all the heap data into a binary file on disk

    void LoadSession(std::string file);

    void LoadProgram(std::string file);     //loads a series of bits to input off of the hard drive

    //These functions are to deal with the running simulation and reference memory addresses that shouldn't be changed while it's running i.e don't modify the logic circuit while it runs, why would you.

    void RunProgram(int ticks, uint32_t wireindex, char* bits, size_t length);

    void AmmendWireque(Wiring* wire);

    void ProcessWireque();

    void AmmendGateque(Gate* gate);

    void ProcessGateque();

    Wiring* getWire(uint32_t UID);

    Gate* getGate(uint32_t UID);
};

#endif