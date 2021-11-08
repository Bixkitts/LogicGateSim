#ifndef GATES
#define GATES

#include "Libraries.h"
#include <string>
#include "BArray.h"
class Wiring;
class Chip;

class Gate
{
public:
    //Gates never reference their own data, they don't need to. Unlike Wires which have recursive functions gates can just return some
    //logic and work through the session functions.

    Wiring* inputs[2] = {nullptr, nullptr};   //All of these are just Universal IDs for wires within the session in the "warray" vector
    Wiring* outputs[1] = {nullptr};
    uint8_t time = 1;   //how far down the gateque the gate gives an output
	std::string name;

	void transferIO(Wiring** w); 	//Transfers the input/output addresses 1:1 from an array of pointers.
	void setInput(int x, Wiring* w);
	void setOutput(int x, Wiring* w);
    virtual bool output();
};

class GateNAND : public Gate
{
	public:
	GateNAND(std::string n, Chip* c);
    bool output() override;
};
/*
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
*/
#endif
