#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

#include "Actor.h"
#include "Penelope.h"

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

StudentWorld::~StudentWorld(){
    cleanUp();
}

int StudentWorld::init()
{
    Actor* p = new Penelope();
    m_actors.push_back(p);
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    
    vector<Actor*>::iterator it = m_actors.begin();
    (*it)->doSomething();

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
}

