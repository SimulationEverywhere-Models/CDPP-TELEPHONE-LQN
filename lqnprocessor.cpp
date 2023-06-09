/*******************************************************************
*
*  DESCRIPTION: Atomic Model LQN Processor
*
*  AUTHOR: Dorin Petriu
*  EMAIL: dorin@sce.carleton.ca
*
*  DATE: 2002-11-29
*
*******************************************************************/

#include "lqnprocessor.h"	// class LqnProcessor


LqnProcessor::LqnProcessor( const string &name ) : 
  Atomic( name ), 
  in( addInputPort( "in" ) ), 
  reply( addOutputPort( "reply" ) ), 
  ready( addOutputPort( "ready" ) ), 
  throughput( addOutputPort( "throughput" ) ),
  utilization( addOutputPort( "utilization" ) )
{
}


Model &LqnProcessor::initFunction()
{
  jobs_through = 0;
  busy_time = 0;
  service_time = 0;
  last_arrival_time = "0:0:0:0";

  return *this ;
}


Model &LqnProcessor::externalFunction( const ExternalMessage &msg )
{
  Value serv_demand;  // service demand in ms
  Value serv_demand_rnd;  // service demand rounded to the nearest ms

  // new job arrival and currently idle
  if( ( msg.port() == in ) && ( this->state() == passive ) ) {
/*
 *    // need to divide the service demand by 1000 to actually obtain ms,
 *    // as the functions in time.h mistakenly create seconds instead of ms
 *    serv_demand = Time( static_cast< float >( msg.value() / 1000.0 ) );
 */
    // remember the arrival time
    last_arrival_time = msg.time();
    // get the service demand, round it to the nearest ms
    serv_demand = msg.value();
    serv_demand_rnd = roundlqn( serv_demand );

    cout << "[" << msg.time() << "] processor: <demand " << serv_demand
         << " ms, rounded to " << serv_demand_rnd << " ms>" << endl;

    // process for the amount of time specified by the demand
    holdIn( active, Time( static_cast< float >( serv_demand_rnd  ) ));
  }
  // else currently busy, so ignore the new job

  return *this;
}


Model &LqnProcessor::internalFunction( const InternalMessage & )
{
  passivate();
  return *this ;
}


Model &LqnProcessor::outputFunction( const InternalMessage &msg )
{
  // done processing, calculate the performance metrics
  service_time = ( msg.time() - last_arrival_time ).asMsecs();
  busy_time += service_time;
  jobs_through++;

  // send the throughput per ms as the number of jobs / elapsed time up to now
  sendOutput( msg.time(), throughput, jobs_through / msg.time().asMsecs() );
  // send the utilization as busy time / elapsed time up to now
  sendOutput( msg.time(), utilization, busy_time / msg.time().asMsecs() );

  // reply and indicate ready for more
  sendOutput( msg.time(), reply, service_time );
  sendOutput( msg.time(), ready, 1 );

  return *this ;
}
