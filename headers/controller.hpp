#pragma once

#include "bug.hpp"
#include "feature.hpp"

class Controller{

    public:
        Controller(Bug::BugInstance* bug, feature* feat);
        void handleInput( SDL_Event& );
        void update();
        void renderFeature();



    private:
        bool textInputActive;
        Bug::BugInstance *thisBug;
        feature *thisFeature;

};