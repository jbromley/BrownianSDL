//
//  Utilities.h
//  SDLTest
//
//  Created by Jay Bromley on 12/11/12.
//  Copyright (c) 2012 Applied Minds, LLC. All rights reserved.
//

#ifndef __SDLTest__Utilities__
#define __SDLTest__Utilities__

#include <SDL/SDL.h>


Uint32
convertRGBAToColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

void
convertColorToRGBA(Uint32 color, Uint8* r, Uint8* g, Uint8* b, Uint8* a);

Uint32
randomRedGreenBlue();

Uint32
randomRedBlueBlack();

Uint32
randomYellowMagentaCyan();

Uint32
randomPrimaryColor();

template <typename T>
T clamp(T& value, T minValue, T maxValue)
{
    if (value < minValue) {
        value = minValue;
    } else if (value > maxValue) {
        value = maxValue;
    }
    return value;
}


#endif /* defined(__SDLTest__Utilities__) */
