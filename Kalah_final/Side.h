//
//  Side.h
//  Kalah
//
//  Created by Ojas Bardiya on 5/18/19.
//  Copyright © 2019 Ojas bardiya. All rights reserved.
//

#ifndef SIDE_h
#define SIDE_h


enum Side { NORTH, SOUTH };

const int NSIDES = 2;
const int POT = 0;

inline
Side opponent(Side s)
{
    return Side(NSIDES - 1 - s);
}



#endif /* Side_h */
