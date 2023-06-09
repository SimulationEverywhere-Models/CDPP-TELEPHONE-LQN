/*******************************************************************
*
*  DESCRIPTION: Plain Old Telephone System Definitions header file
*
*  AUTHOR: Dorin Petriu
*  EMAIL: dorin@sce.carleton.ca
*
*  DATE: 2002-12-24
*
*******************************************************************/

#ifndef _POTSDEFS_H
#define _POTSDEFS_H

#include <list>
#include "atomic.h"  // class Atomic
#include "mainsimu.h"  // MainSimulator::Instance().getParameter( ... )
#include "message.h"  // class ExternalMessage, InternalMessage
#include "time.h"
#include "value.h"

#define MAXPHONE 5  // maximum number of telephones that can be connected
                    // to the switch
#define NO 0
#define YES 1

#define STOP 0
#define START 1

// connection status
#define NOATTEMPT 0

// user to telephone set to switch signals
#define ONHOOK 0
#define OFFHOOK 1
#define NUMBER 2

// switch to telephone set to user signals
#define DIALTONE 3
#define RING 4
#define REMOTERING 5
#define BUSY 6
#define WRONGNUM 7
#define CONNECT 8
#define DISCONNECT 9

#define NOSIGNAL 10


#endif  //_POTSDEFS_H
