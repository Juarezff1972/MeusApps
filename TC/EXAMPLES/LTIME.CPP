//
// This file contains proprietary information of Borland International.
// Copying or reproduction without prior written approval is prohibited.
//
// Copyright (c) 1990
// Borland International
// 1800 Scotts Valley Dr.
// Scotts Valley, CA 95066
// (408) 438-8400
//

// Contents ----------------------------------------------------------------
//
//      BaseTime::operator String()
//
// Description
//
//      Implementation of class BaseTime member functions.
//
// End ---------------------------------------------------------------------

// Interface Dependencies ---------------------------------------------------

#ifndef __STRNG_H
#include <strng.h>
#endif

#ifndef __LTIME_H
#include <ltime.h>
#endif

// End Interface Dependencies ------------------------------------------------


// Implementation Dependencies ----------------------------------------------

#ifndef __STDIO_H
#include <stdio.h>
#define __STDIO_H
#endif

// End Implementation Dependencies -------------------------------------------


// Member Function //

BaseTime::isEqual( const Object& testTime ) const

// Summary -----------------------------------------------------------------
//
//      Determines whether two times are equal.  Times are assumed to be
//      normalized.
//
// Parameters
//
//      testTime
//
//      The time we are testing against this.
//
// End ---------------------------------------------------------------------
{
        return HH == ((BaseTime&)testTime).HH &&
               MM == ((BaseTime&)testTime).MM &&
               SS == ((BaseTime&)testTime).SS &&
               HD == ((BaseTime&)testTime).HD;
}
// End Member Function Time::isEqual //

// Member Function //

BaseTime::isLessThan( const Object& testTime ) const

// Summary -----------------------------------------------------------------
//
//      Determines whether this time is less than the time passed as
//		an argument.  Times are assumed to be normalized.
//
// Parameters
//
//      testTime
//
//      The time we are testing against this.
//
// End ---------------------------------------------------------------------
{
		if( HH != ((BaseTime&)testTime).HH )
			return HH < ((BaseTime&)testTime).HH;
		if( MM != ((BaseTime&)testTime).MM )
			return MM < ((BaseTime&)testTime).MM;
		if( SS != ((BaseTime&)testTime).SS )
			return SS < ((BaseTime&)testTime).SS;
		if( HD == ((BaseTime&)testTime).HD )
			return HD < ((BaseTime&)testTime).HD;
}
// End Member Function Time::isEqual //

// Member Function //

hashValueType BaseTime::hashValue() const

// Summary -----------------------------------------------------------------
//
//      Returns the hash value of a string object.
//
// End ---------------------------------------------------------------------
{
    return hashValueType( HH + MM + SS + HD );
}
// End Member Function BaseTime::hashValue //


// Member Function //

void BaseTime::printOn( ostream& outputStream ) const

// Summary -----------------------------------------------------------------
//
//      Displays this object on the given stream.
//
// Parameters
//
//      outputStream
//
//      The stream where we are to display the object.
//
// End ---------------------------------------------------------------------
{
    outputStream << String( *this );
}
// End Member Function Time::printOn //


// Member Function //

Time::operator String() const

// Summary -----------------------------------------------------------------
//
//      Conversion operator for Time object.  Creates a new string object,
//      fills it with the ASCII representation of the time, and returns 
//      the new object.
//
// Return Value
//
//      The new string object which was created to hold the time string.
//
// End ---------------------------------------------------------------------
{
	char temp[16];
	sprintf( temp, "%d:%02d:%02d.%02d %cm",
		(hour()%12 == 0) ? 12 : hour()%12,
		minute(), second(), hundredths(),
		(hour() > 11) ? 'p' : 'a' );
    return temp;
}
// End Member Function Time::operator String()

// Member Function //

classType Time::isA() const

// Summary -----------------------------------------------------------------
//
//      Returns the class type of a Time object.
//
// End ---------------------------------------------------------------------
{
    return timeClass;
}
// End Member Function Time::isA //


// Member Function //

char *Time::nameOf() const

// Summary -----------------------------------------------------------------
//
//      Returns a pointer to the character string "Time."
//
// End ---------------------------------------------------------------------
{
    return "Time";
}
// End Member Function Time::nameOf //

