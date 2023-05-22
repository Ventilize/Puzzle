#pragma once
#ifndef _Write_h
#define _Write_h
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

#endif
