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
#include <quickfix/FileStore.h>
#include <quickfix/ThreadedSocketAcceptor.h>
#include <quickfix/Log.h>
#include <quickfix/SessionSettings.h>

#include "utilities/settings/settings.hpp"
#include "functions/all.hpp"

#define PATH     "C://Temp/dropbox/"
#define SETTINGS "C://Users/vermosen/Documents/GitHub/myFix/myFix/settings.txt"

//#define DATA "XCEC_MD_GC_20140324_20140328" -> done
#define DATA "XCME_MD_SMC_20140324_20140328"

int main(int argc, char** argv) {

	int exit;														// exit code

	try {
		
		bool end = false;											// main exit indicator 
		int test = 0    ;											// optional test
		std::string data(DATA);										// default to data

		for (int i = 1; i < argc; i++) {							// deals with optional arguments

			std::string arg(argv[i]);								// current std::string

			if (arg.substr(1, 4) == "test") {						// expects -test=n

				std::string str(arg.substr(6, arg.length() - 6));	// the value
				test = boost::lexical_cast<int>(str);				// runs the selected test automatically	
				end = true;											// for later use: only one attempt

			}

			if (arg.substr(1, 4) == "file") {						// expects -test=n

				data.clear();										// reset the data file name
				data = arg.substr(6, arg.length() - 6);				// the value

			}
		}

		do {														// manual selection - loop over the choices

			int res = test;											// for manual definition

			if (test == 0) {

				std::cout											// message
					<< std::endl << "Please select an activity: "
					<< std::endl
					<< std::endl << "####### imports #######"
					<< std::endl << "1 - instrument   import" 
					<< std::endl << "2 - trade        import"
					<< std::endl << "3 - market order import"
					<< std::endl
					<< std::endl << "##### computation #####"
					<< std::endl << "4 - bar computations   "
					<< std::endl
					<< std::endl << "#### unitary tests ####"
					<< std::endl << "5 - debug test"
					<< std::endl << "6 - date facet test		"
					<< std::endl << "7 - single trade insert"
					<< std::endl << "8 - simple trading rule"
					<< std::endl
					<< std::endl << "#### Miscellaneous ####"
					<< std::endl << "0 - exit"
					<< std::endl 
					<< std::endl;

				std::cin >> res;									// user defined test

			}

			switch (res) {											// switch over the available tests

			case 1:

				instrumentImport(std::string(PATH).append(data));
				break;

			case 2:

				tradeImport(std::string(PATH).append(data));		// path to the data file
				break;

			case 3:

				marketOrderImport(std::string(PATH).append(data));	// path to the data file
				break;

			case 4:

				barConversion(										// contract selection
					thOth::instrument(208, "GCJ4"),
					thOth::dateTime(2014, 3, 2) + boost::posix_time::hours(23),
					thOth::dateTime(2014, 4, 5),
					thOth::period(thOth::timeUnit::milliSecond, 100));

				break;

			case 5:

				debug(std::string(PATH).append(data));				// path to the data file
				break;

			case 6:

				boostFacetTest();									// path to the data file
				break;

			case 7:

				singleTradeInsert();								// single insert test
				break;

			case 8:

				simpleTradingRule();								// trading rule test
				break;

			case 0:

				end = true;											// stop the server
				break;

			default:												// unknown, invalid

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
		
		exit = 1; system("pause");

	}
	catch (...) {													// unknown error
				
		std::cout 
			<< "an unknown error occured..." 
			<< std::endl;

		exit = 1; system("pause");

	}

	return exit;

}