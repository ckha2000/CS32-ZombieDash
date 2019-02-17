#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


void Penelope::doSomething(){
    int input = 0;
    
    if(m_world->getKey(input)){
        switch(input){
            case KEY_PRESS_UP:
                setDirection(up);
                if(m_world->validDestination(getX(), getY()+4)){
                    moveTo(getX(), getY()+4);
                }
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                if(m_world->validDestination(getX(), getY()-4)){
                    moveTo(getX(), getY()-4);
                }
                break;
            case KEY_PRESS_LEFT:
                setDirection(left);
                if(m_world->validDestination(getX()-4, getY())){
                    moveTo(getX()-4, getY());
                }
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                if(m_world->validDestination(getX()+4, getY())){
                    moveTo(getX()+4, getY());
                }
                break;
        }
    }
}
