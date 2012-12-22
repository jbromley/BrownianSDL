//
//  Averager.cpp
//  BrownianSDL
//
//  Created by Jay Bromley on 12/21/12.
//  Copyright (c) 2012 Applied Minds, LLC. All rights reserved.
//

#include "Averager.h"

Averager::Averager(double alpha, double startAvg)
: alpha_(alpha), beta_(1.0 - alpha), average_(startAvg)
{
}

Averager::~Averager()
{
}

double
Averager::update(double x)
{
    average_ = alpha_ * x + beta_ * average_;
    return average_;
}

double
Averager::average() const
{
    return average_;
}

double
Averager::alpha() const
{
    return alpha_;
}

void
Averager::setAlpha(double alpha)
{
    alpha_ = alpha;
    beta_ = 1.0 - alpha;
}
