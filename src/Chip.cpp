#include "Chip.h"
#include "BArray.h"
#include "Gates.h"
#include "Wiring.h"

Chip::Chip()
{

	
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

void Chip::LoadProgram(std::string file)
{
	program.loadprogram(file);
	programloaded = 1;
}

void Chip::LoadSession(std::string file)
{

}

void Chip::SaveSession()
{
	std::ofstream nandstream("Chip.gate", std::ofstream::binary);
	nandstream.write((char*)&garray, 12 * garray.size);

}

//Simulating logic circuit:
//wires pass on a signal instantly
//event que of wires to check, and a separate que of gates to check after
//cue is a time que, gates take time. Every time a gate or wire is outputted to it's added to it's respective cue, unless it's wire to wire

//Run program a program, add the events to the que
void Chip::RunProgram(int ticks, uint32_t wireindex, char* bits, size_t length)
{
	//start at the wireindex wiring, and set it's state according to the signal. For this we need to read each bit of each char or efficiency, or whatever. Make our own opcodes.
	//For now I'll just use char '1' and '0' for a digital input. this is essentially the program being read in.
	std::cout << "Running program...\n";
	timestep = 0;

	int l = length;
	for (int i = 0; i < l; i++)	//core loop, this is our program, our signal that changes each timestep. each iteration is a change in signal and in timestep.
	{
		if (warray[wireindex]->state != bits[i]-48)		//if the wire state is different to the input program change the state
			wireque[timestep].Push(warray[wireindex]);
		ProcessWireque();
		ProcessGateque();

		gateque[timestep].ClearData();
		wireque[timestep].ClearData();

		std::cout << "Timestep Advancing...\n";

		if (timestep == 7)
			timestep = 0;
		else
			timestep++;
	}
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
		std::cout << "A gate is processed OR:" << gate->inputa->state <<" || " << gate->inputb->state << "\n";

		bool result = gate->output(gate->inputa->state, gate->inputb->state);

		if (gate->outputWire->state != result)	
			AmmendWireque(gate->outputWire);
	}
}

Wiring* Chip :: getWire(uint32_t UID)
{
	return warray[UID];
}

Gate* Chip :: getGate(uint32_t UID)
{
	return garray[UID];
}
