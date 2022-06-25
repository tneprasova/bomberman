#include "CGame.hpp"

CGame::CGame(const char * title)
: startGame(false),
  mode(SINGLEPLAYER),
  window(nullptr),
  map(nullptr),
  manager(nullptr),
  UI(nullptr)
{
    using std::cout, std::endl, std::runtime_error, std::invalid_argument;
    try
    {
        initSDL();
        checkConfig();
        this->window = new CRenderWindow(title, screenWidth, screenHeight);
        newMap();
        this->gameOn = true;
    }
    // Deal with potential errors, cancel the game
    catch (const FileException & err)
    {
        cout << "\033[1;31mGAME COULD NOT START DUE TO AN ERROR IN READING CONFIGURATION FILE:\033[0m" << endl;
        cout << err.what() << endl;
        this->gameOn = false;        
    }
    catch (const runtime_error & err)
    {
        cout << "\033[1;31mGAME COULD NOT START DUE TO ISSUES IN INITIALIZATION:\033[0m" << endl;
        cout << err.what() << endl;
        this->gameOn = false;        
    }
    catch (const invalid_argument & err)
    {
        cout << "\033[1;31mGAME COULD NOT START DUE TO INVALID CONFIGURATION FILE:\033[0m" << endl;
        cout << err.what() << endl;
        this->gameOn = false;        
    }
}

CGame::~CGame()
{
    delete this->window;
    
    // Properly close all subsystems and libraries for SDL
    TTF_Quit();
    IMG_Quit();
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_Quit();
}

void CGame::run()
{
    using std::cout, std::endl, std::runtime_error;

    // Cancel the game, if it couldn't get started
    if (! this->gameOn)
        return;

    // Initialize needed calsses
    try { this->window->startRender(); }
    catch (const runtime_error & err)
    {
        cout << "\033[1;31mGAME COULD NOT START DUE TO ISSUES IN LOADING TEXTURES:\033[0m" << endl;
        cout << err.what() << endl;
        return;
    }
    this->UI.reset(new CUserInterface(this->window));
    this->manager.reset(new CObjectEventManager(this->window));

    // Start the game clock
    Uint32 frameStart = 0;
    int frameTime = 0;

    // The game loop
    while(this->isRunning())
    {
        frameStart = SDL_GetTicks();
        this->handleEvents();

        if (this->manager->needsNewMap)
        {
            this->newMap();
            this->manager->loadFromMap(this->map->getMap().first);
        }
        // The game has ended, show the UI
        if (this->manager->endGame)
        {
            this->startGame = false;
            this->UI->show();
        }

        this->window->clear();
        this->UI->render();
        if (this->startGame)
        {
            this->manager->update();
            this->manager->manageEvents();
        }
        this->window->display();

        // Delay the game as much as needed to maintain the same lenght of the frames
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
    }
}

void CGame::handleEvents()
{
    using std::cout, std::endl, std::invalid_argument;

    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        // Close the game window and end the program
        if (event.type == SDL_QUIT)
            this->gameOn = false;
        
        const uint8_t * currentKeyStates = SDL_GetKeyboardState(nullptr);

        // Quicksave in singleplayer mode
        if (currentKeyStates[SDL_SCANCODE_F5] && this->mode == SINGLEPLAYER && this->startGame)
        {
            this->map->setMap(this->manager->saveIntoMap());
            this->map->save();
        }
        // UI events - signals which button was pressed (if any)
        switch (this->UI->handleEvents(&event))
        {
        case UI_NEW_GAME:
            this->mode = SINGLEPLAYER;
            newGame();
            break;

        case UI_LOAD:
            this->startGame = true;
            try
            {
                this->map->load();
                this->manager->startGame(this->map->getMap());
            }
            // In case the game could not be loaded from the file, write an error message
            // into the terminal and don't do anything
            catch (const FileException & err)
            {
                cout << "\033[1;31mGAME COULD NOT BE LOADED FROM A FILE:\033[0m" << endl;
                cout << err.what() << endl;
                this->startGame = false;
                this->UI->show();
            }
            catch (const invalid_argument & err)
            {
                cout << "\033[1;31mLOAD FILE IS CORRUPTED:\033[0m" << endl;
                cout << err.what() << endl;
                this->startGame = false;
                this->UI->show();
            }
            break;

        case UI_DUEL:
            this->mode = DUEL;
            newGame();
            break;

        case UI_EXIT:
            this->gameOn = false;
            break;
        
        default:
            break;
        }
        // Jump to UI (discarding the current game)
        if (currentKeyStates[SDL_SCANCODE_ESCAPE])
        {
            this->startGame = false;
            this->UI->show();
        }
    }
}

bool CGame::isRunning() const { return this->gameOn; }

void CGame::newMap() { this->map.reset(new CMap(this->mode)); }

void CGame::newGame()
{
    this->startGame = true;
    this->newMap();
    this->manager->startGame(this->map->getMap());
}

void CGame::initSDL()
{
    using namespace std;

    if (SDL_Init(SDL_INIT_VIDEO) > 0 || ! IMG_Init(IMG_INIT_PNG) || TTF_Init() == -1)
        throw runtime_error("SDL initialization error: "s.append(SDL_GetError()));
}

void CGame::checkConfig()
{
    using namespace std;

    if (tileWidth < 64 || tileWidth > 256 || tileWidth % 32 != 0)
        throw invalid_argument("Invalid tile width. Minimum: 64, maximum: 256. Must be a multiple of 32.\nYours: "s
        .append(to_string(tileWidth)));

    if (FPS < 60)
        throw invalid_argument("Invalid FPS. Minimum: 60. Game would be lagging otherwise.");

    if (screenWidth < 5 * tileWidth || screenHeight < 5 * tileWidth 
     || screenWidth > 2240 / tileWidth * tileWidth || screenHeight > 1440 / tileWidth * tileWidth)
        throw invalid_argument("Invalid screen dimensions. For specified tile width "s.append(to_string(tileWidth))
        .append("\n\nThe minimum is:\nScreen width: ").append(to_string(5 * tileWidth))
        .append("\nScreen height: ").append(to_string(5 * tileWidth))
        .append("\n\nThe maximum is:\nScreen width: ").append(to_string(2240 / tileWidth * tileWidth))
        .append("\nScreen height: ").append(to_string(1440 / tileWidth * tileWidth)));

    if (loadData(config,"High score") < 0)
        throw invalid_argument("Negative high score");

    loadData(config,"Save file", true);

    int tmp = loadData(config,"Enemies");
    if (tmp < 0 || tmp > 10)
        throw invalid_argument("Invalid number of enemies. Minimum: 0, maximum: 10");

    tmp = loadData(config,"Breakables");
    if (tmp < 0 || tmp > 100)
        throw invalid_argument("Invalid number of breakables. Minimum: 0, maximum: 100");
    
    tmp = loadData(config,"Duel rounds");
    if (tmp < 1 || tmp > 10)
        throw invalid_argument("Invalid number of duel rounds. Minimum: 1, maximum: 10");

    tmp = loadData(config,"Bonus chance");
    if (tmp < 0 || tmp > 100)
        throw invalid_argument("Invalid bonus chance in percent. Minimum: 0, maximum: 100");

    tmp = loadData(config,"Bonus mega bombs");
    if (tmp < 1)
        throw invalid_argument("Invalid mega bombs bonus size. Minimum: 0");

    tmp = loadData(config,"Bonus speed");
    if (tmp < 0)
        throw invalid_argument("Invalid speed bonus size. Minimum: 0");
}