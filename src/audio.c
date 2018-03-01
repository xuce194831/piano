////////////////////////////////////////////////////
//
//  Copyright(C), 2013-2017, GEC Tech. Co., Ltd.
//
//  文件: piano/src/audio.c
//  日期: 2017-9
//  描述: 播放MP3线程
//
//  作者: Vincent Lin (林世霖)  微信公众号：秘籍酷
//
//  技术微店: http://weidian.com/?userid=260920190
//  技术交流: 260492823（QQ群）
//
////////////////////////////////////////////////////

#include "audio.h"

int vol = 0;

void *play_note(void *n)
{
	pthread_detach(pthread_self());

	char *cmd = calloc(1, 64);
	bzero(cmd, 64);

	// 拼凑恰当的mp3文件的名称
	sprintf(cmd, "%s %s/d%d.mp3 -a %d", MAD_PLAYER, NOTES_PATH, (int)n, vol);

	// 运行madplay来播放指定的mp3
	system("killall madplay");
	system(cmd);

	free(cmd);
}
