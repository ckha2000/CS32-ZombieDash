#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;
class Goodie;

//////////////
//////////////          Actor class

class Actor: public GraphObject{
public:
    Actor(int imageID, double startX, double startY, StudentWorld* w, Direction dir = right, int depth = 0)
    :GraphObject(imageID, startX, startY, dir, depth), m_isAlive(true), m_world(w) {}
    
    virtual ~Actor(){};
    virtual void doSomething() = 0;

    // object characteristics
    virtual bool isPassable() const {return false;}     // returns whether object blocks movement
    virtual bool blocksFlame() const {return false;}    // returns whether object blocks flames
    virtual bool isInfectable() const {return false;}   // only Penelope and citizens can be Infected
    virtual bool triggersActiveLandmines() const {return false;}    // for people and zombies
    
    // mutators/accessors
    void setIsAlive(bool isAlive){ m_isAlive = isAlive; }
    bool getIsAlive() const {return m_isAlive;}
    StudentWorld* getWorld() const {return m_world;}
    
    // overlap functions
    virtual void activateIfAppropriate(Actor* a){}      // for activators
    virtual void useExitIfAppropriate(){}               // for people
    virtual void dieByFallOrBurnIfAppropriate(){}       // for people, zombies, goodies and landmines
    virtual void pickUpGoodieIfAppropriate(Goodie* g){} // for penelope
    virtual void beVomitedOnIfAppropriate(){}           // for people
    
private:
    bool m_isAlive;
    StudentWorld* m_world;
};

//////////////
//////////////          Agent

class Agent: public Actor{                 // People and Zombies
public:
    Agent(int imageID, double startX, double startY, StudentWorld* w, double speed = 0.0)
    :Actor(imageID, startX, startY, w), m_paralyzed(false), m_moveSpeed(speed){}
    
    virtual bool triggersActiveLandmines() const {return true;}
    
protected:
    bool getParalyzed() const {return m_paralyzed;}
    void setParalyzed(bool p){m_paralyzed = p;}
    double getMoveSpeed(){return m_moveSpeed;}
    void determineMoveDest(Direction d, double& destX, double& destY);
    
private:
    bool m_paralyzed;
    double m_moveSpeed;
};


//////////////
//////////////          Person

class Person: public Agent{             // Citizens and Penelope
public:
    Person(int imageID, double startX, double startY, StudentWorld* w, double speed = 0.0)
    :Agent(imageID, startX, startY, w, speed), m_isInfected(false), m_infectCount(0) {}
    
    virtual bool isInfectable() const {return true;}
    virtual void beVomitedOnIfAppropriate();
    int getInfectCount() const {return m_infectCount;}
    
protected:
    virtual void dieByInfection() = 0;
    bool incrementInfection();                           // return true if infection is 500 (dead)
    bool isInfected(){return m_isInfected;}
    void clearInfection(){m_isInfected = false;}         // called when Penelope uses vaccine
    
private:
    bool m_isInfected;
    int m_infectCount;
};

//////////////
//////////////          Citizen

class Citizen: public Person{
public:
    Citizen(double startX, double startY, StudentWorld* w)
    :Person(IID_CITIZEN, startX, startY, w, 2.0){}
    
    virtual void doSomething();
    
    virtual void beVomitedOnIfAppropriate();
    virtual void useExitIfAppropriate();
    virtual void dieByFallOrBurnIfAppropriate();
    
protected:
    virtual void dieByInfection();

private:
    bool tryDirections(Direction v, Direction h, Direction& df, double& destX, double& destY);
};

//////////////
//////////////          Penelope class

class Penelope: public Person{
public:
    Penelope(double startX, double startY, StudentWorld* w)
    :Person(IID_PLAYER, startX, startY, w), m_exited(false)
    {
        m_nLandmines = m_nFlames = m_nVaccines = 0;
    }
    virtual void doSomething();
    
    // Accessor functions --> called for updating status message
    int getLandmines() const {return m_nLandmines;}
    int getFlames() const {return m_nFlames;}
    int getVaccines() const {return m_nVaccines;}
    bool hasExited() const {return m_exited;}
    
    // Mutator functions --> called by Goodies' activation function
    void addLandmines(int charges){m_nLandmines += charges;} // mutators
    void addFlames(int charges){m_nFlames += charges;}
    void addVaccines(int charges){m_nVaccines += charges;}
    
    // overrides
    virtual void pickUpGoodieIfAppropriate(Goodie* g);
    virtual void useExitIfAppropriate();
    virtual void dieByFallOrBurnIfAppropriate();
    
protected:
    virtual void dieByInfection();
    
private:
    int m_nLandmines;
    int m_nFlames;
    int m_nVaccines;
    bool m_exited;
    
    // helper functions
    void useVaccine();
    void useFlamethrower();
    void dropLandmine();
};

//////////////
//////////////          Zombie

class Zombie: public Agent{
public:
    Zombie(double startX, double startY, StudentWorld* w, int pointsWorth)
    :Agent(IID_ZOMBIE, startX, startY, w, 1), m_movePlan(0), m_score(pointsWorth){}
    
    virtual void doSomething();
    virtual void dieByFallOrBurnIfAppropriate();
    
protected:
    virtual Direction chooseDirection();
    
private:
    int m_movePlan;
    int m_score;
    bool vomitIfTarget();
};

class DumbZombie: public Zombie{
public:
    DumbZombie(double startX, double startY, StudentWorld* w)
    :Zombie(startX, startY, w, 1000){}
    
    virtual void dieByFallOrBurnIfAppropriate();
    
private:
    void throwVaccine();
};

class SmartZombie: public Zombie{
public:
    SmartZombie(double startX, double startY, StudentWorld* w)
    :Zombie(startX, startY, w, 2000){}
    
protected:
    virtual Direction chooseDirection();
};


//////////////
//////////////          Wall class
class Wall: public Actor{
public:
    Wall(double startX, double startY, StudentWorld* w)
    :Actor(IID_WALL, startX, startY, w) {}
    virtual void doSomething(){}                    // Wall class does nothing
    
    virtual bool blocksFlame() const {return true;}
};

//////////////
//////////////          Activators

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

//////////////
//////////////
    // Projectiles --> Flames and Vomit - last 2 tics
class Projectile: public Activator{
public:
    Projectile(int imageID, double startX, double startY, StudentWorld* w, Direction d)
    :Activator(imageID, startX, startY, w, d, 0), m_liveTics(2){}
    
    virtual void doSomething();
    
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
class Vomit: public Projectile{
public:
    Vomit(double startX, double startY, StudentWorld* w, Direction d)
    :Projectile(IID_VOMIT, startX, startY, w, d){}
    
    virtual void activateIfAppropriate(Actor* a);
};

//////////////
//////////////          Goodies
class Goodie: public Activator{
public:
    Goodie(int imageID, double startX, double startY, StudentWorld* w)
    :Activator(imageID, startX, startY, w, right, 1){}
    
    virtual void activateIfAppropriate(Actor* a);
    void pickup(Penelope* p);
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
