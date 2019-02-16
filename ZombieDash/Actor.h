#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

    // Actor class
class Actor: public GraphObject{
public:
    Actor(int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0)
    :GraphObject(imageID, startX, startY, dir, depth, size)
    {}
    
    virtual ~Actor(){};
    virtual void doSomething() = 0;
};

// Wall class
class Wall: public Actor{
public:
    Wall(double startX, double startY)
    :Actor(IID_WALL, startX, startY) {}
    virtual void doSomething(){}
};

    // Penelope class
class Penelope: public Actor{
public:
    Penelope(double startX, double startY)
    :Actor(IID_PLAYER, startX, startY, 1, 0, 1.0)
    {}
    virtual void doSomething();
};


#endif // ACTOR_H_
