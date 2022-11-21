#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <io.h>
#include <fcntl.h>
#include <ctype.h>

#define SAVE_FILE "CASTLE"

void find_path(const char* data, int cur_room, unsigned char* path_dir)
{
	// 找出各房間離目的地的最短路徑方向 //
	for (;;)
	{
		// 找出各方間往目的地的方向 //
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
	// 顯示方向 //
	for (;;)
	{
		int dir = path_dir[cur_room];
		if (dir == 0xfe) break;
		if (dir == 0xff)
		{
			printf("無\n");
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
			printf("bad cur room\n");
			return;
		}
	}
	printf("\n");
}

//找物品
//輸入：data = 房間資料, item = 物品編號, cur_room = 玩家所在房間
void find_item(const char* data, int item, int cur_room)
{
	unsigned char path_dir[64];		// 各房間往目的地的方向,0=左,1=右,2=上,3=下,4=梯,5=櫥,0xFE=目的地,0xFF=尚未定義 //
	memset(path_dir,0xff,sizeof(path_dir));
	// 找到物品所在的房間 //
	for (int i=1; i<=60; ++i)
	{
		int pos = i*256+0x80;
		for (int j=0; j<8; ++j)
			if ((data[pos+j*0x10] == 0x30) && (data[pos+j*0x10+2] == item))
			{
				path_dir[i] = 0xfe;
				break;
			}
	}
	find_path(data,cur_room,path_dir);
}

//找出口
//輸入：data = 房間資料, cur_room = 玩家所在房間
void find_out(const char* data, int cur_room)
{
	unsigned char path_dir[64];		// 各房間往目的地的方向,0=左,1=右,2=上,3=下,4=梯,0xFE=目的地,0xFF=尚未定義 //
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
	find_path(data,cur_room,path_dir);
}

//輸入：data = 房間資料, cur_room = 玩家所在房間, has_plan = 是否已拿到計畫
void dump(const char* data, int cur_room, bool has_plan)
{
	if ((cur_room <= 0) || (cur_room > 60))
	{
		printf("bad cur room\n");
		return;
	}
	printf("目前所在房間: %d\n",cur_room);
	// 找物品: 作戰計畫 //
	if (!has_plan)
	{
		printf("作戰計畫: ");
		find_item(data,0x0F,cur_room);
	}
	// 找出口 //
	printf("出口: ");
	find_out(data,cur_room);
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
		read(file,data,61*256);
		close(file);
		int cur_room = data[0x40];
		bool has_plan = data[0x6c] != 0;
		dump(data,cur_room,has_plan);
	}
	system("pause");
}
