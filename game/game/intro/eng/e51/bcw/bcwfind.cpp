#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <io.h>
#include <fcntl.h>
#include <ctype.h>

#define SAVE_FILE "GAME.DAT"

const char* item_name[] = 
   {"無",
    "子彈",
    "金錢",
    "通行證",
    "火藥桶",
    "衣帽間",
    "藝術品",
    "鑰匙",
    "鑰匙",
    "日記",
    "警報系統",
    "匕首",
    "急救箱",
    "開鎖工具",
    "通道",
    "炸彈"};

void find_path(const char* data, int cur_room, unsigned char* path_dir)
{
	// 找出各房間離目的地最短路徑方向 //
	for (;;)
	{
		// 找出各方間往目的地的方向 //
		bool updated = false;
		for (int i=1; i<=63; ++i)
		{
			if (path_dir[i] != 0xff) continue;	// 已處理過 //
			int pos = (i-1)*256+0x49;
			for (int j=0; j<5; ++j)
			{
				int room = data[pos+j];
				if ((room <= 0) || (room > 63)) continue;
				if (path_dir[room] == 0xff) continue;
				path_dir[i] = j;
				updated = true;
				break;
			}
			pos = (i-1)*256+0x80;
			for (int j=0; j<8; ++j)
				if ((data[pos+j*0x10] == 0x20) && (data[pos+j*0x10+2] == 0xe))
				{
					int room = data[pos+j*0x10+10];
					if ((room <= 0) || (room > 63)) break;
					if (path_dir[room] == 0xff) break;
					path_dir[i] = 5;
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
		// 顯示該房間的櫥櫃物品 //
		/*
		int pos = (cur_room-1)*256+0x80;
		bool has_data = false;
		for (int j=0; j<8; ++j)
			if (data[pos+j*0x10] == 0x20)
			{
				int item = data[pos+j*0x10+2];
				if (item == 0) continue;
				if (item > 0xf)
				{
					printf("bad item");
					return;
				}
				if (has_data) printf(",");
				else printf("(");
				has_data = true;
				printf("%s",item_name[item]);
			}
		if (has_data) printf(")");
		*/
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
		case 5: printf("櫥"); break;
		}
		if (dir == 5)
		{
			int pos = (cur_room-1)*256+0x80;
			for (int j=0; j<8; ++j)
				if ((data[pos+j*0x10] == 0x20) && (data[pos+j*0x10+2] == 0xe))
				{
					cur_room = data[pos+j*0x10+10];
					break;
				}
		}
		else
		{
			int pos = (cur_room-1)*256+0x49;
			cur_room  = data[pos+dir];
		}
		if ((cur_room <= 0) || (cur_room > 63))
		{
			printf("bad room\n");
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
	for (int i=1; i<=63; ++i)
	{
		int pos = (i-1)*256+0x80;
		for (int j=0; j<8; ++j)
			if ((data[pos+j*0x10] == 0x20) && (data[pos+j*0x10+2] == item))
			{
				path_dir[i] = 0xfe;
				break;
			}
	}
	find_path(data,cur_room,path_dir);
}

//找會議室
//輸入：data = 房間資料, cur_room = 玩家所在房間
void find_room(const char* data, int cur_room)
{
	unsigned char path_dir[64];		// 各房間往目的地的方向,0=左,1=右,2=上,3=下,4=梯,5=櫥,0xFE=目的地,0xFF=尚未定義 //
	memset(path_dir,0xff,sizeof(path_dir));
	// 找到物品所在的房間 //
	for (int i=1; i<=63; ++i)
	{
		int pos = (i-1)*256+0x80;
		for (int j=0; j<8; ++j)
			if (data[pos+j*0x10] == 0x60)
			{
				path_dir[i] = 0xfe;
				break;
			}
	}
	find_path(data,cur_room,path_dir);
}

//找回程
//輸入：data = 房間資料, cur_room = 玩家所在房間
void find_back_path(const char* data, int cur_room)
{
	unsigned char path_dir[64];		// 各房間往目的地的方向,0=左,1=右,2=上,3=下,4=梯,5=櫥,0xFE=目的地,0xFF=尚未定義 //
	memset(path_dir,0xff,sizeof(path_dir));
	path_dir[1] = 0xfe;	// 回到房間#1 //
	find_path(data,cur_room,path_dir);
}

//輸入：data = 房間資料, cur_room = 玩家所在房間, has_boom = 是否已有炸彈
void dump(const char* data, int cur_room, bool has_boom)
{
	if ((cur_room <= 0) || (cur_room > 63))
	{
		printf("bad cur room\n");
		return;
	}
	printf("目前所在房間: %d\n",cur_room);
	// 找物品: 警報系統 //
	//printf("警報系統: ");
	//find_item(data,0x0A,cur_room);
	// 找物品: 定時炸彈 //
	if (!has_boom)
	{
		printf("炸彈: ");
		find_item(data,0x0F,cur_room);
	}
	// 找會議室 //
	printf("會議室: ");
	find_room(data,cur_room);
	// 找回程 //
	if (cur_room > 1)
	{
		printf("回程: ");
		find_back_path(data,cur_room);
	}
}

void main(void)
{
	// 讀取資料 //
	int file = open(SAVE_FILE,O_RDONLY | O_BINARY);
	if (file <= 0)
	{
		printf("無法開啟檔案: %s\n",SAVE_FILE);
		printf("請將本程式放在[德軍總部2]目錄下執行\n");
	}
	else
	{
		char data[16384];
		int start_pos = 0x14400;
		for (int i=0; i<4; ++i)
		{
			int pos = 0x14400 + i*0x2400;
			lseek(file,pos,SEEK_SET);
			read(file,data+i*0x1000,0x1000);
		}
		close(file);
		int cur_room = data[0x22];
		bool has_boom = data[4] != 0;
		dump(data+0xfc,cur_room,has_boom);
	}
	system("pause");
}
