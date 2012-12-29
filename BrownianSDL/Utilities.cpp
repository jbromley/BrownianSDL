//
//  Utilities.cpp
//  SDLTest
//
//  Created by Jay Bromley on 12/11/12.
//  Copyright (c) 2012 Applied Minds, LLC. All rights reserved.
//

#include <cmath>
#include "Utilities.h"


Uint32
convertRGBAToColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    return a + (b << 8) + (g << 16) + (r << 24);
}

void
convertColorToRGBA(Uint32 color, Uint8* r, Uint8* g, Uint8* b, Uint8* a)
{
    *a = color & 0xff;
    color >>= 8;
    *b = color & 0xff;
    color >>= 8;
    *g = color & 0xff;
    color >>= 8;
    *r = color & 0xff;
}

GLColor
randomRedGreenBlue()
{
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    u_int32_t component = arc4random_uniform(3);
    switch (component) {
        case 0:
            r = 1.0f;
            break;
        case 1:
            g = 1.0f;
            break;
        case 2:
            b = 1.0f;
            break;
    }
    
    return GLColor(r, g, b, 1.0f);
}

GLColor
randomRedBlueBlack()
{
    float r = 0.0f;
    float b = 0.0f;
    u_int32_t component = arc4random_uniform(3);
    switch (component) {
        case 0:
            r = 1.0f;
            break;
        case 1:
            b = 1.0f;
            break;
        default:
            // Do nothing, return a black color.
            break;
    }

    return GLColor(r, 0.0f, b, 1.0f);
}

GLColor
randomYellowMagentaCyan()
{
    float components[3] = {1.0f};
    
    u_int32_t notComp = arc4random_uniform(3);
    components[notComp] = 0.0f;
    return GLColor(components[0], components[1], components[2], 1.0f);
}


GLColor
randomPrimaryColor()
{
    Uint8 r = arc4random_uniform(2);
    Uint8 g = arc4random_uniform(2);
    Uint8 b = arc4random_uniform(2);
    return GLColor(r, g, b, 1.0);
}

float
randomFloat()
{
    return static_cast<float>(arc4random()) / UINT32_MAX;
}

float
randomFloat(float minValue, float maxValue)
{
    return minValue + (randomFloat() * (maxValue - minValue));
}

float
roundToInt(float value)
{
    return floor(value + 0.5f);
}

int nextPowerOfTwo(int x)
{
	double logbase2 = log(x) / log(2);
	return round(pow(2, ceil(logbase2)));
}

