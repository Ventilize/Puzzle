/*  
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
请在第153,187行根据文件位置修改路径!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

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
int ifopen = 0;
int ifsave = 0;
typedef struct line {
	char linetext[100];
	struct line* next;
}line;
typedef struct  myNote {
	char mytitle[40];  //笔记名字
	char myclass;  //笔记类别
	struct myNote* next;
}Note;
Note* head = NULL, * tail = NULL;
int current_line = 0;
int line_count = 0;
int current_line_length = 0;
char text[MAX_ROWS][MAX_LINE + 1];
char notename[50][100];
char noteclass[50];
int allnotes = 0;

/*打开对话框*/
int FileDialogOpen(char* path);
/*保存对话框*/
int FileDialogSave(char* path);
/*打开文件*/
void openfile();
/*保存文件*/
void savefile();
/*保存用户笔记数据*/
void SaveUserData();
/*每次关闭程序时保存数据*/
void UpDataDelete();
/*每次打开程序时读入历史用户数据*/
void ReadUserData();





/*打开对话框*/
int FileDialogOpen(char* path)
{
	OPENFILENAME ofn; //  公共对话框结构
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn); // 结构大小
	ofn.lpstrFile = path; // 路径
	ofn.nMaxFile = MAX_PATH; // 路径大小
	ofn.lpstrFilter = "txt\0"; // 文件类型
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	return GetOpenFileName(&ofn); //成功1,失败0
}

/*保存对话框*/
int FileDialogSave(char* path)
{
	OPENFILENAME ofn; //  公共对话框结构
	TCHAR szFile[MAX_PATH]; //  保存获取文件名称的缓冲区
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn); // 结构大小
	ofn.lpstrFile = path; // 路径
	ofn.nMaxFile = MAX_PATH; // 路径大小
	ofn.lpstrFilter = "txt\0"; // 文件类型
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	return GetSaveFileName(&ofn);
}

/* 打开文件 */
void openfile()
{
	char filename[512] = { 0 };
	if (ifopen == 1)
	{
		if (FileDialogOpen(filename) == 0)
		{
			return;
		}
		else
		{
			/* 打开文件*/
			FILE* fp = fopen(filename, "r+");

			/* 设置字符串*/
			memset(text, 0, sizeof(text));
			line_count = 0;
			current_line = 0;
			while (fgets(text[line_count], 100, fp) != NULL) {
				line_count++;
				current_line++;
			}
			current_line_length = GetLine_length(text[current_line]);
			write_display();
			fclose(fp);
		}
	}
}

/*保存文件*/
void savefile()
{
	int n = 0;
	char* name;
	char filename[512] = { 0 };
	if (ifsave == 1)
	{
		if (FileDialogSave(filename) == 0)
		{
		}
	}
	name = strrchr(filename, 92) + 1;
	strcpy(notename[allnotes], name);
	noteclass[allnotes] = 'A';
	FILE* fp = fopen(notename[allnotes], "w");
	while (n <= line_count && fputs(text[n], fp) != EOF) {
		fputc('\n', fp);
		n++;
	}
	fclose(fp);
	allnotes++;
	InitConsole();
	int i = 0;
	for (i = 0; i < allnotes; i++) {
		printf("%s%c\n", notename[i], noteclass[i]);
	}
	write_display();
}

/*保存用户笔记数据*/
void SaveUserData() {
	FILE* fp;

	//请根据文件在电脑的位置 修改路径 ! ! !
	//替换 C:\\Users\\15473\\Desktop\\大作业.txt 即可 !
	fp = fopen("C:\\Users\\15473\\Desktop\\大作业.txt", "w+");
	
	int n = 0;
	while (n < allnotes) {
		if (fputs(notename[n], fp) == EOF)break;
		fputc(' ', fp);
		fputc(noteclass[n], fp);
		fputc('\n', fp);
		n++;
	}
	fclose(fp);
}

/*每次关闭程序时保存数据*/
void UpDataDelete()
{
	Note* p = head;
	int i;
	memset(notename, 0, sizeof(notename));
	memset(noteclass, 0, sizeof(noteclass));
	for (i = 0; p != NULL; p = p->next) {
		strcpy(notename[i], p->mytitle);
		noteclass[i] = p->myclass;
		i++;
	}
	if (i < allnotes)allnotes = i;
	return;
}

/*每次打开程序时读入历史用户数据*/
void ReadUserData()
{
	FILE* fp;

	//请根据文件在电脑的位置 修改路径 ! ! !
	//替换 C:\\Users\\15473\\Desktop\\大作业.txt 即可 !
	fp = fopen("C:\\Users\\15473\\Desktop\\大作业.txt", "r+");

	if (fp == NULL)exit(-1);
	char line[1024];
	int i = 0;
	while (fgets(line, sizeof(line), fp) != NULL) {
		// Remove newline character from the end of the line
		line[strcspn(line, "\n")] = '\0';

		// Extract filename and character from the line
		sscanf(line, "%s %c", notename[i], &noteclass[i]);
		i++;
		allnotes++;
	}
	fclose(fp);
}
