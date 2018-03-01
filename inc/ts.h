////////////////////////////////////////////////////
//
//  Copyright(C), 2013-2017, GEC Tech. Co., Ltd.
//
//  文件: piano/inc/ts.h
//  日期: 2017-9
//  描述: 触摸屏逻辑处理头文件
//
//  作者: Vincent Lin (林世霖)  微信公众号：秘籍酷
//
//  技术微店: http://weidian.com/?userid=260920190
//  技术交流: 260492823（QQ群）
//
////////////////////////////////////////////////////

#ifndef __TS_H_
#define __TS_H_

#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <pthread.h>

#ifdef GEC6818
#define TOUCH_PANEL "/dev/input/event0"
#elif  GEC210
#define TOUCH_PANEL "/dev/event0"
#endif

struct coordinate
{
	int x;
	int y;
};

void ts_trace(int ts, struct coordinate *coor, bool *released);

#endif
