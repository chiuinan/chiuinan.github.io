#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <io.h>
#include <fcntl.h>
#include <ctype.h>

#define SAVE_FILE "EOBDATA.SAV"
#define STRING_FILE "EOBDATA6.PAK"
#define STRING_NO 93
#define RACE_NAME_NO 6 /* 種族名稱數 */
#define CLASS_NAME_NO 6 /* 職業名稱數 */
#define CLASS_NO 15 /* 職業數 */

static const char* Race_Name[RACE_NAME_NO] = {"Human","Elf","Half Elf","Dwarf","Gnome","Halfling"};
static const char* Class_Name[CLASS_NAME_NO] = {"Fighter","Ranger","Paladin","Mage","Cleric","Thief"};
static int Class_Combine[CLASS_NO] = {0,1,2,3,4,5,4*16,5*16,3*16,3*16+5*256,5+3*16,4+5*16,4*16+3*256,1+4*16,4+3*16};
static char* Item_Name[STRING_NO];

static const char* Mage_Scroll_Name[] =
{"","Armor","Burning Hands","Detect Magic","Magic Missile","Read Magic (Don't use)","Shield","Shocking Grasp","Invisibility","Knock (Don't use)",
"Melf's Acid Arrow","Stinking Cloud (No effect)","Dispel Magic","Fireball","Flame Arrow","Haste","Hold Person","Invisibility 10'","Lightning bolt",
"Vampiric Touch","Fear","Ice Storm","Stoneskin","Cloud Kill (No effect)","Cone of Cold","Hold Monster"};

static const char* Cleric_Scroll_Name[] =
{"","Bless","Cure Light Wnds","Cause Light Wnds","Detect Magic","Protect Evil","Aid","Flame Blade","Hold Person",
"Slow Poison","Create Food","Dispel Magic","Magical Vestment","Prayer","Remove Paralysis","Cure Serious","Cause Serious",
"Neutralize Poison","Protect Evil 10'","Protect Lightning","Cure Critical","Cause Critical","Flame Strike","Raise Dead"};

static const char* Potion_Name[] =
{"","Giant Strength","Healing","Extra Healing","Poison","Vitality","Speed","Invisibility","Cure Poison"};

static const char* Ring_Name[] = 
{"Adornment","Wizardry","Sustenance","Feather Fall"};

static const char* Wand_Name[] =
{"Stick","Lightning","Frost","Curing","Fireball","Silvias","Magic Missile","Magical Vestment"};

#define ITEM_DATA_NO 68
unsigned char Item_Data[ITEM_DATA_NO][6] = /* 內定的物品資料 */
{{0x1B,0x01,0x40,0x07,0x00,0x00},
{0x36,0x01,0x40,0x01,0x01,0x00},
{0x06,0x01,0x40,0x02,0x02,0x00},
{0x26,0x01,0xC0,0x25,0x03,0x00},
{0x05,0x01,0x40,0x0F,0x05,0x00},
{0x0E,0x27,0xC0,0x2A,0x06,0x08},
{0x32,0x01,0x40,0x0A,0x07,0x00},
{0x2C,0x01,0x40,0x06,0x09,0x00},
{0x1D,0x01,0x40,0x09,0x0A,0x00},
{0x23,0x01,0x40,0x04,0x0B,0x00},
{0x51,0x01,0x40,0x05,0x0C,0x00},
{0x04,0x01,0x40,0x08,0x0D,0x00},
{0x34,0x01,0x40,0x12,0x0E,0x00},
{0x1C,0x01,0x40,0x0E,0x0F,0x00},
{0x2F,0x01,0x40,0x10,0x10,0x00},
{0x28,0x01,0x40,0x13,0x12,0x00},
{0x4A,0x01,0x40,0x1C,0x13,0x00},
{0x1E,0x01,0x40,0x1D,0x14,0x00},
{0x1F,0x01,0x40,0x14,0x15,0x00},
{0x02,0x01,0x40,0x1F,0x16,0x00},
{0x3F,0x01,0x40,0x1A,0x18,0x00},
{0x40,0x01,0x40,0x1E,0x19,0x00},
{0x30,0x01,0x40,0x16,0x1B,0x00},
{0x07,0x01,0x40,0x38,0x1C,0x00},
{0x08,0x01,0xC0,0x23,0x1D,0x00},
{0x09,0x01,0x40,0x37,0x1E,0x00},
{0x16,0x01,0x40,0x1B,0x1E,0x00},
{0x0B,0x01,0x40,0x26,0x1F,0x64},
{0x29,0x01,0x40,0x27,0x1F,0x19},
{0x41,0x01,0x40,0x15,0x20,0x00},
{0x59,0x01,0x40,0x2C,0x21,0x00},
{0x12,0x01,0xC0,0x24,0x22,0x01},
{0x12,0x01,0xC0,0x57,0x23,0x01},
{0x54,0x01,0xC0,0x41,0x25,0x01},
{0x3E,0x01,0xC0,0x42,0x25,0x02},
{0x55,0x01,0xC0,0x3F,0x25,0x03},
{0x33,0x01,0xC0,0x3E,0x25,0x04},
{0x2D,0x01,0xC0,0x3C,0x25,0x05},
{0x3B,0x01,0xC0,0x3D,0x25,0x06},
{0x43,0x01,0xC0,0x40,0x25,0x07},
{0x21,0x01,0x40,0x30,0x26,0x01},
{0x31,0x01,0x40,0x31,0x26,0x02},
{0x37,0x01,0x40,0x32,0x26,0x03},
{0x19,0x01,0x40,0x33,0x26,0x04},
{0x10,0x01,0x40,0x34,0x26,0x05},
{0x4B,0x01,0x40,0x35,0x26,0x06},
{0x0D,0x01,0x40,0x11,0x26,0x07},
{0x4C,0x01,0x40,0x18,0x26,0x08},
{0x0E,0x01,0xC0,0x28,0x27,0x01},
{0x0E,0x01,0xC0,0x29,0x27,0x02},
{0x0E,0x01,0xC0,0x29,0x27,0x03},
{0x0E,0x01,0xC0,0x2A,0x27,0x04},
{0x0E,0x01,0xC0,0x2A,0x27,0x05},
{0x0E,0x01,0xC0,0x28,0x27,0x06},
{0x0E,0x01,0xC0,0x2A,0x27,0x07},
{0x0E,0x01,0x40,0x29,0x27,0x08},
{0x0F,0x01,0x40,0x2E,0x28,0x01},
{0x0F,0x01,0xC0,0x2F,0x28,0x02},
{0x03,0x01,0x40,0x20,0x29,0x00},
{0x13,0x01,0xC0,0x51,0x2A,0x00},
{0x44,0x01,0x40,0x19,0x2B,0x00},
{0x38,0x01,0x40,0x22,0x2C,0x00},
{0x38,0x48,0xC0,0x22,0x2E,0x01},
{0x13,0x01,0xC0,0x39,0x2F,0x01},
{0x13,0x01,0xC0,0x51,0x2F,0x02},
{0x13,0x01,0xC0,0x50,0x2F,0x03},
{0x11,0x01,0xDF,0x36,0x30,0x01},
{0x42,0x01,0x40,0x3A,0x31,0x0A}};

#define ITEM_CATALOG_NO 50
char Item_Catalog[ITEM_CATALOG_NO] = /* 0=不可改模式, 1=武器, 2=防具, 3=魔力無作用的武器防具, 4=由資料選擇, 5=列表選擇 */
{1,1,1,0,1,1,0,1,0,1,1,1,1,1,1,1,
 1,0,1,2,2,3,2,0,2,2,0,2,0,0,4,4,
 3,0,5,5,0,4,4,4,4,2,2,2,3,0,3,4,
 5,0};

#ifdef WIN32 /* VC++ */
void clrscr(void)
{
   int i;
   for (i=0; i<20; ++i) printf("\n");
}
#else /* Turbo C */
#define bool  int
#define true  1
#define false 0
#endif

int open_file(void)
{
   return open(SAVE_FILE,O_RDWR | O_BINARY);
}

bool load_data(void) /* 載入資料 */
{
   int i, len;
   char s[80], *ns;
   int in = open(STRING_FILE,O_RDONLY | O_BINARY);
   if (in <= 0)
   {
      printf("Can not open %s\n",STRING_FILE);
      return false;
   }
   lseek(in,0x411C8,SEEK_SET);
   for (i=0; i<STRING_NO; ++i)
   {
      read(in,s,35);
      len = strlen(s);
      ns = (char*)malloc(len+1);
      strcpy(ns,s);
      Item_Name[i] = ns;
   }
   close(in);
   return true;
}

void print_mage_scroll_name(int id)
{
   printf("Mage Scroll of %s",Mage_Scroll_Name[id]);
}

void print_cleric_scroll_name(int id)
{
   printf("Cleric Scroll of %s",Cleric_Scroll_Name[id]);
}

void print_potion_name(int id)
{
   printf("Potion of %s",Potion_Name[id]);
}

void print_gem_name(int id)
{
   if (id == 1) printf("Red Gem");
   else if (id == 2) printf("Blue Gem");
   else printf("Gem");
}

void print_ring_name(int id)
{
   printf("Ring of %s",Ring_Name[id]);
}

void print_wand_name(int id)
{
   printf("Wand of %s",Wand_Name[id]);
}

void write_item_data(int item_index, char data[14])
{
   int file, pos;
   file = open_file();
   pos = item_index*14+1522;
   lseek(file,pos,SEEK_SET);
   write(file,data,14);
   close(file);
}

void read_item_data(int item_index, int file, char data[14])
{
   int pos = item_index*14+1522;
   lseek(file,pos,SEEK_SET);
   read(file,data,14);
}

void print_item_data(int item_index, char data[14], bool dump) /* 顯示物品名稱 */
{
   int item_name_index, item_type;
   if (((unsigned char)data[6] == 0xFF) && ((unsigned char)data[7] == 0xFF))
   {
      printf("<Deleted Item>");
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
      if ((item_type <= 0x1B) && (data[13] < 0)) printf("Cursed ");
      if ((data[1] != 1) || ((item_type != 0x22) && (item_type != 0x23) && (item_type != 0x27) && (item_type != 0x28) && (item_type != 0x2F) && (item_type != 0x30))) printf(Item_Name[item_name_index]);
      if (data[1] == 1)
      {
         if ((data[13] != 0) && ((Item_Catalog[item_type] == 1) || (Item_Catalog[item_type] == 2))) printf(" %+d",(signed char)data[13]);
         else if (item_type == 0x22) print_mage_scroll_name(data[13]);
         else if (item_type == 0x23) print_cleric_scroll_name(data[13]);
         else if (item_type == 0x27) print_potion_name(data[13]);
         else if (item_type == 0x28) print_gem_name(data[13]);
         else if (item_type == 0x2F) print_ring_name(data[13]);
         else if (item_type == 0x30) print_wand_name(data[13]);
      }
   }
   if (item_type == 0x30) printf(" (%d)",data[2]&0x1F);
}

void print_item_name(int item_index, int file, bool dump) /* 顯示物品名稱 */
{
   char s[14];
   if (item_index == 0)
   {
      printf("<No Item>");
      return;
   }
   read_item_data(item_index,file,s);
   print_item_data(item_index,s,dump);
}

void print_race_name(int race_id) /* 顯示種族名稱 */
{
   printf("%s ",Race_Name[race_id/2]);
   if (race_id & 1) printf("Female");
   else printf("Male");
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
      printf("%s",Class_Name[id]);
   }
}

void max_level(int index) /* 等級最大化 */
{
   int i, pos, class_id, each_class[3], file;
   char class_index, hp[2], level[3];
   long exp[3];
   file = open_file();
   pos = index*0xf3;
   lseek(file,pos+0x20,SEEK_SET);
   read(file,&class_index,1);
   class_id = Class_Combine[class_index];
   each_class[0] = class_id & 0x0F;
   each_class[1] = (class_id >> 4) & 0x0F;
   each_class[2] = (class_id >> 8) & 0x0F;
   for (i=0; i<3; ++i)
      if (each_class[i] == 4) level[i] = 10;
      else level[i] = 11;
   hp[0] = hp[1] = 127;
   for (i=0; i<3; ++i) exp[i] = 900000L;
   lseek(file,pos+0x1B,SEEK_SET);
   write(file,hp,2);
   lseek(file,pos+0x24,SEEK_SET);
   write(file,level,3);
   lseek(file,pos+0x27,SEEK_SET);
   write(file,exp,3*4);
   close(file);
}

void max_attribute(int index) /* 屬性最大化 */
{
   char attrb[7*2];
   int pos, file;
   file = open_file();
   pos = index*0xf3;
   attrb[0] = 22;
   attrb[1] = 22;
   attrb[2] = attrb[3] = 0;
   memset(attrb+4,19,5*2);
   lseek(file,pos+0x0d,SEEK_SET);
   write(file,attrb,7*2);
   close(file);
}

void max_spell(int index) /* 巫師法術全學 */
{
   int pos, file;
   long value;
   file = open_file();
   pos = index*0xf3;
   value = 0x01BFFAEF;
   lseek(file,pos+0x73,SEEK_SET);
   write(file,&value,4);
   close(file);
}

void max_all(int index)
{
   max_attribute(index);
   max_level(index);
   max_spell(index); 
}

void change_race(int index) /* 變更種族 */
{
   int i, pos, file;
   char fun;
   for (i=0; i<RACE_NAME_NO*2; ++i) 
   {
      printf("\n%c. ",i+'A');
      print_race_name(i);
   }
   printf("\n\nSelect: ");
   fun = getch();
   printf("%c\n",fun);
   fun = toupper(fun);
   fun -= 'A';
   if ((fun < 0) || (fun >= RACE_NAME_NO*2)) return;
   file = open_file();
   pos = index*0xf3;
   lseek(file,pos+0x1F,SEEK_SET);
   write(file,&fun,1);
   close(file);
}

void change_class(int index) /* 變職業 */
{
   int i, pos, file;
   char fun, hp[2], level[3];
   long exp[3];
   for (i=0; i<CLASS_NO; ++i)
   {
      printf("\n%c. ",i+'A');
      print_class_name(i);
   }
   printf("\n\nSelect: ");
   fun = getch();
   printf("%c\n",fun);
   fun = toupper(fun);
   fun -= 'A';
   if ((fun < 0) || (fun >= CLASS_NO)) return;
   file = open_file();
   pos = index*0xf3;
   lseek(file,pos+0x20,SEEK_SET);
   write(file,&fun,1);
   level[0] = level[1] = level[2] = 1;
   hp[0] = hp[1] = 15;
   for (i=0; i<3; ++i) exp[i] = 0;
   lseek(file,pos+0x1B,SEEK_SET);
   write(file,hp,2);
   lseek(file,pos+0x24,SEEK_SET);
   write(file,level,3);
   lseek(file,pos+0x27,SEEK_SET);
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
   printf("\nA. Short Sword\nB. Axe\nC. Long Sword\nD. Halberd\nE. Staff\nF. Mace\nG. Flail\nH. Rock\nI. Arrow\nJ. Dart\nK. Dagger\nL. Spear\nM. Sling\nN. Bow\n\nSelect: ");
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
   default: return;
   }
   if (item_type == data[4]) return;
   init_item_data(data,item_type);
}

void change_armor(char data[14]) /* 變更成防具 */
{
   char fun;
   int item_type;
   printf("\nA. Robe\nB. Leather Armor\nC. Scale Mail\nD. Chain Mail\nE. Banded Armor\nF. Plate Mail\nG. Shield\nH. Helm\nI. Boots\nJ. Bracers\nK. Ring\nL. Necklace\n\nSelect: ");
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
   default: return;
   }
   if (item_type == data[4]) return;
   init_item_data(data,item_type);
}

void change_spell_item(char data[14]) /* 變更成帶法術物品 */
{
   char fun;
   int item_type;
   printf("\nA. Spellbook\nB. Holy Symbol\nC. Mage Scroll\nD. Cleric Scroll\nE. Wand\nF. Ring\nG. Necklace\n\nSelect: ");
   fun = getch();
   printf("%c\n",fun);
   switch(toupper(fun))
   {
   case 'A': item_type = 0x1D; break;
   case 'B': item_type = 0x1E; break;
   case 'C': item_type = 0x22; break;
   case 'D': item_type = 0x23; break;
   case 'E': item_type = 0x30; break;
   case 'F': item_type = 0x2F; break;
   case 'G': item_type = 0x2E; break;
   default: return;
   }
   if (item_type == data[4]) return;
   init_item_data(data,item_type);
}

void change_misc_item(char data[14]) /* 變更成其他物品 */
{
   char fun;
   int item_type;
   printf("\nA. Rations\nB. Lock picks\nC. Key\nD. Gem\nE. Orb of Power\nF. Stone Items\nG. Potion\nH. Dwarven Healing Potion\nI. Kenku Egg\n\nSelect: ");
   fun = getch();
   printf("%c\n",fun);
   switch(toupper(fun))
   {
   case 'A': item_type = 0x1F; break;
   case 'B': item_type = 0x1C; break;
   case 'C': item_type = 0x26; break;
   case 'D': item_type = 0x28; break;
   case 'E': item_type = 0x03; break;
   case 'F': item_type = 0x25; break;
   case 'G': item_type = 0x27; break;
   case 'H': item_type = 0x06; break;
   case 'I': item_type = 0x31; break;
   default: return;
   }
   if (item_type == data[4]) return;
   init_item_data(data,item_type);
}

void change_item_type(char data[14]) /* 變更物品型態 */
{
   char fun;
   printf("\n1. Weapon\n2. Armor\n3. Spell\n4. Misc\n\nSelect: ");
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
      printf("Item: ");
      print_item_data(0,data,false);
      printf("\n\n+. Add Magic Power\n-. Dec Magic Power\n0. Exit\n\nSelect: ");
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
      printf("Item: ");
      print_item_data(0,data,false);
      printf("\n\n+. Next Magic\n-. Prev Magic\n0. Exit\n\nSelect: ");
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
   printf("\n\nSelect: ");
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
   bound_inc_mode_value(data,1,25);
}

void select_cleric_scroll(char data[14]) /* 選擇牧師卷軸法術 */
{
   bound_inc_mode_value(data,1,23);
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
      else if (item_type == 0x30) select_wand(data);
      break;
   }
}

bool change_item(char data[14]) /* 變更物品 */
{
   char fun;
   for (;;)
   {
      clrscr();
      printf("Item: ");
      print_item_data(0,data,false);
      printf("\n\n1. Change Item Type\n2. Change Item Mode\n3. Charge Wand\n9. Save and Exit\n0. Abort and Exit\n\nSelect: ");
      fun = getch();
      printf("%c\n",fun);
      data[2] |= 0x40;
      switch(fun)
      {
      case '1': change_item_type(data); break;
      case '2': change_item_mode(data); break;
      case '3': if (data[4] == 0x30) data[2] |= 0x1F; break;
      case '0': return false;
      case '9': return true;
      }
   }
}

void create_item(int user_index, int slot) /* 建立物品 */
{
   char data[14];
   int pos, file;
   short i;
   file = open_file();
   for (i=1; i<500; ++i) /* 找出空的物品 */
   {
      read_item_data(i,file,data);
      if (((unsigned char)data[6] == 0xFF) && ((unsigned char)data[7] == 0xFF)) break;
   }
   close(file);
   if (i >= 500) return;
   init_item_data(data,0);
   if (!change_item(data)) return;
   write_item_data(i,data);
   file = open_file();
   pos = user_index*0xf3;
   lseek(file,pos+0x7B+slot*2,SEEK_SET);
   write(file,&i,2);
   close(file);
}

void modify_items(int index) /* 修改物品 */
{
   int i, pos, file;
   char s[80], fun;
   short item_index[14];
   pos = index*0xf3;
   for (;;)
   {
      clrscr();
      file = open_file();
      lseek(file,pos+2,SEEK_SET);
      read(file,s,10);
      printf("***** Items In %s (%d) *****\n",s,index+1);
      lseek(file,pos+0x7B,SEEK_SET);
      read(file,item_index,14*2);
      for (i=0; i<14; ++i)
      {
         printf("\n%c. ",i+'A');
         print_item_name(item_index[i],file,false);
      }
      close(file);
      printf("\n\n1. Create Item\n0. Exit\n\nSelect: ");
      fun = getch();
      printf("%c\n",fun);
      switch(fun)
      {
      case '0': return;
      case '1':
         for (i=0; i<14; ++i)
            if (item_index[i] == 0) break;
         if (i >= 14) break;
         create_item(index,i);
         break;
      default:
         fun = toupper(fun)-'A';
         if ((fun < 0) || (fun >= 14)) break;
         if (item_index[fun] == 0) break;
         file = open_file();
         read_item_data(item_index[fun],file,s);
         close(file);
         if (!change_item(s)) break;
         write_item_data(item_index[fun],s);
         break;
      }
   }
}

void modify_char(int index) /* 修改人物 */
{
   int i, pos, race_id, class_index, class_id, each_class[3], id, file;
   char s[80], attrb[8*2], level[3], fun;
   long exp[3];
   pos = index*0xf3;
   for (;;)
   {
      clrscr();
      file = open_file();
      lseek(file,pos+2,SEEK_SET);
      read(file,s,10);
      printf("***** Character: %s (%d) *****\n\n",s,index+1);
      lseek(file,pos+0x0d,SEEK_SET);
      read(file,attrb,8*2);
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
      printf("\nLevel=");
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
      printf(", Exp=");
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
      printf("\n\n STR  INT WIS DEX CON CHA HP\n");
      if (attrb[3] != 0) printf("%2d/%02d",attrb[1],attrb[3]%100);
      else printf(" %2d  ",attrb[1]);
      for (i=2; i<8; ++i) printf(" %2d ",attrb[i*2+1]);
      printf("\n\n1. Change Race\n2. Change Class\n3. Max Attribute\n4. Max Level/HP\n5. Learn All Mage Spell\n6. Max All\n7. Modify Items\n0. Exit\n\nSelect: ");
      fun = getch();
      printf("%c\n",fun);
      switch(fun)
      {
      case '0': return;
      case '1': change_race(index); break;
      case '2': change_class(index); break;
      case '3': max_attribute(index); break;
      case '4': max_level(index); break;
      case '5': max_spell(index); break;
      case '6': max_all(index); break;
      case '7': modify_items(index); break;
      }
   }
}

void main_menu(void) /* 顯示主選單 (人物) */
{
   int i, pos, file;
   char s[20], fun;
   for (;;)
   {
      clrscr();
      printf("**** Eye of the Beholder, Character Editor V1.00 by Chi'u I-Nan **** \n\nCharacter:\n");
      file = open_file();
      for (i=0; i<6; ++i)
      {
         pos = i*0xf3;
         lseek(file,pos+2,SEEK_SET);
         read(file,s,10);
         if (s[0] == 0) break;
         printf("  %d. %s\n",i+1,s);
      }
      close(file);
      printf("  0. Exit\n\nSelect: ");
      fun = getch();
      printf("%c\n",fun);
      fun -= '0';
      if ((fun < 0) || (fun > i)) continue;
      if (fun == 0) return;
      modify_char(fun-1);
   }
}

void dump_all_items(void)
{
   int i, file;
   file = open_file();
   for (i=1; i<500; ++i)
   {
      print_item_name(i,file,true);
      printf("\n");
   }
   close(file);
}

void main(void)
{
   int file;
   file = open_file(); /* 確認存檔存在 */
   if (file <= 0)
   {
      printf("Can not open %s\n",SAVE_FILE);
      return;
   }
   close(file);
   if (!load_data()) return;
   main_menu();
   /*dump_all_items();*/
   system("pause");
}
