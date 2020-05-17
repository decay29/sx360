//Copyright (c) 2007 Spanware Inc.
//All rights reserved.
//See bsdlicense.txt for license information.


#ifndef __PUNCHFILE_H_
#define __PUNCHFILE_H_

#ifdef _WIN32
	#ifdef __DLL
	#define DLLDATA _declspec(dllexport)
	#else
	#define DLLDATA _declspec(dllimport)
	#endif
#else
	#define DLLDATA
#endif

#include <vector>
#include <string>
#include <memory>

#include "sxcommon.h"
#include "sxrecord.h"
#include "sxindex.h"

using std::string;
using std::FILE;
using std::auto_ptr;

namespace sx360
{
#ifdef _WIN64
	template class DLLDATA auto_ptr<sxIterator>;
#endif

	class sxIterator;
		
	class DLLDATA sxPunchFile
	{
	private:
		auto_ptr<sxIterator> iter_;
		FILE* file_;
		ByteMap byteMap_;
		FileSetup fileSetup_;
		unsigned int recordCount_;
	public:
		sxPunchFile(ByteMap bmap);
		~sxPunchFile();
		bool open( string name, FileSetup setup);
		bool close();
			
			
		int getCardCount () {return fileSetup_.cards; }
		sxIterator* getIterator() const { return iter_.get();}
		const FileSetup& getSetup() const {return fileSetup_;} 
			
		bool appendRecord( sxRecord* cards );
			
		int seek( off_t offset, int whence );
			
		unsigned int getRecordCount()
		{
			return recordCount_;
		}
			
			
		inline off_t tell()
		{
			return ftello(file_);
		}
		bool read(void* ptr, size_t size)
		{
			return (fread(ptr, size, 1, file_) == 1);
		}
		bool write(void *ptr, size_t size)
		{
			return (fwrite(ptr, size, 1, file_) == 1);
		}
		
		
		
			
	};
	
	
}

#endif
