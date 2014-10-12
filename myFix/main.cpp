/*
*
* myFix project
* Copyright (C) 2014 Jean-Mathieu Vermosen
*
*/

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 )
#endif

#include <string>
#include <iostream>
#include <fstream>

#include <boost/lexical_cast.hpp>

#include "quickfix/FileStore.h"
#include "quickfix/ThreadedSocketAcceptor.h"
#include "quickfix/Log.h"
#include "quickfix/SessionSettings.h"

#include "quickfix/MySQLLog.h"

void myFunc() {};

int main(int argc, char** argv) {

	int exit;														// exit code

	try {

		// some operations...
		std::string file = "C:/temp/test.txt";
		FIX::SessionSettings settings      (file    );				// settings
		FIX::FileStoreFactory storeFactory (settings);				// store factory
		FIX::ScreenLogFactory logFactory   (settings);				// log factory

		FIX::Log * log = logFactory.create();

		bool end = false;											// main exit indicator 
		int test = 0    ;											// optional test

		for (int i = 1; i < argc; i++) {							// deals with optional arguments

			std::string arg(argv[i]);								// current std::string

			if (arg.substr(1, 4) == "test") {						// expects -test=n

				std::string str(arg.substr(6, arg.length() - 6));	// the value
				test = boost::lexical_cast<int>(str);				// runs the selected test automatically	
				end = true;											// for later use: only one attempt

			}
		}

		do {														// manual selection - loop over the choices

			int res = test;											// for manual definition

			if (test == 0) {

				std::cout											// message
					<< std::endl
					<< "Please select an activity: "
					<< std::endl
					<< "1 - CME file parsing"
					<< std::endl
					<< "0 - exit"
					<< std::endl
					<< std::endl;

				std::cin >> res;									// user defined test

			}

			switch (res) {											// switch over the tests available

			case 1:

				myFunc();
				break;

			case 0:

				end = true;										// stop the server
				break;

			default:											// unknown, invalid

				std::cout
					<< "invalid selection, please try again"
					<< std::endl;

				break;

			}

		} while (end == false);										// loop until exit

		exit = 0;

	}
	catch (std::exception & e) {									// exception management

		std::cout 
			<< "an error occured: " 
			<< e.what() 
			<< std::endl;
		exit = 1;
		system("pause");

	}
	catch (...) {													// unknown error
				
		std::cout 
			<< "an unknown error occured..." 
			<< std::endl;
		exit = 1;
		system("pause");

	}

	return exit;

}