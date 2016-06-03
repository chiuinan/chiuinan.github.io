#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <io.h>
#include <fcntl.h>
#include <ctype.h>

#define SAVE_FILE "EOBDATA%d.SAV"
#define EXE_FILE "START.EXE"
#define STRING_FILE "ITEM.DAT"
#define STRING_NO 121
#define RACE_NAME_NO 6 /* 種族名稱數 */
#define CLASS_NAME_NO 6 /* 職業名稱數 */
#define CLASS_NO 15 /* 職業數 */

static const char* Race_Name[RACE_NAME_NO] = {"人類","精靈","半精靈","矮人","侏儒","半身人"};
static const char* Class_Name[CLASS_NAME_NO] = {"戰士","遊俠","武士","巫師","牧師","小偷"};
static int Class_Combine[CLASS_NO] = {0,1,2,3,4,5,4*16,5*16,3*16,3*16+5*256,5+3*16,4+5*16,4*16+3*256,1+4*16,4+3*16};
static char* Item_Name[STRING_NO];

#define MAGIC_NO 63 /* 法術數目 */
static char* Magic_Name[MAGIC_NO];

static const char* Potion_Name[] =
{"增加力量","一般治療","增強生命力","有毒","恢復元氣","增加活力速度","隱形用","解毒用"};

static const char* Ring_Name[] = 
{"裝飾用","增強巫師記憶法術","保持元氣不會飢渴","墜落陷坑不會受傷"};

static const char* Wand_Name[] =
{"會黏手的","雷電術","冷錐術","治療重傷","火球術","","魔法飛彈","解咒術"};

#define ITEM_DATA_NO 95
unsigned char Item_Data[ITEM_DATA_NO][6] = /* 內定的物品資料 */
{{0x12,0x01,0x40,0x07,0x00,0x00},
{0x1E,0x01,0x40,0x01,0x01,0x00},
{0x06,0x01,0x40,0x02,0x02,0x00},
{0x05,0x01,0x40,0x0F,0x05,0x00},
{0x2D,0x01,0x40,0x0A,0x07,0x00},
{0x4F,0x01,0x40,0x06,0x09,0x00},
{0x54,0x01,0x40,0x0D,0x0A,0x00},
{0x16,0x01,0x40,0x04,0x0B,0x00},
{0x36,0x01,0x40,0x05,0x0C,0x00},
{0x04,0x01,0x40,0x08,0x0D,0x00},
{0x21,0x01,0x40,0x12,0x0E,0x00},
{0x2C,0x01,0x40,0x0E,0x0F,0x00},
{0x30,0x01,0x40,0x10,0x10,0x00},
{0x28,0x01,0xC0,0x3D,0x11,0x00},
{0x5B,0x01,0xC0,0x3D,0x11,0x01},
{0x64,0x01,0xC0,0x3D,0x11,0x02},
{0x69,0x01,0xC0,0x3D,0x11,0x03},
{0x67,0x01,0xC0,0x3D,0x11,0x04},
{0x0D,0x01,0x40,0x13,0x12,0x00},
{0x50,0x01,0x40,0x1C,0x13,0x00},
{0x13,0x01,0x40,0x1D,0x14,0x00},
{0x1F,0x01,0x40,0x14,0x15,0x00},
{0x02,0x01,0x40,0x1F,0x16,0x00},
{0x3C,0x01,0xC0,0x00,0x17,0x00},
{0x20,0x01,0x40,0x1A,0x18,0x00},
{0x34,0x01,0x40,0x1E,0x19,0x00},
{0x2A,0x2B,0xC0,0x5D,0x1A,0x0D},
{0x5F,0x5F,0xC0,0x63,0x1A,0x1C},
{0x1B,0x01,0x40,0x16,0x1B,0x00},
{0x07,0x01,0x40,0x38,0x1C,0x00},
{0x08,0x01,0xC0,0x23,0x1D,0x00},
{0x09,0x01,0xC0,0x37,0x1E,0x00},
{0x17,0x01,0x40,0x1B,0x1E,0x00},
{0x0B,0x01,0x40,0x26,0x1F,0x64},
{0x15,0x01,0x40,0x27,0x1F,0x19},
{0x5E,0x01,0x40,0x27,0x1F,0xFF},
{0x0A,0x01,0x40,0x15,0x20,0x00},
{0x65,0x01,0x40,0x2C,0x21,0x00},
{0x11,0x01,0xC0,0x24,0x22,0x01},
{0x11,0x01,0xC0,0x57,0x23,0x22},
{0x23,0x01,0xC0,0x3B,0x25,0x01},
{0x24,0x01,0xC0,0x3E,0x25,0x02},
{0x26,0x01,0xC0,0x41,0x25,0x04},
{0x0E,0x01,0x40,0x30,0x26,0x01},
{0x0F,0x01,0x40,0x31,0x26,0x03},
{0x1A,0x01,0x40,0x5A,0x26,0x04},
{0x19,0x01,0x40,0x59,0x26,0x05},
{0x22,0x01,0x40,0x40,0x26,0x06},
{0x49,0x01,0x40,0x68,0x26,0x07},
{0x39,0x01,0x40,0x32,0x26,0x08},
{0x3A,0x01,0x40,0x32,0x26,0x09},
{0x41,0x01,0x40,0x33,0x26,0x0A},
{0x46,0x01,0x40,0x3C,0x26,0x0B},
{0x47,0x01,0x40,0x2F,0x26,0x0C},
{0x48,0x01,0x40,0x3A,0x26,0x0D},
{0x14,0x01,0xC0,0x29,0x27,0x00},
{0x14,0x01,0xC0,0x29,0x27,0x01},
{0x14,0x01,0xC0,0x29,0x27,0x02},
{0x14,0x01,0xC0,0x28,0x27,0x03},
{0x14,0x01,0xC0,0x28,0x27,0x04},
{0x14,0x01,0xC0,0x28,0x27,0x05},
{0x14,0x01,0xC0,0x28,0x27,0x06},
{0x14,0x01,0x40,0x2A,0x27,0x07},
{0x3D,0x01,0x40,0x5F,0x28,0x01},
{0x3F,0x01,0xC0,0x60,0x28,0x02},
{0x3E,0x01,0xC0,0x61,0x28,0x03},
{0x40,0x01,0xC0,0x62,0x28,0x04},
{0x73,0x01,0xC0,0x5F,0x28,0x05},
{0x75,0x01,0xC0,0x5F,0x28,0x06},
{0x74,0x01,0xC0,0x5F,0x28,0x07},
{0x03,0x01,0x40,0x20,0x29,0x00},
{0x51,0x01,0xC0,0x51,0x2A,0x00},
{0x31,0x01,0x40,0x19,0x2B,0x00},
{0x32,0x01,0x40,0x21,0x2C,0x00},
{0x38,0x01,0x40,0x2E,0x2D,0x00},
{0x57,0x01,0x40,0x6C,0x2E,0x00},
{0x42,0x01,0x40,0x35,0x2E,0x01},
{0x45,0x45,0xC0,0x45,0x2E,0x01},
{0x4A,0x4A,0x40,0x64,0x2E,0x02},
{0x4B,0x4B,0x40,0x46,0x2E,0x03},
{0x56,0x01,0xC0,0x6F,0x2E,0x04},
{0x79,0x01,0x40,0x6D,0x2E,0x05},
{0x7A,0x01,0x40,0x6D,0x2E,0x06},
{0x29,0x01,0xC0,0x39,0x2F,0x01},
{0x29,0x01,0xC0,0x51,0x2F,0x02},
{0x29,0x01,0xC0,0x50,0x2F,0x03},
{0x4E,0x01,0xC0,0x6B,0x2F,0x0A},
{0x37,0x01,0xDF,0x36,0x30,0x01},
{0x44,0x01,0xC0,0x18,0x39,0x02},
{0x4D,0x01,0xC0,0x11,0x3B,0x01},
{0x4C,0x01,0xC0,0x65,0x3C,0x02},
{0x58,0x01,0xC0,0x6E,0x3D,0x00},
{0x5A,0x01,0xC0,0x6E,0x3D,0x01},
{0x37,0x01,0xDF,0x36,0x3E,0x01},
{0x59,0x01,0x40,0x23,0x3F,0x00}};

#define ITEM_CATALOG_NO 64
char Item_Catalog[ITEM_CATALOG_NO] = /* 0=不可改模式, 1=武器, 2=防具, 3=魔力無作用的武器防具, 4=由資料選擇, 5=列表選擇 */
{1,1,1,0,1,1,0,1,0,1,1,1,1,1,1,1,
 1,4,1,2,2,2,2,1,2,2,4,2,0,0,4,4,
 2,0,5,5,0,4,4,4,4,2,2,2,3,1,4,4,
 5,0,0,0,0,0,0,0,0,2,5,2,1,4,5,0};

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

int open_file(int file_index)
{
   char s[40];
   sprintf(s,SAVE_FILE,file_index);
   return open(s,O_RDWR | O_BINARY);
}

bool load_data(void) /* 載入資料 */
{
   int i, j, len;
   char s[80], *ns;
   int in = open(STRING_FILE,O_RDONLY | O_BINARY);
   if (in <= 0)
   {
      printf("Can not open %s\n",STRING_FILE);
      return false;
   }
   lseek(in,0x1806,SEEK_SET);
   for (i=0; i<STRING_NO; ++i)
   {
      read(in,s,35);
      len = strlen(s);
      ns = (char*)malloc(len+1);
      strcpy(ns,s);
      Item_Name[i] = ns;
   }
   close(in);
   in = open(EXE_FILE,O_RDONLY | O_BINARY);
   if (in <= 0)
   {
      printf("Can not open %s\n",EXE_FILE);
      return false;
   }
   lseek(in,0x31598,SEEK_SET);
   for (i=0; i<MAGIC_NO+3; ++i)
   {
      for (j=0; ;++j)
      {
         read(in,s+j,1);
         if (s[j] == 0) break;
      }
      len = strlen(s) & (~0x01);
      s[len] = 0;
      ns = (char*)malloc(len+1);
      strcpy(ns,s);
      Magic_Name[i] = ns;
   }
   close(in);
   return true;
}

void print_mage_scroll_name(int id)
{
   printf(convert("巫師的%s卷軸"),convert(Magic_Name[id-1]));
}

void print_cleric_scroll_name(int id)
{
   printf(convert("牧師的%s卷軸"),convert(Magic_Name[id-1]));
}

void print_potion_name(int id)
{
   printf(convert("%s的藥水"),convert(Potion_Name[id]));
}

void print_ring_name(int id)
{
   if (id == 0x0A) printf(convert("紅色的環"));
   else printf(convert("%s的戒指"),convert(Ring_Name[id]));
}

void print_wand_name(int id)
{
   if (id == 0x05) printf(convert("星火寶杖"));
   else printf(convert("%s魔杖"),convert(Wand_Name[id]));
}

void write_item_data(int file_index, int item_index, char data[14])
{
   int file, pos;
   file = open_file(file_index);
   pos = item_index*14+2182;
   lseek(file,pos,SEEK_SET);
   write(file,data,14);
   close(file);
}

void read_item_data(int item_index, int file, char data[14])
{
   int pos = item_index*14+2182;
   lseek(file,pos,SEEK_SET);
   read(file,data,14);
}

void print_item_data(int item_index, char data[14], bool dump) /* 顯示物品名稱 */
{
   int item_name_index, item_type;
   if (((unsigned char)data[6] == 0xFF) && ((unsigned char)data[7] == 0xFF))
   {
      printf(convert("<已刪除物品>"));
      return;
   }
   if (dump) printf("%02X: %02X %02X %02X %02X [%02X] (%03X) ",data[4],data[0],data[1],(unsigned char)data[2],data[3],(unsigned char)data[13],item_index);
   item_name_index = data[1];
   if (item_name_index == 1) item_name_index = data[0];
   item_name_index -= 2;
   item_type = data[4];
   if ((item_name_index < 0) || (item_name_index >= STRING_NO)) printf("<Bad Item Index>");
   else
   {
      if ((data[2]&0x20) || ((item_type <= 0x1B) && (data[13] < 0))) printf(convert("被詛咒的"));
      if ((data[13] != 0) && (Item_Catalog[item_type] >= 1) && (Item_Catalog[item_type] <= 3)) printf("%+d",(signed char)data[13]);
      if (data[1] != 1) printf(convert(Item_Name[item_name_index]));
      else if (item_type == 0x22) print_mage_scroll_name(data[13]);
      else if (item_type == 0x23) print_cleric_scroll_name(data[13]);
      else if (item_type == 0x27) print_potion_name(data[13]);
      else if (item_type == 0x2F) print_ring_name(data[13]);
      else if ((item_type == 0x30) || (item_type == 0x3E)) print_wand_name(data[13]);
      else printf(convert(Item_Name[item_name_index]));
   }
   if ((item_type == 0x30) || (item_type == 0x3E)) printf(convert(" (剩%d次)"),data[2]&0x1F);
}

void print_item_name(int item_index, int file, bool dump) /* 顯示物品名稱 */
{
   char s[14];
   if (item_index == 0)
   {
      printf(convert("<無>"));
      return;
   }
   read_item_data(item_index,file,s);
   print_item_data(item_index,s,dump);
}

void print_race_name(int race_id) /* 顯示種族名稱 */
{
   if (race_id & 1) printf(convert("女性"));
   else printf(convert("男性"));
   printf("%s",convert(Race_Name[race_id/2]));
}

void print_class_name(int class_id) /* 顯示職業名稱 */
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

void max_level(int file_index,int index) /* 等級最大化 */
{
   int i, pos, file;
   short hp[2];
   char level[3];
   long exp[3];
   file = open_file(file_index);
   pos = index*0x159+20;
   level[0] = level[1] = level[2] = 13;
   hp[0] = hp[1] = 900;
   for (i=0; i<3; ++i) exp[i] = 1500000L;
   lseek(file,pos+0x1B,SEEK_SET);
   write(file,hp,4);
   lseek(file,pos+0x26,SEEK_SET);
   write(file,level,3);
   lseek(file,pos+0x29,SEEK_SET);
   write(file,exp,3*4);
   close(file);
}

void max_attribute(int file_index, int index) /* 屬性最大化 */
{
   char attrb[7*2];
   int pos, file;
   file = open_file(file_index);
   pos = index*0x159+20;
   attrb[0] = attrb[1] = 18;
   attrb[2] = attrb[3] = 100;
   memset(attrb+4,19,5*2);
   lseek(file,pos+0x0d,SEEK_SET);
   write(file,attrb,7*2);
   close(file);
}

void max_spell(int file_index, int index) /* 巫師法術全學 */
{
   int pos, file;
   long value;
   file = open_file(file_index);
   pos = index*0x159+20;
   value = 0xFFFFFFFF;
   lseek(file,pos+0xD9,SEEK_SET);
   write(file,&value,4);
   close(file);
}

void max_all(int file_index, int index)
{
   max_attribute(file_index,index);
   max_level(file_index,index);
   max_spell(file_index,index); 
}

void recover(int file_index, int index) /* 恢復生命狀態與詛咒 */
{
   char status, food, s[20];
   int pos, file, i;
   short hp[2], item_index[2];
   long status_time;
   file = open_file(file_index);
   pos = index*0x159+20;
   lseek(file,pos+0x1B,SEEK_SET);
   read(file,hp,4);
   lseek(file,pos+0xDD,SEEK_SET);
   read(file,item_index,2*2);
   hp[0] = hp[1];
   status = 1;
   status_time = 0;
   food = 100;
   lseek(file,pos+1,SEEK_SET);
   write(file,&status,1);
   lseek(file,pos+0x1B,SEEK_SET);
   write(file,hp,4);
   lseek(file,pos+0x25,SEEK_SET);
   write(file,&food,1);
   lseek(file,pos+0x117,SEEK_SET);
   write(file,&status_time,4);
   close(file);
   for (i=0; i<2; ++i)
   {
      if (item_index[i] == 0) continue;
      file = open_file(file_index);
      read_item_data(item_index[i],file,s);
      close(file);
      s[2] |= 0x40;
      s[2] &= ~0x20;
      write_item_data(file_index,item_index[i],s);
   }
}

void cast_all(int file_index) /* 最佳施法 */
{
   int pos, file, i;
   long local_cast, global_cast;

   local_cast = global_cast = 0xFFFFFFFFL;
   file = open_file(file_index);
   for (i=0; i<6; ++i)
   {
      pos = i*0x159+20;
      lseek(file,pos+0x149,SEEK_SET);
      write(file,&local_cast,4);
   }
   lseek(file,0x838,SEEK_SET);
   write(file,&global_cast,4);
   close(file);
}

void change_race(int file_index, int index) /* 變更種族 */
{
   int i, pos, file;
   char fun;
   for (i=0; i<RACE_NAME_NO*2; ++i) 
   {
      printf("\n%c. ",i+'A');
      print_race_name(i);
   }
   printf(convert("\n\n請選擇: "));
   fun = getch();
   printf("%c\n",fun);
   fun = toupper(fun);
   fun -= 'A';
   if ((fun < 0) || (fun >= RACE_NAME_NO*2)) return;
   file = open_file(file_index);
   pos = index*0x159+20;
   lseek(file,pos+0x21,SEEK_SET);
   write(file,&fun,1);
   close(file);
}

void change_class(int file_index, int index) /* 變職業 */
{
   int i, pos, file;
   char fun, level[3];
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
   file = open_file(file_index);
   pos = index*0x159+20;
   lseek(file,pos+0x22,SEEK_SET);
   write(file,&fun,1);
   level[0] = level[1] = level[2] = 1;
   hp[0] = hp[1] = 40;
   for (i=0; i<3; ++i) exp[i] = 0;
   lseek(file,pos+0x1B,SEEK_SET);
   write(file,hp,4);
   lseek(file,pos+0x26,SEEK_SET);
   write(file,level,3);
   lseek(file,pos+0x29,SEEK_SET);
   write(file,exp,3*4);
   close(file);
}

void init_item_data(char data[14], int item_type) /* 初始化物品資料 */
{
   int i;
   memset(data,0,14);
   for (i=0; i<ITEM_DATA_NO; ++i)
   {
      if (Item_Data[i][4] == item_type)
      {
         memcpy(data,Item_Data[i],5);
         data[13] = Item_Data[i][5];
         return;
      }
   }
}

void change_weapon(char data[14]) /* 變更成武器 */
{
   char fun;
   int item_type;
   printf(convert("\nA. 短劍\nB. 斧\nC. 長劍\nD. 戟\nE. 杖\nF. 釘頭鎚\nG. 鏈枷\nH. 石塊\nI. 箭\nJ. 鏢\nK. 匕首\nL. 矛\nM. 投石索\nN. 弓\nO. 三叉戟\nP. 雙手劍\nQ. 水晶鎚\n\n請選擇: "));
   fun = getch();
   printf("%c\n",fun);
   switch(toupper(fun))
   {
   case 'A': item_type = 0x02; break;
   case 'B': item_type = 0x00; break;
   case 'C': item_type = 0x01; break;
   case 'D': item_type = 0x0A; break;
   case 'E': item_type = 0x0D; break;
   case 'F': item_type = 0x0B; break;
   case 'G': item_type = 0x0C; break;
   case 'H': item_type = 0x12; break;
   case 'I': item_type = 0x10; break;
   case 'J': item_type = 0x0F; break;
   case 'K': item_type = 0x05; break;
   case 'L': item_type = 0x09; break;
   case 'M': item_type = 0x0E; break;
   case 'N': item_type = 0x07; break;
   case 'O': item_type = 0x17; break;
   case 'P': item_type = 0x2D; break;
   case 'Q': item_type = 0x3C; break;
   default: return;
   }
   if (item_type == data[4]) return;
   init_item_data(data,item_type);
}

void change_armor(char data[14]) /* 變更成防具 */
{
   char fun;
   int item_type;
   printf(convert("\nA. 長袍\nB. 皮甲\nC. 鱗甲\nD. 鎖子甲\nE. 板條甲\nF. 鎧甲\nG. 盾\nH. 頭盔\nI. 鞋子\nJ. 護腕帶\nK. 戒指\nL. 項鍊\nM. 梅杜莎護盾\nN. 鍾納斯之袍\n\n請選擇: "));
   fun = getch();
   printf("%c\n",fun);
   switch(toupper(fun))
   {
   case 'A': item_type = 0x29; break;
   case 'B': item_type = 0x16; break;
   case 'C': item_type = 0x19; break;
   case 'D': item_type = 0x14; break;
   case 'E': item_type = 0x13; break;
   case 'F': item_type = 0x18; break;
   case 'G': item_type = 0x1B; break;
   case 'H': item_type = 0x15; break;
   case 'I': item_type = 0x20; break;
   case 'J': item_type = 0x2B; break;
   case 'K': item_type = 0x2A; break;
   case 'L': item_type = 0x2C; break;
   case 'M': item_type = 0x39; break;
   case 'N': item_type = 0x3B; break;
   default: return;
   }
   if (item_type == data[4]) return;
   init_item_data(data,item_type);
}

void change_spell_item(char data[14]) /* 變更成帶法術物品 */
{
   char fun;
   int item_type;
   printf(convert("\nA. 魔法書\nB. 聖符\nC. 巫師卷軸\nD. 牧師卷軸\nE. 魔杖\nF. 戒指\nG. 生命項鍊\nH. 施法物品\n\n請選擇: "));
   fun = getch();
   printf("%c\n",fun);
   switch(toupper(fun))
   {
   case 'A': item_type = 0x1D; break;
   case 'B': item_type = 0x1E; break;
   case 'C': item_type = 0x22; break;
   case 'D': item_type = 0x23; break;
   case 'E': item_type = 0x3E; break;
   case 'F': item_type = 0x2F; break;
   case 'G': item_type = 0x3D; break;
   case 'H': item_type = 0x1A; break;
   default: return;
   }
   if (item_type == data[4]) return;
   init_item_data(data,item_type);
}

void change_misc_item(char data[14]) /* 變更成其他物品 */
{
   char fun;
   int item_type;
   printf(convert("\nA. 糧食\nB. 開鎖工具\nC. 鑰匙\nD. 寶石\nF. 石器\nG. 藥水\nI. 特殊物品\nJ. 號角\nK. 貼紙\n\n請選擇: "));
   fun = getch();
   printf("%c\n",fun);
   switch(toupper(fun))
   {
   case 'A': item_type = 0x1F; break;
   case 'B': item_type = 0x1C; break;
   case 'C': item_type = 0x26; break;
   case 'D': item_type = 0x28; break;
   case 'F': item_type = 0x25; break;
   case 'G': item_type = 0x27; break;
   case 'I': item_type = 0x2E; break;
   case 'J': item_type = 0x11; break;
   case 'K': item_type = 0x3F; break;
   default: return;
   }
   if (item_type == data[4]) return;
   init_item_data(data,item_type);
}

void change_item_type(char data[14]) /* 變更物品型態 */
{
   char fun;
   printf(convert("\n1. 武器\n2. 防具\n3. 法術\n4. 其他\n\n請選擇: "));
   fun = getch();
   printf("%c\n",fun);
   switch(fun)
   {
   case '1': change_weapon(data); break;
   case '2': change_armor(data); break;
   case '3': change_spell_item(data); break;
   case '4': change_misc_item(data); break;
   }
}

void inc_mode_value(char data[14]) /* 增減魔力數值 */
{
   char fun;
   for (;;)
   {
      clrscr();
      printf(convert("物品: "));
      print_item_data(0,data,false);
      printf(convert("\n\n+. 增加魔法力\n-. 減少魔法力\n0. 離開\n\n請選擇: "));
      fun = getch();
      printf("%c\n",fun);
      switch(fun)
      {
      case '+': ++data[13]; break;
      case '-': --data[13]; break;
      case '0':
         if (data[13] == 0) data[2] &= 0x7F;
         else data[2] |= 0x80;
         return;
      }
   }
}

void bound_inc_mode_value(char data[14], int low, int high) /* 限制式增減魔力數值 */
{
   char fun;
   for (;;)
   {
      clrscr();
      printf("物品: ");
      print_item_data(0,data,false);
      printf(convert("\n\n+. 下個法術\n-. 上個法術\n0. 離開\n\n請選擇: "));
      fun = getch();
      printf("%c\n",fun);
      switch(fun)
      {
      case '+': 
         ++data[13]; 
         if (data[13] > high) data[13] = low;
         break;
      case '-': 
         --data[13]; 
         if (data[13] < low) data[13] = high;
         break;
      case '0':
         if (data[13] == 0) data[2] &= 0x7F;
         else data[2] |= 0x80;
         return;
      }
   }
}

void select_mode(char data[14]) /* 選擇魔力數值 */
{
   int i, select_index[20], select_no, item_type, index;
   char temp_data[14], fun;
   item_type = data[4];
   select_no = 0;
   for (i=0; i<ITEM_DATA_NO; ++i)
   {
      if (Item_Data[i][4] == item_type)
      {
         printf("\n%c. ",select_no+'A');
         select_index[select_no++] = i;
         memcpy(temp_data,Item_Data[i],5);
         temp_data[13] = Item_Data[i][5];
         print_item_data(0,temp_data,false);
      }
   }
   printf(convert("\n\n請選擇: "));
   fun = getch();
   printf("%c\n",fun);
   fun = toupper(fun)-'A';
   if ((fun < 0) || (fun >= select_no)) return;
   index = select_index[fun];
   memcpy(data,Item_Data[index],5);
   data[13] = Item_Data[index][5];
}

void select_mage_scroll(char data[14]) /* 選擇巫師卷軸法術 */
{
   bound_inc_mode_value(data,1,32);
}

void select_cleric_scroll(char data[14]) /* 選擇牧師卷軸法術 */
{
   bound_inc_mode_value(data,34,MAGIC_NO-1);
}

void select_wand(char data[14]) /* 選擇魔杖法術 */
{
   bound_inc_mode_value(data,0,7);
}

void change_item_mode(char data[14]) /* 變更物品模式 */
{
   int item_type, item_catalog;
   item_type = data[4];
   item_catalog = Item_Catalog[item_type];
   switch(item_catalog)
   {
   case 1:
   case 2: inc_mode_value(data); break;
   case 4: select_mode(data); break;
   case 5:
      if (item_type == 0x22) select_mage_scroll(data);
      else if (item_type == 0x23) select_cleric_scroll(data);
      else if ((item_type == 0x30) || (item_type == 0x3E)) select_wand(data);
      break;
   }
}

bool change_item(char data[14]) /* 變更物品 */
{
   char fun;
   for (;;)
   {
      clrscr();
      printf(convert("物品: "));
      print_item_data(0,data,false);
      printf(convert("\n\n1. 變更物品型態\n2. 變更物品魔力或編號\n3. 魔杖剩餘次數最大化\n9. 儲存並離開\n0. 放棄並離開\n\n請選擇: "));
      fun = getch();
      printf("%c\n",fun);
      data[2] |= 0x40;
      data[2] &= ~0x20;
      switch(fun)
      {
      case '1': change_item_type(data); break;
      case '2': change_item_mode(data); break;
      case '3': if ((data[4] == 0x30) || (data[4] == 0x3E)) data[2] |= 0x1F; break;
      case '0': return false;
      case '9': return true;
      }
   }
}

void create_item(int file_index, int user_index, int slot) /* 建立物品 */
{
   char data[14];
   int pos, file;
   short i;
   file = open_file(file_index);
   for (i=1; i<600; ++i) /* 找出空的物品 */
   {
      read_item_data(i,file,data);
      if (((unsigned char)data[6] == 0xFF) && ((unsigned char)data[7] == 0xFF)) break;
   }
   close(file);
   if (i >= 600) return;
   init_item_data(data,0);
   if (!change_item(data)) return;
   write_item_data(file_index,i,data);
   file = open_file(file_index);
   pos = user_index*0x159+20;
   lseek(file,pos+0xE1+slot*2,SEEK_SET);
   write(file,&i,2);
   close(file);
}

void modify_items(int file_index, int index) /* 修改物品 */
{
   int i, pos, file;
   char s[80], fun;
   short item_index[14];
   pos = index*0x159+20;
   for (;;)
   {
      clrscr();
      file = open_file(file_index);
      lseek(file,pos+2,SEEK_SET);
      read(file,s,10);
      printf(convert("***** %s (%d) 的物品 *****\n"),convert(s),index+1);
      lseek(file,pos+0xE1,SEEK_SET);
      read(file,item_index,14*2);
      for (i=0; i<14; ++i)
      {
         printf("\n%c. ",i+'A');
         print_item_name(item_index[i],file,false);
      }
      close(file);
      printf(convert("\n\n1. 創造新物品\n2. 辨識背包所有物品\n0. 離開\n\n請選擇: "));
      fun = getch();
      printf("%c\n",fun);
      switch(fun)
      {
      case '0': return;
      case '1':
         for (i=0; i<14; ++i)
            if (item_index[i] == 0) break;
         if (i >= 14) break;
         create_item(file_index,index,i);
         break;
      case '2':
         for (i=0; i<14; ++i)
         {
            if (item_index[i] == 0) continue;
            file = open_file(file_index);
            read_item_data(item_index[i],file,s);
            close(file);
            s[2] |= 0x40;
            s[2] &= ~0x20;
            write_item_data(file_index,item_index[i],s);
         }
         break;
      default:
         fun = toupper(fun)-'A';
         if ((fun < 0) || (fun >= 14)) break;
         if (item_index[fun] == 0) break;
         file = open_file(file_index);
         read_item_data(item_index[fun],file,s);
         close(file);
         if (!change_item(s)) break;
         write_item_data(file_index,item_index[fun],s);
         break;
      }
   }
}

void modify_char(int file_index, int index) /* 修改人物 */
{
   int i, pos, race_id, class_index, class_id, each_class[3], id, file;
   char s[80], attrb[9*2], level[3], fun, status;
   long exp[3];
   pos = index*0x159+20;
   for (;;)
   {
      clrscr();
      file = open_file(file_index);
      lseek(file,pos+1,SEEK_SET);
      read(file,&status,1);
      read(file,s,10);
      printf(convert("***** 人物: %s (%d) *****\n\n"),convert(s),index+1);
      lseek(file,pos+0x0d,SEEK_SET);
      read(file,attrb,9*2);
      read(file,s,7);
      race_id = s[2];
      class_index = s[3];
      read(file,level,3);
      read(file,exp,3*4);
      close(file);
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
      if (attrb[3] != 0) printf("%2d/%02d ",attrb[1],attrb[3]%100);
      else printf(" %2d   ",attrb[1]);
      for (i=2; i<7; ++i) printf(" %2d  ",attrb[i*2+1]);
      printf("   %5d/%d",((short*)(attrb+7*2))[0],((short*)(attrb+7*2))[1]);
      printf(convert("\n\n1. 變更種族\n2. 變更職業\n3. 屬性最大化\n4. 等級/生命最大化\n5. 學習所有巫師法術\n6. 全部最佳化\n7. 編輯物品\n8. 恢復狀態/生命/食物/詛咒\n9. 最佳施法\n0. 離開\n\n請選擇: "));
      fun = getch();
      printf("%c\n",fun);
      switch(fun)
      {
      case '0': return;
      case '1': change_race(file_index,index); break;
      case '2': change_class(file_index,index); break;
      case '3': max_attribute(file_index,index); break;
      case '4': max_level(file_index,index); break;
      case '5': max_spell(file_index,index); break;
      case '6': max_all(file_index,index); break;
      case '7': modify_items(file_index,index); break;
      case '8': recover(file_index,index); break;
      case '9': cast_all(file_index); break;
      }
   }
}

void select_char(int file_index) /* 選擇人物 */
{
   int i, pos, file;
   char s[21], fun;
   for (;;)
   {
      clrscr();
      file = open_file(file_index);
      read(file,s,20);
      s[20] = 0;
      printf(convert("**** 存檔: %s **** \n\n人物:\n"),convert(s));
      for (i=0; i<6; ++i)
      {
         pos = i*0x159+20;
         lseek(file,pos+2,SEEK_SET);
         read(file,s,10);
         if (s[0] == 0) break;
         printf("  %d. %s\n",i+1,convert(s));
      }
      close(file);
      printf(convert("  0. 離開\n\n請選擇: "));
      fun = getch();
      printf("%c\n",fun);
      fun -= '0';
      if ((fun < 0) || (fun > i)) continue;
      if (fun == 0) return;
      modify_char(file_index,fun-1);
   }
}

int select_file(void) /* 選擇存檔 */
{
   int file, i;
   char fun, s[21];
   for (;;)
   {
      clrscr();
      printf(convert("**** [魔眼殺機２]存檔編輯器, 1.00V - 邱奕南（青衫） **** \n\n存檔:\n"));
      for (i=0; i<6; ++i)
      {
         file = open_file(i);
         if (file <= 0) printf(convert("  %d. <尚未使用>\n"),i+1);
         else
         {
            read(file,s,20);
            close(file);
            s[20] = 0;
            printf("  %d. %s\n",i+1,convert(s));
         }
      }
      printf(convert("  0. 離開\n\n請選擇: "));
      fun = getch();
      printf("%c\n",fun);
      fun -= '0';
      if ((fun < 0) || (fun > 6)) continue;
      if (fun == 0) return -1;
      --fun;
      file = open_file(fun);
      if (file <= 0) continue;
      close(file);
      return fun;
   }
}

void dump_all_items(int file_index)
{
   int i, file;
   file = open_file(file_index);
   for (i=1; i<600; ++i)
   {
      print_item_name(i,file,true);
      printf("\n");
   }
   close(file);
}

void main(void)
{
   int file_index;
   if (load_data())
   {
      file_index = select_file();
      if (file_index >= 0) select_char(file_index);
      /*dump_all_items(file_index);*/
   }
   system("pause");
}
