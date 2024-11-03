#include "bug.hpp"
#include "math.hpp"
#include <cmath>
#include <SDL.h>

namespace Bug{

BugInstance::BugInstance( const int numJoints, const int conDist, const int JointSize, const double x, const double y ) :
                            mBaseVel(5),
                            mBaseRotVel(10),
                            mVel(0),
                            rotVel(0),
                            mSpineLen(conDist)
                            
{
    mJointSizes.reserve(numJoints);
    mJointsPos.reserve(numJoints);
    mJointPivots.reserve(numJoints-1);


    for(int i=0; i<numJoints; i++){

        if(JointSize - i*2 <= 0 )
            mJointSizes.push_back( 2 );
        else
            mJointSizes.push_back( JointSize - 2*i );
        
        
        if(i>0){
            mJointsPos.push_back( 
                {
                    mJointsPos[i-1].x + mJointSizes[i-1]/2.f + conDist + mJointSizes[i],
                    y,
                    180.f
                }
                );

            mJointPivots.push_back( 
                {
                    mJointsPos[i-1].x+mJointSizes[i-1]/2,
                    y, 
                    mJointsPos[i].x-mJointSizes[i]/2,
                    y
                }
                );
 
        }
        else{

            mJointsPos.push_back( 
                        {
                            x,
                            y,
                            180.f
                        }
                        );
        }
    }

}

/*
    mJointPivots is numJoints - 1 
    2 joints will have 1 connector
    o-o
    3 joints will have 2 connectors
    o-o-o
    etc..
 */


void BugInstance::stopBug(){
    rotVel = 0;
    mVel = 0;
}

void BugInstance::render(){

    srcRect evil;
    evil.head = {0,0,200,200};
    
    SDL_FRect cur;
    float x=0,y=0;

    int len = mJointsPos.size();
    for( int i = 0; i<len ; i++ ){


        float thisJointSize = static_cast<float>(mJointSizes[i]);  
                           // we are rendering it at an offset of the joints size over 2 with the joint sizes widthh
                            // cuz its a cirlce ... -_- 
        cur = {static_cast<float>(mJointsPos[i].x) - thisJointSize/2, static_cast<float>(mJointsPos[i].y) - thisJointSize/2, thisJointSize, thisJointSize};
       // between = {x, y, thisJointSize, thisJointSize};
        SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

        if(i==0){
            evil.head = {0, 0, 200, 200};
            SDL_RenderCopyExF(gRenderer, mTexture, &evil.head, &cur, mJointsPos[i].rot, NULL, SDL_FLIP_NONE );
        }
        else if( i<len-1){
            evil.body = { 200, 0 , 200, 200};
            SDL_RenderCopyExF(gRenderer, mTexture, &evil.body, &cur, mJointsPos[i].rot, NULL, SDL_FLIP_NONE );
        }
        else{
            evil.tail = { 400, 0, 200, 200};
            SDL_RenderCopyExF(gRenderer, mTexture, &evil.tail, &cur, mJointsPos[i].rot, NULL, SDL_FLIP_NONE );
        }
        SDL_SetRenderDrawColor(gRenderer, 0, 200, 0, 255);
        SDL_RenderDrawLineF( gRenderer, static_cast<float>(mJointPivots[i].frontX), static_cast<float>(mJointPivots[i].frontY), 
                                        static_cast<float>(mJointPivots[i].backX), static_cast<float>(mJointPivots[i].backY) );
    }

}


const jPos& BugInstance::getHead() const{
    return mJointsPos[0];
}


const int BugInstance::getHeadSize() const{
    return mJointSizes[0];
}


void BugInstance::updateBug(){

    

    // the bug moves by the head moving and the body following it
 
    mJointsPos[0].rot += rotVel/144.f;

    
    double x = cos(mJointsPos[0].rot*M_PI/180);
    double y = sin(mJointsPos[0].rot*M_PI/180);
    normal n = normalize(x,y); 
    mJointsPos[0].x += (n.x * mVel)/144;
    mJointsPos[0].y += (n.y * mVel)/144;
    mJointPivots[0].frontX = mJointsPos[0].x + mJointSizes[0]/2.f *cos((mJointsPos[0].rot-180)*M_PI/180);
    mJointPivots[0].frontY = mJointsPos[0].y + mJointSizes[0]/2.f *sin((mJointsPos[0].rot-180)*M_PI/180);


    int len = mJointsPos.size();


    for(int i = 1; i<len; i++){
        if(squaredDistCollision( mJointsPos[i-1].x, mJointsPos[i-1].y, static_cast<double>(mJointSizes[i-1]),
                                 mJointsPos[i].x, mJointsPos[i].y, static_cast<double>(mJointSizes[i])) )
            continue;

        
        x = mJointPivots[i-1].frontX-mJointPivots[i-1].backX;
        y = mJointPivots[i-1].frontY-mJointPivots[i-1].backY;

        double prevRot = mJointsPos[i].rot;
        double newRot{0};

        newRot = getRot(y,x);

        n = normalize(x,y);
        mJointsPos[i].rot = newRot;

        // squared dist collision
        if(x*x+y*y > mSpineLen*mSpineLen ){
            mJointsPos[i].x += (n.x * mBaseVel)/144.f;
            mJointsPos[i].y += (n.y * mBaseVel)/144.f;
        }
        
        if(i<len-1){
            mJointPivots[i].frontX =  mJointsPos[i].x + mJointSizes[i]/2.f *cos((mJointsPos[i].rot-180)*M_PI/180) ;
            mJointPivots[i].frontY =  mJointsPos[i].y + mJointSizes[i]/2.f *sin((mJointsPos[i].rot-180)*M_PI/180);
        }
        mJointPivots[i-1].backX = mJointsPos[i].x + mJointSizes[i]/2.f * cos((newRot)*M_PI/180);
        mJointPivots[i-1].backY = mJointsPos[i].y + mJointSizes[i]/2.f * sin((newRot)*M_PI/180);
    }  


}



void BugInstance::getMove( SDL_Event& e ) {

    if( e.type == SDL_KEYDOWN && e.key.repeat==0  ){
        switch(e.key.keysym.sym){
            //up
            case SDLK_w:
                mVel += mBaseVel;
                break;
            
            //left
            case SDLK_a:
                rotVel -= mBaseRotVel;
                break;

            //right
            case SDLK_d:
                rotVel += mBaseRotVel;
                break;

            default:
                break;
        }
    }

    //reverts the velocities once we release the key

    //ex. if the keyup was a w it means we previously pressed w
    // we are going up now we want to stop going up

    else if( e.type == SDL_KEYUP ){
        switch(e.key.keysym.sym){

            case SDLK_w:
                mVel -= mBaseVel;
                break;
            case SDLK_a:
                rotVel += mBaseRotVel;
                break;
            case SDLK_d:
                rotVel -= mBaseRotVel;
                break;

            default:
                break;
        }
    }
}



}