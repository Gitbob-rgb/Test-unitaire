#ifndef GRID_H  
#define GRID_H  

#include <vector>  
#include <string>  
#include <fstream>  
#include <iostream> 
class Grid {
public:
    Grid(int rows, int cols);
    void toggleCell(int row, int col, int state); // 1 pour vivante, 0 pour morte  
    void update(); // Met à jour l'état de la grille  
    void initializeFromInput(const std::string& filename); // Initialise à partir d'un fichier  
    void saveToFile(const std::string& dir, const std::string& filename, int generation); // Enregistre dans un fichier  
    bool areAllCellsDead() const; // Vérifie si toutes les cellules sont mortes  
    int countLivingCells() const; // Compte les cellules vivantes  
    const std::vector<std::vector<int>>& getCells() const; // Fournit l'état des cellules  
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    bool compareWithExpectedState(const std::vector<std::vector<int>>& expectedState) const;

    

private:
    int rows; // Nombre de lignes  
    int cols; // Nombre de colonnes  
    std::vector<std::vector<int>> cells; // 0 pour morte, 1 pour vivante  
};

#endif // GRID_H

  

Grid::Grid(int rows, int cols) : rows(rows), cols(cols) {
    // Initialiser la grille avec toutes les cellules mortes  
    cells.resize(rows, std::vector<int>(cols, 0));
}

void Grid::toggleCell(int row, int col, int state) {
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        cells[row][col] = state; // state est 0 pour morte, 1 pour vivante  
    }
}

void Grid::update() {
    std::vector<std::vector<int>> newCells(rows, std::vector<int>(cols, 0)); // Nouvelle grille pour la mise à jour  

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int aliveNeighbors = 0;

            // Compter les voisins vivants  
            for (int ni = -1; ni <= 1; ++ni) {
                for (int nj = -1; nj <= 1; ++nj) {
                    if (ni == 0 && nj == 0) continue; // Ignorer la cellule elle-même  
                    int newRow = i + ni;
                    int newCol = j + nj;
                    if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols) {
                        aliveNeighbors += cells[newRow][newCol];
                    }
                }
            }

            // Appliquer les règles de Conway pour décider de l'état futur  
            if (cells[i][j] == 1) { // Cellule vivante  
                newCells[i][j] = (aliveNeighbors == 2 || aliveNeighbors == 3) ? 1 : 0; // Reste vivante ou meurt  
            }
            else { // Cellule morte  
                newCells[i][j] = (aliveNeighbors == 3) ? 1 : 0; // Devient vivante si 3 voisins vivants  
            }
        }
    }

    // Mettre à jour l'état des cellules  
    cells = newCells;
}

void Grid::initializeFromInput(const std::string& filename) {
    std::ifstream inFile(filename);
    if (inFile.is_open()) {
        inFile >> rows >> cols;
        cells.resize(rows, std::vector<int>(cols, 0)); // Réinitialisation de la taille  
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                inFile >> cells[i][j]; // Lecture de chaque cellule  
            }
        }
        inFile.close();
    }
}

void Grid::saveToFile(const std::string& dir, const std::string& filename, int generation) {
    std::ofstream outFile(dir + "/" + filename + "_gen_" + std::to_string(generation) + ".txt");
    if (outFile.is_open()) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                outFile << cells[i][j] << " "; // Écriture de l'état de la cellule dans le fichier  
            }
            outFile << "\n";
        }
        outFile.close();
    }
}

bool Grid::areAllCellsDead() const {
    for (const auto& row : cells) {
        for (const auto& cell : row) {
            if (cell == 1) return false; // Si une cellule est vivante, retourne faux  
        }
    }
    return true; // Toutes les cellules sont mortes  
}

int Grid::countLivingCells() const {
    int count = 0;
    for (const auto& row : cells) {
        for (const auto& cell : row) {
            if (cell == 1) { // Cellule vivante  
                count++;
            }
        }
    }
    return count;
}

const std::vector<std::vector<int>>& Grid::getCells() const {
    return cells; // Renvoie une référence constante à l'état des cellules  
}

// Grid.cpp  
bool Grid::compareWithExpectedState(const std::vector<std::vector<int>>& expectedState) const {
    if (expectedState.size() != cells.size()) {
        return false; // Different number of rows  
    }
    for (size_t i = 0; i < expectedState.size(); ++i) {
        if (expectedState[i].size() != cells[i].size()) {
            return false; // Different number of columns in row i  
        }
        for (size_t j = 0; j < expectedState[i].size(); ++j) {
            if (cells[i][j] != expectedState[i][j]) {
                return false; // Cells do not match  
            }
        }
    }
    return true; // All cells match  
}
