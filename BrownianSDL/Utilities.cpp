//
//  Utilities.cpp
//  SDLTest
//
//  Created by Jay Bromley on 12/11/12.
//  Copyright (c) 2012 Applied Minds, LLC. All rights reserved.
//

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

Uint32
randomRedGreenBlue()
{
    int r = 0;
    int g = 0;
    int b = 0;
    u_int32_t component = arc4random_uniform(3);
    switch (component) {
        case 0:
            r = 255;
            break;
        case 1:
            g = 255;
            break;
        case 2:
            b = 255;
            break;
    }
    
    return convertRGBAToColor(r, g, b, 255);
}

Uint32
randomRedBlueBlack()
{
    int r = 0;
    int b = 0;
    u_int32_t component = arc4random_uniform(3);
    switch (component) {
        case 0:
            r = 255;
            break;
        case 1:
            b = 255;
            break;
        default:
            // Do nothing, return a black color.
            break;
    }
    return convertRGBAToColor(r, 0, b, 255);
}

Uint32
randomYellowMagentaCyan()
{
    Uint8 components[3] = {0};
    
    u_int32_t comp1 = arc4random_uniform(3);
    u_int32_t comp2 = comp1;
    while (comp2 == comp1) {
        comp2 = arc4random_uniform(3);
    }
    components[comp1] = 255;
    components[comp2] = 255;
    return convertRGBAToColor(components[0], components[1], components[2], 255);
}


Uint32
randomPrimaryColor()
{
    Uint8 r = arc4random_uniform(2);
    Uint8 g = arc4random_uniform(2);
    Uint8 b = arc4random_uniform(2);
    return convertRGBAToColor(r * 255, g * 255, b * 255, 255);
}
