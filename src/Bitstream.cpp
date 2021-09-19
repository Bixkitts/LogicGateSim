#include "Bitstream.h"
#include "Chip.h"
#include "BArray.h"
#include <map>

Program::~Program()
{
	delete bitstream;
}

void Program::loadprogram(std::string file)
{
	prog.open(file, std::ios::binary);
	if (prog.is_open())
	{
		size = prog.tellg();
		bitstream = new char[size];
		prog.seekg(0, std::ios::beg);
		prog.read(bitstream, size);
		prog.close();
	}
}

char& Program::getbitstream()
{
	return bitstream[0];
}

bitstream::HDL bitstream::loadHDL(std::string file)	//Loads a hdl file into a char array and returns a pointer to the start
{
	std::ifstream session;
	std::streampos size;


	session.open(file, std::ios::binary);
	if (session.is_open())
	{
		size = session.tellg();
		char* bitstream = new char[size];
		session.seekg(0, std::ios::beg);
		session.read(bitstream, size);
		session.close();
		HDL chip{ bitstream, (size_t)size };
		return chip;
	}
}

Chip* bitstream::parseHDL(bitstream::HDL hdl)
{

	bool synerror = 0;										//is there a syntax error
	int chipcount = 0;	

	std::map<std::string, Chip> chips;	
	std::map<std::string, Chip>::iterator chipsIT;

	std::map<std::string, Gate> localGates; 				//A list of gates declared within local scope of the chip
	std::map<std::string, Gate>::iterator localGatesIT;

	std::map<std::string, Wiring> localWires;				//List of local scope wires- both this and the "localGates" are ultimately stored
	std::map<std::string, Wiring>::iterator localWiresIT;	//in the map of "chips" inside each chip
												


	//start at the first character, and check each one for anything that might initiate a command.
	//skip over whitespace and line breaks.
	for (size_t i = 0; i < hdl.length; i++)
	{
		if (hdl.string[i] == ' ')
		{
			//it's whitespace
			continue;
		}
		if (hdl.string[i] == 10)
		{
			//it's a line break
			continue;
		}
		if (hdl.string[i] == '/' && hdl.string[i + 1] == '/')
		{
			i += 2;
			while (hdl.string[i] != 10)
				i++;
			continue;
		}
		if (hdl.string[i] == 'C' && hdl.string[i + 3] == 'P' && hdl.string[i + 1] == 'H' && hdl.string[i + 2] == 'I')
		{
			//a new Chip is declared, everything is built into it until a closing bracket.
			//every logic gate and chip declared here is put into local scope map.
			//Chips cannot be defined inside other chips!
			//A definition will be looked up in chip map and hooked in at the declared point,
			//either way a new chip will be allocated.


		//	Chip* CurrentChip = new Chip();	//allocate a chip on the heap

			i += 4;
			if(hdl.string[i] == ' ')
			{
				i++;
				std::string name;	//the string we'll store away in the local scope map
					while (bitstream::isLetter(hdl.string[i]))	//variable names should only be comprised of letters
					{
						name += hdl.string[i];
						i++;
					}
				chipsIT = chips.find(name);	//find the name in the map of chips
				if(chipsIT != chips.end())	//if it's found, this is an already defined chip so we just copy the data in here
				{


				}	
				//else we're go


			}
		}
	}
}

bool bitstream::isLetter(char c)
{
	return (c > 64 && c <91)||(c > 96 && c < 123);	//checks if a character is an uppercase or lowercase letter
}
