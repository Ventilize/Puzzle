#pragma once
#ifndef _Event_h
#define _Event_h
/*用户键盘事件响应函数*/
void myKeyboardEvent(int key, int event);
/*用户字符事件响应函数*/
void myCharEvent(char ch);
/*用户鼠标事件响应函数*/
void myMouseEvent(int x, int y, int button, int event);
#endif
