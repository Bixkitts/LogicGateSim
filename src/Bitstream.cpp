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

void Hardware::parseHDL(Hardware::HDL hdl, Session* session, bool cat)
{
	std::cout << "\nParsing HDL file(s)...";
	bool synerror = 0;										//is there a syntax error
	int chipcount = 0;	

	struct counter Ppos {0 , 0}; 	//Parsing position. Line, then character.										
 	size_t& i {Ppos.pos};
	//start at the first character, and check each one for anything that might initiate a command.
	//skip over whitespace and line breaks.
	for (i = 0; i < hdl.length; i ++)
	{
		skipWhitespace(hdl.string, Ppos); //self explanatory. Skip spaces and newlines and returns.

		if (isWord("//", hdl.string, i)) //One line comments.
		{
			i += 2;
			while (hdl.string[i] != 10)
				i++;
			continue;
		}
		if(isWord("CHIP", hdl.string, i))
		{
			parseCHIP(hdl.string, Ppos, session, cat);
		}
		if(isWord("include:", hdl.string, i))
		{
			parseInclude(hdl.string, Ppos, session);
		}
	}
}

void Hardware::parseCHIP(char * s, counter &Ppos, Session* session, bool cat)
{
	//a new Chip is declared, everything is built into it until a closing bracket.
	//every logic gate and chip declared here is put into local scope map.
	//Chips cannot be defined inside other chips!
	//A definition will be looked up in chip map and hooked in at the declared point,
	//either way a new chip will be allocated.

	size_t& i = Ppos.pos;

	i += 4;
	if(s[i] != ' ')
		syntaxError(1, 1);
	i++;
	std::string name="default";
	i += readWord(s, i, name);
	Chip* CurrentChip = new Chip(name, session, cat);	//allocate a chip on the heap. Naturally, it pushes itself to the session passed to the parser.
	if(s[i] != ' ')
		syntaxError(1, 1);
	i++;
	if(s[i] != '{')
		syntaxError(0, 0);
	i++;
	//The bracket is opened. We skip whitespace and parse it's PARTS
	//until the next closing bracket.
	skipWhitespace(s, Ppos);	
 	parsePARTS(s, Ppos, CurrentChip);


}

void Hardware::parsePARTS(char * s, counter &Ppos, Chip* chip)
{

	size_t& i = Ppos.pos;
//1. Create string maps for variables declared in the HDL.
//These will hold pointers to gates and wires under their declared names.
//We'll link them all together, then arbitrarily push them into 'chip''s appropriate BArrays.
	std::map<std::string, Wiring*> wires;	
	std::map<std::string, Wiring*>::iterator Iwires = wires.begin();

	std::map<std::string, Gate*> gates;	
	std::map<std::string, Gate*>::iterator Igates = gates.begin();


//2. We look for the inputs and outputs IN and OUT.
//These are in input and output wires.
//-----IN-------//
	i+=2;
	if(s[i] != ' ')
		syntaxError(Ppos.line, i);
	i++;

	bool inputsListed = 0;
	while( inputsListed == 0) 	//A loop to look through the user-defined variables for chip inputs and push them to our local map.
	{
		if(!isLetter(s[i]) || isNumber(s[i]))
			syntaxError(Ppos.line, i);
		std::string name = "default";
		i += readWord(s, i, name);	
		Wiring* wire = new Wiring(chip);
		wires.insert(std::pair<std::string, Wiring*>(name, wire));
		chip->MarkInput(name, wire);
		if(s[i] == ',')
		{
			i++;
			skipWhitespace(s, Ppos);
			continue;
		}
		if(s[i] == ';')
		{
			inputsListed = 1;
			break;
		}
		syntaxError(Ppos.line, i);
	}
	i++;
	skipWhitespace(s, Ppos);
//-----OUT-----//
	isWord("OUT", s, i);
	i+=3;
	if(s[i] != ' ')
		syntaxError(Ppos.line, i);
	i++;

	bool outputsListed = 0;
	while( outputsListed == 0) 	//A loop to look through the user-defined variables for chip inputs and push them to our local map.
	{
		if(!isLetter(s[i])|| isNumber(s[i]))
			syntaxError(Ppos.line, i);
		std::string name = "default";
		i += readWord(s, i, name);	
		Wiring* wire = new Wiring(chip);
		wires.insert(std::pair<std::string, Wiring*>(name, wire));
		chip -> MarkOutput(name, wire);

		if(s[i] == ',')
		{
			i++;
			skipWhitespace(s, Ppos);
			continue;
		}
		if(s[i] == ';')
		{
			outputsListed = 1;
			break;
		}
		syntaxError(Ppos.line, i);
	}
	i++;
	skipWhitespace(s, Ppos);

//3. We look under 'PARTS:' for gates, and run functions to link them together with wires.
	if(!isWord("PARTS:", s, i))
		syntaxError(Ppos.line, i);
	i += 7;
	skipWhitespace(s, Ppos);
	//Here we start listing gates and what they link to....
	bool partsListed = 0;
	while(partsListed == 0)
	{
			Gate* gate;
			bool isNotGate = 0;
			if(isWord("OR", s, i))
			{
				i+=2;
				gate = new GateOR();
			}
			if(isWord("AND", s, i))
			{
				i+=3;
				gate = new GateAND();
			}
			if(isWord("NOT", s, i))
			{
				i+=3;
				gate = new GateNOT();
				isNotGate = 1;
			}

			if(s[i] != ' ')
				syntaxError(Ppos.line, i);
			i++;
			std::string name;
			i += readWord(s, i, name);
			//remember to detect a syntax error here.
			gates.insert(std::pair<std::string, Gate*>(name, gate));
			if(s[i] != ' ')
				syntaxError(Ppos.line, i);
			skipWhitespace(s, Ppos);
			if(s[i] != '{')
				syntaxError(Ppos.line, i);
			i++;
			bool linksListed = 0;
			while(linksListed == 0)
			{
				//first parameter- inputa. NOT gates are special case.
				i+= readWord(s, i, name);
				Iwires = wires.begin();
				Iwires = wires.find(name);
				if(Iwires != wires.end())
				{
					gate->inputa = Iwires->second;
					gate->inputa->addOutput(gate);
				}
				Igates = gates.begin();
				Igates = gates.find(name);
				if(Igates != gates.end())
				{
					if(Igates->second->outputWire->LinkedGate!=nullptr)
					{
						Wiring* inputwire1 = new Wiring(chip);
						gate->inputa = inputwire1;
						inputwire1->addOutput(gate);
						inputwire1->addInput(Igates->second->outputWire);
						Igates->second->outputWire->addOutput(inputwire1);
					}
					else{
						gate->inputa = Igates->second->outputWire;
						gate->inputa->addOutput(gate);

					}
				}

				if(s[i] == '}')
				{
					break;
				}

				if(s[i] != ',')
					syntaxError(Ppos.line, i);
				
				if(isNotGate == 0)
				{
					//second parameter- inputb
					i += 2;
					i+= readWord(s, i, name);
					Iwires = wires.begin();
					Iwires = wires.find(name);
					if(Iwires != wires.end())
					{
						gate->inputb = Iwires->second;
						gate->inputb->addOutput(gate);
					}
					Igates = gates.begin();
					Igates = gates.find(name);
					if(Igates != gates.end())
					{
						gate->inputb = Igates->second->outputWire;
						gate->inputb->addOutput(gate);
					}

					if(s[i] == '}')
					{
						Wiring* outputWire = new Wiring(chip);
						gate->outputWire = outputWire;
						outputWire->addInput(gate);
						break;
					}

					if(s[i] != ',')
						syntaxError(Ppos.line, i);
				}
				//last parameter- outputWire. This is only made use of when the gate outputs to an OUT wiring.
				i += 2;
				i+= readWord(s, i, name);
				Iwires = wires.begin();
				Iwires = wires.find(name);
				if(Iwires != wires.end())
				{
					gate->outputWire = Iwires->second;
					gate->outputWire->addInput(gate);
					if(s[i] == '}')
						break;
					skipWhitespace(s, Ppos);
					if(s[i] == '}')
						break;
					syntaxError(Ppos.line, i);
				}
				linksListed = 1;
			}
			i++;
			if(s[i] != ';')
				syntaxError(Ppos.line, i);
			i++;
			skipWhitespace(s, Ppos);
			if(s[i] == '}')
				partsListed = 1;
	}	
//4.Finally, push all these maps into the chip.

	Iwires = wires.begin();
	Igates = gates.begin();
	while(Iwires != wires.end())
	{
		chip -> PushComponent(Iwires->second);	
		Iwires++;
	}
	while(Igates != gates.end())
	{
		chip -> PushComponent(Igates->second);	
		Igates++;
	}
	
	std::cout << "\n Parsing parts completed successfully!";
}

void Hardware::parseInclude(char * s, counter &Ppos, Session * session)
{
	size_t& i = Ppos.pos;
	i+=8;
	if(s[i] !=' ')
		syntaxError(Ppos.line, i);
	i++;
	std::string file;
	i+=readWord(s, i, file);
	HDL newhdl = loadHDL(file);
	parseHDL(newhdl, session, 1);
	if(s[i] != ';')
		syntaxError(Ppos.line, i);
	i++;
	skipWhitespace(s, Ppos);
}

bool Hardware::isLetter(char c)
{
	return (c > 64 && c <91)||(c > 96 && c < 123);	//checks if a character is an uppercase or lowercase letter
}

bool Hardware::isNumber(char c)
{
	return (c > 47 && c < 58);
}
bool Hardware::isWord(std::string w, char * c, size_t pos)
{
	for(int i = 0; i < w.length(); i++)
	{
		if(w[i] != c[pos + i])
			return 0;
	}
	return 1;
//When parsing you may or may not then want to move the pos forward, do it outside this function.
}

int Hardware::readWord(char * c, size_t pos, std::string& s)
{
	std::string s2;
	int i = 0;
	
	while(isLetter(c[pos+i])|| isNumber(c[pos+i]))
	{
		
		if(i<32)
		{
			s2.push_back(c[pos+i]);
			i++;
			continue;
		}
		break;
		//insert error here
	}
	s = s2;
return i;
//When parsing you may or may not then want to move the pos forward, do it outside this function.
}

void Hardware::skipWhitespace(char * c, counter &Ppos)
{
	size_t& i = Ppos.pos;

	while(c[i] == ' ' || c[i] == 10 || c[i] == 13 || c[i] == 9) 	
	{
		if(c[i] == 10)
		{
			Ppos.line++;
		}
		i++;
	}
	return;
}

void Hardware::syntaxError(size_t line, size_t pos)
{
	std::cout << "\nSyntax error at: line " << line+1 << "\n";
	exit(EXIT_FAILURE);
}

