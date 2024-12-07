#include "pch.h" // Inclure les pr�compil�s si n�cessaire  
#include "Grid.h" // Inclure votre classe Grid  
#include <fstream>  
#include <gtest/gtest.h>  

class GridTests : public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        // Installer des fichiers ou des configurations communes si n�cessaire  
    }

    static void TearDownTestSuite() {
        // Nettoyer des fichiers ou des configurations communes si n�cessaire  
    }

    void SetUp() override {
        // Cette m�thode sera appel�e avant chaque test  
        grid = new Grid(5, 10);
    }

    void TearDown() override {
        // Cette m�thode sera appel�e apr�s chaque test  
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

    // V�rifiez que les cellules sont initialis�es correctement  
    EXPECT_FALSE(grid->areAllCellsDead()); // Pas toutes mortes  
    EXPECT_EQ(grid->getRows(), 5);
    EXPECT_EQ(grid->getCols(), 10);
}

TEST_F(GridTests, TestUpdate) {
    grid->initializeFromInput("nyamchi.txt"); // Initialiser � partir du fichier  

    int livingBeforeUpdate = grid->countLivingCells();  // Admettant que cette m�thode existe  
    grid->update();
    int livingAfterUpdate = grid->countLivingCells();   // Idem  

    // V�rifiez que le nombre de cellules vivantes a chang�  
    EXPECT_NE(livingBeforeUpdate, livingAfterUpdate); // Doit �tre diff�rent  
}

TEST_F(GridTests, TestAreAllCellsDead) {
    EXPECT_TRUE(grid->areAllCellsDead()); // Toutes les cellules sont mortes au d�but  

    // Activer une cellule  
    grid->toggleCell(0, 0, 1); // 1 pour vivante  
    EXPECT_FALSE(grid->areAllCellsDead()); // Pas toutes mortes  

    // R�initialiser toutes les cellules  
    grid->toggleCell(0, 0, 0); // 0 pour morte  
    EXPECT_TRUE(grid->areAllCellsDead()); // Toutes les cellules sont mortes  
}

TEST_F(GridTests, TestSaveToFile) {  
    grid->toggleCell(0, 0, 1); // Activer une cellule  

    // Sauvegarder dans un fichier  
    grid->saveToFile(".", "nyamchi_output", 1);  

    // V�rifiez que le fichier a �t� cr�� et contient les bonnes valeurs  
    std::ifstream inFile("./nyamchi_output_gen_1.txt");  
    EXPECT_TRUE(inFile.is_open());  

    // V�rifiez le contenu du fichier pour la premi�re ligne  
    std::string line;  
    std::getline(inFile, line);  
    EXPECT_EQ(line, "1 0 0 0 0 0 0 0 0 0 "); // V�rifiez la premi�re ligne  

    // Nettoyez apr�s le test  
    inFile.close();  
    remove("./nyamchi_output_gen_1.txt");  
} 


TEST_F(GridTests, TestValidateGridAtSpecificGeneration) {
    grid->initializeFromInput("nyamchi.txt");

    // Effectuer un certain nombre d'it�rations pour atteindre la g�n�ration d�sir�e.  
    int desiredGeneration = 1; // Choisissez la g�n�ration  
    for (int i = 0; i < desiredGeneration; i++) {
        grid->update(); // Mettez � jour la grille  
    }

    // Maintenant testez si le nombre de cellules vivantes est celui attendu  
    int expectedAliveCount = 4; // � ajuster selon vos attentes sp�cifiques  
    EXPECT_TRUE(grid->validateGrid(expectedAliveCount)); // V�rifiez la validit�  
}

// Main pour ex�cuter tous les tests  
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}