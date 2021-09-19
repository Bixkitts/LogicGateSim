#include "Session.h"
#include "Chip.h"

int main()
{
	Session* CurrentSession = new Session();

    Chip* Chip1 = new Chip(CurrentSession);

    Chip1->SpawnComponent(OR);
    Chip1->SpawnComponent(WIRE);
    Chip1->SpawnComponent(WIRE);
    Chip1->SpawnComponent(WIRE);
    Chip1->AttachWiring(0, 0, 'a');
    Chip1->AttachWiring(1, 0, 'b');
    Chip1->AttachWiring(2, 0, 'o');
    char program[3] = { '1', '1', '1' };
	CurrentSession -> RunProgram(program, 3, 0, 0);


    return 0;
}

