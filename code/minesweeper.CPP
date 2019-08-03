//INCLUDE FILES
#include<iostream.h>
#include<time.h>
#include<conio.h>
#include<fstream.h>
#include<graphics.h>
#include<stdio.h>
#include<stdlib.h>
#include<dos.h>
#include<iomanip.h>
#include<string.h>

//GLOBAL VARIABLES
union REGS i,o;

//CLASSES
class times
{
	protected:
		int time;
	public:
		times()
		{}
};

class mines
{
	protected:
		int minesleft;
	public:
		mines(){}
		mines(int m)
		{
			minesleft=m;
		}
		int operator++(int)
		{
			int m;
			m=minesleft++;
			return m;
		}
		int operator--(int)
		{
			int m;
			m=minesleft--;
			return m;
		}
};

class score:public times,public mines
{
	friend class mainbox;
	protected:
		char st[15];
	public:
		score(){}
		score(int m):mines(m)
		{}
};

class mainbox:public score
{
	protected:
		int sbn[100][100];
		int lx,ly,rx,ry;
		int rows,columns;
		int nmines;
		int *minelistx;
		int *minelisty;
		static long t;
		clock_t cst,cet;
	public:
		mainbox(int lx,int ly,int rx,int ry,int rows,int columns,int nmines);
		void drawb();
		void display();
		void leftclick(int,int);
		void rightclick(int,int);
		void assign();
		int ismine(int r,int c);
		int calcr(int x,int y);
		int calcc(int x,int y);
		void reveal(int r,int c);
		void gameover();
		void revealboard();
		void revealsb(int r,int c);
		void predictb(int x,int y);
		void reveal0(int r,int c);
		void displaytm();
		int gameend();
		void winmsg();
		void storescore();
		void startclock();
		int stopclock();
};

//MAIN
void main()
{
	struct textsettingstype *oldt;
	int lx,ly,rx,ry,mx,my,x,y,b,bp;
	char stop='g';
	int gd=DETECT,gm,ge;
	initgraph(&gd,&gm,"");
	if((ge=graphresult())!=grOk)
	{
		cout<<grapherrormsg(ge);
		flushall();
		getch();
		exit(0);
	}
	mx=getmaxx();
	my=getmaxy();
	cleardevice();
	gettextsettings(oldt);
	setbkcolor(YELLOW);
	moveto(mx/2,my/2);
	settextstyle(1,HORIZ_DIR,2);
	setcolor(RED);
	outtextxy(90,90,"OOPS PROJECT");
	settextstyle(GOTHIC_FONT,HORIZ_DIR,6);
	setcolor(RED);
	outtextxy(mx/2-textwidth("MINESWEEPER")/2,my/2-textheight("MINESWEEPER")/2-34,"MINESWEEPER");
	settextstyle(SMALL_FONT,HORIZ_DIR,4);
	setcolor(RED);
	outtextxy(394,344,"BY");
	outtextxy(407,380,"T.V.ARUN RAM(2753570)");
	outtextxy(407,395,"S.J.JEBA JERIN(2753583)");
	outtextxy(407,410,"R.PON NARESH(2753593)");
	outtextxy(407,425,"L.VIJAY(27501379)");
	outtextxy(407,440,"C.NITIN PAUL(27501372)");
	outtextxy(407,455,"S.ROBIN CHANDRA(27501375)");
	flushall();
	getch();
	cleardevice();
	settextstyle(oldt->font,oldt->direction,oldt->charsize);
	setbkcolor(LIGHTGREEN);
	lx=mx/2-245;
	rx=mx/2+125;
	ly=my/2-185;
	ry=my/2+185;
	mainbox a(lx,ly,rx,ry,20,20,50);
	a.drawb();
	gettextsettings(oldt);
	setcolor(LIGHTGRAY);
	settextstyle(GOTHIC_FONT,HORIZ_DIR,1);
	outtextxy(200,25,"MINESWEEPER");
	settextstyle(oldt->font,oldt->direction,oldt->charsize);
	i.x.ax=0;
	int86(0x33,&i,&o);
	i.x.ax=1;
	int86(0x33,&i,&o);
	a.startclock();
	while(stop=='g')
	{
		i.x.ax=3;
		int86(0x33,&i,&o);
		b=o.x.bx;
		x=o.x.cx;
		y=o.x.dx;
		if(b==1)
		{
			a.leftclick(x,y);
		}
		else if(b==2)
		{

			a.rightclick(x,y);
		}
		if(a.gameend())
		{
			a.winmsg();
			getch();
			break;
		}
		a.displaytm();
		if(kbhit())
		{
			if((stop=getch())!=27)
				stop='g';
		}
	}
	closegraph();
}

//FUNCTIONS
mainbox::mainbox(int lx,int ly,int rx,int ry,int rows,int columns,int nmines):score(nmines)
{
	int i,j,k;
	randomize();
	minelistx=new int[nmines];
	minelisty=new int[nmines];
	for(i=0;i<nmines;i++)
	{
		minelistx[i]=NULL;
		minelisty[i]=NULL;
	}
	this->lx=lx;
	this->ly=ly;
	this->rx=rx;
	this->ry=ry;
	this->rows=rows;
	this->columns=columns;
	this->nmines=nmines;
	for(i=0;i<nmines;i++)
	{
		minelistx[i]=random(rows);
		minelisty[i]=random(columns);
		for(j=0;j<i;j++)
		{
			if(minelistx[j]==minelistx[i] && minelisty[j]==minelisty[i])
			{
				i--;
				break;
			}
		}
	}
	assign();
}

void mainbox::drawb()
{
	int i,j;
	setcolor(MAGENTA);
	rectangle(lx,ly,rx,ry);
	setcolor(WHITE);
	for(i=0;i<columns;i++)
	{
		setcolor(MAGENTA);
		line(lx+i*(float)(rx-lx)/columns,ly,lx+i*(float)(rx-lx)/columns,ry);
		setcolor(WHITE);
	}
	for(i=0;i<rows;i++)
	{
		setcolor(MAGENTA);
		line(lx,ly+i*(float)(ry-ly)/rows,rx,ly+i*(float)(ry-ly)/rows);
		setcolor(WHITE);
	}
	display();
}

void mainbox::display()
{
	int i,j;
	setfillstyle(SOLID_FILL,WHITE);
	for(i=0;i<columns;i++)
	{
		for(j=0;j<rows;j++)
		{
			floodfill(lx+i*((float)(rx-lx)/columns)+1,ly+j*((float)(ry-ly)/rows)+1,MAGENTA);
		}
	}
}

void mainbox::assign()
{
	int i,j,k,c;
	for(i=0;i<rows;i++)
	{
		for(j=0;j<columns;j++)
		{
			if(ismine(i,j)==1)
				sbn[i][j]=-1;
			else
				sbn[i][j]=ismine(i-1,j-1)+ismine(i-1,j)+ismine(i-1,j+1)+ismine(i,j-1)+ismine(i,j+1)+ismine(i+1,j-1)+ismine(i+1,j)+ismine(i+1,j+1);
		}
	}
}

int mainbox::ismine(int r,int c)
{
	int i;
	if(r<0 || c<0 || r>=rows || c>=columns)
		return 0;
	for(i=0;i<nmines;i++)
	{
		if(minelistx[i]==r && minelisty[i]==c)
		{
			return 1;
		}
	}
	return 0;
}

void mainbox::leftclick(int x,int y)
{
	int r1,c1;
	int co;
	i.x.ax=2;
	int86(0x33,&i,&o);
	co=getpixel(x,y);
	i.x.ax=1;
	int86(0x33,&i,&o);
	r1=calcr(x,y);
	c1=calcc(x,y);
	if(co==WHITE)
	{
		if(sbn[r1][c1]==-1)
			gameover();
		reveal(r1,c1);
	}
}
void mainbox::rightclick(int x1,int y1)
{
	int r1,c1,co;
	r1=calcr(x1,y1);
	c1=calcc(x1,y1);
	i.x.ax=2;
	int86(0x33,&i,&o);
	co=getpixel(lx+c1*((float)(rx-lx)/columns)+1,ly+r1*((float)(ry-ly)/rows)+1);
	i.x.ax=1;
	int86(0x33,&i,&o);
	if(co==WHITE)
	{
		predictb(x1,y1);
		minesleft--;
	}
	else if(co==LIGHTBLUE)
	{
		minesleft++;
		i.x.ax=2;
		int86(0x33,&i,&o);
		setfillstyle(SOLID_FILL,WHITE);
		floodfill(lx+c1*((float)(rx-lx)/columns)+1,ly+r1*((float)(ry-ly)/rows)+1,MAGENTA);
		i.x.ax=1;
		int86(0x33,&i,&o);
	}
}

int mainbox::calcc(int x,int y)
{
	int c1;y;
	if(x<=lx || x>=rx)
	{
		return -1;
	}
	c1=(float)(x-lx)/((float)(rx-lx)/columns);
	return c1;
}
int mainbox::calcr(int x,int y)
{
	int r1;x;
	if(y<=ly || y>=ry)
	{
		return -1;
	}
	r1=(float)(y-ly)/((float)(ry-ly)/rows);
	return r1;
}

void mainbox::reveal(int r1,int c1)
{
	char n[2];
	i.x.ax=2;
	int86(0x33,&i,&o);
	if(sbn[r1][c1]==0)
		reveal0(r1,c1);
	else
	{
		setfillstyle(SOLID_FILL,GREEN);
		floodfill(lx+c1*((float)(rx-lx)/columns)+1,ly+r1*((float)(ry-ly)/rows)+1,MAGENTA);
		itoa(sbn[r1][c1],n,10);
		outtextxy(lx+c1*((float)(rx-lx)/columns)+((float)(rx-lx)/columns)/2-textwidth(n)/2,ly+r1*((float)(ry-ly)/rows)+((float)(ry-ly)/rows)/2-textheight(n)/2,n);
	}
	i.x.ax=1;
	int86(0x33,&i,&o);
}

void mainbox::gameover()
{
	int i,mx,my;
	revealboard();
	flushall();
	getch();
	cleardevice();
	mx=getmaxx()/2-200;
	my=getmaxy()/2;
	for(i=0;i<15;i++)
	{
		delay(50);
		cleardevice();
		settextstyle(SMALL_FONT,HORIZ_DIR,i);
		outtextxy(mx,my,"U HAV LOST THE GAME!");
	}
	getch();
	closegraph();
}

void mainbox::revealboard()
{
	int i,j;
	for(i=0;i<rows;i++)
	{
		for(j=0;j<columns;j++)
			revealsb(i,j);
	}
}

void mainbox::revealsb(int r1,int c1)
{
	char n[2];
	i.x.ax=2;
	int86(0x33,&i,&o);
	if(!ismine(r1,c1))
	{
		setfillstyle(SOLID_FILL,GREEN);
		floodfill(lx+c1*((float)(rx-lx)/columns)+1,ly+r1*((float)(ry-ly)/rows)+1,MAGENTA);
		itoa(sbn[r1][c1],n,10);
		outtextxy(lx+c1*((float)(rx-lx)/columns)+((float)(rx-lx)/columns)/2-textwidth(n)/2,ly+r1*((float)(ry-ly)/rows)+((float)(ry-ly)/rows)/2-textheight(n)/2,n);
	}
	else
	{
		if(getpixel(lx+c1*((float)(rx-lx)/columns)+1,ly+r1*((float)(ry-ly)/rows)+1)!=LIGHTBLUE)
		{
			setfillstyle(SOLID_FILL,RED);
			floodfill(lx+c1*((float)(rx-lx)/columns)+1,ly+r1*((float)(ry-ly)/rows)+1,MAGENTA);
		}
	}

	i.x.ax=1;
	int86(0x33,&i,&o);
}

void mainbox::predictb(int x1,int y1)
{
	char *n="pb";
	int r1,c1;
	r1=calcr(x1,y1);
	c1=calcc(x1,y1);
	i.x.ax=2;
	int86(0x33,&i,&o);
	setfillstyle(SOLID_FILL,LIGHTBLUE);
	floodfill(x1,y1,MAGENTA);
	outtextxy(lx+c1*((float)(rx-lx)/columns)+((float)(rx-lx)/columns)/2-textwidth(n)/2,ly+r1*((float)(ry-ly)/rows)+((float)(ry-ly)/rows)/2-textheight(n)/2,n);
	i.x.ax=1;
	int86(0x33,&i,&o);
}

void mainbox::reveal0(int r1,int c1)
{
	int co;
	i.x.ax=2;
	int86(0x33,&i,&o);
	co=getpixel(lx+c1*((float)(rx-lx)/columns)+1,ly+r1*((float)(ry-ly)/rows)+1);
	if(co==GREEN)
	{
		i.x.ax=1;
		int86(0x33,&i,&o);
		return;
	}
	setfillstyle(SOLID_FILL,GREEN);
	floodfill(lx+c1*((float)(rx-lx)/columns)+1,ly+r1*((float)(ry-ly)/rows)+1,MAGENTA);
	outtextxy(lx+c1*((float)(rx-lx)/columns)+((float)(rx-lx)/columns)/2-textwidth("0")/2,ly+r1*((float)(ry-ly)/rows)+((float)(ry-ly)/rows)/2-textheight("0")/2,"0");
	i.x.ax=1;
	int86(0x33,&i,&o);
	if(sbn[r1-1][c1-1]==0 && (r1-1)>=0 && (r1-1)<rows && (c1-1)>=0 && (c1-1)<columns)
	{
		reveal0(r1-1,c1-1);
	}
	else if(sbn[r1-1][c1-1]!=0 && (r1-1)>=0 && (r1-1)<rows && (c1-1)>=0 && (c1-1)<columns)
	{
		reveal(r1-1,c1-1);
	}
	if(sbn[r1-1][c1]==0 && (r1-1)>=0 && (r1-1)<rows && (c1)>=0 && (c1)<columns)
	{
		reveal0(r1-1,c1);
	}
	else if(sbn[r1-1][c1]!=0 && (r1-1)>=0 && (r1-1)<rows && (c1)>=0 && (c1)<columns)
	{
		reveal(r1-1,c1);
	}
	if(sbn[r1-1][c1+1]==0 && (r1-1)>=0 && (r1-1)<rows && (c1+1)>=0 && (c1+1)<columns)
	{
		reveal0(r1-1,c1+1);
	}
	else if(sbn[r1-1][c1+1]!=0 && (r1-1)>=0 && (r1-1)<rows && (c1+1)>=0 && (c1+1)<columns)
	{
		reveal(r1-1,c1+1);
	}
	if(sbn[r1][c1-1]==0 && (r1)>=0 && (r1)<rows && (c1-1)>=0 && (c1-1)<columns)
	{
		reveal0(r1,c1-1);
	}
	else if(sbn[r1][c1-1]!=0 && (r1)>=0 && (r1)<rows && (c1-1)>=0 && (c1-1)<columns)
	{
		reveal(r1,c1-1);
	}
	if(sbn[r1][c1+1]==0 && (r1)>=0 && (r1)<rows && (c1+1)>=0 && (c1+1)<columns)
	{
		reveal0(r1,c1+1);
	}
	else if(sbn[r1][c1+1]!=0 && (r1)>=0 && (r1)<rows && (c1+1)>=0 && (c1+1)<columns)
	{
		reveal(r1,c1+1);
	}
	if(sbn[r1+1][c1-1]==0 && (r1+1)>=0 && (r1+1)<rows && (c1-1)>=0 && (c1-1)<columns)
	{
		reveal0(r1+1,c1-1);
	}
	else if(sbn[r1+1][c1-1]!=0 && (r1+1)>=0 && (r1+1)<rows && (c1-1)>=0 && (c1-1)<columns)
	{
		reveal(r1+1,c1-1);
	}
	if(sbn[r1+1][c1]==0 && (r1+1)>=0 && (r1+1)<rows && (c1)>=0 && (c1)<columns)
	{
		reveal0(r1+1,c1);
	}
	else if(sbn[r1+1][c1]!=0 && (r1+1)>=0 && (r1+1)<rows && (c1)>=0 && (c1)<columns)
	{
		reveal(r1+1,c1);
	}
	if(sbn[r1+1][c1+1]==0 && (r1+1)>=0 && (r1+1)<rows && (c1+1)>=0 && (c1+1)<columns)
	{
		reveal0(r1+1,c1+1);
	}
	else if(sbn[r1+1][c1+1]!=0 && (r1+1)>=0 && (r1+1)<rows && (c1+1)>=0 && (c1+1)<columns)
	{
		reveal(r1+1,c1+1);
	}
}

int mainbox::gameend()
{
	int i1,j1;
	int flag=1;
	for(i1=0;i1<rows;i1++)
	{
		for(j1=0;j1<columns;j1++)
		{
			if(!ismine(i1,j1))
			{
				i.x.ax=1;
				int86(0x33,&i,&o);
				if(getpixel(lx+j1*((float)(rx-lx)/columns)+1,ly+i1*((float)(ry-ly)/rows)+1)==WHITE)
				{
					flag=0;
					break;
				}
				i.x.ax=2;
				int86(0x33,&i,&o);
			}
		}
		if(flag==0)
		{
			break;
		}
	}
	return flag;
}

void mainbox::winmsg()
{
	int i,mx,my;
	cleardevice();
	mx=getmaxx()/2-200;
	my=getmaxy()/2;
	time=stopclock();
	for(i=0;i<15;i++)
	{
		delay(50);
		cleardevice();
		settextstyle(SMALL_FONT,HORIZ_DIR,i);
		outtextxy(mx,my,"U HAV WON THE GAME!");
	}
	storescore();
}
long mainbox::t=0;
void mainbox::displaytm()
{
	int mx,my;
	char *str="TIME = ";
	char *str1;
	char *s;
	strcpy(str1,str);
	t++;
	setcolor(BLUE);
	gotoxy(65,5);
	cout<<"TIME = "<<setfill(0)<<setw(3)<<stopclock();
	gotoxy(65,10);
	cout<<"MINES = "<<setw(3)<<minesleft;
}

void mainbox::storescore()
{
	int t,t1;
	char str2[50],str3[50];
	fstream sc;
	sc.open("score.dat",ios::out|ios::in|ios::app);
	if(!sc)
	{
		cout<<"score file opening error";
		getch();
	}
	cleardevice();
	gotoxy(23,15);
	cout<<"NAME:";
	cin>>st;
	sc<<st<<'\t'<<time<<"\n";
	sc.close();
	fstream sco;
	cleardevice();
	gotoxy(1,1);
	sco.open("score.dat",ios::in);
	cout.setf(ios::left,ios::adjustfield);
	cout<<"\t\t\t"<<"********************************\n";
	cout<<"\t\t\tNAME\t\tCLEARANCE TIME\n";
	cout<<"\t\t\t"<<"********************************\n";
	sco>>str2>>t1;
	while(sco)
	{
		cout<<"\t\t\t"<<setw(20)<<str2<<setw(5)<<t1<<endl;
		sco>>str2>>t1;
	}
	sco.close();
}

void mainbox::startclock()
{
	cst=clock();
}

int mainbox::stopclock()
{
	cet=clock();
	return (cet-cst)/CLK_TCK;
}
