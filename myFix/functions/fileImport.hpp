#ifndef functions_file_import_hpp
#define functions_file_import_hpp

#include <fstream>

#include <boost/timer.hpp>

#define PATH "C://Temp/"
#define FILE "XCME_MD_ES_20140303_20140307"

// FAST file import function
// field 1128 with value 9 means FIX50SP2
// field 35 with value X means MarketDataIncrementalRefresh message
// structure:
// 
// Component Block - <MDIncGrp>	Y	Number of entries following.
// 75	TradeDate

void fileImport() {

	// create the data dictionary
	boost::timer t;

	//open file
	std::ifstream infile(
		std::string(PATH).append(FILE));

	// get through the lines
	int nline = 1;
	std::string line;
	while (std::getline(infile, line)) {

		std::istringstream iss(line);
		std::string a;
		if (!(iss >> a)) { break; }

		if (nline++ % 1000 == 0) {

			std::cout
				<< "read "
				<< nline - 1
				<< " lines."
				<< std::endl;

			std::cout << line << std::endl;

		}
	}

	std::cout 
		<< "file read in "
		<< t.elapsed() 
		<< " seconds" 
		<< std::endl;

}

#endif