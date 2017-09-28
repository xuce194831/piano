//////////////////////////////////////////////////////////////////
//
//  Copyright(C), 2013-2016, GEC Tech. Co., Ltd.
//
//  File name: piano/piano.c
//
//  Author: Vincent Lin (林世霖)  微信公众号：秘籍酷
//
//  Date: 2016-11
//  
//  Description: 音乐钢琴
//
//  程序调用接口：
//  ./piano 音量值[0-3] 0: 关闭音量
//                      1：默认音量
//                      2：大音量
//                      3：超大音量
//
//  GitHub: github.com/vincent040   Bug Report: 2437231462@qq.com
//
//////////////////////////////////////////////////////////////////

#include "bmp.h"
#include "ts.h"
#include "audio.h"
#include <signal.h>
#include <pthread.h>

char *FB = NULL;
char *reserve = NULL;
int  frm_size = 0;

void catch_sig(int sig)
{
	memcpy(FB, reserve, frm_size);
	exit(0);
}

char * init_lcd(struct fb_var_screeninfo *vinfo)
{
	int lcd = open(LCD, O_RDWR);

	bzero(vinfo, sizeof(struct fb_var_screeninfo));
	ioctl(lcd, FBIOGET_VSCREENINFO, vinfo);

	int frm_size = vinfo->xres * vinfo->yres * vinfo->bits_per_pixel/8;

	char *FB = mmap(NULL, frm_size,
			PROT_READ|PROT_WRITE,
			MAP_SHARED, lcd, 0);
	if(FB == MAP_FAILED)
	{
		perror("mmap LCD failed");
		exit(0);
	}

	return FB;
}

void usage(const char *prog)
{
	printf("Usage: %s volume[0-3]\n", prog);
}


// 点击了在琴键之外区域
bool out_of_range(struct coordinate *coor)
{
	if(coor->x < 10 || coor->x > 790 ||
	   coor->y < 47 || coor->y > 327)
	{
		return true;
	}

	return false;
}

// 点击了“关闭”按钮退出游戏
bool exit_the_game(struct coordinate *coor)
{
	return (coor->x > 700 && coor->x < 800 &&
		coor->y > 0   && coor->y < 47);
}

int main(int argc, char **argv)
{
	usage(argv[0]);

	if(argc == 2)
	{
		switch(atoi(argv[1]))	
		{
		case 0:
			vol =-175; break;
		case 1:
			vol = -15; break; // 这是默认的音量
		case 2:
			vol = 0;   break;
		case 3:
			vol = 10;  break;
		}
	}
	else
	{
		vol = -15;
	}

	signal(SIGINT, catch_sig);

	// 检查并配置ALSA库相关音频设备节点文件
	system("./bin/snd.sh");

	// 准备LCD, 获取LCD显存并保存当前画面
	struct fb_var_screeninfo vinfo;
	FB = init_lcd(&vinfo);

	frm_size = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel/8;
	reserve = calloc(1, frm_size);
	memcpy(reserve, FB, frm_size);

	// 显示背景
	bmp2lcd(BACKGROUND, FB, &vinfo, 0, 0);

	// 显示琴键
	int i;
	for(i=0; i<12; i++)
	{
		bmp2lcd(KEYOFF, FB, &vinfo, 65*i + 10, 47);
	}

	// 显示标题栏和钢琴logo
	bmp2lcd(BAR,  FB, &vinfo, 0, 0);
	bmp2lcd(LOGO, FB, &vinfo, 229, 355);

	// 准备触摸屏
	int ts = open(TOUCH_PANEL, O_RDONLY);

	pthread_t tid;
	struct coordinate coor;

	int new_pos=0, old_pos=0;
	bool first_time = true;

	while(1)
	{
		old_pos = new_pos;

		// 等待手指触碰琴键
		bool released = false;
		ts_trace(ts, &coor, &released);

#ifdef DEBUG
		fprintf(stderr, "\r(%d, %d)   ", coor.x, coor.y);
#endif

		// 点击了退出按钮或按了ctrl+c强制退出
		if(exit_the_game(&coor))
		{
			fprintf(stderr, "bye-bye!\n");
			break;
		}

		// 松手了，或者手指滑动到琴键之外
		// 则将琴键恢复弹起状态
		if(released || out_of_range(&coor))
		{
			for(i=0; i<12; i++)
			{
				bmp2lcd(KEYOFF, FB, &vinfo, 65*i + 10, 47);
			}
			first_time = true;
			continue;
		}

		new_pos = 10 + (coor.x-10)/65*65;

		if(new_pos != old_pos || first_time)
		{
			// 1，更新琴键状态
			bmp2lcd(KEYON, FB, &vinfo, new_pos, 47);

			if(!first_time)
			{
				bmp2lcd(KEYOFF, FB, &vinfo, old_pos, 47);
			}
			else
				first_time = false;	

			// 2，使用madplay播放恰当的琴音
			pthread_create(&tid, NULL, play_note, (void *)((coor.x-10)/65+1));
		}
	}

	// 恢复程序执行之前的系统界面，并释放相应资源
	memcpy(FB, reserve, frm_size);
	munmap(FB, frm_size);
	close(ts);

	pthread_exit(NULL);
}
