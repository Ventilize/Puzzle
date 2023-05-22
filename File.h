#pragma once
#ifndef _File_h
#define _File_h
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

#endif
