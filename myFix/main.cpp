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

//#include "Application.hpp"
#include "utilities/settings/settings.hpp"
#include "functions/all.hpp"

#define PATH     "C://Temp/"
#define SETTINGS "C://Users/vermosen/Documents/GitHub/myFix/myFix/settings.txt"

// SPY
//#define DATA  "XCME_MD_ES_20140303_20140307" -> done
//#define DATA  "XCME_MD_ES_20140310_20140314" -> done
//#define DATA  "XCME_MD_ES_20140317_20140321" -> done
//#define DATA  "XCME_MD_ES_20140324_20140328" -> done
//#define DATA  "XCME_MD_ES_20140331_20140404" -> done

// NASDAQ
//#define DATA	"XCME_MD_NQ_20140303_20140307" -> done
//#define DATA	"XCME_MD_NQ_20140310_20140314" -> done
//#define DATA	"XCME_MD_NQ_20140317_20140321" -> done
//#define DATA	"XCME_MD_NQ_20140324_20140328" -> done
//#define DATA	"XCME_MD_NQ_20140331_20140404" -> done

// gold contracts
//#define DATA "XCEC_MD_GC_20140303_20140307" -> done
//#define DATA "XCEC_MD_GC_20140310_20140314" -> done
//#define DATA "XCEC_MD_GC_20140317_20140321" -> done
//#define DATA "XCEC_MD_GC_20140324_20140328" -> done
#define DATA "XCME_MD_NQ_20140331_20140404"

int main(int argc, char** argv) {

	int exit;														// exit code

	try {
		
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
					<< std::endl << "Please select an activity: "
					<< std::endl
					<< std::endl << "#### main commands ####"
					<< std::endl << "1 - instrument import  " 
					<< std::endl << "2 - trade      import  "
					<< std::endl << "3 - bar        import  "
					<< std::endl
					<< std::endl << "#### unitary tests ####"
					<< std::endl << "4 - debug"
					<< std::endl << "5 - date facet"
					<< std::endl << "6 - single trade insert"
					<< std::endl
					<< std::endl << "#### miscelanious  ####"
					<< std::endl << "0 - exit"
					<< std::endl 
					<< std::endl;

				std::cin >> res;									// user defined test

			}

			switch (res) {											// switch over the available tests

			case 1:

				instrumentImport(std::string(PATH).append(DATA));
				break;

			case 2:

				tradeImport(std::string(PATH).append(DATA));		// path to the data file
				break;

			case 3:

				barImport(											// contract selection
					thOth::instrument(208, "GCJ4"),
					thOth::dateTime(2014, 3, 2) + boost::posix_time::hours(23),
					thOth::dateTime(2014, 4, 5),
					thOth::period(thOth::timeUnit::milliSecond, 100));

				break;

			case 4:

				debug(std::string(PATH).append(DATA));				// path to the data file
				break;

			case 5:

				boostFacetTest();									// path to the data file
				break;

			case 6:

				singleTradeInsert();								// single insert test
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