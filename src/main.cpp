#include <windows.h>
#include <string>

// Global variables
HINSTANCE hInst;
HWND hWndMain;

// Function Prototypes
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void LaunchEmulator();
void OpenFileExplorer();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    hInst = hInstance;

    // 1. Register the Window Class
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = "EverythingAppClass";

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // 2. Create the Window
    hWndMain = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "EverythingAppClass",
        "EverythingApp - Portable Environment",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, hInstance, NULL
    );

    if (hWndMain == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hWndMain, nCmdShow);
    UpdateWindow(hWndMain);

    // 3. Message Loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            // Create buttons for the main features
            CreateWindow("BUTTON", "Launch Emulator (QEMU)", 
                         WS_VISIBLE | WS_CHILD, 
                         20, 20, 200, 40, 
                         hwnd, (HMENU)1, hInst, NULL);

            CreateWindow("BUTTON", "Open File Explorer", 
                         WS_VISIBLE | WS_CHILD, 
                         20, 80, 200, 40, 
                         hwnd, (HMENU)2, hInst, NULL);
            break;
        }
        case WM_COMMAND: {
            if (LOWORD(wParam) == 1) {
                LaunchEmulator();
            } else if (LOWORD(wParam) == 2) {
                OpenFileExplorer();
            }
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void LaunchEmulator() {
    // This looks for qemu-system-x86_64.exe in a subfolder named 'qemu'
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    
    // Command line arguments for QEMU (WinXP example)
    char cmd[] = "qemu\\qemu-system-x86_64.exe -m 2048 -drive file=winxp.img,format=raw";

    if (CreateProcess(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    } else {
        MessageBox(hWndMain, "Failed to find QEMU in the portable directory.", "Error", MB_OK);
    }
}

void OpenFileExplorer() {
    // Opens the current directory in the default file explorer
    ShellExecute(NULL, "explore", ".", NULL, NULL, SW_SHOWNORMAL);
}
