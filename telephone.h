/*******************************************************************
*
*  DESCRIPTION: Atomic Model LQN Telephone Set
*
*  AUTHOR: Dorin Petriu
*  EMAIL: dorin@sce.carleton.ca
*
*  DATE: 2002-12-24
*
*******************************************************************/

#ifndef _TELEPHONE_H
#define _TELEPHONE_H

#include "potsdefs.h"

class Telephone : public Atomic
{
public:
  // default constructor
  Telephone( const string &name = "Telephone" );

  virtual string className() const ;

protected:
  Model &initFunction();
  Model &externalFunction( const ExternalMessage & );
  Model &internalFunction( const InternalMessage & );
  Model &outputFunction( const InternalMessage & );

private:
  // signals and message value are defined in potsdefs.h
  // input ports from the user
  const Port &handset;  // telephone handset being picked up or put down
                        // 0 indicates ONHOOK (handset put down)
                        // 1 indicates OFFHOOK (handset picked up)
  const Port &dialnum;  // dialed number indicated by the message value
  // input port from the switch
  const Port &fromswitch;  // message value can be one of
                           // { DIALTONE, RING, CONNECT, BUSY, WRONGNUM }
  // output ports to the user
  Port &dialtone;  // dial tone, value can be { STOP, START }
  Port &ring;  // telephone ringing, value can be { STOP, START }
  Port &remotering;  // ringing tone on the remote end,
                     // value can be { STOP, START }
  Port &busy;  // busy tone, value can be { STOP, START }
  Port &wrongnum;  // wrong number tone, value can be { STOP, START }
  Port &connect;  // a connection has been established
  // output ports to the switch
  Port &toswitch;  // message value can be { ONHOOK, OFFHOOK }
  Port &callnum;  // message value is number being called

  // variables
  Value called_num;  // number being called
  Value dial_delay;  // delay in ms to assemble a number from the dialed digits

  // flags
  int handsetpos;  // handset position, can be one of { ONHOOK, OFFHOOK }
  int status;  // connection status, can be one of
               // { NOATTEMPT, NUMBER, DIALTONE, RING, BUSY, WRONGNUM,
               //   CONNECT, DISCONNECT }
  int sendmsg;  // send an output message, can be one of
                // { ONHOOK, OFFHOOK, NUMBER, DIALTONE, RING, BUSY, WRONGNUM,
                //   CONNECT, DISCONNECT, NOSIGNAL }
};
// class Telephone


// inline class name
inline
string Telephone::className() const
{
  return "Telephone" ;
}

#endif  //_TELEPHONE_H
