#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor: public GraphObject{
public:
    Actor(int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0)
        :GraphObject(imageID, startX, startY, dir, depth, size)
    {}
    
    virtual ~Actor(){};
    virtual void doSomething() = 0;
};

#endif // ACTOR_H_
