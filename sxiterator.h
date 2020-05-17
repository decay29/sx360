//Copyright (c) 2007 Spanware Inc.
//All rights reserved.
//See bsdlicense.txt for license information.


#ifndef __SXITERATOR_H_
#define __SXITERATOR_H_

#ifdef _WIN32
	#ifdef __DLL
	#define DLLDATA _declspec(dllexport)
	#else
	#define DLLDATA _declspec(dllimport)
	#endif
#else
	#define DLLDATA
#endif

#include <cstdio>

using std::size_t;
 
namespace sx360
{
 
	class sxRecord;
	class sxPunchFile;
	struct ByteMap;
	class sxEvent;
	class sxIndex;

	class DLLDATA sxIterator
	{
	private:
		size_t currentIndex_;
		unsigned int totalCount_;
		int bytesPerCard_;
		sxPunchFile* owner_;
		sxRecord* currentRecord_;
		int dataSize_;
	public:
		sxIterator( sxPunchFile* pf, ByteMap* bmap );
		virtual ~sxIterator();

		size_t getCurrentIndex() const;
		
		sxRecord* first();
		sxRecord* last();
		sxRecord* next();
		sxRecord* previous();
		

		bool write();
		sxEvent* recordChangeEvent;
		
		sxRecord* gotoRecord( size_t i );
		sxRecord* gotoRecord( sxIndex* idx, size_t i );
		void registerEvent( sxEvent* e );

	};

}

#endif
