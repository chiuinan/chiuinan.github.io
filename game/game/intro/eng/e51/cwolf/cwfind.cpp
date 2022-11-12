#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <io.h>
#include <fcntl.h>
#include <ctype.h>

#define SAVE_FILE "CASTLE"

int get_current_room(char* data)
{
	return (unsigned char)data[0x40];
}

void dump_path(char* data)
{
	unsigned char path_dir[64];		// 各房間往出口的方向,0=左,1=右,2=上,3=下,4=梯,0xFF=尚未定義 //
	memset(path_dir,0xff,sizeof(path_dir));
	// 找到出口前的房間 //
	for (int i=1; i<=60; ++i)
	{
		int pos = i*256+0x49;
		for (int j=0; j<5; ++j)
			if ((unsigned char)data[pos+j] > 60)
			{
				path_dir[i] = j;
				break;
			}
	}
	// 找出各房間離出口的最短路徑方向 //
	for (;;)
	{
		// 找出各方間往出口的方向 //
		bool updated = false;
		for (int i=1; i<=60; ++i)
		{
			if (path_dir[i] != 0xff) continue;	// 已處理過 //
			int pos = i*256+0x49;
			for (int j=0; j<5; ++j)
			{
				int room = data[pos+j];
				if ((room <= 0) || (room > 60)) continue;
				if (path_dir[room] == 0xff) continue;
				path_dir[i] = j;
				updated = true;
				break;
			}
		}
		if (!updated) break;
	}
	// 取得目前玩家所在的房間編號 //
	int cur_room = get_current_room(data);
	if ((cur_room <= 0) || (cur_room > 60))
	{
		printf("bad cur room value\n");
		return;
	}
	printf("目前所在房間：#%d\n",cur_room);
	printf("逃脫方向：");
	// 顯示方向 //
	for (;;)
	{
		int dir = path_dir[cur_room];
		if (dir == 0xff)
		{
			printf("bad path dir (no exit)\n");
			return;
		}
		switch(dir)
		{
		case 0: printf("左"); break;
		case 1: printf("右"); break;
		case 2: printf("上"); break;
		case 3: printf("下"); break;
		case 4: printf("梯"); break;
		}
		int pos = cur_room*256+0x49;
		int next_room = data[pos+dir];
		if (next_room > 60) break;
		cur_room = next_room;
		if ((cur_room <= 0) || (cur_room > 60))
		{
			printf("bad cur room value\n");
			return;
		}
	}
	printf("\n");
}

void main(void)
{
	// 讀取資料 //
	int file = open(SAVE_FILE,O_RDONLY | O_BINARY);
	if (file <= 0)
	{
		printf("無法開啟檔案: %s\n",SAVE_FILE);
		printf("請將本程式放在[德軍總部]目錄下執行\n");
	}
	else
	{
		char data[16384];
		read(file,data,63*256);
		close(file);
		dump_path(data);
	}
	system("pause");
}
