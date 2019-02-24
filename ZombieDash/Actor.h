#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;
class Goodie;

    // Actor class
class Actor: public GraphObject{
public:
    Actor(int imageID, double startX, double startY, StudentWorld* w, Direction dir = right, int depth = 0)
    :GraphObject(imageID, startX, startY, dir, depth)
    {
        m_isAlive = true;
        m_world = w;
    }
    
    virtual ~Actor(){};
    virtual void doSomething() = 0;

    // object characteristics
    virtual bool isPassable() const {return true;}      // returns whether object blocks movement
    virtual bool blocksFlame() const {return false;}    // returns whether object blocks flames
    virtual bool isDamagable() const {return true;}     // everything but flames, exits and pitfalls can be damaged
    virtual bool isInfectable() const {return false;}   // only Penelope and citizens can be Infected
    virtual bool isSavable() const {return false;}      // only Penelope and citizens can be saved
    virtual bool isHostile() const {return false;}      // zombies are hostile
    virtual bool picksUpGoodies() const {return false;} // Penelope
    
//    virtual void getDamaged(){}         // maybe later
    
    // mutators/accessors
    void setIsAlive(bool isAlive){ m_isAlive = isAlive; }
    bool getIsAlive() const {return m_isAlive;}
    virtual void setInfected(bool inf){}                       // for People only
    virtual bool isInfected() const {return false;}           // only use if Infectable
    StudentWorld* getWorld() const {return m_world;}
    
    // overlap functions
    virtual void activateIfAppropriate(Actor* a){}      // for activators
    virtual void useExitIfAppropriate(){}               // for people
    virtual void dieByFallOrBurnIfAppropriate(){}       // for people, zombies and goodies (and landmines?)
    virtual void pickUpGoodieIfAppropriate(Goodie* g){} // for penelope
    
private:
    bool m_isAlive;
    StudentWorld* m_world;
};

    // Wall class
class Wall: public Actor{
public:
    Wall(double startX, double startY, StudentWorld* w)
    :Actor(IID_WALL, startX, startY, w) {}
    virtual void doSomething(){}                    // Wall class does nothing
    
    virtual bool isPassable() const {return false;}
    virtual bool blocksFlame() const {return true;}
    virtual bool isDamagable() const {return false;}
};

class Person: public Actor{             // Citizens and Zombies
public:
    Person(int imageID, double startX, double startY, StudentWorld* w)
    :Actor(imageID, startX, startY, w)
    {
        m_infectCount = 0;
    }
    
    virtual bool isPassable() const {return false;}       // false for all People
    virtual bool isInfectable() const {return true;}
    virtual bool isSavable() const { return true;}
    
    // mutators and accessors
    virtual void setInfected(bool inf){ m_isInfected = inf;}
    virtual bool isInfected() const { return m_isInfected;}
    
    int getInfectCount() const {return m_infectCount;}
    bool incrementInfection(){                           // return true if infection is 500 (dead)
        m_infectCount++;
        return m_infectCount >= 500;
    }
    
private:
    bool m_isInfected = false;
    int m_infectCount;
};

class Citizen: public Person{
public:
    Citizen(double startX, double startY, StudentWorld* w)
    :Person(IID_CITIZEN, startX, startY, w), m_paralyzed(false){}
    
    virtual void doSomething();
    virtual void useExitIfAppropriate();
    
private:
    bool m_paralyzed;
};

    // Penelope class
class Penelope: public Person{
public:
    Penelope(double startX, double startY, StudentWorld* w)
    :Person(IID_PLAYER, startX, startY, w)
    {
        m_nLandmines = m_nFlames = m_nVaccines = 0;
        m_exited = false;
    }
    virtual void doSomething();
    
    virtual bool picksUpGoodies() const {return true;}
    virtual void pickUpGoodieIfAppropriate(Goodie* g);
    
    int getLandmines() const {return m_nLandmines;}        // accessors
    int getFlames() const {return m_nFlames;}
    int getVaccines() const {return m_nVaccines;}
    
    void setLandmines(int charges){m_nLandmines = charges;} // mutators
    void setFlames(int charges){m_nFlames = charges;}
    void setVaccines(int charges){m_nVaccines = charges;}
    
    bool hasExited() const {return m_exited;}
    void exit(){m_exited = true;}
    virtual void useExitIfAppropriate();
    
private:
    int m_nLandmines;
    int m_nFlames;
    int m_nVaccines;
    bool m_exited;
};


    // Activator class --> triggered by overlap
class Activator: public Actor{
public:
    Activator(int imageID, double startX, double startY, StudentWorld* w, Direction dir = right, int depth = 0)
    :Actor(imageID, startX, startY, w, dir, depth){}
    
    virtual void doSomething();
    virtual bool appropriateType(Actor *a){return false;}             // checks if "a" can activate the object
};

    // Exit class
class Exit: public Activator{
public:
    Exit(double startX, double startY, StudentWorld* w)
    :Activator(IID_EXIT, startX, startY, w, right, 1) {}
    
    virtual bool isDamagable() const {return false;}
    virtual bool blocksFlame() const {return true; }
    virtual void activateIfAppropriate(Actor *a);
    virtual bool appropriateType(Actor *a){return a->isSavable();}
};

    // Goodies
class Goodie: public Activator{
public:
    Goodie(int imageID, double startX, double startY, StudentWorld* w)
    :Activator(imageID, startX, startY, w, right, 1){}
    
    virtual void activateIfAppropriate(Actor* a);
    virtual bool appropriateType(Actor* a){return a->picksUpGoodies();}
    virtual void pickup(Penelope* p);
    virtual void incrementSupply(Penelope *p) = 0;
};

class GasCanGoodie: public Goodie{
public:
    GasCanGoodie(double startX, double startY, StudentWorld* w)
    :Goodie(IID_GAS_CAN_GOODIE, startX, startY, w){}
    
    virtual void incrementSupply(Penelope *p){
        p->setFlames(p->getFlames()+5);
    }
};

class LandmineGoodie: public Goodie{
public:
    LandmineGoodie(double startX, double startY, StudentWorld* w)
    :Goodie(IID_LANDMINE_GOODIE, startX, startY, w){}
    
    virtual void incrementSupply(Penelope *p){
        p->setLandmines(p->getLandmines()+2);
    }
};

class VaccineGoodie: public Goodie{
public:
    VaccineGoodie(double startX, double startY, StudentWorld* w)
    :Goodie(IID_VACCINE_GOODIE, startX, startY, w){}
    
    virtual void incrementSupply(Penelope *p){
        p->setVaccines(p->getVaccines()+1);
    }
};

#endif // ACTOR_H_
