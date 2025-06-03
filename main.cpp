#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <fstream>

const int SIZE = 4;
int grid[SIZE][SIZE];
int score = 0;
int highscore = 0;

void loadHighScore() {
    std::ifstream in("highscore.txt");
    if (in.is_open()) {
        in >> highscore;
    }
}

void saveHighScore() {
    if (score > highscore) {
        std::ofstream out("highscore.txt");
        if (out.is_open()) {
            out << score;
        }
    }
}

void printGrid() {
    system("cls"); 
    std::cout << "Score: " << score << "  High Score: " << highscore << "\n";
    std::cout << "+------------------------+\n";
    for (int i = 0; i < SIZE; ++i) {
        std::cout << "|";
        for (int j = 0; j < SIZE; ++j) {
            if (grid[i][j] == 0)
                std::cout << std::setw(5) << "." << " ";
            else
                std::cout << std::setw(5) << grid[i][j] << " ";
        }
        std::cout << "|\n";
    }
    std::cout << "+------------------------+\n";
    std::cout << "Controls: W=Up, A=Left, S=Down, D=Right, Q=Quit\n";
}

bool canMove() {
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j) {
            if (grid[i][j] == 0) return true;
            if (j < SIZE - 1 && grid[i][j] == grid[i][j + 1]) return true;
            if (i < SIZE - 1 && grid[i][j] == grid[i + 1][j]) return true;
        }
    return false;
}

void addRandomTile() {
    std::vector<std::pair<int, int>> empty;
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            if (grid[i][j] == 0)
                empty.emplace_back(i, j);
    if (!empty.empty()) {
        auto [i, j] = empty[rand() % empty.size()];
        grid[i][j] = (rand() % 10 == 0) ? 4 : 2;
    }
}

bool slideAndMergeRow(std::vector<int>& row) {
    bool moved = false;
    std::vector<int> newRow(SIZE, 0);
    int idx = 0;


    for (int i = 0; i < SIZE; ++i)
        if (row[i] != 0)
            newRow[idx++] = row[i];

    for (int i = 0; i < SIZE - 1; ++i) {
        if (newRow[i] != 0 && newRow[i] == newRow[i + 1]) {
            newRow[i] *= 2;
            score += newRow[i];
            newRow[i + 1] = 0;
            moved = true;
        }
    }

    std::vector<int> finalRow(SIZE, 0);
    idx = 0;
    for (int i = 0; i < SIZE; ++i)
        if (newRow[i] != 0)
            finalRow[idx++] = newRow[i];

    if (row != finalRow) {
        row = finalRow;
        moved = true;
    }

    return moved;
}

bool move(char direction) {
    bool moved = false;

    switch (direction) {
        case 'w':
            for (int j = 0; j < SIZE; ++j) {
                std::vector<int> col(SIZE);
                for (int i = 0; i < SIZE; ++i)
                    col[i] = grid[i][j];
                if (slideAndMergeRow(col)) {
                    for (int i = 0; i < SIZE; ++i)
                        grid[i][j] = col[i];
                    moved = true;
                }
            }
            break;
        case 's':
            for (int j = 0; j < SIZE; ++j) {
                std::vector<int> col(SIZE);
                for (int i = 0; i < SIZE; ++i)
                    col[i] = grid[SIZE - 1 - i][j];
                if (slideAndMergeRow(col)) {
                    for (int i = 0; i < SIZE; ++i)
                        grid[SIZE - 1 - i][j] = col[i];
                    moved = true;
                }
            }
            break;
        case 'a':
            for (int i = 0; i < SIZE; ++i) {
                std::vector<int> row(grid[i], grid[i] + SIZE);
                if (slideAndMergeRow(row)) {
                    for (int j = 0; j < SIZE; ++j)
                        grid[i][j] = row[j];
                    moved = true;
                }
            }
            break;
        case 'd':
            for (int i = 0; i < SIZE; ++i) {
                std::vector<int> row(SIZE);
                for (int j = 0; j < SIZE; ++j)
                    row[j] = grid[i][SIZE - 1 - j];
                if (slideAndMergeRow(row)) {
                    for (int j = 0; j < SIZE; ++j)
                        grid[i][SIZE - 1 - j] = row[j];
                    moved = true;
                }
            }
            break;
    }

    return moved;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    loadHighScore();
    addRandomTile();
    addRandomTile();

    while (true) {
        printGrid();
        if (!canMove()) {
            std::cout << "Game Over! Press Q to quit.\n";
            char ch;
            std::cin >> ch;
            break;
        }

        char ch;
        std::cin >> ch;
        ch = std::tolower(ch);

        if (ch == 'q') break;

        if (move(ch))
            addRandomTile();
    }

    saveHighScore();
    return 0;
}
