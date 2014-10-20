/*
 *
 * custom TwsApi
 * Copyright (C) 2014 Jean-Mathieu Vermosen
 *
 */

#include "utilities/settings/settings.hpp"

namespace myFix {

	settings::settings()						// default values
		: verbosity_(0),
		port_    (DB_PORT    ),
		server_  (DB_SERVER  ), 
		user_    (DB_USER    ),
		password_(DB_PASSWORD),
		dataBase_(DB_ID      ),
		log_     (LOGPATH    ) {

												// null pointer, safe ?
			dictionary_ = new FIX::DataDictionary(DICT);

			connect_ = mysql_init(NULL);		// initialize a null connection

			if (!connect_)						// fails to initialize mySQL
				throw std::exception("mySQL initialization failed");

			connect_ = mysql_real_connect(		// open a new connection to the database
				connect_,
				server().c_str(),
				user().c_str(),
				password().c_str(),
				dataBase().c_str(),
				port(),
				NULL, 0);

			if (!connect_)						// fails to initialize mySQL
				throw std::exception("unable to reach mySQL database");
		
		}

	void settings::updateConnection() {
	
		// need to lock guard all the components
		boost::lock_guard<boost::mutex> guard(connectionMutex_);

		if (connect_) {							// need to delete the previous connection
			
			mysql_close(connect_);				// close the previous connection
			
			connect_ = mysql_init(NULL);		// reset the MYSQL handle

			connect_ = mysql_real_connect(		// open a new connection
				connect_,
				server().c_str(),
				user().c_str(),
				password().c_str(),
				dataBase().c_str(),
				port(),
				NULL, 0);

		}

		if (!connect_)							// check
			throw std::exception("unable to reach mySQL database");

	}

	void settings::verbosity(const unsigned int v) {
		
		boost::lock_guard<boost::mutex> guard(verbosityMutex_);

		verbosity_ = v;
	
	}

	unsigned int settings::verbosity() const {
	
		boost::lock_guard<boost::mutex> guard(verbosityMutex_);

		return verbosity_; 
	
	}

	void settings::port(const unsigned int v) {

		{

			boost::lock_guard<boost::mutex> guard(portMutex_);

			port_ = v;

		}

		updateConnection();						// update the connection

	}

	unsigned int settings::port() const {


		boost::lock_guard<boost::mutex> guard(portMutex_);

		return port_;

	}

	void settings::server(const std::string & s) {

			{

				boost::lock_guard<boost::mutex> guard(serverMutex_);

				server_ = s;

			}

		updateConnection();						// update the connection

	}

	std::string settings::server() const {

		boost::lock_guard<boost::mutex> guard(serverMutex_);

		return server_;

	}

	void settings::user(const std::string & s) {

		{

			boost::lock_guard<boost::mutex> guard(userMutex_);

			user_ = s;

		}

		updateConnection();						// update the connection

	}

	std::string settings::user() const {

		boost::lock_guard<boost::mutex> guard(userMutex_);

		return user_;

	}

	void settings::password(const std::string & s) {

		{

			boost::lock_guard<boost::mutex> guard(passwordMutex_);

			password_ = s; 
			
		}

		updateConnection();						// update the connection

	}

	std::string settings::password() const {

		boost::lock_guard<boost::mutex> guard(passwordMutex_);

		return password_;

	}

	void settings::dataBase(const std::string & s) {

		{
			
			boost::lock_guard<boost::mutex> guard(dataBaseMutex_);

			dataBase_ = s;
				
		}
		
		updateConnection();						// update the connection

	}

	std::string settings::dataBase() const {

		boost::lock_guard<boost::mutex> guard(dataBaseMutex_);

		return dataBase_;

	}

	void settings::dictionary(const std::string & s) {
	
		boost::lock_guard<boost::mutex> guard(dictionaryMutex_);

		delete dictionary_;
		dictionary_ = new FIX::DataDictionary(s);// update the pointer
	
	}

}