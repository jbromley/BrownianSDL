#include "GameEngine.h"
#include <vector>
#include <SDL/SDL_ttf.h>

class Particle;

class BrownianEngine: public GameEngine
{
public:
    BrownianEngine(const char* resourcePath);
    virtual ~BrownianEngine();
    
    virtual void initializeData();
    virtual void update(long elapsedTime);
    virtual void render(SDL_Surface* destSurface);
 
    virtual void keyDown(int keyCode);
    virtual void keyUp(int keyCode);
    virtual void mouseButtonUp(int button, int x, int y, int dx, int dy);
    
private:
    void addParticle(int x, int y);
    void renderStatistics();
    
    std::vector<Particle*> particles_;
    int particleRadius_;
    double particleSpeed_;
    bool particleAveraging_;
    double particleAlpha_;
    TTF_Font* font_;
    SDL_Color textColor_;
    bool showStats_;
};
