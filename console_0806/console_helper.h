#pragma once
#include <iostream>
#include <Windows.h>
#include <conio.h>

constexpr int KBufferMaxWidth{ 80 };
constexpr int KBufferMaxHeight{ 50 };
constexpr int KBufferMaxByteSize{ (KBufferMaxWidth + 1) * KBufferMaxHeight };

class console_helper
{
public:
	console_helper(int buffer_width, int buffer_height, int offset_x, int offset_y) :
		m_BufferWidth{ buffer_width }, m_BufferHeight{ buffer_height },
		m_OffsetX{ offset_x }, m_OffsetY{ offset_y }
	{
		m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		CONSOLE_FONT_INFOEX cfi{};
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = 0;
		cfi.dwFontSize.Y = 18;
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;
		wcscpy_s(cfi.FaceName, L"Consolas");
		BOOL r = SetCurrentConsoleFontEx(m_hConsole, FALSE, &cfi);

		if (m_OffsetX == 0)
		{
			m_OffsetX = 1;
		}

		if (m_OffsetY == 0)
		{
			m_OffsetY = 1;
		}
	};

	~console_helper() {};

	void ClearBuffer(char ch)
	{
		for (int y = 0; y < m_BufferHeight; ++y)
		{
			memset(m_Buffer[y], ch, sizeof(char) * m_BufferWidth);
		}
	}

	void SetCursorVisibility(bool value)
	{
		CONSOLE_CURSOR_INFO cci{};
		cci.dwSize = sizeof(CONSOLE_CURSOR_INFO);
		cci.bVisible = value;
		
		SetConsoleCursorInfo(m_hConsole, &cci);
	}

	void SetCursorPosition(int x, int y)
	{
		static COORD coord{}; //static을 붙이면 life time이 전역변수와 같아짐. 다만 접근을 특정 범위에서만 할 수 있음.
		
		coord.X = x;
		coord.Y = y;

		SetConsoleCursorPosition(m_hConsole, coord);
	}

	void SetCharacter(int x, int y, char ch)
	{
		if ((x < 0) || (x >= m_BufferWidth))
		{
			return;
		}

		if ((y < 0) || (y >= m_BufferHeight))
		{
			return;
		}

		m_Buffer[y][x] = ch;
	}

	void PrintScreen()
	{
		SetCursorPosition(m_OffsetX, m_OffsetY);

		for (int y = 0; y < m_BufferHeight; ++y)
		{
			SetCursorPosition(m_OffsetX, m_OffsetY + y);

			std::cout << m_Buffer[y];
		}
	}

	void PrintBorderHorizontal(char ch, int mark_pos)
	{
		for (int x = 0; x < m_BufferWidth; ++x)
		{
			SetCursorPosition(m_OffsetX + x, m_OffsetY - 1);

			if (x == mark_pos)
			{
				std::cout << 'V';
			}
			else
			{
				std::cout << ch;
			}
			
		}

		for (int x = 0; x < m_BufferWidth; ++x)
		{
			SetCursorPosition(m_OffsetX + x, m_OffsetY + m_BufferHeight);

			std::cout << ch;
		}
	}

	void PrintBorderVertical(char ch, int mark_pos)
	{
		for (int y = 0; y < m_BufferHeight; ++y)
		{
			SetCursorPosition(m_OffsetX - 1, m_OffsetY + y);

			if (y == mark_pos)
			{
				std::cout << '>';
			}
			else
			{
				std::cout << ch;
			}
		}

		for (int y = 0; y < m_BufferHeight; ++y)
		{
			SetCursorPosition(m_OffsetX + m_BufferWidth, m_OffsetY + y);

			std::cout << ch;
		}
	}

	int GetBufferWidth()
	{
		return m_BufferWidth;
	}

	int GetBufferHeight()
	{
		return m_BufferHeight;
	}

private:
	HANDLE m_hConsole{};
	char m_Buffer[KBufferMaxHeight][KBufferMaxWidth + 1]{}; //앞의 대괄호는 높이. 뒤의 대괄호는 너비

	int m_BufferWidth{};
	int m_BufferHeight{};

	int m_OffsetX{};
	int m_OffsetY{};
};
