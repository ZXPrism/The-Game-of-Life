#pragma once

#include <SDL.h>

class Renderer
{
public:
    bool Init(SDL_Window *pWindow);
    void NewFrame();
    void EndFrame();
    void Shutdown();

    void RenderGrids(int gridResolution);
    void FillGrid(int gridResolution, int x, int y);

private:
    int _windowSizeLength = 0;
    SDL_Renderer *_pRenderer;
};
