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

Hardware::HDL Hardware::loadHDL(char* file)	//Loads a hdl file into a char array and returns a pointer to the start
{
	std::cout << "\n Loading HDL....";
	std::ifstream session;
	std::streampos size;

	try
	{
		session.open(file, std::ios::binary);
		if (session.is_open())
		{
			session.seekg(0, std::ios::end);
			size = session.tellg();
			std::cout << "\n hdl size is " << size;
			char* bitstream = new char[size];
			session.seekg(0, std::ios::beg);
			session.read(bitstream, size);
			session.close();
			
			HDL loadedhdl{ bitstream, (size_t)size };
			return loadedhdl;
		}
		throw 1;
	}
	catch (int e)
	{
 		std::cout<<"Could not open file. \n";
	}

	HDL emptyhdl {nullptr, 0};
	return emptyhdl;
}

Hardware::HDL Hardware::loadHDL(std::string file)	//Loads a hdl file into a char array and returns a pointer to the start
{
	std::cout << "\n Loading HDL....";
	std::ifstream session;
	std::streampos size;

	try
	{
		session.open(file, std::ios::binary);
		if (session.is_open())
		{
			session.seekg(0, std::ios::end);
			size = session.tellg();
			std::cout << "\n hdl size is " << size;
			char* bitstream = new char[size];
			session.seekg(0, std::ios::beg);
			session.read(bitstream, size);
			session.close();
			
			HDL loadedhdl{ bitstream, (size_t)size };
			loadedhdl.Ppos.pos = 0;
			return loadedhdl;
		}
		throw 1;
	}
	catch (int e)
	{
 		std::cout<<"Could not open file. \n";
	}

	HDL emptyhdl {nullptr, 0};
	return emptyhdl;
}

void Hardware::parseHDL(HDL &hdl, Session* session, bool cat)
{
	hdl.cat = cat;
	std::cout << "\nParsing HDL file(s)...";
	bool synerror = 0;										//is there a syntax error
	int chipcount = 0;	

 	size_t& i {hdl.Ppos.pos};
	//start at the first character, and check each one for anything that might initiate a command.
	//skip over whitespace and line breaks.
	for (i = 0; i < hdl.length; i ++)
	{
		skipWhitespace(hdl); //self explanatory. Skip spaces and newlines and returns.

		if (isWord("//", hdl)) //One line comments.
		{
			i += 2;
			while (hdl.string[i] != 10)
				i++;
			continue;
		}
		if(isWord("CHIP", hdl))
		{
			std::cout <<"\nIsWord CHIP detected...";
			parseCHIP(hdl, session);
		}
		if(isWord("include:", hdl))
		{
			parseInclude(hdl, session);
		}
	}
}

void Hardware::parseCHIP(HDL &hdl, Session* session)
{
	//a new Chip is declared, everything is built into it until a closing bracket.
	//every logic gate and chip declared here is put into local scope map.
	//Chips cannot be defined inside other chips!
	//A definition will be looked up in chip map and hooked in at the declared point,
	//either way a new chip will be allocated.
 	char * s = hdl.string;
	size_t& i = hdl.Ppos.pos;

	i += 4;
	if(s[i] != ' ')
		syntaxError(hdl);
	i++;
	std::string name="default";
	i += readWord(hdl, name);
	Chip* CurrentChip = new Chip(name, session, hdl.cat);	//allocate a chip on the heap. Naturally, it pushes itself to the session passed to the parser.
	if(s[i] != ' ')
		syntaxError(hdl);
	i++;
	if(s[i] != '{')
		syntaxError(hdl);
	i++;
	//The bracket is opened. We skip whitespace and parse it's PARTS
	//until the next closing bracket.
	skipWhitespace(hdl);	
 	parsePARTS(hdl, CurrentChip);


}

void Hardware::parsePARTS(HDL &hdl, Chip* chip)
{

	Session* session = chip->LinkedSession;
	char * s = hdl.string;
	size_t& i = hdl.Ppos.pos;
//1. Create string maps for variables declared in the HDL.
//These will hold pointers to hdl.gates and hdl.wires under their declared names.
//We'll link them all together, then arbitrarily push them into 'chip''s appropriate BArrays.


//2. We look for the inputs and outputs IN and OUT.
//These are in input and output hdl.wires.
//-----IN-------//
	i+=2;
	if(s[i] != ' ')
		syntaxError(hdl);
	i++;

	bool inputsListed = 0;
	while( inputsListed == 0) 	//A loop to look through the user-defined variables for chip inputs and push them to our local map.
	{
		if(!isLetter(s[i]) || isNumber(s[i]))
			syntaxError(hdl);
		//Name and create a new wiring in the chip and mark it as in an input
		std::string name;
		i += readWord(hdl, name);	
		Wiring* wire = new Wiring(name, chip);
		chip->MarkInput(wire);
		//------------------
		if(s[i] == ',')
		{
			i++;
			skipWhitespace(hdl);
			continue;
		}
		if(s[i] == ';')
		{
			inputsListed = 1;
			break;
		}
		syntaxError(hdl);
	}
	i++;
	skipWhitespace(hdl);
//-----OUT-----//
	isWord("OUT", hdl);
	i+=3;
	if(s[i] != ' ')
		syntaxError(hdl);
	i++;

	bool outputsListed = 0;
	while( outputsListed == 0) 	//A loop to look through the user-defined variables for chip inputs and push them to our local map.
	{
		if(!isLetter(s[i])|| isNumber(s[i]))
			syntaxError(hdl);
		//Read the string, create a wire with that name
		std::string name;
		i += readWord(hdl, name);	
		Wiring* wire = new Wiring(name, chip);
		chip -> MarkOutput(wire);
		//
		if(s[i] == ',')
		{
			i++;
			skipWhitespace(hdl);
			continue;
		}
		if(s[i] == ';')
		{
			outputsListed = 1;
			break;
		}
		syntaxError(hdl);
	}
	i++;
	skipWhitespace(hdl);

//3. We look under 'PARTS:' for gate or chip interfaces, and run functions to link them together with hdl.wires.
	if(!isWord("PARTS:", hdl))
		syntaxError(hdl);
	i += 7;
	skipWhitespace(hdl);
	//Here we start listing hdl.gates/chips and what they link to....
	
	//'InOut' temporarily holds all the inputs and outputs
	//of a NAND gate or a freshly created chip
	//so we can operate on them in an indifferent manner
	BArray<Wiring*> InOut;
	int nInputs = 0;
	int nOutputs = 0;

	Gate* gate;
	Chip* newchip;

	bool partsListed = 0;
	while(partsListed == 0)
	{
		bool isChip = 0;
		//a part is declared- it's either a NAND gate or a chip.
		//
		if(isWord("NAND", hdl))
		{
			i+=4;
			if(s[i] != ' ')
				syntaxError(hdl);
			i++;
			std::string name;
			i += readWord(hdl, name);
			gate = new GateNAND(name, chip);
			//these are initialised to null pointers from the fresh gate... for clarity.
			InOut.Push(gate->inputs[0]);
			InOut.Push(gate->inputs[1]);
			nInputs = 2;
			InOut.Push(gate->outputs[0]);
			nOutputs = 1;
		}
		else
		{
			isChip = 1;
			std::string chipLookup;
			i+=readWord(hdl, chipLookup);
			newchip = new Chip(*session->SearchCat(chipLookup)); 	//Need to get copy constructor perfect for this
 			//Push the chip's input and output Wiring pointers to the array for processing
			for(int x = 0; x < newchip->Inputs.size; x++)
			{
 			 	InOut.Push(newchip->Inputs[x]);
				nInputs++;
			}
			for(int x = 0; x < newchip->Outputs.size; x++)
			{
 			 	InOut.Push(newchip->Outputs[x]);
				nOutputs++;
			}
			if(s[i] != ' ')
				syntaxError(hdl);
			i++;
			std::string newName; //The chip is already going to have a name from the catalogue.
								 //This is the new one that refers to the specific existing instance and not it's Cat entry.
			i+=readWord(hdl, newName);
			newchip->name = newName;
		}

		if(s[i] != ' ')
			syntaxError(hdl);
		skipWhitespace(hdl);
		if(s[i] != '{')
			syntaxError(hdl);
		i++;
		bool linksListed = 0;

		for(int x = 0; x < nInputs; x++)
		{
			//look through each input parameter.
			//In this little block, the parameter is looked up in the chips.
			std::string sParam;
			i+=readWord(hdl, sParam);
			InOut[x] = parsePartParam(sParam, chip);

			if(s[i] != ',')
				syntaxError(hdl);
			
		i+=2;	
		}
		for(int x = nInputs; x <= InOut.size; x++)
		{
			//look through each input parameter.
			//In this little block, the parameter is looked up in the chips.
			std::string sParam;
			i+=readWord(hdl, sParam);
			InOut[x] = parsePartParam(sParam, chip);

			if(s[i] == '}')
			{
				break;
			}
			if(s[i] != ',')
				syntaxError(hdl);
			
		i+=2;	
		}
	
		i++;
		if(s[i] != ';')
			syntaxError(hdl);
		i++;
		skipWhitespace(hdl);
		//To finish a part and it's I/O, copy the I/O BArray from here into it...
		if(isChip == 0)
			gate->transferIO(&InOut[0]);
		else
			chip->transferIO(&InOut[0]);

		if(s[i] == '}')
			partsListed = 1;
	}	
	
	std::cout << "\n Parsing parts completed successfully!\n";
}

void Hardware::parseInclude(HDL &hdl, Session * session)
{
	char * s = hdl.string;
	size_t& i = hdl.Ppos.pos;
	i+=8;
	if(s[i] !=' ')
		syntaxError(hdl);
	i++;
	std::string file;
	i+=readWord(hdl, file);
	HDL newhdl = loadHDL(file);
	parseHDL(newhdl, session, 1);

	if(s[i] != ';')
		syntaxError(hdl);
	i++;
	std::cout<<"\nparseInclude over\n";
}
