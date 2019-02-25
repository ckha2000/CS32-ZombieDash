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
    virtual bool triggersActiveLandmines() const {return false;}    // for people and zombies
    
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
    virtual bool isSavable() const {return true;}
    virtual bool triggersActiveLandmines() const {return true;}
    
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
    virtual void dieByFallOrBurnIfAppropriate();
    
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
    
    void addLandmines(int charges){m_nLandmines += charges;} // mutators
    void addFlames(int charges){m_nFlames += charges;}
    void addVaccines(int charges){m_nVaccines += charges;}
    
    bool hasExited() const {return m_exited;}
    void exit(){m_exited = true;}
    virtual void useExitIfAppropriate();
    
    virtual void dieByFallOrBurnIfAppropriate();
    
private:
    int m_nLandmines;
    int m_nFlames;
    int m_nVaccines;
    bool m_exited;
    
    void useVaccine();
    void useFlamethrower();
    void dropLandmine();
};


    // Activator class --> triggered by overlap
class Activator: public Actor{
public:
    Activator(int imageID, double startX, double startY, StudentWorld* w, Direction dir = right, int depth = 0)
    :Actor(imageID, startX, startY, w, dir, depth){}
    
    virtual void doSomething();
};

    // Exit class
class Exit: public Activator{
public:
    Exit(double startX, double startY, StudentWorld* w)
    :Activator(IID_EXIT, startX, startY, w, right, 1) {}
    
    virtual bool blocksFlame() const {return true; }
    virtual void activateIfAppropriate(Actor* a);
};

    // Pit class
class Pit: public Activator{
public:
    Pit(double startX, double startY, StudentWorld* w)
    :Activator(IID_PIT, startX, startY, w){}
    
    virtual void activateIfAppropriate(Actor* a);
};

    // Flame class
class Flame: public Activator{
public:
    Flame(double startX, double startY, StudentWorld* w, Direction d)
    :Activator(IID_FLAME, startX, startY, w, d, 0), m_liveTicks(2){}
    
    virtual void doSomething();
    
    virtual void activateIfAppropriate(Actor* a);
private:
    int m_liveTicks;
};

class Landmine: public Activator{
public:
    Landmine(double startX, double startY, StudentWorld* w)
    :Activator(IID_LANDMINE, startX, startY, w, right, 1), m_safetyTics(30){}
    
    virtual void doSomething();
    virtual void activateIfAppripriate(Actor* a);
    virtual void dieByFallOrBurnIfAppropriate();
    
private:
    int m_safetyTics;
    void trigger();
};

    // Goodies
class Goodie: public Activator{
public:
    Goodie(int imageID, double startX, double startY, StudentWorld* w)
    :Activator(imageID, startX, startY, w, right, 1){}
    
    virtual void activateIfAppropriate(Actor* a);
    virtual void pickup(Penelope* p);
    virtual void incrementSupply(Penelope *p) = 0;
    
    virtual void dieByFallOrBurnIfAppropriate();
};

class GasCanGoodie: public Goodie{
public:
    GasCanGoodie(double startX, double startY, StudentWorld* w)
    :Goodie(IID_GAS_CAN_GOODIE, startX, startY, w){}
    
    virtual void incrementSupply(Penelope *p) {p->addFlames(5);}
};

class LandmineGoodie: public Goodie{
public:
    LandmineGoodie(double startX, double startY, StudentWorld* w)
    :Goodie(IID_LANDMINE_GOODIE, startX, startY, w){}
    
    virtual void incrementSupply(Penelope *p) {p->addLandmines(2);}
};

class VaccineGoodie: public Goodie{
public:
    VaccineGoodie(double startX, double startY, StudentWorld* w)
    :Goodie(IID_VACCINE_GOODIE, startX, startY, w){}
    
    virtual void incrementSupply(Penelope *p) {p->addVaccines(1);}
};

#endif // ACTOR_H_
