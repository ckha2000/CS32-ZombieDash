#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <vector>
#include <map>

class Actor;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();

    bool validDestination(int destX, int destY);        // checks if location is valid - not blocked by a Wall
    
private:
    std::vector<Actor*> m_actors;             // map to a vector of Actor pointers
    Level m_level;
};

#endif // STUDENTWORLD_H_
