#include "Application.h"

#include <format>
#include <iostream>
#include <vector>

int ruleForAlive[] = {0, 0, 1, 1, 0, 0, 0, 0, 0};
int ruleForDead[] = {0, 0, 0, 1, 0, 0, 0, 0, 0};

int main(int argc, char *argv[])
{
    Application app;
    auto &renderer = app.GetRenderer();

    std::cout << "**The Game of Life Configuration**\n" << std::endl;

    std::cout << "Select a window resolution (unit: px):" << std::endl; // assume the window is a square

    for (int i = 1; i <= 8; i++)
    {
        std::cout << std::format("{}. {}x{}", i, i * 100, i * 100) << std::endl;
    }

    int resolutionChoice = 0;
    std::cin >> resolutionChoice;

    if (resolutionChoice < 1 || resolutionChoice > 8)
    {
        std::cout << "Invalid choice!\n";
        return 0;
    }

    resolutionChoice *= 100;
    std::cout << "Selected resolution: " << std::format("{}x{}", resolutionChoice, resolutionChoice) << '\n' << std::endl;

    std::cout << "Select the grid resolution:" << std::endl;
    int gridResolutionChoice = 0;

    {
        std::vector<int> gridResolutions;
        int j = 1;

        for (int i = 2; i <= resolutionChoice / 20; i++)
        {
            if (resolutionChoice % i == 0)
            {
                std::cout << std::format("{}. {}x{}", j, i, i) << std::endl;
                gridResolutions.push_back(i);
                ++j;
            }
        }

        std::cin >> gridResolutionChoice;

        if (gridResolutionChoice < 1 || gridResolutionChoice > gridResolutions.size())
        {
            std::cout << "Invalid choice!\n";
            return 0;
        }

        gridResolutionChoice = gridResolutions[gridResolutionChoice - 1];
    }

    std::cout << "Selected grid resolution: " << std::format("{}x{}", gridResolutionChoice, gridResolutionChoice) << '\n' << std::endl;

    float animSpeed = 0.0;
    std::cout << "Enter the animation speed (unit: ms): ";
    std::cin >> animSpeed;
    animSpeed /= 1000.0f;

    std::cout << "Configuration completed. You can now click a grid to set it \"alive\"" << std::endl;
    std::cout << "Then press <enter> to start the game!" << std::endl;

    bool gameStarted = false;
    int gridSizeLength = resolutionChoice / gridResolutionChoice;
    std::vector<std::vector<int>> gridsFront(gridResolutionChoice, std::vector<int>(gridResolutionChoice));
    std::vector<std::vector<int>> gridsBack(gridResolutionChoice, std::vector<int>(gridResolutionChoice));

    auto epoch = [&](float dt) {
        static float animTimer = 0.0f;
        animTimer += dt;
        if (animTimer >= animSpeed)
        {
            animTimer = 0.0f;

            // update according to the data in gridsFront
            // write updated data to gridsBack
            // swap them so that the updated data can be displayed
            // i.e. double buffer
            for (int x = 0; x < gridResolutionChoice; x++)
            {
                for (int y = 0; y < gridResolutionChoice; y++)
                {
                    int alive = 0;

                    for (int dx = -1; dx <= 1; dx++)
                    {
                        for (int dy = -1; dy <= 1; dy++)
                        {
                            if (!dx && !dy)
                            {
                                continue;
                            }

                            int nx = x + dx;
                            int ny = y + dy;
                            if (nx >= 0 && nx < gridResolutionChoice && ny >= 0 && ny < gridResolutionChoice && gridsFront[nx][ny])
                            {
                                ++alive;
                            }
                        }
                    }

                    gridsBack[x][y] = gridsFront[x][y] ? ruleForAlive[alive] : ruleForDead[alive];
                }
            }

            gridsFront.swap(gridsBack);
        }
    };

    double cursorX = 0.0, cursorY = 0.0;

    app.RegisterOnMouseButtonFunc([&](int button, int action, int modes) {
        static bool trigger = false;

        if (!gameStarted)
        {
            if (!trigger && action == SDL_MOUSEBUTTONDOWN)
            {
                gridsFront[cursorX / gridSizeLength][cursorY / gridSizeLength] = 1;
                trigger = true;
            }
            else if (action == SDL_MOUSEBUTTONUP)
            {
                trigger = false;
            }
        }
    });

    app.RegisterOnCursorPosFunc([&](double x, double y) {
        cursorX = x;
        cursorY = y;
    });

    app.RegisterOnKeyFunc([&](int key, int scanCode, int action, int mods) {
        static bool trigger = false;

        if (!trigger && key == SDLK_RETURN)
        {
            gameStarted = true;
            trigger = true;
        }
    });

    app.Init(resolutionChoice);

    app.Run([&](float dt) {
        if (!gameStarted)
        {
            renderer.RenderGrids(gridResolutionChoice);
        }
        else
        {
            epoch(dt);
        }

        for (int x = 0; x < gridResolutionChoice; x++)
        {
            for (int y = 0; y < gridResolutionChoice; y++)
            {
                if (gridsFront[x][y])
                {
                    renderer.FillGrid(gridResolutionChoice, x, y);
                }
            }
        }
    });

    app.Shutdown();

    return 0;
}
