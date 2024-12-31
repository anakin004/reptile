#include "app.hpp"
#include "init.hpp"
#include "bug.hpp"
#include "controller.hpp"


void APP::run (){


    const int numJoints = 7;
    const int SpineLen = 10;
    const int JointSize = 50;

    feature emsk;
    Bug::BugInstance brenard(numJoints, SpineLen, JointSize, WIDTH/2.f, HEIGHT/2.f);

    if( !init() || !loadMedia(&brenard, &emsk) )
        return;
    
    Controller player(&brenard, &emsk);
    const int frameRate = 144;

    SDL_Event e;
    bool quit = false;

    Uint32 StartTicks, EndTicks;
    while( !quit ){
        StartTicks = SDL_GetTicks();

        // while we have a poll event
        while( SDL_PollEvent(&e)  ){
            if(e.type == SDL_QUIT){
                quit = true;
                break;
            }
            
            player.handleInput(e);
        }


        SDL_RenderClear(gRenderer);

        // if feature isnt active dont update bug

        player.update();
        emsk.renderButton();
        brenard.render();
        player.renderFeature();


        
        
        SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderPresent( gRenderer );
        
        EndTicks = SDL_GetTicks();
        float StdFrameTime = frameRate/1000.f;
        if(EndTicks-StartTicks < StdFrameTime )
            SDL_Delay( StdFrameTime - ( EndTicks-StartTicks ) );
        
    }

    cleanup();
}

void APP::cleanup(){

    SDL_DestroyRenderer( gRenderer );
    gRenderer = nullptr;

    SDL_DestroyWindow( gWindow );
    gWindow = nullptr;


}

