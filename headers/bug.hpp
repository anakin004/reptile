#pragma once
#include "LTexture.hpp"
#include <vector>

// under construction

/*  each bug will inherit the properties of LTexture 
    LTexture stores info such as the texture itsself along 
    with info about texture such as width and height of texture'
    --- also, might take out stuff about rendering text
        as if we have many bugs we dont want to store all of extra
        data about TTF_Font if we dont use it at all
*/

namespace Bug{

struct jPos{
    double x, y;
    double rot;
};

struct cPos{
    double frontX;
    double frontY;
    double backX;
    double backY;
};



union srcRect{
    SDL_Rect head;
    SDL_Rect body;
    SDL_Rect tail;
};


class BugInstance : public LTexture {

    public:
        BugInstance( const int numJoints, const int conDist, const int JointSize, const double x, const double y );
        void getMove( SDL_Event& );
        void render();
        void updateBug();
        void stopBug();
        const jPos& getHead() const;
        const int getHeadSize() const;

    private:
        
        std::vector<jPos> mJointsPos;
        std::vector<int> mJointSizes;
        std::vector<cPos> mJointPivots;

        int mBaseVel;
        int mBaseRotVel;
        int mVel;
        int rotVel;
        int mSpineLen;

};


}


extern const int WIDTH;
extern const int HEIGHT;