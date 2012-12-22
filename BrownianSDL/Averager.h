//
//  Averager.h
//  BrownianSDL
//
//  Created by Jay Bromley on 12/21/12.
//  Copyright (c) 2012 Applied Minds, LLC. All rights reserved.
//

#ifndef __BrownianSDL__Averager__
#define __BrownianSDL__Averager__

#include <iostream>

class Averager
{
public:
    Averager(double alpha, double startAvg = 0.0);
    ~Averager();
    
    double update(double x);
    double average() const;
    double alpha() const;
    void setAlpha(double alpha);

private:
    double alpha_;
    double beta_;
    double average_;
};

#endif /* defined(__BrownianSDL__Averager__) */
