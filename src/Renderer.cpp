#include "Renderer.h"

#include "Logger.h"

bool Renderer::Init(SDL_Window *pWindow)
{
    SDL_GetWindowSize(pWindow, &_windowSizeLength, &_windowSizeLength);

    _pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!_pRenderer)
    {
        LOG_ERROR("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        return false;
    }

    return true;
}

void Renderer::NewFrame()
{
    SDL_SetRenderDrawColor(_pRenderer, 255, 255, 255, 255);
    SDL_RenderClear(_pRenderer);
}

void Renderer::EndFrame()
{
    SDL_RenderPresent(_pRenderer);
}

void Renderer::Shutdown()
{
    SDL_DestroyRenderer(_pRenderer);
}

void Renderer::RenderGrids(int gridResolution)
{
    int gridSizeLength = _windowSizeLength / gridResolution;

    SDL_SetRenderDrawColor(_pRenderer, 100, 100, 100, 255);
    for (int i = 0; i < _windowSizeLength; i += gridSizeLength)
    {
        SDL_RenderDrawLine(_pRenderer, i, 0, i, _windowSizeLength);
        SDL_RenderDrawLine(_pRenderer, 0, i, _windowSizeLength, i);
    }
}

void Renderer::FillGrid(int gridResolution, int x, int y)
{
    int gridSizeLength = _windowSizeLength / gridResolution;

    SDL_Rect rect;
    rect.x = x * gridSizeLength;
    rect.y = y * gridSizeLength;
    rect.w = gridSizeLength;
    rect.h = gridSizeLength;

    SDL_SetRenderDrawColor(_pRenderer, 0, 0, 0, 255);
    SDL_RenderFillRect(_pRenderer, &rect);
}
