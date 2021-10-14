#include "Chip.h"
#include "BArray.h"
#include "Gates.h"
#include "Wiring.h"

Chip::Chip(Session* session)
{
	LinkedSession = session;
	ChipID = LinkedSession ->ChipCount;
	timestep = LinkedSession -> timestep;
	LinkedSession -> AddChip(this);
}

Chip::Chip()
{
	std::cout << "Chip instantiated without a Linked Session!";
}

void Chip::LinkSession(Session* session)
{
	LinkedSession = session;
	ChipID = LinkedSession ->ChipCount;
	timestep = LinkedSession -> timestep;
	LinkedSession -> AddChip(this);
}

void Chip::SpawnComponent(ElectronicObjects type)
{
	switch (type)	
	{

	case NAND:  
	{
		GateNAND* somegate = new GateNAND();
		garray.Push(somegate);
		break;
	}
	case AND:	
	{
		GateAND* somegate = new GateAND();
		garray.Push(somegate);
		break;
	}
	case OR:	
	{
		std::cout << "OR gate made \n";
		GateOR* somegate = new GateOR();
		garray.Push(somegate);
		break;
	}
	case NOT:
	{
		GateNOT* somegate = new GateNOT();
		garray.Push(somegate);
		break;
	}
	case WIRE:
	{
		Wiring* somewire = new Wiring(this);
		warray.Push(somewire);
		break;
	}
	default: 
	{
		std::cout << "default gate created. It doesn't do anything... WHY. \n";
		GateNAND* somegate = new GateNAND();
		garray.Push(somegate);
		break;
	}
	}


	std::cout << "Spawned a gate \n";
}

void Chip::DeleteComponent(size_t UID, ElectronicObjects type)
{
	if (type == WIRE)	//we're deleting a wire
	{	

		//first clear outputs and inputs of this wire of references to it.
		if (warray[UID]->InputGate != nullptr)	
		{
			warray[UID]->InputGate->outputWire = nullptr;
		}
		if (warray[UID]->LinkedGate != nullptr)
		{
			if (warray[UID]->LinkedGate->inputa == warray[UID])
				warray[UID]->LinkedGate->inputa = nullptr;
			else
				warray[UID]->LinkedGate->inputb = nullptr;
		}
		for(int i = 0; i < warray[UID]->LinkedWires.size; i++)
		{ 
			warray[UID]->LinkedWires[i]->removeInput();
		}
		//-----------------------------------------------------------------
		
		delete warray[UID];
		warray.Remove(UID);
		return;
	}
		//Else
        //we're deleting a gate
		//first clear outputs and inputs of this object of references to it
		garray[UID]->inputa->removeOutput(garray[UID]);
		garray[UID]->inputb->removeOutput(garray[UID]);
		garray[UID]->outputWire->removeInput();
		//-----------------------------------------------------------------
		delete garray[UID];
		garray.Remove(UID);
}

void Chip::AttachWiring(uint32_t Index1, uint32_t Index2)
{
	warray[Index1]->addOutput(warray[Index2]);
	warray[Index2]->addInput(warray[Index1]);
}

void Chip::AttachWiring(uint32_t Index1, uint32_t Index2, char pin)
{
	if (pin == 'a')
	{
		warray[Index1]->addOutput(garray[Index2]);
		garray[Index2]->inputa = warray[Index1];
		return;
	}
	if (pin == 'b')
	{
		warray[Index1]->addOutput(garray[Index2]);
		garray[Index2]->inputb = warray[Index1];
		return;
	}
	if (pin == 'o')
	{
		warray[Index1]->addInput(garray[Index2]);
		garray[Index2]->outputWire = warray[Index1];
		return;
	}
}

void Chip::DetachWiring(uint32_t Index1, uint32_t Index2)	//detach wiring from wiring.
{
	if (warray[Index1]->InputWire == warray[Index2])
	{
		warray[Index1]->removeInput();
		for (int i = 0; i < warray[Index2]->LinkedWires.size; i++)
		{
			if (warray[Index2]->LinkedWires[i] == warray[Index1])
				warray[Index2]->LinkedWires.Remove(i);
		}
	}
	else
	{
		warray[Index2]->removeInput();
		for (int i = 0; i < warray[Index1]->LinkedWires.size; i++)
		{
			if (warray[Index1]->LinkedWires[i] == warray[Index2])
				warray[Index1]->LinkedWires.Remove(i);
		}
	}
}

void Chip::DetachWiring(uint32_t wIndex, uint32_t gIndex, char pin)	//detach wiring from any gate pin, pass wiring in first
{
	if (warray[wIndex]->InputGate == garray[gIndex])
	{
		warray[wIndex]->removeInput();
		garray[gIndex]->outputWire = nullptr;
	}
	else
	{
		warray[wIndex]->LinkedGate = nullptr;
		if (pin == 'a')
			garray[gIndex]->inputa = nullptr;
		else
			garray[gIndex]->inputb = nullptr;

	}
}

void Chip::MarkInput(Wiring* wire)
{
	Inputs.Push(wire);
}
void Chip::MarkOutput(Wiring* wire)
{
	Outputs.Push(wire);
}	

//Simulating logic circuit:
//wires pass on a signal instantly
//event que of wires to check, and a separate que of gates to check after
//cue is a time que, gates take time. Every time a gate or wire is outputted to it's added to it's respective cue, unless it's wire to wire

void Chip::Impulse(uint64_t wire, bool bit)
{
		if (warray[wire]->state != bit)		//if the wire state is different to the input bit change the state
			wireque[timestep].Push(warray[wire]);
}

void Chip::AmmendWireque(Wiring* wire)	//Adds a wire to the wireque at the next timestep, used for gates typically
{
	if(wire != nullptr)
		wireque[(timestep+1) % 7].Push(wire);
}

//Toggles the state of wires at the current timestep, recursively toggling their output wires and adding their output gates to the gateque
void Chip::ProcessWireque()	
{
	for (int i = 0; i < wireque[timestep].size; i++)
	{
		wireque[timestep][i]->toggleState();
	}
}

void Chip::AmmendGateque(Gate* gate)
{
	if(gate != nullptr)
		gateque[(timestep + gate->time) % 7].Push(gate);
}

void Chip::ProcessGateque()
{
	for (int i = 0; i < gateque[timestep].size; i++)
	{
		Gate* gate = gateque[timestep][i];

		bool result = gate->output(gate->inputa->state, gate->inputb->state);

		if (gate->outputWire->state != result)	
			AmmendWireque(gate->outputWire);
	}
}

void Chip::SyncTimestep()
{
	timestep = LinkedSession ->timestep;
}

Wiring* Chip :: getWire(uint32_t UID)
{
	return warray[UID];
}

Gate* Chip :: getGate(uint32_t UID)
{
	return garray[UID];
}
