//Copyright (c) 2007 Spanware Inc.
//All rights reserved.
//See bsdlicense.txt for license information.


#include "sxiterator.h"
#include "sxpunchfile.h"
#include "sxrecord.h"
#include "sxindex.h"
#include "sxevent.h"


using namespace sx360;


sxIterator ::	sxIterator( sxPunchFile* pf, ByteMap* bmap ) : owner_(pf), currentIndex_(0), currentRecord_(NULL), recordChangeEvent(NULL)
{
	currentRecord_ = new sxRecord( owner_->getSetup().cards, owner_->getSetup().columnsPerCard, bmap );

	bytesPerCard_ = owner_->getSetup().columnsPerCard * bmap->byteCount;
	
	owner_->seek( 0, SEEK_END );
	off_t pos = owner_->tell();
	owner_->seek( 0, SEEK_SET );
	
	int columnBytes = bmap->byteCount * owner_->getSetup().columnsPerCard;
	
	dataSize_ = columnBytes * owner_->getSetup().cards;

	totalCount_ = (unsigned int)(pos / dataSize_);
	
	
}

sxIterator :: ~sxIterator()
{
	delete currentRecord_;
}



sxRecord* sxIterator :: first()
{

	if (recordChangeEvent)
		recordChangeEvent->execute(  );

	owner_->seek( 0, SEEK_SET );
	
	owner_->read( currentRecord_->getRawData(), dataSize_);
	currentIndex_ = 0;
	
	owner_->seek(0, SEEK_SET);

	return currentRecord_;
}

sxRecord* sxIterator :: last()
{

	if (recordChangeEvent)
		recordChangeEvent->execute(  );
	
	owner_->seek( -(owner_->getCardCount() * bytesPerCard_ ), SEEK_END);
	
	off_t place = owner_->tell();
	
	owner_->read( currentRecord_->getRawData(), dataSize_);
	currentIndex_ = ( place / (owner_->getCardCount() * bytesPerCard_) );
	
	owner_->seek ( place, SEEK_SET );
	
	return currentRecord_;
	
}

sxRecord* sxIterator :: next()
{
	++currentIndex_;
	if (currentIndex_ >= (totalCount_ ))
	{
		currentIndex_ = totalCount_ - 1;
		return NULL;
	}

	if (recordChangeEvent)
		recordChangeEvent->execute(  );

	owner_->seek( dataSize_, SEEK_CUR);
	off_t place = owner_->tell();

	owner_->read( currentRecord_->getRawData(), dataSize_);
	owner_->seek(place, SEEK_SET );
	
	return currentRecord_;
	
	
}


sxRecord* sxIterator :: previous()
{
	if (currentIndex_ == 0)
		return NULL;
		
	if (recordChangeEvent)
		recordChangeEvent->execute(  );

	currentIndex_--;

	owner_->seek(-(owner_->getCardCount() * bytesPerCard_), SEEK_CUR);
	
	owner_->read( currentRecord_->getRawData(), dataSize_);
	
	owner_->seek(-(owner_->getCardCount() * bytesPerCard_), SEEK_CUR);	
	
	return currentRecord_;
}

sxRecord* sxIterator :: gotoRecord( size_t i )
{
	if (i >= totalCount_)
		return NULL;
		
	currentIndex_ = i;
	
	owner_->seek(dataSize_ * i, SEEK_SET);
	owner_->read( currentRecord_->getRawData(), dataSize_);
	owner_->seek(-(owner_->getCardCount() * bytesPerCard_), SEEK_CUR);	
	
	return currentRecord_;
	
	
}

sxRecord* sxIterator :: gotoRecord( sxIndex* idx, size_t i)
{
	off_t z = idx->find(i);
	if (z < 0)
		return NULL;
		
	return gotoRecord( z );

}


bool sxIterator :: write()
{
	owner_->write( currentRecord_->getRawData(), dataSize_ );
	owner_->seek(-(owner_->getCardCount() * bytesPerCard_), SEEK_CUR);	
	return true;
}


size_t sxIterator :: getCurrentIndex() const
{
	return currentIndex_;
}












