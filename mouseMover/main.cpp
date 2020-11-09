///////////////////////////////////////////
// (c) - Luta Vlad 2019
//https://github.com/meemknight/mouseMover
///////////////////////////////////////////
#include <windows.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <assert.h>

bool running = true;

#define DISPLAY(x) std::cout << #x << ": " << x

std::vector<std::pair<HMONITOR, MONITORINFO>> monitors;

BOOL CALLBACK registerMonitor(HMONITOR hMonitor, HDC hdc, LPRECT rect, LPARAM data)
{
	MONITORINFO temp;
	monitors.emplace_back(hMonitor, temp);
	return 1;
}

void loadMonitors()
{
	std::cout << "reloaded: ";
	monitors.clear();
	EnumDisplayMonitors(NULL, NULL, registerMonitor, 0);

	
	for (auto &i : monitors)
	{
		i.second.cbSize = sizeof(i.second);
		GetMonitorInfo(i.first, &i.second);
	}


	std::sort(monitors.begin(), monitors.end(), [](std::pair<HMONITOR, MONITORINFO> i, std::pair<HMONITOR, MONITORINFO> j)
	{
		return (i.second.rcMonitor.left < j.second.rcMonitor.left);
	});
	std::cout << monitors.size() << "\n";
}

void getMouseData(int &index, POINT &position)
{
	index = -1;
	position;
	GetCursorPos(&position);

	for (int i = 0; i < monitors.size(); i++)
	{
		if (position.x >= monitors[i].second.rcMonitor.left)
		{
			index = i;
		}
	}

}


void decrease()
{
	int current;
	POINT position;
	getMouseData(current, position);
	if(current > 0)
	{
		int padding = position.x - monitors[current].second.rcMonitor.left;
		SetCursorPos({ monitors[current - 1].second.rcMonitor.left + padding }, position.y);
	}
};

void increase()
{
	int current;
	POINT position;
	getMouseData(current, position);
	if(current < monitors.size()-1)
	{
		int padding = position.x - monitors[current].second.rcMonitor.left;
		SetCursorPos({ monitors[current + 1].second.rcMonitor.left + padding }, position.y);
	}

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:

		RegisterHotKey(hwnd, 1, MOD_ALT | MOD_WIN | MOD_NOREPEAT, VK_LEFT);
		RegisterHotKey(hwnd, 2, MOD_ALT | MOD_WIN | MOD_NOREPEAT, VK_RIGHT);

		break;
	case WM_DESTROY:
		UnregisterHotKey(0, 1);
		UnregisterHotKey(0, 2);
		running = false;
		PostQuitMessage(0);
		ExitProcess(0);
		break;
	case WM_DEVICECHANGE:
		loadMonitors();
		break;
	case WM_HOTKEY:
		if (wParam == 1)
		{
			decrease();
		}
		else
		if (wParam == 2)
		{
			increase();
		}
		break;
	default:
		DefWindowProc(hwnd, uMsg, wParam, lParam);
		break;
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	
	LPCSTR szWindowClass = "window";

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			"Call to RegisterClassEx failed!",
			"Windows Desktop Guided Tour",
			NULL);

		return 1;
	}

	HWND hWnd = CreateWindow(
		szWindowClass,
		"mouseMover",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		300, 90,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hWnd)
	{
		MessageBox(NULL,
			"Call to CreateWindow failed!",
			"Windows Desktop Guided Tour",
			NULL);

		return 1;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	loadMonitors();

	bool pressed1 = 0;
	bool pressed2 = 0;

	MSG msg;
	while (running)
	{	
		GetMessage(&msg, NULL, 0, 0);
	
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		
	}

	return 0;
}