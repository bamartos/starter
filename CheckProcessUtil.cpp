static const char *RcsId = "$Header$";
//+=============================================================================
//
// file :         CheckProcessUtil.cpp
//
// description :  C++ source for the CheckProcessUtil
//
// project :      TANGO Device Server
//
// $Author$
//
// $Revision$
//
// $Log$
// Revision 3.10  2008/06/18 08:17:03  pascal_verdier
// Pb with case unsensitive on win32 fixed.
//
// Revision 3.9  2008/06/06 07:56:50  pascal_verdier
// Case unsensitive on instance name added.
//
// Revision 3.8  2008/06/04 09:08:03  pascal_verdier
// javaw process control added.
// Java -cp classpath parsing mmodified.
//
// Revision 3.7  2008/05/15 08:07:18  pascal_verdier
// TangoSys_MemStream replaced by TangoSys_OMemStream
// (for leaking problem under win32)
//
// Revision 3.6  2008/04/28 12:36:09  pascal_verdier
// Eception in solaris modified.
//
// Revision 3.5  2008/04/24 06:33:52  pascal_verdier
// Bug in solaris management fixed.
//
// Revision 3.4  2008/04/10 12:15:05  jensmeyer
// Added compile options for MacOSX and FreeBSD
//
// Revision 3.3  2008/04/09 14:39:57  pascal_verdier
// Better trace on pread failed
//
// Revision 3.2  2008/03/03 13:26:15  pascal_verdier
// is_process_running() method added.
//
// Revision 3.1  2008/02/29 15:15:05  pascal_verdier
// Checking running processes by system call added.
//
//
// copyleft :     European Synchrotron Radiation Facility
//                BP 220, Grenoble 38043
//                FRANCE
//
//-=============================================================================



#include <CheckProcessUtil.h>

#ifndef	TIME_VAR
#ifndef WIN32

#	define	TimeVal	struct timeval
#	define	GetTime(t)	gettimeofday(&t, NULL);
#	define	Elapsed(before, after)	\
		1000.0*(after.tv_sec-before.tv_sec) + \
		((double)after.tv_usec-before.tv_usec) / 1000

#else

#	define	TimeVal	struct _timeb
#	define	GetTime(t)	_ftime(&t);
#	define	Elapsed(before, after)	\
		1000*(after.time - before.time) + (after.millitm - before.millitm)

#endif	/*	WIN32		*/
#endif	/*	TIME_VAR	*/


namespace Starter_ns
{
//=============================================================
//=============================================================
ProcessData::ProcessData()
{
#ifdef WIN32
	//	Under win 2000 or before the process name is shorted to 15 char
	//	If win 2000, take it from command line.
	win2000 = isWin2000();
#endif	/*	WIN32		*/
}
//=============================================================
//=============================================================
ProcessData::~ProcessData()
{
	//	clear previous list
	for (int i=0 ; i<proc_list.size() ; i++)
		delete proc_list[i];
	proc_list.clear();
}

#ifdef WIN32
//=============================================================
//=============================================================
string  ProcessData::parseNameFromCmdLine(string name, string cmdline)
{
	//	Search last position of name
	string::size_type	pos = 0;
	string::size_type	tmp;
	while ((tmp=cmdline.find(name, pos+1))!=string::npos)
		pos = tmp;

	//	Get name before sppace char
	string::size_type	end = cmdline.find(" ", pos);
	if (end==string::npos)
		end = cmdline.find("\t", pos);
	string	full_name = cmdline.substr(pos, end-pos);

	//	Take off extention if any
	end = full_name.find(".");
	if (end==string::npos)
		return full_name;
	else
		return full_name.substr(0, end);
}
//=============================================================
//=============================================================
bool ProcessData::isWin2000(void)
{
	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if(!(bOsVersionInfoEx=GetVersionEx((OSVERSIONINFO *)&osvi)))
	{
		osvi.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
		if (!GetVersionEx((OSVERSIONINFO *)&osvi) )
			return true;
	}
	double	osversion = osvi.dwMajorVersion + (1.0*osvi.dwMinorVersion/10);
	if (osversion<5.1)
	{
		cout << "/=======================================" << endl;
		cout << "	Windows 2000 or before !!!" << endl;
		cout << "/=======================================" << endl;
		return true;
	}
	else
	{
		cout << "Windows XP or later" << endl;
		return false;
	}
}
//=============================================================
//=============================================================
string ProcessData::wchar2string(WCHAR *wch, int size)
{
	char	*ch = new char[size+1];
	int	i;
	for (i=0 ; wch[i]!=0 && i<size ; i++)
		ch[i] = wch[i];
	ch[i] = 0x0;
	string	str(ch);
	delete ch;
	return str;
}
//=============================================================
//=============================================================
WCHAR *ProcessData::string2wchar(string str)
{
	char	*ch  = new char[str.length()+1];
	WCHAR	*wch = new WCHAR[str.length()+1];
	strcpy(ch, str.c_str());

	int	i;
	for (i=0 ; ch[i]!=0 ; i++)
		wch[i] = (short)ch[i];
	wch[i] = 0x0;
	delete ch;
	return wch;
}
//=============================================================
//=============================================================
void ProcessData::read_process_list_from_sys()
{
	//	clear previous list
	for (int i=0 ; i<proc_list.size() ; i++)
		delete proc_list[i];
	proc_list.clear();

	// Take a snapshot of all processes in the system.
	HANDLE	hProcessSnap = 
		CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS | TH32CS_SNAPMODULE, 0);
	if( hProcessSnap == INVALID_HANDLE_VALUE)
	{
		string desc = errorCodeToString(GetLastError(), "CreateToolhelp32Snapshot" );
		Tango::Except::throw_exception(
						(const char *)"PROCESS_LIST_FAILED",
						(const char *) desc.c_str(),
						(const char *)"Starter::get_process_list()");
	}

	// Set the size of the structure before using it.
	PROCESSENTRY32	pe32;
	pe32.dwSize = sizeof( PROCESSENTRY32 );

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if( !Process32First( hProcessSnap, &pe32 ) )
	{
		string desc = errorCodeToString(GetLastError(), "Process32First" );  // Show cause of failure
		CloseHandle( hProcessSnap );     // Must clean up the snapshot object!
		Tango::Except::throw_exception(
						(const char *)"PROCESS_LIST_FAILED",
						(const char *) desc.c_str(),
						(const char *)"Starter::get_process_list()");
	}

	//	Get module ntdll
    NTQIP						*lpfnNtQueryInformationProcess;
	PROCESS_BASIC_INFORMATION	pbi;
	WCHAR	*wc = string2wchar("ntdll.dll");
	HINSTANCE	hLibrary = GetModuleHandle(wc);
	delete wc;
    if (hLibrary != NULL)
    {
        lpfnNtQueryInformationProcess = (NTQIP *)GetProcAddress(hLibrary, "ZwQueryInformationProcess");
    }
	else
    {
		string	desc = errorCodeToString(GetLastError(), "GetModuleHandle() ");
		Tango::Except::throw_exception(
						(const char *)"PROCESS_LIST_FAILED",
						(const char *) desc.c_str(),
						(const char *)"Starter::get_process_list()");
    }

	__INFOBLOCK	block;
    __PEB 		PEB;
	char		*c_cmdline = NULL;
    DWORD		dwSize=0;

	// Now walk the snapshot of processes, and
	pbi.PebBaseAddress = (PPEB)0x7ffdf000;
	do
	{
		// Retrieve the priority class.
		DWORD	dwPriorityClass = 0;
		HANDLE	hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID );
		if( hProcess != NULL )
		{
	        if (lpfnNtQueryInformationProcess != NULL)
    	        (*lpfnNtQueryInformationProcess)(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), &dwSize);

			string cmdline("");
			dwPriorityClass = GetPriorityClass( hProcess );
			if( !dwPriorityClass )
				errorCodeToString(GetLastError(), "GetPriorityClass" );

			if (ReadProcessMemory(hProcess, 
					pbi.PebBaseAddress,
					&PEB,
					sizeof(PEB),
					&dwSize))
			{
				if (ReadProcessMemory(hProcess, 
						(LPVOID)PEB.dwInfoBlockAddress,
						&block,
						sizeof(block),
						&dwSize))
				{
					WCHAR	*buff = new WCHAR[block.wMaxLength+1];
    	        	if (ReadProcessMemory(hProcess, 
									(LPVOID)block.dwCmdLineAddress, 
									buff, 
									block.wMaxLength, 
									&dwSize))
						cmdline = wchar2string(buff, dwSize);
					else
						errorCodeToString(GetLastError(), "3-ReadProcessMemory()" );
					delete buff;
				}
				else
					errorCodeToString(GetLastError(), "2-ReadProcessMemory()" );
			}
			else
				errorCodeToString(GetLastError(), "1-ReadProcessMemory()" );
			
			CloseHandle( hProcess );

			//	build process object to be added in vector
			Process	*process = new Process();

			//	Remove extention from exe name
			string	full_name = wchar2string(pe32.szExeFile);
			string::size_type	pos = full_name.find('.');
			if (pos!=string::npos)
				process->name = full_name.substr(0, pos);
			else
				process->name = full_name;

			//	Parse name frome cmd line because file manager truncate it at 15 chars
			if (win2000 && process->name.length()>13)
				process->name = parseNameFromCmdLine(process->name, cmdline);

			//	On win32 -> exe file is case unsesitive
			transform(process->name.begin(), process->name.end(),
				process->name.begin(), ::tolower);


			//	add pid and cmd line
			process->pid  = pe32.th32ProcessID;
			process->line = cmdline;
			proc_list.push_back(process);
		}
	} while ( Process32Next(hProcessSnap, &pe32) );

	CloseHandle(hProcessSnap);
}



// ============================================================================
// Win32ProcessManager::errorCodeToString
// ============================================================================
string ProcessData::errorCodeToString (DWORD err_code,  string src)
{
	WCHAR	*buff;
	string	msg;

	if(err_code != ERROR_SUCCESS)
	{
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			err_code,
			0, // Default language
			(LPTSTR) &buff,	
			0,
			NULL 
			);
		msg = src + string(" failed : ");
		msg += wchar2string(buff);

		// Free the buffer.
		LocalFree(buff);
	}
	else
		msg = "No Error";

	cerr << msg << endl;

	return msg;
}



#else	//	WIN32




//=============================================================
//=============================================================
void ProcessData::read_process_list_from_sys()
{
	//	clear previous list
	for (int i=0 ; i<proc_list.size() ; i++)
		delete proc_list[i];
	proc_list.clear();

	//	build processes list
	DIR		*proc = opendir ("/proc") ;
	if(proc == NULL)
	{
		string	desc;
		//	error
		switch(errno)
		{
		case EACCES: desc = "Permission denied.";
			break;
		case EMFILE: desc = "Too many file descriptors in use by process.";
			break;
		case ENFILE: desc = "Too many file are currently open in the system.";
			break;
		case ENOENT: desc = "Directory does not exist or NAME is an empty string.";
			break;
		case ENOMEM: desc = "Insufficient memory to complete the operation.";
			break;
		case ENOTDIR:desc  = "NAME is not a directory.";
			break;
		}
		Tango::Except::throw_exception(
				(const char *)"READ_PROCESS_LIST_FAILED",
				desc,
				(const char *)"Starter::get_process_list()");
		
	} 

	struct dirent	*ent;
	while (ent = readdir (proc))
	{
		if (isdigit (ent->d_name[0]))
		{
			//	Get PID
			Process	*process = new Process();
			process->pid = atoi(ent->d_name);
			try
			{
				//	if process can be read, add process object to vector
				if (manageProcFiles(process))
					proc_list.push_back(process);
				else
					delete process;
			}
			catch(Tango::DevFailed &e)
			{
				cout << "Excepion catch during manageProcFiles for pid = "
					<< process->pid << endl;
				cout << e.errors[0].desc;
				delete process;
			}
			catch(...)
			{
				cout << "Excepion catch during manageProcFiles for pid = "
					<< process->pid << endl;
				delete process;
			}
		}
	}
	closedir(proc);
}


//=============================================================
/**
 *	Manage the /proc files
 */
//=============================================================
bool  ProcessData::manageProcFiles(Process *process)
{
#if (defined linux) || (defined __darwin__) || (defined __freebsd__)

	//	Read command line file
	TangoSys_OMemStream	fname;
	fname << "/proc/" << process->pid <<"/cmdline";

	//	Read file
	ifstream	ifs((char *)fname.str().c_str());
	if (ifs)
	{
		TangoSys_OMemStream	sstr;
		sstr << ifs.rdbuf() << ends;
		ifs.close();

		//	Get command line
		process->line = sstr.str();
		//	Replace NULL with SPACE char
		string::size_type	pos;
		while ((pos=process->line.find('\0'))!=string::npos)
			process->line.replace(pos, 1, " ");
		return true;
	}
	else
	{
		cerr << fname.str() << ":	" << strerror(errno) << endl;
		return false;
	}
#else	//	solaris

	//	Read psinfo file		
	TangoSys_OMemStream	fname;
	fname << "/proc/" << process->pid <<"/psinfo";

	int				fid;
	struct psinfo	ps;	
	if ((fid=open(fname.str().c_str(), O_RDONLY))!=-1)
	{
		read(fid, (void *) &ps, sizeof(struct psinfo));
		close(fid);
		process->name = ps.pr_fname;
		
		//	Check if a real process or a shell (cannot be a server)
		if (process->name=="ssh" ||
			process->name=="bash" ||
			process->name=="sh")
			return false;

		uid_t	euid = geteuid();
		if ((euid==0) || (euid == ps.pr_euid))
		{
			/*
			 * To get the argv vector and environment variables
			 * for the process you need to be either root or the owner of the process.
			 * Otherwise you will not be able to open the processes memory.
			 */
			int fdesc;
			TangoSys_OMemStream	filepath;
			filepath << "/proc/" << process->pid <<"/as";
			if ( (fdesc = open(filepath.str().c_str(), O_RDONLY|O_NONBLOCK)) < 0 )
			{
				TangoSys_OMemStream	tms;
				tms << "Cannot open " << filepath.str() << ":	" <<
					strerror(errno) << endl;
#ifdef TRACE
				cerr << tms.str();
#endif
				Tango::Except::throw_exception(
						(const char *)"PROCESS_READ_FAILED",
						(const char *) tms.str().c_str(),
						(const char *)"Starter::manageProcFiles()");
			}
			else
			{
				//	Allocate a pointer array
				size_t	size = sizeof(char *) * (ps.pr_argc+1);
				char **argvp = (char **) malloc(size);

				//	And initialize
				if (pread(fdesc, argvp, size, ps.pr_argv)>0)
				{
					TangoSys_OMemStream	line;

					//	If argv[n] read -> append to command line
					char	buff[0x100];
					for (int n=0; n<ps.pr_argc; n++)
						if (pread(fdesc, buff ,0xFF, (off_t)argvp[n])>0)
							line << buff << " ";

					process->line = line.str();
				}
				else
				{
					free(argvp);
					close(fdesc);
					TangoSys_OMemStream	tms;
					tms << "pread failed when getting command line arguments " <<
						" from memory for process  " << process->name  << " (" <<
						filepath.str() << ")\n" << strerror(errno) << endl;
					cerr << tms.str();
					Tango::Except::throw_exception(
							(const char *)"PROCESS_READ_FAILED",
							(const char *) tms.str().c_str(),
							(const char *)"Starter::manageProcFiles()");
				}
				free(argvp);
				close(fdesc);
			}
		}
		else
		{
			//	Not Owner -> get only the  ps.pr_psargs
			process->line = ps.pr_psargs;
			close(fid);
		}
	}
	else
	{
		TangoSys_OMemStream	tms;
		tms << "open(" << fname.str() << ")  failed\n" <<  strerror(errno) << endl;
		cerr << tms.str();
		Tango::Except::throw_exception(
						(const char *)"PROCESS_READ_FAILED",
						(const char *) tms.str().c_str(),
						(const char *)"Starter::manageProcFiles()");
	}
	return true;
#endif
}
#endif	//	WIN32



//=============================================================
/**
 *	Not only cpp
 *	Check for other than java and python processes
 */
//=============================================================
void ProcessData::check_cpp_process(Process* process)
{
	//	Remove path
#ifndef WIN32
	if (process->line_args.size()==0)
		process->name = "";
	else
		process->name = name_from_path(process->line_args[0]);
#endif

	for (int i=1 ; i<process->line_args.size() ; i++)
		process->proc_args.push_back(process->line_args[i]);
}
//=============================================================
/**
 *	Check for  java processes
 */
//=============================================================
bool ProcessData::check_java_process(Process* process)
{
	if (process->line_args.size()==0)
		return false;
#ifdef WIN32
	if (process->name!="java" &&
		process->name!="javaw")
		return false;
#else
	if (name_from_path(process->line_args[0])!="java")
		return false;
#endif

	//	Parse class and instance name
	bool	found=false;
	for (int i=process->line_args.size()-1 ; !found && i>0 ; i--)
	{
		if (process->line_args[i]!="" && process->line_args[i].c_str()[0]!='-')
		{
			if (i>1)	
			{

				//	To get class name, remove package name of previous arg
				string	full_name(process->line_args[i-1]);
				string::size_type	start = 0;
				string::size_type	end;
				while ((end=full_name.find('.', start))!=string::npos)
					start = end+1;
				//	Get last one
				process->name = full_name.substr(start);
				
				//	and take this one as instance 
				process->proc_args.push_back(process->line_args[i]);
				found = true;
			}
		}
	}
	return true;
}
//=============================================================
/**
 *	Check for  python processes
 */
//=============================================================
bool ProcessData::check_python_process(Process* process)
{
	if (process->line_args.size()==0)
		return false;
#ifdef WIN32
	if (process->name!="python")
		return false;
#else
	if (name_from_path(process->line_args[0])!="python")
		return false;
#endif

	if (process->line_args.size()<2)
		return false;	//	No module loaded

	//	To get python module name
	bool	found = false;
	int		args_idx = 2;
	for (int i=1 ; !found &&  i<process->line_args.size()-1 ; i++)
	{
		//	Do not check if python option
		if (process->line_args[i]!="" && process->line_args[i].c_str()[0]!='-')
		{
			string	full_name(process->line_args[i]);
			string::size_type	start = 0;
			string::size_type	end;
			while ((end=full_name.find('/', start))!=string::npos)
				start = end+1;
			while ((end=full_name.find('\\', start))!=string::npos)
				start = end+1;

			//	Get last one
			end = full_name.find(".py", start);
			process->name = full_name.substr(start, (end-start));
			args_idx = i+1;
		}
	}

	for (int i=args_idx ; i<process->line_args.size() ; i++)
		process->proc_args.push_back(process->line_args[i]);
	return true;
}
//=============================================================
//=============================================================
string ProcessData::name_from_path(string full_name)
{
	string::size_type	start = 0;
	string::size_type	end;
	while ((end=full_name.find('/', start))!=string::npos)
		start = end+1;
	//	Get last one
	return full_name.substr(start);
}
//=============================================================
//=============================================================
void ProcessData::build_server_names(Process* process)
{
	// server is a process with at least one arg
	if (process->proc_args.size()>0)
	{
		process->servname  = process->name;
		process->servname += "/";
		string	instance(process->proc_args[0]);
		transform(instance.begin(), instance.end(),
				instance.begin(), ::tolower);
		process->servname += instance;
#ifdef WIN32
	//	Wain32 is case unsensitive
	transform(process->servname.begin(), process->servname.end(),
					process->servname.begin(), ::tolower);
#endif
}
	else
		process->servname  = "";
}
//=============================================================
//=============================================================




//=============================================================
/**
 *	Public method to update and build process process 
 */
//=============================================================
//#define TRACE
void ProcessData::update_process_list()
{
	omni_mutex_lock sync(*this);

	TimeVal	t0, t1;
	GetTime(t0);
	read_process_list_from_sys();
	GetTime(t1);
#ifdef TRACE
	TimeVal	t2, t3;
	double max_t = 0;
	Process	*max_t_proc;
	cout << "	Reading process list = " << Elapsed(t0, t1) << " ms" << endl;
#endif

	for (int i=0 ; i<proc_list.size() ; i++)
	{
#ifdef TRACE
		GetTime(t2);
#endif
		Process	*process = proc_list[i];

		//	Split on Space char
		string::size_type	start = 0;
		string::size_type	end;
		bool	in_cotes = false;
		while ((end=process->line.find(' ', start))!=string::npos)
		{
			string	s = process->line.substr(start, (end-start));
			start = end+1;
			//	Check if not empty
			if (s!="" && s!=" " && s!="\t")
			{
				//	Check if between cotes
				if (in_cotes==false && s.find('\"')!=string::npos) //	starting
				{
					process->line_args.push_back(s);
					in_cotes = true;
				}
				else
				if (in_cotes==true) // inside
				{
					//	Get last arg and concat with new one
					string	arg = process->line_args.back();
					arg += " " + s;
					//	And replace
					process->line_args.pop_back();
					process->line_args.push_back(arg);

					if (s.find('\"')!=string::npos) // ending
						in_cotes = false;
				}
				else
					process->line_args.push_back(s);
			}
		}
		//	Get last one
		string	s = process->line.substr(start);
		if (s!="")
			process->line_args.push_back(s);


#ifndef WIN32
		if (process->line_args.size()>0)
			process->name = process->line_args[0];
		else
			process->name = "";
			
#endif

		//	Check if java or python process
		if (check_java_process(process)==false)
			if(check_python_process(process)==false)
				check_cpp_process(process);
		build_server_names(process);
#ifdef TRACE2
		cout << process->pid << "	" << process->name;
		if (process->proc_args.size()>0)
			cout << " " << process->proc_args[0];
		cout << endl;
#endif

#ifdef TRACE
		GetTime(t3);
		double	t = Elapsed(t2, t3);
		if (t>max_t)
		{
			max_t = t;
			max_t_proc = process;
		}
#endif

	}
#ifdef TRACE
	GetTime(t1);
	cout << "		total = " << Elapsed(t0, t1) << " ms" << endl;
	cout << "	max:	" << max_t << "  for " << max_t_proc->name << " (" <<
				 max_t_proc->pid << ")" << endl;
#endif

}
//=============================================================
//=============================================================
int ProcessData::get_server_pid(string argin)
{
	omni_mutex_lock sync(*this);
	for (int i=0 ; i<proc_list.size() ; i++)
	{
		Process	*process = proc_list[i];
		// server is a process with at least one arg
		if (process->proc_args.size()>0)
		{
			string	servname(process->name);
			servname += "/";
			servname += process->proc_args[0];
			//cout << servname << "==" << argin << endl;
			if (servname == argin)
				return process->pid;
		}
	}
	return -1;
}
//=============================================================
/**
 * Returs true if server running 
 */
//=============================================================
bool ProcessData::is_server_running(string argin)
{
	omni_mutex_lock sync(*this);
	for (int i=0 ; i<proc_list.size() ; i++)
	{
		Process	*process = proc_list[i];
		if (process->servname == argin)
			return true;
	}
	return false;
}
//=============================================================
/**
 * Returs true if process running (do not check instance name)
 */
//=============================================================
bool ProcessData::is_process_running(string argin)
{
	omni_mutex_lock sync(*this);
	for (int i=0 ; i<proc_list.size() ; i++)
	{
		Process	*process = proc_list[i];
		if (process->name == argin)
			return true;
	}
	return false;
}
//=============================================================
//=============================================================
vector<Process> ProcessData::get_process_list()
{
	omni_mutex_lock sync(*this);
	
	//	copy list
	vector<Process>	ret;
	for (int i=0 ; i<proc_list.size() ; i++)
	{
		Process	*p_src = proc_list[i];
		Process	process;
		Process *p_target = &process;
		*p_target = *p_src;
		ret.push_back(process);
	}

	return ret;
}
//=============================================================
//=============================================================






//=============================================================
//=============================================================
int CheckProcessUtil::get_server_pid(string argin)
{
	return data->get_server_pid(argin);
}
//=============================================================
/**
 * Returs true if server running
 */
//=============================================================
bool CheckProcessUtil::is_server_running(string argin)
{
	//	Make sure instance is lower case
	string::size_type	pos = argin.find('/');
	if (pos==string::npos)
		return false;	//	Not a server name
	pos++;
	string	dsname(argin.substr(0, pos));
#ifdef WIN32
	//	Wain32 is case unsensitive
	transform(dsname.begin(), dsname.end(),
					dsname.begin(), ::tolower);
#endif
	string	instance(argin.substr(pos));
	transform(instance.begin(), instance.end(),
					instance.begin(), ::tolower);
	dsname += instance;
	return data->is_server_running(dsname);
}
//=============================================================
/**
 * Returs true if process running (do not check instance name)
 */
//=============================================================
bool CheckProcessUtil::is_process_running(string argin)
{
	return data->is_process_running(argin);
}
//=============================================================
//=============================================================
vector<Process> CheckProcessUtil::get_process_list()
{
	return data->get_process_list();
}
//=============================================================
//=============================================================
void *CheckProcessUtil::run_undetached(void *ptr)
{
	while (stop_thread==false)
	{
		try
		{
			data->update_process_list();
		}
		catch(Tango::DevFailed &e)
		{
			Tango::Except::print_exception(e);
		}
		

		//	And wait n times for next loop
		for (int i=0 ; i<2 && stop_thread==false ; i++)
		{
			omni_mutex_lock sync(*data);
			data->wait(1000);
		}
	}
	delete data;
	return NULL;
}
//=============================================================
//=============================================================


}	//	namespace