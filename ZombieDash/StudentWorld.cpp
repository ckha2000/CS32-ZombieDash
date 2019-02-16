#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
using namespace std;

#include "Actor.h"

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), m_level(assetPath)
{}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    ostringstream s;
    s << "level0" << getLevel() << ".txt";
    string levelTxt = s.str();
    
    Level::LoadResult lev = m_level.loadLevel(levelTxt);
    
    if(lev == Level::load_fail_file_not_found){
        cerr << "Cannot find level01.txt data file" << endl;
    }else if(lev == Level::load_fail_bad_format){
        cerr << "Your level was improperly formatted" << endl;
    }else if(lev == Level::load_success){
        cerr << "Successfully loaded level" << endl;
        
        
    }
    
    Actor* p = new Penelope(100, 200, this);
    m_actors.push_back(p);
    
    Actor* w = new Wall(100, 200);
    m_actors.push_back(w);
    
    
    
    
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    vector<Actor*>::iterator it = m_actors.begin();
    while(it != m_actors.end()){
        (*it)->doSomething();
        it++;
    }

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    vector<Actor*>::iterator it = m_actors.begin();
    while(it != m_actors.end()){
        delete (*it);
        it = m_actors.erase(it);
    }
}

bool StudentWorld::validDestination(int destX, int destY){
    return true;
}
