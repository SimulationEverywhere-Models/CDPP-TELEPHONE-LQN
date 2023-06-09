/*******************************************************************
*
*  DESCRIPTION: Atomic Model LQN Queue
*
*  AUTHOR: Dorin Petriu
*  EMAIL: dorin@sce.carleton.ca
*
*  BASED ON: Queue model by Amir Barylko & Jorge Beyoglonian 
*
*  DATE: 2002-11-29
*  DATE: 2002-12-12  v2 with reply capability
*
*******************************************************************/

#ifndef _LQNQUEUE_H
#define _LQNQUEUE_H

#include "lqndefs.h"

class LqnQueue : public Atomic
{
public:
  // default constructor
  LqnQueue( const string &name = "LqnQueue" );

  virtual string className() const ;

protected:
  Model &initFunction();
  Model &externalFunction( const ExternalMessage & );
  Model &internalFunction( const InternalMessage & );
  Model &outputFunction( const InternalMessage & );

private:
  // input ports
  const Port &in;
  const Port &response;
  const Port &ready;
  // output ports
  Port &out;
  Port &reply;
  Port &averagesize;
  Port &averagewait;
  // Port &sumwait;

  int rep_pend;  // flag indicating that a reply is pending
  int call_pend;  // flag indicating that a call is pending processing

  Value rep_dest;  // the destination for the reply
  
  Value lqnmsg[ LQNFIELDS ];  // LQN message
  int fnum;  // index into the LQN message

  // queues
  time_queue_type time_queue;
  val_queue_type src_queue;
  val_queue_type val_queue;

  // ready to process flag
  int ready_to_process;
  // number of elements that passed through the queue
  Value elements_through;
  // cumulative wait time in ms for all the elements in the queue
  double sum_wait;
  // average queue size
  double av_size;
  // time that the queue last changed
  Time last_qxtime;
};
// class LqnQueue


// inline class name
inline
string LqnQueue::className() const
{
  return "LqnQueue" ;
}

#endif  //_LQNQUEUE_H
