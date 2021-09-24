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
	char max = 127;
	std::cout << max;
    char program[1] = { max };
	uint64_t wireindexes[1] = { 0 };
	CurrentSession -> RunProgram(program, 80, 0, wireindexes, 1, 5);


    return 0;
}

