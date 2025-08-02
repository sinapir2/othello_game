#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class Board {
private:
    vector<vector<char>> grid;

public:
    Board() {
        grid = vector<vector<char>>(8, vector<char>(8, '.'));

        // put 4 pieces in the middle
        grid[3][3] = 'W';
        grid[3][4] = 'B';
        grid[4][3] = 'B';
        grid[4][4] = 'W';
    }

    void display() {
        cout << "  A B C D E F G H" << endl;
        for (int i = 0; i < 8; i++) {
            cout << i + 1 << " ";
            for (int j = 0; j < 8; j++) {
                cout << grid[i][j] << " ";
            }
            cout << endl;
        }
    }

    void reset() {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                grid[i][j] = '.';

        grid[3][3] = 'W';
        grid[3][4] = 'B';
        grid[4][3] = 'B';
        grid[4][4] = 'W';
    }

    bool isEmpty(int row, int col) {
        if (row < 0 || row >= 8 || col < 0 || col >= 8)
            return false;
        return grid[row][col] == '.';
    }

    bool placePiece(int row, int col, char piece) {
        if (!isEmpty(row, col))
            return false;
        grid[row][col] = piece;
        return true;
    }

    string toString() {
        string s = "";
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++)
                s += grid[i][j];
            s += "\n";
        }
        return s;
    }

    void fromString(const string& data) {
        int k = 0;
        for (int i = 0; i < 8 && k < data.size(); i++) {
            for (int j = 0; j < 8 && k < data.size(); j++) {
                if (data[k] != '\n')
                    grid[i][j] = data[k++];
            }
            if (k < data.size() && data[k] == '\n')
                k++;
        }
    }
};

class Player {
public:
    char color;
    Player(char c) {
        color = c;
    }
    char getColor() {
        return color;
    }
};

class Game {
private:
    Board board;
    Player player1{'B'};
    Player player2{'W'};
    Player* currentPlayer;

public:
    Game() {
        currentPlayer = &player1; // B starts
    }

    void newGame() {
        board.reset();
        currentPlayer = &player1;
        cout << "New game started\n";
        board.display();
        cout << "Player Turn: " << currentPlayer->getColor() << endl;
    }

    void switchTurn() {
        if (currentPlayer == &player1)
            currentPlayer = &player2;
        else
            currentPlayer = &player1;
    }

    int colCharToIndex(char c) {
        if (c >= 'A' && c <= 'H') return c - 'A';
        if (c >= 'a' && c <= 'h') return c - 'a';
        return -1;
    }

    void place(string pos) {
        if (pos.length() != 2) {
            cout << "Wrong format. Use like D4\n";
            return;
        }

        int col = colCharToIndex(pos[0]);
        int row = pos[1] - '1';

        if (col < 0 || col > 7 || row < 0 || row > 7) {
            cout << "Invalid position\n";
            return;
        }

        if (!board.isEmpty(row, col)) {
            cout << "This cell is not empty\n";
            return;
        }

        if (board.placePiece(row, col, currentPlayer->getColor())) {
            cout << "Piece placed at " << pos << endl;
            switchTurn();
            board.display();
            cout << "Player Turn: " << currentPlayer->getColor() << endl;
        } else {
            cout << "Can't place here\n";
        }
    }

    void save(string filename) {
        ofstream fout(filename);
        if (!fout.is_open()) {
            cout << "Can't open file to save\n";
            return;
        }

        fout << board.toString();
        fout << "Turn: " << currentPlayer->getColor() << endl;
        fout.close();
        cout << "Game saved\n";
    }

    void load(string filename) {
        ifstream fin(filename);
        if (!fin.is_open()) {
            cout << "File not found\n";
            return;
        }

        string line, data = "";
        for (int i = 0; i < 8; i++) {
            getline(fin, line);
            data += line + "\n";
        }

        getline(fin, line);
        if (line.find("Turn: B") != string::npos)
            currentPlayer = &player1;
        else
            currentPlayer = &player2;

        board.fromString(data);
        fin.close();

        cout << "Game loaded\n";
        board.display();
        cout << "Player Turn: " << currentPlayer->getColor() << endl;
    }

    void run() {
        string cmd;

        while (true) {
            cout << "> ";
            getline(cin, cmd);

            if (cmd == "new") {
                newGame();
            } else if (cmd.substr(0, 5) == "place") {
                if (cmd.length() > 6)
                    place(cmd.substr(6));
                else
                    cout << "Use like: place D4\n";
            } else if (cmd.substr(0, 4) == "save") {
                if (cmd.length() > 5)
                    save(cmd.substr(5));
                else
                    cout << "Use like: save game.oth\n";
            } else if (cmd.substr(0, 4) == "load") {
                if (cmd.length() > 5)
                    load(cmd.substr(5));
                else
                    cout << "Use like: load game.oth\n";
            } else if (cmd == "exit") {
                cout << "Bye!\n";
                break;
            } else {
                cout << "Command not recognized\n";
            }
        }
    }
};

int main() {
    Game g;
    g.newGame();
    g.run();
    return 0;
}
