//Copyright (c) 2007 Spanware Inc.
//All rights reserved.
//See bsdlicense.txt for license information.

#ifndef __SXRECORD_H_
#define __SXRECORD_H_
 
#ifdef _WIN64
	#ifdef __DLL
	#define DLLDATA _declspec(dllexport)
	#else
	#define DLLDATA _declspec(dllimport)
	#endif
#else
	#define DLLDATA
#endif

#include <cstdio>
#include <string>
#include <exception>

using std::FILE;
using std::string;
using std::exception;

namespace sx360
{
	struct ByteMap;

	class DLLDATA sxRecord
	{
	private:
		char* cardData_;
		const char* map_; //const char version of the actual bytemap
		
		int dataSize_;
		ByteMap* byteMap_;
		int byteCount_;
		int columnsPerCard_;
		int columnBytes_;
	public:
		sxRecord( int cardCount, int columnsPerCard, ByteMap* bmap );
		virtual ~sxRecord ();

		bool punched(int card, int col, char punch) const;
		int columns(int card, int col, int size) const;
		string columnValue(int card, int col) const;
		
		bool punch(int card, int col, char punch) const;
		bool punch(int card, int col, const char* punches) const;
		bool punch(int card, int col, int size, int value) const;	
		
		bool fillPunch(int card, int col, char punch) const;
		
		char* getRawData();
		
	};


	class DLLDATA PunchOutOfRange : public exception
	{
	public:
		virtual const char* what() const throw()
		{
			return ("Punch out of Range");
		}
	};

}


#endif
