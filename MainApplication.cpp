//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ozgun Bas
//Student number: b8037359

#include <iostream>

#include "DatabaseManager.h"
#include "MenuSystem.h"

// Entry point for the system
int main(int argc, char** argv)
{
	// Let the database load all the data.
	DatabaseManager::instance().load_data();
	return MenuSystem::instance().run();
}
