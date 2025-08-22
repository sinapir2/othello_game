#pragma once

#include <string>

class Player {
private:
    char color;
    int score;

public:
    explicit Player(char c);
    
    char getColor() const;
    int getScore() const;
    
    void setScore(int newScore);
    
    char getOpponentColor() const;
    std::string getColorName() const;
};
