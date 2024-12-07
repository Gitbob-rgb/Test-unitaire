#include "pch.h" // Inclure les précompilés si nécessaire  
#include "Grid.h" // Inclure votre classe Grid  
#include <fstream>  
#include <gtest/gtest.h>  

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

    int livingBeforeUpdate = grid->countLivingCells();  // Admettant que cette méthode existe  
    grid->update();
    int livingAfterUpdate = grid->countLivingCells();   // Idem  

    // Vérifiez que le nombre de cellules vivantes a changé  
    EXPECT_NE(livingBeforeUpdate, livingAfterUpdate); // Doit être différent  
}

TEST_F(GridTests, TestAreAllCellsDead) {
    EXPECT_TRUE(grid->areAllCellsDead()); // Toutes les cellules sont mortes au début  

    // Activer une cellule  
    grid->toggleCell(0, 0, 1); // 1 pour vivante  
    EXPECT_FALSE(grid->areAllCellsDead()); // Pas toutes mortes  

    // Réinitialiser toutes les cellules  
    grid->toggleCell(0, 0, 0); // 0 pour morte  
    EXPECT_TRUE(grid->areAllCellsDead()); // Toutes les cellules sont mortes  
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
    for (int i = 0; i < desiredGeneration; i++) {
        grid->update(); // Mettez à jour la grille  
    }

    // Maintenant testez si le nombre de cellules vivantes est celui attendu  
    int expectedAliveCount = 4; // À ajuster selon vos attentes spécifiques  
    EXPECT_TRUE(grid->validateGrid(expectedAliveCount)); // Vérifiez la validité  
}

// Main pour exécuter tous les tests  
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}