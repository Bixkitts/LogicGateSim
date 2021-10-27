#include "Session.h"
#include "Chip.h"

int main(int argc, char *argv[])
{
	Session* CurrentSession = new Session();
	Hardware::HDL file = Hardware::loadHDL(argv[1]);
	Hardware::parseHDL(file, CurrentSession, 0);
	LED LED1(CurrentSession, "LED1", CurrentSession->chips[0]->Outputs.find("out")->second);
	char max = 24;
    char program[2] = { 0x00, max };
	uint64_t wireindexes[1] = { 0 };
	CurrentSession -> RunProgram(program, 100, 0, wireindexes, 1, 10);

    return 0;
}

