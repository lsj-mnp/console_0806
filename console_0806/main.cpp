#include "console_helper.h"

int main()
{
	console_helper a{ console_helper(40, 20, 0, 0) };

	a.SetCursorVisibility(false);

	int ch_x{}, ch_y{};

	while (true)
	{
		if (_kbhit() != 0)
		{
			int key{ _getch() };

			if (key == 27)
			{
				break;
			}

			if (key == 224)
			{
				key = _getch();

				if (key == 72)
				{
					if (ch_y > 0)
					{
						--ch_y;
					}
				}
				else if (key == 75)
				{
					if (ch_x > 0)
					{
						--ch_x;
					}
				}
				else if (key == 77)
				{
					if (ch_x < a.GetBufferWidth() - 1)
					{
						++ch_x;
					}
				}
				else if (key == 80)
				{
					if (ch_y < a.GetBufferHeight() - 1)
					{
						++ch_y;
					}
				}
			}
		}

		a.ClearBuffer(' ');

		a.SetCharacter(ch_x, ch_y, '@');

		a.PrintScreen();

		a.PrintBorderHorizontal('-', ch_x);
		a.PrintBorderVertical('|', ch_y);

		a.SetCursorPosition(43, 1);

		std::cout << "X: " << ch_x << " ";

		a.SetCursorPosition(43, 2);

		std::cout << "Y: " << ch_y << " ";
	}

	return 0;
}