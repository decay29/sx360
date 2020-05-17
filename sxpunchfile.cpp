//Copyright (c) 2007 Spanware Inc.
//All rights reserved.
//See bsdlicense.txt for license information.


#include "sxpunchfile.h"
#include "sxiterator.h"

using namespace sx360;

sxPunchFile :: sxPunchFile(ByteMap map) : byteMap_(map), recordCount_(0)
{
}

sxPunchFile :: ~sxPunchFile()
{
	if (file_)
		fclose(file_);
}


bool sxPunchFile :: open(string name, FileSetup setup)
{
	fileSetup_ = setup;
	file_ = fopen( name.c_str(), FILE_OPEN_MODE[setup.mode]);

	if (!file_)
		return false;
	
	if (setup.mode != CREATE)
	{
		iter_ = auto_ptr<sxIterator>(new sxIterator(this, &byteMap_));
		iter_->first();
	}
	
	int recordsize = setup.cards * setup.columnsPerCard * byteMap_.byteCount;
	fseeko( file_, 0, SEEK_END);
	off_t size = ftello( file_ );
	fseeko( file_, 0, SEEK_SET);
	recordCount_ = size / recordsize;
	
	

	return true;
}


bool sxPunchFile :: close()
{
	int i = fclose(file_);
	return (i == 0);
}


bool sxPunchFile :: appendRecord( sxRecord* cards)
{
	if (fileSetup_.mode != CREATE)
		return false;
		
	int dataSize = fileSetup_.cards * byteMap_.byteCount * fileSetup_.columnsPerCard;

	bool res = this->write( cards->getRawData(), dataSize );
	if (res)
		++recordCount_;
		
	return res;
	
}


int sxPunchFile :: seek( off_t offset, int whence )
{
	return fseeko( file_, offset, whence );
}

