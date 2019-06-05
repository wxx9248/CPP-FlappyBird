﻿// stdafx.hpp: 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 项目特定的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <Windows.h>
#include <conio.h>
#include <tchar.h>
#include <corecrt_wtime.h>
#include <atlimage.h>

// C++ 库头文件
#include <locale>
#include <string>
#include <vector>
#include <queue>
#include <random>

// C++ C兼容性头文件
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <ctime>
#include <cmath>

// C 库头文件


// EasyX 图形库
#include <easyx.h>
#include <graphics.h>


// 在此处引用程序需要的其他标头
#include "stdWCexception.hpp"


// 自定义工具函数

// 在字符串里匹配字符集
extern int strchrs(const char *str, const char *chrs, bool strictmode = false);

// 转换宽字符（使用new开辟内存，如不使用请用delete[]释放）
extern wchar_t *char2wchar(char *cstr);

// 清除std::queue队列
template<class T>
std::queue<T> &clearQueue(std::queue<T> &q)
{
	std::queue<T> empty;
	swap(empty, q);
	return q;
}
