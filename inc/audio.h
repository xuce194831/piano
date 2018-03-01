////////////////////////////////////////////////////
//
//  Copyright(C), 2013-2017, GEC Tech. Co., Ltd.
//
//  文件: piano/src/audio.h
//  日期: 2017-9
//  描述: 音频数据处理头文件
//
//  作者: Vincent Lin (林世霖)  微信公众号：秘籍酷
//
//  技术微店: http://weidian.com/?userid=260920190
//  技术交流: 260492823（QQ群）
//
////////////////////////////////////////////////////

#ifndef __AUDIO_H
#define __AUDIO_H

#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <pthread.h>

#define MAD_PLAYER "./bin/madplay"
#define NOTES_PATH "./res/notes"

extern int vol;

void *play_note(void *arg);

#endif
