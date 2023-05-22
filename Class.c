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

extern char text[MAX_ROWS][MAX_LINE + 1];
typedef struct  myNote {
	char mytitle[40];  //笔记名字
	char myclass;  //笔记类别
	struct myNote* next;
}Note;
extern Note* head, * tail;
extern int classflag;
extern double winwidth, winheight;
char lei = 0;//取值为A~F
extern char notename[50][100];
extern char noteclass[50];
extern int allnotes;
extern int flag;
extern int line_count;


/*支持点击查看文件*/
void class_display();
/*画出6个类别*/
void drawClass();
/*进入某一类别*/
void displayclassAll();
/*查找笔记*/
int search(char* name);
/*创建链表*/
void LinkList();
/*笔记删除*/
void Delete(char* name);

void class_display()        
{
	/*菜单*/
	{
		SetFont("宋体");
		SetPointSize(15);
		SetStyle(1);
		static char* menuListBack[] = { "返回", "上一级", "退出" };
		static char* menuListFunc[] = { "功能", "查找", "删除" };
		setMenuColors("White", "Blue", "White", "black", 1);

		int selection = 0;
		double fH = GetFontHeight();
		double x = 0;
		double y = winheight;
		double h = fH * 1.8;
		double w = TextStringWidth(menuListBack[0]) * 3;
		drawMenuBar(x, y - h, 6 * w, h);//

		/*以下归属于"返回"*/
		selection = menuList(GenUIID(0), x, y - h, w, w, h, menuListBack, sizeof(menuListBack) / sizeof(menuListBack[0]));
		if (selection == 1)
		{
			/*返回上一级*/
			memset(text, 0, sizeof(text));
			flag = 0;
			DisplayStart();
		}
		if (selection == 2)
		{
			exit(-1);
		}

		/*查找和删除功能*/
		selection = menuList(GenUIID(0), x + w, y - h, w, w, h, menuListFunc, sizeof(menuListFunc) / sizeof(menuListFunc[0]));
		if (selection == 1)
		{
			int find;
			char name[100];
			/*查找*/
			InitConsole();
			printf("请输入需要查找的笔记\n");
			scanf("%s", name);
			find = search(name);
			FILE* fp = fopen(notename, "r+");
			memset(text, 0, sizeof(text));
			line_count = 0;
			while (fgets(text[line_count], 100, fp) != NULL)line_count++;
			flag = 1;
			write_display();
			fclose(fp);
		}
		if (selection == 2)
		{
			int find;
			char name[100];
			/*查找*/
			InitConsole();
			printf("请输入需要删除的笔记\n");
			scanf("%s", name);
			find = search(name);
			LinkList();
			Delete(notename[find]);
			UpDataDelete();
			SaveUserData();
		}
	}

	/*6类*/
	drawClass();//显示当前所有类别,包括默认分类和自定义分类
}

void drawClass()
{
	int thesize = GetPenSize();
	double chang = 1.5;
	double gao = 1.8;
	double xishushang = 0.55;
	double xishuxia = 0.20;

	SetPenSize(5);
	SetPointSize(30);

	setButtonColors("Blue", "Black", "Black", "Black", 0);
	if (button(GenUIID(0), 0.10 * winwidth, xishushang * winheight, chang, gao, "默认"))
	{
		SetPenSize(thesize);
		DisplayClear();
		classflag = 1;
	}
	setButtonColors("Green", "Black", "Black", "Black", 0);
	if (button(GenUIID(1), 0.40 * winwidth, xishushang * winheight, chang, gao, "线性代数"))
	{
		SetPenSize(thesize);
		DisplayClear();
		classflag = 2;
	}
	setButtonColors("Magenta", "Black", "Black", "Black", 0);
	if (button(GenUIID(2), 0.70 * winwidth, xishushang * winheight, chang, gao, "C语言"))
	{
		SetPenSize(thesize);
		classflag = 3;
		DisplayClear();
	}
	setButtonColors("Cyan", "Black", "Black", "Black", 0);
	if (button(GenUIID(0), 0.10 * winwidth, xishuxia * winheight, chang, gao, "离散数学"))
	{
		SetPenSize(thesize);
		classflag = 4;
		DisplayClear();
	}
	setButtonColors("Gray", "Black", "Black", "Black", 0);
	if (button(GenUIID(1), 0.40 * winwidth, xishuxia * winheight, chang, gao, "英语"))
	{
		SetPenSize(thesize);
		classflag = 5;
		DisplayClear();
	}
	setButtonColors("Yellow", "Black", "Black", "Black", 0);
	if (button(GenUIID(2), 0.70 * winwidth, xishuxia * winheight, chang, gao, "通识"))
	{
		SetPenSize(thesize);
		classflag = 6;
		DisplayClear();
	}

	SetPenSize(1);
	SetPointSize(20);
	setTextBoxColors("Blue", "Black", "Black", "Black", 0);
	textbox(GenUIID(0), 0.1 * winwidth, xishushang * winheight - 0.5, chang, gao * 0.25, "默认分类", 20);

	setButtonColors("Blue", "Black", "Red", "Red", 0);
}

/*进入某一类别*/
void displayclassAll()
{
	FILE* fp = NULL;
	DisplayClear();
	/*菜单*/
	{
		SetFont("宋体");
		SetPointSize(15);
		SetStyle(1);
		static char* menuListBack[] = { "返回", "上一级", "退出" };
		setMenuColors("White", "Blue", "White", "Black", 1);

		int selection = 0;
		double fH = GetFontHeight();
		double x = 0;
		double y = winheight;
		double h = fH * 1.8; //菜单的高度
		double w = TextStringWidth(menuListBack[0]) * 3; //是"返回"宽度的3倍
		drawMenuBar(x, y - h, 6 * w, h);

		//以下归属于"返回"
		selection = menuList(GenUIID(0), x, y - h, w, w, h, menuListBack, sizeof(menuListBack) / sizeof(menuListBack[0]));
		if (selection == 1)
		{
			classflag = 0;
			class_display();
		}
		if (selection == 2)
		{
			exit(-1);
		}
	}

	if (classflag == 1) {
		lei = 'A';
	}
	else if (classflag == 2) {
		lei = 'B';
	}
	else if (classflag == 3) {
		lei = 'C';
	}
	else if (classflag == 4) {
		lei = 'D';
	}
	else if (classflag == 5) {
		lei = 'E';
	}
	else if (classflag == 6) {
		lei = 'F';
	}

	int k;/*遍历变量*/
	int pos;/*位置编号*/
	for (k = 0, pos = 0; k < allnotes; k++, pos++)
	{
		SetPointSize(25);
		if (noteclass[k] == lei)
		{
			if (pos < 3) {
				if (button(GenUIID(pos), 0.1 * winwidth + pos * 0.2 * winwidth, 0.55 * winheight, 1.5, 1.8, notename[k]))
				{
					fp = fopen(notename[k], "r + ");
					memset(text, 0, sizeof(text));
					line_count = 0;
					while (fgets(text[line_count], 100, fp) != NULL)line_count++;
					flag = 1;
					classflag = 0;
					write_display();
				}
			}
			else if (pos >= 3 && pos < 6) {
				if (button(GenUIID(pos), 0.1 * winwidth + (pos - 3) * 0.2 * winwidth, 0.35 * winheight, 1.5, 1.8, notename[k]))
				{
					fp = fopen(notename[k], "r + ");
					memset(text, 0, sizeof(text));
					line_count = 0;
					while (fgets(text[line_count], 100, fp) != NULL)line_count++;
					flag = 1;
					classflag = 0;
					write_display();
				}
			}
			else if (k >= 6 && k < 9) {
				if (button(GenUIID(pos), 0.1 * winwidth + (pos - 6) * 0.3 * winwidth, 0.15 * winheight, 1.5, 1.8, notename[k]))
				{
					fp = fopen(notename[k], "r + ");
					memset(text, 0, sizeof(text));
					line_count = 0;
					while (fgets(text[line_count], 100, fp) != NULL)line_count++;
					flag = 1;
					classflag = 0;
					write_display();
				}
			}
		}
	}
}

/*查找笔记*/
int search(char* name)
{
	int find;
	for (find = 0; find < allnotes; find++) {
		if (strcmp(notename[find], name) == 0)return find;
	}
	InitConsole();
	printf("不存在该笔记");
	exit(-1);
}

/*创建链表*/
void LinkList() {
	int i;
	for (i = 0; i < allnotes; i++) {
		Note* p = (Note*)malloc(sizeof(Note*));
		strcpy(p->mytitle, notename[i]);
		p->myclass = noteclass[i];
		if (head == NULL)tail = head = p;
		else {
			tail->next = p;
			tail = p;
		}
	}
	tail->next = NULL;
}

/*笔记删除*/
void Delete(char* name) 
{
	if (head == NULL)return;
	Note* p = head;
	if (strcmp(head->mytitle, name) == 0) {
		head = head->next;
		InitConsole();
		printf("删除成功");
		return;
	}
	while (p->next != NULL) {
		if (strcmp(p->next->mytitle, name) == 0) {
			p->next = p->next->next;
			InitConsole();
			printf("删除成功");
			return;
		}
		if (p->next == NULL) {
			break;
		}
		p = p->next;
	}
	return;
}
