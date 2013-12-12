#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
 
#define LEFT 75
#define RIGHT 77
#define DOWN 80
#define SPACE 32
#define UP 72
 
 
//전역 함수
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
void setColor(int, int);
void shadow_draw(int y);
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
} //콘솔창에서 커서 보이기,숨기기 함수

void gotoxy(int x, int y)
{
   HANDLE hOut;
   COORD Cur;
 
   hOut = GetStdHandle(STD_OUTPUT_HANDLE);
   Cur.X = x;
   Cur.Y = y;
   SetConsoleCursorPosition(hOut, Cur);
} // 좌표를 나타내는 함수

inline int _drgb ( bool d, bool r, bool g, bool b)
{
   return (d << 3) + (r << 2) + (g << 1) + (b << 0);
} // 색 함수를 사용 할 때 필요한 함수
 
inline int _clr ( int fc, int bc )
{
   return (fc << 0) + (bc << 4);
} // 색 함수를 사용 할 떄 필요한 함수
 
void setColor(int fore_color, int back_color)
{
   HANDLE hOut;
   hOut = GetStdHandle(STD_OUTPUT_HANDLE);
 
   int color = _clr(fore_color, back_color);
   SetConsoleTextAttribute(hOut, color);
} // 색 넣는 함수 정의

//전역 변수
int board_bg[12][22];
int board_tetris[12][22];
int board_preview[12][22];   // 위의 세개포함 - 게임판의 크기 선언 
int preview_board[7][4];    // 다음 모양 나오는 창 크기 선언

int cx, cy, cr;  // 현재 블럭의 위치, 회전
 
// I, O, L, J, T, S, Z
int colors[7] = {9, 10, 11, 12, 13, 14, 6}; // 테트로미노 색 설정

int now_tetris; // 현재 테트리스
int next_tetris;    // 다음 테트리스
int sto_tetris=-1;
 
bool store = true;
bool shado_draw = true;
 
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
 
void clear_line() // 게임에서 라인을 지우는 함수
{
   int r, c, i;
   bool clear;
   for(r=20;r>=1;r--)
   {
      clear = true;
      for(c=1;c<=10;c++)
         if(board_bg[c][r] == 0)
         { //빈칸이 있다면
            clear=false;
            break; 
            printf("Level : %d", r);
         }
 
         if(clear)
         {
         // 빈칸이 없다면
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

   clock_t begin, end; // 게임을 진행 할 떄 시간을 재는 변수 선언
   begin = clock();    // 게임 시작
   double all_clock;   // 게임 진행 후 총 시간 출력 변수 선언
 
   rotate_tetris();
   srand(clock());
   Cursor(0);
 
   board_draw(true);
 
   now_tetris = rand() % 7;
   next_tetris = rand() % 7; // 현재 테트리스, 다음 테트리스 랜덤
 
   next_draw();
 
   char keyvalue;
 
   clock_t s_time = clock();
   clock_t e_time;  //  시간에 관한 함수
 
    cx = 4; cy = -1;
    cr = 0;
 
   while(1)
   {
 
   if(_kbhit())  //키보드 이벤트 처리
   {
      keyvalue = _getch();
 
   switch(keyvalue)
   {
   case SPACE: // spsce bar 를 누를 시 모양이 내려간다.
      cy = point_get()-1;
      preview_cp();
      now_cp();
      cy++;
      now = true;
      break;
      
   case LEFT: //방향키 왼쪽을 누를 시 도형 왼쪽 이동
      cx--;
      if(move_ck(LEFT))
      cx++;
      else 
      now = true;
      break;
      
   case RIGHT:// 방향키 오른쪽 을 누를 시 도형 오른쪽 이동
      cx++;
      if(move_ck(RIGHT))cx--;
      else now = true;
      break;
 
   case UP: // 방향키 위로를 누를 시 도형이 회전
      //회전 가능한지 조사하고
      //가능하면 회전한다.
      cr = (cr+1) % 4;
      if(move_ck(UP))
      cr = (cr-1) % 4;
      else 
      now = true;
      break;
      case ',': // 아래로 한칸 이동
      cy++;
      now = true;
      break;
      }
   }
   e_time = clock();
   if((float)(e_time - s_time) / (float)CLOCKS_PER_SEC >=0.5
   ^ now == true)
   {
      // 정해진 시간과 now 중 하나만 true일 때 진입
 
   s_time=clock();
 
   if(!now)
    cy++;
   // 충돌 검사
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
			setColor(0,15);
    
			printf("========== Game Over ==========");
			setColor(7,0);

			end = clock(); // 게임 종료 시간
			all_clock = (double)(end-begin)/(double)CLOCKS_PER_SEC; // 총 게임시간 출력
			gotoxy(0,0);
			printf(" 총 플레이 시간은 %lf 초 입니다. ", all_clock);

			gotoxy(40, 23);
			// printf("\n");
			 exit(0);
		}
		store = true;
		now_cp();
	}
	board_draw(false);
	shadow_draw(point_get());
  }
 }
 
 return 0;
}
 
void now_cp()
{
	memcpy(board_tetris, board_preview, sizeof(int) * 12 * 22);
	return;
} // 현재 테트리스 모양 복사
 
void preview_cp()
{
	int n_x, n_y;
	int r, j;
	memset(board_preview, 0, sizeof(int) * 12 * 22);
 
	short int tetris = preview_board[now_tetris][cr];
 
	for(r=0;r<16;r++)
	{
		bool tetris_b = tetris & (1 << (15-r)); // 테트로미노 분석
		if(tetris_b)
		{
			n_y = r / 4;
			n_x = r % 4;
			board_preview[cx+n_x][cy+n_y] = colors[now_tetris];
		}
	}
} // 미리보기 테트리스 모양 복사
 
bool move_ck(int move)
{
 // 벽과의 충돌 판단
 // 이미 쌓여진 블럭과 충돌 판단
 // true가 반환되면 충돌, false가 반환되면 출돌 아님
 
	int r,c;
	preview_cp();
 
	if(move==UP)
	{
	// 슈퍼로테이션
 
		if(cx < 5) // 좌측 벽과 충돌
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
		{ // 우측 벽과 충돌
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
 
	setColor(colors[sto_tetris],0);

	for(int r=0;r<16;r++)
	{
		bool tetris_b = tetris & (1 << (15-r));
 
		gotoxy(cx, cy);
		if(tetris_b)
		printf("■");
		else
		printf("  ");
		cx+=2;
 
		if((r-3) % 4 == 0)
		{
			cx = 5;
			cy++;
		}
	}
} // 테트리스 그리기
 

 
void next_draw()
{
	int cx, cy;
	short int tetris = preview_board[next_tetris][0];
	cx = 42; cy = 2;
 
	setColor(colors[next_tetris],0);
 
	for(int r=0;r<16;r++)
	{
		bool tetris_b = tetris & (1 << (15-r));
 
		gotoxy(cx, cy);
		if(tetris_b)
			printf("■");
		else
			printf("  ");
			cx+=2;
 
		if((r-3) % 4 == 0)
		{
			cx = 42;
			cy++;
		}
	}
} // 다음 테트리스 그리기

void rotate_tetris()
{
	int r, c;
 
	// 7개의 테트로미노를 4가지(회전)로표현
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
			if(board_bg[r-15][c]) //아래와의 조건은 배타적
			{
				setColor(board_bg[r-15][c],0);
				printf("■");
			} 
		else if(board_tetris[r-15][c])
		{
			setColor(board_tetris[r-15][c],0);
			printf("■");
		}
		else
			printf("  ");
 
		}
 
		if(!start)return;

		//next
		setColor(15,0);
		gotoxy(40, 1); printf("■■■■■■");
		gotoxy(40, 2); printf("■        ■");
		gotoxy(40, 3); printf("■        ■");
		gotoxy(40, 4); printf("■        ■");
		gotoxy(40, 5); printf("■        ■");
		gotoxy(40, 6); printf("■■■■■■");
 
		gotoxy(41, 12);
		printf("Key Help:");
		gotoxy(41, 14);
		printf("<- : Move Left");
		gotoxy(41, 16);
		printf("-> : Move Right");
        gotoxy(41, 18);
		printf("↑: Rotate");
		gotoxy(41, 20);
		printf("SpaceBar: Drop");

	return;
} // 테트리스 게임판 그리기

void shadow_draw(int y)
{
	short int tetris = preview_board[now_tetris][cr];
 
	int B_x, B_y;
	bool tetris_b;
	setColor(8,0);
	for(int i=0;i<16;i++)
	{
		tetris_b = tetris & (1 << (15-i));
		if(tetris_b)
		{
			B_y = i / 4;
			B_x = i % 4;
 
			gotoxy((cx + B_x)*2 + 16, y + B_y);
			if(tetris_b && !board_tetris[cx+B_x][y+B_y-1])
			printf("■");
		}
	}
} // 그림자 테트리스 그리기