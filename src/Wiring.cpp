#include "Wiring.h"
#include "Chip.h"

Wiring::Wiring(Chip* chip)
{
	LinkedChip = chip;
	chip->PushComponent(this);
}
Wiring::Wiring(std::string n, Chip* chip)
{
	name = n;
	LinkedChip = chip;
	chip->PushComponent(this);
}
Wiring::~Wiring()
{

}

void Wiring::addOutput(Wiring* wire) //wirings can have whatever as their output so we pass it an enum for that.
{
		LinkedWires.Push(wire);
}

void Wiring::addOutput(Gate* gate) //wirings can have whatever as their output so we pass it an enum for that.
{
	LinkedGate = gate;
}

void Wiring::removeOutput(Wiring* wire)	//Search through this wire's connections to remove the correct output from it's list of outputs
{
		for (int i; i < LinkedWires.size; i++)
		{
			if (LinkedWires[i] == wire)
			{
				LinkedWires.Remove(i);
			}
		}

}

void Wiring::removeOutput(Gate* gate)	//Search through this wire's connections to remove the correct output from it's list of outputs
{
	LinkedGate = nullptr;
}

void Wiring::addInput(Wiring* wire)
{
	InputWire = wire;
}


void Wiring::addInput(Gate* gate)
{
	InputGate = gate;
}

void Wiring::removeInput()
{
	InputWire = nullptr;
	InputGate = nullptr;
}

void Wiring::setState(bool s)	//sets the state of this wire but also all those attached
{
	state = s;
	LinkedChip->AmmendGateque(LinkedGate);
	for (int i = 0; i < LinkedWires.size; i++)
	{
		LinkedWires[i]->setState(s);
	}
}

void Wiring::toggleState()	//toggles the state of this wire but also all those attached. The state is changed so it adds it's gate to the gateque if it outputs to a gate
{
	state = !state;
	LinkedChip->AmmendGateque(LinkedGate);
	for (int i = 0; i < LinkedWires.size; i++)
	{
		LinkedWires[i]->toggleState();
	}
}
