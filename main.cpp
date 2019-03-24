///////////////////////////////////////////
// (c) - Luta Vlad 2019
//https://github.com/meemknight/mouseMover
///////////////////////////////////////////
#include <windows.h>
#include <iostream>
#include <vector>
#include <algorithm>

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

	int c = 0;
	for (auto &i : monitors)
	{

		i.second.cbSize = sizeof(i.second);
		if (!GetMonitorInfo(i.first, &i.second))
		{
			std::cout << "error getting some monitor data: " << GetLastError() << "\n";
		}
		else
		{
			/*
			//std::cout << "Monitor: " << c << " " << info.second.szDevice << "\n";
			DISPLAY(i.second.rcMonitor.bottom) << " ";
			DISPLAY(i.second.rcMonitor.top) << " ";
			DISPLAY(i.second.rcMonitor.left) << " ";
			DISPLAY(i.second.rcMonitor.right) << "\n";

			DISPLAY(i.second.rcWork.bottom) << " ";
			DISPLAY(i.second.rcWork.top) << " ";
			DISPLAY(i.second.rcWork.left) << " ";
			DISPLAY(i.second.rcWork.right) << "\n";

			std::cout << ((i.second.dwFlags & MONITORINFOF_PRIMARY) ? "Primary Monitor" : "Non primary Monitor");

			std::cout << "\n\n";
			*/
		}

		c++;
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
	case WM_DESTROY:
		PostQuitMessage(0);
		ExitProcess(0);
		break;
	case WM_DEVICECHANGE:
		loadMonitors();
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
		break;
	}

	return 0;
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
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
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


	// The parameters to CreateWindow explained:
	// szWindowClass: the name of the application
	// szTitle: the text that appears in the title bar
	// WS_OVERLAPPEDWINDOW: the type of window to create
	// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
	// 500, 100: initial size (width, length)
	// NULL: the parent of this window
	// NULL: this application dows not have a menu bar
	// hInstance: the first parameter from WinMain
	// NULL: not used in this application
	HWND hWnd = CreateWindow(
		szWindowClass,
		"mouseMover",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		500, 500,
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

	// Main message loop:


	std::cout << "\n";
	
	HCURSOR defaultCursor = GetCursor();
	struct RAII_
	{
		HCURSOR c;
		void(*function)(HCURSOR c);
		~RAII_() { function(c); }
	}instance{defaultCursor, [](HCURSOR c)
	{
		SetCursor(c);
	} };

	auto a = [defaultCursor]() {};

	loadMonitors();

	bool pressed1 = 0;
	bool pressed2 = 0;


	while (1)
	{
		MSG msg;

		GetMessage(&msg, NULL, 0, 0);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		

		//SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,0 });
		//std::cout << monitorIndex << " ";

		if(pressed1 == 0 && GetAsyncKeyState('Q'))
		{
			pressed1 = 1;
			decrease();
		}
		else
		{
			if(!GetAsyncKeyState('Q'))
			{
				pressed1 = 0;
			}
		}
		
		if (pressed2 == 0 && GetAsyncKeyState('E'))
		{
			pressed2 = 1;
			increase();
		}
		else
		{
			if (!GetAsyncKeyState('E'))
			{
				pressed2 = 0;
			}
		}

	}

	return 0;
}