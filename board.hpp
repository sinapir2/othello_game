#pragma once

#include <vector>
#include <string>
#include <utility>

class Board {
private:
    std::vector<std::vector<char>> grid;
    static const int BOARD_SIZE = 8;
    
    static const int directions[8][2];

public:
    Board();
    
    void display() const;
    void reset();
    char getPiece(int row, int col) const;
    bool isEmpty(int row, int col) const;
    bool isValidPosition(int row, int col) const;
    
    bool isValidMove(int row, int col, char player) const;
    std::vector<std::pair<int, int>> getValidMoves(char player) const;
    bool placePiece(int row, int col, char player);
    bool hasValidMoves(char player) const;
    
    std::pair<int, int> getScore() const; // Returns {black_count, white_count}
    
    std::string toString() const;
    void fromString(const std::string& data);
    
    static int colCharToIndex(char c);
    static char indexToColChar(int index);

private:
    std::vector<std::pair<int, int>> getFlippedPieces(int row, int col, char player) const;
    void flipPieces(const std::vector<std::pair<int, int>>& pieces);
    std::vector<std::pair<int, int>> checkDirection(int row, int col, int dr, int dc, char player) const;
};
