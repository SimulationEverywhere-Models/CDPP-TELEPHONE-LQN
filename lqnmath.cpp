#include "lqnmath.h"


// round a double value to the nearest integer
double roundlqn( const double val ) 
{
  double rnd_val = 0;  // rounded value
  double mod = fmod( val, 1.0 );  // decimal remainder of value

  // if positive value
  if( val > 0 ) {
    // if the decimal remainder is less than 0.5 or if it is equal to 1.0
    // (bug in fmod sometimes returns 1.0 as a remainder when dividing by 1.0)
    if( ( mod < 0.5 ) || ( mod == 1.0 ) ) {
      rnd_val = static_cast< int >( val );  // round down
    }
    // else the decimal remainder is 0.5 or more
    else {
      rnd_val = static_cast< int >( val + 1 );  // round up
    }
  }
  // else negative value
  else {
    // if the decimal remainder is more than -0.5 or if it is equal to -1.0
    // (bug in fmod sometimes returns -1.0 as a remainder when dividing by 1.0)
    if( ( mod > -0.5 ) || ( mod == -1.0 ) ) {
      rnd_val = static_cast< int >( val );  // round up
    }
    // else the decimal remainder is -0.5 or less
    else {
      rnd_val = static_cast< int >( val - 1 );  // round down
    }
  }
/*
 *  cout << "roundlqn: input = " << val << "  mod(1) = " << mod 
 *       << "  output = " << rnd_val << endl;
 */

  return rnd_val;  // return the rounded value
}


static long Xa1;
static long Xa2;
static long Xa1vw;
static long Xa2vw;
static long Xa1w;
static long Xa2w;
static long Xcg1[32];
static long Xcg2[32];
static long Xig1[32];
static long Xig2[32];
static long Xlg1[32];
static long Xlg2[32];
static long Xm1;
static long Xm2;
static long Xqanti[32];


/*
     Advances the state  of  the current  generator  by 2^K values  and
     resets the initial seed to that value.
*/
void advnstlqn( long k )
{
#define numg 32L
static long g, i, ib1, ib2;
static long qrgnin;
    gsrgslqn( 0L, &qrgnin );
    if( qrgnin ) goto S10;
    fputs( " advnstlqn called before random generator initialized - ABORT\n", 
	  stderr );
    exit( 1 );
S10:
    gscgnlqn( 0L, &g );
    ib1 = Xa1;
    ib2 = Xa2;
    for( i=1; i<=k; i++ ) {
        ib1 = mltmodlqn( ib1, ib1, Xm1 );
        ib2 = mltmodlqn( ib2, ib2, Xm2 );
    }
    setsdlqn( mltmodlqn( ib1, *( Xcg1+g-1 ), Xm1 ), mltmodlqn( ib2, *( Xcg2+g-1 ), Xm2 ) );
#undef numg
}


/* 
     Transfers sign of argument sign to argument num 
*/
float fsignlqn( float num, float sign )
{
if ( ( sign>0.0f && num<0.0f ) || ( sign<0.0f && num>0.0f ) )
    return -num;
else return num;
}


/*
    Prints msg to standard error and then exits
*/
void ftnstoplqn( char* msg )
{
  if ( msg != NULL ) fprintf( stderr, "%s\n", msg );
  exit( 0 );
}


/* 
    GeNerate BETa random deviate 
*/
float genbetlqn( float aa, float bb )
{
#define expmax 87.49823
#define infnty 1.0E38
#define minlog 1.0E-37
static float olda = -1.0E37;
static float oldb = -1.0E37;
static float genbet, a, alpha, b, beta, delta, gamma, k1, k2, r, s, t, u1, u2, v, w, y, z;
static long qsame;

    qsame = olda == aa && oldb == bb;
    if( qsame ) goto S20;
    if( !( aa < minlog || bb < minlog ) ) goto S10;
    fputs( " AA or BB < 1.0E-37 in GENBET - Abort!\n", stderr );
    fprintf( stderr, " AA: %16.6E BB %16.6E\n", aa, bb );
    exit( 1 );
S10:
    olda = aa;
    oldb = bb;
S20:
    if( !( minlqn( aa, bb ) > 1.0 ) ) goto S100;
    if( qsame ) goto S30;
    a = minlqn( aa, bb );
    b = maxlqn( aa, bb );
    alpha = a+b;
    beta = sqrt( ( alpha-2.0 )/( 2.0*a*b-alpha ) );
    gamma = a+1.0/beta;
S30:
S40:
    u1 = ranflqn();
    u2 = ranflqn();
    v = beta*log( u1/( 1.0-u1 ) );
    if( v > expmax ) goto S55;
    w = exp( v );
    if( w > infnty/a ) goto S55;
    w *= a;
    goto S60;
S55:
    w = infnty;
S60:
    z = pow( u1, 2.0 )*u2;
    r = gamma*v-1.3862944;
    s = a+r-w;
    if( s+2.609438 >= 5.0*z ) goto S70;
    t = log( z );
    if( s > t ) goto S70;
    if( alpha/( b+w ) < minlog ) goto S40;
    if( r+alpha*log( alpha/( b+w ) ) < t ) goto S40;
S70:
    if( !( aa == a ) ) goto S80;
    genbet = w/( b+w );
    goto S90;
S80:
    genbet = b/( b+w );
S90:
    goto S230;
S100:
    if( qsame ) goto S110;
    a = maxlqn( aa, bb );
    b = minlqn( aa, bb );
    alpha = a+b;
    beta = 1.0/b;
    delta = 1.0+a-b;
    k1 = delta*( 1.38889E-2+4.16667E-2*b )/( a*beta-0.777778 );
    k2 = 0.25+( 0.5+0.25/delta )*b;
S110:
S120:
    u1 = ranflqn();
    u2 = ranflqn();
    if( u1 >= 0.5 ) goto S130;
    y = u1*u2;
    z = u1*y;
    if( 0.25*u2+z-y >= k1 ) goto S120;
    goto S170;
S130:
    z = pow( u1, 2.0 )*u2;
    if( !( z <= 0.25 ) ) goto S160;
    v = beta*log( u1/( 1.0-u1 ) );
    if( a > 1.0 ) goto S135;
    if( v > expmax ) goto S132;
    w = a*exp( v );
    goto S200;
S132:
    w = v + log( a );
    if( w > expmax ) goto S140;
    w = exp( w );
    goto S200;
S135:
    if( v > expmax ) goto S140;
    w = exp( v );
    if( w > infnty/a ) goto S140;
    w *= a;
    goto S200;
S140:
    w = infnty;
    goto S200;
S160:
    if( z >= k2 ) goto S120;
S170:
    v = beta*log( u1/( 1.0-u1 ) );
    if( a > 1.0 ) goto S175;
    if( v > expmax ) goto S172;
    w = a*exp( v );
    goto S190;
S172:
    w = v + log( a );
    if( w > expmax ) goto S180;
    w = exp( w );
    goto S190;
S175:
    if( v > expmax ) goto S180;
    w = exp( v );
    if( w > infnty/a ) goto S180;
    w *= a;
    goto S190;
S180:
    w = infnty;
S190:
    if( alpha/( b+w ) < minlog ) goto S120;
    if( alpha*( log( alpha/( b+w ) )+v )-1.3862944 < log( z ) ) goto S120;
S200:
    if( !( a == aa ) ) goto S210;
    genbet = w/( b+w );
    goto S220;
S210:
    genbet = b/( b+w );
S230:
S220:
    return genbet;
#undef expmax
#undef infnty
#undef minlog
}


/* 
     Generates random deviate from the distribution of a chisquare
     with DF degrees of freedom random variable. 
*/
float genchilqn( float df )
{
static float genchi;

    if( !( df <= 0.0 ) ) goto S10;
    fputs( " DF <= 0 in GENCHI - ABORT\n", stderr );
    fprintf( stderr, " Value of DF: %16.6E\n", df );
    exit( 1 );
S10:
    genchi = 2.0*sgammalqn( df/2.0 );
    return genchi;
}


/*
     Generates a single random deviate from an exponential
     distribution with mean AV.
*/
float genexplqn( float av )
{
static float genexp;

    if( av >= 0.0 ) goto S10;
    fputs( " AV < 0 in GENEXP - ABORT\n", stderr );
    fprintf( stderr, " Value of AV: %16.6E\n", av );
    exit( 1 );
S10:
    genexp = sexpolqn()*av;
    return genexp;
}


/*
     Generates a random deviate from the F ( variance ratio )
     distribution with DFN degrees of freedom in the numerator
     and DFD degrees of freedom in the denominator.
*/
float genflqn( float dfn, float dfd )
{
static float genf, xden, xnum;

    if( !( dfn <= 0.0 || dfd <= 0.0 ) ) goto S10;
    fputs( " Degrees of freedom nonpositive in GENF - abort!\n", stderr );
    fprintf( stderr, " DFN value: %16.6E DFD value: %16.6E\n", dfn, dfd );
    exit( 1 );
S10:
    xnum = 2.0*sgammalqn( dfn/2.0 )/dfn;
    xden = 2.0*sgammalqn( dfd/2.0 )/dfd;
    if( !( xden <= 1.0E-37*xnum ) ) goto S20;
    fputs( " GENF - generated numbers would cause overflow\n", stderr );
    fprintf( stderr, " Numerator %16.6E Denominator %16.6E\n", xnum, xden );
    fputs( " GENF returning 1.0E37\n", stderr );
    genf = 1.0E37;
    goto S30;
S20:
    genf = xnum/xden;
S30:
    return genf;
}


/*
     Generates random deviates from the gamma distribution whose
     density is
          ( A**R )/Gamma( R ) * X**( R-1 ) * Exp( -A*X )
*/
float gengamlqn( float a, float r )
{
static float gengam;
    if( a > 0.0 && r > 0.0 ) goto S10;
    fputs( " A or R nonpositive in GENGAM - abort!\n", stderr );
    fprintf( stderr, " A value: %16.6E R value: %16.6E\n", a, r );
    exit( 1 );
S10:
    gengam = sgammalqn( r );
    gengam /= a;
    return gengam;
}


/*
    GENerate Multivariate Normal random deviate
*/
void genmnlqn( float *parm, float *x, float *work )
{
static long i, icount, j, p, D1, D2, D3, D4;
static float ae;

    p = ( long ) ( *parm );
    for( i=1; i<=p; i++ ) *( work+i-1 ) = snormlqn();
    for( i=1, D3=1, D4=( p-i+D3 )/D3; D4>0; D4--, i+=D3 ) {
        icount = 0;
        ae = 0.0;
        for( j=1, D1=1, D2=( i-j+D1 )/D1; D2>0; D2--, j+=D1 ) {
            icount += ( j-1 );
            ae += ( *( parm+i+( j-1 )*p-icount+p )**( work+j-1 ) );
        }
        *( x+i-1 ) = ae+*( parm+i );
    }
}


/*
     GENerate an observation from the MULtinomial distribution
*/
void genmullqn( long n, float *p, long ncat, long *ix )
{
static float prob, ptot, sum;
static long i, icat, ntot;
    if( n < 0 ) ftnstoplqn( "N < 0 in GENMUL" );
    if( ncat <= 1 ) ftnstoplqn( "NCAT <= 1 in GENMUL" );
    ptot = 0.0F;
    for( i=0; i<ncat-1; i++ ) {
        if( *( p+i ) < 0.0F ) ftnstoplqn( "Some P( i ) < 0 in GENMUL" );
        if( *( p+i ) > 1.0F ) ftnstoplqn( "Some P( i ) > 1 in GENMUL" );
        ptot += *( p+i );
    }
    if( ptot > 0.99999F ) ftnstoplqn( "Sum of P( i ) > 1 in GENMUL" );
    ntot = n;
    sum = 1.0F;
    for( i=0; i<ncat; i++ ) ix[i] = 0;
    for( icat=0; icat<ncat-1; icat++ ) {
        prob = *( p+icat )/sum;
        *( ix+icat ) = ignbinlqn( ntot, prob );
        ntot -= *( ix+icat );
	if( ntot <= 0 ) return;
        sum -= *( p+icat );
    }
    *( ix+ncat-1 ) = ntot;
    return;
}


/*
     Generates random deviate  from the  distribution  of a  noncentral
     chisquare with DF degrees  of freedom and noncentrality  parameter
     xnonc.
*/
float gennchlqn( float df, float xnonc )
{
static float gennch;

    if( !( df < 1.0 || xnonc < 0.0 ) ) goto S10;
    fputs( "DF < 1 or XNONC < 0 in GENNCH - ABORT\n", stderr );
    fprintf( stderr, "Value of DF: %16.6E Value of XNONC: %16.6E\n", df, xnonc );
    exit( 1 );
S10:
    if( df >= 1.000001 ) goto S20;
    gennch = pow( snormlqn()+sqrt( xnonc ), 2.0 );
    goto S30;
S20:
    gennch = 2.0*sgammalqn( ( df-1.0 )/2.0 )+pow( snormlqn()+sqrt( xnonc ), 2.0 );
S30:
    return gennch;
}


/*
     Generates a random deviate from the  noncentral F ( variance ratio )
     distribution with DFN degrees of freedom in the numerator, and DFD
     degrees of freedom in the denominator, and noncentrality parameter
     XNONC.
*/
float gennflqn( float dfn, float dfd, float xnonc )
{
static float gennf, xden, xnum;
static long qcond;

    qcond = dfn < 1.0 || dfd <= 0.0 || xnonc < 0.0;
    if( !qcond ) goto S10;
    fputs( "In GENNF - Either ( 1 ) Numerator DF < 1.0 or\n", stderr );
    fputs( " ( 2 ) Denominator DF <= 0.0 or\n", stderr );
    fputs( " ( 3 ) Noncentrality parameter < 0.0\n", stderr );
    fprintf( stderr, 
      "DFN value: %16.6E DFD value: %16.6E XNONC value: \n%16.6E\n", dfn, dfd, 
      xnonc );
    exit( 1 );
S10:
    if( dfn >= 1.000001 ) goto S20;
    xnum = pow( snormlqn()+sqrt( xnonc ), 2.0 );
    goto S30;
S20:
    xnum = ( 2.0*sgammalqn( ( dfn-1.0 )/2.0 )+pow( snormlqn()+sqrt( xnonc ), 2.0 ) )/dfn;
S30:
    xden = 2.0*sgammalqn( dfd/2.0 )/dfd;
    if( !( xden <= 1.0E-37*xnum ) ) goto S40;
    fputs( " GENNF - generated numbers would cause overflow\n", stderr );
    fprintf( stderr, " Numerator %16.6E Denominator %16.6E\n", xnum, xden );
    fputs( " GENNF returning 1.0E37\n", stderr );
    gennf = 1.0E37;
    goto S50;
S40:
    gennf = xnum/xden;
S50:
    return gennf;
}


/*
     Generates a single random deviate from a normal distribution
     with mean, AV, and standard deviation, SD.
*/
float gennorlqn( float av, float sd )
{
static float gennor;

    if( sd >= 0.0 ) goto S10;
    fputs( " SD < 0 in GENNOR - ABORT\n", stderr );
    fprintf( stderr, " Value of SD: %16.6E\n", sd );
    exit( 1 );
S10:
    gennor = sd*snormlqn()+av;
    return gennor;
}


/*
     GENerate random PeRMutation of iarray
*/
void genprmlqn( long *iarray, int larray )
{
static long i, itmp, iwhich, D1, D2;

    for( i=1, D1=1, D2=( larray-i+D1 )/D1; D2>0; D2--, i+=D1 ) {
        iwhich = ignuinlqn( i, larray );
        itmp = *( iarray+iwhich-1 );
        *( iarray+iwhich-1 ) = *( iarray+i-1 );
        *( iarray+i-1 ) = itmp;
    }
}


/*
     Generates a real uniformly distributed between LOW and HIGH.
*/
float genunflqn( float low, float high )
{
static float genunf;

    if( !( low > high ) ) goto S10;
    fprintf( stderr, "LOW > HIGH in GENUNF: LOW %16.6E HIGH: %16.6E\n", low, high );
    fputs( "Abort\n", stderr );
    exit( 1 );
S10:
    genunf = low+( high-low )*ranflqn();
    return genunf;
}


/*
     Returns the value of two integer seeds of the current generator
*/
void getsdlqn( long *iseed1, long *iseed2 )
{
#define numg 32L
static long g;
static long qrgnin;
    gsrgslqn( 0L, &qrgnin );
    if( qrgnin ) goto S10;
    fprintf( stderr, "%s\n", 
      " GETSD called before random number generator  initialized -- abort!" );
    exit( 0 );
S10:
    gscgnlqn( 0L, &g );
    *iseed1 = *( Xcg1+g-1 );
    *iseed2 = *( Xcg2+g-1 );
#undef numg
}


/*
     Gets or returns in G the number of the current generator
*/
void gscgnlqn( long getset, long *g )
{
#define numg 32L
static long curntg = 1;
    if( getset == 0 ) *g = curntg;
    else  {
        if( *g < 0 || *g > numg ) {
            fputs( " Generator number out of range in GSCGN\n", stderr );
            exit( 0 );
        }
        curntg = *g;
    }
#undef numg
}


/*
     Gets or sets whether random generators set ( initialized ).
     Initially ( data statement ) state is not set
*/
void gsrgslqn( long getset, long *qvalue )
{
static long qinit = 0;

    if( getset == 0 ) *qvalue = qinit;
    else qinit = *qvalue;
}


/*
          Get or Set whether Seed is Set
*/
void gssstlqn( long getset, long *qset )
{
static long qstate = 0;
    if( getset != 0 ) qstate = 1;
    else  *qset = qstate;
}


/*
     Generates a single random deviate from a binomial
     distribution whose number of trials is N and whose
     probability of an event in each trial is P.
*/
long ignbinlqn( long n, float pp )
{
static float psave = -1.0E37;
static long nsave = -214748365;
static long ignbin, i, ix, ix1, k, m, mp, T1;
static float al, alv, amaxp, c, f, f1, f2, ffm, fm, g, p, p1, p2, p3, p4, q, qn, r, u, v, w, w2, x, x1, 
    x2, xl, xll, xlr, xm, xnp, xnpq, xr, ynorm, z, z2;

    if( pp != psave ) goto S10;
    if( n != nsave ) goto S20;
    if( xnp < 30.0 ) goto S150;
    goto S30;
S10:
    if( pp < 0.0F ) ftnstoplqn( "PP < 0.0 in IGNBIN" );
    if( pp > 1.0F ) ftnstoplqn( "PP > 1.0 in IGNBIN" );
    psave = pp;
    p = minlqn( psave, 1.0-psave );
    q = 1.0-p;
S20:
    if( n < 0L ) ftnstoplqn( "N < 0 in IGNBIN" );
    xnp = n*p;
    nsave = n;
    if( xnp < 30.0 ) goto S140;
    ffm = xnp+p;
    m = static_cast< long >( ffm );
    fm = m;
    xnpq = xnp*q;
    p1 = ( long ) ( 2.195*sqrt( xnpq )-4.6*q )+0.5;
    xm = fm+0.5;
    xl = xm-p1;
    xr = xm+p1;
    c = 0.134+20.5/( 15.3+fm );
    al = ( ffm-xl )/( ffm-xl*p );
    xll = al*( 1.0+0.5*al );
    al = ( xr-ffm )/( xr*q );
    xlr = al*( 1.0+0.5*al );
    p2 = p1*( 1.0+c+c );
    p3 = p2+c/xll;
    p4 = p3+c/xlr;
S30:
    u = ranflqn()*p4;
    v = ranflqn();
    if( u > p1 ) goto S40;
    ix = static_cast< long >( xm - p1 * v + u );
    goto S170;
S40:
    if( u > p2 ) goto S50;
    x = xl+( u-p1 )/c;
    v = v*c+1.0-ABSlqn( xm-x )/p1;
    if( v > 1.0 || v <= 0.0 ) goto S30;
    ix = static_cast< long >( x );
    goto S70;
S50:
    if( u > p3 ) goto S60;
    ix = static_cast< int >( xl + log( v ) / xll );
    if( ix < 0 ) goto S30;
    v *= ( ( u-p2 )*xll );
    goto S70;
S60:
    ix = static_cast< long >( xr - log( v ) / xlr );
    if( ix > n ) goto S30;
    v *= ( ( u-p3 )*xlr );
S70:
    k = ABSlqn( ix-m );
    if( k > 20 && k < xnpq/2-1 ) goto S130;
    f = 1.0;
    r = p/q;
    g = ( n+1 )*r;
    T1 = m-ix;
    if( T1 < 0 ) goto S80;
    else if( T1 == 0 ) goto S120;
    else  goto S100;
S80:
    mp = m+1;
    for( i=mp; i<=ix; i++ ) f *= ( g/i-r );
    goto S120;
S100:
    ix1 = ix+1;
    for( i=ix1; i<=m; i++ ) f /= ( g/i-r );
S120:
    if( v <= f ) goto S170;
    goto S30;
S130:
    amaxp = k/xnpq*( ( k*( k/3.0+0.625 )+0.1666666666666 )/xnpq+0.5 );
    ynorm = -( k*k/( 2.0*xnpq ) );
    alv = log( v );
    if( alv < ynorm-amaxp ) goto S170;
    if( alv > ynorm+amaxp ) goto S30;
    x1 = ix+1.0;
    f1 = fm+1.0;
    z = n+1.0-fm;
    w = n-ix+1.0;
    z2 = z*z;
    x2 = x1*x1;
    f2 = f1*f1;
    w2 = w*w;
    if( alv <= xm*log( f1/x1 )+( n-m+0.5 )*log( z/w )+( ix-m )*log( w*p/( x1*q ) )+( 13860.0-
      ( 462.0-( 132.0-( 99.0-140.0/f2 )/f2 )/f2 )/f2 )/f1/166320.0+( 13860.0-( 462.0-
      ( 132.0-( 99.0-140.0/z2 )/z2 )/z2 )/z2 )/z/166320.0+( 13860.0-( 462.0-( 132.0-
      ( 99.0-140.0/x2 )/x2 )/x2 )/x2 )/x1/166320.0+( 13860.0-( 462.0-( 132.0-( 99.0
      -140.0/w2 )/w2 )/w2 )/w2 )/w/166320.0 ) goto S170;
    goto S30;
S140:
    qn = pow( q, ( double )n );
    r = p/q;
    g = r*( n+1 );
S150:
    ix = 0;
    f = qn;
    u = ranflqn();
S160:
    if( u < f ) goto S170;
    if( ix > 110 ) goto S150;
    u -= f;
    ix += 1;
    f *= ( g/ix-r );
    goto S160;
S170:
    if( psave > 0.5 ) ix = n-ix;
    ignbin = ix;
    return ignbin;
}


/*
     Returns a random integer following a uniform distribution over
     ( 1, 2147483562 ) using the current generator.
*/
long ignlgilqn( void )
{
#define numg 32L
static long ignlgi, curntg, k, s1, s2, z;
static long qqssd, qrgnin;
    gsrgslqn( 0L, &qrgnin );
    if( !qrgnin ) inrgcmlqn();
    gssstlqn( 0, &qqssd );
    if( !qqssd ) setalllqn( 1234567890L, 123456789L );
    gscgnlqn( 0L, &curntg );
    s1 = *( Xcg1+curntg-1 );
    s2 = *( Xcg2+curntg-1 );
    k = s1/53668L;
    s1 = Xa1*( s1-k*53668L )-k*12211;
    if( s1 < 0 ) s1 += Xm1;
    k = s2/52774L;
    s2 = Xa2*( s2-k*52774L )-k*3791;
    if( s2 < 0 ) s2 += Xm2;
    *( Xcg1+curntg-1 ) = s1;
    *( Xcg2+curntg-1 ) = s2;
    z = s1-s2;
    if( z < 1 ) z += ( Xm1-1 );
    if( *( Xqanti+curntg-1 ) ) z = Xm1-z;
    ignlgi = z;
    return ignlgi;
#undef numg
}


/*
     Generates a single random deviate from a negative binomial
     distribution.
*/
long ignnbnlqn( long n, float p )
{
static long ignnbn;
static float y, a, r;
    if( n <= 0L ) ftnstoplqn( "N <= 0 in IGNNBN" );
    if( p <= 0.0F ) ftnstoplqn( "P <= 0.0 in IGNNBN" );
    if( p >= 1.0F ) ftnstoplqn( "P >= 1.0 in IGNNBN" );
    r = ( float )n;
    a = p/( 1.0F-p );
    y = sgammalqn( r )/a;
    ignnbn = ignpoilqn( y );
    return ignnbn;
}


/*
     Generates a single random deviate from a Poisson
     distribution with mean MU.
*/
long ignpoilqn( float mu )
{
extern float fsignlqn( float num, float sign );
static float a0 = -0.5;
static float a1 = 0.3333333;
static float a2 = -0.2500068;
static float a3 = 0.2000118;
static float a4 = -0.1661269;
static float a5 = 0.1421878;
static float a6 = -0.1384794;
static float a7 = 0.125006;
static float muold = -1.0E37;
static float muprev = -1.0E37;
static float fact[10] = {
    1.0, 1.0, 2.0, 6.0, 24.0, 120.0, 720.0, 5040.0, 40320.0, 362880.0
};
static long ignpoi, j, k, kflag, l, ll, m;
static float b1, b2, c, c0, c1, c2, c3, d, del, difmuk, e, fk, fx, fy, g, omega, p, p0, px, py, q, s, 
    t, u, v, x, xx, pp[35];

    if( mu == muprev ) goto S10;
    if( mu < 10.0 ) goto S120;
    muprev = mu;
    s = sqrt( mu );
    d = 6.0*mu*mu;
    ll = ( long ) ( mu-1.1484 );
S10:
    g = mu+s*snormlqn();
    if( g < 0.0 ) goto S20;
    ignpoi = ( long ) ( g );
    if( ignpoi >= ll ) return ignpoi;
    fk = ( float )ignpoi;
    difmuk = mu-fk;
    u = ranflqn();
    if( d*u >= difmuk*difmuk*difmuk ) return ignpoi;
S20:
    if( mu == muold ) goto S30;
    muold = mu;
    omega = 0.3989423/s;
    b1 = 4.166667E-2/mu;
    b2 = 0.3*b1*b1;
    c3 = 0.1428571*b1*b2;
    c2 = b2-15.0*c3;
    c1 = b1-6.0*b2+45.0*c3;
    c0 = 1.0-b1+3.0*b2-15.0*c3;
    c = 0.1069/mu;
S30:
    if( g < 0.0 ) goto S50;
    kflag = 0;
    goto S70;
S40:
    if( fy-u*fy <= py*exp( px-fx ) ) return ignpoi;
S50:
    e = sexpolqn();
    u = ranflqn();
    u += ( u-1.0 );
    t = 1.8+fsignlqn( e, u );
    if( t <= -0.6744 ) goto S50;
    ignpoi = ( long ) ( mu+s*t );
    fk = ( float )ignpoi;
    difmuk = mu-fk;
    kflag = 1;
    goto S70;
S60:
    if( c*fabs( u ) > py*exp( px+e )-fy*exp( fx+e ) ) goto S50;
    return ignpoi;
S70:
    if( ignpoi >= 10 ) goto S80;
    px = -mu;
    py = pow( mu, ( double )ignpoi )/ *( fact+ignpoi );
    goto S110;
S80:
    del = 8.333333E-2/fk;
    del -= ( 4.8*del*del*del );
    v = difmuk/fk;
    if( fabs( v ) <= 0.25 ) goto S90;
    px = fk*log( 1.0+v )-difmuk-del;
    goto S100;
S90:
    px = fk*v*v*( ( ( ( ( ( ( a7*v+a6 )*v+a5 )*v+a4 )*v+a3 )*v+a2 )*v+a1 )*v+a0 )-del;
S100:
    py = 0.3989423/sqrt( fk );
S110:
    x = ( 0.5-difmuk )/s;
    xx = x*x;
    fx = -0.5*xx;
    fy = omega*( ( ( c3*xx+c2 )*xx+c1 )*xx+c0 );
    if( kflag <= 0 ) goto S40;
    goto S60;
S120:
    muprev = -1.0E37;
    if( mu == muold ) goto S130;
    if( mu >= 0.0 ) goto S125;
    fprintf( stderr, "MU < 0 in IGNPOI: MU %16.6E\n", mu );
    fputs( "Abort\n", stderr );
    exit( 1 );
S125:
    muold = mu;
    m = maxlqn( 1L, ( long ) ( mu ) );
    l = 0;
    p = exp( -mu );
    q = p0 = p;
S130:
    u = ranflqn();
    ignpoi = 0;
    if( u <= p0 ) return ignpoi;
    if( l == 0 ) goto S150;
    j = 1;
    if( u > 0.458 ) j = minlqn( l, m );
    for( k=j; k<=l; k++ ) {
        if( u <= *( pp+k-1 ) ) goto S180;
    }
    if( l == 35 ) goto S130;
S150:
    l += 1;
    for( k=l; k<=35; k++ ) {
        p = p*mu/( float )k;
        q += p;
        *( pp+k-1 ) = q;
        if( u <= q ) goto S170;
    }
    l = 35;
    goto S130;
S170:
    l = k;
S180:
    ignpoi = k;
    return ignpoi;
}


/*
     Generates an integer uniformly distributed between LOW and HIGH.
*/
long ignuinlqn( long low, long high )
{
#define maxnum 2147483561L
static long ignuin, ign, maxnow, range, ranp1;

    if( !( low > high ) ) goto S10;
    fputs( " low > high in ignuin - ABORT\n", stderr );
    exit( 1 );

S10:
    range = high-low;
    if( !( range > maxnum ) ) goto S20;
    fputs( " high - low too large in ignuin - ABORT\n", stderr );
    exit( 1 );

S20:
    if( !( low == high ) ) goto S30;
    ignuin = low;
    return ignuin;

S30:
    ranp1 = range+1;
    maxnow = maxnum/ranp1*ranp1;
S40:
    ign = ignlgilqn()-1;
    if( !( ign <= maxnow ) ) goto S40;
    ignuin = low+ign%ranp1;
    return ignuin;
#undef maxnum
#undef err1
#undef err2
}


/*
     Reinitializes the state of the current generator
*/
void initgnlqn( long isdtyp )
{
#define numg 32L
static long g;
static long qrgnin;
    gsrgslqn( 0L, &qrgnin );
    if( qrgnin ) goto S10;
    fprintf( stderr, "%s\n", 
      " INITGN called before random number generator  initialized -- abort!" );
    exit( 1 );
S10:
    gscgnlqn( 0L, &g );
    if( -1 != isdtyp ) goto S20;
    *( Xlg1+g-1 ) = *( Xig1+g-1 );
    *( Xlg2+g-1 ) = *( Xig2+g-1 );
    goto S50;
S20:
    if( 0 != isdtyp ) goto S30;
    goto S50;
S30:
    if( 1 != isdtyp ) goto S40;
    *( Xlg1+g-1 ) = mltmodlqn( Xa1w, *( Xlg1+g-1 ), Xm1 );
    *( Xlg2+g-1 ) = mltmodlqn( Xa2w, *( Xlg2+g-1 ), Xm2 );
    goto S50;
S40:
    fprintf( stderr, "%s\n", "isdtyp not in range in INITGN" );
    exit( 1 );
S50:
    *( Xcg1+g-1 ) = *( Xlg1+g-1 );
    *( Xcg2+g-1 ) = *( Xlg2+g-1 );
#undef numg
}


/*
     Initializes common area  for random number  generator.  This saves
     the  nuisance  of  a  BLOCK DATA  routine  and the  difficulty  of
     assuring that the routine is loaded with the other routines.
*/
void inrgcmlqn( void )
{
#define numg 32L
static long T1;
static long i;
    Xm1 = 2147483563L;
    Xm2 = 2147483399L;
    Xa1 = 40014L;
    Xa2 = 40692L;
    Xa1w = 1033780774L;
    Xa2w = 1494757890L;
    Xa1vw = 2082007225L;
    Xa2vw = 784306273L;
    for( i=0; i<numg; i++ ) *( Xqanti+i ) = 0;
    T1 = 1;
    gsrgslqn( 1L, &T1 );
#undef numg
}


/* 
     Returns the length of str ignoring trailing blanks but not 
     other white space.
*/
long lennoblqn( char *str )
{
  long i, i_nb;

  for ( i=0, i_nb= -1L; *( str+i ); i++ )
    if ( *( str+i ) != ' ' ) i_nb = i;
	
  return ( i_nb+1 );
}


/*
     Returns ( A*S ) MOD M
*/
long mltmodlqn( long a, long s, long m )
{
#define h 32768L
static long mltmod, a0, a1, k, p, q, qh, rh;
/*
     H = 2**( ( b-2 )/2 ) where b = 32 because we are using a 32 bit
      machine. On a different machine recompute H
*/
    if( !( a <= 0 || a >= m || s <= 0 || s >= m ) ) goto S10;
    fputs( " a, m, s out of order in mltmod - ABORT!\n", stderr );
    fprintf( stderr, " a = %12ld s = %12ld m = %12ld\n", a, s, m );
    fputs( " mltmod requires: 0 < a < m; 0 < s < m\n", stderr );
    exit( 1 );
S10:
    if( !( a < h ) ) goto S20;
    a0 = a;
    p = 0;
    goto S120;
S20:
    a1 = a/h;
    a0 = a-h*a1;
    qh = m/h;
    rh = m-h*qh;
    if( !( a1 >= h ) ) goto S50;
    a1 -= h;
    k = s/qh;
    p = h*( s-k*qh )-k*rh;
S30:
    if( !( p < 0 ) ) goto S40;
    p += m;
    goto S30;
S40:
    goto S60;
S50:
    p = 0;
S60:
    if( !( a1 != 0 ) ) goto S90;
    q = m/a1;
    k = s/q;
    p -= ( k*( m-a1*q ) );
    if( p > 0 ) p -= m;
    p += ( a1*( s-k*q ) );
S70:
    if( !( p < 0 ) ) goto S80;
    p += m;
    goto S70;
S90:
S80:
    k = p/qh;
    p = h*( p-k*qh )-k*rh;
S100:
    if( !( p < 0 ) ) goto S110;
    p += m;
    goto S100;
S120:
S110:
    if( !( a0 != 0 ) ) goto S150;
    q = m/a0;
    k = s/q;
    p -= ( k*( m-a0*q ) );
    if( p > 0 ) p -= m;
    p += ( a0*( s-k*q ) );
S130:
    if( !( p < 0 ) ) goto S140;
    p += m;
    goto S130;
S150:
S140:
    mltmod = p;
    return mltmod;
#undef h
}


/*
     Uses a phrase ( character string ) to generate two seeds for the RGN
     random number generator.
*/
void phrtsdlqn( char* phrase, long *seed1, long *seed2 )
{

static char table[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+[];:'\\\"<>?, ./";

long ix;

static long twop30 = 1073741824L;
static long shift[5] = {
    1L, 64L, 4096L, 262144L, 16777216L
};
static long i, ichr, j, lphr, values[5];
extern long lennoblqn( char *str );

    *seed1 = 1234567890L;
    *seed2 = 123456789L;
    lphr = lennoblqn( phrase ); 
    if( lphr < 1 ) return;
    for( i=0; i<=( lphr-1 ); i++ ) {
	for ( ix=0; table[ix]; ix++ ) if ( *( phrase+i ) == table[ix] ) break; 
	/* JJV added ix++; to bring index in line with fortran's index */
	ix++;
        if ( !table[ix] ) ix = 0;
        ichr = ix % 64;
        if( ichr == 0 ) ichr = 63;
        for( j=1; j<=5; j++ ) {
            *( values+j-1 ) = ichr-j;
            if( *( values+j-1 ) < 1 ) *( values+j-1 ) += 63;
        }
        for( j=1; j<=5; j++ ) {
            *seed1 = ( *seed1+*( shift+j-1 )**( values+j-1 ) ) % twop30;
            *seed2 = ( *seed2+*( shift+j-1 )**( values+6-j-1 ) )  % twop30;
        }
    }
}


/*
                RANDom number generator as a Function
*/
float ranflqn( void )
{
static float ranf;
/*
     4.656613057E-10 is 1/M1  M1 is set in a data statement in IGNLGI
      and is currently 2147483563. If M1 changes, change this also.
*/
    ranf = ignlgilqn()*4.656613057E-10;
    return ranf;
}


float sdotlqn( long n, float *sx, long incx, float *sy, long incy )
{
static long i, ix, iy, m, mp1;
static float sdot, stemp;
    stemp = sdot = 0.0;
    if( n <= 0 ) return sdot;
    if( incx == 1 && incy == 1 ) goto S20;
    ix = iy = 1;
    if( incx < 0 ) ix = ( -n+1 )*incx+1;
    if( incy < 0 ) iy = ( -n+1 )*incy+1;
    for( i=1; i<=n; i++ ) {
        stemp += ( *( sx+ix-1 )**( sy+iy-1 ) );
        ix += incx;
        iy += incy;
    }
    sdot = stemp;
    return sdot;
S20:
    m = n % 5L;
    if( m == 0 ) goto S40;
    for( i=0; i<m; i++ ) stemp += ( *( sx+i )**( sy+i ) );
    if( n < 5 ) goto S60;
S40:
    mp1 = m+1;
    for( i=mp1; i<=n; i+=5 ) 
      stemp += ( *( sx+i-1 )**( sy+i-1 )+*( sx+i )**( sy+i )+*( sx+i+1 )**( sy+i+1 )+*( sx+i+2 )**( sy+i+2 )+*( sx+i+3 )**( sy+i+3 ) );
S60:
    sdot = stemp;
    return sdot;
}


/*
    SET ALL random number generators
*/
void setalllqn( long iseed1, long iseed2 )
{
#define numg 32L
static long T1;
static long g, ocgn;
static long qrgnin;
    T1 = 1;
    gssstlqn( 1, &T1 );
    gscgnlqn( 0L, &ocgn );
    gsrgslqn( 0L, &qrgnin );
    if( !qrgnin ) inrgcmlqn();
    *Xig1 = iseed1;
    *Xig2 = iseed2;
    initgnlqn( -1L );
    for( g=2; g<=numg; g++ ) {
        *( Xig1+g-1 ) = mltmodlqn( Xa1vw, *( Xig1+g-2 ), Xm1 );
        *( Xig2+g-1 ) = mltmodlqn( Xa2vw, *( Xig2+g-2 ), Xm2 );
        gscgnlqn( 1L, &g );
        initgnlqn( -1L );
    }
    gscgnlqn( 1L, &ocgn );
#undef numg
}


/*
     Sets whether the current generator produces antithetic values.
*/
void setantlqn( long qvalue )
{
#define numg 32L
static long g;
static long qrgnin;
    gsrgslqn( 0L, &qrgnin );
    if( qrgnin ) goto S10;
    fprintf( stderr, "%s\n", 
      " SETANT called before random number generator  initialized -- abort!" );
    exit( 1 );
S10:
    gscgnlqn( 0L, &g );
    Xqanti[g-1] = qvalue;
#undef numg
}


/*
      Places P, MEANV, and the Cholesky factoriztion of COVM
      in GENMN.
*/
void setgmnlqn( float *meanv, float *covm, long p, float *parm )
{
extern void spofalqn( float *a, long lda, long n, long *info );
static long T1;
static long i, icount, info, j, D2, D3, D4, D5;
    T1 = p*( p+3 )/2+1;
    if( !( p <= 0 ) ) goto S10;
    fputs( "P nonpositive in SETGMN\n", stderr );
    fprintf( stderr, "Value of P: %12ld\n", p );
    exit( 1 );
S10:
    *parm = p;
    for( i=2, D2=1, D3=( p+1-i+D2 )/D2; D3>0; D3--, i+=D2 ) *( parm+i-1 ) = *( meanv+i-2 );
    spofalqn( covm, p, p, &info );
    if( !( info != 0 ) ) goto S30;
    fputs( " COVM not positive definite in SETGMN\n", stderr );
    exit( 1 );
S30:
    icount = p+1;
    for( i=1, D4=1, D5=( p-i+D4 )/D4; D5>0; D5--, i+=D4 ) {
        for( j=i-1; j<p; j++ ) {
            icount += 1;
            *( parm+icount-1 ) = *( covm+i-1+j*p );
        }
    }
}


/*
     Resets the initial  seed of  the current  generator to  ISEED1 and
     ISEED2. The seeds of the other generators remain unchanged.
*/
void setsdlqn( long iseed1, long iseed2 )
{
#define numg 32L
static long g;
static long qrgnin;
    gsrgslqn( 0L, &qrgnin );
    if( qrgnin ) goto S10;
    fprintf( stderr, "%s\n", 
      " SETSD called before random number generator  initialized -- abort!" );
    exit( 1 );
S10:
    gscgnlqn( 0L, &g );
    *( Xig1+g-1 ) = iseed1;
    *( Xig2+g-1 ) = iseed2;
    initgnlqn( -1L );
#undef numg
}


/*
     ( STANDARD- )  E X P O N E N T I A L   DISTRIBUTION                
*/
float sexpolqn( void )
{
static float q[8] = {
    0.6931472, 0.9333737, 0.9888778, 0.9984959, 0.9998293, 0.9999833, 0.9999986, 
    .9999999
};
static long i;
static float sexpo, a, u, ustar, umin;
static float *q1 = q;
    a = 0.0;
    u = ranflqn();
    goto S30;
S20:
    a += *q1;
S30:
    u += u;
    if( u < 1.0 ) goto S20;
    u -= 1.0;
    if( u > *q1 ) goto S60;
    sexpo = a+u;
    return sexpo;
S60:
    i = 1;
    ustar = ranflqn();
    umin = ustar;
S70:
    ustar = ranflqn();
    if( ustar < umin ) umin = ustar;
    i += 1;
    if( u > *( q+i-1 ) ) goto S70;
    sexpo = a+umin**q1;
    return sexpo;
}


/*
     ( STANDARD- )  G A M M A  DISTRIBUTION                             
     INPUT: A =PARAMETER ( MEAN ) OF THE STANDARD GAMMA DISTRIBUTION
     OUTPUT: SGAMMA = SAMPLE FROM THE GAMMA-( A )-DISTRIBUTION
*/
float sgammalqn( float a )
{
extern float fsignlqn( float num, float sign );
static float q1 = 4.166669E-2;
static float q2 = 2.083148E-2;
static float q3 = 8.01191E-3;
static float q4 = 1.44121E-3;
static float q5 = -7.388E-5;
static float q6 = 2.4511E-4;
static float q7 = 2.424E-4;
static float a1 = 0.3333333;
static float a2 = -0.250003;
static float a3 = 0.2000062;
static float a4 = -0.1662921;
static float a5 = 0.1423657;
static float a6 = -0.1367177;
static float a7 = 0.1233795;
static float e1 = 1.0;
static float e2 = 0.4999897;
static float e3 = 0.166829;
static float e4 = 4.07753E-2;
static float e5 = 1.0293E-2;
static float aa = 0.0;
static float aaa = 0.0;
static float sqrt32 = 5.656854;
static float sgamma, s2, s, d, t, x, u, r, q0, b, b0, si, c, v, q, e, w, p;
    if( a == aa ) goto S10;
    if( a < 1.0 ) goto S120;
    aa = a;
    s2 = a-0.5;
    s = sqrt( s2 );
    d = sqrt32-12.0*s;
S10:
    t = snormlqn();
    x = s+0.5*t;
    sgamma = x*x;
    if( t >= 0.0 ) return sgamma;
    u = ranflqn();
    if( d*u <= t*t*t ) return sgamma;
    if( a == aaa ) goto S40;
    aaa = a;
    r = 1.0/ a;
    q0 = ( ( ( ( ( ( q7*r+q6 )*r+q5 )*r+q4 )*r+q3 )*r+q2 )*r+q1 )*r;
    if( a <= 3.686 ) goto S30;
    if( a <= 13.022 ) goto S20;
    b = 1.77;
    si = 0.75;
    c = 0.1515/s;
    goto S40;
S20:
    b = 1.654+7.6E-3*s2;
    si = 1.68/s+0.275;
    c = 6.2E-2/s+2.4E-2;
    goto S40;
S30:
    b = 0.463+s+0.178*s2;
    si = 1.235;
    c = 0.195/s-7.9E-2+1.6E-1*s;
S40:
    if( x <= 0.0 ) goto S70;
    v = t/( s+s );
    if( fabs( v ) <= 0.25 ) goto S50;
    q = q0-s*t+0.25*t*t+( s2+s2 )*log( 1.0+v );
    goto S60;
S50:
    q = q0+0.5*t*t*( ( ( ( ( ( a7*v+a6 )*v+a5 )*v+a4 )*v+a3 )*v+a2 )*v+a1 )*v;
S60:
    if( log( 1.0-u ) <= q ) return sgamma;
S70:
    e = sexpolqn();
    u = ranflqn();
    u += ( u-1.0 );
    t = b+fsignlqn( si*e, u );
    if( t < -0.7187449 ) goto S70;
    v = t/( s+s );
    if( fabs( v ) <= 0.25 ) goto S80;
    q = q0-s*t+0.25*t*t+( s2+s2 )*log( 1.0+v );
    goto S90;
S80:
    q = q0+0.5*t*t*( ( ( ( ( ( a7*v+a6 )*v+a5 )*v+a4 )*v+a3 )*v+a2 )*v+a1 )*v;
S90:
    if( q <= 0.0 ) goto S70;
    if( q <= 0.5 ) goto S100;
    if( q < 15.0 ) goto S95;
    if( ( q+e-0.5*t*t ) > 87.49823 ) goto S115;
    if( c*fabs( u ) > exp( q+e-0.5*t*t ) ) goto S70;
    goto S115;
S95:
    w = exp( q )-1.0;
    goto S110;
S100:
    w = ( ( ( ( e5*q+e4 )*q+e3 )*q+e2 )*q+e1 )*q;
S110:
    if( c*fabs( u ) > w*exp( e-0.5*t*t ) ) goto S70;
S115:
    x = s+0.5*t;
    sgamma = x*x;
    return sgamma;
S120:
    b0 = 1.0+0.3678794*a;
S130:
    p = b0*ranflqn();
    if( p >= 1.0 ) goto S140;
    sgamma = exp( log( p )/ a );
    if( sexpolqn() < sgamma ) goto S130;
    return sgamma;
S140:
    sgamma = -log( ( b0-p )/ a );
    if( sexpolqn() < ( 1.0-a )*log( sgamma ) ) goto S130;
    return sgamma;
}


/*
     ( STANDARD- )  N O R M A L  DISTRIBUTION                           
*/
float snormlqn( void )
{
static float a[32] = {
    0.0, 3.917609E-2, 7.841241E-2, 0.11777, 0.1573107, 0.1970991, 0.2372021, 0.2776904, 
    0.3186394, 0.36013, 0.4022501, 0.4450965, 0.4887764, 0.5334097, 0.5791322, 
    0.626099, 0.6744898, 0.7245144, 0.7764218, 0.8305109, 0.8871466, 0.9467818, 
    1.00999, 1.077516, 1.150349, 1.229859, 1.318011, 1.417797, 1.534121, 1.67594, 
    1.862732, 2.153875
};
static float d[31] = {
    0.0, 0.0, 0.0, 0.0, 0.0, 0.2636843, 0.2425085, 0.2255674, 0.2116342, 0.1999243, 
    0.1899108, 0.1812252, 0.1736014, 0.1668419, 0.1607967, 0.1553497, 0.1504094, 
    0.1459026, 0.14177, 0.1379632, 0.1344418, 0.1311722, 0.128126, 0.1252791, 
    0.1226109, 0.1201036, 0.1177417, 0.1155119, 0.1134023, 0.1114027, 0.1095039
};
static float t[31] = {
    7.673828E-4, 2.30687E-3, 3.860618E-3, 5.438454E-3, 7.0507E-3, 8.708396E-3, 
    1.042357E-2, 1.220953E-2, 1.408125E-2, 1.605579E-2, 1.81529E-2, 2.039573E-2, 
    2.281177E-2, 2.543407E-2, 2.830296E-2, 3.146822E-2, 3.499233E-2, 3.895483E-2, 
    4.345878E-2, 4.864035E-2, 5.468334E-2, 6.184222E-2, 7.047983E-2, 8.113195E-2, 
    9.462444E-2, 0.1123001, 0.136498, 0.1716886, 0.2276241, 0.330498, 0.5847031
};
static float h[31] = {
    3.920617E-2, 3.932705E-2, 3.951E-2, 3.975703E-2, 4.007093E-2, 4.045533E-2, 
    4.091481E-2, 4.145507E-2, 4.208311E-2, 4.280748E-2, 4.363863E-2, 4.458932E-2, 
    4.567523E-2, 4.691571E-2, 4.833487E-2, 4.996298E-2, 5.183859E-2, 5.401138E-2, 
    5.654656E-2, 5.95313E-2, 6.308489E-2, 6.737503E-2, 7.264544E-2, 7.926471E-2, 
    8.781922E-2, 9.930398E-2, 0.11556, 0.1404344, 0.1836142, 0.2790016, 0.7010474
};
static long i;
static float snorm, u, s, ustar, aa, w, y, tt;
    u = ranflqn();
    s = 0.0;
    if( u > 0.5 ) s = 1.0;
    u += ( u-s );
    u = 32.0*u;
    i = ( long ) ( u );
    if( i == 32 ) i = 31;
    if( i == 0 ) goto S100;
    ustar = u-( float )i;
    aa = *( a+i-1 );
S40:
    if( ustar <= *( t+i-1 ) ) goto S60;
    w = ( ustar-*( t+i-1 ) )**( h+i-1 );
S50:
    y = aa+w;
    snorm = y;
    if( s == 1.0 ) snorm = -y;
    return snorm;
S60:
    u = ranflqn();
    w = u*( *( a+i )-aa );
    tt = ( 0.5*w+aa )*w;
    goto S80;
S70:
    tt = u;
    ustar = ranflqn();
S80:
    if( ustar > tt ) goto S50;
    u = ranflqn();
    if( ustar >= u ) goto S70;
    ustar = ranflqn();
    goto S40;
S100:
    i = 6;
    aa = *( a+31 );
    goto S120;
S110:
    aa += *( d+i-1 );
    i += 1;
S120:
    u += u;
    if( u < 1.0 ) goto S110;
    u -= 1.0;
S140:
    w = u**( d+i-1 );
    tt = ( 0.5*w+aa )*w;
    goto S160;
S150:
    tt = u;
S160:
    ustar = ranflqn();
    if( ustar > tt ) goto S50;
    u = ranflqn();
    if( ustar >= u ) goto S150;
    u = ranflqn();
    goto S140;
}


/*
     SPOFA FACTORS A REAL SYMMETRIC POSITIVE DEFINITE MATRIX.
*/
void spofalqn( float *a, long lda, long n, long *info )
{
static long j, jm1, k;
static float t, s;
    for( j=1; j<=n; j++ ) {
        *info = j;
        s = 0.0;
        jm1 = j-1;
        if( jm1 < 1 ) goto S20;
        for( k=0; k<jm1; k++ ) {
            t = *( a+k+( j-1 )*lda )-sdotlqn( k, ( a+k*lda ), 1L, ( a+( j-1 )*lda ), 1L );
            t /=  *( a+k+k*lda );
            *( a+k+( j-1 )*lda ) = t;
            s += ( t*t );
        }
S20:
        s = *( a+j-1+( j-1 )*lda )-s;
        if( s <= 0.0 ) goto S40;
        *( a+j-1+( j-1 )*lda ) = sqrt( s );
    }
    *info = 0;
S40:
    return;
}
