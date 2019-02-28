#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


//////////////
//////////////          Agent

// calculates the move destination for a citizen for a given Direction
void Agent::determineMoveDest(Direction d, double& destX, double& destY){
    destX = getX();
    destY = getY();
    
    switch(d){
        case up:
            destY += m_moveSpeed;
            break;
        case down:
            destY -= m_moveSpeed;
            break;
        case left:
            destX -= m_moveSpeed;
            break;
        case right:
            destX += m_moveSpeed;
            break;
    }
}

//////////////
//////////////          Person

void Person::beVomitedOnIfAppropriate(){
    if(!m_isInfected){
        m_isInfected = true;
    }
}

bool Person::incrementInfection(){       // increments the infection count and returns whether the count exceeds 500
    if(m_isInfected)
        m_infectCount++;
    if(m_infectCount >= 500){
        dieByInfection();
        return true;
    }
    return false;
}

//////////////
//////////////          Penelope

void Penelope::doSomething(){
    if(!getIsAlive()){              // check alive state first
        return;
    }
    
    if(incrementInfection())        // increment infection -> if died from infection, return immediately
        return;
    
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
    if(m_nVaccines > 0){
        clearInfection();
        m_nVaccines--;
    }
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
        m_exited = true;
    }
}

void Penelope::dieByInfection(){
    setIsAlive(false);
    getWorld()->playSound(SOUND_PLAYER_DIE);
}

void Penelope::pickUpGoodieIfAppropriate(Goodie* g){
    g->pickup(this);
}

void Penelope::dieByFallOrBurnIfAppropriate(){
    setIsAlive(false);
    getWorld()->decLives();
    getWorld()->playSound(SOUND_PLAYER_DIE);
}

//////////////
//////////////          Citizen

void Citizen::doSomething(){
    if(!getIsAlive())           // return immediately if not alive
        return;
    
    if(incrementInfection())    // return immediately if dead after incrementing infection
        return;
    
    if(getParalyzed()){            // paralyzed every other tic
        setParalyzed(false);
        return;
    }else
        setParalyzed(true);
    
    double triggerX, triggerY, triggerDist;
    triggerX = triggerY = triggerDist = 0.0;
    bool isThreat;
    
    // if true, it means that no trigger is within 80 units or there are no Triggers in the level so do nothing
    if(!getWorld()->locateNearestCitizenTrigger(getX(), getY(), triggerX, triggerY, triggerDist, isThreat) || triggerDist > 80)
        return;
    
    // the closest trigger is Penelope
    if(!isThreat){
        Direction vertical = -1;
        Direction horizontal = -1;
        
        // find the two Directions that the Citizen could move to in order to get closer to Penelope
        if(triggerX > getX()){
            horizontal = right;
        }else if(triggerX < getX()){
            horizontal = left;
        }
        if(triggerY > getY()){
            vertical = up;
        }else if(triggerY < getY()){
            vertical = down;
        }
        
        Direction d;
        double destX, destY;
        if(tryDirections(vertical, horizontal, d, destX, destY)){       // tries both directions
            setDirection(d);                    // if movement is valid, moves and returns immediately
            moveTo(destX, destY);
            return;
        }
    }
    
    // if true, there are no threats in the level or within 80 units so do nothing
    if(!getWorld()->locateNearestCitizenThreat(getX(), getY(), triggerX, triggerY, triggerDist) || triggerDist > 80){
        return;
    }

    double farthestDist = triggerDist;      // farthestDist current is the closest threat to the Citizen
    double tempDist = 0.0;                  // we want to find directions that make the distance bigger
    Direction safestDir = -1;
    
    double destX, destY, threatX, threatY;
                                            // compare the distances to the closest zombie in each direction
    // up
    determineMoveDest(up, destX, destY);
    if(getWorld()->validDestination(destX, destY, this)){
        if(getWorld()->locateNearestCitizenThreat(destX, destY, threatX, threatY, tempDist)){
            if(tempDist > farthestDist){
                farthestDist = tempDist;
                safestDir = up;
            }
        }
    }
    // down
    determineMoveDest(down, destX, destY);
    if(getWorld()->validDestination(destX, destY, this)){
        if(getWorld()->locateNearestCitizenThreat(destX, destY, threatX, threatY, tempDist)){
            if(tempDist > farthestDist){
                farthestDist = tempDist;
                safestDir = down;
            }
        }
    }
    // left
    determineMoveDest(left, destX, destY);
    if(getWorld()->validDestination(destX, destY, this)){
        if(getWorld()->locateNearestCitizenThreat(destX, destY, threatX, threatY, tempDist)){
            if(tempDist > farthestDist){
                farthestDist = tempDist;
                safestDir = left;
            }
        }
    }
    // right
    determineMoveDest(right, destX, destY);
    if(getWorld()->validDestination(destX, destY, this)){
        if(getWorld()->locateNearestCitizenThreat(destX, destY, threatX, threatY, tempDist)){
            if(tempDist > farthestDist){
                farthestDist = tempDist;
                safestDir = right;
            }
        }
    }

    if(safestDir == -1)
        return;
    
    // safestDir should give us the direction that has the greatest distance from
    determineMoveDest(safestDir, destX, destY);
    moveTo(destX, destY);
}

// given two possible directions, select one randomly and see
// if movement is possible is possible in that Direction
// if not, try the other Direction
    // if either Direction works, set df that that direction, caluculate destX and destY, and return true
// if both Directions don't work, return false and leave variables unchanged
bool Citizen::tryDirections(Direction v, Direction h, Direction& df, double& destX, double& destY){
    Direction first;
    Direction second;
    
    if(randInt(0,1)){   // decide order of trying Directions
        first = v;
        second = h;
    }else{
        first = h;
        second = v;
    }
    
    double tempX, tempY;
    
    if(first != -1){
        determineMoveDest(first, tempX, tempY);
        if(getWorld()->validDestination(tempX, tempY, this)){
            df = first;
            destX = tempX;
            destY = tempY;
            return true;
        }
    }
    if(second != -1){
        determineMoveDest(second, tempX, tempY);
        if(getWorld()->validDestination(tempX, tempY, this)){
            df = second;
            destX = tempX;
            destY = tempY;
            return true;
        }
    }
    return false;   // neither direction results in valid movement
}

void Citizen::useExitIfAppropriate(){
    getWorld()->increaseScore(500);
    getWorld()->playSound(SOUND_CITIZEN_SAVED);
    setIsAlive(false);
}

void Citizen::dieByInfection(){
    setIsAlive(false);
    getWorld()->increaseScore(-1000);
    
    getWorld()->playSound(SOUND_ZOMBIE_BORN);
    int rand = randInt(0, 9);
    if(rand <= 2){              // there is a 30% chance of generating a smart zombie (0 - 2)
        getWorld()->addActor(new SmartZombie(getX(), getY(), getWorld()));
    }else{                      // else, (3 - 9) a DumbZombie is generated instead
        getWorld()->addActor(new DumbZombie(getX(), getY(), getWorld()));
    }
}

void Citizen::dieByFallOrBurnIfAppropriate(){
    setIsAlive(false);
    getWorld()->increaseScore(-1000);
    getWorld()->playSound(SOUND_CITIZEN_DIE);
}

void Citizen::beVomitedOnIfAppropriate(){
    if(!isInfected()){
        getWorld()->playSound(SOUND_CITIZEN_INFECTED);
        Person::beVomitedOnIfAppropriate();
    }
}


//////////////
//////////////      Zombie

void Zombie::doSomething(){
    if(!getIsAlive()){          // check if alive
        return;
    }
    
    if(getParalyzed()){            // paralyzed every other tic
        setParalyzed(false);
        return;
    }else
        setParalyzed(true);
    
    if(vomitIfTarget())
        return;

    if(m_movePlan <= 0){
        m_movePlan = randInt(3, 10);
        setDirection(chooseDirection());   // DumbZombie and SmartZombie only
    }                                       // differ in the direction of their movePlan
    
    double destX, destY;
    determineMoveDest(getDirection(), destX, destY);
    
    if(getWorld()->validDestination(destX, destY, this)){
        moveTo(destX, destY);
        m_movePlan--;
    }else{
        m_movePlan = 0;
    }
}

bool Zombie::vomitIfTarget(){
    int rand = randInt(0, 2);       // there is only a 1 in 3 chance of vomiting so
    if(rand > 0){                   // if the randInt is 0, the zombie will vomit
        return false;
    }
    
    double tarX = getX();
    double tarY = getY();
    
    switch(getDirection()){             // targets location directly in front of zombie
        case right:
            tarX += SPRITE_WIDTH;
            break;
        case left:
            tarX -= SPRITE_WIDTH;
            break;
        case up:
            tarY += SPRITE_HEIGHT;
            break;
        case down:
            tarY -= SPRITE_HEIGHT;
            break;
    }
    
    if(getWorld()->validVomitTargetAt(tarX, tarY)){    // checks whether target location has an Infectable actor
        getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
        getWorld()->addActor(new Vomit(tarX, tarY, getWorld(), getDirection()));
        return true;
    }
    return false;
}

void Zombie::dieByFallOrBurnIfAppropriate(){
    setIsAlive(false);
    getWorld()->increaseScore(m_score);
    getWorld()->playSound(SOUND_ZOMBIE_DIE);
}

void DumbZombie::dieByFallOrBurnIfAppropriate(){
    int rand = randInt(0, 9);         // DumbZombies have a 1/10 chance of dropping a vaccine
    if(rand == 0){
        throwVaccine();
    }
    Zombie::dieByFallOrBurnIfAppropriate();
}

void DumbZombie::throwVaccine(){
    Direction d = Zombie::chooseDirection();        // DumbZombie's chooseDirection() gives a random Direction
    
    double tarX = getX();
    double tarY = getY();
    
    switch(d){             // targets location directly in front of zombie
        case right:
            tarX += SPRITE_WIDTH;
            break;
        case left:
            tarX -= SPRITE_WIDTH;
            break;
        case up:
            tarY += SPRITE_HEIGHT;
            break;
        case down:
            tarY -= SPRITE_HEIGHT;
            break;
    }
    
    if(getWorld()->isEmptyLocation(tarX, tarY)){
        getWorld()->addActor(new VaccineGoodie(tarX, tarY, getWorld()));
    }
}

Direction Zombie::chooseDirection(){
    int rand = randInt(0, 3);               // DumbZombies choose a random Direction regardless of positions of
                                                // other actors
    switch(rand){
        case 0:
            return right;
            break;
        case 1:
            return down;
            break;
        case 2:
            return left;
            break;
        default:
            return up;
    }
}

Direction SmartZombie::chooseDirection(){
    double x , y, d;
    x = y = d = 0.0;
    
    if(!getWorld()->locateNearestVomitTrigger(getX(), getY(), x, y, d)){    // if no targets in range
        return Zombie::chooseDirection();                                       // choose a random Direction
    }
    
    Direction vertical = -1;
    Direction horizontal = -1;
    
    if(x < getX()){
        horizontal = left;
    }else if(x > getX()){
        horizontal = right;
    }
    if(y < getY()){
        vertical = down;
    }else if (y > getY()){
        vertical = up;
    }
    
    if(vertical == -1){     // if either direction is vertical, it means that the zombie is alligned in the other
        return horizontal;          // direction with the Actor
    }else if(horizontal == -1){
        return vertical;
    }else{
        int rand = randInt(0, 1);
        if(rand == 0)
            return horizontal;
        else
            return vertical;
    }
}

//////////////
//////////////          Activators

void Activator::doSomething(){
    if(getIsAlive()){
        getWorld()->activateOnAppropriateActors(this);
    }
}

    // Exit
void Exit::activateIfAppropriate(Actor* a){
    a->useExitIfAppropriate();
}

    // Pit
void Pit::activateIfAppropriate(Actor* a){
    a->dieByFallOrBurnIfAppropriate();
}

    // Landmine
void Landmine::doSomething(){
    if(m_safetyTics > 0){
        m_safetyTics--;
        return;
    }
    Activator::doSomething();
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

void Landmine::dieByFallOrBurnIfAppropriate(){
    trigger();
}

void Landmine::activateIfAppropriate(Actor *a){
    if(a->triggersActiveLandmines())
        trigger();
}

//////////////
//////////////          Projectiles

void Projectile::doSomething(){
    if(m_liveTics <= 0){
        setIsAlive(false);
        return;
    }
    Activator::doSomething();
    m_liveTics--;
}

void Flame::activateIfAppropriate(Actor* a){
    a->dieByFallOrBurnIfAppropriate();
}

void Vomit::activateIfAppropriate(Actor* a){
    a->beVomitedOnIfAppropriate();
}

//////////////
//////////////          Goodies

void Goodie::activateIfAppropriate(Actor* a){
    if(getIsAlive())
        a->pickUpGoodieIfAppropriate(this);
}

void Goodie::pickup(Penelope* p){
    incrementSupply(p);
    getWorld()->increaseScore(50);
    getWorld()->playSound(SOUND_GOT_GOODIE);
    setIsAlive(false);
}

void Goodie::dieByFallOrBurnIfAppropriate(){
    setIsAlive(false);
}
