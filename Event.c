#include <Windows.h>
#include <winuser.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"


#include"Class.h"
#include"File.h"
#include"UI.h"
#include"Write.h"
#include"Event.h"

#define MAX_LINE 100
#define MAX_ROWS 50
extern int current_line;
extern int current_line_length;
extern int line_count;
extern char text[MAX_ROWS][MAX_LINE + 1];
extern char notename[50][100];
extern char noteclass[50];
int reserveOneChar = 1;
double mouse_x = 0, mouse_y = 0; //鼠标位置
int flag = 0; //针对diaplay0
int classflag = 0; //用于进入具体类别 1->A  2->B  ...   6->F

/*用户键盘事件响应函数*/
void myKeyboardEvent(int key, int event);
/*用户字符事件响应函数*/
void myCharEvent(char ch);
/*用户鼠标事件响应函数*/
void myMouseEvent(int x, int y, int button, int event);




/*用户键盘事件响应函数*/
void myKeyboardEvent(int key, int event)
{
	char ch;
	if (event == KEY_UP) {

		switch (key) {
		case VK_LEFT:
			if (current_line_length == 0)break;
			ch = text[current_line][current_line_length - 1];
			if (ch > 0)current_line_length--;
			else if (ch < 0)current_line_length -= 2;
			break;
		case VK_UP:
			if (current_line > 0) {
				current_line--;
				current_line_length = strlen(text[current_line]);
				write_display();
			}
			break;

		case VK_RIGHT:
			if (text[current_line][current_line_length] == '\0') {
				text[current_line][current_line_length + 1] = '\0';
				text[current_line][current_line_length] = ' ';
			}
			current_line_length++;
			break;
		case VK_NEXT:
		case VK_DOWN:
			if (current_line < line_count) {
				current_line++;
				current_line_length = strlen(text[current_line]);
				write_display();
			}
			break;
		case VK_RETURN:
			if (InsertNewLine())
				write_display();
			break;
		case VK_BACK:
			if (current_line_length > 0) {
				int i = 1;
				if (text[current_line][current_line_length - 1] < 0)i = 2;
				while (i--)text[current_line][--current_line_length] = '\0';
				write_display();
			}
			else if (RemoveLine(current_line)) {
				if (current_line > 0)
					current_line--;
				current_line_length = strlen(text[current_line]);
				write_display();
			}
			break;
		}
	}
}

/*用户字符事件响应函数*/
void myCharEvent(char ch)
{
	int maxnum = MAX_LINE;
	uiGetChar(ch);

	switch (ch) {
	case VK_ESCAPE:
		exit(-1);
	default:
		/*在行的中间写入*/
		if (isPrintable(ch) && current_line_length < maxnum && text[current_line][current_line_length] != '\0') {
			int linelength = 0;
			int offset = 1;
			if (ch < 0) {
				offset = 2;
				maxnum -= reserveOneChar;
				reserveOneChar = 1 - reserveOneChar;
			}
			for (linelength = GetLine_length(text[current_line]) - 1; linelength >= current_line_length; linelength--) {
				if (reserveOneChar == 0 || !(ch < 0))text[current_line][linelength + offset] = text[current_line][linelength];
			}
			text[current_line][current_line_length] = ch;
			current_line_length++;
			//将字符加入到行尾
			//if (ch < 0) {
				// 当 ch<0, reserveOneChar=1, 表示汉字的第1个char
				// 当 ch<0, reserveOneChar=0, 表示汉字的第2个char
			//	maxnum -= reserveOneChar;
			//	reserveOneChar = 1 - reserveOneChar;
			//}
			if (reserveOneChar == 1)write_display();
		}

		/*在行的末尾写入*/
		else if (isPrintable(ch) && current_line_length < maxnum && text[current_line][current_line_length] == '\0') {
			if (ch < 0)  maxnum -= reserveOneChar;
			text[current_line][current_line_length++] = ch;
			//InitConsole();
			//printf("%d\n", current_line_length);
			//将字符加入到行尾
			text[current_line][current_line_length] = '\0';
			if (ch < 0)
				// 当 ch<0, reserveOneChar=1, 表示汉字的第1个char
				// 当 ch<0, reserveOneChar=0, 表示汉字的第2个char
				reserveOneChar = 1 - reserveOneChar;
			write_display();
		}
		break;
	}
}

/*用户鼠标事件响应函数*/
void myMouseEvent(int x, int y, int button, int event)
{
	mouse_x = ScaleXInches(x);
	mouse_y = ScaleYInches(y);
	uiGetMouse(x, y, button, event);
	DisplayStart();
	if (classflag != 0) {
		displayclassAll();
	}
	else if (flag == 1) {
		write_display();
	}
}
