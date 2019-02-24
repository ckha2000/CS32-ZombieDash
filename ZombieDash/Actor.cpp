#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


void Penelope::doSomething(){
    if(!getIsAlive()){
        return;
    }
    
    int input = 0;
    if(getWorld()->getKey(input)){
        switch(input){                  // decides what to do depending on input
            case KEY_PRESS_UP:
                setDirection(up);
                if(getWorld()->validDestination(getX(), getY()+4, this)){    // checks if destination is blocked
                    moveTo(getX(), getY()+4);
                }
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                if(getWorld()->validDestination(getX(), getY()-4, this)){
                    moveTo(getX(), getY()-4);
                }
                break;
            case KEY_PRESS_LEFT:
                setDirection(left);
                if(getWorld()->validDestination(getX()-4, getY(), this)){
                    moveTo(getX()-4, getY());
                }
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                if(getWorld()->validDestination(getX()+4, getY(), this)){
                    moveTo(getX()+4, getY());
                }
                break;
        }
    }
}

void Penelope::useExitIfAppropriate(){
    if(getWorld()->citizensLeft()){
        getWorld()->playSound(SOUND_LEVEL_FINISHED);
        exit();
    }
}

void Penelope::pickUpGoodieIfAppropriate(Goodie* g){
    g->pickup(this);
}



void Citizen::useExitIfAppropriate(){
    getWorld()->increaseScore(500);
    getWorld()->playSound(SOUND_CITIZEN_SAVED);
    setIsAlive(false);
}

void Citizen::doSomething(){
    if(!getIsAlive())           // return immediately if not alive
        return;
    
    if(isInfected()){
        if(incrementInfection()){
            setIsAlive(false);
            
            // killCitizen() function
//            getWorld()->killCitizen(getX(), getX());
            return;
        }
    }
    
    double dist_p = getWorld()->distToPenelope(getX(), getY());
    double dist_z = getWorld()->distToNearestZombie(getX(), getY());
    
    if(dist_p < dist_z && dist_p <= 80){
         getWorld()->moveToPenelope(this);                             // parameters: (pointer to citizen?)
    }
    if(dist_z <= 80){
//       avoidZombie();
    }
}

void Goodie::activateIfAppropriate(Actor *a){
    if(appropriateType(a)){
        a->pickUpGoodieIfAppropriate(this);
    }
}

void Activator::doSomething(){
    getWorld()->activateOnAppropriateActors(this);
}

void Exit::activateIfAppropriate(Actor *a){
    if(appropriateType(a)){
        a->useExitIfAppropriate();
    }
}

void Goodie::pickup(Penelope *p){
    incrementSupply(p);
    getWorld()->increaseScore(50);
    getWorld()->playSound(SOUND_GOT_GOODIE);
    setIsAlive(false);
}

