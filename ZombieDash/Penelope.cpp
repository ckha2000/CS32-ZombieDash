//
//  Penelope.cpp
//  ZombieDash
//
//  Created by christopher kha on 2/15/19.
//  Copyright © 2019 David Smallberg. All rights reserved.
//

#include "Penelope.h"

Penelope::~Penelope(){}

void Penelope::doSomething(){
    moveTo(getX()+1, getY());
}
