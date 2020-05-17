//Copyright (c) 2007 Spanware Inc.
//All rights reserved.
//See bsdlicense.txt for license information.

#ifndef __INDEX_H_
#define __INDEX_H_

#ifdef _WIN32
	#ifdef __DLL
	#define DLLDATA _declspec(dllexport)
	#else
	#define DLLDATA _declspec(dllimport)
	#endif
#else
	#define DLLDATA
#endif

#include "sxiterator.h"

#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>

using std::vector;
using std::ofstream;
using std::ostream_iterator;
using std::ifstream;
using std::istream_iterator;


typedef vector<unsigned long> IntVector;

namespace sx360
{

#ifdef _WIN64
	template class DLLDATA std::allocator<unsigned long>;
	template class DLLDATA std::vector<unsigned long, std::allocator<unsigned long> >;
#endif


	class DLLDATA sxIndex
	{
	private:
		int card_;
		int start_;
		int end_;


		IntVector vec_;
	public:
		sxIndex()
		{
		}
		sxIndex( int s, int x, int e, sxIterator* i ) : card_(s), start_(x), end_(e)
		{
			sxRecord* cards = i->first();
			while (cards)
			{
				vec_.push_back (cards->columns( card_, start_, end_ - start_ + 1));
				cards = i->next();
			}
		}
		void write ( string filename )
		{
			ofstream f( filename.c_str() );
			copy ( vec_.begin(), vec_.end(), ostream_iterator<unsigned long>(f, "\n") );
		}
		void read ( string filename )
		{
			ifstream i( filename.c_str() );
			copy( istream_iterator<unsigned long>( i ), istream_iterator<unsigned long>(), back_inserter( vec_ ) );
		}
		off_t find( unsigned long i )
		{
			IntVector::iterator it = std::find ( vec_.begin(), vec_.end(), i );
			if (it != vec_.end())
				return distance(vec_.begin(), it);
			else
				return -1;
		}
	};


}

#endif





