#ifndef BITSTREAM
#define BITSTREAM
#include <map>
#include <fstream>
#include <string>

class Wiring;
class Gate;
class Chip;
class Session;

class Program 	//An object to load a stream of bits from a file and hold it in bitstream.
{
private:
	std::ifstream prog;
	std::streampos size;
	char * bitstream;
public:
	~Program();

	void loadprogram(std::string file);
	char& getbitstream();
};

namespace Hardware 	//A namespace to hold functions and structures that revolve around
					//Loading, parsing and instantiating virtual hardware
					//from one or more HDL files at a time.
{
	struct HDL 	//The HDL after it's loaded in and ready to parse.
	{
		char* string;
		size_t length;
		size_t tellg;	//where we are in the char array at any point in the program. Set to i when we leave the main loop for e.g declareChip() function.

	};
	
	struct counter
	{
		size_t line;
		size_t pos;
	};
	
	HDL loadHDL(char* file);	//reads a file and returns a HDL object.
	HDL loadHDL(std::string file);	//reads a file and returns a HDL object.

//------------------------HDL PARSING---------------------------//
	void parseHDL(HDL hdl, Session* session, bool cat);	//After creating a HDL object, this function
								//Parses it, running the relevant functions
								//to create the virtual hardware.
//parsing group of functions. Each of these attend to different types of objects and are called from parseHDL();
//They interpret their respective HDL sections, create the relevant object(s), link them together locally, 
//then push them into their appropriate Vectors.
	void parseCHIP(char * s, counter &Ppos, Session * session, bool cat);
	void parsePARTS(char * s,  counter &Ppos, Chip * chip);
	void parseInclude(char * s, counter &Ppos, Session * session);


//Here follow parsing utilities to be called from anywhere//
	bool isLetter(char c); 
	bool isNumber(char c);
	bool isWord(std::string w, char * c, size_t pos); //You pass in the string you want to check for, and a pointer to a position in the hdl char array from parseHDL(...)
	int readWord(char * c, size_t pos, std::string &s); //Reads chars up until a non-letter character and puts them in a string '&s'. Returns the length of the word.
	void skipWhitespace(char * c, counter &Ppos); //Input the index in the c-style string and it'll set it to the next index containing a character, skipping over whitespace.
	void declareChip(std::string name);
	void syntaxError(size_t line, size_t pos);
//-----------------------------------------------------------------//
}

#endif
