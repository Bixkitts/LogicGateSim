#ifndef BITSTREAM
#define BITSTREAM

#include <fstream>
#include <string>

class Chip;

class Program
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

namespace bitstream
{
	struct HDL
	{
		const char* string;
		const size_t length;
		static size_t tellg;	//where we are in the char array at any point in the program. Set to i when we leave the main loop for e.g declareChip() function.
	};

	HDL loadHDL(std::string file);	//reads a file into a char array
	Chip* parseHDL(HDL hdl);	//loops through the length of the hdl file looking for chip declerations and definitions

	bool isLetter(char c); //ammends characters into a string until whitespace is met
	void declareChip(std::string name);
}

#endif