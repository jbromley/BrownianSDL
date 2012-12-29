#include <iostream>
#include <iomanip>
#include <sstream>
#include <functional>
#include "BrownianEngine.h"
#include "Particle.h"
#include "Utilities.h"


BrownianEngine::BrownianEngine(const char* resourcesPath)
  : GameEngine(800, 600, resourcesPath),
    particleRadius_(16),
    particleSpeed_(64.0),
    particleAveraging_(true),
    particleAlpha_(0.25),
    font_(NULL),
    textColor_(GLColor::white()),
    showStats_(false)
{
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf error: " << TTF_GetError() << std::endl;
    }
    std::string fontPath = resourcePath() + "/Arial.ttf";
    font_ = TTF_OpenFont(fontPath.c_str(), 12);
    if (font_ == NULL) {
        std::cerr << "SDL_ttf error: " << TTF_GetError() << std::endl;
    }
//    TTF_SetFontStyle(font_, TTF_STYLE_BOLD);
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

void BrownianEngine::render()
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
    renderText(statMsg.str(), 2.0f, 2.0f);
}

void
BrownianEngine::renderText(const std::string &text, float x, float y)
{
	// Use SDL_TTF to render the text onto an initial surface.
	SDL_Surface* textSurface = TTF_RenderText_Blended(font_, text.c_str(), textColor_.toSDLColor());
	
	/* Convert the rendered text to a known format */
	int w = nextPowerOfTwo(textSurface->w);
	int h = nextPowerOfTwo(textSurface->h);
	
	SDL_Surface* intermediary = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, w, h, 32,
                                                     0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_SetAlpha(textSurface, 0, 0);
	SDL_BlitSurface(textSurface, NULL, intermediary, NULL);
	
	/* Tell GL about our new texture */
    GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_BGRA,
                 GL_UNSIGNED_BYTE, intermediary->pixels );
	
	/* GL_NEAREST looks horrible, if scaled... */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
	/* prepare to render our texture */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glColor4f(textColor_.r, textColor_.g, textColor_.b, textColor_.a);
	
	// Draw a quad at location.
	glBegin(GL_QUADS);
    // Recall that the origin is in the lower-left corner. That is why the
    // TexCoords specify different corners than the Vertex coors seem to.
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(x, y);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(x + w, y);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(x + w, y + h);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(x, y + h);
	glEnd();
	
	// Bad things happen if we delete the texture before it finishes.
	glFinish();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
	
	// Clean up.
	SDL_FreeSurface(intermediary);
	SDL_FreeSurface(textSurface);
	glDeleteTextures(1, &texture);
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
            if (backgroundColor() == GLColor::black()) {
                setBackgroundColor(GLColor::white());
                textColor_ = GLColor::black();
            } else {
                setBackgroundColor(GLColor::black());
                textColor_ = GLColor::white();
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
    GLColor color = randomPrimaryColor();
    Particle* p = new Particle(x, y, particleRadius_, particleSpeed_, color, this);
    particles_.push_back(p);
}

// compile on mac os x:
// g++ -Wall -lSDLmain -lSDL Engine.cpp Main.cpp -framework Cocoa
