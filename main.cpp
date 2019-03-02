///////////////////////////////////////////
// (c) - Luta Vlad 2019
//https://github.com/meemknight/mouseMover
///////////////////////////////////////////
#include <windows.h>
#include <iostream>
#include <vector>

#define DISPLAY(x) std::cout << #x << ": " << x

std::vector<HMONITOR> monitors;

BOOL CALLBACK registerMonitor(HMONITOR hMonitor, HDC hdc, LPRECT rect, LPARAM data)
{
	monitors.push_back(hMonitor);
	return 1;
}



int main()
{
	EnumDisplayMonitors(NULL, NULL, registerMonitor, 0);

	std::cout << "number of visible screens: " << GetSystemMetrics(SM_CMONITORS) << "\n";
	std::cout << "number of screens: " << monitors.size() << "\n\n";

	int c = 0;
	for(auto i: monitors)
	{
		MONITORINFOEX info;
		info.cbSize = sizeof(info);
		if (!GetMonitorInfo(i, &info))
		{
			std::cout << "error getting some monitor data: " << GetLastError() << "\n";
		}else
		{
			std::cout << "Monitor: " << c << " " << info.szDevice << "\n";
			DISPLAY(info.rcMonitor.bottom) << " ";
			DISPLAY(info.rcMonitor.top) << " ";
			DISPLAY(info.rcMonitor.left) << " ";
			DISPLAY(info.rcMonitor.right) << "\n";

			DISPLAY(info.rcWork.bottom) << " ";
			DISPLAY(info.rcWork.top) << " ";
			DISPLAY(info.rcWork.left) << " ";
			DISPLAY(info.rcWork.right) << "\n";

			std::cout << ((info.dwFlags & MONITORINFOF_PRIMARY) ? "Primary Monitor" : "Non primary Monitor");

			std::cout << "\n\n";
		}

		c++;
	}


	bool pressed = 0;

	while (1)
	{

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