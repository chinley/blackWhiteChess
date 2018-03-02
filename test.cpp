///////////////////////////////////////////////////////////
// 程序名称：翻转棋
// 编译环境：Visual C++ 2010/6.0，EasyX_v20120304(beta)
// 程序编写：黄春丽 673072090@qq.com
// 最后更新：2017-7-9
//
////////////////////////////////////////////////////////////


#include <Graphics.h>	// EasyX库
#include <strstream>
#include <ctime>
#include <fstream>
#include <stdio.h>
#include <conio.h>


#pragma comment(lib, "Winmm.lib")
#define T(c) ((c == 'B') ? 'W' : 'B')
using namespace std;

/*******************************定义全局变量*****************************/

const int difficult = 6;	// 难度
const int move[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1},
{-1, -1}, {1, -1}, {1, 1}, {-1, 1}};
// 八个方向扩展
int size=8;
char map[100][100];				// 棋盘
IMAGE img[5];				// 保存图片
int black, white;			// 双方的棋子数
int X, Y;					// 白棋的下子点
int player;					//输入"先下"or"后下"的变量
char playercolor;           //玩家的棋色
char computercolor;			//计算机的棋色
int mouseX;				   //鼠标的X值
int mouseY;	               //鼠标的Y值
bool isMouseDown;

/**********************************函数声明*****************************/
void load(void);				// 加载素材
void print(void);				// 画棋盘
void draw(int, int, char);		// 下当前子
int judge(int, int, char);		// 判断当前是否可以落下
bool baidu(char);				// 判断是否有棋可吃
bool quit(char);				// 判断是否有棋存活
bool ask(void);					// 弹出对话框
int D(char, int);				// 动态规划
void play(void);				// 游戏过程
bool isIn(int x,int y,int blockX1,int blockX2,int blockY1,int blockY2); //坐标是否在某个范围里
void save(void);                //存档
bool read(void);				//读档



/**********************************定义函数*****************************/


bool IsIn(int x, int y, int blockX1,int blockX2,int blockY1,int blockY2)
{
	return ((x >= blockX1 && x <=blockX2) && (y >= blockY1 && y <=  blockY2));
}

void load(void)		// 加载素材
{
	// 加载图片
	loadimage(&img[0], "图片\\空位.bmp");
	loadimage(&img[1], "图片\\黑子.bmp");
	loadimage(&img[2], "图片\\白子.bmp");
	loadimage(&img[3], "图片\\黑子1.bmp");
	loadimage(&img[4], "图片\\白子1.bmp");
	
	// 加载音乐
	mciSendString("open 音乐\\梁祝.mp3", NULL, 0, NULL);
	mciSendString("open 音乐\\和局.wma", NULL, 0, NULL);
	mciSendString("open 音乐\\胜利.wma", NULL, 0, NULL);
	mciSendString("open 音乐\\失败.wma", NULL, 0, NULL);
	mciSendString("open 音乐\\下子.wma", NULL, 0, NULL);
	
	// 初始化棋盘
	initgraph(640, 480);
	IMAGE qipan;
	loadimage(&qipan, "图片\\棋盘.bmp");
	putimage(0, 0, &qipan);
	IMAGE beijing;
	loadimage(&beijing,"图片\\白色.bmp");
	putimage(0,0,&beijing);
	IMAGE begin;
	loadimage(&begin,"图片\\begin.bmp");
	putimage(450,140,&begin);
	IMAGE save;
	loadimage(&save,"图片\\save.bmp");
	putimage(455,200,&save);
	IMAGE quit;
	loadimage(&quit,"图片\\quit.bmp");
	putimage(450,260,&quit);
	
	
	setorigin(40, 40);
	SetWindowText(GetHWnd(), "翻转棋");
	
	
}


void print(void)	// 画棋盘
{
	
	int x, y;
	black = white = 0;
	for(x = 0; x < size; x++)
		for(y = 0; y < size; y++)
			switch(map[x][y])
		{
				case 0:
					putimage(37 * y, 37 * x, &img[0]);
					break;
				case 'B':
					putimage(37 * y, 37 * x, &img[1]);
					black++;
					break;
				case 'W':
					putimage(37 * y, 37 * x, &img[2]);
					white++;
					break;
		}
}

void draw(int x, int y, char a)	// 下当前子
{
	char b = T(a);									// 敌方子
	int i, x1, y1, x2, y2;
	bool sign;			
	for (i = 0; i < 8; i++)
	{
		sign = false;//一直往某个方向走，没有棋子都标记为false
		x1 = x + move[i][0];
		y1 = y + move[i][1];    //(x1,y1)指向当前点的八个方向
		while (0 <= x1 && x1 < size && 0 <= y1 && y1 < size && map[x1][y1]) //当Map[x1][y1]不为空，x1,y1在棋盘内时
		{
			if(map[x1][y1] == b)  //如果是同色的棋子，标号为真，如果相邻有同色的棋子
				sign = true;
			else//当棋子不是同色的时候
			{
				if(sign)//当有同色的棋子与x,y相邻时，而有异色的棋子与x,y相邻
				{
					x1 -= move[i][0];//第一次x1,y1回到有同色棋子的店
					y1 -= move[i][1];
					x2 = x + move[i][0];//x2,y2在x,y点的move[i]方向
					y2 = y + move[i][1];
					while (((x <= x2 && x2 <= x1) || (x1 <= x2 && x2 <= x)) && ((y <= y2 && y2 <= y1) || (y1 <= y2 && y2 <= y)))
					{
						map[x2][y2] = a;
						x2 += move[i][0];
						y2 += move[i][1];
					}
				}
				break;
			}
			x1 += move[i][0];//当未发现有相邻的同色棋，发现相邻的异色棋时，向当前方向走
			y1 += move[i][1];
		}
	}
	map[x][y] = a;
}

int judge(int x, int y, char a)	// 判断当前是否可以落下，同draw函数
{
	if(map[x][y])						// 如果当前不是空的返回0值
		return 0;
	char b = T(a);
	int i, x1, y1;
	int n = 0, sign;
	for (i = 0; i < 8; i++)
	{
		sign = 0;
		x1 = x + move[i][0];
		y1 = y + move[i][1];
		while (0 <= x1 && x1 < size && 0 <= y1 && y1 < size && map[x1][y1])
		{
			if(map[x1][y1] == b)  //如果不是同色的棋子
				sign++;
			else
			{
				n += sign;     //是同色的，记录进n
				break;
			}
			x1 += move[i][0];
			y1 += move[i][1];
		}
	}
	return n;		// 返回可吃棋数
}

bool baidu(char c)	// 判断是否有棋可吃
{
	int x, y;
	for(x = 0; x < size; x++)
		for(y = 0; y < size; y++)
			if(judge(x, y, c))
				return true;
			return false;
}

bool quit(char c)	// 判断是否有棋存活 
{
	int x, y;
	bool b = false, w = false;
	for(x = 0; x < size; x++)
		for(y = 0; y < size; y++)
		{
			if(map[x][y] == c)
				return false;
		}
		return true;
}

bool ask(void)	// 弹出对话框
{
	HWND wnd = GetHWnd();
	int key;
	char str[50];
	ostrstream strout(str, 50);
	strout <<"黑：" <<black <<"  白：" <<white <<endl;
	if (black == white)
		strout <<"平局";
	else if(black > white)
	{
		if(playercolor=='W')
		{
			strout<<"小样，还想赢我。";
		}
		else if(playercolor=='W')
		{
			strout <<"恭喜你赢了！";
		}
	}
	else
	{
		if(playercolor=='W')
		{
			strout<<"恭喜你赢了！";
		}
		else if(playercolor=='W')
		{
			strout <<"小样，还想赢我。";
		}
	}
	strout <<"\n再来一局吗？" <<ends;
	if(black == white)
		key = MessageBox(wnd, str, "和局", MB_YESNO | MB_ICONQUESTION);
	else if(black > white)
		key = MessageBox(wnd, str, "黑胜", MB_YESNO | MB_ICONQUESTION);
	else
		key = MessageBox(wnd, str, "白胜", MB_YESNO | MB_ICONQUESTION);
	if(key == IDYES)
		return true;
	else
		return false;
}

int D(char c, int step)
{
	// 判断是否结束递归
	if (step > difficult)	// 约束步数之内
		return 0;
	if (!baidu(c)) //当没有C对应的颜色棋可以吃的时候
	{
		if (baidu(T(c)))//如果敌方的棋可以吃
			return -D(T(c), step);//返回负的最优解
		else
			return 0;
	}
	
	int i, j, max = 0, temp, x, y;
	bool ans = false;
	
	// 建立临时数组
	char **t = new char *[size];
	for (i = 0; i < size; i++)
		t[i] = new char [size];
	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
			t[i][j] = map[i][j];
		
		// 搜索解法
		for (i = 0; i < size; i++)
			for (j = 0; j < size; j++)
				if (temp = judge(i, j, c))//是否可以落下，temp是可吃棋数
				{
					draw(i, j, c);//下子
					temp -= D(T(c), step + 1);
					if (temp > max || !ans)//找到最大的可吃棋数
					{
						max = temp;
						x = i;
						y = j;
						ans = true;
					}
					for (int k = 0; k < size; k++)
						for (int l = 0; l < size; l++)
							map[k][l] = t[k][l];
				}
				
				// 撤销空间
				for (i = 0; i < size; i++)
					delete [] t[i];
				delete [] t;
				
				// 如果是第一步则标识白棋下子点
				if (step == 1)
				{
					X = x;
					Y = y;
				}
				
				return max;	// 返回最优解
}
void save(void)
{
	ofstream fout("存档.dat");
	fout <<computercolor		<<'\t'
		<<playercolor	<<'\t'
		<<endl;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
			fout <<map[i][j] <<'\t';
		fout <<endl;
	}
	fout.close();
	HWND wnd = GetHWnd();
	MessageBox(wnd, "保存成功!", "保存游戏", MB_OK);
	exit(0);
}

bool read(void)
{
	ifstream fin("存档.dat");
	if (!fin)
		throw -1;	// 如果打开失败则抛出异常
	
	// 读存档
	HWND wnd = GetHWnd();
	SetWindowText(wnd, "翻转棋");
	if (MessageBox(wnd, "是否继续上次游戏？","游戏开始",MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		fin >>computercolor >>playercolor;
		for (int i = 0; i < size; i++)
		{	for (int j = 0; j < size; j++)
		{
			char t;
			fin >>t;
			map[i][j] =t;
		}
		}
		fin.close();
		return true;
		
	}
	else
		return false;
	
}


void play(void)		
{
	MOUSEMSG m;
	int x, y;
	// 开始游戏
	print();
	mciSendString("play 音乐\\梁祝.mp3 from 0 repeat", NULL, 0, NULL);
	if(playercolor=='B'&&computercolor=='W')
	{
		do
		{
			if (baidu('B'))										// 如果玩家有下子位置							
			{
A:
			while(true)
			{
				m = GetMouseMsg();
				mouseX = m.x;   mouseY = m.y;// 获取鼠标消息
				if(m.uMsg == WM_LBUTTONDOWN && m.x - 26 < 37 * 8 && m.y - 26 < 37 * 8)
					// 如果左键点击
					break;
				if(m.uMsg == WM_LBUTTONDOWN && IsIn(mouseX, mouseY,455,600,260,300))
				{
					exit(0);
					
				}
				if(m.uMsg == WM_LBUTTONDOWN && IsIn(mouseX, mouseY,455,600,200,250))
				{
					//写存档
					save();
					break;
				}
			}
			
			x = (m.y - 40) / 37;
			y = (m.x - 40) / 37;
			if(judge(x, y,'B'))							// 如果当前位置有效
			{
				draw(x, y,'B');							// 下子
				mciSendString("play 音乐\\下子.wma from 0", NULL, 0, NULL);
				print();
				putimage(37 * y, 37 * x, &img[3]);	               	// 标识下子点
				LOGFONT f;
				gettextstyle(&f);                     // 获取当前字体设置
				f.lfHeight = 15;                      // 设置字体高度为 48
				_tcscpy(f.lfFaceName, _T("黑体"));    // 设置字体为“黑体”(高版本 VC 推荐使用 _tcscpy_s 函数)
				f.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
				settextstyle(&f);                     // 设置字体样式
				char s[5];
				sprintf(s, "%d", x+1);
			//	outtextxy(450, 20, "下子的横坐标:");
				outtextxy(450, 40, s);
				outtextxy(460, 40,',');
				sprintf(s, "%d", y+1);
			//	outtextxy(450, 55, "下子的纵坐标:");
				outtextxy(470, 40, s);
			}
			else
				goto A;
			if (quit('W'))							// 计算机是否失败
				break;
			}
			if (baidu('W'))										// 如果计算机有下子位置
			{
				clock_t start;
				start = clock();
				D('W', 1);										// 搜索解法
				while (clock() - start < CLOCKS_PER_SEC);
				draw(X, Y, 'W');
				print();
				mciSendString("play 音乐\\下子.wma from 0", NULL, 0, NULL);
				putimage(37 * Y, 37 * X, &img[4]);
				// 标识下子点
				
				if (quit('B'))									// 玩家是否失败
					
					break;
				
			}
			
		}while (baidu('B') || baidu ('W'));
	}
	else 
	{	do
	{
		if (baidu('B'))										// 如果计算机有下子位置
		{
			clock_t start;
			start = clock();
			D('B', 1);										// 搜索解法
			while (clock() - start < CLOCKS_PER_SEC);
			draw(X, Y, 'B');
			print();
			mciSendString("play 音乐\\下子.wma from 0", NULL, 0, NULL);
			putimage(37 * Y, 37 * X, &img[3]);
			// 标识下子点
			if (quit('W'))									// 玩家是否失败
				break;
		}
		if (baidu('W'))										// 如果玩家有下子位置							
		{
C:
		while(true)
		{
			m = GetMouseMsg();
			mouseX = m.x; mouseY = m.y;// 获取鼠标消息
			if(m.uMsg == WM_LBUTTONDOWN && m.x - 26 < 37 * 8 && m.y - 26 < 37 * 8)
			{	// 如果左键点击
				break;
			}
			if(m.uMsg == WM_LBUTTONDOWN && IsIn(mouseX, mouseY,455,600,260,300))
			{
				exit(0);
				
			}
			if(m.uMsg == WM_LBUTTONDOWN &&  IsIn(mouseX, mouseY,455,600,200,250))
			{
				save();
				break;
				
			}
		}
		x = (m.y - 40) / 37;
		y = (m.x - 40) / 37;
		if(judge(x, y,'W'))							// 如果当前位置有效
		{
			draw(x, y,'W');							// 下子
			mciSendString("play 音乐\\下子.wma from 0", NULL, 0, NULL);
			print();
			putimage(37 * y, 37 * x, &img[4]);	               	// 标识下子点
			
		}
		else
			goto C;
		
		if (quit('B'))							// 计算机是否失败
			break;
		}
	}while (baidu('B') || baidu ('A'));
	}
	// 播放庆祝音乐
	mciSendString("stop 音乐\\梁祝.mp3", NULL, 0, NULL);
	if(playercolor=='B'&&computercolor=='W')
	{	if (black > white)
	mciSendString("play 音乐\\胜利.wma from 0", NULL, 0, NULL);
	else if (black < white)
		mciSendString("play 音乐\\失败.wma from 0", NULL, 0, NULL);
	else
		mciSendString("play 音乐\\和局.wma from 0", NULL, 0, NULL);
	}
	else
	{	if (black > white)
	mciSendString("play 音乐\\失败.wma from 0", NULL, 0, NULL);
	else if (black < white)
		mciSendString("play 音乐\\胜利.wma from 0", NULL, 0, NULL);
	else
		mciSendString("play 音乐\\和局.wma from 0", NULL, 0, NULL);
	}
}




// 主函数
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	
	MOUSEMSG mmsg;
	load();
	
	
	while(true)
	{
		//处理鼠标消息
		while(MouseHit())
		{
			
			mmsg = GetMouseMsg();
			switch(mmsg.uMsg)
			{
			case WM_MOUSEMOVE:		 mouseX = mmsg.x;  mouseY = mmsg.y;	break;
			case WM_LBUTTONDOWN:	 isMouseDown = true;				break;
			case WM_LBUTTONUP:		 isMouseDown = false;				break;
			}
		}
		if(IsIn(mouseX, mouseY,450,600,140,190))
		{
			if(isMouseDown)
			{
				
				if(read())
				{
					
					play();
					isMouseDown=false;
					continue;
				}
				else
				{	do{
					//定义字符串缓冲区，接受用户输入
					char s[10];
					InputBox(s,10,"请输入棋盘大小(>4,默认为8):");
					
					//将用户输入转换为数字
					sscanf(s,"%d",&size);
					
					char a[10];
					InputBox(a,10,"请选择先下还是后下，先下为黑子，后下为白子(0为先下，1为后下):");
					sscanf(a,"%d",&player);
					if(player==0)
					{
						playercolor='B';
						computercolor='W';
					}
					else if(player==1)
					{
						playercolor='W';
						computercolor='B';
					}
					else
					{
						InputBox(a,10,"输入错误，请重新输入(0为先下，1为后下):");
						sscanf(a,"%d",&player);
					}
					int x, y;
					
					// 初始化棋子
					for(x = 0; x < size; x++)
					{	for(y = 0; y < size; y++)
					{		map[x][y] = 0;
					map[(size-1)/2][(size+1)/2] = map[(size+1)/2][(size-1)/2] = 'B';
					map[(size-1)/2][(size-1)/2] = map[(size+1)/2][(size+1)/2] = 'W';
					}
					}
					
					play();
				}while(ask());
				}
				
			}
		}
		if(IsIn(mouseX, mouseY,450,600,260,300))
		{
			if(isMouseDown)
			{
				exit(0);
			}	
		}
		if(IsIn(mouseX, mouseY,455,600,200,250))
		{
			if(isMouseDown)
			{
				
				
				
				HWND wnd = GetHWnd();
				MessageBox(wnd, "游戏未开始，保存失败!","保存游戏",MB_OK);
				isMouseDown=false;
				continue;
				
			}
			
		}
		
		
		
	}
	
	
	
	
	// 关闭音乐
	
	mciSendString("close 音乐\\梁祝.mp3", NULL, 0, NULL);
	mciSendString("close 音乐\\和局.wma", NULL, 0, NULL);
	mciSendString("close 音乐\\胜利.wma", NULL, 0, NULL);
	mciSendString("close 音乐\\失败.wma", NULL, 0, NULL);
	mciSendString("close 音乐\\下子.wma", NULL, 0, NULL);
	
	closegraph();
	return 0;
}

/***********************************THE END************************************/