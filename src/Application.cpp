#include "Application.h"

#include "Logger.h"

#include <chrono>

void Application::Init(int windowSideLength)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        LOG_ERROR("Failed to initialize SDL: %s", SDL_GetError());
        return;
    }

    _pWindow = SDL_CreateWindow("The Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowSideLength, windowSideLength,
                                SDL_WINDOW_SHOWN);
    if (!_pWindow)
    {
        LOG_ERROR("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return;
    }

    if (!_renderer.Init(_pWindow))
    {
        return;
    }
}

void Application::Run(const std::function<void(float)> &callback)
{
    namespace ch = std::chrono;

    auto t1 = ch::steady_clock::now();
    float dt = 0.0f;

    while (_AppRunning)
    {
        ProcessEvents();

        _renderer.NewFrame();
        callback(dt);
        _renderer.EndFrame();

        auto t2 = ch::steady_clock::now();
        dt = ch::duration_cast<ch::microseconds>(t2 - t1).count() / 1000000.0;
        t1 = t2;
    }
}

void Application::Shutdown()
{
    _renderer.Shutdown();
    SDL_DestroyWindow(_pWindow);

    SDL_Quit();
}

void Application::ProcessEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event) != 0)
    {
        switch (event.type)
        {
        case SDL_QUIT:
            _AppRunning = false;
            break;

        case SDL_KEYDOWN:
        case SDL_KEYUP:
            OnKey(event.key.keysym.sym, event.key.keysym.scancode, event.key.type, event.key.keysym.mod);
            break;

        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                OnWindowSize(event.window.data1, event.window.data2);
            }
            break;

        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            OnMouseButton(event.button.button, event.button.type, SDL_GetModState());
            break;

        case SDL_MOUSEMOTION:
            OnCursorPos(event.motion.x, event.motion.y);
            break;

        default:
            break;
        }
    }
}

void Application::RegisterOnKeyFunc(OnKeyFunc func)
{
    _OnKeyVec.push_back(func);
}

void Application::RegisterOnMouseButtonFunc(OnMouseButtonFunc func)
{
    _OnMouseButtonVec.push_back(func);
}

void Application::RegisterOnCursorPosFunc(OnCursorPosFunc func)
{
    _OnCursorPosVec.push_back(func);
}

void Application::RegisterOnWindowSizeFunc(OnWindowSizeFunc func)
{
    _OnWindowSizeVec.push_back(func);
}

void Application::OnKey(int key, int scanCode, int action, int mods)
{
    for (auto &fn : _OnKeyVec)
        fn(key, scanCode, action, mods);
}

void Application::OnMouseButton(int button, int action, int mods)
{
    for (auto &fn : _OnMouseButtonVec)
        fn(button, action, mods);
}

void Application::OnCursorPos(double xPos, double yPos)
{
    for (auto &fn : _OnCursorPosVec)
        fn(xPos, yPos);
}

void Application::OnWindowSize(int width, int height)
{
    for (auto &fn : _OnWindowSizeVec)
        fn(width, height);
}

Renderer &Application::GetRenderer()
{
    return _renderer;
}
