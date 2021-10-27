#include "Output.h"
#include "Wiring.h"
#include "Session.h"
#include <iostream>

void Output :: doMyThing()
{
	std::cout << "This should not be called. Stop it.";
}

void LED :: doMyThing()
{
	if(wires[0]->state == 1 && wirestate == 0)
	{
		std::cout<<"\n>LED " << name << " turns on at timestep " << MySession->globaltimestep << "\n";	
		wirestate = 1;
		return;
	}
	
	if(wires[0]->state == 0 && wirestate == 1)
	{	
		std::cout<<"\n>LED " << name << " turns off at timestep " << MySession->globaltimestep << "\n";	
		wirestate = 0;
	}
	return;
}

LED :: LED(Session* session, std::string n, Wiring* in)
{
	MySession = session;
	session -> AddOutput(this); 
	wires.Push(in);
	wirestate = 0;
	name = n;
}
