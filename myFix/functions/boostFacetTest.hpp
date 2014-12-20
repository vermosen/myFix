#ifndef functions_facet_test_hpp
#define functions_facet_test_hpp

#include <fstream>

#include <boost/timer.hpp>

#include <thOth/time/DateTime.hpp>

#include "utilities/settings/settings.hpp"


// a test devoted to look at date 
// conversion using std::locale and facets
void boostFacetTest() {

	boost::timer t;										// timer

	try {

		// facet test
		std::stringstream ss1;
		boost::posix_time::time_input_facet * facet1									// create the facet
			= new boost::posix_time::time_input_facet("%Y%m%d%H%M%s");
		ss1.imbue(std::locale(std::locale(), facet1));
		std::string strDt = "20021112000000123";
		thOth::dateTime dt1 = thOth::dateTime::strToDate(strDt, ss1);
		dt1 += thOth::dateTime::milliSeconds(boost::lexical_cast<int>(strDt.substr(14, 3)));
		std::cout << dt1 << std::endl;

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
		<< "test executed in "
		<< t.elapsed()
		<< " seconds"
		<< std::endl;

	system("pause");

}

#endif