/*******************************************************************
*
*  DESCRIPTION: statistical functions header for use with LQN
*
*  AUTHOR: Dorin Petriu 
*  Original randlib AUTHOR: Amir Barylko & Jorge Beyoglonian 
*
*  DATE: 14/12/2002
*
*******************************************************************/

#ifndef _LQNRAND_H
#define _LQNRAND_H

#include <iostream.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define ABSlqn( x ) ( ( x ) >= 0 ? ( x ) : -( x ) )
#define minlqn( a, b ) ( ( a ) <= ( b ) ? ( a ) : ( b ) )
#define maxlqn( a, b ) ( ( a ) >= ( b ) ? ( a ) : ( b ) )


// round a value
extern double roundlqn( const double val );

// Prototypes for all user accessible RANDLIB routines
extern void advnstlqn( long k );
extern void ftnstoplqn( char* msg );
extern float fsignlqn( float num, float sign );
extern float genbetlqn( float aa, float bb );
extern float genchilqn( float df );
extern float genexplqn( float av );
extern float genflqn( float dfn, float dfd );
extern float gengamlqn( float a, float r );
extern void genmnlqn( float *parm, float *x, float *work );
extern void genmullqn( long n, float *p, long ncat, long *ix );
extern float gennchlqn( float df, float xnonc );
extern float gennflqn( float dfn, float dfd, float xnonc );
extern float gennorlqn( float av, float sd );
extern void genprmlqn( long *iarray, int larray );
extern float genunflqn( float low, float high );
extern void getsdlqn( long *iseed1, long *iseed2 );
extern void gscgnlqn( long getset, long *g );
extern void gsrgslqn( long getset, long *qvalue );
extern void gssstlqn( long getset, long *qset );
extern long ignbinlqn( long n, float pp );
extern long ignlgilqn( void );
extern long ignnbnlqn( long n, float p );
extern long ignpoilqn( float mu );
extern long ignuinlqn( long low, long high );
extern void initgnlqn( long isdtyp );
extern void inrgcmlqn( void );
extern long lennoblqn( char *str );
extern long mltmodlqn( long a, long s, long m );
extern void phrtsdlqn( char* phrase, long* seed1, long* seed2 );
extern float ranflqn( void );
extern float sdotlqn( long n, float *sx, long incx, float *sy, long incy );
extern void setalllqn( long iseed1, long iseed2 );
extern void setantlqn( long qvalue );
extern void setgmnlqn( float *meanv, float *covm, long p, float *parm );
extern void setsdlqn( long iseed1, long iseed2 );
extern float sexpolqn( void );
extern float sgammalqn( float a );
extern float snormlqn( void );
extern void spofalqn( float *a, long lda, long n, long *info );

#endif  // _LQNRAND_H
