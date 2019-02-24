#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <vector>
#include <map>

class Actor;
class Penelope;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();

    bool validDestination(double destX, double destY, Actor* a);       // checks if location is valid -> not blocked by a Wall/person
    
    // citizen functions
    double distToNearestZombie(double posX, double posY);    // calculates distance to the nearest zombie - citizens
    double distToPenelope(double posX, double posY);         // calculates distance to penelope - citizens/zombies
    void moveToPenelope(Actor* a);
    void avoidZombie(Actor* a){}
    
    // overlap
    void activateOnAppropriateActors(Actor* a);            // determines overlap and calls Actor's activate function
    bool citizensLeft();
    
private:
    std::vector<Actor*> m_actors;             // map to a vector of Actor pointers
    Penelope* m_penelope;
    Level m_level;
    
    bool isOverlapping(Actor* a, Actor* b) const;
    void updateDisplayMessage();
    
};

#endif // STUDENTWORLD_H_
