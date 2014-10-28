#ifndef functions_debug_hpp
#define functions_debug_hpp

#include <fstream>

#include <boost/timer.hpp>

#include <thOth/time/DateTime.hpp>

#include "utilities/settings/settings.hpp"
#include "parser/parser.hpp"
#include "recordset/tableTradeRecordset/tableTradeRecordset.hpp"

// test an insert on the database
void debug(const std::string & data_) {

	boost::timer t;										// timer

	try {
	
		// declare parser
		//myFix::parser parser(								// create the file parser
		//	myFix::settings::instance().dictionary());

		// add the requested symbols
		//parser.addSymbol(std::pair<myFix::dataBase::recordId, std::string>(1, "TOTO"));

		myFix::dataBase::tableTradeRecordset(			// recordset connect to the database
			myFix::settings::instance().connection());

		//myFix::tradeMessage t = {};

		//std::string		symbol_ = ""	;		// symbol name (FIX field 107)
		//thOth::dateTime time_			;		// quote time
		//double          price_ = .0		;		// price
		//int             quantity_ = 0	;

		

	}
	catch (std::exception & e) {
	
		std::cout << "an exception occured: " 
				  << std::endl
				  << e.what()
				  << std::endl;
	
	}
	catch (...) {
	
		std::cout << "unknown exception occured" << std::endl;
	
	}
	
	std::cout
		<< "file read in "
		<< t.elapsed()
		<< " seconds"
		<< std::endl;

	system("pause");

}

#endif