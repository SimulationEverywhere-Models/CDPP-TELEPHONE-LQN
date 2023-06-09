/*******************************************************************
*
*  DESCRIPTION: LQN Definitions header file
*
*  AUTHOR: Dorin Petriu
*  EMAIL: dorin@sce.carleton.ca
*
*  DATE: 2002-11-29
*
*******************************************************************/

#ifndef _LQNDEFS_H
#define _LQNDEFS_H

#include <list>
#include "atomic.h"  // class Atomic
#include "mainsimu.h"  // MainSimulator::Instance().getParameter( ... )
#include "message.h"  // class ExternalMessage, InternalMessage
#include "time.h"
#include "value.h"

#define MAXCONN 10  // maximum connections for any one type of entity
#define MAXPHASES 2  // maximum number of phases per entry
#define MAXCALLS 20  // maximum number of call statements per phase

#define LQNFIELDS 2  // number of fields in an LQN message
#define LQNENTITY 0  // number of the source/destination entity field
#define LQNSRC 0  // number of the source field
#define LQNDEST 0  // number of the destination field
#define LQNVAL 1  // number of the value field

#define PROCINITFIELDS 2  // number of fields in a processor call init message
#define SERVINITFIELDS 3  // number of fields in a server call init message
#define PHASE 0  // number of the initialized phase field
#define VALUE 1  // number of the initialization value field
#define TARGET 2  // number of the initialized call target field


// definition of queue types
typedef list< Time > time_queue_type;  // list of time elements
typedef list< Value > val_queue_type;  // list of value elements
typedef list< int > int_queue_type;  // list of integer elements

#endif  //_LQNDEFS_H
