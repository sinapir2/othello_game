#include <iostream>
#include "game.hpp"

using namespace std;

int main() {
    cout << "=== Welcome to Othello/Reversi ===" << endl;
    cout << "Type 'help' for available commands" << endl;
    cout << endl;
    
    Game game;
    game.newGame();
    game.run();
    
    return 0;
}
