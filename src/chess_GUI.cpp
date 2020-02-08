#include "chess_gui.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <unistd.h>

chess_GUI::chess_GUI(int W)
    : nanogui::Screen(Eigen::Vector2i(W, W), "Skak", false)
{
    setBackground(WIN_COLOR);

    board_size = W;
    s_size = W / 10;

    auto* pausegui = new FormHelper(this);
    pausedWindow = pausegui->addWindow(Eigen::Vector2i(10, 10), "Paused");

    pausegui->addButton("Continue", [this]() {
        ui_state = UI_STATE::IN_GAME;
        pausedWindow->setVisible(false);
    });
    pausegui->addButton("Quit to Main Menu", [this]() {
        ui_state = UI_STATE::MAIN_MENU;
        pausedWindow->setVisible(false);
    });
    pausegui->addButton("Quit", [this]() { exit(1); });

    pausedWindow->center();
    pausedWindow->setFocused(true);
    pausedWindow->performLayout(nvgContext());
    pausedWindow->setVisible(false);

    auto* gameselectgui = new FormHelper(this);
    gameSelectWindow = gameselectgui->addWindow(Eigen::Vector2i(10, 10), "Choose game");

    // Start a new group
    gameselectgui->addButton("Player vs. Player", [this]() {
        StartGame(Player_Type::HUMAN, Player_Type::HUMAN);
    });
    gameselectgui->addButton("Player vs. Computer", [this]() {
        StartGame(Player_Type::HUMAN, Player_Type::COMPUTER);
    });
    gameselectgui->addButton("Computer vs. Player", [this]() {
        StartGame(Player_Type::COMPUTER, Player_Type::HUMAN);
    });
    gameselectgui->addButton("Computer vs. Computer", [this]() {
        StartGame(Player_Type::COMPUTER, Player_Type::COMPUTER);
        drawAll();

        compMoveThread = std::async(
            std::launch::async, [](Board board, std::atomic_bool& complete) {
                Move mv = Computer::GetMiniMaxMove(board);
                complete = true;
                return mv;
            },
            board, std::ref(complete));

        while (!board.IsCheckMate()) {
            auto animationProgress = (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - animationStart) / animationLengthInMilliSec;

            if (complete && animationProgress > 1) {
                complete = false;
                Move compMove = compMoveThread.get();
                board.Move_Piece(compMove);
                AnimateMove(compMove);

                compMoveThread = std::async(
                    std::launch::async, [](Board board, std::atomic_bool& complete) {
                        Move mv = Computer::GetMiniMaxMove(board);
                        complete = true;
                        return mv;
                    },
                    board, std::ref(complete));
            }

            glfwPollEvents();
            drawAll();
            if (ui_state != UI_STATE::IN_GAME) {
                if (ui_state != UI_STATE::PAUSED)
                    break;

                while (ui_state == UI_STATE::PAUSED) {
                    this_thread::sleep_for(10ms);
                    drawAll();
                    glfwPollEvents();
                }
            }

            this_thread::sleep_for(20ms);
        }

        if (board.IsCheckMate())
            pausedWindow->setVisible(true);
    });

    gameSelectWindow->center();
    gameSelectWindow->setFocused(true);
    gameSelectWindow->performLayout(nvgContext());
    gameSelectWindow->setVisible(false);
}

void chess_GUI::StartGame(Player_Type white, Player_Type black)
{
    board = Board(Board::Default_Board());
    board.board_size = board_size;
    board.s_size = s_size;

    WHITE = white;
    BLACK = black;

    ui_state = UI_STATE::IN_GAME;
    gameSelectWindow->setVisible(false);
}

void chess_GUI::draw(NVGcontext* ctx)
{
    switch (ui_state) {
    case UI_STATE::MAIN_MENU:
        DrawMainMenu(ctx);
        break;
    case UI_STATE::SETTINGS:
        DrawSettings(ctx);
        break;
    case UI_STATE::IN_GAME:
        DrawBoard(ctx);
        break;
    case UI_STATE::PAUSED:
        DrawBoard(ctx);
        if (!pausedWindow->visible())
            pausedWindow->setVisible(true);
        break;
    case UI_STATE::GAMEOVER:
        break;
    }

    Screen::draw(ctx);
}

float lengthOfVec(Vector2f vec)
{
    return sqrt(pow(vec.x(), 2) + pow(vec.y(), 2));
}

bool chess_GUI::ComputerTurn()
{
    if (WHITE == Player_Type::COMPUTER && BLACK == Player_Type::COMPUTER) {
        return false;
    }

    if ((board.currentPlayer == Chess_Color::White && WHITE == Player_Type::COMPUTER) || //
        (board.currentPlayer == Chess_Color::Black && BLACK == Player_Type::COMPUTER)) {
        return true;
    }

    return false;
}

void chess_GUI::DrawBoard(NVGcontext* ctx)
{
    // Draw numbers and letters outside board
    nvgBeginPath(nvgContext());
    nvgFontSize(nvgContext(), s_size * 0.5);
    nvgFontFace(nvgContext(), "sans");
    nvgFillColor(nvgContext(), nvgRGBA(200, 200, 200, 255)); // Text color
    nvgTextAlign(nvgContext(), NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

    char row[2] = { '8', '\0' };
    char col[2] = { 'A', '\0' };
    for (int r = 1; r < 9; r++) {
        nvgText(nvgContext(), s_size / 2, s_size * r + s_size / 2, row, nullptr);
        nvgText(nvgContext(), board_size - s_size / 2, s_size * r + s_size / 2, row,
            nullptr);
        nvgText(nvgContext(), s_size * r + s_size / 2, s_size / 2, col, nullptr);
        nvgText(nvgContext(), s_size * r + s_size / 2, board_size - s_size / 2, col,
            nullptr);

        row[0] = row[0] - 1;
        col[0] = col[0] + 1;
    }

    // Draw squares
    bool white = true;
    for (int r = 1; r < 9; r++) {
        for (int c = 1; c < 9; c++) {
            if (white) {
                nvgBeginPath(nvgContext());
                nvgRect(nvgContext(), s_size * c, s_size * r, s_size, s_size);
                nvgFillColor(nvgContext(), nvgRGBA(180, 180, 180, 255));
                nvgFill(nvgContext());
            } else {
                nvgBeginPath(nvgContext());
                nvgRect(nvgContext(), s_size * c, s_size * r, s_size, s_size);
                nvgFillColor(nvgContext(), nvgRGBA(120, 120, 120, 255));
                nvgFill(nvgContext());
            }
            white = !white;
        }
        white = !white;
    }

    // Draw pieces
    nvgBeginPath(nvgContext());
    nvgFontSize(nvgContext(), s_size);
    nvgFontFace(nvgContext(), "chess");
    nvgFillColor(nvgContext(), nvgRGBA(0, 0, 0, 255)); // Text color
    nvgTextAlign(nvgContext(), NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            // The animated piece shouldn't be drawn
            if (r == board.lastMove.r2 && c == board.lastMove.c2)
                continue;

            nvgText(nvgContext(), s_size * (c + 1.5), s_size * (r + 1.5),
                ChessPieceIcon(board.Get_Piece(r, c).chessType, board.Get_Piece(r, c).color), nullptr);
        }
    }

    // Draw animated piece
    // Calculate a number between 1 and one that determines how much of the animation has gone by
    auto animationProgress = (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - animationStart) / animationLengthInMilliSec;

    // We want the animation to stop at 100%
    if (animationProgress > 1)
        animationProgress = 1;

    Vector2f moveFromPos = { s_size * (board.lastMove.c1 + 1.5), s_size * (board.lastMove.r1 + 1.5) };
    Vector2f moveToPos = { s_size * (board.lastMove.c2 + 1.5), s_size * (board.lastMove.r2 + 1.5) };

    Vector2f targetPos = moveFromPos + ((moveToPos - moveFromPos) * animationProgress);

    nvgText(nvgContext(), targetPos.x(), targetPos.y(),
        ChessPieceIcon(board.Get_Piece(board.lastMove.r2, board.lastMove.c2).chessType, board.Get_Piece(board.lastMove.r2, board.lastMove.c2).color), nullptr);

    // After first click, mark it in upper left corner, and draw possible moves
    if (click1.x() >= 0 && board.Get_Piece(click1.x(), click1.y()).color == board.currentPlayer) {

        nvgBeginPath(nvgContext());
        nvgRect(nvgContext(), s_size * 0.25, s_size * 0.25, s_size * 0.5,
            s_size * 0.5);
        nvgFillColor(nvgContext(), Color(0, 255, 0, 255)); // Text color
        nvgFill(nvgContext());

        vector<Move> possibleMoves = board.ValidMoves(click1.x(), click1.y());

        for (auto& possibleMove : possibleMoves) {
            float r = s_size * 0.1;

            float x = s_size * (possibleMove.c2 + 1) + s_size * 0.5;
            float y = s_size * (possibleMove.r2 + 1) + s_size * 0.5;

            nvgBeginPath(nvgContext());
            nvgCircle(nvgContext(), x, y, r);
            nvgFillColor(nvgContext(), nvgRGBA(255, 0, 0, 255)); // Text color
            nvgFill(nvgContext());
        }
    } else {
        nvgBeginPath(nvgContext());
        nvgRect(nvgContext(), s_size * 0.25, s_size * 0.25, s_size * 0.5,
            s_size * 0.5);
        nvgFillColor(nvgContext(), WIN_COLOR); // Text color
        nvgFill(nvgContext());
    }

    // Draw and arrow showing the last move
    if (board.lastMove.r1 != -1) {
        nvgBeginPath(nvgContext());
        const Vector2f start = { s_size * (board.lastMove.c1 + 1.5f), s_size * (board.lastMove.r1 + 1.5f) };
        const Vector2f originend = targetPos;

        const Vector2f end = start + ((originend - start) * (1 - (20 / lengthOfVec(originend - start))));

        // Arrow body
        nvgMoveTo(nvgContext(), start.x(), start.y());
        nvgLineTo(nvgContext(), end.x(), end.y());

        Vector2f between = ((start - end) * 10 / lengthOfVec((start - end)));

        nvgStrokeWidth(nvgContext(), 5);
        nvgStrokeColor(nvgContext(), nvgRGBA(255, 255, 255, 255));
        nvgStroke(nvgContext());
        nvgStrokeWidth(nvgContext(), 1);

        nvgBeginPath(nvgContext());
        Vector2f rightarrow = { between.y(), -between.x() };
        rightarrow += between;
        rightarrow += end;

        Vector2f leftarrow = { -between.y(), between.x() };
        leftarrow += between;
        leftarrow += end;

        // Draw triangle
        nvgMoveTo(nvgContext(), end.x(), end.y());
        nvgLineTo(nvgContext(), rightarrow.x(), rightarrow.y());
        nvgLineTo(nvgContext(), leftarrow.x(), leftarrow.y());
        nvgLineTo(nvgContext(), end.x(), end.y());
        nvgFillColor(nvgContext(), nvgRGBA(50, 50, 50, 255));
        nvgFill(nvgContext());

        nvgStrokeWidth(nvgContext(), 5);
        nvgStrokeColor(nvgContext(), nvgRGBA(255, 255, 255, 255));
        nvgStroke(nvgContext());
        nvgStrokeWidth(nvgContext(), 1);
    }

    if (ComputerTurn()) {
        if (computerMoveReady) {
            Move computerMove = compMoveThread.get();
            board.Move_Piece(computerMove);
            AnimateMove(computerMove);
            computerMoveReady = false;

            if (board.IsCheck()) {
                cout << "Check" << endl;
            }
            if (board.IsCheckMate()) {
                cout << "Checkmate" << endl;
                pausedWindow->setVisible(true);
            }
        }
    }
}

void chess_GUI::Add_Buttons(vector<string> buttonTexts)
{
    auto totalButtonHeight = (distButtons * (buttonTexts.size() - 1) + buttonHeight * (buttonTexts.size() - 1));

    for (int i = 0; i < buttonTexts.size(); i++) {
        // i = 1 and offset = totalButtonHeight-buttonheight/2
        // i = 2 and offset =

        int offset = totalButtonHeight - distButtons * i - buttonHeight * i + centerOffset;
        int x = board_size / 2 - buttonWidth / 2;
        int y = board_size / 2 - buttonHeight / 2 - (offset - totalButtonHeight / 2);

        nvgBeginPath(nvgContext());
        nvgRect(nvgContext(), x, y, buttonWidth, buttonHeight);
        nvgFillColor(nvgContext(), buttonColor);
        nvgFill(nvgContext());

        nvgBeginPath(nvgContext());
        nvgFontSize(nvgContext(), 35.0f);
        nvgFontFace(nvgContext(), "sans");
        nvgFillColor(nvgContext(), nvgRGBA(255, 255, 255, 255));
        nvgTextAlign(nvgContext(), NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
        nvgText(nvgContext(), board_size / 2, y + buttonHeight / 2,
            buttonTexts.at(i).c_str(), nullptr);
    }
}

void chess_GUI::DrawMainMenu(NVGcontext* ctx)
{
    // Draw Title
    nvgBeginPath(ctx);
    nvgFontSize(ctx, 60.0f);
    nvgFontFace(ctx, "sans");
    nvgFillColor(ctx, nvgRGBA(255, 255, 255, 255));
    nvgTextAlign(ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
    nvgText(ctx, board_size / 2, board_size / 10, "Skak - Programmering", NULL);

    // Add Buttons: Play, Settings, Credits, Quit
    Add_Buttons(menuButtons);

    // Add names
    nvgBeginPath(ctx);
    nvgFontSize(ctx, 14.0f);
    nvgFontFace(ctx, "sans");
    nvgFillColor(ctx, nvgRGBA(255, 255, 255, 255));
    nvgTextAlign(ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
    nvgText(ctx, board_size / 2, board_size * 0.97f,
        "Made By: Mathias Gredal, Niklas Haim, Peter - 2.X", nullptr);
}

void chess_GUI::DrawSettings(NVGcontext* ctx)
{
    /*
  fl_rectf(0,0,board_size,board_size, WIN_COLOR);

  fl_color(FL_WHITE);

  fl_draw("Yeet", board_size/2, board_size/10);
  */
}

void chess_GUI::DrawCredits(NVGcontext* ctx) {}

void chess_GUI::AnimateMove(Move someMove)
{
    animationStart = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    targetAnimation = someMove;
}

bool chess_GUI::keyboardEvent(int key, int /*scancode*/, int action,
    int /*modifiers*/)
{
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        if (ui_state != UI_STATE::PAUSED)
            ui_state = UI_STATE::PAUSED;
        else
            ui_state = UI_STATE::IN_GAME;
    }
    cout << "you clicked " << key << endl;
}

bool chess_GUI::mouseButtonEvent(const Vector2i& p, int button, bool down,
    int modifiers)
{
    Screen::mouseButtonEvent(p, button, down, modifiers);

    // After 2 clicks move the piece
    if (down) {
        switch (ui_state) {
        case UI_STATE::MAIN_MENU:
            HandleMainMenu(p);
            break;
        case UI_STATE::SETTINGS:
            HandleSettings(p);
            break;
        case UI_STATE::IN_GAME:
            if (button == GLFW_MOUSE_BUTTON_1 && down)
                HandleInGame(p);
            break;
        case UI_STATE::PAUSED:
            break;
        case UI_STATE::GAMEOVER:
            break;
        }
    }

    return false;
}

void chess_GUI::HandleInGame(const Vector2i& p)
{
    // Get click coordinates
    Vector2i click = { p.y() / s_size - 1, p.x() / s_size - 1 };

    // Click not within chess board
    if (click.x() < 0 || click.x() >= 8 || click.y() < 0 || click.y() >= 8)
        return;

    if (click1.x() < 0 && board.Get_Piece(click.x(), click.y()).chessType != Chess_Type::EMPTY) {
        // This is the first click
        click1 = click;

        // Check if player selected opponents piece
        if (board.Get_Piece(click.x(), click.y()).color != board.currentPlayer)
            click1 = { -1, -1 };
    } else {
        Move tmp = Move(click1.x(), click1.y(), click.x(), click.y());

        if (board.ValidMove(tmp)) {
            board.Move_Piece(tmp);
            AnimateMove(tmp);

            if (board.IsCheck()) {
                cout << "Check" << endl;
            }
            if (board.IsCheckMate()) {
                cout << "Checkmate" << endl;
                pausedWindow->setVisible(true);
            }

            if (ComputerTurn() && not calculating) {
                compMoveThread = std::async(
                    std::launch::async, [](Board board, std::atomic_bool& calculating, std::atomic_bool& ready) {
                        calculating = true;
                        Move mv = Computer::GetMiniMaxMove(board);
                        calculating = false;
                        ready = true;
                        return mv;
                    },
                    board, std::ref(calculating), std::ref(computerMoveReady));
            }
        }
        drawAll();
        click1 = { -1, -1 };
    }
}

bool Click_Within_Range(vector<int> range, vector<int> inputclick)
{
    if (inputclick[0] >= range[0] && inputclick[0] <= range[2]) {
        if (inputclick[1] >= range[1] && inputclick[1] <= range[3]) {
            return true;
        }
    }

    return false;
}

void chess_GUI::HandleMainMenu(const Vector2i& p)
{
    std::vector<int> click = { p.x(), p.y() };

    std::vector<vector<int>> buttonRanges = {};

    auto totalButtonHeight = (distButtons * (menuButtons.size() - 1) + buttonHeight * (menuButtons.size() - 1));

    for (int i = 0; i < menuButtons.size(); i++) {
        int offset = totalButtonHeight - distButtons * i - buttonHeight * i + centerOffset;
        int x = board_size / 2 - buttonWidth / 2;
        int y = board_size / 2 - buttonHeight / 2 - (offset - totalButtonHeight / 2);

        buttonRanges.push_back({ x, y, x + buttonWidth, y + buttonHeight });
    }

    for (int i = 0; i < buttonRanges.size(); i++) {
        if (Click_Within_Range(buttonRanges[i], click)) {
            switch (i) {
            case 0: // Play
                // Ask player what type of game they want
                gameSelectWindow->setVisible(true);

                // ui_state = UI_STATE::IN_GAME;
                return;
            case 1: // Settings
                ui_state = UI_STATE::SETTINGS;
                return;
            case 2: // Credits
                return;
            case 3: // Quit
                exit(1);
                return;
            }
        }
    }
}

void chess_GUI::HandleSettings(const Vector2i& p) {}

void chess_GUI::HandleCredits(const Vector2i& p) {}
