#include "Chip.h"

int main()
{

    //Session* CurrentSession;
    Chip* CurrentSession = new Chip();
    //CurrentSession = sesh1;

    CurrentSession->SpawnComponent(OR);
    CurrentSession->SpawnComponent(WIRE);
    CurrentSession->SpawnComponent(WIRE);
    CurrentSession->SpawnComponent(WIRE);
    CurrentSession->AttachWiring(0, 0, 'a');
    CurrentSession->AttachWiring(1, 0, 'b');
    CurrentSession->AttachWiring(2, 0, 'o');
    char program[3] = { '1', '1', '1' };
    CurrentSession->RunProgram(3, 0, program, 3);


    return 0;
}

