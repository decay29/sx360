/*
 *  sxevent.h
 *  sxtest
 *
 *  Created by David Koretzky on 11/16/07.
 *  Copyright 2007 Spanware, Inc. All rights reserved.
 *
 */
 
 #ifndef __SXEVENT_H_
 #define __SXEVENT_H_
 
 #include "sxpunchfile.h"
 #include "sxiterator.h"
 #include "sxrecord.h"
 
 #include <iostream>
 
 namespace sx360
 {
 
	 class sxEvent
	 {
	 protected:
		sxPunchFile* pf_;
	 public:
		sxEvent(sxPunchFile* f) : pf_(f)
		{
		}
		virtual void execute() = 0;
		virtual ~sxEvent()
		{
		}
	 };
	 

}
	 

#endif