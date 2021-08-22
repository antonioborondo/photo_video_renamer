// renamer.cpp : Defines the entry point for the application.
//

#include "renamer.h"

#include <filesystem>

using namespace std;

int main()
{
	cout << std::filesystem::current_path() << endl;
	
	return 0;
}
