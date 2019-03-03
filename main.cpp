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

int main()
{
	std::cout << "\n";
	
	//std::cout << "number of visible screens: " << GetSystemMetrics(SM_CMONITORS) << "\n";
	//std::cout << "number of screens: " << monitors.size() << "\n\n";
	
	loadMonitors();

	bool pressed1 = 0;
	bool pressed2 = 0;


	while (1)
	{


		
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

}