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
    ostringstream s;
    s << "level";
    s.fill('0');
    s << setw(2) << getLevel();
    s << ".txt";
    string levelTxt = s.str();
    
//    Level::LoadResult lev = m_level.loadLevel(levelTxt);              // REMEMBER TO UNCOMMENT THIS
    
    
    ////////////////////////////////
    Level::LoadResult lev = m_level.loadLevel("level06.txt");         // for testing
    
    if(getLevel() >= 99 || lev == Level::load_fail_file_not_found){
        return GWSTATUS_PLAYER_WON;
        
    }else if(lev == Level::load_fail_bad_format){
        return GWSTATUS_LEVEL_ERROR;
    }else if(lev == Level::load_success){
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
                    case Level::citizen:
                        a = new Citizen(level_x*SPRITE_WIDTH, level_y*SPRITE_HEIGHT, this);
                        m_actors.push_back(a);
                        break;
                    case Level::gas_can_goodie:
                        a = new GasCanGoodie(level_x*SPRITE_WIDTH, level_y*SPRITE_HEIGHT, this);
                        m_actors.push_back(a);
                        break;
                    case Level::landmine_goodie:
                        a = new LandmineGoodie(level_x*SPRITE_WIDTH, level_y*SPRITE_HEIGHT, this);
                        m_actors.push_back(a);
                        break;
                    case Level::vaccine_goodie:
                        a = new VaccineGoodie(level_x*SPRITE_WIDTH, level_y*SPRITE_HEIGHT, this);
                        m_actors.push_back(a);
                        break;
                    case Level::pit:
                        a = new Pit(level_x*SPRITE_WIDTH, level_y*SPRITE_HEIGHT, this);
                        m_actors.push_back(a);
                        break;
                    case Level::dumb_zombie:
                        a = new DumbZombie(level_x*SPRITE_WIDTH, level_y*SPRITE_HEIGHT, this);
                        m_actors.push_back(a);
                        break;
                    case Level::smart_zombie:
                        a = new SmartZombie(level_x*SPRITE_WIDTH, level_y*SPRITE_HEIGHT, this);
                        m_actors.push_back(a);
                        break;
                    case Level::empty:
                        break;
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    m_penelope->doSomething();              // Penelope moves first
    
    for(int i = 0; i < m_actors.size(); i++){ // the rest of the Actors move
        if(!m_penelope->getIsAlive())       // if Penelope dies at any point, let world know
            return GWSTATUS_PLAYER_DIED;

        m_actors[i]->doSomething();
        
        if(m_penelope->hasExited())         // if Penelope completes the level at any point, let world know
            return GWSTATUS_FINISHED_LEVEL;
    }

    vector<Actor*>::iterator it = m_actors.begin();
    while(it != m_actors.end()){   // delete any dead Actors
        if(!(*it)->getIsAlive()){
            delete (*it);
            it = m_actors.erase(it);
        }else{
            it++;
        }
        
    }
    
    updateDisplayMessage();                 // update status message
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    vector<Actor*>::iterator it = m_actors.begin();
    while(it != m_actors.end()){
        delete (*it);
        it = m_actors.erase(it);
    }
    if(m_penelope != nullptr){
        delete m_penelope;
        m_penelope = nullptr;
    }
}

bool StudentWorld::validDestination(double destX, double destY, Actor* a){  // only call after level has been set
    if(m_penelope != a){                                       // "a" is the actor that is moving
                                                                // citizens and zombies must check for Penelope
        if( (abs(destX - m_penelope->getX()) < SPRITE_WIDTH) &&
            (abs(destY - m_penelope->getY()) < SPRITE_HEIGHT) ){
            return false;
        }
    }
    
    // check other Actors
    for(int i = 0; i < m_actors.size(); i++){
        if(m_actors[i] == a){         // make sure that the current moving actor doesn't block itself
            continue;
        }
        if(m_actors[i]->getIsAlive() && !(m_actors[i]->isPassable())){
            Actor* curr = m_actors[i];
            
            if( (abs(destX - curr->getX()) < SPRITE_WIDTH) &&
                (abs(destY - curr->getY()) < SPRITE_HEIGHT) ){
                return false;
            }
        }
    }
    return true; 
}

bool StudentWorld::validFlameDestination(double destX, double destY){
    for(int i = 0; i < m_actors.size(); i++){
        if(m_actors[i]->blocksFlame()){                   // walls and exits block flames
            Actor* curr = m_actors[i];
            
            if( (abs(destX - curr->getX()) < SPRITE_WIDTH) &&
               (abs(destY - curr->getY()) < SPRITE_HEIGHT) ){
                return false;
            }
        }
    }
    return true;
}

void StudentWorld::updateDisplayMessage(){
    ostringstream msg;
    msg << "Score: ";
    msg.fill('0');
    if(getScore() < 0){                                         // score
        msg << "-";
        msg << setw(5) << abs(getScore()) << " ";
    }else{
        msg << setw(6) << getScore() << "  ";
    }
    msg << "Level: " << getLevel() << "  ";                     // level
    msg << "Lives: " << getLives() << "  ";                     // lives
    msg << "Vaccines: " << m_penelope->getVaccines() << "  ";   // vaccines
    msg << "Flames: " << m_penelope->getFlames() << "  ";       // flames
    msg << "Mines: " << m_penelope->getLandmines() << "  ";     // landmines
    msg << "Infected: " << m_penelope->getInfectCount();        // infection count
    
    setGameStatText(msg.str());
}

void StudentWorld::activateOnAppropriateActors(Actor *a){
    if(isOverlapping(a, m_penelope) && m_penelope->getIsAlive()){
        a->activateIfAppropriate(m_penelope);
    }
    for(int i = 0; i < m_actors.size(); i++){
        if(!a->getIsAlive())                                // if "a" dies at any point, return immediately
            return;
    
        if(isOverlapping(a, m_actors[i]) && m_actors[i]->getIsAlive()){
            a->activateIfAppropriate(m_actors[i]);
        }
    }
}

bool StudentWorld::citizensLeft(){
    for(int i = 0 ; i < m_actors.size(); i++){
        if(m_actors[i]->isInfectable())
            return false;
    }
    return true;
}

double StudentWorld::calcDist(double x1, double y1, double x2, double y2) const {
    double difX = x1 - x2;
    double difY = y1 - y2;
    return sqrt(difX*difX + difY*difY);
}

bool StudentWorld::isOverlapping(Actor* a, Actor* b) const{
    if(a == b)
        return false;
    
    return (calcDist(a->getX(), a->getY(), b->getX(), b->getY()) <= 10);
}

bool StudentWorld::isOverlapping(Actor *a, double x, double y) const{   // use for Zombies
    return (calcDist(a->getX(), a->getY(), x, y) <= 10);
} 

// citizen helper functions
double StudentWorld::distToNearestZombie(double posX, double posY){
    return 81;
}

double StudentWorld::distToPenelope(double posX, double posY){
    double x = posX - m_penelope->getX();
    double y = posY - m_penelope->getY();
    return sqrt(x*x + y*y);
}

void StudentWorld::moveToPenelope(Actor* a){
    double citX = a->getX();
    double citY = a->getY();
    double pX = m_penelope->getX();
    double pY = m_penelope->getY();
    
    if(citY == pY){
        if(citX > pX){              // move left
            if(validDestination(citX-2, citY, a)){
                a->moveTo(citX-2, citY);
            }
        }else if (citX < pX){       // move right
            if(validDestination(citX+2, citY, a)){
                a->moveTo(citX+2, citY);
            }
        }
    }else if(citX == pX){
        if(citY > pY){              // move down
            if(validDestination(citX, citY-2, a)){
                a->moveTo(citX, citY-2);
            }
        }else if (citY < pY){       // move up
            if(validDestination(citX, citY+2, a)){
                a->moveTo(citX, citY+2);
            }
        }
    }
}

    //Zombies
bool StudentWorld::validVomitTargetAt(double tarX, double tarY){
    if(isOverlapping(m_penelope, tarX, tarY))
        return true;
    
    for(int i = 0; i < m_actors.size(); i++){
        if(m_actors[i]->isInfectable()){
            if(isOverlapping(m_actors[i], tarX, tarY))
                return true;
        }
    }
    return false;
}

        // used by SmartZombie to determine closest infectable Actor
        // returns true if there is a person within 80 units,
            // if true, sets otherX and otherY to that Person's coordinates and distance to the Euclidean distance
        // if false, otherX, otherY, and distance are left unchanged
bool StudentWorld::locateNearestVomitTrigger(double x, double y, double& otherX, double& otherY, double& distance){
    Actor* closest;
    double closestDist = 0;
    
    closest = m_penelope;
    closestDist = calcDist(x, y, m_penelope->getX(), m_penelope->getY());
    
    for(int i = 0; i < m_actors.size(); i++){
        if(!m_actors[i]->isInfectable())
            continue;
        
        double tempDist = calcDist(x, y, m_actors[i]->getX(), m_actors[i]->getY());
        if(tempDist < closestDist){
            closest = m_actors[i];
            closestDist = tempDist;
        }
    }
    
    if(closestDist <= 80){
        otherX = closest->getX();
        otherY = closest->getY();
        distance = closestDist;
        return true;
    }
    return false;
}


