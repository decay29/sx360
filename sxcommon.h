//Copyright (c) 2007 Spanware Inc.
//All rights reserved.
//See bsdlicense.txt for license information.


#ifndef __COMMON_H_
#define __COMMON_H_

#include <string>

using std::string;


	
//compatibility defines for Win32 using ftello and fseeko
//you MUST use the static version of MSVCRT for this code to compile in VS .NET 2003
//Therefore, it is recommended that you not use 2003 to build a DLL, and instead
//include the source code directly into your project
//VS .NET 2005 does not require the use of the imports

#ifdef _WIN32 
		#define off_t __int64
		
#if _MSC_VER < 1400
		extern "C" int _cdecl _fseeki64(FILE *, __int64, int);
		extern "C" __int64 __cdecl _ftelli64(FILE *);
#endif
		
	#define ftello(fm) _ftelli64(fm)
	#define fseeko(fm, pos, type) _fseeki64(fm, pos, type )
	
	#ifdef __DLL
	#define DLLDATA _declspec(dllexport)
	#else
	#define DLLDATA _declspec(dllimport)
	#endif
	
#else
	#define DLLDATA
#endif

#pragma warning(disable : 4996)

namespace sx360
{

	enum OPEN_MODE {READ_ONLY, READ_WRITE, CREATE};
	char const* const FILE_OPEN_MODE[] = {"rb", "r+b", "w+b"};
	
	struct DLLDATA ByteMap
	{
		string stringMap;
		int byteCount; //number of bytes per column
		int usableBitsPerByte; //actual number of bits used in each byte.  Default is 6.
		ByteMap(string s = "3210XY987654", int bc = 2 , int b = 6) : stringMap(s), byteCount(bc), usableBitsPerByte(b)
		{
			
		}
	};
	
	struct DLLDATA FileSetup
	{
		int cards;
		OPEN_MODE mode;
		int columnsPerCard;
	};
	
}
	
#endif
