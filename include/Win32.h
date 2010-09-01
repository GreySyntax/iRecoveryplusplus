/*
 * Win32.h
 *
 *  Created on: Sep 1, 2010
 *      Author: jordan
 */

#ifndef WIN32_H_
#define WIN32_H_

/* Taken from: http://svn.gnumonks.org/trunk/scez/sio/platform.h
 Try and figure out if we're running under Windows and/or Win32.  We have
 to jump through all sorts of hoops later on, not helped by the fact that
 the method of detecting Windows at compile time changes with different
 versions of Visual C (it's different for each of VC 2.0, 2.1, 4.0, and
 4.1.  It actually remains the same from 4.1 to 4.2) */

#if !defined( __WINDOWS__ ) && ( defined( _Windows ) || defined( _WINDOWS ) )
	#define __WINDOWS__
#endif /* !__WINDOWS__ && ( _Windows || _WINDOWS ) */

#if !defined( __WIN32__ ) && ( defined( WIN32 ) || defined( _WIN32 ) )
	#ifndef __WINDOWS__
		#define __WINDOWS__
	#endif /* __WINDOWS__ */
	#define __WIN32__
#endif /* !__WIN32__ && ( WIN32 || _WIN32 ) */

#if defined( __WINDOWS__ ) && !defined( __WIN32__ )
	#define __WIN16__
#endif /* __WINDOWS__ && !__WIN32__ */

/* and now I map it to my version of the above define --tjh */
#ifdef __WINDOWS__
	#ifndef WINDOWS
		#define WINDOWS
	#endif
#endif /* __WINDOWS__ */


#endif /* WIN32_H_ */
