#include "Session.h"
#include "Chip.h"

int main(int argc, char *argv[])
{
	Session* CurrentSession = new Session();
	Hardware::HDL file = Hardware::loadHDL(argv[1]);
	Hardware::parseHDL(file, CurrentSession);
	char max = 24;
    char program[2] = { max, max };
	uint64_t wireindexes[1] = { 0 };
	CurrentSession -> RunProgram(program, 80, 0, wireindexes, 1, 5);


    return 0;
}

