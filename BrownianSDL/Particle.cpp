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
                   const GLColor& color, GameEngine* world)
  : q_(Vector2d(x, y)),
    heading_(M_PI * arc4random_uniform(4) / 2.0),
    radius_(radius),
    speed_(speed),
    borderColor_(color),
    fillColor_(color.lighten(0.5f)),
    useAverager_(true),
    world_(world)
{    
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
    const int segments = 16;
    const float increment = 2.0f * M_PI / segments;
    Vector2d vertices[segments];
    
	float theta = 0.0f;
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(fillColor_.r, fillColor_.g, fillColor_.b, fillColor_.a);
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < segments; ++i) {
        vertices[i] = q_ + radius_ * Vector2d(cos(theta), sin(theta));
		glVertex2f(vertices[i](0), vertices[i](1));
		theta += increment;
	}
	glEnd();
	glDisable(GL_BLEND);
    
	theta = 0.0f;
	glColor4f(borderColor_.r, borderColor_.g, borderColor_.b, borderColor_.a);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < segments; ++i) {
		glVertex2f(vertices[i](0), vertices[i](1));
	}
	glEnd();
    
    Vector2d vh(q_ + radius_ * Vector2d(cos(heading_), sin(heading_)));
	glBegin(GL_LINES);
	glVertex2f(q_(0), q_(1));
	glVertex2f(vh(0), vh(1));
	glEnd();

//    SDL_Surface* surface = world_->surface();
//    filledCircleColor(surface, q_(0), q_(1), radius_, fillColor_);
//    circleColor(surface, q_(0), q_(1), radius_, borderColor_);
//    
//    Vector2d v(q_(0) + radius_ * cos(heading_), q_(1) + radius_ * sin(heading_));
//    aalineColor(surface, q_(0), q_(1), v(0), v(1), borderColor_);
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
