#include <Windows.h>
#include <winuser.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include"UI.h"
#include"Event.h"
double winheight, winwidth; //最大窗口尺寸
int ifpress = 0; //用于是否显示说明
extern flag, classflag;


/*整个程序的第一层UI*/
void DisplayStart();
/*输出“欢迎使用nia笔记本”*/
void DrawHeadArea();
/*画出笔记本和说明*/
void DrawNotebooks();





/*输出“欢迎使用nia笔记本”*/
void DrawHeadArea()
{
	DefineColor("mycolor", 0.90196, 0.90196, 0.90196);
	SetPenColor("mycolor");
	drawRectangle(0, winheight - 0.3125 * 4, winwidth, 3 * 0.3125, 1);
	SetPenColor("Black");
	SetFont("楷体");
	SetPointSize(40);
	drawLabel(0.33 * winwidth, winheight - 2 * 0.3125 - 0.3, "欢迎使用nia笔记本");
}

void DisplayStart()
{
	DisplayClear();

	if (flag == 0)
	{
		DrawNotebooks();
		DrawHeadArea();
	}
	if (flag == 1) //新建笔记
	{
		write_display();
	}
	if (flag == 2 && classflag == 0) //查看笔记
	{
		class_display(); //直接列出6类
	}
	if (flag == 3)
	{
		int size = GetPointSize();
		if (ifpress)
		{
			DefineColor("mycolor", 0.62, 0.85, 0.78);
			SetPenColor("mycolor");
			drawRectangle(0.10 * winwidth, 0.02 * winheight, 0.80 * winwidth, 0.19 * winheight, 1);
			SetPenColor("Black");
			SetPointSize(20);
			drawLabel(0.15 * winwidth, 0.18 * winheight, "1.请在第一次使用前修改笔记数据的打开路径!具体修改位置需要进入File.c中;");
			drawLabel(0.15 * winwidth, 0.15 * winheight, "2.请不要在保存的进程中退出;");
			drawLabel(0.15 * winwidth, 0.12 * winheight, "3.保存文件时,请在文件命名添加后缀.txt;");
			drawLabel(0.15 * winwidth, 0.09 * winheight, "4.请不要关闭笔记分类时打开的控制台;");
			drawLabel(0.15 * winwidth, 0.06 * winheight, "5.笔记窗口可能会把控制台挡住哦;");
			drawLabel(0.15 * winwidth, 0.03 * winheight, "6.该系统暂时支持6类,每类最多9个笔记;");

		}
		SetPointSize(size);
		DrawNotebooks();
		DrawHeadArea();
	}
}

/*画出笔记本和说明*/
void DrawNotebooks()
{
	double w = 2.0;
	double h = 2.6;
	if (button(GenUIID(0), 0.20 * winwidth, 0.33 * winheight, w, h, "新建笔记")) {
		flag = 1;
		DisplayClear();
	}
	if (button(GenUIID(0), 0.60 * winwidth, 0.33 * winheight, w, h, "查看笔记")) {
		flag = 2;
		DisplayClear();
	}
	if (button(GenUIID(0), 0.33 * winwidth, 0.2 * winheight, 0.33 * winwidth, 0.08 * winheight, ifpress ? "关闭说明" : "使用说明"))
	{
		ifpress = !ifpress;
		flag = 3;
	}
}
