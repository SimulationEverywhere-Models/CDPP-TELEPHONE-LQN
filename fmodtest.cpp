#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main( void ) 
{
  double i;
  char s1[10];

  strcpy( s1, "LQN test" );

  printf( "%s\n\n", s1 );

  for( i = -100; i < 100; i += 0.1 ) {
    printf( "%lf fmod 1 = %lf\n", i, fmod( i, 1 ) );
  }
}
