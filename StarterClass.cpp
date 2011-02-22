static const char *ClassId    = "$Id$";
static const char *CvsPath    = "$Source$";
static const char *SvnPath    = "$HeadURL: $";
static const char *RcsId     = "$Header$";
static const char *TagName   = "$Name$";
static const char *HttpServer= "http://www.esrf.fr/computing/cs/tango/tango_doc/ds_doc/";
//+=============================================================================
//
// file :        StarterClass.cpp
//
// description : C++ source for the StarterClass. A singleton
//               class derived from Tango::DeviceClass. It implements the
//               command list and all properties and methods required
//               by the Starter once per process.
//
// project :     TANGO Device Server
//
// $Author$
//
// $Revision$
//
// $Log$
// Revision 3.26  2008/12/12 13:29:56  pascal_verdier
// Log in file start and stop for servers and itself.
//
// Revision 3.25  2008/09/23 14:19:40  pascal_verdier
// Log files history added.
//
// Revision 3.24  2008/06/18 08:17:02  pascal_verdier
// Pb with case unsensitive on win32 fixed.
//
// Revision 3.23  2008/05/15 08:07:18  pascal_verdier
// TangoSys_MemStream replaced by TangoSys_OMemStream
// (for leaking problem under win32)
//
// Revision 3.22  2008/04/09 14:46:11  pascal_verdier
// Bug in init of NotifdState attribute fixed.
//
// Revision 3.21  2008/04/07 13:10:27  pascal_verdier
// New pogo property management.
//
// Revision 3.20  2008/04/07 08:54:55  pascal_verdier
// Check if this starter instance is able to run on this host.
//
// Revision 3.19  2008/02/29 15:15:05  pascal_verdier
// Checking running processes by system call added.
//
// Revision 3.18  2007/02/13 06:45:13  pascal_verdier
// *** empty log message ***
//
// Revision 3.17  2006/11/10 14:53:28  pascal_verdier
// Remove vc8 warnings.
//
// Revision 3.16  2006/06/13 19:38:15  pascal_verdier
// Minor changes.
//
// Revision 3.15  2006/06/06 12:01:26  pascal_verdier
// Bug in log file fixed.
//
// Revision 3.14  2006/06/05 07:20:36  pascal_verdier
// Server startup is now delayed (with timeout) in a startup level.
// New state MOVING added.
// At startup, starter loop until properties have been read.
//
// Revision 3.13  2006/05/15 11:11:07  pascal_verdier
// Tag Release added to const char *TagName.
//
// Revision 3.12  2006/04/24 08:58:10  pascal_verdier
// *** empty log message ***
//
// Revision 3.11  2006/04/24 07:06:28  pascal_verdier
// A thread is started for each level when at servers startup.
//
// Revision 3.10  2006/02/08 07:13:55  pascal_verdier
// Minor changes.
//
// Revision 3.9  2005/09/30 12:01:50  pascal_verdier
// RunningServers and StoppedServers spectrum attribute max_x set to 200.
//
// Revision 3.8  2005/09/16 09:24:23  pascal_verdier
// For backward compatibility, set UseEvents property to boolean was not a good idea.
//
// Revision 3.7  2005/09/13 14:36:10  pascal_verdier
// Pogo-4.4.0 compatibility.
//
// Revision 3.6  2005/08/26 07:18:16  pascal_verdier
// FQDN management added.
// Time between startup level added.
//
// Revision 3.5  2005/04/21 07:18:48  pascal_verdier
// Add a little timeout for ping.
// Fix a bug for windows (SIGCHLD).
//
// Revision 3.4  2004/12/10 08:57:19  pascal_verdier
// Tango 5 compatibility (attribute management).
//
// Revision 3.3  2004/06/29 04:24:26  pascal_verdier
// First revision using events.
//
// Revision 3.2  2004/05/19 08:56:32  pascal_verdier
// Bug fixed on service mode.
// CreateProcess used under Windows.
//
// Revision 3.0.1.3  2004/02/27 09:53:02  pascal_verdier
// - The starter device is now warned by Database server when something change on a server.
// It replace the DbGetHostServersInfo polling.
// - HostState, ControlledRunningServers, ontrolledStoppedServers attributes added.
//
// Revision 3.0.1.2  2003/12/08 08:53:52  pascal_verdier
// Cluster (multi-host) control implemented.
// Control of notify daemon implemented but not tested.
//
// Revision 3.0.1.1  2003/09/18 12:02:49  pascal_verdier
// Problem on Windows service startup fixed.
//
// Revision 3.0  2003/06/17 12:06:36  pascal_verdier
// TANGO 3.x server.
// polling bugs fixed.
//
// Revision 2.0  2003/01/09 13:35:50  verdier
// TANGO 2.2
//
// Revision 1.22  2002/12/18 08:09:19  verdier
// omniORB compatibility
//
// Revision 1.6  2002/10/15 18:55:21  verdier
// The host state is now calculated during the State polled command.
//
// Revision 1.5  2002/02/27 15:19:32  verdier
// WinNt services implemented.
//
// Revision 1.4  2001/04/03 09:40:50  verdier
// WInNt version is running.
//
// Revision 1.3  2001/02/12 09:34:21  verdier
// SunOS and Linux OK.
//
// Revision 1.2  2000/10/12 08:54:13  verdier
// Compatible with tango 2 done
//  Update from database info added.
//
// Revision 1.1  2000/07/31 13:48:42  verdier
// Initial revision
//
//
// copyleft :   European Synchrotron Radiation Facility
//              BP 220, Grenoble 38043
//              FRANCE
//
//-=============================================================================
//
//  		This file is generated by POGO
//	(Program Obviously used to Generate tango Object)
//
//             (c) - Pascal Verdier - ESRF
//=============================================================================


#include <tango.h>

#include <Starter.h>
#include <StarterClass.h>

namespace Starter_ns
{
//+----------------------------------------------------------------------------
//
// method : 		HardKillServerClass::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be executed
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *HardKillServerClass::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "HardKillServerClass::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	((static_cast<Starter *>(device))->hard_kill_server(argin));
	return new CORBA::Any();
}

//+----------------------------------------------------------------------------
//
// method : 		NotifyDaemonStateCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *NotifyDaemonStateCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "NotifyDaemonStateCmd::execute(): arrived" << endl;

	return insert((static_cast<Starter *>(device))->notify_daemon_state());
}



//+----------------------------------------------------------------------------
//
// method : 		UpdateServersInfoCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *UpdateServersInfoCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "UpdateServersInfoCmd::execute(): arrived" << endl;

	((static_cast<Starter *>(device))->update_servers_info());
	return new CORBA::Any();
}


//+----------------------------------------------------------------------------
//
// method : 		DevReadLogCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DevReadLogCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DevReadLogCmd::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	return insert((static_cast<Starter *>(device))->dev_read_log(argin));
}

//+----------------------------------------------------------------------------
//
// method : 		DevStopAllCmd::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DevStopAllCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DevStopAllCmd::execute(): arrived" << endl;

	Tango::DevShort	argin;
	extract(in_any, argin);

	((static_cast<Starter *>(device))->dev_stop_all(argin));
	return new CORBA::Any();
}


//+----------------------------------------------------------------------------
//
// method : 		DevStartAllCmd::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DevStartAllCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DevStartAllCmd::execute(): arrived" << endl;

	Tango::DevShort	argin;
	extract(in_any, argin);

	((static_cast<Starter *>(device))->dev_start_all(argin));
	return new CORBA::Any();
}


//+----------------------------------------------------------------------------
//
// method : 		DevGetStopServersCmd::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DevGetStopServersCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DevGetStopServersCmd::execute(): arrived" << endl;

	Tango::DevBoolean	argin;
	extract(in_any, argin);

	return insert((static_cast<Starter *>(device))->dev_get_stop_servers(argin));
}


//+----------------------------------------------------------------------------
//
// method : 		DevGetRunningServerCmd::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DevGetRunningServersCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DevGetRunningServersCmd::execute(): arrived" << endl;

	Tango::DevBoolean	argin;
	extract(in_any, argin);

	return insert((static_cast<Starter *>(device))->dev_get_running_servers(argin));
}


//+----------------------------------------------------------------------------
//
// method : 		DevStartCmd::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DevStartCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DevStartCmd::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	((static_cast<Starter *>(device))->dev_start(argin));
	return new CORBA::Any();
}


//+----------------------------------------------------------------------------
//
// method : 		DevStopCmd::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DevStopCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DevStopCmd::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	((static_cast<Starter *>(device))->dev_stop(argin));
	return new CORBA::Any();
}

//
//----------------------------------------------------------------
//	Initialize pointer for singleton pattern
//----------------------------------------------------------------
//
StarterClass *StarterClass::_instance = NULL;

//+----------------------------------------------------------------------------
//
// method : 		StarterClass::StarterClass(string &s)
// 
// description : 	constructor for the StarterClass
//
// in : - s : The class name
//
//-----------------------------------------------------------------------------
StarterClass::StarterClass(string &s):Tango::DeviceClass(s)
{

	cout2 << "Entering StarterClass constructor" << endl;
	set_default_property();
	write_class_property();

	get_class_property();

	cout2 << "Leaving StarterClass constructor" << endl;
}

//+----------------------------------------------------------------------------
//
// method : 		StarterClass::~StarterClass()
// 
// description : 	destructor for the StarterClass class
//
//-----------------------------------------------------------------------------
StarterClass::~StarterClass()
{
	_instance = NULL;
}

//+----------------------------------------------------------------------------
//
// method : 		StarterClass::instance
// 
// description : 	Create the object if not already done. Otherwise, just
//			return a pointer to the object
//
// in : - name : The class name
//
//-----------------------------------------------------------------------------
StarterClass *StarterClass::init(const char *name)
{
	if (_instance == NULL)
	{
		try
		{
			string s(name);
			_instance = new StarterClass(s);
		}
		catch (bad_alloc)
		{
			throw;
		}		
	}		
	return _instance;
}

StarterClass *StarterClass::instance()
{
	if (_instance == NULL)
	{
		cerr << "Class is not initialised !!" << endl;
		exit(-1);
	}
	return _instance;
}

//+----------------------------------------------------------------------------
//
// method : 		StarterClass::command_factory
// 
// description : 	Create the command object(s) and store them in the 
//			command list
//
//-----------------------------------------------------------------------------
void StarterClass::command_factory()
{
	command_list.push_back(new DevStartAllCmd("DevStartAll",
		Tango::DEV_SHORT, Tango::DEV_VOID,
		"Startup level.",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DevStopAllCmd("DevStopAll",
		Tango::DEV_SHORT, Tango::DEV_VOID,
		"Startup Level.",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DevGetRunningServersCmd("DevGetRunningServers",
		Tango::DEV_BOOLEAN, Tango::DEVVAR_STRINGARRAY,
		"True for all servers. False for controled servers only.",
		"List of the processes which are running.",
		Tango::OPERATOR));
	command_list.push_back(new DevGetStopServersCmd("DevGetStopServers",
		Tango::DEV_BOOLEAN, Tango::DEVVAR_STRINGARRAY,
		"True for all servers. False for controled servers only.",
		"List of the processes which are not running.",
		Tango::OPERATOR));
	command_list.push_back(new DevStartCmd("DevStart",
		Tango::DEV_STRING, Tango::DEV_VOID,
		"Server to be started.",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DevStopCmd("DevStop",
		Tango::DEV_STRING, Tango::DEV_VOID,
		"Servero be stopped.",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DevReadLogCmd("DevReadLog",
		Tango::DEV_STRING, Tango::DEV_STRING,    //  Tango::CONST_DEV_STRING
		"server name and domain",
		"ig Starter/corvus)",
		Tango::OPERATOR));
	command_list.push_back(new HardKillServerClass("HardKillServer",
		Tango::DEV_STRING, Tango::DEV_VOID,
		"Server name",
		"",
		Tango::OPERATOR));
	command_list.push_back(new NotifyDaemonStateCmd("NotifyDaemonState",
		Tango::DEV_VOID, Tango::DEV_STATE,
		"",
		"Tango::ON if Notify daemon is running else Tango::FAULT.",
		Tango::OPERATOR));
	command_list.push_back(new UpdateServersInfoCmd("UpdateServersInfo",
		Tango::DEV_VOID, Tango::DEV_VOID,
		"",
		"",
		Tango::OPERATOR));

	//	add polling if any
	for (unsigned int i=0 ; i<command_list.size(); i++)
	{
	}
}

//+----------------------------------------------------------------------------
//
// method : 		StarterClass::get_class_property
// 
// description : 	Get the class property for specified name.
//
// in :		string	name : The property name
//
//+----------------------------------------------------------------------------
Tango::DbDatum StarterClass::get_class_property(string &prop_name)
{
	for (unsigned int i=0 ; i<cl_prop.size() ; i++)
		if (cl_prop[i].name == prop_name)
			return cl_prop[i];
	//	if not found, return  an empty DbDatum
	return Tango::DbDatum(prop_name);
}
//+----------------------------------------------------------------------------
//
// method : 		StarterClass::get_default_device_property()
// 
// description : 	Return the default value for device property.
//
//-----------------------------------------------------------------------------
Tango::DbDatum StarterClass::get_default_device_property(string &prop_name)
{
	for (unsigned int i=0 ; i<dev_def_prop.size() ; i++)
		if (dev_def_prop[i].name == prop_name)
			return dev_def_prop[i];
	//	if not found, return  an empty DbDatum
	return Tango::DbDatum(prop_name);
}

//+----------------------------------------------------------------------------
//
// method : 		StarterClass::get_default_class_property()
// 
// description : 	Return the default value for class property.
//
//-----------------------------------------------------------------------------
Tango::DbDatum StarterClass::get_default_class_property(string &prop_name)
{
	for (unsigned int i=0 ; i<cl_def_prop.size() ; i++)
		if (cl_def_prop[i].name == prop_name)
			return cl_def_prop[i];
	//	if not found, return  an empty DbDatum
	return Tango::DbDatum(prop_name);
}
//+----------------------------------------------------------------------------
//
// method : 		StarterClass::device_factory
// 
// description : 	Create the device object(s) and store them in the 
//			device list
//
// in :		Tango::DevVarStringArray *devlist_ptr : The device name list
//
//-----------------------------------------------------------------------------
void StarterClass::device_factory(const Tango::DevVarStringArray *devlist_ptr)
{

	//	Create all devices.(Automatic code generation)
	//-------------------------------------------------------------
	for (unsigned long i=0 ; i < devlist_ptr->length() ; i++)
	{
		cout4 << "Device name : " << (*devlist_ptr)[i].in() << endl;
						
		// Create devices and add it into the device list
		//----------------------------------------------------
		device_list.push_back(new Starter(this, (*devlist_ptr)[i]));							 

		// Export device to the outside world
		// Check before if database used.
		//---------------------------------------------
		if ((Tango::Util::_UseDb == true) && (Tango::Util::_FileDb == false))
			export_device(device_list.back());
		else
			export_device(device_list.back(), (*devlist_ptr)[i]);
	}
	//	End of Automatic code generation
	//-------------------------------------------------------------

}
//+----------------------------------------------------------------------------
//	Method: StarterClass::attribute_factory(vector<Tango::Attr *> &att_list)
//-----------------------------------------------------------------------------
void StarterClass::attribute_factory(vector<Tango::Attr *> &att_list)
{
	//	Attribute : NotifdState
	NotifdStateAttrib	*notifd_state = new NotifdStateAttrib();
	Tango::UserDefaultAttrProp	notifd_state_prop;
	notifd_state_prop.set_label("Notifd State");
	notifd_state_prop.set_description("return ON or FAULT if notify daemon is running or not.");
	notifd_state->set_default_properties(notifd_state_prop);
	notifd_state->set_polling_period(1000);
	att_list.push_back(notifd_state);

	//	Attribute : HostState
	HostStateAttrib	*host_state = new HostStateAttrib();
	att_list.push_back(host_state);

	//	Attribute : RunningServers
	RunningServersAttrib	*running_servers = new RunningServersAttrib();
	running_servers->set_polling_period(1000);
	att_list.push_back(running_servers);

	//	Attribute : StoppedServers
	StoppedServersAttrib	*stopped_servers = new StoppedServersAttrib();
	Tango::UserDefaultAttrProp	stopped_servers_prop;
	stopped_servers_prop.set_label("All Stopped Servers");
	stopped_servers_prop.set_description("Return all the Stopped servers.\n");
	stopped_servers->set_default_properties(stopped_servers_prop);
	stopped_servers->set_polling_period(1000);
	att_list.push_back(stopped_servers);

	//	Attribute : Servers
	ServersAttrib	*servers = new ServersAttrib();
	Tango::UserDefaultAttrProp	servers_prop;
	servers_prop.set_label("Servers");
	servers_prop.set_description("Return all registred servers for this host.\nServer names are followed by their states and controls");
	servers->set_default_properties(servers_prop);
	servers->set_polling_period(1000);
	att_list.push_back(servers);

	//	End of Automatic code generation
	//-------------------------------------------------------------
}
//+----------------------------------------------------------------------------
//
// method : 		StarterClass::get_class_property
// 
// description : 	Get the class property from database.
//
//-----------------------------------------------------------------------------
void StarterClass::get_class_property()
{
	//	Initialize your default values here.
	//------------------------------------------
	readInfoDbPeriod  = 4;
	nbStartupLevels   = 5;
	cmdPollingTimeout = 60;
	useEvents = false;

	//	Read class properties from database.(Automatic code generation)
	//-------------------------------------------------------------
	cl_prop.push_back(Tango::DbDatum("ReadInfoDbPeriod"));
	cl_prop.push_back(Tango::DbDatum("NbStartupLevels"));
	cl_prop.push_back(Tango::DbDatum("CmdPollingTimeout"));
	cl_prop.push_back(Tango::DbDatum("UseEvents"));
	cl_prop.push_back(Tango::DbDatum("ServerStartupTimeout"));

	//	Call database and extract values
	//--------------------------------------------
	if (Tango::Util::instance()->_UseDb==true)
		get_db_class()->get_property(cl_prop);
	Tango::DbDatum	def_prop;
	int	i = -1;

	//	Try to extract ReadInfoDbPeriod value
	if (cl_prop[++i].is_empty()==false)	cl_prop[i]  >>  readInfoDbPeriod;
	else
	{
		//	Check default value for ReadInfoDbPeriod
		def_prop = get_default_class_property(cl_prop[i].name);
		if (def_prop.is_empty()==false)
		{
			def_prop  >>  readInfoDbPeriod;
			cl_prop[i]  <<  readInfoDbPeriod;
		}
	}

	//	Try to extract NbStartupLevels value
	if (cl_prop[++i].is_empty()==false)	cl_prop[i]  >>  nbStartupLevels;
	else
	{
		//	Check default value for NbStartupLevels
		def_prop = get_default_class_property(cl_prop[i].name);
		if (def_prop.is_empty()==false)
		{
			def_prop  >>  nbStartupLevels;
			cl_prop[i]  <<  nbStartupLevels;
		}
	}

	//	Try to extract CmdPollingTimeout value
	if (cl_prop[++i].is_empty()==false)	cl_prop[i]  >>  cmdPollingTimeout;
	else
	{
		//	Check default value for CmdPollingTimeout
		def_prop = get_default_class_property(cl_prop[i].name);
		if (def_prop.is_empty()==false)
		{
			def_prop  >>  cmdPollingTimeout;
			cl_prop[i]  <<  cmdPollingTimeout;
		}
	}

	//	Try to extract UseEvents value
	if (cl_prop[++i].is_empty()==false)	cl_prop[i]  >>  useEvents;
	else
	{
		//	Check default value for UseEvents
		def_prop = get_default_class_property(cl_prop[i].name);
		if (def_prop.is_empty()==false)
		{
			def_prop  >>  useEvents;
			cl_prop[i]  <<  useEvents;
		}
	}

	//	Try to extract ServerStartupTimeout value
	if (cl_prop[++i].is_empty()==false)	cl_prop[i]  >>  serverStartupTimeout;
	else
	{
		//	Check default value for ServerStartupTimeout
		def_prop = get_default_class_property(cl_prop[i].name);
		if (def_prop.is_empty()==false)
		{
			def_prop  >>  serverStartupTimeout;
			cl_prop[i]  <<  serverStartupTimeout;
		}
	}

	//	End of Automatic code generation
	//-------------------------------------------------------------
	cout2 << "readInfoDbPeriod  = " << readInfoDbPeriod << endl;
	cout2 << "nbStartupLevels   = " << nbStartupLevels << endl;
	cout2 << "cmdPollingTimeout = " << cmdPollingTimeout << endl;
	cout2 << "useEvents         = " << ((useEvents)? "True":"False") << endl;
}

//+----------------------------------------------------------------------------
//
// method : 	StarterClass::set_default_property
// 
// description: Set default property (class and device) for wizard.
//              For each property, add to wizard property name and description
//              If default value has been set, add it to wizard property and
//              store it in a DbDatum.
//
//-----------------------------------------------------------------------------
void StarterClass::set_default_property()
{
	string	prop_name;
	string	prop_desc;
	string	prop_def;

	vector<string>	vect_data;
	//	Set Default Class Properties
	prop_name = "ReadInfoDbPeriod";
	prop_desc = "Period to read database for new info if not fired from Database server.";
	prop_def  = "";
	vect_data.clear();
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		cl_def_prop.push_back(data);
		add_wiz_class_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_class_prop(prop_name, prop_desc);

	prop_name = "NbStartupLevels";
	prop_desc = "Number of startup levels managed by starter.";
	prop_def  = "5";
	vect_data.clear();
	vect_data.push_back("5");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		cl_def_prop.push_back(data);
		add_wiz_class_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_class_prop(prop_name, prop_desc);

	prop_name = "CmdPollingTimeout";
	prop_desc = "Timeout value in seconds to stop polling if no command has been received.";
	prop_def  = "";
	vect_data.clear();
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		cl_def_prop.push_back(data);
		add_wiz_class_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_class_prop(prop_name, prop_desc);

	prop_name = "UseEvents";
	prop_desc = "Use events if not null.";
	prop_def  = "0";
	vect_data.clear();
	vect_data.push_back("0");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		cl_def_prop.push_back(data);
		add_wiz_class_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_class_prop(prop_name, prop_desc);

	prop_name = "ServerStartupTimeout";
	prop_desc = "Timeout on device server startup in seconds.";
	prop_def  = "1";
	vect_data.clear();
	vect_data.push_back("1");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		cl_def_prop.push_back(data);
		add_wiz_class_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_class_prop(prop_name, prop_desc);

	//	Set Default Device Properties
	prop_name = "StartDsPath";
	prop_desc = "Path to find executable files\nto start device servers";
	prop_def  = "";
	vect_data.clear();
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);

	prop_name = "WaitForDriverStartup";
	prop_desc = "The Starter will wait a bit before starting servers, to be sure than the drivers\nare started.This time is in seconds.";
	prop_def  = "0";
	vect_data.clear();
	vect_data.push_back("0");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);

	prop_name = "UseEvents";
	prop_desc = "Use events if not null.";
	prop_def  = "0";
	vect_data.clear();
	vect_data.push_back("0");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);

	prop_name = "StartServersAtStartup";
	prop_desc = "Skip starting servers at startup if false.";
	prop_def  = "true";
	vect_data.clear();
	vect_data.push_back("true");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);

	prop_name = "InterStartupLevelWait";
	prop_desc = "Time to wait before two startup levels in seconds.";
	prop_def  = "1";
	vect_data.clear();
	vect_data.push_back("1");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);

	prop_name = "ServerStartupTimeout";
	prop_desc = "Timeout on device server startup in seconds.";
	prop_def  = "5";
	vect_data.clear();
	vect_data.push_back("5");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);

	prop_name = "KeepLogFiles";
	prop_desc = "Number of log file kept.";
	prop_def  = "3";
	vect_data.clear();
	vect_data.push_back("3");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);

}
//+----------------------------------------------------------------------------
//
// method : 		StarterClass::write_class_property
// 
// description : 	Set class description as property in database
//
//-----------------------------------------------------------------------------
void StarterClass::write_class_property()
{
	//	First time, check if database used
	//--------------------------------------------
	if (Tango::Util::_UseDb == false)
		return;

	Tango::DbData	data;
	string	classname = get_name();
	string	header;
	string::size_type	start, end;

	//	Put title
	Tango::DbDatum	title("ProjectTitle");
	string	str_title("Starter for Tango Administration");
	title << str_title;
	data.push_back(title);

	//	Put Description
	Tango::DbDatum	description("Description");
	vector<string>	str_desc;
	str_desc.push_back("This device server is able to control <b>Tango</b> components (database, device servers, clients...).");
	str_desc.push_back("It is able to start or stop and to report the status of these components.");
	description << str_desc;
	data.push_back(description);
		
	//	put cvs or svn location
	string	filename(classname);
	filename += "Class.cpp";
	
	// Create a string with the class ID to
	// get the string into the binary
	string	class_id(ClassId);
	
	// check for cvs information
	string	src_path(CvsPath);
	start = src_path.find("/");
	if (start!=string::npos)
	{
		end   = src_path.find(filename);
		if (end>start)
		{
			string	strloc = src_path.substr(start, end-start);
			//	Check if specific repository
			start = strloc.find("/cvsroot/");
			if (start!=string::npos && start>0)
			{
				string	repository = strloc.substr(0, start);
				if (repository.find("/segfs/")!=string::npos)
					strloc = "ESRF:" + strloc.substr(start, strloc.length()-start);
			}
			Tango::DbDatum	cvs_loc("cvs_location");
			cvs_loc << strloc;
			data.push_back(cvs_loc);
		}
	}
	// check for svn information
	else
	{
		string	src_path(SvnPath);
		start = src_path.find("://");
		if (start!=string::npos)
		{
			end = src_path.find(filename);
			if (end>start)
			{
				header = "$HeadURL: ";
				start = header.length();
				string	strloc = src_path.substr(start, (end-start));
				
				Tango::DbDatum	svn_loc("svn_location");
				svn_loc << strloc;
				data.push_back(svn_loc);
			}
		}
	}

	//	Get CVS or SVN revision tag
	
	// CVS tag
	string	tagname(TagName);
	header = "$Name: ";
	start = header.length();
	string	endstr(" $");
	
	end   = tagname.find(endstr);
	if (end!=string::npos && end>start)
	{
		string	strtag = tagname.substr(start, end-start);
		Tango::DbDatum	cvs_tag("cvs_tag");
		cvs_tag << strtag;
		data.push_back(cvs_tag);
	}
	
	// SVN tag
	string	svnpath(SvnPath);
	header = "$HeadURL: ";
	start = header.length();
	
	end   = svnpath.find(endstr);
	if (end!=string::npos && end>start)
	{
		string	strloc = svnpath.substr(start, end-start);
		
		string tagstr ("/tags/");
		start = strloc.find(tagstr);
		if ( start!=string::npos )
		{
			start = start + tagstr.length();
			end   = strloc.find(filename);
			string	strtag = strloc.substr(start, end-start-1);
			
			Tango::DbDatum	svn_tag("svn_tag");
			svn_tag << strtag;
			data.push_back(svn_tag);
		}
	}

	//	Get URL location
	string	httpServ(HttpServer);
	if (httpServ.length()>0)
	{
		Tango::DbDatum	db_doc_url("doc_url");
		db_doc_url << httpServ;
		data.push_back(db_doc_url);
	}

	//  Put inheritance
	Tango::DbDatum	inher_datum("InheritedFrom");
	vector<string> inheritance;
	inheritance.push_back("Device_4Impl");
	inher_datum << inheritance;
	data.push_back(inher_datum);

	//	Call database and and values
	//--------------------------------------------
	get_db_class()->put_property(data);
}

}	// namespace