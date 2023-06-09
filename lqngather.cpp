/*******************************************************************
*
*  DESCRIPTION: Atomic Model LQN Gather Connector
*
*  AUTHOR: Dorin Petriu
*  EMAIL: dorin@sce.carleton.ca
*
*  DATE: 2002-11-29
*  DATE: 2002-12-12  v2 with reply capability
*
*******************************************************************/

#include "lqngather.h"	// class LqnGather


LqnGather::LqnGather( const string &name ) : 
  Atomic( name ), 
  in0( addInputPort( "in0" ) ), 
  in1( addInputPort( "in1" ) ), 
  in2( addInputPort( "in2" ) ), 
  in3( addInputPort( "in3" ) ), 
  in4( addInputPort( "in4" ) ), 
  in5( addInputPort( "in5" ) ), 
  in6( addInputPort( "in6" ) ), 
  in7( addInputPort( "in7" ) ), 
  in8( addInputPort( "in8" ) ), 
  in9( addInputPort( "in9" ) ), 
  response( addInputPort( "response" ) ), 
  out( addOutputPort( "out" ) ),
  reply0( addOutputPort( "reply0" ) ),
  reply1( addOutputPort( "reply1" ) ),
  reply2( addOutputPort( "reply2" ) ),
  reply3( addOutputPort( "reply3" ) ),
  reply4( addOutputPort( "reply4" ) ),
  reply5( addOutputPort( "reply5" ) ),
  reply6( addOutputPort( "reply6" ) ),
  reply7( addOutputPort( "reply7" ) ),
  reply8( addOutputPort( "reply8" ) ),
  reply9( addOutputPort( "reply9" ) )
{
}


Model &LqnGather::initFunction()
{
  return *this ;
}


Model &LqnGather::externalFunction( const ExternalMessage &msg )
{
  // no reply is pending yet
  rep_pend = 0;

  // new response message arrives, so reply right away
  if( msg.port() == response ) {
    rep_dest = msg.value();
    rep_pend = 1;
    holdIn( active, Time::Zero );
  }
  // new call message arrives
  // resolve the incoming port and send right away
  else if( msg.port() == in0 ) {
    call_src = 0;
    call_val = msg.value();
    holdIn( active, Time::Zero );
  }
  else if( msg.port() == in1 ) {
    call_src = 1;
    call_val = msg.value();
    holdIn( active, Time::Zero );
  }
  else if( msg.port() == in2 ) {
    call_src = 2;
    call_val = msg.value();
    holdIn( active, Time::Zero );
  }
  else if( msg.port() == in3 ) {
    call_src = 3;
    call_val = msg.value();
    holdIn( active, Time::Zero );
  }
  else if( msg.port() == in4 ) {
    call_src = 4;
    call_val = msg.value();
    holdIn( active, Time::Zero );
  }
  else if( msg.port() == in5 ) {
    call_src = 5;
    call_val = msg.value();
    holdIn( active, Time::Zero );
  }
  else if( msg.port() == in6 ) {
    call_src = 6;
    call_val = msg.value();
    holdIn( active, Time::Zero );
  }
  else if( msg.port() == in7 ) {
    call_src = 7;
    call_val = msg.value();
    holdIn( active, Time::Zero );
  }
  else if( msg.port() == in8 ) {
    call_src = 8;
    call_val = msg.value();
    holdIn( active, Time::Zero );
  }
  else if( msg.port() == in9 ) {
    call_src = 9;
    call_val = msg.value();
    holdIn( active, Time::Zero );
  }
  
  return *this;
}


Model &LqnGather::internalFunction( const InternalMessage & )
{
  passivate();
  return *this ;
}


Model &LqnGather::outputFunction( const InternalMessage &msg )
{
  // if a reply is pending
  if( rep_pend ) {
    // switch based on the destination and send the reply
    switch( static_cast< int >( rep_dest ) ) {
    case 0 :
      sendOutput( msg.time(), reply0, 0 );
      break;
    case 1 :
      sendOutput( msg.time(), reply1, 0 );
      break;
    case 2 :
      sendOutput( msg.time(), reply2, 0 );
      break;
    case 3 :
      sendOutput( msg.time(), reply3, 0 );
      break;
    case 4 :
      sendOutput( msg.time(), reply4, 0 );
      break;
    case 5 :
      sendOutput( msg.time(), reply5, 0 );
      break;
    case 6 :
      sendOutput( msg.time(), reply6, 0 );
      break;
    case 7 :
      sendOutput( msg.time(), reply7, 0 );
      break;
    case 8 :
      sendOutput( msg.time(), reply8, 0 );
      break;
    case 9 :
      sendOutput( msg.time(), reply9, 0 );
      break;
    default:
      // do not reply to anything, show error message
      cout << endl
           << "ERROR LqnGather: cannot reply to source " << rep_dest << "!"
           << endl << "(index of source replied to must be between 0 and " 
           << MAXCONN - 1 << ")" << endl;
      break;
    }
    // a reply is no longer pending
    rep_pend = 0;
  }
  // else a call is pending
  else {
    sendOutput( msg.time(), out, call_src );
    sendOutput( msg.time(), out, call_val );
  }

  return *this ;
}
