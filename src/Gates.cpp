#include "Gates.h"
#include "Wiring.h"
#include "Chip.h"
/*
Gate::Gate(std::string n, Chip* c)
{
	name = n;
	c->PushComponent(this);	
}
*/
bool Gate::output()
{
    std::cout << "Default Gate output called. WHY. \n";
    return 0;
}

void Gate::transferIO(Wiring** w)
{
	setInput(0, w[0]);
	setInput(1, w[1]);
	setOutput(0, w[2]);

}
void Gate::setInput(int x, Wiring* w)
{
	inputs[x] = w;
}

void Gate::setOutput(int x, Wiring* w)
{
	outputs[x] = w;
}


bool GateNAND :: output()
{
    return !(inputs[0]->state && inputs[1]->state);
}
GateNAND :: GateNAND(std::string n, Chip* c)
{
	name = n;
	c->PushComponent(this);


}
/*
bool GateAND::output(bool a, bool b)
{
    return (a && b);
}

bool GateOR::output(bool a, bool b)
{
	
    return (a || b);
}
bool GateNOT::output(bool a, bool b)
{
    return (!a);
}
*/
