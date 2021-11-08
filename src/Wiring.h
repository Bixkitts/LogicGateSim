#ifndef WIRING
#define WIRING

#include "BArray.h"
#include "Enums.h"
class Gate;
class Chip;

class Wiring
{
public:
    Chip* LinkedChip = nullptr;
    Gate* LinkedGate = nullptr; //connected gate to this wire
    BArray<Wiring*> LinkedWires; //connected wires to pass a charge onto on the same timestep
    Wiring* InputWire = nullptr; //The wire/gate that inputs into this wire. Needed to be noted so that if this wire is deleted/disconnected it can reference the object before it instead of checking every object in the sim.
    Gate* InputGate = nullptr;
    bool state = 0; //remember to optimise booleans to save memory ofc. this is the 1/0 state of the wire
	std::string name;

    Wiring(Chip* chip);
	Wiring(std::string n, Chip* chip);
    ~Wiring();

    void addOutput(Wiring* wire); //function that ammends a gate to this wiring that cares about it's state so it can pass the signal on to them
    void addOutput(Gate* gate);
    void removeOutput(Wiring* wire); //function to remove an output gate based on it's UID, meaning we'll need to search it up but that's a non issue as it's not per-tick
    void removeOutput(Gate* gate);
    void addInput(Wiring* wire);
    void addInput(Gate* gate);
    void removeInput();
    void setState(bool s);
    void toggleState();
    
};


#endif
