#include "CEnemy.hpp"

CEnemy::CEnemy(const std::pair<int,int> & position, const ETileType & tile,
               const std::shared_ptr<CRenderWindow::CTexture> & texture)
: CObject(position, tile, texture),
  availableDirections({STAY, UP, DOWN, LEFT, RIGHT})
{
    setDirection();
}

void CEnemy::update(Events & events, const TileSet & tileSet, const std::list<std::shared_ptr<CObject>> & objects)
{
    render();
    move(tileSet, objects);
    createEvents(events, tileSet, objects);
}

void CEnemy::createEvents(Events & events, const TileSet & tileSet, const std::list<std::shared_ptr<CObject>> & objects)
{
    using std::make_pair;

    // Enemy was caught in an explosion
    if (objectCollision(objects, BOOM))
    {
        this->toRemove = true;
        events.emplace_back(POINTS, make_pair(0, 0), 100, nullptr);
        events.emplace_back(ENEMY_DEAD, make_pair(0, 0), 0, nullptr);
    }
}

std::pair<int,int> CEnemy::getTilePos() const
{
    int x = (this->position.first + tileWidth * (xBox + wBox)) / tileWidth;
    int y = (this->position.second + tileWidth * (yBox + hBox)) / tileWidth;

    return std::make_pair(x, y);
}

void CEnemy::move(const TileSet & tileSet, const std::list<std::shared_ptr<CObject>> & objects)
{
    int dirX = 0;
    int dirY = 0;

    // Moves the enemy in a given direction at a given speed
    switch (this->currentDirection)
    {
    case UP:
        dirY = -1;
        changePos(this->position.first, this->position.second - enemySpeed);
        break;
    
    case DOWN:
        dirY = 1;
        changePos(this->position.first, this->position.second + enemySpeed);
        break;

    case LEFT:
        dirX = -1;
        changePos(this->position.first - enemySpeed, this->position.second);
        break;

    case RIGHT:
        dirX = 1;
        changePos(this->position.first + enemySpeed, this->position.second);
        break;
    
    default:
        break;
    }

    -- this->frameNumber;
    setCollisionBox();

    if (objectCollision(objects, BOOM))
    {
        this->position.first -= enemySpeed * dirX;
        this->position.second -= enemySpeed * dirY;
        this->availableDirections.erase(this->currentDirection);

        setDirection();
    }

    // The collision box is temporarily set from a CObject, so the enemy moves steadily
    CObject::setCollisionBox();

    // Pushes the enemy back in case it wallked into a wall or an explosion
    // It then removes the direction of its movement from the pool an tries a different one
    if (wallCollision(tileSet))
    {
        this->position.first -= enemySpeed * dirX;
        this->position.second -= enemySpeed * dirY;
        this->availableDirections.erase(this->currentDirection);

        setDirection();
    }

    // The enemy finished moving in a direction, restore the direction pool and start the loop again
    else if(! this->frameNumber)
    {
        this->availableDirections.insert({STAY, UP, DOWN, LEFT, RIGHT});
        setDirection();
    }

    // Set the enemy's actual collision box for collision with players and explosions
    setCollisionBox();
}

void CEnemy::setDirection()
{
    // An insurance against errors, in case the direction pool is empty
    // (it shouldn't happen though)
    if (this->availableDirections.empty())
    {
        this->currentDirection = STAY;
        return;
    }

    // Randomly chooses the direction
    auto it = this->availableDirections.begin();
    this->currentDirection = *next(it, randomInt(0, this->availableDirections.size() - 1));

    // Shorten the time for which the enemy stands in place
    // We don't want it to stay in place for too long
    if (this->currentDirection == STAY)
        this->frameNumber = tileWidth / enemySpeed;
    else
        this->frameNumber = randomInt(1, std::max(mapHeight, mapWidth) / 2) * tileWidth / enemySpeed;
}

void CEnemy::setCollisionBox()
{
    this->box.x = this->position.first + tileWidth * xBox;
    this->box.y = this->position.second + tileWidth * yBox;
    this->box.w = tileWidth - tileWidth * wBox;
    this->box.h = tileWidth - tileWidth * hBox;
}