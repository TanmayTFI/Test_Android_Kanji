//====================================
// Framework
// Console class
//====================================

#ifndef FRMWRK_CONSOLE_DEF
#define FRMWRK_CONSOLE_DEF

#include "LangDefs.h"

#define PRINT(x)	KPTK::logMessage((x)) //Console::Instance->Print(x);
	
		#define WARN(x)		KMiscTools::messageBox((const char*)StringTable::getStr("WRNG").c_str(),(x)); //KMiscTools::showMessConsole::Instance->Warning(x);
		#define FATAL(x)	{KMiscTools::messageBox((const char*)StringTable::getStr("FATAL").c_str(),(x));exit(0);} //Console::Instance->Error(x);

#endif
