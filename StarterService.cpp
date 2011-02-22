static const char *RcsId = "$Header$";
//+=============================================================================
//
// file :         Starter.cpp
//
// description :  C++ source for the Starter if used has windows service.
//
// project :      TANGO Device Server
//
// $Author$
//
// $Revision$
//
// $Log$
// Revision 3.7  2007/09/27 15:12:01  pascal_verdier
// Get TANGO_HOST from registry and add it to environement.
//
// Revision 3.6  2004/09/28 07:13:19  pascal_verdier
// bug on state with notify daemon fixed.
//
// Revision 3.5  2004/07/05 11:01:20  pascal_verdier
// PB on service fixed.
//
// Revision 3.4  2004/06/29 04:24:26  pascal_verdier
// First revision using events.
//
//
// copyleft :     European Synchrotron Radiation Facility
//                BP 220, Grenoble 38043
//                FRANCE
//
//-=============================================================================


#ifdef WIN32

#include <tango.h>
#include <StarterService.h>
#include <Starter.h>



/**
 *	The NT service class
 */

using namespace std;



//+------------------------------------------------------------------
/**
 *	The service constructor
 */
//+------------------------------------------------------------------
StarterService::StarterService(char *exe_name):NTService(exe_name)
{
}
//+------------------------------------------------------------------
/**
 *	Start the NT Service
 */
//+------------------------------------------------------------------
void StarterService::start(int argc, char **argv, Tango::NTEventLogger *log)
{
	log->info("Starting start method");

	Tango::Util	*tg;
	Tango::Util::_daemon = true;
	Tango::Util::_sleep_between_connect = 5;

	log->info("Wait database done");

	try
	{
		// Initialise logger
		//----------------------------------------
		Tango::Util::_service = true;
		log->info("Tango::Util::_service = true;");
 
		// Initialise the device server
		//----------------------------------------
		tg = Tango::Util::init(argc, argv);
		log->info("tg = Tango::Util::init(argc, argv);");

		// Create the device server singleton 
		//	which will create everything
		//----------------------------------------
		tg->server_init();
		log->info("tg->server_init();");


		//	Set TANGO_HOST  gotten from registry 
		//	to environement for sterted servers
		//----------------------------------------
		char	*tango_host;
		Tango::Database	*db = tg->get_database();
		db->get_tango_host_from_reg(&tango_host, tg->get_ds_exec_name(), tg->get_ds_inst_name());
		string	s("TANGO_HOST=");
		s += (tango_host==NULL)? "NULL" : tango_host;
		log->info(s.c_str());

		_putenv(s.c_str());
		delete tango_host;

		// Run the endless loop
		//----------------------------------------
		tg->server_run();
	}
	catch(bad_alloc) {
		logger_->error("Cannot allocate memory to store device object");
	}
	catch(Tango::DevFailed &e) {
		logger_->error(e.errors[0].desc.in());
	}
	catch(CORBA::Exception &) {
		logger_->error("CORBA Exception");
	}
}

#endif