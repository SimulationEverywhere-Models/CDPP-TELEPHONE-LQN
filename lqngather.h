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

#ifndef _LQNGATHER_H
#define _LQNGATHER_H

#include "lqndefs.h"

class LqnGather : public Atomic
{
public:
  // default constructor
  LqnGather( const string &name = "LqnGather" );

  virtual string className() const ;

protected:
  Model &initFunction();
  Model &externalFunction( const ExternalMessage & );
  Model &internalFunction( const InternalMessage & );
  Model &outputFunction( const InternalMessage & );

private:
  // input ports
  const Port &in0;
  const Port &in1;
  const Port &in2;
  const Port &in3;
  const Port &in4;
  const Port &in5;
  const Port &in6;
  const Port &in7;
  const Port &in8;
  const Port &in9;
  const Port &response;
  // output ports
  Port &out;
  Port &reply0;
  Port &reply1;
  Port &reply2;
  Port &reply3;
  Port &reply4;
  Port &reply5;
  Port &reply6;
  Port &reply7;
  Port &reply8;
  Port &reply9;

  Value call_src;  // source of the call
  Value call_val;  // value of the call

  int rep_pend;  // flag indicating that a reply is pending
  Value rep_dest;  // destination of the reply
};
// class LqnGather


// inline class name
inline
string LqnGather::className() const
{
  return "LqnGather" ;
}

#endif  // _LQNGATHER_H
