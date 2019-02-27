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
    
    void addActor(Actor* a){
        m_actors.push_back(a);
    }
    
    bool validDestination(double destX, double destY, Actor* a);       // checks if location is valid -> not blocked by a Wall/person
    
    bool validFlameDestination(double destX, double destY);
    bool validVomitTargetAt(double tarX, double tarY);
    
    // citizen functions
    double distToNearestZombie(double posX, double posY);    // calculates distance to the nearest zombie - citizens
    double distToPenelope(double posX, double posY);         // calculates distance to penelope - citizens/zombies
    void moveToPenelope(Actor* a);
    void avoidZombie(Actor* a){}
    
    // SmartZombie
    bool locateNearestVomitTrigger(double x, double y, double& otherX, double& otherY, double& distance);
    
    // overlap
    void activateOnAppropriateActors(Actor* a);            // determines overlap and calls Actor's activate function
    bool citizensLeft();
    
private:
    std::vector<Actor*> m_actors;             // map to a vector of Actor pointers
    Penelope* m_penelope;
    Level m_level;
    
    double calcDist(double x1, double y1, double x2, double y2) const;
    
    bool isOverlapping(Actor* a, Actor* b) const;             // Overloaded - compares overlap for Actors a and b
    bool isOverlapping(Actor* a, double x, double y) const;   // compares if Actor a overlaps at (x,y)
    void updateDisplayMessage();
};

#endif // STUDENTWORLD_H_
