#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <D3D11App.h>
#include <sstream>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);
        }
        return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[]  = L"SharedSurface11";
    
    WNDCLASS wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.
    HWND hwnd = CreateWindowEx(
        0,                     // Optional window styles.
        CLASS_NAME,            // Window class
        L"SharedSurface11",    // Window text
        WS_OVERLAPPEDWINDOW,   // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    D3D11App app;
    app.Init(hwnd);

    STARTUPINFO si {0};
    PROCESS_INFORMATION pi {0};

    std::wstringstream cmdline;
    cmdline << L"../../HelloTriangle/Debug/HelloTriangle.exe"
            << L' ' << app.GetSurface()
            << L' ' << app.GetFence();

    bool status = CreateProcess(
        nullptr,
        (wchar_t*)cmdline.str().c_str(),
        nullptr,
        nullptr,
        false,
        0,
        nullptr,
        nullptr,
        &si,
        &pi
    );

    // Run the message loop.
    MSG msg = { };
    while (app.Update())
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    app.Shutdown();
    return 0;
}
