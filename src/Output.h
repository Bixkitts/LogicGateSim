#ifndef OUTPUT
#define OUTPUT

#include "BArray.h"
#include <string>
class Wiring;
class Session;

class Output
{
public:
	virtual void doMyThing();

	BArray<Wiring*> wires;
	Session* MySession;

};

class LED : public Output
{
	LED(Session* session, std::string n, Wiring* in);
	void doMyThing();

	bool wirestate;
	std::string name;
};

#endif
