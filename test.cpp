///////////////////////////////////////////////////////////
// �������ƣ���ת��
// ���뻷����Visual C++ 2010/6.0��EasyX_v20120304(beta)
// �����д���ƴ��� 673072090@qq.com
// �����£�2017-7-9
//
////////////////////////////////////////////////////////////


#include <Graphics.h>	// EasyX��
#include <strstream>
#include <ctime>
#include <fstream>
#include <stdio.h>
#include <conio.h>


#pragma comment(lib, "Winmm.lib")
#define T(c) ((c == 'B') ? 'W' : 'B')
using namespace std;

/*******************************����ȫ�ֱ���*****************************/

const int difficult = 6;	// �Ѷ�
const int move[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1},
{-1, -1}, {1, -1}, {1, 1}, {-1, 1}};
// �˸�������չ
int size=8;
char map[100][100];				// ����
IMAGE img[5];				// ����ͼƬ
int black, white;			// ˫����������
int X, Y;					// ��������ӵ�
int player;					//����"����"or"����"�ı���
char playercolor;           //��ҵ���ɫ
char computercolor;			//���������ɫ
int mouseX;				   //����Xֵ
int mouseY;	               //����Yֵ
bool isMouseDown;

/**********************************��������*****************************/
void load(void);				// �����ز�
void print(void);				// ������
void draw(int, int, char);		// �µ�ǰ��
int judge(int, int, char);		// �жϵ�ǰ�Ƿ��������
bool baidu(char);				// �ж��Ƿ�����ɳ�
bool quit(char);				// �ж��Ƿ�������
bool ask(void);					// �����Ի���
int D(char, int);				// ��̬�滮
void play(void);				// ��Ϸ����
bool isIn(int x,int y,int blockX1,int blockX2,int blockY1,int blockY2); //�����Ƿ���ĳ����Χ��
void save(void);                //�浵
bool read(void);				//����



/**********************************���庯��*****************************/


bool IsIn(int x, int y, int blockX1,int blockX2,int blockY1,int blockY2)
{
	return ((x >= blockX1 && x <=blockX2) && (y >= blockY1 && y <=  blockY2));
}

void load(void)		// �����ز�
{
	// ����ͼƬ
	loadimage(&img[0], "ͼƬ\\��λ.bmp");
	loadimage(&img[1], "ͼƬ\\����.bmp");
	loadimage(&img[2], "ͼƬ\\����.bmp");
	loadimage(&img[3], "ͼƬ\\����1.bmp");
	loadimage(&img[4], "ͼƬ\\����1.bmp");
	
	// ��������
	mciSendString("open ����\\��ף.mp3", NULL, 0, NULL);
	mciSendString("open ����\\�;�.wma", NULL, 0, NULL);
	mciSendString("open ����\\ʤ��.wma", NULL, 0, NULL);
	mciSendString("open ����\\ʧ��.wma", NULL, 0, NULL);
	mciSendString("open ����\\����.wma", NULL, 0, NULL);
	
	// ��ʼ������
	initgraph(640, 480);
	IMAGE qipan;
	loadimage(&qipan, "ͼƬ\\����.bmp");
	putimage(0, 0, &qipan);
	IMAGE beijing;
	loadimage(&beijing,"ͼƬ\\��ɫ.bmp");
	putimage(0,0,&beijing);
	IMAGE begin;
	loadimage(&begin,"ͼƬ\\begin.bmp");
	putimage(450,140,&begin);
	IMAGE save;
	loadimage(&save,"ͼƬ\\save.bmp");
	putimage(455,200,&save);
	IMAGE quit;
	loadimage(&quit,"ͼƬ\\quit.bmp");
	putimage(450,260,&quit);
	
	
	setorigin(40, 40);
	SetWindowText(GetHWnd(), "��ת��");
	
	
}


void print(void)	// ������
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

void draw(int x, int y, char a)	// �µ�ǰ��
{
	char b = T(a);									// �з���
	int i, x1, y1, x2, y2;
	bool sign;			
	for (i = 0; i < 8; i++)
	{
		sign = false;//һֱ��ĳ�������ߣ�û�����Ӷ����Ϊfalse
		x1 = x + move[i][0];
		y1 = y + move[i][1];    //(x1,y1)ָ��ǰ��İ˸�����
		while (0 <= x1 && x1 < size && 0 <= y1 && y1 < size && map[x1][y1]) //��Map[x1][y1]��Ϊ�գ�x1,y1��������ʱ
		{
			if(map[x1][y1] == b)  //�����ͬɫ�����ӣ����Ϊ�棬���������ͬɫ������
				sign = true;
			else//�����Ӳ���ͬɫ��ʱ��
			{
				if(sign)//����ͬɫ��������x,y����ʱ��������ɫ��������x,y����
				{
					x1 -= move[i][0];//��һ��x1,y1�ص���ͬɫ���ӵĵ�
					y1 -= move[i][1];
					x2 = x + move[i][0];//x2,y2��x,y���move[i]����
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
			x1 += move[i][0];//��δ���������ڵ�ͬɫ�壬�������ڵ���ɫ��ʱ����ǰ������
			y1 += move[i][1];
		}
	}
	map[x][y] = a;
}

int judge(int x, int y, char a)	// �жϵ�ǰ�Ƿ�������£�ͬdraw����
{
	if(map[x][y])						// �����ǰ���ǿյķ���0ֵ
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
			if(map[x1][y1] == b)  //�������ͬɫ������
				sign++;
			else
			{
				n += sign;     //��ͬɫ�ģ���¼��n
				break;
			}
			x1 += move[i][0];
			y1 += move[i][1];
		}
	}
	return n;		// ���ؿɳ�����
}

bool baidu(char c)	// �ж��Ƿ�����ɳ�
{
	int x, y;
	for(x = 0; x < size; x++)
		for(y = 0; y < size; y++)
			if(judge(x, y, c))
				return true;
			return false;
}

bool quit(char c)	// �ж��Ƿ������� 
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

bool ask(void)	// �����Ի���
{
	HWND wnd = GetHWnd();
	int key;
	char str[50];
	ostrstream strout(str, 50);
	strout <<"�ڣ�" <<black <<"  �ף�" <<white <<endl;
	if (black == white)
		strout <<"ƽ��";
	else if(black > white)
	{
		if(playercolor=='W')
		{
			strout<<"С��������Ӯ�ҡ�";
		}
		else if(playercolor=='W')
		{
			strout <<"��ϲ��Ӯ�ˣ�";
		}
	}
	else
	{
		if(playercolor=='W')
		{
			strout<<"��ϲ��Ӯ�ˣ�";
		}
		else if(playercolor=='W')
		{
			strout <<"С��������Ӯ�ҡ�";
		}
	}
	strout <<"\n����һ����" <<ends;
	if(black == white)
		key = MessageBox(wnd, str, "�;�", MB_YESNO | MB_ICONQUESTION);
	else if(black > white)
		key = MessageBox(wnd, str, "��ʤ", MB_YESNO | MB_ICONQUESTION);
	else
		key = MessageBox(wnd, str, "��ʤ", MB_YESNO | MB_ICONQUESTION);
	if(key == IDYES)
		return true;
	else
		return false;
}

int D(char c, int step)
{
	// �ж��Ƿ�����ݹ�
	if (step > difficult)	// Լ������֮��
		return 0;
	if (!baidu(c)) //��û��C��Ӧ����ɫ����ԳԵ�ʱ��
	{
		if (baidu(T(c)))//����з�������Գ�
			return -D(T(c), step);//���ظ������Ž�
		else
			return 0;
	}
	
	int i, j, max = 0, temp, x, y;
	bool ans = false;
	
	// ������ʱ����
	char **t = new char *[size];
	for (i = 0; i < size; i++)
		t[i] = new char [size];
	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
			t[i][j] = map[i][j];
		
		// �����ⷨ
		for (i = 0; i < size; i++)
			for (j = 0; j < size; j++)
				if (temp = judge(i, j, c))//�Ƿ�������£�temp�ǿɳ�����
				{
					draw(i, j, c);//����
					temp -= D(T(c), step + 1);
					if (temp > max || !ans)//�ҵ����Ŀɳ�����
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
				
				// �����ռ�
				for (i = 0; i < size; i++)
					delete [] t[i];
				delete [] t;
				
				// ����ǵ�һ�����ʶ�������ӵ�
				if (step == 1)
				{
					X = x;
					Y = y;
				}
				
				return max;	// �������Ž�
}
void save(void)
{
	ofstream fout("�浵.dat");
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
	MessageBox(wnd, "����ɹ�!", "������Ϸ", MB_OK);
	exit(0);
}

bool read(void)
{
	ifstream fin("�浵.dat");
	if (!fin)
		throw -1;	// �����ʧ�����׳��쳣
	
	// ���浵
	HWND wnd = GetHWnd();
	SetWindowText(wnd, "��ת��");
	if (MessageBox(wnd, "�Ƿ�����ϴ���Ϸ��","��Ϸ��ʼ",MB_YESNO | MB_ICONQUESTION) == IDYES)
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
	// ��ʼ��Ϸ
	print();
	mciSendString("play ����\\��ף.mp3 from 0 repeat", NULL, 0, NULL);
	if(playercolor=='B'&&computercolor=='W')
	{
		do
		{
			if (baidu('B'))										// ������������λ��							
			{
A:
			while(true)
			{
				m = GetMouseMsg();
				mouseX = m.x;   mouseY = m.y;// ��ȡ�����Ϣ
				if(m.uMsg == WM_LBUTTONDOWN && m.x - 26 < 37 * 8 && m.y - 26 < 37 * 8)
					// ���������
					break;
				if(m.uMsg == WM_LBUTTONDOWN && IsIn(mouseX, mouseY,455,600,260,300))
				{
					exit(0);
					
				}
				if(m.uMsg == WM_LBUTTONDOWN && IsIn(mouseX, mouseY,455,600,200,250))
				{
					//д�浵
					save();
					break;
				}
			}
			
			x = (m.y - 40) / 37;
			y = (m.x - 40) / 37;
			if(judge(x, y,'B'))							// �����ǰλ����Ч
			{
				draw(x, y,'B');							// ����
				mciSendString("play ����\\����.wma from 0", NULL, 0, NULL);
				print();
				putimage(37 * y, 37 * x, &img[3]);	               	// ��ʶ���ӵ�
				LOGFONT f;
				gettextstyle(&f);                     // ��ȡ��ǰ��������
				f.lfHeight = 15;                      // ��������߶�Ϊ 48
				_tcscpy(f.lfFaceName, _T("����"));    // ��������Ϊ�����塱(�߰汾 VC �Ƽ�ʹ�� _tcscpy_s ����)
				f.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
				settextstyle(&f);                     // ����������ʽ
				char s[5];
				sprintf(s, "%d", x+1);
			//	outtextxy(450, 20, "���ӵĺ�����:");
				outtextxy(450, 40, s);
				outtextxy(460, 40,',');
				sprintf(s, "%d", y+1);
			//	outtextxy(450, 55, "���ӵ�������:");
				outtextxy(470, 40, s);
			}
			else
				goto A;
			if (quit('W'))							// ������Ƿ�ʧ��
				break;
			}
			if (baidu('W'))										// ��������������λ��
			{
				clock_t start;
				start = clock();
				D('W', 1);										// �����ⷨ
				while (clock() - start < CLOCKS_PER_SEC);
				draw(X, Y, 'W');
				print();
				mciSendString("play ����\\����.wma from 0", NULL, 0, NULL);
				putimage(37 * Y, 37 * X, &img[4]);
				// ��ʶ���ӵ�
				
				if (quit('B'))									// ����Ƿ�ʧ��
					
					break;
				
			}
			
		}while (baidu('B') || baidu ('W'));
	}
	else 
	{	do
	{
		if (baidu('B'))										// ��������������λ��
		{
			clock_t start;
			start = clock();
			D('B', 1);										// �����ⷨ
			while (clock() - start < CLOCKS_PER_SEC);
			draw(X, Y, 'B');
			print();
			mciSendString("play ����\\����.wma from 0", NULL, 0, NULL);
			putimage(37 * Y, 37 * X, &img[3]);
			// ��ʶ���ӵ�
			if (quit('W'))									// ����Ƿ�ʧ��
				break;
		}
		if (baidu('W'))										// ������������λ��							
		{
C:
		while(true)
		{
			m = GetMouseMsg();
			mouseX = m.x; mouseY = m.y;// ��ȡ�����Ϣ
			if(m.uMsg == WM_LBUTTONDOWN && m.x - 26 < 37 * 8 && m.y - 26 < 37 * 8)
			{	// ���������
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
		if(judge(x, y,'W'))							// �����ǰλ����Ч
		{
			draw(x, y,'W');							// ����
			mciSendString("play ����\\����.wma from 0", NULL, 0, NULL);
			print();
			putimage(37 * y, 37 * x, &img[4]);	               	// ��ʶ���ӵ�
			
		}
		else
			goto C;
		
		if (quit('B'))							// ������Ƿ�ʧ��
			break;
		}
	}while (baidu('B') || baidu ('A'));
	}
	// ������ף����
	mciSendString("stop ����\\��ף.mp3", NULL, 0, NULL);
	if(playercolor=='B'&&computercolor=='W')
	{	if (black > white)
	mciSendString("play ����\\ʤ��.wma from 0", NULL, 0, NULL);
	else if (black < white)
		mciSendString("play ����\\ʧ��.wma from 0", NULL, 0, NULL);
	else
		mciSendString("play ����\\�;�.wma from 0", NULL, 0, NULL);
	}
	else
	{	if (black > white)
	mciSendString("play ����\\ʧ��.wma from 0", NULL, 0, NULL);
	else if (black < white)
		mciSendString("play ����\\ʤ��.wma from 0", NULL, 0, NULL);
	else
		mciSendString("play ����\\�;�.wma from 0", NULL, 0, NULL);
	}
}




// ������
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	
	MOUSEMSG mmsg;
	load();
	
	
	while(true)
	{
		//���������Ϣ
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
					//�����ַ����������������û�����
					char s[10];
					InputBox(s,10,"���������̴�С(>4,Ĭ��Ϊ8):");
					
					//���û�����ת��Ϊ����
					sscanf(s,"%d",&size);
					
					char a[10];
					InputBox(a,10,"��ѡ�����»��Ǻ��£�����Ϊ���ӣ�����Ϊ����(0Ϊ���£�1Ϊ����):");
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
						InputBox(a,10,"�����������������(0Ϊ���£�1Ϊ����):");
						sscanf(a,"%d",&player);
					}
					int x, y;
					
					// ��ʼ������
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
				MessageBox(wnd, "��Ϸδ��ʼ������ʧ��!","������Ϸ",MB_OK);
				isMouseDown=false;
				continue;
				
			}
			
		}
		
		
		
	}
	
	
	
	
	// �ر�����
	
	mciSendString("close ����\\��ף.mp3", NULL, 0, NULL);
	mciSendString("close ����\\�;�.wma", NULL, 0, NULL);
	mciSendString("close ����\\ʤ��.wma", NULL, 0, NULL);
	mciSendString("close ����\\ʧ��.wma", NULL, 0, NULL);
	mciSendString("close ����\\����.wma", NULL, 0, NULL);
	
	closegraph();
	return 0;
}

/***********************************THE END************************************/