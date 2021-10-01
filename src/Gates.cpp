#include "Gates.h"

bool Gate::output(bool a, bool b)
{
    std::cout << "Default Gate output called. WHY. \n";
    return 0;
}
bool Gate::output(bool a)
{
    return 0;
}

bool GateNAND :: output(bool a, bool b)
{
    return !(a && b);
}

bool GateAND::output(bool a, bool b)
{
    return (a && b);
}

bool GateOR::output(bool a, bool b)
{
    return (a || b);
}
bool GateNOT::output(bool a)
{
    return (!a);
}
