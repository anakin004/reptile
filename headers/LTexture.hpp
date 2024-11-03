#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <SDL_ttf.h>

class LTexture {
    public:
        LTexture();
        ~LTexture();
        void free();

        bool loadFont( const std::string& path);
        bool loadFromFile( const std::string& path );
        bool loadFromRenderedText( const std::string& textureText , SDL_Color textColor );
        int getWidth();
        int getHeight();
    
    protected:
        TTF_Font* mFont;
        SDL_Texture* mTexture;
        int mWidth;
        int mHeight;
};

extern const int WIDTH;
extern const int HEIGHT;

extern SDL_Window *gWindow;
extern SDL_Renderer *gRenderer;


