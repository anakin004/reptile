#include "math.hpp"
#include <cmath>
#include <stdio.h>


enum quad{
    I,
    II,
    III,
    IV
};

normal normalize( double x, double y ){
    double mag = sqrt(x*x+y*y);
    return {x/mag, y/mag};
}

//gets the refrence angle and does acos or atan or other based on quadrant on dat jawn
double getRot( double y, double x){  
    int quadPos=0;

    if( x>0 && y >= 0 )
        quadPos = I;

    else if( x<0 && y>0 )
        quadPos = II;

    else if( x<0 && y<0 )
        quadPos = III;
    
    else
        quadPos = IV;

    if( quadPos == I )
        return atan(y/x) * 180.f/M_PI;

    if( quadPos == II )
        return (M_PI + atan(y/x)) * 180.f/M_PI;
    
    if ( quadPos == III )
        return (M_PI + atan(y/x)) * 180.f/M_PI;
    
    else
        return (2*M_PI +atan(y/x))*180.f/M_PI;

         
}




