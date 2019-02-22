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

void Exit::doSomething(){
    getWorld()->exitOverlap(getX(), getY());
}

void Citizen::doSomething(){
    if(!getIsAlive())       // return immediately if not alive
        return;
    
    if(isInfected()){
        if(incrementInfection()){
            setIsAlive(false);
            
            // killCitizen() function
//            getWorld()->killCitizen(getX(), getX());
            return;
        }
    }
    
    
    
    
    
}
