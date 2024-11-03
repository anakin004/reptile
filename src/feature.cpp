#include "feature.hpp"
#include "bug.hpp"
#include "math.hpp"
#include <SDL.h>
#include <string>

feature::feature() : inputText(""), mTextTexture(nullptr), mFeaturePrevEnd(0), mConfirmed(false), mTextBoxWidth(0), mTextBoxHeight(0) , mX(0), mY(0), pw("emsk"), mActive(false) {

}

feature::~feature(){
    freeText();
}

void feature::freeText(){
    if(mTextTexture != nullptr)
        SDL_DestroyTexture(mTextTexture);
        mTextTexture = nullptr;
}


bool feature::initBox(){
    if(mFont!=nullptr){
        TTF_SizeText( mFont, "****", &mTextBoxWidth, &mTextBoxHeight);
        return true;
    }
    return false;
}

bool feature::loadText( const std::string& text, const SDL_Color& textColor )
{

    if(mTextTexture != nullptr)
        SDL_DestroyTexture(mTextTexture);

    SDL_Surface* textSurface = TTF_RenderText_Solid( mFont, text.c_str(), textColor );
    if( textSurface == nullptr )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        mTextTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
        if( mTextTexture == nullptr )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            mTextBoxWidth = textSurface->w;
            mTextBoxHeight = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }
    
    //Return success
    return mTextTexture != nullptr;
}



void feature::renderButton(){

    SDL_Rect r = {mX, mY, (mWidth/2)/2, (mHeight)/2};
    SDL_Rect src = {0,0, mWidth/2 , mHeight};

    SDL_RenderCopy( gRenderer, mTexture, &src, &r );

}

// if we touch button we make the mActive var true enabling writing to screen
bool feature::checkTouched( const  Bug::BugInstance &thisBug ){

    // we want to make sure we delayed long enough
    if(SDL_GetTicks() - mFeaturePrevEnd < 5000)
        return false;
    
    

    const Bug::jPos headPos = thisBug.getHead();
    const int headSize = thisBug.getHeadSize();

    if( squaredDistCollision(mX, mY, mWidth/4, static_cast<int>(headPos.x), static_cast<int>(headPos.y),headSize/2 ))
        return true;
    
    return false;

}



void feature::getInput( SDL_Event& e ){

    if(e.type == SDL_KEYDOWN){
        if(e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0)
            inputText.pop_back();
        
        if(e.key.keysym.sym == SDLK_RETURN)
            mConfirmed = true;
        
        if(e.key.keysym.sym == SDLK_ESCAPE){

            mActive = false;
            mConfirmed = false;
            
            mFeaturePrevEnd = SDL_GetTicks();
            inputText = "";
            SDL_StopTextInput();
        }
        
        
    }
    
    if(e.type == SDL_TEXTINPUT){
        if(e.type == SDL_TEXTINPUT && inputText.length() < 4)
            inputText += e.text.text;
        
    }
}



void feature::renderTextBox(){

 
    // drawing text into box

    if( inputText.length() > 0 ){
        SDL_Color white {0,0,0, 0xFF};
        SDL_Rect boxRect = {WIDTH/2-mTextBoxWidth/2, HEIGHT/2-mTextBoxHeight , mTextBoxWidth ,mTextBoxHeight };
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRect( gRenderer, &boxRect );
        SDL_RenderFillRect( gRenderer, &boxRect );
        SDL_Rect tRect { WIDTH/2 -mTextBoxWidth/2 , HEIGHT/2 - mTextBoxHeight, mTextBoxWidth, mTextBoxHeight };
        std::string stars( inputText.length(),'*');
        loadText( stars , white );
        SDL_RenderCopy(gRenderer, mTextTexture, NULL, &tRect );
    }

    if( inputText.length() == 4 && mConfirmed ){
        if(inputText == pw){
            SDL_Rect src {200,0, 200, 200};
            SDL_Rect dst {WIDTH/2 - 200/2, HEIGHT/2 -200/2, 200, 200 };
            SDL_RenderDrawRect( gRenderer, &dst);
            SDL_RenderCopy(gRenderer, mTexture, &src, &dst);
        }
    }


}

void feature::setActive(bool f){
    mActive = f;
}


bool feature::isActive(){
    return mActive;
}


