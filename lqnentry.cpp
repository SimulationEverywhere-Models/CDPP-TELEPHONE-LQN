/*******************************************************************
*
*  DESCRIPTION: Atomic Model LQN Entry
*  AUTHOR: Dorin Petriu
*  EMAIL: dorin@sce.carleton.ca
*
*  DATE: 2002-12-2
*
*******************************************************************/

#include "lqnentry.h"  // class LqnEntry
#include "randlib.h"


LqnEntry::LqnEntry( const string &name ) : 
  Atomic( name ), 
  in( addInputPort( "in" ) ), 
  procrtn( addInputPort( "procrtn" ) ), 
  servrtn( addInputPort( "servrtn" ) ), 
  initproc( addInputPort( "initproc" ) ), 
  initserv( addInputPort( "initserv" ) ), 
  reply( addOutputPort( "reply" ) ), 
  ready( addOutputPort( "ready" ) ), 
  proccall( addOutputPort( "proccall" ) ), 
  servcall( addOutputPort( "servcall" ) ), 
  avservtime( addOutputPort( "avservtime" ) ),
  avph1time( addOutputPort( "avph1time" ) ),
  avph2time( addOutputPort( "avph2time" ) ),
  throughput( addOutputPort( "throughput" ) ),
  utilization( addOutputPort( "utilization" ) )
{
}


Model &LqnEntry::initFunction()
{
  // initialize the processor call initialization message
  for( pfnum = 0; pfnum < PROCINITFIELDS; pfnum++ ) {
    procinitmsg[ pfnum ] = 0;
  }
  pfnum = 0;

  // initialize the server call initialization message
  for( sfnum = 0; sfnum < SERVINITFIELDS; sfnum++ ) {
    servinitmsg[ sfnum ] = 0;
  }
  sfnum = 0;

  // initialize the processor service demands and calls to 0
  ph1_proc = 0;
  ph2_proc = 0;

  for( int i = 0; i < MAXCALLS; i++ ) {
    ph1_cs[ i ] = 0;
    ph2_cs[ i ] = 0;
  }
  ph1_cs_num = 0;
  ph2_cs_num = 0;

  ph1_call_tot = 0;
  ph2_call_tot = 0;

  // initialize the performance metrics to 0
  jobs_through = 0;
  entry_busy_time = 0;
  ph1_busy_time = 0;
  ph2_busy_time = 0;
  ph1_serv_time = 0;
  ph2_serv_time = 0;

  last_arrival_time = "0:0:0:0";
  last_reply_time = "0:0:0:0";

  // everything is initially done
  ph1_proc_done = 1;
  ph2_proc_done = 1;
  ph1_call_done = 1;
  ph2_call_done = 1;
  ph1_done = 1;
  ph2_done = 1;
  entry_done = 1;

  wait_proc = 0;  // not waiting for the processor
  wait_call = 0;  // not waiting for any calls

/*
 *  // test the random generation
 *  double mean = 0;
 *  double exp = 0;
 *  double obs_mean = 0;
 *  for( int i = 0; i < 10; i++ ) {
 *    mean += 10;
 *    exp = genexplqn( mean );
 *    obs_mean = exp;
 *
 *    cout << "exponential - mean " << mean;
 *    // cout << ":\t" << exp << endl;
 *
 *    for( int j = 1; j < 100000; j++ ) {
 *      exp = genexplqn( mean );
 *      obs_mean = ( ( obs_mean * j ) + exp ) / ( j + 1 );
 *      // cout << "                      \t" << exp << endl;
 *    }
 *
 *    cout << " - observed mean " << obs_mean << endl;
 *  }
 */

  return *this ;
}


Model &LqnEntry::externalFunction( const ExternalMessage &msg )
{
  // if the entry is not done (currently busy)
  if( !entry_done ) {
    // if processor call return and waiting for the processor
    if( ( msg.port() == procrtn ) && wait_proc ) {
      wait_proc = 0;  // stop waiting

      // if phase1 processing is not done
      if( !ph1_proc_done ) {
        // phase1 processing is now done
        ph1_proc_done = 1;
      }
      // else if phase2 processing is not done
      else if( !ph2_done ) {
        // phase2 processing is now done
        ph2_proc_done = 1;
      }

      // update done flags
      // phase is done if both processing and calls are done
      ph1_done = ph1_proc_done && ph1_call_done;
      ph2_done = ph2_proc_done && ph2_call_done;

      // entry is done if phase1 and phase2 are both done
      entry_done = ph1_done && ph2_done ;

      // process right away
      holdIn( active, Time( 0 ) );
    }
    // else if server call return and waiting for a call
    else if( ( msg.port() == servrtn ) && wait_call ) {
      wait_call = 0;  // stop waiting

      // if phase1 calls are not done
      // and calls made are equal to the total number of calls to be made
      if( !ph1_call_done && ( ph1_calls_made == ph1_call_tot ) ) {
        // phase1 calls are now done
        ph1_call_done = 1;
      }
      // else if phase2 calls are not done
      // and calls made are equal to the total number of calls to be made
      else if( !ph2_call_done && ( ph2_calls_made == ph2_call_tot ) ) {
        // phase2 calls are now done
        ph2_call_done = 1;
      }
      
      // update done flags
      // phase is done if both processing and calls are done
      ph1_done = ph1_proc_done && ph1_call_done;
      ph2_done = ph2_proc_done && ph2_call_done;

      // entry is done if phase1 and phase2 are both done
      entry_done = ph1_done && ph2_done;

      // process right away
      holdIn( active, Time( 0 ) );
    }
  }
  // else the entry is done (currently not busy)
  else {
    // if new job arrival
    if( msg.port() == in ) {
      cout << "[" << msg.time() << "] entry: " << "start" << endl;

      // remember the arrival time
      last_arrival_time = msg.time();

      // set the number of calls made to 0
      ph1_calls_made = 0;
      ph2_calls_made = 0;
      server_calls_made = 0;
      // start with the first call statement for each phase
      ph1_cs_i = 0;
      ph2_cs_i = 0;

      // set the flags
      // processing is not done if there are processor calls to be made
      ph1_proc_done = !( ph1_proc );
      ph2_proc_done = !( ph2_proc );
      // calls are not done if there are server calls to be made
      ph1_call_done = !( ph1_call_tot );
      ph2_call_done = !( ph2_call_tot );
      // phase is done if both processing and calls are done
      ph1_done = ph1_proc_done && ph1_call_done;
      ph2_done = ph2_proc_done && ph2_call_done;
      // entry is done if phase1 and phase2 are both done
      entry_done = ph1_done && ph2_done;
      // not replied yet
      replied = 0;

      // process right away
      holdIn( active, Time( 0 ) );
    }
    // else if processor call init
    else if( msg.port() == initproc ) {
      // if pfnum is greater than or equal to the number of proc init fields
      if( pfnum >= PROCINITFIELDS ) {
	// reset pfnum to 0
	pfnum = 0;
      }

      // assemble the initialization fields in order
      procinitmsg[ pfnum ] = msg.value();
      pfnum++;

      // if all the processor call fields have been received
      if( pfnum == PROCINITFIELDS ) {
	// switch on the phase number
	switch( static_cast< int >( procinitmsg[ PHASE ] ) ) {
	case 1 :
	  // initialize the phase1 service time
	  ph1_proc = procinitmsg[ VALUE ];

          cout << "[" << msg.time() << "] entry: " 
               << "init phase1 proc demand mean = " << ph1_proc << " ms" 
               << endl;
	  break;
	case 2 :
	  // initialize the phase2 service time
	  ph2_proc = procinitmsg[ VALUE ];

          cout << "[" << msg.time() << "] entry: " 
               << "init phase2 proc demand mean = " << ph2_proc << " ms"
               << endl;
	  break;
	default:
	  // invalid phase number error message, if so desired
          cout << "[" << msg.time() << "] ERROR entry: " 
               << "init invalid phase " << procinitmsg[ PHASE ] 
               << ", only phases 1 and 2 are valid!" << endl;
	  break;
	}
      }
    }
    // else if server call init
    else if( msg.port() == initserv ) {
      // if sfnum is larger than or equal to the number of call init fields
      if( sfnum >= SERVINITFIELDS ) {
	// reset sfnum to 0
	sfnum = 0;
      }

      // assemble the initialization fields in order
      servinitmsg[ sfnum ] = msg.value();
      sfnum++;

      // if all the call init fields have been received
      if( sfnum == SERVINITFIELDS ) {
	// if the call target is within the acceptable range
	if( ( servinitmsg[ TARGET ] >= 0 ) && 
	    ( servinitmsg[ TARGET ] < MAXCONN ) ) {
	  // switch on the phase number to make the call
	  switch( static_cast< int >( servinitmsg[ PHASE ] ) ) {
	  case 1 :
	    // if the phase1 call statements are not all filled up
	    if( ph1_cs_num < MAXCALLS ) {
	      // add a server and a call statement for phase1
	      ph1_servers[ ph1_cs_num ] = servinitmsg[ TARGET ];
	      ph1_cs[ ph1_cs_num ] = servinitmsg[ VALUE ];
	      // add to the total number of calls to be made by phase1
	      ph1_call_tot += servinitmsg[ VALUE ];
	      // increment the number of call statements
	      ph1_cs_num++;

              cout << "[" << msg.time() << "] entry: " 
                   << "init phase1 call statement " << ph1_cs_num
                   << " = " << servinitmsg[ VALUE ] << " calls to server "
                   << servinitmsg[ TARGET ] << endl;
	    }
	    // else phase1 call statements are all filled up
	    break;
	  case 2 :
	    // if the phase2 call statements are not all filled up
	    if( ph2_cs_num < MAXCALLS ) {
	      // add a server and a call statement for phase2
	      ph2_servers[ ph2_cs_num ] = servinitmsg[ TARGET ];
	      ph2_cs[ ph2_cs_num ] = servinitmsg[ VALUE ];
	      // add to the total number of calls to be made by phase2
	      ph2_call_tot += servinitmsg[ VALUE ];
	      // increment the number of call statements
	      ph2_cs_num++;

              cout << "[" << msg.time() << "] entry: " 
                   << "init phase2 call statement " << ph2_cs_num
                   << " = " << servinitmsg[ VALUE ] << " calls to server "
                   << servinitmsg[ TARGET ] << endl;
	    }
	    // else phase2 call statements are all filled up
	    break;
	  default:
	    // invalid phase number error message if desired
            cout << "[" << msg.time() << "] ERROR entry: " 
                 << "init invalid phase " << procinitmsg[ PHASE ] 
                 << ", only phases 1 and 2 are valid!" << endl;
	    break;
	  }
	}
      }
    }
  }

  return *this;
}


Model &LqnEntry::internalFunction( const InternalMessage & )
{
  passivate();
  return *this ;
}


Model &LqnEntry::outputFunction( const InternalMessage &msg )
{
  int msg_sent = 0;  // no message has been sent yet

  // if the entry is not done (currently busy)
  if( !entry_done ) {
    // if phase1 is not done
    if( !ph1_done ) {
      // if phase1 processing is not done
      if( !ph1_proc_done ) {
        // make a call to the processor
        double ph1_proc_exp = genexplqn( ph1_proc );
        sendOutput( msg.time(), proccall, ph1_proc_exp );
        msg_sent = 1;
        wait_proc = 1;

        cout << "[" << msg.time() << "] entry: "
             << "phase1 proc call <mean " <<  ph1_proc
             << " ms, actual " << ph1_proc_exp << " ms>" << endl;
      }
      // else if phase1 calls are not done
      else if( !ph1_call_done ) {
        // if there are more call statements to go through
        if( ph1_cs_i < ph1_cs_num ) {
          // if there are more calls to make to this target
          if( server_calls_made < ph1_cs[ ph1_cs_i ] ) {
            // make call to the appropriate server
            sendOutput( msg.time(), servcall, ph1_servers[ ph1_cs_i ] );
            msg_sent = 1;
            wait_call = 1;

            cout << "[" << msg.time() << "] entry: " 
                 << "phase1 server call <server " << ph1_servers[ ph1_cs_i ]
                 << ">" << endl;

            // advance the call counters
            server_calls_made++;
            ph1_calls_made++;
          }
          // if all the calls to this target have been made
          if( server_calls_made == ph1_cs[ ph1_cs_i ] ) {
            // advance to the next call statement
            ph1_cs_i++;
            // reset the number of calls made to the server
            server_calls_made = 0;
          }
        }
      }
    }
    // else phase1 is done
    else {
      // if a reply has not been sent
      if( !replied ) {
	// calculate the phase1 service and busy times
        ph1_serv_time = ( msg.time() - last_arrival_time ).asMsecs();
	ph1_busy_time += ph1_serv_time;
	// remember the reply time
        last_reply_time = msg.time();

        // reply now with the phase1 service time
        sendOutput( msg.time(), reply, ph1_serv_time );
        msg_sent = 1;
        replied = 1;

        cout << "[" << msg.time() << "] entry: " << "reply" << endl;
      }

      // if phase2 is not done
      if( !ph2_done ) {
        // if processing is not done
        if( !ph2_proc_done ) {
          // make call to the processor
          double ph2_proc_exp = genexplqn( ph2_proc );
          sendOutput( msg.time(), proccall, ph2_proc_exp );
          msg_sent = 1;
          wait_proc = 1;

          cout << "[" << msg.time() << "] entry: "
               << "phase2 proc call <mean " <<  ph2_proc
               << " ms, actual " << ph2_proc_exp << " ms>" << endl;
        }
        // else if calls are not done
        else if( !ph2_call_done ) {
          // if there are more call statements to go through
          if( ph2_cs_i < ph2_cs_num ) {
            // if there are more calls to make to this target
            if( server_calls_made < ph2_cs[ ph2_cs_i ] ) {
	      // make call to the appropriate server
	      sendOutput( msg.time(), servcall, ph2_servers[ ph2_cs_i ] );
	      msg_sent = 1;
	      wait_call = 1;

              cout << "[" << msg.time() << "] entry: " 
                   << "phase2 server call <server " << ph2_servers[ ph2_cs_i ]
                   << ">" << endl;

	      // advance the call counters
	      server_calls_made++;
              ph2_calls_made++;
            }
            // if all the calls to this target have been made
            if( server_calls_made == ph2_cs[ ph2_cs_i ] ) {
              // advance to the next call statement
              ph2_cs_i++;
	      // reset the number of calls made to the server
              server_calls_made = 0;
            }
          }
        }
      }
    }
  }
  // else currently not busy
  else {
    // if a reply has not been sent
    if( !replied ) {
      // calculate the phase1 service and busy times
      ph1_serv_time = ( msg.time() - last_arrival_time ).asMsecs();
      ph1_busy_time += ph1_serv_time;
      // remember the reply time
      last_reply_time = msg.time();

      // reply now with the phase1 service time
      sendOutput( msg.time(), reply, ph1_serv_time );
      msg_sent = 1;
      replied = 1;

      cout << "[" << msg.time() << "] entry: " << "reply" << endl;
    }

    // all done, calculate the performance metrics
    // calculate the phase2 service and busy times
    ph2_serv_time = ( msg.time() - last_reply_time ).asMsecs();
    ph2_busy_time += ph2_serv_time;

    // calculate the entry busy time
    entry_busy_time += ph1_serv_time + ph2_serv_time;
    // increment the number of jobs that have gone through
    jobs_through++;

    // send the average service time in ms
    // as the total busy time / number of jobs
    sendOutput( msg.time(), avservtime, entry_busy_time / jobs_through );
    sendOutput( msg.time(), avph1time, ph1_busy_time / jobs_through );
    sendOutput( msg.time(), avph2time, ph2_busy_time / jobs_through );
    // send the throughput per ms
    // as the number of jobs / elapsed time
    sendOutput( msg.time(), throughput, jobs_through / msg.time().asMsecs() );
    // send the utilization as entry busy time / elapsed time
    sendOutput( msg.time(), utilization, 
                entry_busy_time / msg.time().asMsecs() );

    // indicate ready for more jobs
    sendOutput( msg.time(), ready, 1 );
    msg_sent = 1;

    cout << "[" << msg.time() << "] entry: "
         << "done <phase1 " << ph1_serv_time << " ms, phase2 "
         << ph2_serv_time << " ms>" << endl;
  }

  // if no message has been sent
  if( !msg_sent ) {
    // error message
    cout << "[" << msg.time() 
         << "] entry ERROR: output function did not send a message!" << endl;
  }

  return *this ;
}
