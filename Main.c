#include <Windows.h>
#include <winuser.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "UI.h"
#include"File.h"
#include"Event.h"
#include"Write.h"
#include"Class.h"

#define MAX_LINE 100
#define MAX_ROWS 50

extern char text[MAX_ROWS][MAX_LINE + 1];
extern char notename[50][100];
extern char noteclass[50];
extern int allnotes ;

typedef struct  myNote {
	char mytitle[40];  //笔记名字
	char myclass;  //笔记类别
	struct myNote* next;
}Note;
extern Note* head , * tail;
int line_length_count;/*一行直到'\0'的长度*/

typedef struct line {
	char linetext[100];
	struct line* next;
}line;


extern double winheight, winwidth; //最大窗口尺寸
extern int flag;//针对diaplay0
extern int current_line ;
extern int current_line_length ;
extern int line_count;
extern int ifopen;
extern int ifsave;
extern double mouse_x , mouse_y ; //鼠标位置
extern int reserveOneChar ;
extern int classflag ; //用于进入具体类别 1->A  2->B  ...   6->F
extern int ifpress ;  //用于是否显示说明
extern double text_x, text_y, text_height;
extern char lei;//取值为A~F
int allclasses = 6; //所有类别的数量

char yclass;  


void Main()
{
	ReadUserData();//创建默认笔记
	SetWindowTitle("nia做笔记");
	SetWindowSize(10, 10);
	winheight = 10.0;
	winwidth = 10.0;
	InitGraphics();
	text_height = GetFontHeight() * 1.5;
	text_x = 0.2;
	text_y = GetWindowHeight() / 8 * 7;
	registerCharEvent(myCharEvent);
	registerKeyboardEvent(myKeyboardEvent);
	registerMouseEvent(myMouseEvent);

	DrawHeadArea();
	DrawNotebooks();
}
