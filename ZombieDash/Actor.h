#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

    // Actor class
class Actor: public GraphObject{
public:
    Actor(int imageID, double startX, double startY, StudentWorld* w, Direction dir = right, int depth = 0)
    :GraphObject(imageID, startX, startY, dir, depth)
    {
        m_world = w;
    }
    
    virtual ~Actor(){};
    virtual void doSomething() = 0;
    
    virtual bool isPassable() {return true;}            // returns whether object can be overlapped/touched
    virtual bool isDamagable() {return true;}           // everything but flames, exits and pitfalls can be damaged
    virtual bool isInfectable() {return false;}         // only Penelope and citizens can be Infected
    virtual bool isSavable() {return false;}            // only citizens can be saved
    
//    virtual void getDamaged(){}         // maybe later
    
    void setIsAlive(bool isAlive){ m_isAlive = isAlive; }
    bool getIsAlive(){ return m_isAlive; }
    
    virtual void setInfected(bool inf){}                // for People only
    virtual bool isInfected(){ return false;}           // only use if Infectable
    
    virtual void getSaved(){}                           // use for Citizens taht overlap with exit
    
    StudentWorld* getWorld(){ return m_world;}

private:
    bool m_isAlive = true;
    StudentWorld* m_world;
};

    // Wall class
class Wall: public Actor{
public:
    Wall(double startX, double startY, StudentWorld* w)
    :Actor(IID_WALL, startX, startY, w) {}
    virtual void doSomething(){}                    // Wall class does nothing
    
    virtual bool isPassable() {return false;}
    virtual bool isDamagable(){return false;}
};

class Person: public Actor{             // Citizens and Zombies
public:
    Person(int imageID, double startX, double startY, StudentWorld* w)
    :Actor(imageID, startX, startY, w)
    {
        m_infectCount = 0;
    }
    
    virtual bool isPassable() {return false;}       // false for all People
    virtual bool isInfectable() {return true;}
    
    // mutators and accessors
    virtual void setInfected(bool inf){ m_isInfected = inf;}
    virtual bool isInfected(){ return m_isInfected;}
    int getInfectCount(){return m_infectCount;}
    
private:
    bool m_isInfected = false;
    int m_infectCount;
};

class Citizen: public Person{
public:
    Citizen(double startX, double startY, StudentWorld* w)
    :Person(IID_CITIZEN, startX, startY, w){}
    
    virtual void doSomething();
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
    
    int getLandmines(){return m_nLandmines;}        // accessors
    int getFlames(){return m_nFlames;}
    int getVaccines(){return m_nVaccines;}
    
    bool hasExited(){return m_exited;}
    void exit(){m_exited = true;}
    
private:
    int m_nLandmines;
    int m_nFlames;
    int m_nVaccines;
    
    bool m_exited;
};

    // Exit class
class Exit: public Actor{
public:
    Exit(double startX, double startY, StudentWorld* w)
    :Actor(IID_EXIT, startX, startY, w, right, 1) {}
    virtual void doSomething();
    
    virtual bool isDamagable(){return false;}
    
};


#endif // ACTOR_H_
