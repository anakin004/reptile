#pragma once
#include "LTexture.hpp"
#include "bug.hpp"


class feature : public LTexture {

    public:
        void freeText();
        feature();
        ~feature();
        void renderButton();
        bool checkTouched( const Bug::BugInstance &thisBug );
        bool loadText( const std::string&, const SDL_Color& );
        void getInput( SDL_Event &e );
        bool initBox();
        void renderTextBox();
        bool isActive();
        void setActive(bool f);

    private:
        // we render feature sprites to the middle of the screen
        // we only care about where the button to active feature is
        SDL_Texture *mTextTexture;
        std::string pw;
        std::string inputText;

        bool mActive;
        bool mConfirmed;

        Uint32 mFeaturePrevEnd;
        
        int mX, mY;
        int mTextBoxWidth;
        int mTextBoxHeight;


//    SDL_Color textColor = { 0, 0, 0, 0xFF };


};

extern const int WIDTH;
extern const int HEIGHT;