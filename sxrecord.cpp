//Copyright (c) 2007 Spanware Inc.
//All rights reserved.
//See bsdlicense.txt for license information.


#include "sxrecord.h"
#include "sxcommon.h"


#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <algorithm>

using namespace std;
using namespace sx360;


sxRecord :: sxRecord(int cardCount, int columnsPerCard, ByteMap* bmap) : columnsPerCard_(columnsPerCard), byteMap_(bmap)
{
	byteCount_ = byteMap_->byteCount * byteMap_->usableBitsPerByte; //to determmine total bytes in the map
	columnBytes_ = byteMap_->byteCount * columnsPerCard_; //determine where a read starts..default is 2 * 80;
	map_ = byteMap_->stringMap.c_str();
	
	cardData_ = new char[ cardCount * columnBytes_ ];
	dataSize_ = cardCount * columnBytes_;
	memset(cardData_, 0, dataSize_);
}

sxRecord :: ~sxRecord()
{
	delete [] cardData_;
}


bool sxRecord :: punched(int card, int col, char punch) const
{
	int pos = (card - 1) * (columnBytes_) + (col * byteMap_->byteCount - byteMap_->byteCount);
	//or pos = (card - 1) * columnBytes_ + ((col - 1) * byteMap_->byteCount);
	char* buf = &cardData_[ pos ];
	
	for (int i = 0 ; i < byteCount_; ++i)
	{
		if ( buf[i / byteMap_->usableBitsPerByte] >> ( i % byteMap_->usableBitsPerByte) & 0x1)
			if (byteMap_->stringMap[i] == punch)
				return true;
	}
	
	return false;
	
}


bool sxRecord :: punch(int card, int col, char punch) const
{
	int pos = (card - 1) * (columnBytes_) + (col * byteMap_->byteCount - byteMap_->byteCount );
	char* buf = &cardData_[pos];
	
	const char* p = strchr(map_, punch);
	if (p)
	{
		pos = (int)(p - map_);
		char& c = buf[ pos / byteMap_->usableBitsPerByte];
		c = c | (0x1) << (pos % byteMap_->usableBitsPerByte);
		//buf[ pos / 6] = c;
		return true;
	}
	else
		return false;

}


bool sxRecord :: fillPunch(int card, int col, char punch) const
{
	int pos = (card - 1) * (columnBytes_) + (col * byteMap_->byteCount  - byteMap_->byteCount );
	char* buf = &cardData_[pos];
	
	const char* p = strchr(map_, punch);
	if (p)
	{
		pos = p - map_;
		char& c = buf[pos / byteMap_->usableBitsPerByte];
		//c &= ~((0x1) << (pos % byteMap_->usableBitsPerByte));
		c = c & ~((0x1) << (pos % byteMap_->usableBitsPerByte));		
		return true;
	}
	else
		return false;
}


bool sxRecord :: punch(int card, int col, const char* punches) const
{
	int pos = (card - 1) * columnBytes_ + (col * byteMap_->byteCount  - byteMap_->byteCount );
	char* buf = &cardData_[pos];

	
	bool retval = true;
	
	size_t len = strlen(punches);
	for (unsigned int i = 0 ; i < len; ++i)
	{
		const char* p = strchr( map_, punches[i] );
		if (p)
		{
			pos = p - map_;
			char& c = buf[ pos / byteMap_->usableBitsPerByte];
			c = c | (0x1) << (pos % byteMap_->usableBitsPerByte);
			//buf[ pos / 6] = c;
		}
		else
		{
			//throw PunchOutOfRange();
			retval = false;
		}
	}
	return retval;
}


bool sxRecord :: punch(int card, int col, int size, int value) const
{
	bool retval = true;

	int pos = (card - 1) * (columnBytes_) + (col * byteMap_->byteCount - byteMap_->byteCount );
	char* buf = &cardData_[pos];
	memset( buf, 0, size * byteMap_->byteCount );

	char number[17];
	memset(number, ' ', 16);
	number[16] = 0;
	sprintf(number, "%0*d", size, value);
	
	for (int j = 0; j < size; ++j)
	{
		buf = &cardData_[pos + j * byteMap_->byteCount];
		const char* p = strchr(map_, number[j]);
		if (p)
		{
			int newpos = p - map_;
			char& c = buf[newpos / byteMap_->usableBitsPerByte];
			c = c | (0x1) << (newpos % byteMap_->usableBitsPerByte);
		}
		else
			retval = false;
	}


	return retval;
}




int sxRecord :: columns(int card, int col, int size) const
{
	char number[17];
	memset(number, ' ', 16);
	number[16] = 0;
	
	int pos = (card - 1) * (columnBytes_) + (col * byteMap_->byteCount  - byteMap_->byteCount );
	char* buf = &cardData_[ pos ];
	
	for (int j = 0; j < size; ++j)
	{
		buf = &cardData_[ pos + (j * byteMap_->byteCount )];	
		
		for (int i = 0 ; i < byteCount_; ++i)
		{
			if ( buf[i / byteMap_->usableBitsPerByte] >> (i % byteMap_->usableBitsPerByte) & 0x1)
			{
				number[j] = map_[i];
				break;
			}
		}
		
	}
	return atoi(number);	
	
}

string sxRecord :: columnValue(int card, int col) const
{
	string res("");
	
	int pos = (card - 1) * columnBytes_ + (col * byteMap_->byteCount  -  byteMap_->byteCount );
	char* buf = &cardData_[pos];
	
	for (int i = 0; i < byteCount_; ++i)
	{
		if (buf[i / byteMap_->usableBitsPerByte] >> (i % byteMap_->usableBitsPerByte) &0x1)
			res.push_back( map_[i] );
	}
	
	sort( res.begin(), res.end() );
	
	return res;
	
}

char* sxRecord :: getRawData()
{
	return cardData_;
}



