/*******************************************************************
*
*    DESCRIPTION: main
*
*    DATE: 12/11/99
*
*******************************************************************/

/** include files **/
#include <iostream.h>
#include "stdaload.h"   // class StandAloneLoader
#include "netload.h"    // class NetLoader
#include "mainsimu.h"   // class MainSimulator

int main( int argc, const char *argv[] ) 
{

	try
	{
		SimLoader *loader;

		#ifndef __unix__
		loader = new StandAloneLoader( argc, argv );
		#else
			#ifdef __ONLY_STAND_ALONE_MODE__
				loader = new StandAloneLoader( argc, argv );
			#else
				if( argc > 1 )
					loader = new StandAloneLoader( argc, argv ) ;
				else
					loader = new NetworkLoader() ;
			#endif
		#endif

		MainSimulator::Instance().loader( loader );

		MainSimulator::Instance().run();

	} catch( const MException &e )
	{
		e.print(cerr);
	} catch( ... )
	{
		cerr << "Unknown Exception!!! "  << endl; 
	}
	return 0;
}
