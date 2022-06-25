#include "CObjectEventManager.hpp"

CObjectEventManager::CObjectEventManager(CRenderWindow * renderer)
: needsNewMap(false),
  endGame(false),
  renderer(renderer),
  currentScore(std::make_pair(0,0)),
  alivePlayers(0),
  aliveEnemies(0),
  rounds(0)
{
    this->bonusChance = loadData(config, "Bonus chance");
}

void CObjectEventManager::update()
{
    using namespace std;
    list<list<shared_ptr<CObject>>::iterator> objToRemove;

    // Render tiles - walls, breakables and grass
    for (auto & row : this->tileSet)
        for (auto & tile : row)
            tile->update();

    // Update each object
    for (auto obj = this->objects.begin(); obj != this->objects.end(); ++ obj)
    {
        (*obj)->update(this->events, this->tileSet, this->objects);
        
        if ((*obj)->toRemove)
            objToRemove.push_back(obj);
    }

    // Remove destroyed objects
    for (auto obj : objToRemove)
        this->objects.erase(obj);
}

void CObjectEventManager::manageEvents()
{
    using namespace std;
    list<Events::iterator> eventsToRemove;

    // Create a door to enother level, once all enemies are dead
    if(this->aliveEnemies == 0 && this->mode == SINGLEPLAYER)
    {
        auto pos = make_pair(mapWidth / 2, mapHeight / 2);
    
        // Replace a BREAKABLE with EMPTY, so the player can step into the door
        if (this->tileSet[pos.second][pos.first]->tileType != EMPTY)
            setTile(pos.first, pos.second, EMPTY);

        addObject(new CDoor(pos, DOOR, this->renderer->getTexture(DOOR)));
        -- this->aliveEnemies;
    }

    // Put an end to a lost game
    if (this->alivePlayers == 0 && this->mode == SINGLEPLAYER)
    {
        this->alivePlayers --;
        this->endGame = true;

        // Save the high score
        if (loadData(config, "High score") < this->currentScore.first)
            saveData(config, "High score", this->currentScore.first);
    }

    // Start next round in duel mode
    if (this->alivePlayers <= 1 && this->mode == DUEL)
    {
        -- this->rounds;
        if (! this->rounds)
            this->endGame = true;
        else
            this->needsNewMap = true;
    }

    // Iterate through events and carry them out
    for (auto event = this->events.begin(); event != this->events.end(); ++ event)
    {
        pair<int,int> pos;

        switch (get<0>(*event))
        {
        // Create a CBomb object
        case PLACE_BOMB:
            addObject(new CBomb(get<1>(*event), BOMB, this->renderer->getTexture(BOMB), get<2>(*event)));
            eventsToRemove.push_back(event);
            break;

        // Create a CExplosion object
        case PLACE_EXPLOSION:
            pos = get<1>(*event);

            // Replace a BREAKABLE with EMPTY, in case it was caught up in the explosion
            if (this->tileSet[pos.second][pos.first]->tileType == BREAKABLE)
            {
                setTile(pos.first, pos.second, EMPTY);

                // Possibly spawn a bonus at a given chance if a breakable was destroyed
                if (this->bonusChance && randomInt(1, 100) % (100 / this->bonusChance) == 0)
                    addObject(new CBonus(pos, BONUS, this->renderer->getTexture(BONUS)));
            }

            addObject(new CExplosion(pos, BOOM, this->renderer->getTexture(BOOM)));
            eventsToRemove.push_back(event);
            break;

        // Decreases the number of surviving enemies
        case ENEMY_DEAD:
            -- this->aliveEnemies;
            eventsToRemove.push_back(event);
            break;

        // Decreases the number of surviving players
        case PLAYER_DEAD:
            -- this->alivePlayers;
            eventsToRemove.push_back(event);
            break;

        // Push the player to another level
        case DOOR_REACHED:
            this->needsNewMap = true;
            break;
        
        default:
            break;
        }
    }

    // Remove events that have been carried out
    for (auto event : eventsToRemove)
        this->events.erase(event);
}

void CObjectEventManager::startGame(const std::pair<Map, int> & map)
{
    this->endGame = false;
    this->needsNewMap = false;
    this->currentScore = std::make_pair(map.second, 0);
    loadFromMap(map.first);

    // Sets the game mode - the manageEvents() needs to know, so it can
    // end the game according to the mode
    if (this->alivePlayers == 1)
        this->mode = SINGLEPLAYER;
    else
    {
        this->mode = DUEL;
        this->rounds = loadData(config,"Duel rounds");
    }
}

void CObjectEventManager::loadFromMap(const Map & map)
{
    using namespace std;

    // Clean up
    this->tileSet.clear();
    this->objects.clear();
    this->events.clear();
    this->needsNewMap = false;
    this->alivePlayers = 0;
    this->aliveEnemies = 0;

    for (int i = 0; i < mapHeight; ++ i)
    {
        vector<unique_ptr<CTile>> tmp;
        for (int j = 0; j < mapWidth; ++ j)
        {
            // Set up tiles
            if (map[i][j] == WALL)
                tmp.push_back(unique_ptr<CTile>(new CTile(j, i, WALL, this->renderer->getTexture(WALL))));

            else if (map[i][j] == BREAKABLE)
                tmp.push_back(unique_ptr<CTile>(new CTile(j, i, BREAKABLE, this->renderer->getTexture(BREAKABLE))));

            else
                tmp.push_back(unique_ptr<CTile>(new CTile(j, i, EMPTY, this->renderer->getTexture(EMPTY))));

            // Set up objects
            if (map[i][j] == PLAYER1)
            {
                addObject(new CPlayer(make_pair(j, i), PLAYER1, this->renderer->getTexture(PLAYER1),
                &this->currentScore.first, this->renderer->getText(PLAYER1_SCORE),
                SDL_SCANCODE_W,
                SDL_SCANCODE_S,
                SDL_SCANCODE_A,
                SDL_SCANCODE_D,
                SDL_SCANCODE_SPACE));
                ++ this->alivePlayers;
            }
            if (map[i][j] == PLAYER2)
            {
                addObject(new CPlayer(make_pair(j, i), PLAYER2, this->renderer->getTexture(PLAYER2),
                &this->currentScore.second, this->renderer->getText(PLAYER2_SCORE),
                SDL_SCANCODE_UP,
                SDL_SCANCODE_DOWN,
                SDL_SCANCODE_LEFT,
                SDL_SCANCODE_RIGHT,
                SDL_SCANCODE_RCTRL));
                ++ this->alivePlayers;
            }
            if (map[i][j] == ENEMY)
            {
                addObject(new CEnemy(make_pair(j, i), ENEMY, this->renderer->getTexture(ENEMY)));
                ++ this->aliveEnemies;
            }

            if (map[i][j] == BOMB)
                addObject(new CBomb(make_pair(j, i), BOMB, this->renderer->getTexture(BOMB)));

            if (map[i][j] == BOOM)
                addObject(new CExplosion(make_pair(j, i), BOOM, this->renderer->getTexture(BOOM)));

            if (map[i][j] == DOOR)
                addObject(new CDoor(make_pair(j, i), DOOR, this->renderer->getTexture(DOOR)));

            if (map[i][j] == BONUS)
                addObject(new CBonus(make_pair(j, i), BONUS, this->renderer->getTexture(BONUS)));
        }
        this->tileSet.push_back(move(tmp));
    }
}

const std::pair<Map, int> CObjectEventManager::saveIntoMap() const
{
    using namespace std;

    Map map;

    for (int i = 0; i < mapHeight; ++ i)
    {
        vector<ETileType> tmp;
        for (int j = 0; j < mapWidth; ++ j)
            tmp.push_back(this->tileSet[i][j]->tileType);

        map.push_back(tmp);
    }
    for (auto obj : this->objects)
    {
        auto pos = obj->getTilePos();
        map[pos.second][pos.first] = obj->getTile();
    }

    return make_pair(map, this->currentScore.first);
}

void CObjectEventManager::addEvent(const EEvent & event, const std::pair<int,int> & position, const int & num, CObject * obj)
{
    this->events.push_back(Event(event, position, num, obj));
}

void CObjectEventManager::addObject(CObject * obj)
{
    this->objects.push_front(std::shared_ptr<CObject>(obj));
}

void CObjectEventManager::setTile(const int & x, const int & y, const ETileType & tileType)
{
    this->tileSet[y][x].reset(new CTile(x, y, tileType, this->renderer->getTexture(tileType)));
}