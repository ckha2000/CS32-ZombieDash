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

    bool validDestination(int destX, int destY);       // checks if location is valid -> not blocked by a Wall/person
    void updateDisplayMessage();
    
    bool exitOverlap(double exitX, double exitY);             // handles exit's overlap with citizens and penelope
    

private:
    std::vector<Actor*> m_actors;             // map to a vector of Actor pointers
    Penelope* m_penelope;
    Level m_level;
    
    void saveOverlappingCitizens(double exitX, double exitY);            // exit overlapping with citizen
    bool isOverlapping(int x1, int y1, int x2, int y2);
};

#endif // STUDENTWORLD_H_
