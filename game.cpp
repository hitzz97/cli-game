#include <bits/stdc++.h>
#define _WIN32_WINNT 0x0601
#include <windows.h>
#include <graphics.h>
#include <dos.h>
#include <conio.h>
#include <thread>
#include <chrono>
#include <stdio.h>

using namespace std;
int main();
typedef struct _CONSOLE_FONT_INFOEX
{
    ULONG cbSize;
    DWORD nFont;
    COORD dwFontSize;
    UINT  FontFamily;
    UINT  FontWeight;
    WCHAR FaceName[LF_FACESIZE];
}CONSOLE_FONT_INFOEX, *PCONSOLE_FONT_INFOEX;

int WIDTH = 600, W=79;
int HEIGHT = 400, H=20;
//int WIDTH = 800, W=106;
//int HEIGHT = 600, H=32;
void printDisk();
COORD coord={0,0}; // this is global variable
                                    //center of axis is set to the top left cornor of the screen
 void gotoxy(int x,int y)
 {
    coord.X=x;
    coord.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
 }

void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

void clrscr(){
    gotoxy(0,0);
    for(int i=0; i < H ; i++){
        for (int j=0;j < W; j++){
            cout << " ";
        }
    }
    gotoxy(0,0);
}
#ifdef __cplusplus
extern "C" {
#endif
BOOL WINAPI SetCurrentConsoleFontEx(HANDLE hConsoleOutput, BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX
lpConsoleCurrentFontEx);
#ifdef __cplusplus
}
#endif

void makeWindow(){
    HWND con = GetConsoleWindow();
    RECT rect;
    GetWindowRect(con,&rect);
    MoveWindow(con , rect.left,rect.top,WIDTH,HEIGHT,TRUE);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    W=columns;
    H=rows-1;

    int newWidth=8,newHeight=16;
    CONSOLE_FONT_INFOEX fontStructure={0};
    fontStructure.cbSize=sizeof(fontStructure);
    fontStructure.dwFontSize.X=newWidth;
    fontStructure.dwFontSize.Y=newHeight;
    HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    SetCurrentConsoleFontEx(hConsole, true, &fontStructure);
//    SetConsoleTextAttribute(outcon, 0x0C);
}

int rds=10,rde=14,lds=10,lde=14;
char upr='\'',dwnr='/',upl='a',dwnl='z';
map<pair<int,int>,int> balls = {{{39,10},1} };   // position , 1
map<pair<int,int>,pair<int,int>> slope = {{{39,10},{1,1}}};   // position , direction/slope
int LEFT_WINS=0,RIGHT_WINS=0,HITS=0,MISS=6-(balls.size());;


void printDisk(){
    for(int i=1;i<H-1;i++){
        gotoxy(W-3,i);
        if(rds<=i && i<rde)printf("|#");
        else printf("  ");

        gotoxy(1,i);
        if(lds<=i && i<lde)printf("#|");
        else printf("  ");
    }
}

void instructions(){
    gotoxy(W/2-4,1); cout<<"DISK DASH";
    gotoxy(W/2-18,2); cout<<"HITS = No's of hits before new BALL";
    gotoxy(W/2-18,3); cout<<"MISS = No's of miss before BALL disappears";

    gotoxy(W/2-18,6);cout<<"NOTE: DO NOT RESIZE THE WINDOW";

    gotoxy(W/2-18,10);cout<<"Left Disk up: A Down: Z";
    gotoxy(W/2-18,11);cout<<"Right Disk up: ' Down: /";
    gotoxy(W/2-18,12);cout<<"Press any key to continue...";

    gotoxy(W/2-4,18);cout<<"by HITZZ97";
    _getch();
}

void start(){
    gotoxy(0,0);
    printf(" ");
    for(int j=1 ; j< W-1; j++)printf("_");               ///upper bound
    printf(" ");

    for(int i = 0 ; i < H-2; i++){
        printf("|");                                    ///left bound
        for(int j = 0 ; j< W-2; j++){
            printf(" ");
        }
        printf("|");                                    ///right bound
    }

    printf("|");
    for(int j=1 ; j< W-1; j++)printf("_");             ///bottom bound
    printf("|");

    printDisk();
}

void updateBall(){
    while(1){
        for(auto i=balls.begin();i!=balls.end();i++){
            gotoxy(i->first.first,i->first.second);
            printf(" ");
            int xSlope=slope[i->first].first, ySlope=slope[i->first].second;
            int x = xSlope + i->first.first;
            int y = ySlope + i->first.second;
            if(HITS==4){
                HITS=0;
                pair<int,int> b = make_pair((rand()+20)%50,(rand()+3)%15);
                balls[b]=1;
                int xs=(rand())%2,ys=(rand())%2;
                if (xs==0)xs=1;
                if (ys==0)ys=1;
                slope[b]={xs,ys};
            }
            if(MISS==0){
                pair<int,int> b;
                b=(--balls.end())->first;
                gotoxy(b.first,b.second);printf(" ");
                balls.erase(b);
                slope.erase(b);
                if(balls.begin()==balls.end()){
                    gotoxy(W/2-10,10);
                    cout<<"GAME OVER..       ";
                    gotoxy(W/2-20,11);

                }
                MISS=6-balls.size();
                if(MISS<=0)MISS=1;
            }
            if(x<=1){
                if(lds<=y && lde>=y)HITS++;
                else{
                    if(y<=0 || y>=W);
                    else{
                    gotoxy(W/2 - 7,H/2);
                    ++LEFT_WINS;
                    printf("LEFT:%d RIGHT:%d   ",LEFT_WINS,RIGHT_WINS);
                    MISS--;
                    }
                }
            }
            if(x>=W-2){
                if(rds<=y && rde>=y)HITS++;
                else{
                    if(y<=0 || y>=W);
                    else{
                    gotoxy(W/2 - 7,H/2);
                    ++RIGHT_WINS;
                    printf("LEFT:%d RIGHT:%d   ",LEFT_WINS,RIGHT_WINS);
                    MISS--;
                    }
                }
            }
            if(x<=1 || x>=W-2){
                x=x-xSlope;
                xSlope=-xSlope;
                x=x+xSlope;
            }
            if(y<=0 || y>=H-1){
                y=y-ySlope;
                ySlope=-ySlope;
                y=y+ySlope;
            }

            gotoxy(x,y);
            printf("O");
            slope[{x,y}] = {xSlope,ySlope};
            slope.erase(i->first);
        }
        balls.erase(balls.begin(),balls.end());
        for(auto i=slope.begin();i!=slope.end();i++){
            balls[i->first]=1;
        }

        gotoxy(W/2-9,1);printf("HITS:%d MISS:%d     ",HITS,MISS);
        printDisk();
        std::this_thread::sleep_for (std::chrono::milliseconds((rand()+10)%80));
    }
}

void updateDisk(){
    while(1){
        char x = _getch();
        if(x == upr && (rds-1)>=1){
            rde-=2;rds-=2;
        }
        else if(x == dwnr && (rde-1)<= H-1){
            rde+=2;rds+=2;
        }
        if(x == upl && (lds-1)>=1){
            lde-=2;lds-=2;
        }
        else if(x == dwnl && (lde-1)<= H-1){
            lde+=2;lds+=2;
        }

    }
}

int main(){
    makeWindow();
    ShowConsoleCursor(false);

    start();
    instructions();
    start();

    thread Disk (updateDisk);
    thread Ball (updateBall);
    Disk.join();
    Ball.join();

    return 0;
}
