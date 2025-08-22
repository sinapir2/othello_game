#pragma once

#include "board.hpp"
#include "player.hpp"
#include <vector>
#include <string>

struct GameState {
    std::string boardState;
    char currentPlayerColor;
    int blackScore;
    int whiteScore;
    
    GameState(const std::string& board, char player, int bScore, int wScore)
        : boardState(board), currentPlayerColor(player), blackScore(bScore), whiteScore(wScore) {}
};

class Game {
private:
    Board board;
    Player player1;
    Player player2;
    Player* currentPlayer;
    
    std::vector<GameState> gameHistory;
    int historyIndex;
    
    bool gameEnded;
    std::string winner;

public:
    Game();
    
    void newGame();
    void run();
    
    bool place(const std::string& position);
    void undo();
    void redo();
    
    void save(const std::string& filename) const;
    void load(const std::string& filename);
    void autoSave() const;
    
    void displayBoard() const;
    void displayScore() const;
    void displayGameStatus() const;
    
    static std::pair<int, int> parsePosition(const std::string& position);
    bool isValidCommand(const std::string& command) const;

private:
    void switchTurn();
    void updateScores();
    void saveGameState();
    void checkGameEnd();
    Player* getPlayerByColor(char color);
    
    bool handleTurnSkip();
    void processMove(int row, int col);
    
    void processCommand(const std::string& command);
    void showHelp() const;
};
