/*******************************************************************
*
*  DESCRIPTION: Atomic Model LQN Entry
*  AUTHOR: Dorin Petriu
*  EMAIL: dorin@sce.carleton.ca
*
*  DATE: 2002-12-2
*
*******************************************************************/

#ifndef _LQNENTRY_H
#define _LQNENTRY_H

#include "lqndefs.h"
#include "lqnmath.h"


class LqnEntry : public Atomic
{
public:
  // default constructor
  LqnEntry( const string &name = "LqnEntry" );

  virtual string className() const ;

protected:
  Model &initFunction();
  Model &externalFunction( const ExternalMessage & );
  Model &internalFunction( const InternalMessage & );
  Model &outputFunction( const InternalMessage & );

private:
  // input ports
  const Port &in;  // job from the queue
  const Port &procrtn;  // processor call return
  const Port &servrtn;  // server call return
  const Port &initproc;  // initialize the processing time for a phase
                         // first msg is the phase number (first or second)
                         // second msg is the service time in ms
  const Port &initserv;  // initialize server calls for a phase
                         // first msg is the phase number (first or second)
                         // second msg is the number of calls
                         // third msg is the target server
  // output ports
  Port &reply;  // reply
  Port &ready;  // ready for more jobs
  Port &proccall;  // call the processor
  Port &servcall;  // call a server
  Port &avservtime;  // average service time for the entry
  Port &avph1time;  // average phase1 service time
  Port &avph2time;  // average phase2 service time
  Port &throughput;  // entry throughput
  Port &utilization;  // entry utilization

  Value procinitmsg[ PROCINITFIELDS ];  // processor call init message
  int pfnum;  // index into the processor call init message

  Value servinitmsg[ SERVINITFIELDS ];  // server call init message
  int sfnum;  // index into the server call init message

  Value ph1_proc;  // phase1 processor service demand in ms
  Value ph2_proc;  // phase2 processor service demand in ms

  Value ph1_servers[ MAXCALLS ];  // phase1 target servers
  Value ph2_servers[ MAXCALLS ];  // phase2 target servers
  Value ph1_cs[ MAXCALLS ];  // phase1 call statements
  Value ph2_cs[ MAXCALLS ];  // phase2 call statements

  int ph1_cs_num;  // number of phase1 call statements
  int ph2_cs_num;  // number of phase2 call statements
  int ph1_cs_i;  // phase1 call statement index
  int ph2_cs_i;  // phase2 call statement index

  Value ph1_call_tot;  // total number of calls to be made by phase1
  Value ph2_call_tot;  // total number of calls to be made by phase2
  Value ph1_calls_made;  // number of calls made by phase1
  Value ph2_calls_made;  // number of calls made by phase2
  Value server_calls_made;  // number of calls made to a server

  int ph1_proc_done;  // phase1 processing is done
  int ph2_proc_done;  // phase2 processing is done
  int ph1_call_done;  // phase1 calls are done
  int ph2_call_done;  // phase2 calls are done
  int ph1_done;  // phase1 is all done
  int ph2_done;  // phase2 is all done
  int entry_done;  // entry is done

  int wait_proc;  // waiting for the processor
  int wait_call;  // waiting for a server call

  int replied;  // reply was sent

  Value jobs_through;  // number of jobs that have gone through the entry

  double ph1_serv_time;  // phase1 service time in ms
  double ph2_serv_time;  // phase2 service time in ms
  double entry_busy_time;  // total entry busy time in ms
  double ph1_busy_time;  // total phase1 busy time in ms
  double ph2_busy_time;  // total phase2 busy time in ms

  Time last_arrival_time;  // time of the last job arrival
  Time last_reply_time;  // time of the last reply
};
// class LqnEntry


// inline class name
inline
string LqnEntry::className() const
{
  return "LqnEntry" ;
}

#endif  //_LQNENTRY_H
