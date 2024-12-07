#include "pch.h" // Inclure les précompilés si nécessaire  
#include "Grid.h" // Inclure votre classe Grid  
#include <fstream>  
#include <gtest/gtest.h>   
#include <sstream>  
#include <vector>  
#include <windows.h>  
// Fonction utilitaire pour lire le fichier et récupérer l'état de la grille  
std::vector<std::vector<int>> readExpectedStateFromFile(const std::string& filename) {
    std::ifstream inFile(filename);
    std::vector<std::vector<int>> state;
    std::string line;

    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        std::vector<int> row;
        int cell;

        while (iss >> cell) {
            row.push_back(cell);
        }
        state.push_back(row);
    }

    return state;
}

class GridTests : public ::testing::Test {

protected:
    static void SetUpTestSuite() {
        // Installer des fichiers ou des configurations communes si nécessaire  
    }

    static void TearDownTestSuite() {
        // Nettoyer des fichiers ou des configurations communes si nécessaire  
    }

    void SetUp() override {
        // Cette méthode sera appelée avant chaque test  
        grid = new Grid(5, 10);
    }

    void TearDown() override {
        // Cette méthode sera appelée après chaque test  
        delete grid;
    }

    Grid* grid; // Pointeur vers une instance de Grid pour utilisation dans les tests  
};

TEST_F(GridTests, TestConstructor) {
    EXPECT_EQ(grid->getRows(), 5);
    EXPECT_EQ(grid->getCols(), 10);
    EXPECT_TRUE(grid->areAllCellsDead());
}

TEST_F(GridTests, TestInitializeFromInput) {
    std::ofstream outFile("nyamchi.txt");
    outFile << "5 10\n";
    outFile << "0 0 1 0 0 0 0 0 0 0\n";
    outFile << "0 0 0 1 0 0 0 0 0 0\n";
    outFile << "0 1 1 1 0 0 0 0 0 0\n";
    outFile << "0 0 0 0 0 0 0 0 0 0\n";
    outFile << "0 0 0 0 0 0 0 0 0 0\n";
    outFile.close();

    grid->initializeFromInput("nyamchi.txt");

    // Vérifiez que les cellules sont initialisées correctement  
    EXPECT_FALSE(grid->areAllCellsDead()); // Pas toutes mortes  
    EXPECT_EQ(grid->getRows(), 5);
    EXPECT_EQ(grid->getCols(), 10);
}

TEST_F(GridTests, TestUpdate) {
    grid->initializeFromInput("nyamchi.txt"); // Initialiser à partir du fichier  
    // Obtenir l'état de la grille avant la mise à jour  
    auto stateBeforeUpdate = grid->getCells();

    grid->update(); // Mettre à jour la grille  

    // Obtenir l'état de la grille après la mise à jour  
    auto stateAfterUpdate = grid->getCells();

    // Comparer les états de la grille avant et après  
    bool isDifferent = false;
    for (int i = 0; i < grid->getRows(); ++i) {
        for (int j = 0; j < grid->getCols(); ++j) {
            if (stateBeforeUpdate[i][j] != stateAfterUpdate[i][j]) {
                isDifferent = true; // On a trouvé une différence  
                break;
            }
        }
        if (isDifferent) break; // Pas besoin de continuer  
    }

    EXPECT_TRUE(isDifferent); // Vérifiez que l'état a changé    
}

TEST_F(GridTests, TestAreAllCellsDead) {
    
    // Réinitialiser toutes les cellules  
    grid->toggleCell(0, 0, 1); // 0 pour morte  
    EXPECT_FALSE(grid->areAllCellsDead()); // Toutes les cellules sont mortes  
}

TEST_F(GridTests, TestSaveToFile) {  
    grid->toggleCell(0, 0, 1); // Activer une cellule  

    // Sauvegarder dans un fichier  
    grid->saveToFile(".", "nyamchi_output", 1);  

    // Vérifiez que le fichier a été créé et contient les bonnes valeurs  
    std::ifstream inFile("./nyamchi_output_gen_1.txt");  
    EXPECT_TRUE(inFile.is_open());  

    // Vérifiez le contenu du fichier pour la première ligne  
    std::string line;  
    std::getline(inFile, line);  
    EXPECT_EQ(line, "1 0 0 0 0 0 0 0 0 0 "); // Vérifiez la première ligne  

    // Nettoyez après le test  
    inFile.close();  
    remove("./nyamchi_output_gen_1.txt");  
} 


TEST_F(GridTests, TestValidateGridAtSpecificGeneration) {
    grid->initializeFromInput("nyamchi.txt");

    // Effectuer un certain nombre d'itérations pour atteindre la génération désirée.  
    int desiredGeneration = 1; // Choisissez la génération 
    
    std::string expectedFile = "nyamchi.txt_gen_" + std::to_string(desiredGeneration) + ".txt";
    auto expectedState = readExpectedStateFromFile(expectedFile);

    for (int i = 0; i < desiredGeneration; i++) {
        grid->update(); // Mettez à jour la grille  
    }

    // Récupérer l'état actuel de la grille  
    auto currentState = grid->getCells(); // Utilisation correcte  

    // Vérifier l'état de la grille après la génération souhaitée  
    for (size_t i = 0; i < expectedState.size(); ++i) {
        for (size_t j = 0; j < expectedState[i].size(); ++j) {
            // Comparer directement sans vérification de taille  
            EXPECT_EQ(currentState[i][j], expectedState[i][j])
                << u8"Erreur à la position (" << i << ", " << j << ")";
        }
    }
}
// Main pour exécuter tous les tests  
int main(int argc, char** argv) {
    SetConsoleOutputCP(65001);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}