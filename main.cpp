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



int main()
{
	std::cout << "\n";

	EnumDisplayMonitors(NULL, NULL, registerMonitor, 0);
	
	std::cout << "number of visible screens: " << GetSystemMetrics(SM_CMONITORS) << "\n";
	std::cout << "number of screens: " << monitors.size() << "\n\n";

	int c = 0;
	for(auto &i: monitors)
	{
		
		i.second.cbSize = sizeof(i.second);
		if (!GetMonitorInfo(i.first, &i.second))
		{
			std::cout << "error getting some monitor data: " << GetLastError() << "\n";
		}else
		{
			
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
			
		}

		c++;
	}
	

	std::sort(monitors.begin(), monitors.end(), [](std::pair<HMONITOR, MONITORINFO> i, std::pair<HMONITOR, MONITORINFO> j)
	{
		return (i.second.rcMonitor.left < j.second.rcMonitor.left);
	});

	bool pressed = 0;

	std::cout << monitors[0].second.rcMonitor.right << " ";
	std::cout << monitors[1].second.rcMonitor.right << " ";

	while (1)
	{
		int monitorIndex = -1;
		POINT mousePosition;
		GetCursorPos(&mousePosition);

		for(int i=0; i<monitors.size(); i++)
		{
			if(mousePosition.x > monitors[i].second.rcMonitor.left)
			{
				monitorIndex = i;
			}
		}

		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,0 });
		std::cout << monitorIndex << " ";

		if(pressed == 0 && GetAsyncKeyState(VK_NUMPAD7))
		{
			pressed = 1;
			//SetCursorPos(3000, 100);
			
		}
		else
		{
			if(!GetAsyncKeyState(VK_NUMPAD7))
			{
				pressed = 0;
				ClipCursor(nullptr);
			}
		}

	}

}