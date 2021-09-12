#include "Session.h"
#include "Chip.h"

int main()
{

    Session CurrentSession;

    Chip* Chip1 = new Chip(CurrentSession);

    Chip1->SpawnComponent(OR);
    Chip1->SpawnComponent(WIRE);
    Chip1->SpawnComponent(WIRE);
    Chip1->SpawnComponent(WIRE);
    Chip1->AttachWiring(0, 0, 'a');
    Chip1->AttachWiring(1, 0, 'b');
    Chip1->AttachWiring(2, 0, 'o');
    char program[3] = { '1', '1', '1' };
    Chip1->RunProgram(3, 0, program, 3);


    return 0;
}

