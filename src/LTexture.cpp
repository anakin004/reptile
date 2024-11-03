#include "LTexture.hpp"

LTexture::LTexture() {
    mFont = nullptr;
    mTexture = nullptr;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture() {
    free();
}

void LTexture::free()
{
    //Free texture if it exists
    if( mTexture != nullptr )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = nullptr;
        mWidth = 0;
        mHeight = 0;
    }
    if( mFont != nullptr ){
        TTF_CloseFont( mFont );
        mFont = nullptr;
    }
}

int LTexture::getWidth() {
    return mWidth;
}

int LTexture::getHeight() {
    return mHeight;
}

bool LTexture::loadFont( const std::string& path ){
    

    mFont = TTF_OpenFont( "fonts/lazy.ttf", 28 );

    //if font is null then false will return , else true
    return ( mFont == nullptr ) ? false : true;

}


bool LTexture::loadFromRenderedText( const std::string& textureText , SDL_Color textColor )
{
    
    SDL_Surface* textSurface = TTF_RenderText_Solid( mFont, textureText.c_str(), textColor );
    if( textSurface == nullptr )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
        if( mTexture == nullptr )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }
    
    //Return success
    return mTexture != nullptr;
}




bool LTexture::loadFromFile( const std::string& path )
{
    //Get rid of preexisting texture
    free();

    //The final texture
    SDL_Texture* newTexture = nullptr;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );

    SDL_Surface* src = SDL_GetWindowSurface( gWindow );


    if( loadedSurface == nullptr )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        SDL_Surface* optimized = SDL_ConvertSurface ( loadedSurface, src->format, 0 );
        
        if( optimized == nullptr )
            printf( "Failed to convert surface %s\n", SDL_GetError() );
        
        else{          
            
            // sets black to transparent for texture
             SDL_SetColorKey(optimized, SDL_TRUE, SDL_MapRGB(optimized->format, 0,  0, 0));  

            //Create texture from surface pixels
            newTexture = SDL_CreateTextureFromSurface( gRenderer, optimized );

            if( newTexture == nullptr )
                printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
            
            else{
                //Get image dimensions
                mWidth = loadedSurface->w;
                mHeight = loadedSurface->h;
            }

            //Get rid of old loaded surface
            SDL_FreeSurface( optimized );

        }
        
        SDL_FreeSurface( loadedSurface );

    }

    //Return success
    mTexture = newTexture;
    return mTexture != nullptr;
}


