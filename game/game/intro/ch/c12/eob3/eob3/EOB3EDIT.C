#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <io.h>
#include <fcntl.h>
#include <ctype.h>

#define SAVE_FILE "SAVEGAME\\ITEMS_%02d.BIN"
#define DIR_FILE "SAVEGAME\\SAVEGAME.DIR"
#define STRING_FILE "EYE.RES"

#define RACE_NO 16 /* 種族數 */
static char* Race_Name[RACE_NO];

#define CLASS_NAME_NO 6 /* 職業名稱數 */
static char* Class_Name[CLASS_NAME_NO];
#define CLASS_NO 15 /* 職業數 */
static int Class_Combine[CLASS_NO] = {0,1,2,3,4,5,4*16,5*16,3*16,3*16+5*256,5+3*16,4+5*16,4*16+3*256,1+4*16,4+3*16};

#define MAGIC_NO 88 /* 法術數目 */
static char* Magic_Name[MAGIC_NO];

#define ITEM_NO 166 /* 物品數目 */
static char* Item_Name[ITEM_NO];

static unsigned char Item_Name_Index[] =
{0x06,0x0E,0x0D,0x08,0x11,0x09,0x0B,0x15,0x6D,0x7B,0x88,0x51,0x14,0x57,0x47,0x12,
 0x4D,0x25,0x17,0x13,0x20,0x2E,0x02,0x01,0x35,0x54,0x4F,0x2A,0x83,0x1B,0x19,0x78,
 0x2D,0x71,0x36,0x37,0x38,0x3D,0x39,0x3A,0x3B,0x3C,0x2B,0x29,0x28,0x27,0x00,0x00,
 0x00,0x00,0x00,0x00,0x01,0x3E,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1E,0x00,0x00,0x03,0x00,0x00,
 0x22,0x00,0x00,0x00,0x00,0x2F,0x00,0x00,0x30,0x00,0x00,0x31,0x00,0x00,0x33,0x00,
 0x00,0x34,0x00,0x00,0x00,0x00,0x26,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2C,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x1C,0x00,0x00,0x00,0x00,0x21,0x00,0x00,0x5F,0x00,0x00,0x64,0x00,0x00,0x7D,0x00,
 0x00,0x7E,0x00,0x00,0x4A,0x00,0x00,0x72,0x00,0x00,0x23,0x00,0x00,0x24,0x00,0x00,
 0x99,0x00,0x00,0x94,0x00,0x00,0x00,0x00,0x00,0x91,0x00,0x00,0x95,0x00,0x00,0x52,
 0x00,0x00,0x53,0x00,0x00,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x85,0x00,0x00,0x9F,0x00,0x00,0x04,
 0x00,0x00,0x05,0x00,0x00,0x73,0x00,0x00,0x80,0x00,0x00,0x74,0x00,0x00,0x45,0x00,
 0x00,0x6E,0x00,0x00,0x93,0x00,0x00,0x96,0x00,0x00,0x92,0x00,0x00,0x98,0x00,0x00,
 0x00,0x00,0x00,0x8A,0x00,0x00,0x9A,0x00,0x00,0x8B,0x00,0x00,0x8F,0x00,0x00,0x9B,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x82,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x97,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x5D,0x00,0x00,0x00,0x00,0x5E,0x00,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,
 0x9C,0x00,0x00,0x9D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x41,0x00,0x00,0x42,
 0x00,0x00,0x00,0x00,0x00,0x00,0x9E,0x00,0x00,0x1F,0x00,0x00,0x00,0x00,0x00,0x46,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,
 0x00,0x00,0x00,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,0x55,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x65,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x68,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18};

static char Item_Type[] =
{ 1, 2, 0,13,16,14, 3, 8, 9, 7,15,12,10,11,22,21,
 24,19,23,20,18,36,29,30,37,26,17,28,25,27, 4, 8,
 17,47,38,38,38,38,38,38,38,38,34,34,34,34,-1,-1,
 -1,-1,-1,-1,30,33,31,-1,-1,-1,-1,-1,-1,-1,-1,-1,
 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,30,-1,-1,42,-1,-1,
 32,-1,-1,-1,-1,42,-1,-1,42,-1,-1,43,-1,-1,42,-1,
 -1,42,-1,-1,-1,-1,34,-1,-1,-1,-1,-1,-1,-1,-1,42,
 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
 37,-1,-1,-1,-1,37,-1,-1,42,-1,-1,42,-1,-1,42,-1,
 -1,42,-1,-1,42,-1,-1, 6,-1,-1,44,-1,-1,44,-1,-1,
 40,-1,-1,40,-1,-1,-1,-1,-1,40,-1,-1,40,-1,-1,42,
 -1,-1,42,-1,-1,42,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
 -1,-1,-1,-1,-1,-1,-1,-1,-1,42,-1,-1,42,-1,-1,39,
 -1,-1,39,-1,-1,39,-1,-1,39,-1,-1,39,-1,-1,39,-1,
 -1,39,-1,-1,39,-1,-1,39,-1,-1,39,-1,-1,39,-1,-1,
 -1,-1,-1,39,-1,-1,45,-1,-1,45,-1,-1,45,-1,-1,43,
 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
 -1,-1,-1,-1,-1,-1,-1,46,-1,-1,-1,-1,-1,-1,-1,-1,
 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
 -1,-1,-1,-1,-1,-1,-1,16,-1,-1,-1,-1,-1,-1,-1,-1,
 -1,-1,-1,-1,-1,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
 -1,-1, 7,-1,-1,-1,-1, 7,-1,-1,48,-1,-1,-1,-1,-1,
 13,-1,-1,13,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
 -1,-1,-1,48,-1,-1,-1,-1,-1,-1,-1,-1, 0,-1,-1, 6,
 -1,-1,-1,-1,-1,-1, 9,-1,-1,47,-1,-1,-1,-1,-1,47,
 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,41,
 -1,-1,-1,-1, 3,-1,-1,-1,-1,-1, 5,-1,-1,-1,-1,-1,
 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 4,
 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
 -1,-1,-1,-1,21,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,18,-1,-1,-1,
 -1,-1,-1,-1,-1,-1,-1,-1,22};

static const char* Item_Type_Name[] =
{"短劍","斧頭","長劍","三叉戟","雙手劍","戟","無用物品","棍杖","狼牙棒","鏈枷",
 "箭","石塊","飛鏢","匕首","叉矛","投石索","弓","長袍","盾","皮甲",
 "鱗甲","鎖子甲","板條甲","鎧甲","頭盔","防護腕帶","鞋子","裝飾的項鍊","保護戒指","魔法書",
 "聖符","卷軸","法術頭盔","魔杖","法術戒指","","開鎖器","糧食","藥水","鑰匙",
 "寶石","棍棒","特殊物品","施法物品","法術手套","文件","施法項鍊","飛鎚","法術暫時產生物"};

static char Item_Type_Catalog[] = /* 0=不處理, 1=近戰武器, 2=投射武器, 3=防具, 4=無作用防具, 5=施法物品, 6=施法物品(選法術), 7=一般物品 */
{1,1,1,1,1,1,0,1,1,1,
 2,2,2,2,2,2,2,3,3,3,
 3,3,3,3,3,3,4,4,3,5,
 5,6,5,6,5,0,7,7,7,7,
 7,1,0,5,5,0,6,2,0};

static char Mage_Magic_Index[] =
{0x00,0x01,0x02,0x03,0x04,0x05,-1,-1,-1,-1,
 0x06,0x07,0x08,0x09,0x0A,0x0B,-1,-1,-1,-1,
 0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,0x14,0x15,
 0x16,0x17,0x18,0x19,-1,-1,-1,-1,-1,-1,
 0x1A,0x1B,0x1C,-1,-1,-1,-1,-1,-1,-1,
 0x1D,0x1E,0x1F,0x20,0x21,-1,-1,-1,-1,-1,
 0x22,0x23,0x24,0x25,-1,-1,-1,-1,-1,-1,
 0x26,0x27,-1,-1,-1,-1,-1,-1,-1,-1,
 0x28,0x29,0x2A,0x2B};

static char Cleric_Magic_Index[] =
{0x2C,0x2D,0x2E,0x2F,0x30,0x31,0x32,-1,-1,-1,
 0x33,0x34,0x35,0x36,0x37,0x38,0x39,-1,-1,-1,
 0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,0x40,0x41,-1,-1,
 0x42,0x43,0x44,0x45,0x46,0x47,0x48,-1,-1,-1,
 0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,-1,-1,-1,-1,
 0x50,0x51,0x52,0x53,-1,-1,-1,-1,-1,-1,
 0x54,0x55};

#define MAX_DATA_INDEX 999
static char* File_Data[MAX_DATA_INDEX+1];

#ifdef WIN32 /* VC++ */

#include "windows.h"

void clrscr(void)
{
   int i;
   for (i=0; i<20; ++i) printf("\n");
}

const char* convert(const char* s)
{
   static char s_outs[2][256];
   static int out_index=0;
   wchar_t ws[256];
   out_index = (out_index+1) & 0x01;
   char* outs = s_outs[out_index];
   memset(ws,0,256*2);
   if (!MultiByteToWideChar(950,MB_PRECOMPOSED,s,strlen(s),ws,255)) return s;
   memset(outs,0,256);
   WideCharToMultiByte(CP_THREAD_ACP,0,ws,wcslen(ws),outs,255,NULL,NULL);
   return outs;
}

#else /* Turbo C */

#define bool  int
#define true  1
#define false 0
const char* convert(const char* s) { return s; }

#endif

bool load_string(int file, long pos, int no, char* string[])
{
   int i, len;
   char s[80], *ns, last_ch;
   lseek(file,pos,SEEK_SET);
   for (i=0; i<no; ++i)
   {
      last_ch = 0;
      for (;;)
      {
         read(file,s,1);
         if ((s[0] == ':') && (last_ch == 'S')) break;
         last_ch = s[0];
      }
      for (len=0;;++len)
      {
         read(file,s+len,1);
         if (s[len] == 0) break;
      }
      ns = (char*) malloc(len+1);
      strcpy(ns,s);
      string[i] = ns;
      while (*ns != 0)
      {
         if ((unsigned char)*ns == 0x80) *ns = 0x5D;
         ++ns;
      }
   }
   return true;
}

bool load_data(void) /* 載入字串資料 */
{
   bool ok;
   int in = open(STRING_FILE,O_RDONLY | O_BINARY);
   if (in <= 0)
   {
      printf("Can not open %s\n",STRING_FILE);
      return false;
   }
   ok = load_string(in,0x3DF1E8L,RACE_NO,Race_Name);
   if (ok) ok = load_string(in,0x3DF358L,CLASS_NAME_NO,Class_Name);
   if (ok) ok = load_string(in,0x3DF90DL,MAGIC_NO,Magic_Name);
   if (ok) ok = load_string(in,0x3E0407L,ITEM_NO,Item_Name);
   close(in);
   return ok;
}

int open_save_file(int file_index) /* 開啟存檔 */
{
   char s[80];
   int file;
   sprintf(s,SAVE_FILE,file_index);
   file = open(s,O_RDWR | O_BINARY);
   if (file <= 0) printf("Can not open %s\n",s);
   return file;
}

bool load_file(int file_index) /* 載入存檔 */
{
   int i, pos, size, index, file, need_size;
   char block[8], *data;
   long len;
   for (i=0; i<=MAX_DATA_INDEX; ++i)
   {
      if (File_Data[i] == NULL) continue;
      free(File_Data[i]);
      File_Data[i] = NULL;
   }
   file = open_save_file(file_index);
   if (file <= 0) return false;
   len = filelength(file);
   lseek(file,1,SEEK_SET);
   for (;;)
   {
      pos = tell(file);
      if (pos >= len) break;
      if (read(file,block,8) != 8)
      {
         printf("Not enough header (pos=%lx)\n",pos);
         close(file);
         return false;
      }
      index = *((short*)block);
      need_size = size = *((unsigned short*)(block+6));
      if ((index < 0) || (index > MAX_DATA_INDEX))
      {
         printf("Bad item index=%d (pos=%lx)\n",index,pos);
         close(file);
         return false;
      }
      if (File_Data[index] != NULL)
      {
         printf("Duplicate item index=%d (pos=%lx)\n",index,pos);
         close(file);
         return false;
      }
      if (need_size < 15) need_size = 15; /* 至少15 byte，以便修改物品資料 */
      File_Data[index] = data = (char*)malloc(need_size+8);
      memcpy(data,block,8);
      if (read(file,data+8,size) != size)
      {
         printf("Not enough data (pos=%lx)\n",pos);
         close(file);
         return false;
      }
   }
   close(file);
   return true;
}

void save_file(int file_index) /* 儲入存檔 */
{
   int file, i, size;
   char header, *data;
   file = open_save_file(file_index);
   if (file <= 0) return;
   header = 0x1A;
   write(file,&header,1);
   for (i=0; i<=MAX_DATA_INDEX; ++i)
   {
      data = File_Data[i];
      if (data == NULL) continue;
      size = *((short*)(data+6))+8;
      write(file,data,size);
   }
   chsize(file,tell(file));
   close(file);
}

char* get_data(int data_index, int data_type)
{
   char* data;
   data = File_Data[data_index];
   if ((data_type >= 0) && (*((short*)(data+2)) != data_type)) return NULL;
   return data;
}

char* get_party_data(void)
{
   char* data;
   data = get_data(0,0x0566);
   if (data == NULL) return NULL;
   return data+8;
}

int get_char_data_index(int user_index)
{
   char* data;
   data = get_party_data();
   if (data == NULL) return NULL;
   return ((short*)(data+0xE5))[user_index];
}

char* get_char_data(int user_index)
{
   char* data;
   user_index = get_char_data_index(user_index);
   if (user_index < 0) return NULL;
   data = get_data(user_index,0x0559);
   if (data == NULL) return NULL;
   return data+8;
}

void print_magic_name(const char* format, int magic_type, int id)
{
   if (magic_type == 0) printf(convert(format),convert(Magic_Name[Mage_Magic_Index[id]]));
   else printf(convert(format),convert(Magic_Name[Cleric_Magic_Index[id]]));
}

void print_item_data(const char *data, bool dump) /* 顯示物品名稱 */
{
   int item_name_index, item_type, size, item_index, i, item_catalog;
   item_index = *((short*)data);
   item_type = *((short*)(data+2));
   size = *((short*)(data+6));
   item_name_index = item_type - 0x52B;
   if ((item_index < 0) || (item_type < 0))
   {
      printf(convert("<已刪除>"));
      return;
   }
   if (dump) 
   {
      printf("%X(%d): ",item_type,item_name_index);
      for (i=2; i<6; ++i) printf("%02X ",(unsigned char)data[i]);
      printf("/");
      for (i=0; i<size; ++i) printf("%02X ",(unsigned char)data[i+8]);
      printf("(%03X) ",item_index);
   }
   if ((item_name_index < 0) || (item_name_index >= sizeof(Item_Name_Index)/sizeof(Item_Name_Index[0])) || (Item_Name_Index[item_name_index] < 0))
   {
      printf("Bad Item Index");
      return;
   }
   item_type = Item_Type[item_name_index];
   if (item_type < 0)
   {
      printf("Unkown item type");
      return;
   }
   item_catalog = Item_Type_Catalog[item_type];
   item_name_index = Item_Name_Index[item_name_index];
   if (data[19]&0x04) printf(convert("被詛咒的"));
   if (item_type == 31) print_magic_name(Item_Name[item_name_index-data[20]],data[20],data[21]);
   else if ((item_type == 33) || (item_type == 46)) print_magic_name(Item_Name[item_name_index],data[20],data[21]);
   else printf(convert(Item_Name[item_name_index]));
   if ((data[20] != 0) && (item_catalog >= 1) && (item_catalog <= 3)) printf("%+d",(signed char)data[20]);
   if ((item_type == 33) || (item_type == 46)) printf(convert(" (剩%d次)"),data[22]);
}

void print_item_name(int item_index, bool dump) /* 顯示物品名稱 */
{
   if ((item_index < 0) || (item_index > MAX_DATA_INDEX))
   {
      printf(convert("<無>"));
      return;
   }
   print_item_data(File_Data[item_index],dump);
}

int select_item_type(int in_type) /* 從內部物品型態選實際物品型態 */
{
   int i, fun, type_list_no=0, type_list[100];
   for (i=0; i<sizeof(Item_Name_Index)/sizeof(Item_Name_Index[0]); ++i)
   {
      if (Item_Type[i] != in_type) continue;
      type_list[type_list_no++] = i;
   }
   if (type_list_no <= 0) return -1;
   if (type_list_no == 1) return type_list[0];
   for (i=0; i<type_list_no; ++i)
   {
      printf("\n%c. %s",i+'A',convert(Item_Name[Item_Name_Index[type_list[i]]]));
   }
   printf(convert("\n\n請選擇: "));
   fun = getch();
   printf("%c\n",fun);
   fun = toupper(fun)-'A';
   if ((fun < 0) || (fun >= type_list_no)) return -1;
   return type_list[fun];
}

void range_change_item_type(char* data, int start, int end) /* 從物品類別選物品 */
{
   int i, fun, in_type, item_type, catalog, type_list_no=0, type_list[100];
   for (i=0; i<sizeof(Item_Type_Catalog)/sizeof(Item_Type_Catalog[0]); ++i)
   {
      if ((start > Item_Type_Catalog[i]) || (Item_Type_Catalog[i] > end)) continue;
      type_list[type_list_no++] = i;
   }
   if (type_list_no <= 0) return;
   for (i=0; i<type_list_no; ++i)
   {
      printf("\n%c. %s",i+'A',convert(Item_Type_Name[type_list[i]]));
   }
   printf(convert("\n\n請選擇: "));
   fun = getch();
   printf("%c\n",fun);
   fun = toupper(fun)-'A';
   if ((fun < 0) || (fun >= type_list_no)) return;
   in_type = type_list[fun];
   item_type = select_item_type(in_type);
   if (item_type < 0) return;
   catalog = Item_Type_Catalog[in_type];
   *((short*)(data+2)) = item_type+0x52B;
   data[20] = data[21] = 0;
   data[22] = 1;
   if ((catalog >= 1) && (catalog <= 3)) *((short*)(data+6)) = 13;
   else if (catalog == 6) *((short*)(data+6)) = 15;
   else *((short*)(data+6)) = 12;
   if ((in_type == 33) || (in_type == 46)) data[22] = 127;
   memset(data+10,0xFF,8);
}

void change_item_type(char* data) /* 變更物品型態 */
{
   int fun;
   printf(convert("\n1. 近戰武器\n2. 投射武器\n3. 防具\n4. 法術\n5. 其他\n\n請選擇: "));
   fun = getch();
   printf("%c\n",fun);
   switch(fun)
   {
   case '1': range_change_item_type(data,1,1); break;
   case '2': range_change_item_type(data,2,2); break;
   case '3': range_change_item_type(data,3,4); break;
   case '4': range_change_item_type(data,5,6); break;
   case '5': range_change_item_type(data,7,7); break;
   }
}

void inc_mode_value(char* data) /* 增減魔力數值 */
{
   int fun;
   for (;;)
   {
      clrscr();
      printf(convert("物品: "));
      print_item_data(data,false);
      printf(convert("\n\n+. 增加魔法力\n-. 減少魔法力\n0. 離開\n\n請選擇: "));
      fun = getch();
      printf("%c\n",fun);
      switch(fun)
      {
      case '+': ++data[20]; break;
      case '-': --data[20]; break;
      case '0': return;
      }
   }
}

void select_magic(char* data) /* 變更法術 */
{
   int fun, magic_no;
   const char* magic_index;
   for (;;)
   {
      if (data[20] == 0)
      {
         magic_no = sizeof(Mage_Magic_Index)/sizeof(Mage_Magic_Index[0]);
         magic_index = Mage_Magic_Index;
      }
      else
      {
         magic_no = sizeof(Cleric_Magic_Index)/sizeof(Cleric_Magic_Index[0]);
         magic_index = Cleric_Magic_Index;
      }
      clrscr();
      printf("物品: ");
      print_item_data(data,false);
      printf(convert("\n\n+. 下個法術\n-. 上個法術\n1. 切換法術體系\n0. 離開\n\n請選擇: "));
      fun = getch();
      printf("%c\n",fun);
      switch(fun)
      {
      case '0': return;
      case '1':
         if (data[20] == 0) data[20] = 1;
         else data[20] = 0;
         data[21] = 0;
         break;
      case '+': 
         ++data[21];
         if (data[21] >= magic_no) data[21] = 0;
         while (magic_index[data[21]] < 0) ++data[21];
         break;
      case '-': 
         --data[21];
         if (data[21] < 0) data[21] = magic_no-1;
         while (magic_index[data[21]] < 0) --data[21];
         break;
      }
   }
}

void change_item_mode(char* data) /* 變更物品模式 */
{
   int item_type, item_catalog;
   item_type = *((short*)(data+2));
   if (item_type < 0) return;
   item_type -= 0x52B;
   item_type = Item_Type[item_type];
   item_catalog = Item_Type_Catalog[item_type];
   switch(item_catalog)
   {
   case 1:
   case 2: 
   case 3: inc_mode_value(data); break;
   case 6: select_magic(data); break;
   }
}

bool change_item(int item_index) /* 變更物品 */
{
   char fun, *data;
   int item_type;
   data = File_Data[item_index];
   for (;;)
   {
      clrscr();
      printf(convert("物品: "));
      print_item_data(data,false);
      printf(convert("\n\n1. 變更物品型態\n2. 變更物品魔力或法術\n3. 魔杖剩餘次數最大化\n9. 儲存並離開\n0. 放棄並離開\n\n請選擇: "));
      fun = getch();
      printf("%c\n",fun);
      data[19] = 0;
      switch(fun)
      {
      case '1': change_item_type(data); break;
      case '2': change_item_mode(data); break;
      case '3':
         item_type = *((short*)(data+2));
         if (item_type < 0) break;
         item_type -= 0x52B;
         item_type = Item_Type[item_type];
         if ((item_type == 33) || (item_type == 46)) data[22] = 127;
         break;
      case '0': return false;
      case '9': return true;
      }
   }
}

void create_item(int file_index, int user_index, int slot) /* 建立物品 */
{
   short i;
   char* data;
   if (!load_file(file_index)) return;
   for (i=MAX_DATA_INDEX; i>=0; --i) /* 找出空的物品 */
   {
      if (File_Data[i] == NULL) continue;
      if (*((short*)(File_Data[i]+2)) < 0) break;
   }
   if ((i < 0) || (i > MAX_DATA_INDEX)) return;
   data = File_Data[i];
   *((short*)(data+2)) = 0x054E; /* 做成一般治療藥水 */
   *((short*)(data+4)) = *((short*)(data+18)) = 0;
   *((short*)(data+6)) = 12;
   *((short*)(data+8)) = get_char_data_index(user_index);
   *((short*)(data+10)) = *((short*)(data+12)) = *((short*)(data+14)) = *((short*)(data+16)) = -1;
   data = get_char_data(user_index);
   if (data == NULL) return;
   memcpy(data+0x5B+slot*2,&i,2);
   if (!change_item(i)) return;
   save_file(file_index);
}

void modify_items(int file_index, int user_index) /* 修改物品 */
{
   char fun, *data;
   short item_index[14], i;
   for (;;)
   {
      clrscr();
      if (!load_file(file_index)) return;
      data = get_char_data(user_index);
      if (data == NULL) return;
      printf(convert("***** %s (%d) 的物品 *****\n"),convert(data+0x93),user_index+1);
      memcpy(item_index,data+0x5B,14*2);
      for (i=0; i<14; ++i)
      {
         printf("\n%c. ",i+'A');
         print_item_name(item_index[i],false);
      }
      printf(convert("\n\n1. 創造新物品\n2. 辨識背包所有物品\n0. 離開\n\n請選擇: "));
      fun = getch();
      printf("%c\n",fun);
      switch(fun)
      {
      case '0': return;
      case '1':
         for (i=0; i<14; ++i)
            if (item_index[i] < 0) break;
         if (i >= 14) break;
         create_item(file_index,user_index,i);
         break;
      case '2':
         if (!load_file(file_index)) return;
         for (i=0; i<14; ++i)
         {
            if (item_index[i] < 0) continue;
            File_Data[item_index[i]][19] = 0;
         }
         save_file(file_index);
         break;
      default:
         fun = toupper(fun)-'A';
         if ((fun < 0) || (fun >= 14)) break;
         if (item_index[fun] < 0) break;
         if (!load_file(file_index)) return;
         if (!change_item(item_index[fun])) break;
         save_file(file_index);
         break;
      }
   }
}

void print_race_name(int race_id)
{
   printf("%s",convert(Race_Name[race_id]));
}

void print_class_name(int class_id)
{
   int each_class[3], i, id;
   class_id = Class_Combine[class_id];
   each_class[0] = class_id & 0x0F;
   each_class[1] = (class_id >> 4) & 0x0F;
   each_class[2] = (class_id >> 8) & 0x0F;
   for (i=0; i<3; ++i)
   {
      id = each_class[i];
      if (i != 0)
      {
         if (id == 0) break;
         printf("/");
      }
      printf("%s",convert(Class_Name[id]));
   }
}

void max_level(int file_index, int user_index, bool save)
{
   short hp[2], i;
   char level[3], *data;
   long exp[3];
   if (save && (!load_file(file_index))) return;
   data = get_char_data(user_index);
   if (data == NULL) return;
   level[0] = level[1] = level[2] = 20;
   hp[0] = hp[1] = 900;
   for (i=0; i<3; ++i) exp[i] = 3750000L;
   memcpy(data+0xAD,level,3);
   memcpy(data+0xB5,hp,4);
   memcpy(data+0xBD,exp,3*4);
   if (save) save_file(file_index);
}

void max_attribute(int file_index, int user_index, bool save)
{
   char attrb[7], *data;
   if (save && (!load_file(file_index))) return;
   data = get_char_data(user_index);
   if (data == NULL) return;
   attrb[0] = 18;
   attrb[1] = 100;
   memset(attrb+2,19,5);
   memcpy(data+0xC9,attrb,7);
   if (save) save_file(file_index);
}

void max_spell(int file_index, int user_index, bool save)
{
   int i;
   char* data;
   if (save && (!load_file(file_index))) return;
   data = get_char_data(user_index);
   if (data == NULL) return;
   for (i=0; i<90; ++i)
      if ((unsigned char)data[0x1AD+i] == 0xFF) data[0x1AD+i] = 0;
   for (i=0; i<70; ++i)
      if ((unsigned char)data[0x211+i] == 0xFF) data[0x211+i] = 0;
   if (save) save_file(file_index);
}

void max_all(int file_index, int user_index)
{
   if (!load_file(file_index)) return;
   max_attribute(file_index,user_index,false);
   max_level(file_index,user_index,false);
   max_spell(file_index,user_index,false); 
   save_file(file_index);
}

void recover(int file_index, int user_index)
{
   char status, food, *data;
   short hp[2], item_index[2], i;
   if (!load_file(file_index)) return;
   data = get_char_data(user_index);
   if (data == NULL) return;
   memcpy(item_index,data+0x7B,2);
   memcpy(item_index+1,data+0x83,2);
   memcpy(hp,data+0xB5,4);
   hp[0] = hp[1];
   status = 0;
   food = 100;
   data[0xAB] = status;
   memcpy(data+0xB5,hp,4);
   data[0xBB] = food;
   for (i=0; i<2; ++i)
   {
      if (item_index[i] < 0) continue;
      data = File_Data[item_index[i]];
      if (data == NULL) continue;
      data[19] = 0;
   }
   save_file(file_index);
}

void cast_all(int file_index)
{
   char* data, i;
   long local_cast, global_cast;
   if (!load_file(file_index)) return;
   data = get_party_data();
   if (data == NULL) return;
   local_cast = global_cast = 0xffffffff;
   if (*((long*)(data+0x105)) == global_cast) local_cast = global_cast = 0;
   *((long*)(data+0x105)) = global_cast;
   for (i=0; i<6; ++i)
   {
      data = get_char_data(i);
      if (data == NULL) continue;
      *((long*)(data+0xD1)) = local_cast;
   }
   save_file(file_index);
}

void change_race(int file_index, int user_index)
{
   int i, fun;
   char* data;
   for (i=0; i<RACE_NO; ++i) 
   {
      printf("\n%c. ",i+'A');
      print_race_name(i);
   }
   printf(convert("\n\n請選擇: "));
   fun = getch();
   printf("%c\n",fun);
   fun = toupper(fun);
   fun -= 'A';
   if ((fun < 0) || (fun >= RACE_NO)) return;
   if (!load_file(file_index)) return;
   data = get_char_data(user_index);
   if (data == NULL) return;
   data[0xA7] = fun;
   save_file(file_index);
}

void change_class(int file_index, int user_index)
{
   int i, fun;
   char level[3], *data;
   short hp[2];
   long exp[3];
   for (i=0; i<CLASS_NO; ++i)
   {
      printf("\n%c. ",i+'A');
      print_class_name(i);
   }
   printf(convert("\n\n請選擇: "));
   fun = getch();
   printf("%c\n",fun);
   fun = toupper(fun);
   fun -= 'A';
   if ((fun < 0) || (fun >= CLASS_NO)) return;
   if (!load_file(file_index)) return;
   data = get_char_data(user_index);
   if (data == NULL) return;
   data[0xA8] = fun;
   level[0] = level[1] = level[2] = 1;
   hp[0] = hp[1] = 40;
   for (i=0; i<3; ++i) exp[i] = 0;
   memcpy(data+0xAD,level,3);
   memcpy(data+0xB5,hp,4);
   memcpy(data+0xBD,exp,3*4);
   save_file(file_index);
}

void modify_char(int file_index, int user_index) /* 修改人物 */
{
   int i, race_id, class_index, class_id, each_class[3], id;
   char *data, attrb[7], level[3], fun, status, *cast_name;
   short hp[2];
   long exp[3], global_cast;
   for (;;)
   {
      clrscr();
      if (!load_file(file_index)) return;
      data = get_char_data(user_index);
      if (data == NULL) return;
      printf(convert("***** 人物: %s (%d) *****\n\n"),convert(data+0x93),user_index+1);
      race_id = data[0xA7];
      class_index = data[0xA8];
      status = data[0xAB];
      memcpy(level,data+0xAD,3);
      memcpy(hp,data+0xB5,4);
      memcpy(exp,data+0xBD,3*4);
      memcpy(attrb,data+0xC9,7);
      print_race_name(race_id);
      printf(", ");
      print_class_name(class_index);
      class_id = Class_Combine[class_index];
      each_class[0] = class_id & 0x0F;
      each_class[1] = (class_id >> 4) & 0x0F;
      each_class[2] = (class_id >> 8) & 0x0F;
      printf(convert("\n等級="));
      for (i=0; i<3; ++i)
      {
         id = each_class[i];
         if (i != 0)
         {
            if (id == 0) break;
            printf("/");
         }
         printf("%d",level[i]);
      }
      printf(convert(", 經驗="));
      for (i=0; i<3; ++i)
      {
         id = each_class[i];
         if (i != 0)
         {
            if (id == 0) break;
            printf("/");
         }
         printf("%ld",exp[i]);
      }
      printf(convert("\n狀態: "));
      if (status & 0x08) printf(convert("石化"));
      else if (status & 0x04) printf(convert("麻痺"));
      else if (status & 0x02) printf(convert("中毒"));
      else printf(convert("正常"));
      printf(convert("\n\n 力量 智能 學問 敏捷 體質 魅力 目前生命/最大生命\n"));
      if (attrb[1] != 0) printf("%2d/%02d ",attrb[0],attrb[1]%100);
      else printf(" %2d   ",attrb[0]);
      for (i=2; i<7; ++i) printf(" %2d  ",attrb[i]);
      printf("   %5d/%d",hp[0],hp[1]);
      data = get_party_data();
      if (data == NULL) return;
      global_cast = 0xffffffff;
      if (*((long*)(data+0x105)) == global_cast) cast_name = "關閉";
      else cast_name = "";
      printf(convert("\n\n1. 變更種族\n2. 變更職業\n3. 屬性最大化\n4. 等級/生命最大化\n5. 學習所有法術\n6. 全部最佳化\n7. 編輯物品\n8. 恢復狀態/生命/食物/詛咒\n9. %s最佳施法\n0. 離開\n\n請選擇: "),convert(cast_name));
      fun = getch();
      printf("%c\n",fun);
      switch(fun)
      {
      case '0': return;
      case '1': change_race(file_index,user_index); break;
      case '2': change_class(file_index,user_index); break;
      case '3': max_attribute(file_index,user_index,true); break;
      case '4': max_level(file_index,user_index,true); break;
      case '5': max_spell(file_index,user_index,true); break;
      case '6': max_all(file_index,user_index); break;
      case '7': modify_items(file_index,user_index); break;
      case '8': recover(file_index,user_index); break;
      case '9': cast_all(file_index); break;
      }
   }
}

void select_char(int file_index, const char* filename) /* 選擇人物 */
{
   int i;
   char fun, *data;
   for (;;)
   {
      clrscr();
      if (!load_file(file_index)) return;
      printf(convert("**** 存檔: %s **** \n\n人物:\n"),convert(filename));
      for (i=0; i<6; ++i)
      {
         data = get_char_data(i);
         if (data == NULL) continue;
         printf("  %d. %s\n",i+1,convert(data+0x93));
      }
      printf(convert("  0. 離開\n\n請選擇: "));
      fun = getch();
      printf("%c\n",fun);
      fun -= '0';
      if ((fun < 0) || (fun > 6)) continue;
      if (fun == 0) return;
      modify_char(file_index,fun-1);
   }
}

int select_file(char filename[]) /* 選擇存檔 */
{
   int file, i, len;
   char fun, buf[1024], *s, *s2, *name[10];
   for (;;)
   {
      file = open(DIR_FILE,O_RDONLY | O_BINARY);
      if (file <= 0)
      {
         printf("Can not open %s\n",DIR_FILE);
         return -1;
      }
      len = filelength(file);
      if (len >= 1024)
      {
         printf("%s: File too large\n",DIR_FILE);
         return -1;
      }
      read(file,buf,len);
      buf[len] = 0;
      printf(convert("\n**** [魔眼殺機３]存檔編輯器, 1.00V - 邱奕南（青衫） **** \n\n存檔:\n"));
      s = buf;
      for (i=0; i<10; ++i)
      {
         s2 = s;
         while ((*s2 != 0x0D) && (*s2 != 0)) ++s2;
         if (*s2 == 0) break;
         *s2 = 0;
         printf("  %c. %s\n",i+'A',convert(s));
         name[i] = s;
         s = s2+1;
         if (*s == 0x0A) ++s;
      }
      printf(convert("  0. 離開\n\n請選擇: "));
      fun = getch();
      printf("%c\n",fun);
      if (fun == '0') return -1;
      fun = toupper(fun)-'A';
      if ((fun < 0) || (fun >= 10)) continue;
      if (!load_file(fun+1)) continue;
      strcpy(filename,name[fun]);
      close(file);
      return fun+1;
   }
}

void dump_all_items(int file_index)
{
   int i;
   if (!load_file(file_index)) return;
   for (i=0; i<=MAX_DATA_INDEX; ++i)
   {
      print_item_data(File_Data[i],true);
      printf("\n");
   }
}

void main(void)
{
   int file_index=3;
   char filename[80]="";
   memset(File_Data,0,sizeof(File_Data));
   if (load_data())
   {
      file_index = select_file(filename);
      if (file_index >= 0) select_char(file_index,filename);
      /*dump_all_items(file_index);*/
   }
   system("pause");
}
