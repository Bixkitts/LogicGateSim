#include "Bitstream.h"
#include "Session.h"
#include "Chip.h"
#include <iostream>

bool Hardware::isLetter(char c)
{
	return (c > 64 && c <91)||(c > 96 && c < 123);	//checks if a character is an uppercase or lowercase letter
}

bool Hardware::isNumber(char c)
{
	return (c > 47 && c < 58);
}
bool Hardware::isWord(std::string w, HDL &hdl)
{
	char * c = hdl.string;
	size_t& pos = hdl.Ppos.pos;

	for(int i = 0; i < w.length(); i++)
	{
		if(w[i] != c[pos + i])
			return 0;
	}
	return 1;
//When parsing you may or may not then want to move the pos forward, do it outside this function.
}

int Hardware::readWord(HDL &hdl, std::string& s)
{
	std::string s2;
	size_t& pos = hdl.Ppos.pos;
	char * c = hdl.string;
	int i = 0;
	
	while(isLetter(c[pos+i])|| isNumber(c[pos+i]) || c[pos+i] == '.')
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

void Hardware::skipWhitespace(HDL &hdl)
{
	char * c = hdl.string;
	size_t& i = hdl.Ppos.pos;

	while(c[i] == ' ' || c[i] == 10 || c[i] == 13 || c[i] == 9) 	
	{
		if(c[i] == 10)
		{
		hdl.Ppos.line++;
		}
		i++;
	}
	return;
}

void Hardware::syntaxError(HDL &hdl)
{
	std::cout << "\nSyntax error at: line " << hdl.Ppos.line+1 << "\n";
	exit(EXIT_FAILURE);
}

std::string Hardware::extractFromDelimiter(std::string &s, bool right)
{
	std::string Ret;
	if(right == 0)
	{
		for(char const &c: s)
		{
			if(c != '.')
				Ret.push_back(c);	
		}
		return Ret;
	}

	int x = 0;
	while(x < s.length())
	{
		if(s[x] == '.')
		{
			x++;
			break;
		}
		x++;
	}
	while(x < s.length())
	{
		Ret.push_back(s[x]);
		x++;	
	}
	return Ret;
}

Wiring* Hardware::parsePartParam(std::string &s, Session* session)
{
	Chip* chip = session->chips[0];
	Wiring* iParam = chip->searchWarray(s);
	if(iParam == nullptr)
		iParam = chip->searchGarray(s)->outputs[0];
	if(iParam == nullptr)
	{
		std::string chipname = extractFromDelimiter(s, 0);
		std::string wirename = extractFromDelimiter(s, 1);
		for(int y = 1; y<session->ChipCount; y++)
		{
			if(session->chips[y]->name == chipname)
				iParam = session->chips[y]->searchWarray(wirename);
		}

	}
	if(iParam == nullptr)
	{
		std::cout<<"\nERROR: bad parameter '" << s << "'. \nTerminating process...";
		exit(EXIT_FAILURE);
	}
	return iParam;
}
