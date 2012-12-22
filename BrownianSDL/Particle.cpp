//
//  Particle.cpp
//  SDLTest
//
//  Created by Jay Bromley on 12/11/12.
//  Copyright (c) 2012 Applied Minds, LLC. All rights reserved.
//

#include "Particle.h"
#include "Utilities.h"
#include <cmath>
#include <SDL_gfxPrimitives.h>
#include <iostream>


Particle::Particle(double x, double y, int radius, double speed,
                   Uint32 color, GameEngine* world)
: q_(Vector2d(x, y)), heading_(M_PI * arc4random_uniform(4) / 2.0),
  radius_(radius), speed_(speed), borderColor_(color), useAverager_(true),
  world_(world)
{
    // Make the fill color a lighter version of the border.
    Uint8 r, g, b, a;
    convertColorToRGBA(borderColor_, &r, &g, &b, &a);
    r = static_cast<Uint8>(r * 0.5);
    g = static_cast<Uint8>(g * 0.5);
    b = static_cast<Uint8>(b * 0.5);
    a = static_cast<Uint8>(a * 0.5);
    fillColor_ = convertRGBAToColor(r, g, b, a);
    
    averager_ = new Averager(0.5, heading_);
}

Particle::~Particle()
{
    delete averager_;
}

void
Particle::update(long timeElapsed)
{
    double dheading = -M_PI / 4.0 + (arc4random_uniform(60) / 60.0) * M_PI / 2.0;
    if (useAverager_) {
        heading_ = averager_->update(heading_ + dheading);
    } else {
        heading_ += dheading;
    }
    Vector2d dq(speed_ * cos(heading_) * timeElapsed / 1000.0,
                speed_ * sin(heading_) * timeElapsed / 1000.0);
    q_ += dq;
    
    // Wrap the Particle around if necessary.
    if (q_(0) < -radius_) {
        q_(0) = world_->width() + radius_ - 1;
    }
    else if (q_(0) >= world_->width() + radius_) {
        q_(0) = 0;
    }

    if (q_(1) < -radius_) {
        q_(1) = world_->height() + radius_ - 1;
    }
    else if (q_(1) >= world_->height() + radius_) {
        q_(1) = -radius_;
    }
}

void
Particle::render()
{
    SDL_Surface* surface = world_->surface();
    filledCircleColor(surface, q_(0), q_(1), radius_, fillColor_);
    circleColor(surface, q_(0), q_(1), radius_, borderColor_);
    
    Uint32 white = 0xffffffff;
    Vector2d v(q_(0) + radius_ * cos(heading_), q_(1) + radius_ * sin(heading_));
    aalineColor(surface, q_(0), q_(1), v(0), v(1), white);
}

void
Particle::setAveraging(bool useAverage)
{
    useAverager_ = useAverage;
}

void
Particle::setAlpha(double alpha)
{
    averager_->setAlpha(alpha);
}

double
Particle::alpha() const
{
    return averager_->alpha();
}
