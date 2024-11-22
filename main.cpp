#include "pch.h"
#include <windows.h>

int main() {
    SetDllDirectory("dlls"); // Set the DLL directory to the dlls folder

    srand(time(nullptr)); //Seed the random number generator

    MainMenu mainMenu; //Create a main menu object

    std::vector<std::string> tutorialPaths = {
        "Textures/Tutorials/tutorial1.png",
        "Textures/Tutorials/tutorial2.jpg",
        "Textures/Tutorials/tutorial3.jpg",
        "Textures/Tutorials/tutorial4.jpg",
        "Textures/Tutorials/tutorial5.png",
        // Add as many images as you need
    };
    mainMenu.loadTutorialImages(tutorialPaths); //Load the tutorial images

    InitGameStart initGameStart; //Create a init game start object
    Game game(mainMenu, initGameStart); //Create a game object

    game.run(); //Run the game

    //End of the program
    return 0;
}
