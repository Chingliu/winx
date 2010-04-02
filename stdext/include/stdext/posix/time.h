/* -------------------------------------------------------------------------
// WINX: a C++ template GUI library - MOST SIMPLE BUT EFFECTIVE
// 
// This file is a part of the WINX Library.
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/licenses/cpl.php)
// which can be found in the file CPL.txt at this distribution. By using
// this software in any fashion, you are agreeing to be bound by the terms
// of this license. You must not remove this notice, or any other, from
// this software.
// 
// Module: stdext/posix/time.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2010-2-2 1:20:48
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_POSIX_TIME_H
#define STDEXT_POSIX_TIME_H

#ifndef STDEXT_BASIC_H
#include "../Basic.h"
#endif

// -------------------------------------------------------------------------

#if defined(X_CC_VC)

	#ifndef _INC_TIME
	#include <time.h>
	#endif

	inline struct tm* localtime_r(const time_t* timep, struct tm* result)
	{
		localtime_s(result, timep);
		return result;
	}

#else
	#ifndef	_TIME_H
	#include <time.h>
	#endif
#endif

// -------------------------------------------------------------------------

#endif /* STDEXT_POSIX_TIME_H */
