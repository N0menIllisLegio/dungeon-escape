#include <Windows.h>
#include <thread>

#include "Game.h"
#include "Level.h"
#include "resource.h"
#include "Serial.h"

#define COM_PORT 5
#define BAUD_RATE 9600

using namespace std;

Game *game;
BOOL listeningPort = true;

void CreateNewGame(HWND hwnd)
{
	string levelDirectory = "levels";

	Level::initializeLevelDirectory(levelDirectory);
	shared_ptr<Level> level = Level::loadLevel(levelDirectory);
	
	if (!Level::checkLevel(hwnd, level))
	{
		exit(-1);
	}

	if (game != nullptr)
	{
		delete game;
	}

	game = new Game(hwnd, level, 2);
	game->Start();
}

void ListenCOM(HWND *hwnd)
{
	CSerial port;

    if (port.Open(COM_PORT, BAUD_RATE))
    {
        MessageBox(*hwnd, "Port is open!", "Message", MB_OK | MB_ICONINFORMATION);
        char* lpBuffer = new char[500];

        while(listeningPort)
        {
            if (port.ReadDataWaiting() > 0)
            {
                int nBytesRead = port.ReadData(lpBuffer, 500);
				std::string command = std::string(lpBuffer).substr(0, nBytesRead);

				switch (command[0])
				{
				case 'U':
					PostMessage(*hwnd, WM_KEYDOWN, 0x57, 0);
					break;
				case 'D':
					PostMessage(*hwnd, WM_KEYDOWN, 0x53, 0);
					break;
				case 'R':
					PostMessage(*hwnd, WM_KEYDOWN, 0x44, 0);
					break;
				case 'L':
					PostMessage(*hwnd, WM_KEYDOWN, 0x41, 0);
					break;
				case 'Q':
					PostMessage(*hwnd, WM_CLOSE, 0, 0);
					break;
				default:
					break;
				}
            }
        }
    }
    else
    {
		MessageBox(*hwnd, "Port is not open!", "Warning!", MB_OK | MB_ICONERROR);
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
    switch (msg) 
	{
    case WM_KEYDOWN:
        switch (wParam) 
		{
        case 'W':
			game->MovePlayer(Command::MoveNorth);
			InvalidateRect(hwnd, NULL, false);
            break;
        case 'D':
			game->MovePlayer(Command::MoveEast);
			InvalidateRect(hwnd, NULL, false);
            break;
        case 'S':
			game->MovePlayer(Command::MoveSouth);
			InvalidateRect(hwnd, NULL, false);
            break;
        case 'A':
			game->MovePlayer(Command::MoveWest);
			InvalidateRect(hwnd, NULL, false);
            break;
        }
        break;
	case WM_PAINT:
		game->DisplayLevel();
		break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
		return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
	MSG msg = { 0 };
    WNDCLASS wc = { 0 };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "DungeonClass";
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);

	HWND hwnd = CreateWindow("DungeonClass", "Dungeon Escape", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 0, 0,
		250, 250, NULL, NULL, hInstance, NULL);

	SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	DestroyIcon(hIcon);
	CreateNewGame(hwnd);
	
	ShowWindow(hwnd, nCmdShow);

	BOOL bRet;
	std::thread listenCOM(ListenCOM, &hwnd);

	while ((bRet = GetMessage(&msg, hwnd, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			MessageBox(hwnd, "Ooops somthing gone wrong...", "Error!", MB_OK | MB_ICONERROR);
			break;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (game->isWin())
				game->Start();
		}
	}

	listeningPort = false;
	listenCOM.join();

	delete game;

    return 0;
}


