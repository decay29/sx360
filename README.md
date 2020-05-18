# sx360

sx360 is a c++ library for reading and writing of 360 column binary files.

Copyright (c) 2007 Spanware Inc.
All rights reserved.

As you can see from the Copyright message this is exceptionally old code.  It however compiles and runs fine on all major platforms.  For Linux, CMake is provided.  For macOS, there is a very old Xcode project.  For Windows, there is a .sln file.  If for some reason, none of those work for you, it should be trivial to build a project with the small number of files.

Updates to modern c++ are forthcoming.

sx360 is licensed under the BSD license. Licensing is in bsdlicense.txt

*Documentation is in doxygen in the docs/html subdirectory.*

If you are building the source on your own for Windows (not using the supplied project files), you must define the _DLL macro.

## Understanding the 360 column binary format
Chances are if you are here, you probably already know the format, but here is a quick primer on the format:

Imagine an 80 column punch card.  No really, try.  Well, if you can't (because you are under 1000 years old) here is a link to a picture:

https://en.wikipedia.org/wiki/Punched_card#/media/File:Used_Punchcard_(5151286161).jpg

So, for some reason, certain industries (market research is one) decided that turning this idea into a binary file format for a PC was a good idea.  It's kind of like when someone leaves a hit TV show, and their spinoff is painfully bad.  Except, in this case, people still thought this should continue.  So, here we are.  

The "standard" binary representation of a punch card is 80 columns of 12 bytes.  The numbers range from 0-9 and X, Y.  The PC representation expects a certain order of the punches, not like what you see above with the 0 - 9, and a couple of rows above them.

Because there are 12 answers, we can use 12 bits to store all possible answers for a column on the card.  That's two bytes.  The two bytes are evenly distributed, so each byte wastes two bits.  The standard is that the punch order is 3210XY for the six bits of the first byte, and 987654 for the six bits of the second byte.  

"Punching" a card is simply a matter of flipping a bit at the position of the column and punch.  So, to punch 3 on column 1 (using the standard punch order), simply flip the first bit of the first byte.  If you want to punch 9 on column 1, then you would flip the first bit of the second byte, and so on.

In many cases, there will be a series of cards grouped together.  You would then reference an answer by Card, Column, and Punch (ccp).  Card 2, Column 10, Punch X for instance.  

What if you want to be able to put numbers into this system?  This is a binary punch system after all.

The answer is simple, but space consuming.  If you want to put the number 345 into a punch card, you will use three whole columns, with one digit in each column.  Unlike binary punches, there can only be one punch per column, and it has to be 0-9, and can't be X or Y.  An example of this would be a number that takes three columns, starting at column 20, so on each card column 20 would have the first digit, column 21 would have the second digit, and column 22 would have the third digit. 

By looking at a punch card, there is no certain way to determine whether a punch is binary or is part of a set of numeric columns.  Sometimes its obvious, but accompanying the binary files is some sort of setup or dictionary file that describes what columns have what punches, and what they mean.  Otherwise, it's just a bag of bits.

## sx360 handling of binary files 

At the top level is sxPunchFile.  When you open an `sxPunchFile`, you will pass it a few parameters for the layout of the punches.  You are allowed to configure them however you want, including not wasting the 2 bits per byte.  If you choose to do nothing the default 3210XY and 987654 mapping per byte is used.  Files have two modes, `CREATE` and the very poorly named `READ_ONLY`.  If you open a file in `CREATE` mode, you can only append records, you cannot read them.  If you open in `READ_ONLY`, you can make actual changes to existing records, but you cannot add new ones.

Because this code was originally written when RAM was not as plentiful as it is now, the file is read record by record into an `sxRecord`, thru `sxIterator`.  `sxIterator` lets you move back and forth through the file.  `sxIterator` does not exist when you CREATE a binary file, so that is why you can only append `sxRecord`s to the end of the file.

`sxRecord` gives a lot of options.  You can punch a punch in a column.  You can actually unpunch a column as well.  You can set a number across multiple columns, and you can even punch multiple punches in one column.  You get similar flexilbity when reading punched data.

Examples of this are in the doxygen documentation.


August 5, 2007 - Initial Release


Version 1.1 - November 13, 2007

This version fixes some bugs, and some glaring omissions.  First, the sxPunchFile::getRecordCount function was never implemented.  This has been corrected.  There were other signed/unsigned oversights on my behalf that have also been fixed.  These related to navigating to the first record, or the use of an sxIndex.

Version 2.0 - November 27, 2007

Event handling has been added.  The sxRecord::fillPunch function has been fixed.  There was a bug that would wipe out the whole column instead of just one punch when called.
