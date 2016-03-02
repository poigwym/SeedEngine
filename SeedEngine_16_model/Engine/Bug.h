#pragma once

#define BUG

#include<fstream>
#include<string>
#include<iostream>
using std::ofstream;
using std::string;
using std::endl;


namespace Seed
{
	extern ofstream LogOut;


	inline void Log(const string &str)
	{
#ifdef BUG
		LogOut << str << endl;

#endif
	}
}