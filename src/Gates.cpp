#include "Gates.h"
#include "Wiring.h"

bool Gate::output()
{
    std::cout << "Default Gate output called. WHY. \n";
    return 0;
}

bool GateNAND :: output()
{
    return !(inputs[0]->state && inputs[1]->state);
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
