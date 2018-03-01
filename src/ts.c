////////////////////////////////////////////////////
//
//  Copyright(C), 2013-2017, GEC Tech. Co., Ltd.
//
//  文件: piano/src/ts.c
//  日期: 2017-9
//  描述: 触摸屏逻辑处理
//
//  作者: Vincent Lin (林世霖)  微信公众号：秘籍酷
//
//  技术微店: http://weidian.com/?userid=260920190
//  技术交流: 260492823（QQ群）
//
////////////////////////////////////////////////////

#include "ts.h"

// 获取一对坐标，并且让手指松开的时候也能获取其状态
void ts_trace(int ts, struct coordinate *coor, bool *released)
{
	struct input_event buf;

	bool x_ready = false;
	bool y_ready = false;

	while(1)
	{
		bzero(&buf, sizeof(buf));
		read(ts, &buf, sizeof(buf));

#ifdef GEC6818
		if(buf.type == EV_KEY &&
		   buf.code == BTN_TOUCH &&
		   buf.value == 0)
#elif GEC210
		if(buf.type == EV_ABS &&
		   buf.code == ABS_PRESSURE &&
		   buf.value == 0)
#endif
		{
			*released = true;
			break;
		}


		if(buf.type != EV_ABS)
			continue;

		if(buf.type == EV_ABS &&
		   buf.code == ABS_X)
		{
			coor->x = buf.value;
			x_ready = true;
		}

		if(buf.type == EV_ABS &&
		   buf.code == ABS_Y)
		{
			coor->y = buf.value;
			y_ready = true;
		}

		if(x_ready && y_ready)
			break;
	}
}
