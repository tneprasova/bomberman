#include "CMap.hpp"

int CMap::numberOfBreakables = -1;
int CMap::numberOfEnemies = -1;

CMap::CMap(const EGameMode & mode, const std::string & saveFile)
: score(0),
  saveFile(saveFile)
{    
    using std::vector, std::make_pair;

    if (this->numberOfBreakables == -1 || this->numberOfEnemies == -1)
    {
        this->numberOfBreakables = loadData(config, "Breakables");
        this->numberOfEnemies = loadData(config, "Enemies");
    }

    // Sets up all the unbreakable walls and the wallkable (empty) tiles
    for (int y = 0; y != mapHeight; ++ y)
    {        
        vector<ETileType> tmp;
        for (int x = 0; x != mapWidth; ++ x)
        {
            if (y == 0 || y == mapHeight - 1)
                tmp.push_back(WALL);

            else if ((x % 2 == 0 && y % 2 == 0) || x == 0 || x == mapWidth - 1)
                tmp.push_back(WALL);

            else
            {
                tmp.push_back(EMPTY);
                this->availableTiles.insert(make_pair(x, y));
            }
        }
        this->map.push_back(tmp);
        tmp.clear();
    }
    // Tiles taken by players, enemies and the ones around the players get reserved
    // during the map's creation, so they are not rewritten to something else
    setPlayer(PLAYER1);

    if (mode == SINGLEPLAYER)
        setEnemies();

    // Add a second player for duel
    else
        setPlayer(PLAYER2);                                                                                                                                       

    setBreakables();
}

void CMap::save() const
{
    using std::stringstream;

    // First save the score
    saveData(this->saveFile.c_str(), "Score", this->score);
    stringstream stream("");

    // Create a stream of enums out of the map
    for (int y = 0; y != mapHeight; ++ y)
    {
        for (int x = 0; x != mapWidth; ++ x)
        {
            stream << this->map[y][x];
            if (x != mapWidth - 1)
                stream << " ";
        }
        if (y != mapHeight - 1)
            stream << "\n";
    }
    // Finally save the map
    saveData(this->saveFile.c_str(), "Map", stream);
}

void CMap::load()
{
    using namespace std;

    // Set up the score
    this->score = loadData(this->saveFile.c_str(), "Score");
    if (this->score < 0)
        throw invalid_argument("Loaded negative score");

    stringstream map = loadData(this->saveFile.c_str(), "Map", true);   // loaded map stream
    string line;                                                        // loaded line
    Map res;                                                            // result map

    // Loads the stream line by line and then parses it into separate tiles
    for (int j = 0; j < mapHeight; ++ j)
    {
        vector<ETileType> tmp;
        getline(map, line);
        trim(line);     // Trims excessive white spaces
        stringstream num(line);
        int n;

        for(int i = 0; i < mapWidth; ++ i)
        {
            // Load tile by tile
            num >> n;
            // Checks, whether the width of the map is correct
            if (num.eof() && i + 1 != mapWidth)
                mapLoadError();

            // Check, whether the tiles are in range
            if (n < 0 || n > ETileType_MAX)
                throw invalid_argument("Unknown tile type loaded"s);

            // Finaly set the tile
            tmp.push_back(ETileType(n));
        }
        // Checks, whether the width of the map is correct
        if (! num.eof())
            mapLoadError();

        res.push_back(tmp);

        if (map.eof() && j + 1 != mapHeight)
            mapLoadError();
    }
    // Checks, whether the height of the map is correct
    if (! map.eof())
        mapLoadError();

    // When everything went well, set the map up
    setUp(res);
}

void CMap::setMap(const std::pair<Map, int> & data)
{
    this->map = data.first;
    this->score = data.second;
}

std::pair<Map, int> CMap::getMap() const
{
    return std::make_pair(this->map, this->score);
}

void CMap::setTile(const int & x, const int & y, const ETileType & tile) { this->map[y][x] = tile; }

void CMap::setPlayer(const ETileType & player) { takeAdjacentTiles(takeRandomTile(player)); }

void CMap::setEnemies()
{
    for (int i = 0; i < this->numberOfEnemies; ++ i)
    {
        if (this->availableTiles.empty())
            break;
        takeRandomTile(ENEMY);
    }
}

void CMap::setBreakables()
{
    for (int i = 0; i < this->numberOfBreakables; ++ i)
    {
        if (this->availableTiles.empty())
            break;
        takeRandomTile(BREAKABLE);
    }
}

std::pair<int, int> CMap::takeRandomTile(const ETileType & tileType)
{
    // Randomly chooses an available tile
    auto selectRandom = [&] ()
    {
        auto it = this->availableTiles.begin();
        std::advance(it, randomInt(0, this->availableTiles.size() - 1));
        return it;
    };

    auto tile = selectRandom();
    // A temporary copy is needed so we can continue working with the data after erasing
    // the iterator from the container
    auto tmp = *tile;
    this->availableTiles.erase(tile);
    setTile(tmp.first, tmp.second, tileType);

    return tmp;
}

void CMap::takeAdjacentTiles(const std::pair<int, int> & tile)
{    
    for (int x = tile.first - 1; x <= tile.first + 1; ++ x)
        for (int y = tile.second - 1; y <= tile.second + 1; ++ y)
            this->availableTiles.erase(std::make_pair(x, y));
}

void CMap::setUp(const Map & newMap)
{
    using namespace std;

    // Iterate through tiles of the map
    for (int y = 0; y != mapHeight; ++ y)
    {
        for (int x = 0; x != mapWidth; ++ x)
        {
            // Check, whether the walls are set properly
            if ((  y == 0 
                || y == mapHeight - 1
                || (x % 2 == 0 && y % 2 == 0) 
                || x == 0
                || x == mapWidth - 1)
                && newMap[y][x] != WALL)
                throw invalid_argument("The map layout is incorrect"s);

            // Check, whether the walls aren't where they shouldn't be
            if ((x % 2 != 0 || y % 2 != 0)
              && y != 0
              && y != mapHeight - 1
              && x != 0
              && x != mapWidth - 1
              && newMap[y][x] == WALL)
                throw invalid_argument("The map layout is incorrect"s);
        }
    }
    // Finally load the map when everything is successful
    this->map = newMap;
}

void CMap::mapLoadError()
{
    using namespace std;

    throw invalid_argument
    ("The map you are trying to load has invalid dimensions. Required width: "s
    .append(to_string(mapWidth)).append(", height: ").append(to_string(mapHeight)));
}