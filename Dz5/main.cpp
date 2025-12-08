#include <iostream>
#include <memory>
#include "GameView.h"
#include "GameScene.h"

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    try
    {
        auto gameView = std::unique_ptr<GameView>(NewGameView({ 800, 600 }));
        auto gameScene = std::unique_ptr<GameScene>(NewGameScene());

        EnterGameLoop(*gameView, UpdateGameScene, DrawGameScene, gameScene.get());
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Ошибка: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
