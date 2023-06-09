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

#include "lqnqueue.h"	// class LqnQueue


LqnQueue::LqnQueue( const string &name ) : 
  Atomic( name ), 
  in( addInputPort( "in" ) ), 
  response( addInputPort( "response" ) ), 
  ready( addInputPort( "ready" ) ), 
  out( addOutputPort( "out" ) ), 
  reply( addOutputPort( "reply" ) ), 
  averagesize( addOutputPort( "averagesize" ) ),
  averagewait( addOutputPort( "averagewait" ) )
  // sumwait( addOutputPort( "sumwait" ) )
{
}


Model &LqnQueue::initFunction()
{
  // initialize the LQN message
  for( fnum = 0; fnum < LQNFIELDS; fnum++ ) {
    lqnmsg[ fnum ] = 0;
  }
  fnum = 0;

  // make sure the queues are empty
  time_queue.erase( time_queue.begin(), time_queue.end() ) ;
  val_queue.erase( val_queue.begin(), val_queue.end() ) ;
  src_queue.erase( src_queue.begin(), src_queue.end() ) ;

  // initialize the queue counters
  elements_through = 0;
  sum_wait = 0;
  av_size = 0;
  last_qxtime = "0:0:0:0";

  rep_dest = 0;

  // assume that to begin with the entity is ready
  ready_to_process = 1;

  // no reply or call is pending yet
  rep_pend = 0;
  call_pend = 0;

  return *this ;
}


Model &LqnQueue::externalFunction( const ExternalMessage &msg )
{
  // new call arrives at the queue
  if( msg.port() == in ) {
    // if fnum is larger than or equal to the number of LQN fields
    if( fnum >= LQNFIELDS ) {
      // reset fnum to 0
      fnum = 0;
    }

    // assemble the LQN fields in order
    lqnmsg[ fnum ] = msg.value();
    fnum++;

    // if all the fields have been received
    if( fnum == LQNFIELDS ) {
      // calculate the average queue size up to now
      av_size = ( ( av_size * last_qxtime.asMsecs() ) + 
                val_queue.size() * ( msg.time() - last_qxtime ).asMsecs() ) / 
	        msg.time().asMsecs();

      // add to the back of the queues
      time_queue.push_back( msg.time() );
      src_queue.push_back( lqnmsg[ LQNSRC ] );
      val_queue.push_back( lqnmsg[ LQNVAL ] );

      // update the last queue change time
      last_qxtime = msg.time();

      // if the processing entity is ready
      if( ready_to_process ) {
	// no longer ready to process
	ready_to_process = 0;
	// a call is ready to be processed
	call_pend = 1;
	// send right away
	holdIn( active, Time::Zero );
      }
    }
  }
  // reply message
  else if( msg.port() == response ) {
    // a reply is pending
    rep_pend = 1;
    // reply right away
    holdIn( active, Time::Zero );
  }
  // ready message
  else if( msg.port() == ready ) {
    // if there are elements waiting in the queue
    if( val_queue.size() ) {
      // not ready to process
      ready_to_process = 0;
      // a call is ready to be processed
      call_pend = 1;
      // send right away
      holdIn( active, Time::Zero );
    }
    // else the queue is empty
    else {
      // now ready to process
      ready_to_process = 1;
    }
  }

  return *this;
}


Model &LqnQueue::internalFunction( const InternalMessage & )
{
  passivate();
  return *this ;
}


Model &LqnQueue::outputFunction( const InternalMessage &msg )
{
  // if a reply is pending
  if( rep_pend ) {
    // send the reply
    sendOutput( msg.time(), reply, rep_dest );
    // a reply is no longer pending
    rep_pend = 0;
  }

  // if a call is ready to be processed
  if( call_pend ) {
    // increment the number of elements that have gone through the queue
    elements_through++;
    // add the element's waiting time in ms to the cumulative waiting time
    sum_wait += ( msg.time() - time_queue.front() ).asMsecs();
    // calculate the average queue size up to now
    av_size = ( ( av_size * last_qxtime.asMsecs() ) + 
              val_queue.size() * ( msg.time() - last_qxtime ).asMsecs() ) / 
              msg.time().asMsecs();

    // send the performance metrics out
    sendOutput( msg.time(), averagesize, av_size );
    sendOutput( msg.time(), averagewait, sum_wait / elements_through );
    // sendOutput( msg.time(), sumwait, sum_wait );

    // send the call value to be processed
    sendOutput( msg.time(), out, val_queue.front() );

    // use the call's source as the reply destination
    rep_dest = src_queue.front();

    // de-queue the first elements
    time_queue.pop_front();
    src_queue.pop_front();
    val_queue.pop_front();
    // update the last queue change time
    last_qxtime = msg.time();

    // a call is no longer ready to be processed
    call_pend = 0;
  }

  return *this ;
}
