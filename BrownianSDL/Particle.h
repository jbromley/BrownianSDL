//
//  Particle.h
//  SDLTest
//
//  Created by Jay Bromley on 12/11/12.
//  Copyright (c) 2012 Applied Minds, LLC. All rights reserved.
//

#ifndef __SDLTest__Particle__
#define __SDLTest__Particle__

#include "GameEngine.h"
#include "Averager.h"
#include <iostream>
#include <SDL/SDL.h>
#include <Eigen/Core>
using namespace Eigen;


class Particle
{
public:
    Particle(double x, double y, int radius, double speed, Uint32 color,
             GameEngine* world);
    ~Particle();
    
    void update(long timeElapsed);
    void render();
    
    void setAveraging(bool useAverage);
    void setAlpha(double alpha);
    double alpha() const;
    
    
private:
    Vector2d q_;
    double heading_;
    int radius_;
    double speed_;
    Uint32 borderColor_;
    Uint32 fillColor_;
    bool useAverager_;
    Averager* averager_;
    GameEngine* world_;
};

#endif /* defined(__SDLTest__Particle__) */
