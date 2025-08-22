#include "player.hpp"
#include <string>

using namespace std;

Player::Player(char c) : color(c), score(0) {
}

char Player::getColor() const {
    return color;
}

int Player::getScore() const {
    return score;
}

void Player::setScore(int newScore) {
    score = newScore;
}

char Player::getOpponentColor() const {
    return (color == 'B') ? 'W' : 'B';
}

string Player::getColorName() const {
    return (color == 'B') ? "Black" : "White";
}
