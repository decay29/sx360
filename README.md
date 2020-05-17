# sx360

sx360 is a c++ library for reading and writing of 360 column binary files.

Copyright (c) 2007 Spanware Inc.
All rights reserved.

As you can see from the Copyright message this is exceptionally old code.  It however compiles and runs fine on all major platforms.  For Linux, CMake is provided.  For macOS, there is a very old Xcode project.  For Windows, there is a .sln file.  If for some reason, none of those work for you, it should be trivial to build a project with the small number of files.

Updates to modern c++ are forthcoming.

sx360 is licensed under the BSD license. Licensing is in bsdlicense.txt

*Documentation is in the docs/html subdirectory.*

If you are building the source on your own for Windows (not using the supplied project files), you must define the _DLL macro.


August 5, 2007 - Initial Release


Version 1.1 - November 13, 2007

This version fixes some bugs, and some glaring omissions.  First, the sxPunchFile::getRecordCount function was never implemented.  This has been corrected.  There were other signed/unsigned oversights on my behalf that have also been fixed.  These related to navigating to the first record, or the use of an sxIndex.

Version 2.0 - November 27, 2007

Event handling has been added.  The sxRecord::fillPunch function has been fixed.  There was a bug that would wipe out the whole column instead of just one punch when called.
