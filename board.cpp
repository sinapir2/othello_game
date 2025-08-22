#include "board.hpp"
#include <iostream>
#include <sstream>

using namespace std;

// 8 directions
const int Board::directions[8][2] = {
    {-1, 0}, {-1, 1}, {0, 1}, {1, 1},
    {1, 0}, {1, -1}, {0, -1}, {-1, -1}
};

Board::Board() {
    reset();
}

void Board::reset() {
    grid = vector<vector<char>>(BOARD_SIZE, vector<char>(BOARD_SIZE, '.'));
    
    // init pos
    grid[3][3] = 'W';
    grid[3][4] = 'B';
    grid[4][3] = 'B';
    grid[4][4] = 'W';
}

void Board::display() const {
    cout << "  A B C D E F G H" << endl;
    for (int i = 0; i < BOARD_SIZE; i++) {
        cout << i + 1 << " ";
        for (int j = 0; j < BOARD_SIZE; j++) {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }
}

char Board::getPiece(int row, int col) const {
    if (!isValidPosition(row, col)) {
        return '\0';
    }
    return grid[row][col];
}

bool Board::isEmpty(int row, int col) const {
    return isValidPosition(row, col) && grid[row][col] == '.';
}

bool Board::isValidPosition(int row, int col) const {
    return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}

bool Board::isValidMove(int row, int col, char player) const {
    if (!isEmpty(row, col)) {
        return false;
    }
    
    return !getFlippedPieces(row, col, player).empty();
}

vector<pair<int, int>> Board::getValidMoves(char player) const {
    vector<pair<int, int>> validMoves;
    
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (isValidMove(row, col, player)) {
                validMoves.push_back({row, col});
            }
        }
    }
    
    return validMoves;
}

bool Board::placePiece(int row, int col, char player) {
    if (!isValidMove(row, col, player)) {
        return false;
    }
    
    vector<pair<int, int>> toFlip = getFlippedPieces(row, col, player);
    
    grid[row][col] = player;
    
    flipPieces(toFlip);
    
    return true;
}

bool Board::hasValidMoves(char player) const {
    return !getValidMoves(player).empty();
}

pair<int, int> Board::getScore() const {
    int blackCount = 0, whiteCount = 0;
    
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (grid[row][col] == 'B') {
                blackCount++;
            } else if (grid[row][col] == 'W') {
                whiteCount++;
            }
        }
    }
    
    return {blackCount, whiteCount};
}

string Board::toString() const {
    string result;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            result += grid[i][j];
        }
        result += "\n";
    }
    return result;
}

void Board::fromString(const string& data) {
    istringstream stream(data);
    string line;
    int row = 0;
    
    while (getline(stream, line) && row < BOARD_SIZE) {
        for (int col = 0; col < BOARD_SIZE && col < (int)line.length(); col++) {
            grid[row][col] = line[col];
        }
        row++;
    }
}

int Board::colCharToIndex(char c) {
    if (c >= 'A' && c <= 'H') return c - 'A';
    if (c >= 'a' && c <= 'h') return c - 'a';
    return -1;
}

char Board::indexToColChar(int index) {
    if (index >= 0 && index < 8) {
        return 'A' + index;
    }
    return '\0';
}

vector<pair<int, int>> Board::getFlippedPieces(int row, int col, char player) const {
    vector<pair<int, int>> flippedPieces;
    
    // check directions
    for (int d = 0; d < 8; d++) {
        vector<pair<int, int>> directionFlips = 
            checkDirection(row, col, directions[d][0], directions[d][1], player);
        
        flippedPieces.insert(flippedPieces.end(), 
                           directionFlips.begin(), directionFlips.end());
    }
    
    return flippedPieces;
}

void Board::flipPieces(const vector<pair<int, int>>& pieces) {
    for (const auto& piece : pieces) {
        int row = piece.first;
        int col = piece.second;
        
        if (grid[row][col] == 'B') {
            grid[row][col] = 'W';
        } else if (grid[row][col] == 'W') {
            grid[row][col] = 'B';
        }
    }
}

vector<pair<int, int>> Board::checkDirection(int row, int col, int dr, int dc, char player) const {
    vector<pair<int, int>> pieces;
    char opponent = (player == 'B') ? 'W' : 'B';
    
    int r = row + dr;
    int c = col + dc;
    
    while (isValidPosition(r, c) && grid[r][c] == opponent) {
        pieces.push_back({r, c});
        r += dr;
        c += dc;
    }
    
    if (isValidPosition(r, c) && grid[r][c] == player && !pieces.empty()) {
        return pieces;
    }
    
    return {};
