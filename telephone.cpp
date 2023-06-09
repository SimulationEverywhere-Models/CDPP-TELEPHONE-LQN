/*******************************************************************
*
*  DESCRIPTION: Atomic Model Telephone Set
*
*  AUTHOR: Dorin Petriu
*  EMAIL: dorin@sce.carleton.ca
*
*  DATE: 2002-12-24
*
*******************************************************************/

#include "Telephone.h"	// class Telephone


Telephone::Telephone( const string &name ) : 
  Atomic( name ), 
  handset( addInputPort( "handset" ) ), 
  dialnum( addInputPort( "dialnum" ) ), 
  fromswitch( addInputPort( "fromswitch" ) ), 
  dialtone( addOutputPort( "dialtone" ) ), 
  ring( addOutputPort( "ring" ) ), 
  remotering( addOutputPort( "remotering" ) ), 
  busy( addOutputPort( "busy" ) ), 
  wrongnum( addOutputPort( "wrongnum" ) ), 
  connect( addOutputPort( "connect" ) ), 
  toswitch( addOutputPort( "toswitch" ) ), 
  callnum( addOutputPort( "callnum" ) )
{
}


Model &Telephone::initFunction()
{
  // set the dialing delay to 100 ms
  dial_delay = 100.0;

  // no number has been called yet
  called_num = 0;

  // set the flags
  handsetpos = ONHOOK;
  sendmsg = NOSIGNAL;
  status = NOATTEMPT;

  cout << endl;

  return *this ;
}


Model &Telephone::externalFunction( const ExternalMessage &msg )
{
  sendmsg = NOSIGNAL;

  // user input
  if( msg.port() == handset ) {
    // switch on the handset position
    switch( static_cast< int >( msg.value() ) ) {
    case ONHOOK :
      // if currently OFFHOOK
      if( handsetpos == OFFHOOK ) {
        cout << "[" << msg.time() << "] telephone: ONHOOK from user"
             << endl << endl;

        // set ONHOOK
        handsetpos = ONHOOK;
        sendmsg = ONHOOK;
        // process message
        holdIn( active, Time::Zero );
      }
      // else ignore the message
      else {
        cout << "[" << msg.time() << "] telephone: ignoring ONHOOK from user"
             << endl;
      }
      break;

    case OFFHOOK :
      // if currently ONHOOK
      if( handsetpos == ONHOOK ) {
        cout << "[" << msg.time() << "] telephone: OFFHOOK from user" << endl;

        // set OFFHOOK
        handsetpos = OFFHOOK;
        sendmsg = OFFHOOK;
        // process message
        holdIn( active, Time::Zero );
      }
      // else ignore
      else {
        cout << "[" << msg.time() << "] telephone: ignoring OFFHOOK from user"
             << endl;
      }
      break;

    default:
      // show error message
      cout << endl 
           << "ERROR telephone: invalid handset position "
           << msg.value() << "!" << endl
           << "(handset position must be 0 for ONHOOK or 1 for OFFHOOK)" 
           << endl << endl;
      break;
    }
  }
  // dial a number
  else if( msg.port() == dialnum ) {
    // if got a dial tone
    if( status == DIALTONE ) {
      // remember the called number
      called_num = msg.value(); 

      cout << "[" << msg.time() << "] telephone: NUMBER \"" << called_num 
           << "\" dialed from user" << endl;

      // send the number to the switch
      sendmsg = NUMBER;
      holdIn( active, Time( static_cast< float >( dial_delay ) ));
    }
    // else ignore the message
    else {
      cout << "[" << msg.time() << "] telephone: ignoring NUMBER \""
           << msg.value() << "\" dialed from user" << endl;
    }
  }
  // signal from the switch
  else if( msg.port() == fromswitch ) {
    // cout << "[" << msg.time() << "] telephone: signal " << msg.value()
    //     << " from switch" << endl;

    // switch on the switch signal
    switch( static_cast< int >( msg.value() ) ) {
    case DIALTONE :
      // if currently OFFHOOK and no connection attempted
      if( ( handsetpos == OFFHOOK ) && ( status == NOATTEMPT ) ) {
        cout << "[" << msg.time() << "] telephone: DIALTONE from switch"
             << endl;

        // send dial tone to the user
        sendmsg = DIALTONE;
        holdIn( active, Time::Zero );
      }
      // else ignore the message
      else {
        cout << "[" << msg.time()
             << "] telephone: ignoring DIALTONE from switch" << endl;
      }
      break;

    case RING :
      // if currently ONHOOK
      if( handsetpos == ONHOOK ) {
        cout << "[" << msg.time() << "] telephone: RING RING RING..."
             << endl;

        // incoming call, brrrrrriiiiinnnnnnnnnnggggggg...  ;)
        sendmsg = RING;
        holdIn( active, Time::Zero );
      }
      // else ignore the message
      else {
        cout << "[" << msg.time() << "] telephone: ignoring RING from switch"
             << endl;
      }
      break;

    case REMOTERING :
      // if number was dialed
      if( status == NUMBER ) {
        cout << "[" << msg.time()
             << "] telephone: REMOTERING tone from switch" << endl;

        // send remote ring tone to the user, wait for a connection
        sendmsg = REMOTERING;
        holdIn( active, Time::Zero );
      }
      // else ignore the message
      else {
        cout << "[" << msg.time() 
             << "] telephone: ignoring REMOTERING from switch" << endl;
      }
      break;

    case BUSY :
      // if number was dialed
      if( status == NUMBER ) {
        cout << "[" << msg.time() << "] telephone: BUSY tone from switch"
             << endl;

        // send busy tone to the user
        sendmsg = BUSY;
        holdIn( active, Time::Zero );
      }
      // else ignore the message
      else {
        cout << "[" << msg.time() << "] telephone: ignoring BUSY from switch"
             << endl;
      }
      break;

    case WRONGNUM :
      // if number was dialed
      if( status == NUMBER ) {
        cout << "[" << msg.time() << "] telephone: WRONGNUM tone from switch"
             << endl;

        // sent wrong number to the user
        sendmsg = WRONGNUM;
        holdIn( active, Time::Zero );
      }
      // else ignore the message
      else {
        cout << "[" << msg.time() 
             << "] telephone: ignoring WRONGNUM from switch" << endl;
      }
      break;

    case CONNECT :
      // if waiting for a connection
      if( ( status == RING ) || ( status == REMOTERING ) ) {
        cout << "[" << msg.time() << "] telephone: CONNECT from switch"
             << endl;

        // connected, can now talk
        sendmsg = CONNECT;
        holdIn( active, Time::Zero );
      }
      // else ignore the message
      else {
        cout << "[" << msg.time()
             << "] telephone: ignoring CONNECT from switch" << endl;
      }
      break;

    case DISCONNECT :
      // if connected
      if( status == CONNECT ) {
        cout << "[" << msg.time() << "] telephone: DISCONNECT from switch"
             << endl;

        // send disconnect to the user
        sendmsg = DISCONNECT;
        holdIn( active, Time::Zero );
      }
      // else ignore the message
      else {
        cout << "[" << msg.time()
             << "] telephone: ignoring DISCONNECT from switch" << endl;
      }
      break;

    default:
      // show error message
      cout << endl 
           << "ERROR telephone: invalid switch signal "
           << msg.value() << "!" << endl
           << "signals must be: " << DIALTONE << " for DIALTONE" << endl 
           << "                 " << RING << " for RING" << endl 
           << "                 " << REMOTERING << " for REMOTERING" << endl 
           << "                 " << BUSY << " for BUSY" << endl 
           << "                 " << WRONGNUM << " for WRONGNUM" << endl 
           << "                 " << CONNECT << " for CONNECT" << endl 
           << "                 " << DISCONNECT << " for DISCONNECT" << endl
           << endl;
      break;
    }
  }

  return *this;
}


Model &Telephone::internalFunction( const InternalMessage & )
{
  passivate();
  return *this ;
}


Model &Telephone::outputFunction( const InternalMessage &msg )
{
  // switch on the message to send
  switch( sendmsg ) {
  case ONHOOK :
    switch( status ) {
      case DIALTONE :
        // stop the dial tone
        sendOutput( msg.time(), dialtone, STOP );
        break;

      case REMOTERING :
        // stop the remote ringing tone
        sendOutput( msg.time(), remotering, STOP );
        break;

      case BUSY :
        // stop the busy tone
        sendOutput( msg.time(), busy, STOP );
        break;

      case WRONGNUM :
        // stop the wrong number tone
        sendOutput( msg.time(), wrongnum, STOP );
        break;

      case CONNECT :
        // stop the connection
        sendOutput( msg.time(), connect, STOP );
        break;
    }
    // send ONHOOK to the switch
    sendOutput( msg.time(), toswitch, ONHOOK );

    // set the status flag
    status = NOATTEMPT;
    break;

  case OFFHOOK :
    // if the phone is ringing
    if( status == RING ) {
      // stop the phone ringing
      sendOutput( msg.time(), ring, STOP );
    }
    // send OFFHOOK to the switch
    sendOutput( msg.time(), toswitch, OFFHOOK );
    break;

  case NUMBER :
    // stop the dial tone
    sendOutput( msg.time(), dialtone, STOP );
    // send the dialed number to the switch
    sendOutput( msg.time(), callnum, called_num );
    // set the status flag
    status = NUMBER;
    break;

  case DIALTONE :
    // start the dial tone
    sendOutput( msg.time(), dialtone, START );
    // set the status flag
    status = DIALTONE;
    break;

  case RING :
    // start the phone ringing
    sendOutput( msg.time(), ring, START );
    // set the status flag
    status = RING;
    break;

  case REMOTERING :
    // start the remote ringing tone
    sendOutput( msg.time(), remotering, START );
    // set the status flag
    status = REMOTERING;
    break;

  case BUSY :
    // start the busy tone
    sendOutput( msg.time(), busy, START );
    // set the status flag
    status = BUSY;
    break;

  case WRONGNUM :
    // start the wrong number tone
    sendOutput( msg.time(), wrongnum, START );
    // set the status flag
    status = WRONGNUM;
    break;

  case CONNECT :
    switch( status ) {
      case RING:
        // stop the ringing
        sendOutput( msg.time(), ring, STOP );
        break;

      case REMOTERING:
        // stop the remote ringing tone
        sendOutput( msg.time(), remotering, STOP );
        break;
    }
    // start the connection
    sendOutput( msg.time(), connect, START );
    // set the status flag
    status = CONNECT;
    break;

  case DISCONNECT :
    // stop the connection
    sendOutput( msg.time(), connect, STOP );
    // set the status flag
    status = DISCONNECT;
    break;
  }

  // set the send message flag
  sendmsg = NOSIGNAL;

  return *this ;
}
