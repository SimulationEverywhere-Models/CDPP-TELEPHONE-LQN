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

#ifndef _LQNPROC_H
#define _LQNPROC_H

#include "lqndefs.h"
#include "lqnmath.h"


class LqnProcessor : public Atomic
{
public:
  // default constructor
  LqnProcessor( const string &name = "LqnProcessor" );

  virtual string className() const ;

protected:
  Model &initFunction();
  Model &externalFunction( const ExternalMessage & );
  Model &internalFunction( const InternalMessage & );
  Model &outputFunction( const InternalMessage & );

private:
  // input ports
  const Port &in;
  // output ports
  Port &reply;
  Port &ready;
  Port &throughput;
  Port &utilization;

  Value jobs_through;  // number of jobs that have gone through the processor
  double busy_time;  // time that the processor was busy in ms
  double service_time;  // time in ms that a job took
  Time last_arrival_time;  // time of the last job arrival
};
// class LqnProcessor


// inline class name
inline
string LqnProcessor::className() const
{
  return "LqnProcessor" ;
}

#endif  // _LQNPROC_H
