#include "LTexture.hpp"
#include "feature.hpp"
#include "bug.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>


/* 
   just going to have globals for renderer and
   window - so width and height will be const globals
*/

SDL_Window *gWindow = nullptr;
SDL_Renderer *gRenderer = nullptr;

const int WIDTH = 900;
const int HEIGHT = 600;

/*
 initalizing sdl subsystems, generating window, and creating renderer
*/
bool init(){

    bool success = true;

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
        printf( "%s\n", SDL_GetError() );
        success = false;
    }
    
    else{
        // makes things look pixely, look up how to downscale for more pixelation
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "0" ) ){
            printf( "%s\n", SDL_GetError() );
            success = false;
        }

        else{

            gWindow = SDL_CreateWindow( "reptiLe", 
                                        SDL_WINDOWPOS_CENTERED, //x
                                        SDL_WINDOWPOS_CENTERED, //y
                                        WIDTH, HEIGHT, SDL_WINDOW_SHOWN ); // width and height and flags
            // create window returns null if failed
            if( gWindow == NULL ){
                printf( "%s\n", SDL_GetError() );
                success = false;
            }

            else{                                        // gets next available driver with -1
                                                         // and uses accelerated drivers
                gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
                
                if( gRenderer == NULL ){
                    printf( "%s\n", SDL_GetError() );
                    success = false;
                }

                else{
                    
                        if( IMG_Init( IMG_INIT_PNG ) != IMG_INIT_PNG ){
                            printf( "%s\n", SDL_GetError() );
                            success = false;
                        }
                        
                        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) == -1 ){
                            printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                            success = false;
                        }

                        if( TTF_Init() == -1 ){
                            printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                            success = false;
                        }

                    }

                }
            }
            
        }
    
        return success; 

    }


bool loadMedia( Bug::BugInstance *thisBug, feature *thisFeature){

    bool success = true;


    // load from file returns bool 
    if( !(thisBug->loadFromFile( "sound_effects/sprites/bugAtlas.png" )) )
        success = false;

    if(!thisFeature->loadFromFile( "sound_effects/sprites/emskAtlas.png")){
        printf("emsk atlas not found");
        success = false;
    }

        
    if( !thisFeature->loadFont( "fonts/lazy.ttf" ) ){
        printf("couldnt get font");
        success = false;
    }

    if( !thisFeature->initBox() ){
        printf("Couldnt init box");
        success = false;
    }

    
    return success;
    
}