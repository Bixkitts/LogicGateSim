#ifndef GATES
#define GATES

#include "Libraries.h"
#include <string>
class Wiring;

class Gate
{
public:
    //Gates never reference their own data, they don't need to. Unlike Wires which have recursive functions gates can just return some
    //logic and work through the session functions.

    Wiring* inputa = nullptr;   //All of these are just Universal IDs for wires within the session in the "warray" vector
    Wiring* inputb = nullptr;
    Wiring* outputWire = nullptr;
    uint8_t time = 1;   //how far down the gateque the gate gives an output

    virtual bool output(bool a, bool b);
    virtual bool output(bool a);
};

class GateNAND : public Gate
{
    bool output(bool a, bool b) override;
};

class GateAND : public Gate
{
    bool output(bool a, bool b) override;
};

class GateOR : public Gate
{
    bool output(bool a, bool b) override;
};
class GateNOT : public Gate
{
    bool output(bool a, bool b) override;
};
#endif
