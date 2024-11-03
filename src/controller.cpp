#include "controller.hpp"
#include "feature.hpp"
#include "bug.hpp"

Controller::Controller(Bug::BugInstance* bug, feature *feat) : thisBug(bug), thisFeature(feat), textInputActive(false){

}


/*
even though it seems we are handing input for the bug 
one may ask why are we just calling thisBug->move()?
if we want to have more bugs, we cant set a controller that is limited to specific keys
if two players are playing on the same keyboard
we can additionally handle for things like pausing shift - speeding up, etc.
*/


void Controller::update(){
    if( thisFeature->checkTouched(*(thisBug)) )
        thisFeature->setActive(true);
    if(!thisFeature->isActive())
        thisBug->updateBug();
}   



void Controller::renderFeature(){
    if(thisFeature->isActive()){
        thisFeature->renderTextBox();
    }
}



void Controller::handleInput( SDL_Event& e ) {

    if( thisFeature->isActive() ){
        thisBug->stopBug();

        if(!textInputActive){
            SDL_StartTextInput();
            textInputActive = true;
        }

        thisFeature->getInput(e);
        

    }
    else{
        textInputActive = false;
        thisBug->getMove(e);
    }
}