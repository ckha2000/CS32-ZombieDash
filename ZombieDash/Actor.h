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
    virtual bool isPassable() const {return false;}     // returns whether object blocks movement
    virtual bool blocksFlame() const {return false;}    // returns whether object blocks flames
    virtual bool isInfectable() const {return false;}   // only Penelope and citizens can be Infected
    virtual bool isHostile() const {return false;}      // zombies are hostile
    virtual bool triggersActiveLandmines() const {return false;}    // for people and zombies
    
//    virtual void getDamaged(){}         // maybe later
    
    // mutators/accessors
    void setIsAlive(bool isAlive){ m_isAlive = isAlive; }
    bool getIsAlive() const {return m_isAlive;}
    StudentWorld* getWorld() const {return m_world;}
    
    // overlap functions
    virtual void activateIfAppropriate(Actor* a){}      // for activators
    virtual void useExitIfAppropriate(){}               // for people
    virtual void dieByFallOrBurnIfAppropriate(){}       // for people, zombies and goodies (and landmines?)
    virtual void pickUpGoodieIfAppropriate(Goodie* g){} // for penelope
    virtual void beVomitedOnIfAppropriate(){}           // for people
    
private:
    bool m_isAlive;
    StudentWorld* m_world;
};

class Person: public Actor{             // Citizens and Penelope
public:
    Person(int imageID, double startX, double startY, StudentWorld* w)
    :Actor(imageID, startX, startY, w), m_isInfected(false), m_infectCount(0) {}
    
    virtual bool isInfectable() const {return true;}
    virtual bool triggersActiveLandmines() const {return true;}
    
    virtual void beVomitedOnIfAppropriate();
    
    void clearInfection(){m_isInfected = false;}
    int getInfectCount() const {return m_infectCount;}
    bool incrementInfection(){                           // return true if infection is 500 (dead)
        if(m_isInfected)
            m_infectCount++;
        return m_infectCount >= 500;
    }
    
private:
    bool m_isInfected;
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
    
    bool hasExited() const {return m_exited;}               // exit
    void exit(){m_exited = true;}
    virtual void useExitIfAppropriate();
    virtual void dieByFallOrBurnIfAppropriate();            // dying
    
private:
    int m_nLandmines;
    int m_nFlames;
    int m_nVaccines;
    bool m_exited;
    
    void useVaccine();
    void useFlamethrower();
    void dropLandmine();
};

class Zombie: public Actor{
public:
    Zombie(double startX, double startY, StudentWorld* w, int pointsWorth)
    :Actor(IID_ZOMBIE, startX, startY, w), m_movePlan(0), m_paralyzed(false), m_score(pointsWorth){}
    
    virtual void doSomething(){}
    
    virtual bool isHostile(){return true;}
    virtual bool triggersActiveLandmines() const {return true;}
    virtual void dieByFallOrBurnIfAppropriate();
    
protected:
    void vomitOrNot();          // isZombieVomitTriggerAt(double x, double y) --> put in StudentWorld
    
private:
    int m_movePlan;
    bool m_paralyzed;
    int m_score;
};

class DumbZombie: public Zombie{
public:
    DumbZombie(double startX, double startY, StudentWorld* w)
    :Zombie(startX, startY, w, 1000){
        int rand = randInt(0, 9);
        if(rand == 0)
            m_carryingVaccine = true;
        else
            m_carryingVaccine = false;
    }
    
private:
    bool m_carryingVaccine;
};

class SmartZombie: public Zombie{
public:
    SmartZombie(double startX, double startY, StudentWorld* w)
    :Zombie(startX, startY, w, 2000){}
    
};


// Wall class
class Wall: public Actor{
public:
    Wall(double startX, double startY, StudentWorld* w)
    :Actor(IID_WALL, startX, startY, w) {}
    virtual void doSomething(){}                    // Wall class does nothing
    
    virtual bool blocksFlame() const {return true;}
};

    // Activator class --> triggered by overlap
class Activator: public Actor{
public:
    Activator(int imageID, double startX, double startY, StudentWorld* w, Direction dir = right, int depth = 0)
    :Actor(imageID, startX, startY, w, dir, depth){}
    
    virtual bool isPassable() const {return true;}
    virtual void doSomething();
};

    // Exit class
class Exit: public Activator{
public:
    Exit(double startX, double startY, StudentWorld* w)
    :Activator(IID_EXIT, startX, startY, w, right, 1) {}
    
    virtual bool blocksFlame() const {return true;}
    virtual void activateIfAppropriate(Actor* a);
};

    // Pit class
class Pit: public Activator{
public:
    Pit(double startX, double startY, StudentWorld* w)
    :Activator(IID_PIT, startX, startY, w){}
    
    virtual void activateIfAppropriate(Actor* a);
};

    // Landmine class
class Landmine: public Activator{
public:
    Landmine(double startX, double startY, StudentWorld* w)
    :Activator(IID_LANDMINE, startX, startY, w, right, 1), m_safetyTics(30){}
    
    virtual void doSomething();
    virtual void activateIfAppropriate(Actor* a);
    virtual void dieByFallOrBurnIfAppropriate();
    
private:
    int m_safetyTics;
    void trigger();
};

    // Projectiles --> Flames and Vomit - last 2 tics
class Projectile: public Activator{
public:
    Projectile(int imageID, double startX, double startY, StudentWorld* w, Direction d)
    :Activator(imageID, startX, startY, w, d, 0), m_liveTics(2){}
    
    virtual void doSomething();
    virtual void activateIfAppropriate(Actor* a) = 0;
    
private:
    int m_liveTics;
};

    // Flame class
class Flame: public Projectile{
public:
    Flame(double startX, double startY, StudentWorld* w, Direction d = right)
    :Projectile(IID_FLAME, startX, startY, w, d){}
    
    virtual void activateIfAppropriate(Actor* a);
};

    //Vomit class
class Vomit: public Activator{
public:
    Vomit(double startX, double startY, StudentWorld* w, Direction d)
    :Activator(IID_VOMIT, startX, startY, w, d, 0){}
    
    virtual void activateIfAppropriate(Actor* a);
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
