#include <cassert>
#include "CGame.hpp"

using namespace std;

int main(int argc, char * args[])
{
    // Test config loading
    assert(string_view(config) == "./examples/config-test.txt");
    assert(loadData(config, "High score") == 600);
    assert(loadData(config, "Save file", true).str() == "./examples/map4.txt"s);
    assert(loadData(config, "Enemies") == 5);
    assert(loadData(config, "Breakables") == 100);
    assert(loadData(config, "Duel rounds") == 5);
    assert(loadData(config, "Bonus chance") == 100);
    assert(loadData(config, "Bonus mega bombs") == 1);
    assert(loadData(config, "Bonus speed") == 2);
    assert(loadData(config, "   \nBoNus SPEED         \n   ") == 2);
    try 
    {
        loadData(config, "Some nonexistent label");
        assert(false);
    }
    catch (const FileException & err)
    {
        assert(string_view(err.what()) == "The item Some nonexistent label was not found");
    }
    try
    {
        loadData("some/nonexistent/file", "With a nonexistent label");
        assert(false);
    }
    catch(const FileException & err)
    {
        assert(string_view(err.what()) == "Failed to open the file");
    }
    try 
    {
        loadData(config, "Error label");
        assert(false);
    }
    catch (const FileException & err)
    {
        assert(string_view(err.what()) == "No data found under label Error label");
    }
    try 
    {
        loadData(config, "Another error label");
        assert(false);
    }
    catch (const FileException & err)
    {
        assert(string_view(err.what()) == "No data found under label Another error label");
    }
    saveData(config, "Another error label", stringstream("Fix the error"));
    assert(loadData(config, "Another error label", true).str() == "Fix the error"s);
    saveData(config, "Another error label", stringstream(""));

    CMap map1(SINGLEPLAYER, "./examples/map4.txt");
    map1.load(); // Should load with no error

    CMap map2(SINGLEPLAYER, "./examples/map5.txt");
    try
    {
        map2.load();
        assert(false);
    }
    catch(const invalid_argument & err)
    {
        assert(string_view(err.what()) == "The map you are trying to load has invalid dimensions. Required width: 23, height: 15");
    }

    CMap map3(SINGLEPLAYER, "./examples/map6.txt");
    try
    {
        map3.load();
        assert(false);
    }
    catch(const invalid_argument & err)
    {
        assert(string_view(err.what()) == "Unknown tile type loaded");
    }

    CMap map4(SINGLEPLAYER, "./examples/map7.txt");
    try
    {
        map4.load();
        assert(false);
    }
    catch(const invalid_argument & err)
    {
        assert(string_view(err.what()) == "The map layout is incorrect");
    }
    
    stringstream stream = loadData("./examples/map7.txt", "Map", true);
    map4.save();
    try
    {
        map4.load();
        assert(true);
    }
    catch(const invalid_argument & err)
    {
        assert(false);
    }
    saveData("./examples/map7.txt", "Map", stream);

    CMap map5(SINGLEPLAYER, "./examples/map8.txt");
    try
    {
        map5.load();
        assert(false);
    }
    catch(const invalid_argument & err)
    {
        assert(string_view(err.what()) == "The map you are trying to load has invalid dimensions. Required width: 23, height: 15");
    }

    CMap map6(SINGLEPLAYER, "./examples/map9.txt");
    try
    {
        map6.save();
        assert(true);
    }
    catch(const invalid_argument & err)
    {
        assert(false);
    }
    remove("./examples/map9.txt");

    CMap map7(SINGLEPLAYER);
    auto tmp1 = map7.getMap().first;

    int players1 = 0;
    int breakables1 = 0;
    int enemies1 = 0;

    for (auto & row : tmp1)
    {
        for (auto & tile : row)
        {
            if (tile == PLAYER1 || tile == PLAYER2)
                ++ players1;
            if (tile == BREAKABLE)
                ++ breakables1;
            if (tile == ENEMY)
                ++ enemies1;
        }
    }
    assert(players1 == 1);
    assert(loadData(config, "Breakables") == breakables1);
    assert(loadData(config, "Enemies") == enemies1);

    CMap map8(DUEL);
    auto tmp2 = map8.getMap().first;

    int players2 = 0;
    int breakables2 = 0;
    int enemies2 = 0;

    for (auto & row : tmp2)
    {
        for (auto & tile : row)
        {
            if (tile == PLAYER1 || tile == PLAYER2)
                ++ players2;
            if (tile == BREAKABLE)
                ++ breakables2;
            if (tile == ENEMY)
                ++ enemies2;
        }
    }
    assert(players2 == 2);
    assert(loadData(config, "Breakables") == breakables2);
    assert(enemies2 == 0);

    cout << "\033[1;32mTESTING SUCCESSFUL\033[0m" << endl;
    return EXIT_SUCCESS;
}