#include <windows.h>
#include <iostream>
#include <chrono>

using namespace std::chrono_literals;

int main()
{
	bool pressed = 0;

	while (1)
	{

		if(pressed == 0 && GetAsyncKeyState(VK_NUMPAD7))
		{
			pressed = 1;
			SetCursorPos(100, 100);
		}
		else
		{
			if(!GetAsyncKeyState(VK_NUMPAD7))
			{
				pressed = 0;
			}
		}

	}

}