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

double text_x, text_y, text_height;
extern char text[MAX_ROWS][MAX_LINE + 1];
extern int line_count;
extern double winheight;
extern int ifsave;
extern int ifopen;
extern char notename[50][100];
extern char noteclass[50];
extern flag;
extern allnotes;
extern int current_line;
extern int current_line_length;

/*笔记的打开保存编辑分析*/
void write_display();
/*菜单*/
void DrawMenu();
/*数据分析*/
void DataAnalyze(int mod);
/*获取行长度*/
int GetLine_length(char* a);
/*是否能输入*/
int isPrintable(int ch);
/*插入新的一行*/
int InsertNewLine();
/*移动行*/
int RemoveLine(int k);




/*笔记的打开保存编辑分析*/
void write_display() { 
	DisplayClear();
	SetPenColor("Black");
	int y;
	double py = text_y;
	double d = GetFontHeight();
	for (y = 0; y < line_count || y == 0; y++) {
		py = text_y - text_height * y;
		MovePen(0, py);
		DrawTextString(text[y]);
	}
	MovePen(0, py -= text_height);
	DrawTextString(text[y]);
	//if (y == 1)py += text_height;
	MovePen(TextStringWidth(text[y]), py - 0.04);
	SetPenColor("Blue");
	DrawLine(0, 0.2);
	DrawMenu(); //画菜单
}

void DrawMenu() {
	SetFont("宋体");
	SetPointSize(15);
	SetStyle(1);
	static char* menuListFile[] = { "文件", "保存", "分类", "打开" };
	static char* menuListBack[] = { "返回", "上一级", "退出" };
	static char* menuListAnalyze[] = { "数据分析", "最努力项", "总数统计" };
	setMenuColors("White", "Blue", "White", "black", 1);//白底黑字,当鼠标移上去的时候,字变成蓝色
	
	int selection = 0;
	double fH = GetFontHeight(); //这是当前字体的高度
	//左上角排列菜单
	double x = 0;//说明菜单从最左边开始
	double y = winheight;//说明菜单从最上面开始
	double h = fH * 1.8;//菜单的高度
	double w = TextStringWidth(menuListFile[0]) * 3; //是"文件"宽度的3倍
	drawMenuBar(x, y - h, 6 * w, h);

	/*以下归属于"文件"*/
	selection = menuList(GenUIID(0), x, y - h, w, w, h, menuListFile, sizeof(menuListFile) / sizeof(menuListFile[0]));
	if (selection == 1)
	{
		/*保存文件*/
		ifsave = 1;
		savefile();
		ifsave = 0;
		SaveUserData();
	}
	if (selection == 2)
	{
		/*笔记分类*/
		char title[40];
		int find = 0;//找到的笔记序号
		InitConsole();
		system("mode con cols=30 lines=10  "); //设置大小
		printf("要分类笔记的名称为:\n");
		scanf("%s", title);
		find = search(title);
		getchar();
		printf("该笔记的类别为:\n请输入A~F之间的大写字母\n");
		noteclass[find] = getchar();
		if (!(noteclass[find] >= 'A' && noteclass[find] <= 'F')) {
			printf("您输入的类别不正确!\n");
			exit(-1);
		}
		SaveUserData();
	}
	if (selection == 3)
	{
		ifopen = 1;
		openfile();
		ifopen = 0;
	}

	/*以下归属"返回"*/
	selection = menuList(GenUIID(0), x + w, y - h, w, w, h, menuListBack, sizeof(menuListBack) / sizeof(menuListBack[0]));
	if (selection == 1)
	{
		/*返回上一级*/
		memset(text, 0, sizeof(text));
		flag = 0;
		DisplayStart();
	}
	if (selection == 2) {
		/*直接退出,作用和 × 一样*/
		exit(-1);
	}

	/*以下为数据分析功能*/
	selection = menuList(GenUIID(0), x + 2 * w, y - h, w, w, h, menuListAnalyze, sizeof(menuListAnalyze) / sizeof(menuListAnalyze[0]));
	if (selection == 1)
	{
		DataAnalyze(1);
	}
	if (selection == 2) {
		DataAnalyze(2);
	}
}

/*数据分析*/
void DataAnalyze(int mod) {
	int i;
	int num[6] = { 0 };
	for (i = 0; i < allnotes; i++) {
		switch (noteclass[i]) {
		case 'A':
			num[0]++;
			break;
		case 'B':
			num[1]++;
			break;
		case 'C':
			num[2]++;
			break;
		case 'D':
			num[3]++;
			break;
		case 'E':
			num[4]++;
			break;
		case 'F':
			num[5]++;
			break;
		}
	}
	/*记得最多的*/
	if (mod == 1) {
		int max, mn = 0;/*mn是对应序号*/
		max = num[0];
		for (i = 0; i < 5; i++) {
			if (num[i] < num[i + 1]) {
				max = num[i + 1];
				mn = i + 1;
			}
		}
		char favor[20];
		switch (mn) {
		case 0:
			strcpy(favor, "默认");
			break;
		case 1:
			strcpy(favor, "线性代数");
			break;
		case 2:
			strcpy(favor, "C语言");
			break;
		case 3:
			strcpy(favor, "离散数学");
			break;
		case 4:
			strcpy(favor, "英语");
			break;
		case 5:
			strcpy(favor, "通识");
			break;
		}
		InitConsole();
		printf("你最爱作的笔记是——%s\n一共记录了%d本", favor, max);
	}
	else if (mod == 2) {
		FreeConsole();
		InitConsole();
		printf("你一共做了%d本笔记", allnotes);
	}
}

/*获取行长度*/
int GetLine_length(char* a) {
	int count;
	for (count = 0; a[count] != '\0'; count++);
	return count;
}

/*是否能输入*/
int isPrintable(int ch)
{
	return ch >= VK_SPACE && ch < 127 || ch < 0;
}


/*插入新的一行*/
int InsertNewLine()
{
	int y;
	if (line_count == MAX_ROWS)
		return 0;
	for (y = line_count; y > current_line + 1; y--)
		strcpy(text[y], text[y - 1]);
	line_count++;
	current_line++;
	text[current_line][0] = '\0';
	current_line_length = 0;
	return 1;
}

/*移动行*/
int RemoveLine(int k)
{
	int y;
	if (line_count < 1)
		return 0;
	for (y = k; y < line_count - 1; y++)
		strcpy(text[y], text[y + 1]);
	line_count--;
	return 1;
}
