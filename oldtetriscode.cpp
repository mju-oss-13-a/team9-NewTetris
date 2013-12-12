#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
 
#define LEFT 0
#define RIGHT 1
#define DOWN 2
#define ROTATE 3
 
 
//���� �Լ�
void rotate_tetris();
void clear_line();
void next_draw();
void preview_cp();
void now_cp();
void join_down();
void sto_draw();
void board_draw(bool start);
void gotoxy(int, int);
void Cursor(bool);

bool check();
bool move_ck(int move);

void Cursor(bool _TF)
{
   HANDLE hOut;
   CONSOLE_CURSOR_INFO CurInfo;
 
   hOut = GetStdHandle(STD_OUTPUT_HANDLE);
   CurInfo.dwSize = 1;
   CurInfo.bVisible = _TF;
   SetConsoleCursorInfo(hOut,&CurInfo);
} //�ܼ�â���� Ŀ�� ���̱�,����� �Լ�

void gotoxy(int x, int y)
{
   HANDLE hOut;
   COORD Cur;
 
   hOut = GetStdHandle(STD_OUTPUT_HANDLE);
   Cur.X = x;
   Cur.Y = y;
   SetConsoleCursorPosition(hOut, Cur);
} // ��ǥ�� ��Ÿ���� �Լ�
 
//���� ����
int board_bg[12][22];
int board_tetris[12][22];
int board_preview[12][22];   // ���� �������� - �������� ũ�� ���� 
int preview_board[7][4];    // ���� ��� ������ â ũ�� ����

int cx, cy, cr;  // ���� ���� ��ġ, ȸ��
 
// I, O, L, J, T, S, Z
int colors[7] = {11, 14, 5, 9, 5, 10, 12}; // ��Ʈ�ι̳� �� ����

int now_tetris; // ���� ��Ʈ����
int next_tetris;    // ���� ��Ʈ����
int sto_tetris=-1;
 
bool store = true;
 
int point_get()
{
   int r, c, B_x, B_y;
   short int tetris = preview_board[now_tetris][cr];
   bool tetris_b;
   for(r=1;r<=18;r++)
   {
      for(c=0;c<16;c++)
      {
         tetris_b = tetris & (1 << (15-c));
         if(tetris_b)
         {
            B_y = c / 4;
            B_x = c % 4;
 
         if(board_bg[cx + B_x][r + B_y])
         return r;
         }
      }
   }
}

void join_down()
{
   for(int r=1;r<11;r++)
   {
      for(int c=1;c<21;c++)
      {
         if(board_tetris[r][c] != 0)
         {
            board_bg[r][c] = board_tetris[r][c];
         }
      }
   }
}
 
void clear_line() // ���ӿ��� ������ ����� �Լ�
{
   int r, c, i;
   bool clear;
   for(r=20;r>=1;r--)
   {
      clear = true;
      for(c=1;c<=10;c++)
         if(board_bg[c][r] == 0)
         { //��ĭ�� �ִٸ�
            clear=false;
            break; 
            printf("Level : %d", r);
         }
 
         if(clear)
         {
         // ��ĭ�� ���ٸ�
         for(c=r;c>=2;c--)
			for(i=1;i<=10;i++)
				board_bg[i][c] = board_bg[i][c-1];
			for(i=1;i<=10;i++)
				board_bg[i][1] = 0;
 
				r++;
         }
   }
}
 
bool check()
{
   int r;
 
   for(r=1;r<=10;r++)
		if(board_bg[r][1] != 0) 
			return true;
 
			return false;
}
 
int main(int argc, char* argv[])
{
   bool now = false;
 
   rotate_tetris();
   srand(clock());
   Cursor(0);
 
   board_draw(true);
 
   now_tetris = rand() % 7;
   next_tetris = rand() % 7; // ���� ��Ʈ����, ���� ��Ʈ���� ����
 
   next_draw();
 
   char keyvalue;
 
   clock_t s_time = clock();
   clock_t e_time;  //  �ð��� ���� �Լ�
 
    cx = 4; cy = -1;
    cr = 0;
 
   while(1)
   {
 
   if(_kbhit())  //Ű���� �̺�Ʈ ó��
   {
      keyvalue = _getch();
 
   switch(keyvalue)
   {
   case 'k' :case 'K': // k�� ���� �� ����� ��������.
      cy = point_get()-1;
      preview_cp();
      now_cp();
      cy++;
      now = true;
      break;
      
   case 'j': case 'J': //j�� ���� �� ���� �̵�
      cx--;
      if(move_ck(LEFT))
      cx++;
      else 
      now = true;
      break;
      
      case 'l':case 'L': // l�� ���� �� ������ �̵�
      cx++;
      if(move_ck(RIGHT))cx--;
      else now = true;
      break;
 
      case 'i':case 'I': // i�� ���� �� ������ ȸ��
      //ȸ�� �������� �����ϰ�
      //�����ϸ� ȸ���Ѵ�.
      cr = (cr+1) % 4;
      if(move_ck(ROTATE))
      cr = (cr-1) % 4;
      else 
      now = true;
      break;
      case ',': // �Ʒ��� ��ĭ �̵�
      cy++;
      now = true;
      break;
      }
   }
   e_time = clock();
   if((float)(e_time - s_time) / (float)CLOCKS_PER_SEC >=0.5
   ^ now == true)
   {
      // ������ �ð��� now �� �ϳ��� true�� �� ����
 
   s_time=clock();
 
   if(!now)
    cy++;
   // �浹 �˻�
   now = false;
   if(!move_ck(DOWN))
    now_cp();
   else 
   {
		now_tetris = next_tetris;
		next_tetris = rand() % 7;
		next_draw();
		cx = 4; cy = -1;
		cr=0;
		join_down();
		clear_line();
		if(check())
		{
			gotoxy(20, 10);
    
			printf("========== Game Over ==========");
    
			gotoxy(40, 23);
			// printf("\n");
			 exit(0);
		}
		store = true;
		now_cp();
	}
	board_draw(false);

  }
 }
 
 return 0;
}
 
void now_cp()
{
	memcpy(board_tetris, board_preview, sizeof(int) * 12 * 22);
	return;
} // ���� ��Ʈ���� ��� ����
 
void preview_cp()
{
	int n_x, n_y;
	int r, j;
	memset(board_preview, 0, sizeof(int) * 12 * 22);
 
	short int tetris = preview_board[now_tetris][cr];
 
	for(r=0;r<16;r++)
	{
		bool tetris_b = tetris & (1 << (15-r)); // ��Ʈ�ι̳� �м�
		if(tetris_b)
		{
			n_y = r / 4;
			n_x = r % 4;
			board_preview[cx+n_x][cy+n_y] = colors[now_tetris];
		}
	}
} // �̸����� ��Ʈ���� ��� ����
 
bool move_ck(int move)
{
 // ������ �浹 �Ǵ�
 // �̹� �׿��� ���� �浹 �Ǵ�
 // true�� ��ȯ�Ǹ� �浹, false�� ��ȯ�Ǹ� �⵹ �ƴ�
 
	int r,c;
	preview_cp();
 
	if(move==ROTATE)
	{
	// ���۷����̼�
 
		if(cx < 5) // ���� ���� �浹
		{
			for(r=1;r<22;r++)
			while(board_bg[0][r] && board_preview[0][r])
			{
				cx++;
				preview_cp();
				break;
			}
		} 
		else 
		{ // ���� ���� �浹
			for(r=1;r<22;r++)
			while(board_bg[11][r] && board_preview[11][r])
			{
				cx--;
				preview_cp();
			}
 
		}
	}
 
	for(r=0;r<12;r++)
		for(c=1;c<22;c++)
			if(board_bg[r][c] && board_preview[r][c])
				return true;
 
				return false;
}
 
void sto_draw()
{
	int cx, cy;
	short int tetris = preview_board[sto_tetris][0];
	cx = 5; cy = 2;
 
	for(int r=0;r<16;r++)
	{
		bool tetris_b = tetris & (1 << (15-r));
 
		gotoxy(cx, cy);
		if(tetris_b)
		printf("��");
		else
		printf("  ");
		cx+=2;
 
		if((r-3) % 4 == 0)
		{
			cx = 5;
			cy++;
		}
	}
} // ��Ʈ���� �׸���
 

 
void next_draw()
{
	int cx, cy;
	short int tetris = preview_board[next_tetris][0];
	cx = 42; cy = 2;
 
 
 
	for(int r=0;r<16;r++)
	{
		bool tetris_b = tetris & (1 << (15-r));
 
		gotoxy(cx, cy);
		if(tetris_b)
			printf("��");
		else
			printf("  ");
			cx+=2;
 
		if((r-3) % 4 == 0)
		{
			cx = 42;
			cy++;
		}
	}
} // ���� ��Ʈ���� �׸���

void rotate_tetris()
{
	int r, c;
 
	// 7���� ��Ʈ�ι̳븦 4����(ȸ��)��ǥ��
	int ro_tetris[7][4] = {{0x4444, 0x00f0, 0x4444, 0x00f0}, // I
	{0x0660, 0x0660, 0x0660, 0x0660}, // O
	{0x02e0, 0x6220, 0x0e80, 0x4460}, // L 
	{0x08e0, 0x2260, 0x0e20, 0x6440}, // J
	{0x0e40, 0x4640, 0x4e00, 0x4c40}, // T
	{0x06c0, 0x4620, 0x06c0, 0x4620}, // S
	{0xc600, 0x2640, 0xc600, 0x2640}}; // Z
 
	memcpy(preview_board, ro_tetris, sizeof(int) * 4 * 7);
 
	for(r=0;r<12;r++)
		for(c=0;c<22;c++)
			board_bg[r][c] = 7;
 
	for(r=1;r<11;r++)
		for(c=1;c<21;c++)
			board_bg[r][c] = 0;
 
	return;
}

void board_draw(bool start)
{
	int r, c;
 
	// field
 
	int st_r = 15, ed_r = 27;
	int st_c = 0, ed_c = 22;
 
	if(!start)
	{
		st_r++; ed_r--;
		st_c++; ed_c--;
	}
 
	for(r=st_r;r<ed_r;r++)
		for(c=st_c;c<ed_c;c++)
		{
			gotoxy(r * 2 - 14, c+1);
			if(board_bg[r-15][c]) //�Ʒ����� ������ ��Ÿ��
			{
					printf("��");
			} 
		else if(board_tetris[r-15][c])
		{
				printf("��");
		}
		else
			printf("  ");
 
		}
 
		if(!start)return;

		//next
		gotoxy(40, 1); printf("�������");
		gotoxy(40, 2); printf("��        ��");
		gotoxy(40, 3); printf("��        ��");
		gotoxy(40, 4); printf("��        ��");
		gotoxy(40, 5); printf("��        ��");
		gotoxy(40, 6); printf("�������");
 
	return;
} // ��Ʈ���� ������ �׸���