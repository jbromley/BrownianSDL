#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <functional>
#ifndef __APPLE__
#include <bsd/stdlib.h>
#endif
#include "BrownianEngine.h"
#include "Particle.h"
#include "Utilities.h"


BrownianEngine::BrownianEngine(const char* resourcesPath)
: GameEngine(800, 600, resourcesPath), particleRadius_(16),
particleSpeed_(64.0), particleAveraging_(true), particleAlpha_(0.25),
font_(NULL), showStats_(false)
{
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf error: " << TTF_GetError() << std::endl;
    }
    std::string fontPath = resourcePath() + "/Arial.ttf";
    font_ = TTF_OpenFont(fontPath.c_str(), 16);
    if (font_ == NULL) {
        std::cerr << "SDL_ttf error: " << TTF_GetError() << std::endl;
    }
    TTF_SetFontStyle(font_, TTF_STYLE_BOLD);

    setBackgroundColor(convertRGBAToColor(0, 0, 0, 0));
    textColor_ = {255, 255, 255};
}

BrownianEngine::~BrownianEngine()
{
    std::vector<Particle*>::iterator p;
    for (p = particles_.begin(); p != particles_.end(); ++p) {
        delete *p;
    }
    TTF_CloseFont(font_);
    TTF_Quit();
}

void
BrownianEngine::initializeData()
{
    arc4random_stir();
    setTitle("Brownian SDL");
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
}
 
void
BrownianEngine::update(long elapsedTime)
{
    for_each(particles_.begin(), particles_.end(),
	     std::bind2nd(std::mem_fun(&Particle::update), elapsedTime));
}

void BrownianEngine::render(SDL_Surface* destSurface)
{
    for_each(particles_.begin(), particles_.end(), std::mem_fun(&Particle::render));

    if (showStats_) {
        renderStatistics();
    }
}

void
BrownianEngine::renderStatistics()
{
    std::ostringstream statMsg;
    statMsg << particles_.size() << " particles, radius " << particleRadius_
        << ", speed " << particleSpeed_
        << ", averaging " << (particleAveraging_ ? "on" : "off")
        << ", alpha " << particleAlpha_
        << ", " << fps() << " fps";
    SDL_Surface* textSurface = TTF_RenderText_Blended(font_,
                                                      statMsg.str().c_str(),
                                                      textColor_);
    Sint16 textY = static_cast<Sint16>(height() - 20);
    SDL_Rect location = { 20, textY, 0, 0};
    SDL_BlitSurface(textSurface, NULL, surface(), &location);
    SDL_FreeSurface(textSurface);
}

void
BrownianEngine::keyDown(int keyCode)
{
    switch (keyCode) {
        case SDLK_1:
            --particleRadius_;
            clamp(particleRadius_, 2, 128);
            break;
        case SDLK_2:
            ++particleRadius_;
            clamp(particleRadius_, 2, 128);
            break;
        case SDLK_3:
            particleSpeed_ -= 1.0;
            clamp(particleSpeed_, 0.0, 256.0);
            break;
        case SDLK_4:
            particleSpeed_ += 1.0;
            clamp(particleSpeed_, 0.0, 256.0);
            break;
        case SDLK_5:
            particleAlpha_ -= 0.01;
            clamp(particleAlpha_, 0.0, 1.0);
            break;
        case SDLK_6:
            particleAlpha_ += 0.01;
            clamp(particleAlpha_, 0.0, 1.0);
            break;
        case SDLK_r:
        {
            std::vector<Particle*>::iterator p;
            for (p = particles_.begin(); p != particles_.end(); ++p) {
                delete *p;
            }
            particles_.clear();
        }
            break;
        case SDLK_a:
            // Toggle averaging and apply to all of the particles.
            particleAveraging_ = !particleAveraging_;
            for_each(particles_.begin(), particles_.end(),
                     std::bind2nd(std::mem_fun(&Particle::setAveraging), particleAveraging_));
            break;
        case SDLK_b:
            if (backgroundColor() == 0x00000000) {
                setBackgroundColor(convertRGBAToColor(255, 255, 255, 255));
                textColor_ = {0, 0, 0};
            } else {
                setBackgroundColor(convertRGBAToColor(0, 0, 0, 0));
                textColor_ = {255, 255, 255};
            }
            break;
        case SDLK_s:
            showStats_ = !showStats_;
            break;
        default:
            std::cout << particles_.size() << " balls, " << fps() << " fps" << std::endl;
            break;
    }
}

void
BrownianEngine::keyUp(int keyCode)
{
    switch (keyCode) {
        case SDLK_5:
        case SDLK_6:
            // Apply the averaging alpha to all of the particles.
            for_each(particles_.begin(), particles_.end(),
                     std::bind2nd(std::mem_fun(&Particle::setAlpha), particleAlpha_));
            break;
    }
}

void
BrownianEngine::mouseButtonUp(int button, int x, int y, int dx, int dy)
{
    int balls = (button == 1 ? 1 : 100);
    for (int i = 0; i < balls; ++i) {
        addParticle(x, y);
    }
}

void
BrownianEngine::addParticle(int x, int y)
{
//    Uint32 color = randomYellowMagentaCyan();
    Uint32 color = randomPrimaryColor();
    Particle* p = new Particle(x, y, particleRadius_, particleSpeed_, color, this);
    particles_.push_back(p);
}

// compile on mac os x:
// g++ -Wall -lSDLmain -lSDL Engine.cpp Main.cpp -framework Cocoa
