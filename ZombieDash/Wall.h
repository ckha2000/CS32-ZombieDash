//
//  Wall.h
//  ZombieDash
//
//  Created by christopher kha on 2/15/19.
//  Copyright © 2019 David Smallberg. All rights reserved.
//

#ifndef Wall_h
#define Wall_h

#include "Actor.h"

class Wall: public Actor{
public:
    Wall();
    virtual ~Wall();
    
    virtual void doSomething(){}    
};


#include <stdio.h>

#endif /* Wall_h */
