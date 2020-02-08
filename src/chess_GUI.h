#ifndef CHESS_GUI_H
#define CHESS_GUI_H

#include "chess_pieces.h"
#include "computer.h"
#include <chrono>
#include <ctime>
#include <future>
#include <iostream>
#include <nanogui/nanogui.h>
#include <thread>
#include <vector>

using namespace nanogui;
using namespace std;

// Color of the window background (around the board)
const Color WIN_COLOR = Color(0x23, 0x27, 0x2A, 255);

enum UI_STATE { MAIN_MENU,
    SETTINGS,
    CREDITS,
    IN_GAME,
    GAMEOVER,
    PAUSED };
enum Player_Type { HUMAN,
    COMPUTER };

// Chess board widget
class chess_GUI : public Screen {
public:
    chess_GUI(int W);

    void draw(NVGcontext* ctx) override;

    bool keyboardEvent(int key, int scancode, int action, int modifiers) override;
    bool mouseButtonEvent(const Vector2i& p, int button, bool down, int modifiers) override;

private:
    UI_STATE ui_state = UI_STATE::MAIN_MENU;
    int board_size;
    int s_size;

    nanogui::ref<Window> pausedWindow;
    nanogui::ref<Window> gameSelectWindow;

    // Game logic
    Board board = Board::Default_Board();

    Player_Type WHITE;
    Player_Type BLACK;
    void StartGame(Player_Type white, Player_Type black);

    // Drawing
    Color buttonColor = Color(0x72, 0x89, 0xDA, 255);
    int buttonWidth = 300;
    int buttonHeight = 70;
    int distButtons = 50;
    int centerOffset = -40;
    vector<string> menuButtons = { "Play", "Settings", "Credits", "Quit" };
    void Add_Buttons(vector<string> buttonTexts);

    void DrawBoard(NVGcontext* ctx);
    void DrawMainMenu(NVGcontext* ctx);
    void DrawSettings(NVGcontext* ctx);
    void DrawCredits(NVGcontext* ctx);

    void AnimateMove(Move someMove);
    std::atomic_bool complete = false;
    std::atomic_bool calculating = false;
    std::atomic_bool computerMoveReady = false;

    std::future<Move> compMoveThread;

    Move targetAnimation = Move(0, 0, 8, 0);
    const double animationLengthInMilliSec = 200;
    uint64_t animationStart = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    // Input handling
    Vector2i click1 = { -1, -1 };
    void HandleInGame(const Vector2i& p);
    void HandleMainMenu(const Vector2i& p);
    void HandleSettings(const Vector2i& p);
    void HandleCredits(const Vector2i& p);
    bool ComputerTurn();
};

#endif // CHESS_GUI.H
