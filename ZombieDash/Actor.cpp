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
            case KEY_PRESS_ENTER:
                useVaccine();
                break;
            case KEY_PRESS_SPACE:
                useFlamethrower();
                break;
            case KEY_PRESS_TAB:
                dropLandmine();
                break;
        }
    }
}

void Penelope::useVaccine(){
    if(m_nVaccines > 0 && isInfected()){
        setInfected(false);
    }
    m_nVaccines--;
}

void Penelope::useFlamethrower(){
    if(m_nFlames > 0){
        m_nFlames--;
        getWorld()->playSound(SOUND_PLAYER_FIRE);
        
        Direction d = getDirection();
        
        double destX = getX();
        double destY = getY();
    
        for(int i = 0; i < 3; i++){
            switch (d) {
                case right:
                    destX += SPRITE_WIDTH;
                    break;
                case left:
                    destX -= SPRITE_WIDTH;
                    break;
                case up:
                    destY += SPRITE_HEIGHT;
                    break;
                case down:
                    destY -= SPRITE_HEIGHT;
            }
            
            if(getWorld()->validFlameDestination(destX, destY)){
                getWorld()->addActor(new Flame(destX, destY, getWorld(), d));
            }else
                break;
        }
    }
}

void Penelope::dropLandmine(){
    if(m_nLandmines > 0){
        m_nLandmines--;
        getWorld()->addActor(new Landmine(getX(), getY(), getWorld()));
    }
}

void Penelope::useExitIfAppropriate(){
    if(getWorld()->citizensLeft()){
        getWorld()->playSound(SOUND_LEVEL_FINISHED);
        exit();
    }
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

// Activator doSomething()'s
void Activator::doSomething(){
    if(getIsAlive()){
        getWorld()->activateOnAppropriateActors(this);
    }
}

void Flame::doSomething(){
    if(m_liveTicks <= 0){
        setIsAlive(false);
        return;
    }
    Activator::doSomething();
    m_liveTicks--;
}

void Landmine::doSomething(){
    if(m_safetyTics > 0){
        m_safetyTics--;
        return;
    }
    Activator::doSomething();
}

void Exit::activateIfAppropriate(Actor* a){
        a->useExitIfAppropriate();
}

void Goodie::activateIfAppropriate(Actor* a){
    if(getIsAlive())
        a->pickUpGoodieIfAppropriate(this);
}

void Penelope::pickUpGoodieIfAppropriate(Goodie* g){
    g->pickup(this);
}

void Goodie::pickup(Penelope* p){
    incrementSupply(p);
    getWorld()->increaseScore(50);
    getWorld()->playSound(SOUND_GOT_GOODIE);
    setIsAlive(false);
}

void Landmine::trigger(){
    setIsAlive(false);
    getWorld()->playSound(SOUND_LANDMINE_EXPLODE);
    
    getWorld()->addActor(new Pit(getX(), getY(), getWorld()));
    
    double posX = getX() - SPRITE_WIDTH;
    double posY = getY() - SPRITE_HEIGHT;
    
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(getWorld()->validFlameDestination(posX, posY))
                getWorld()->addActor(new Flame(posX, posY, getWorld()));
            posX += SPRITE_WIDTH;
        }
        posX = getX() - SPRITE_WIDTH;
        posY += SPRITE_HEIGHT;
    }
}

    // hazard activations
void Pit::activateIfAppropriate(Actor* a){
    a->dieByFallOrBurnIfAppropriate();
}

void Flame::activateIfAppropriate(Actor* a){
    a->dieByFallOrBurnIfAppropriate();
}

void Landmine::activateIfAppropriate(Actor *a){
    if(a->triggersActiveLandmines())
        trigger();
}

    // dieByFallOrBurnIfAppropriate()
void Citizen::dieByFallOrBurnIfAppropriate(){
    setIsAlive(false);
    getWorld()->increaseScore(-1000);
    getWorld()->playSound(SOUND_CITIZEN_DIE);
}

void Penelope::dieByFallOrBurnIfAppropriate(){
    setIsAlive(false);
    getWorld()->decLives();
    getWorld()->playSound(SOUND_PLAYER_DIE);
}

void Goodie::dieByFallOrBurnIfAppropriate(){
    setIsAlive(false);
}

void Landmine::dieByFallOrBurnIfAppropriate(){
    trigger();
}

/*
void Zombie::dieByFallOrBurnIfAppropriate(){
 setIsAlive(false);
 getWorld()->increaseScore(m_score);
 getWorld()->playSound(SOUND_ZOMBIE_DIE);
}
*/
