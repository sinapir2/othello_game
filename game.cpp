#include "game.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

Game::Game() : player1('B'), player2('W'), currentPlayer(&player1), 
               historyIndex(-1), gameEnded(false) {
}

void Game::newGame() {
    board.reset();
    currentPlayer = &player1;
    gameEnded = false;
    winner = "";
    gameHistory.clear();
    historyIndex = -1;
    
    updateScores();
    saveGameState();
    
    cout << "New game started\n";
    displayGameStatus();
}

void Game::run() {
    string command;
    
    while (true) {
        cout << "> ";
        getline(cin, command);
        
        // Trim 
        command.erase(0, command.find_first_not_of(" \t"));
        command.erase(command.find_last_not_of(" \t") + 1);
        
        if (command.empty()) {
            continue;
        }
        
        if (command == "exit") {
            cout << "Thanks for playing Othello!\n";
            break;
        }
        
        processCommand(command);
    }
}

bool Game::place(const string& position) {
    if (gameEnded) {
        cout << "Game has ended. Start a new game to continue playing.\n";
        return false;
    }
    
    auto [row, col] = parsePosition(position);
    if (row == -1 || col == -1) {
        cout << "Invalid position format. Use format like D4\n";
        return false;
    }
    
    if (!board.isValidMove(row, col, currentPlayer->getColor())) {
        cout << "Invalid move. You must flip at least one opponent piece.\n";
        return false;
    }
    
    processMove(row, col);
    return true;
}

void Game::undo() {
    if (historyIndex <= 0) {
        cout << "No moves to undo.\n";
        return;
    }
    
    historyIndex--;
    const GameState& prevState = gameHistory[historyIndex];
    
    board.fromString(prevState.boardState);
    currentPlayer = getPlayerByColor(prevState.currentPlayerColor);
    player1.setScore(prevState.blackScore);
    player2.setScore(prevState.whiteScore);
    gameEnded = false;
    winner = "";
    
    cout << "Move undone.\n";
    displayGameStatus();
}

void Game::redo() {
    if (historyIndex >= (int)gameHistory.size() - 1) {
        cout << "No moves to redo.\n";
        return;
    }
    
    historyIndex++;
    const GameState& nextState = gameHistory[historyIndex];
    
    board.fromString(nextState.boardState);
    currentPlayer = getPlayerByColor(nextState.currentPlayerColor);
    player1.setScore(nextState.blackScore);
    player2.setScore(nextState.whiteScore);
    
    checkGameEnd();
    
    cout << "Move redone.\n";
    displayGameStatus();
}

void Game::save(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open file for saving.\n";
        return;
    }
    
    file << board.toString();
    
    file << "Turn: " << currentPlayer->getColor() << "\n";
    file << "Score: " << player1.getScore() << " " << player2.getScore() << "\n";
    file << "GameEnded: " << (gameEnded ? "1" : "0") << "\n";
    if (gameEnded) {
        file << "Winner: " << winner << "\n";
    }
    
    file.close();
    cout << "Game saved to " << filename << "\n";
}

void Game::load(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open file " << filename << "\n";
        return;
    }
    
    string boardData;
    string line;
    for (int i = 0; i < 8; i++) {
        if (getline(file, line)) {
            boardData += line + "\n";
        }
    }
    
    string turnLine, scoreLine, gameEndedLine, winnerLine;
    getline(file, turnLine);
    getline(file, scoreLine);
    getline(file, gameEndedLine);
    
    file.close();
    
    board.fromString(boardData);
    
    if (turnLine.find("Turn: B") != string::npos) {
        currentPlayer = &player1;
    } else {
        currentPlayer = &player2;
    }
    
    istringstream scoreStream(scoreLine.substr(7)); // Skip "Score: "
    int blackScore, whiteScore;
    scoreStream >> blackScore >> whiteScore;
    player1.setScore(blackScore);
    player2.setScore(whiteScore);
    
    gameEnded = (gameEndedLine.find("1") != string::npos);
    
    gameHistory.clear();
    historyIndex = -1;
    saveGameState();
    
    cout << "Game loaded from " << filename << "\n";
    displayGameStatus();
}

void Game::autoSave() const {
    ofstream file("autosave.oth");
    if (file.is_open()) {
        file << board.toString();
        file << "Turn: " << currentPlayer->getColor() << "\n";
        file << "Score: " << player1.getScore() << " " << player2.getScore() << "\n";
        file << "GameEnded: " << (gameEnded ? "1" : "0") << "\n";
        if (gameEnded) {
            file << "Winner: " << winner << "\n";
        }
        file.close();
    }
}

void Game::displayBoard() const {
    board.display();
}

void Game::displayScore() const {
    const auto [blackScore, whiteScore] = board.getScore();
    cout << "Score - B: " << blackScore << " | W: " << whiteScore << "\n";
}

void Game::displayGameStatus() const {
    displayBoard();
    
    if (gameEnded) {
        displayScore();
        if (winner.empty()) {
            cout << "Game was equal.\n";
        } else {
            cout << "Winner: " << winner << "\n";
        }
    } else {
        cout << "Player Turn: " << currentPlayer->getColor() << "\n";
        displayScore();
        
    }
}

pair<int, int> Game::parsePosition(const string& position) {
    if (position.length() != 2) {
        return {-1, -1};
    }
    
    const int col = Board::colCharToIndex(position[0]);
    const int row = position[1] - '1';
    
    if (col < 0 || col > 7 || row < 0 || row > 7) {
        return {-1, -1};
    }
    
    return {row, col};
}

bool Game::isValidCommand(const string& command) const {
    if (command == "new" || command == "undo" || command == "redo" || 
        command == "help" || command == "exit") {
        return true;
    }
    
    if (command.substr(0, 5) == "place" && command.length() > 6) {
        return true;
    }
    
    if (command.substr(0, 4) == "save" && command.length() > 5) {
        return true;
    }
    
    if (command.substr(0, 4) == "load" && command.length() > 5) {
        return true;
    }
    
    return false;
}

void Game::switchTurn() {
    currentPlayer = (currentPlayer == &player1) ? &player2 : &player1;
}

void Game::updateScores() {
    const auto [blackScore, whiteScore] = board.getScore();
    player1.setScore(blackScore);
    player2.setScore(whiteScore);
}

void Game::saveGameState() {
    if (historyIndex < (int)gameHistory.size() - 1) {
        gameHistory.erase(gameHistory.begin() + historyIndex + 1, gameHistory.end());
    }
    
    updateScores();
    gameHistory.emplace_back(board.toString(), currentPlayer->getColor(), 
                           player1.getScore(), player2.getScore());
    historyIndex++;
}

void Game::checkGameEnd() {
    bool currentHasMoves = board.hasValidMoves(currentPlayer->getColor());
    bool opponentHasMoves = board.hasValidMoves(currentPlayer->getOpponentColor());
    
    if (!currentHasMoves && !opponentHasMoves) {
        gameEnded = true;
        updateScores();
        
        if (player1.getScore() > player2.getScore()) {
            winner = player1.getColorName();
        } else if (player2.getScore() > player1.getScore()) {
            winner = player2.getColorName();
        } else {
            winner = ""; // Tie
        }
    }
}

Player* Game::getPlayerByColor(char color) {
    return (color == 'B') ? &player1 : &player2;
}

bool Game::handleTurnSkip() {
    if (!board.hasValidMoves(currentPlayer->getColor())) {
        cout << "Player " << currentPlayer->getColor() << " has no valid moves. Turn skipped.\n";
        switchTurn();
        return true;
    }
    return false;
}

void Game::processMove(int row, int col) {
    if (board.placePiece(row, col, currentPlayer->getColor())) {
        char colChar = Board::indexToColChar(col);
        cout << "Piece placed at " << colChar << (row + 1) << "\n";
        
        switchTurn();
        
        if (handleTurnSkip()) {
            handleTurnSkip();
        }
        
        checkGameEnd();
        saveGameState();
        displayGameStatus();
    }
}

void Game::processCommand(const string& command) {
    if (command == "new") {
        newGame();
    } else if (command.substr(0, 5) == "place") {
        if (command.length() > 6) {
            place(command.substr(6));
        } else {
            cout << "Usage: place <position> (e.g., place D4)\n";
        }
    } else if (command == "undo") {
        undo();
    } else if (command == "redo") {
        redo();
    } else if (command.substr(0, 4) == "save") {
        if (command.length() > 5) {
            save(command.substr(5));
        } else {
            cout << "Usage: save <filename> (e.g., save game.oth)\n";
        }
    } else if (command.substr(0, 4) == "load") {
        if (command.length() > 5) {
            load(command.substr(5));
        } else {
            cout << "Usage: load <filename> (e.g., load game.oth)\n";
        }
    } else if (command == "help") {
        showHelp();
    } else {
        cout << "Unknown command. Type 'help' for available commands.\n";
    }
}

void Game::showHelp() const {
    cout << "\nAvailable commands:\n";
    cout << "  new                 - Start a new game\n";
    cout << "  place <position>    - Place a piece (e.g., place D4)\n";
    cout << "  undo                - Undo the last move\n";
    cout << "  redo                - Redo an undone move\n";
    cout << "  save <filename>     - Save game to file\n";
    cout << "  load <filename>     - Load game from file\n";
    cout << "  help                - Show this help message\n";
    cout << "  exit                - Exit the game\n\n";
    cout << "Position format: Column (A-H) + Row (1-8), e.g., D4, E3\n";
}
