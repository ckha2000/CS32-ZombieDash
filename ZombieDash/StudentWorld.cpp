#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
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
    ostringstream levNum;
    levNum.fill('0');
    levNum << setw(2) << getLevel();
    
    ostringstream s;
    s << "level" << levNum.str() << ".txt";
    string levelTxt = s.str();
    
//    Level::LoadResult lev = m_level.loadLevel(levelTxt);              // REMEMBER TO UNCOMMENT THIS
    
    ////////////////////////////////
    Level::LoadResult lev = m_level.loadLevel("level06.txt");
    
    if(lev == Level::load_fail_file_not_found){
        cerr << "Cannot find level data file" << endl;
        return GWSTATUS_PLAYER_WON;
        
    }else if(lev == Level::load_fail_bad_format){
        cerr << "Your level was improperly formatted" << endl;
    }else if(lev == Level::load_success){
        cerr << "Successfully loaded level" << endl;
        
        Level::MazeEntry entry;
        for(int level_x = 0; level_x < LEVEL_WIDTH; level_x++){
            for(int level_y = 0; level_y < LEVEL_HEIGHT; level_y++){
                entry = m_level.getContentsOf(level_x, level_y);
                
                Actor* a;
                switch(entry){
                    case Level::player:
                        m_penelope = new Penelope(level_x*SPRITE_WIDTH, level_y*SPRITE_HEIGHT, this);
                        break;
                    case Level::wall:
                        a = new Wall(level_x*SPRITE_WIDTH, level_y*SPRITE_HEIGHT, this);
                        m_actors.push_back(a);
                        break;
                    case Level::exit:
                        a = new Exit(level_x*SPRITE_WIDTH, level_y*SPRITE_HEIGHT, this);
                        m_actors.push_back(a);
                        break;
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    m_penelope->doSomething();
    
    vector<Actor*>::iterator it = m_actors.begin();
    while(it != m_actors.end()){
        (*it)->doSomething();
        it++;
        
        if(m_penelope->hasExited())
            return GWSTATUS_FINISHED_LEVEL;
        
        if(!m_penelope->getIsAlive())
            return GWSTATUS_PLAYER_DIED;
    }
    
    updateDisplayMessage();
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    vector<Actor*>::iterator it = m_actors.begin();
    while(it != m_actors.end()){
        delete (*it);
        it = m_actors.erase(it);
    }
    delete m_penelope;
}

bool StudentWorld::validDestination(int destX, int destY){  // only call after level has been set
    vector<Actor*>::iterator it = m_actors.begin();
    while(it != m_actors.end()){
        if((*it)->getIsAlive() && !(*it)->isPassable()){           // possible error: make sure that the current moving Actor is not "it"
            Actor* curr = *it;
            int currX = curr->getX();
            int currY = curr->getY();
            
            if( (abs(destX - currX) < SPRITE_WIDTH) && (abs(destY - currY) < SPRITE_HEIGHT) ){
                return false;
            }
        }
        it++;
    }
    return true;
}

void StudentWorld::updateDisplayMessage(){
    ostringstream msg;
    msg << "Score: ";
    msg.fill('0');
    msg << setw(6) << getScore() << "  ";                       // score
    msg << "Level: " << getLevel() << "  ";                     // level
    msg << "Lives: " << getLives() << "  ";                     // lives
    msg << "Vaccines: " << m_penelope->getVaccines() << "  ";   // vaccines
    msg << "Flames: " << m_penelope->getFlames() << "  ";       // flames
    msg << "Mines: " << m_penelope->getLandmines() << "  ";     // landmines
    msg << "Infected: " << m_penelope->getInfectCount();        // infection count
    
    setGameStatText(msg.str());
}

bool StudentWorld::exitOverlap(double exitX, double exitY){
    saveOverlappingCitizens(exitX, exitY);
    
    if(isOverlapping(exitX, exitY, m_penelope->getX(), m_penelope->getY())){    // need to check if no citizens left
        // tell world that level is completed
        
        playSound(SOUND_LEVEL_FINISHED);
        m_penelope->exit();
        
        return true;
    }
    return false;
}


void StudentWorld::saveOverlappingCitizens(double exitX, double exitY){
    vector<Actor*>::iterator it;
    
    for(it = m_actors.begin(); it != m_actors.end(); it++){
        if((*it)->isSavable() && isOverlapping(exitX, exitY, (*it)->getX(), (*it)->getY())){
            increaseScore(500);
            (*it)->setIsAlive(false);
            playSound(SOUND_CITIZEN_SAVED);
        }
    }
}

bool StudentWorld::isOverlapping(int x1, int y1, int x2, int y2){
    double xDis = x1 - x2;
    double yDis = y1 - y2;
    
    return (sqrt(xDis*xDis + yDis*yDis) <= 10);
    
}
