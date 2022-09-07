#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <cstdlib>
#include <ctime>
using namespace std;

char const ball='o';
char const bar='-';
char const border='x';
char const bonus='$', b1='+', b2='-', b3='=', b4='~';
char const lifepackage='+';
int qbonus=0, qb1=9, qb2=9, qb3=9, qb4=9;

void gotoxy(int x, int y);
void SetColorAndBackground(int ForgC, int BackC);

void introduction();
void usermanual();

void updatesetup(int lifecount, int level, int score);
void print_border(int width, int height);
void store_bricks(char bricks[][66], int row);
void print_bricks(char bricks[][66], int top, int bottom);

void bar_movement_before_launch(char key, int &left, int &right);
void bar_movement_after_launch(char key, int &left, int &right);
void ball_movement(int &ix, int &iy, int &fx, int &fy, int &left, int &right, char bricks[][66], int &score, int &lifecount, int level, bool &next_interval, bool &endgame, bool &hint, bool &up, bool &down, bool &upright, bool &upleft, bool &downright, bool &downleft, int &qbonus, bool &bonus1, int &bonus1x, int &bonus1y, bool &bonus2, int &bonus2x, int &bonus2y, int &slowdown);
void generate_lifepackage(char design, int &qbonus, bool &bonus1, int &bonus1x, int &bonus1y, bool &bonus2, int &bonus2x, int &bonus2y, int x, int y);
void drop_lifepackage(bool &flag, int &x, int &y, int &left, int&right, int &lifecount, int &score, int &level, char bricks[][66], int &slowdown);

void increment_score(char design, int &score);
bool bringnewlevel(char bricks[][66], int totalbricks, bool bonus1, bool bonus2);

void clear_line(int fromX, int toX, int Y);
void clear_top_block(char bricks[][66], int fx, int fy);
void clear_right_block(char bricks[][66], int fx, int fy);
void clear_left_block(char bricks[][66], int fx, int fy);
void clear_topright_block(char bricks[][66], int fx, int fy);
void clear_topleft_block(char bricks[][66], int fx, int fy);
void clear_bottomright_block(char bricks[][66], int fx, int fy);
void clear_bottomleft_block(char bricks[][66], int fx, int fy);
void clear_bottom_block(char bricks[][66], int fx, int fy);

int main() {
    srand(time(0));

    introduction();
    usermanual();

    char enter;
    while(true) { //block to escape manual and proceed towards playing game
        if(_kbhit) {
            enter=_getch();
            if(enter==' ') {
                system("cls");
                break;
            }
        }
    }

    int lifecount=3, level=0, score=0;
    bool another_one=true;

    while(another_one) { //every level starts from here
        level++;
        Sleep(500);

        updatesetup(lifecount,level,score); //update score count

        print_border(65,22); //game border

        char bricks[7][66];
        store_bricks(bricks,7); //calculates and stores bricks randomly in 2-d array
        print_bricks(bricks, 2, 5);

        SetColorAndBackground(2,0); //initial hint message
        gotoxy(0,23);
        cout<<"Press space-bar to launch the ball.";
        SetColorAndBackground(15,0);

        int left_extreme=23, right_extreme=left_extreme+11; //left and right corner of slider

        SetColorAndBackground(6,0); //hard coding initial location of ball
        gotoxy(left_extreme+5,21);
        cout<<ball;

        SetColorAndBackground(11,0); //hard coding initial location of slider
        for(int i=left_extreme;i<=right_extreme;i++) {
            gotoxy(i,22);
            cout<<bar;
        }
        SetColorAndBackground(15,0);

        bool next=false, start=false, endgame=false; //next clears hint message. start indicates ball and bar's movement after launch. endgame indicates ball count to drop.
        bool up=false, down=false, upright=false, upleft=false, downright=false, downleft=false; //direction bool variables for ball
        bool next_interval=false, hint=true; //next interval is used to transfer ball's direction. hint is initial space messsage at bottom
        char key; //key stores letter entered by user to control slider's movement
        int iX=left_extreme+5,iY=21,fX,fY; //ball's initial position

        bool bonus1=false, bonus2=false; //special brick variables.
        int bonus1x=0, bonus1y=0, bonus2x=0, bonus2y=0; //coordinates of life package if found
        int slowdown=0; //slows falling speed of life package

        while (true) //loop of current level
        {
            if(lifecount==0) {
                if(!bonus1 && !bonus2) {
                    updatesetup(lifecount, level, score);
                    another_one=false;
                    break;
                }
            }
            if(endgame) {
                if(lifecount==0) {
                next=false, start=false, endgame=false, next_interval=false;
                gotoxy(fX,fY);
                cout<<' ';
                SetColorAndBackground(15,0);
                updatesetup(lifecount, level, score);
                }
                else { //resume current level
                next=false, start=false, endgame=false, next_interval=false;
                gotoxy(fX,fY);
                cout<<' ';
                SetColorAndBackground(6,0);
                iX=left_extreme+5, iY=21;
                gotoxy(iX,iY);
                cout<<ball;
                SetColorAndBackground(15,0);
                updatesetup(lifecount, level, score);
                }
            }
            if (_kbhit()) //detects key pressed by user
            {
                key=_getch(); //stores it in variable without printing

                if(!next && key==' ') { //clear's hint
                    next=true;
                    clear_line(0,50,23);
                }
                if(!next) { //ball not launched
                    if(lifecount==0) {
                        bar_movement_after_launch(key, left_extreme, right_extreme);
                        iX=left_extreme+5;
                        drop_lifepackage(bonus1, bonus1x, bonus1y, left_extreme, right_extreme, lifecount, score, level, bricks, slowdown);
                        drop_lifepackage(bonus2, bonus1x, bonus1y, left_extreme, right_extreme, lifecount, score, level, bricks, slowdown);
                        if(bringnewlevel(bricks,36, bonus1, bonus2)) break;
                        }
                    else {
                        bar_movement_before_launch(key, left_extreme, right_extreme);
                        iX=left_extreme+5;
                        drop_lifepackage(bonus1, bonus1x, bonus1y, left_extreme, right_extreme, lifecount, score, level, bricks, slowdown);
                        drop_lifepackage(bonus2, bonus1x, bonus1y, left_extreme, right_extreme, lifecount, score, level, bricks, slowdown);
                        if(bringnewlevel(bricks,36, bonus1, bonus2)) break;
                    }
                }
                else { //ball is launched
                    bar_movement_after_launch(key, left_extreme, right_extreme);
                    if(!start) { //initial push to the ball
                        SetColorAndBackground(6,0);
                        Sleep(100);
                        fX=iX;
                        fY=iY-1;
                        gotoxy(iX,iY);
                            cout<<' ';
                        gotoxy(fX,fY);
                            cout<<ball;
                        start=true;
                        up=true;
                        SetColorAndBackground(15,0);
                    }
                    else {
                        ball_movement(iX, iY, fX, fY, left_extreme, right_extreme, bricks, score, lifecount, level, next_interval, endgame, hint, up, down, upright, upleft, downright, downleft, qbonus, bonus1, bonus1x, bonus1y, bonus2, bonus2x, bonus2y, slowdown);
                        next_interval=false;
                        if(bringnewlevel(bricks,36, bonus1, bonus2)) break;
                        if(endgame) {
                            lifecount--;
                            continue;
                        }
                    }
                }
            } //end of key pressed body
            if(!start) {
                if(next) {
                    SetColorAndBackground(6,0);
                    Sleep(100);
                    fX=iX;
                    fY=iY-1;
                    gotoxy(iX,iY);
                        cout<<' ';
                    gotoxy(fX,fY);
                        cout<<ball;
                    start=true;
                    up=true;
                    SetColorAndBackground(15,0);
                }
                else if(!next && !_kbhit()) {
                    Sleep(100);
                    drop_lifepackage(bonus1, bonus1x, bonus1y, left_extreme, right_extreme, lifecount, score, level, bricks, slowdown);
                    drop_lifepackage(bonus2, bonus2x, bonus2y, left_extreme, right_extreme, lifecount, score, level, bricks, slowdown);
                    if(!bonus1 && !bonus2 && bonus1x!=0) {
                        gotoxy(iX, iY);
                        SetColorAndBackground(6,0);
                        cout<<ball;
                        SetColorAndBackground(15,0);
                        bonus1x=0;
                    }
                    else if(!bonus1 && !bonus2 && bonus2x!=0) {
                        gotoxy(iX, iY);
                        SetColorAndBackground(6,0);
                        cout<<ball;
                        SetColorAndBackground(15,0);
                        bonus2x=0;
                    }
                    if(bringnewlevel(bricks,36, bonus1, bonus2)) break;
                }
            }
            if(start) {
                ball_movement(iX, iY, fX, fY, left_extreme, right_extreme, bricks, score, lifecount, level, next_interval, endgame, hint, up, down, upright, upleft, downright, downleft, qbonus, bonus1, bonus1x, bonus1y, bonus2, bonus2x, bonus2y, slowdown);
                next_interval=false;
                if(bringnewlevel(bricks,36, bonus1, bonus2)) break;
                if (endgame) {
                    lifecount--;
                    continue;
                }
            }
        }//end of current level
    }//end of game

	Sleep(2000);
	system("cls");
	SetColorAndBackground(12,0);
	cout<<"GAME";
	Sleep(500);
	cout<<" OVER"<<endl<<endl;
	Sleep(2000);
	SetColorAndBackground(15,0);
    return 0;
}

void gotoxy(int x, int y) {
	COORD coordinates;     // coordinates is declared as COORD
	coordinates.X = x;     // defining x-axis
	coordinates.Y = y;     //defining  y-axis
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
}

void SetColorAndBackground(int ForgC, int BackC) {
     WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);;
     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
     return;
}

void introduction() {
    SetColorAndBackground(8,0);
    cout<<"BREAK";
    Sleep(500);
    cout<<" THE";
    Sleep(500);
    cout<<" BRICKS";
    Sleep(500);
    clear_line(0,26,0);
    gotoxy(0,0);
    Sleep(500);
    SetColorAndBackground(15,0);
}

void usermanual() {
    SetColorAndBackground(14,0);
    gotoxy(15,1); cout<<"USER MANUAL:";
    gotoxy(15,2); cout<<"____________";

    gotoxy(0,4);
    SetColorAndBackground(4,0);
    cout<<"Motive:"<<endl;
    SetColorAndBackground(3,0);
    cout<<"1. A cluster of bricks is provided with limited lives (3 initially)."<<endl;
    cout<<"2. Your task is to break those bricks using the slider and the ball."<<endl;
    cout<<"3. You can increase your life count by hitting special bricks ($) and catching life package from them."<<endl;
    cout<<"4. Next level starts automatically when all bricks are destroyed."<<endl;
    cout<<"5. Your game will end only when your life count drops to zero."<<endl<<endl;

    SetColorAndBackground(4,0);
    cout<<"How to play?"<<endl;
    SetColorAndBackground(3,0);
    cout<<"1. Press 'a' to move slider leftwards."<<endl;
    cout<<"2. Press 'd' to move slider rightwards."<<endl;
    cout<<"3. Press spacebar to launch the ball."<<endl<<endl;

    SetColorAndBackground(4,0);
    cout<<"Score count of Bricks:"<<endl;

    SetColorAndBackground(3,0);
    cout<<"1. '";
    SetColorAndBackground(2,0);
    cout<<"=";
    SetColorAndBackground(3,0);
    cout<<"' increments your score by '";
    SetColorAndBackground(11,0);
    cout<<1;
    SetColorAndBackground(3,0);
    cout<<"'"<<endl;

    cout<<"2. '";
    SetColorAndBackground(4,0);
    cout<<"~";
    SetColorAndBackground(3,0);
    cout<<"' increments your score by '";
    SetColorAndBackground(11,0);
    cout<<2;
    SetColorAndBackground(3,0);
    cout<<"'"<<endl;

    cout<<"3. '";
    SetColorAndBackground(5,0);
    cout<<"-";
    SetColorAndBackground(3,0);
    cout<<"' increments your score by '";
    SetColorAndBackground(11,0);
    cout<<3;
    SetColorAndBackground(3,0);
    cout<<"'"<<endl;

    cout<<"4. '";
    SetColorAndBackground(8,0);
    cout<<"+";
    SetColorAndBackground(3,0);
    cout<<"' increments your score by '";
    SetColorAndBackground(11,0);
    cout<<4;
    SetColorAndBackground(3,0);
    cout<<"'"<<endl;

    cout<<"5. '";
    SetColorAndBackground(14,0);
    cout<<"$";
    SetColorAndBackground(3,0);
    cout<<"' increments your score by '";
    SetColorAndBackground(11,0);
    cout<<5;
    SetColorAndBackground(3,0);
    cout<<"'. This is the special brick."<<endl;
    cout<<"6. Catching life package '";
    SetColorAndBackground(14,0);
    cout<<"+";
    SetColorAndBackground(3,0);
    cout<<"' increments your score by '";
    SetColorAndBackground(11,0);
    cout<<10;
    SetColorAndBackground(3,0);
    cout<<"'"<<endl<<endl;

    SetColorAndBackground(2,0);
    cout<<"Press SPACEBAR to skip this manual";
    SetColorAndBackground(15,0);
}

void updatesetup(int lifecount, int level, int score) {
    SetColorAndBackground(14,0);
    gotoxy(0,0);
    cout<<"Lifecount:"<<lifecount;
    SetColorAndBackground(12,0);
    gotoxy(15,0);
    cout<<"Level:"<<level;
    SetColorAndBackground(11,0);
    gotoxy(26,0);
    cout<<"Score:"<<score;
    SetColorAndBackground(15,0);
}

void print_border(int width, int height) {
    SetColorAndBackground(3,0);
    gotoxy(0,1);
    for(int i=0;i<height;i++) {
        for(int j=0;j<width;j++) {
            if(i==0)
                cout<<border;
            else {
                if(j==0 || j==width-1)
                    cout<<border;
                else
                    cout<<' ';
            }
        }
    cout<<endl;
    }
    SetColorAndBackground(15,0);
}

void store_bricks(char bricks[][66], int row) {
    qbonus=rand()%3;
    int restore_qb1=qb1, restore_qb2=qb2, restore_qb3=qb3, restore_qb4=qb4, restore_qbonus=qbonus;
    if(qbonus==2) {
        int cutoff=rand()%4; cutoff++;
        if(cutoff==1) qb1--;
        else if(cutoff==2) qb2--;
        else if(cutoff==3) qb3--;
        else if(cutoff==4) qb4--;
        cutoff=rand()%4; cutoff++;
        if(cutoff==1) qb1--;
        else if(cutoff==2) qb2--;
        else if(cutoff==3) qb3--;
        else if(cutoff==4) qb4--;
    }
    else if(qbonus==1) {
        int cutoff=rand()%4; cutoff++;
        if(cutoff==1) qb1--;
        else if(cutoff==2) qb2--;
        else if(cutoff==3) qb3--;
        else if(cutoff==4) qb4--;
    }
    for(int i=2;i<=5;i++) {
        for(int j=1;j<=63;j++) {
            int now=rand()%5;
            int stop=j+6;
            if(now==0 && qbonus>0) {
                while(j<=stop) {
                    bricks[i][j]=bonus;
                    j++;
                    bricks[i][j]='\0';
                }
                j--;
                qbonus--;
            }
            else if(now==1 && qb1>0) {
                while(j<=stop) {
                    bricks[i][j]=b1;
                    j++;
                    bricks[i][j]='\0';
                }
                j--;
                qb1--;
            }
            else if(now==2 && qb2>0) {
                while(j<=stop) {
                    bricks[i][j]=b2;
                    j++;
                    bricks[i][j]='\0';
                }
                j--;
                qb2--;
            }
            else if(now==3 && qb3>0) {
                while(j<=stop) {
                    bricks[i][j]=b3;
                    j++;
                    bricks[i][j]='\0';
                }
                j--;
                qb3--;
            }
            else if(now==4 && qb4>0) {
                while(j<=stop) {
                    bricks[i][j]=b4;
                    j++;
                    bricks[i][j]='\0';
                }
                j--;
                qb4--;
            }
            else j--;
        }
    }
    qb1=restore_qb1, qb2=restore_qb2, qb3=restore_qb3, qb4=restore_qb4, qbonus=restore_qbonus;
    for(int i=0;i<1;i++) {
        for(int j=0;j<65;j++) {
            bricks[i][j]='\0';
        }
    }
    for(int i=0;i<7;i++) {
        bricks[i][65]='\0';
    }
    for(int j=0;j<65;j++) {
        bricks[1][j]='x';
    }
    for(int i=2;i<6;i++) {
        for(int j=0;j<65;j+=64) {
            bricks[i][j]='x';
        }
    }
    for(int j=0;j<65;j++) {
        bricks[6][j]=' ';
    }
}

void print_bricks(char bricks[][66], int top, int bottom) {
    for(int i=top;i<=bottom;i++) {
        for(int j=0+1;j<65-1;j++) {
            gotoxy(j,i);
            if(bricks[i][j]==bonus)
                SetColorAndBackground(14,0);
            else if(bricks[i][j]==b1)
                SetColorAndBackground(8,0);
            else if(bricks[i][j]==b2)
                SetColorAndBackground(5,0);
            else if(bricks[i][j]==b3)
                SetColorAndBackground(2,0);
            else if(bricks[i][j]==b4)
                SetColorAndBackground(4,0);
            cout<<bricks[i][j];
            SetColorAndBackground(15,0);
        }
    }
}

void bar_movement_before_launch(char key, int &left, int &right) {
    if(key=='a' && left>=2) {
        if(left==2) {
            gotoxy(left+5,21); cout<<' ';
            gotoxy(right,22); cout<<' '; right--;


            left--;
            SetColorAndBackground(11,0);
            gotoxy(left,22); cout<<bar;
            SetColorAndBackground(6,0);
            gotoxy(left+5,21); cout<<ball;
        }
        else if(left==3) {
            gotoxy(left+5,21); cout<<' ';
            gotoxy(right,22); cout<<' '; right--;
            gotoxy(right,22); cout<<' '; right--;

            SetColorAndBackground(11,0);
            left--; gotoxy(left,22); cout<<bar;
            left--; gotoxy(left,22); cout<<bar;
            SetColorAndBackground(6,0);
            gotoxy(left+5,21); cout<<ball;
        }
        else {
            gotoxy(left+5,21); cout<<' ';
            gotoxy(right,22); cout<<' '; right--;
            gotoxy(right,22); cout<<' '; right--;
            gotoxy(right,22); cout<<' '; right--;

            SetColorAndBackground(11,0);
            left--; gotoxy(left,22); cout<<bar;
            left--; gotoxy(left,22); cout<<bar;
            left--; gotoxy(left,22); cout<<bar;
            SetColorAndBackground(6,0);
            gotoxy(left+5,21); cout<<ball;
        }
    }
    else if(key=='d' && right<=62) {
        if(right==62) {
            gotoxy(left+5,21); cout<<' ';
            gotoxy(left,22); cout<<' '; left++;

            SetColorAndBackground(11,0);
            right++; gotoxy(right,22); cout<<bar;
            SetColorAndBackground(6,0);
            gotoxy(left+5,21); cout<<ball;
        }
        else if(right==61) {
            gotoxy(left+5,21); cout<<' ';
            gotoxy(left,22); cout<<' '; left++;
            gotoxy(left,22); cout<<' '; left++;

            SetColorAndBackground(11,0);
            right++; gotoxy(right,22); cout<<bar;
            right++; gotoxy(right,22); cout<<bar;
            SetColorAndBackground(6,0);
            gotoxy(left+5,21); cout<<ball;
        }
        else {
            gotoxy(left+5,21); cout<<' ';
            gotoxy(left,22); cout<<' '; left++;
            gotoxy(left,22); cout<<' '; left++;
            gotoxy(left,22); cout<<' '; left++;

            SetColorAndBackground(11,0);
            right++; gotoxy(right,22); cout<<bar;
            right++; gotoxy(right,22); cout<<bar;
            right++; gotoxy(right,22); cout<<bar;
            SetColorAndBackground(6,0);
            gotoxy(left+5,21); cout<<ball;
        }
    }
    SetColorAndBackground(15,0);
}

void bar_movement_after_launch(char key, int &left, int &right) {
    SetColorAndBackground(11,0);
    if(key=='a' && left>=2) {
        if(left==2) {
            gotoxy(right,22); cout<<' '; right--;

            left--; gotoxy(left,22); cout<<bar;
        }
        else if(left==3) {
            gotoxy(right,22); cout<<' '; right--;
            gotoxy(right,22); cout<<' '; right--;

            left--; gotoxy(left,22); cout<<bar;
            left--; gotoxy(left,22); cout<<bar;
        }
        else {
            gotoxy(right,22); cout<<' '; right--;
            gotoxy(right,22); cout<<' '; right--;
            gotoxy(right,22); cout<<' '; right--;

            left--; gotoxy(left,22); cout<<bar;
            left--; gotoxy(left,22); cout<<bar;
            left--; gotoxy(left,22); cout<<bar;
        }
    }
    else if(key=='d' && right<=62) {
        if(right==62) {
            gotoxy(left,22); cout<<' '; left++;

            right++; gotoxy(right,22); cout<<bar;
        }
        else if(right==61) {
            gotoxy(left,22); cout<<' '; left++;
            gotoxy(left,22); cout<<' '; left++;

            right++; gotoxy(right,22); cout<<bar;
            right++; gotoxy(right,22); cout<<bar;
        }
        else {
            gotoxy(left,22); cout<<' '; left++;
            gotoxy(left,22); cout<<' '; left++;
            gotoxy(left,22); cout<<' '; left++;

            right++; gotoxy(right,22); cout<<bar;
            right++; gotoxy(right,22); cout<<bar;
            right++; gotoxy(right,22); cout<<bar;
        }
    }
    SetColorAndBackground(15,0);
}

void ball_movement(int &ix, int &iy, int &fx, int &fy, int &left, int &right, char bricks[][66], int &score, int &lifecount, int level, bool &next_interval, bool &endgame, bool &hint, bool &up, bool &down, bool &upright, bool &upleft, bool &downright, bool &downleft, int &qbonus, bool &bonus1, int &bonus1x, int &bonus1y, bool &bonus2, int &bonus2x, int &bonus2y, int &slowdown) {
    char design;
    SetColorAndBackground(6,0);
    if(up) {
        if(fy<=6 &&fy>=2) {
            if(bricks[fy-1][fx]!='x') {
                if(bricks[fy-1][fx]!=' ') {
                    design=bricks[fy-1][fx]; increment_score(design,score); updatesetup(lifecount,level,score);
                    clear_top_block(bricks,fx,fy);
                    generate_lifepackage(design, qbonus, bonus1, bonus1x, bonus1y, bonus2, bonus2x, bonus2y, fx, fy-1);
                    down=true;
                    up=false;
                    next_interval=true;
                    iy-=2;
                }
            }
            else if(bricks[fy-1][fx]=='x') {
                down=true;
                up=false;
                next_interval=true;
                iy-=2;
            }
        }
        if(!next_interval) {
            Sleep(100);
            fy--; iy--;
            gotoxy(ix,iy); cout<<' ';
            gotoxy(fx,fy); cout<<ball;
        }
    }
    else if(down) {
        if(hint) {
            SetColorAndBackground(2,0);
            gotoxy(0,23);
            cout<<"Hint: edges of bar change the direction of the ball.";
            SetColorAndBackground(6,0);
        }
        if(fy==21) {
            if(fx>=left && fx<=right) {
                if(fx>right-3) {
                    hint=false;
                    clear_line(0,60,23);

                    next_interval=true;
                    down=false;
                    upright=true;
                    iy+=2;
                    ix--;
                }
                else if(fx<left+3) {
                    hint=false;
                    clear_line(0,60,23);

                    next_interval=true;
                    down=false;
                    upleft=true;
                    iy+=2;
                    ix++;
                }
                else {
                    up=true;
                    down=false;
                    next_interval=true;
                    iy+=2;
                }
            }
            else {
                Sleep(100);
                fy++; iy++;
                gotoxy(ix,iy); cout<<' ';
                gotoxy(fx,fy); cout<<ball;
                next_interval=true;
                down=false;
                endgame=true;
            }
        }
        if(!next_interval) {
            Sleep(100);
            fy++; iy++;
            gotoxy(ix,iy); cout<<' ';
            gotoxy(fx,fy); cout<<ball;
        }
    }
    else if(upright) {
        if(fx==63) {
            if(fy<=6 && fy>=2) {
                if(bricks[fy-1][fx]=='x') {
                    upright=false;
                    downleft=true;
                    next_interval=true;
                    iy-=2;
                    ix+=2;
                }
                else {
                    if(bricks[fy-1][fx]!=' ') {
                        design=bricks[fy-1][fx]; increment_score(design,score); updatesetup(lifecount,level,score);
                        clear_top_block(bricks,fx,fy);
                        generate_lifepackage(design, qbonus, bonus1, bonus1x, bonus1y, bonus2, bonus2x, bonus2y, fx, fy-1);
                        upright=false;
                        downleft=true;
                        next_interval=true;
                        iy-=2;
                        ix+=2;
                    }
                    else {
                        upright=false;
                        upleft=true;
                        next_interval=true;
                        ix+=2;
                    }
                }
            }
            else {
                upleft=true;
                upright=false;
                next_interval=true;
                ix+=2;
            }
        }
        else {
            if(fy<=6 && fy>=2) {
                if(bricks[fy-1][fx]=='x') {
                    if(bricks[fy][fx+1]!=' ') {
                        design=bricks[fy][fx+1]; increment_score(design,score); updatesetup(lifecount,level,score);
                        clear_right_block(bricks, fx, fy);
                        generate_lifepackage(design, qbonus, bonus1, bonus1x, bonus1y, bonus2, bonus2x, bonus2y, fx+1, fy);
                        upright=false;
                        downleft=true;
                        next_interval=true;
                        iy-=2;
                        ix+=2;
                    }
                    else {
                        upright=false;
                        downright=true;
                        next_interval=true;
                        iy-=2;
                    }
                }
                else if(bricks[fy-1][fx]!='x') {
                    if(bricks[fy-1][fx]!=' ') {
                        if(bricks[fy][fx+1]!=' ') {
                            design=bricks[fy-1][fx]; increment_score(design,score); updatesetup(lifecount,level,score);
                            clear_top_block(bricks, fx, fy);
                            generate_lifepackage(design, qbonus, bonus1, bonus1x, bonus1y, bonus2, bonus2x, bonus2y, fx, fy-1);
                            design=bricks[fy][fx+1]; increment_score(design,score); updatesetup(lifecount,level,score);
                            clear_right_block(bricks, fx, fy);
                            generate_lifepackage(design, qbonus, bonus1, bonus1x, bonus1y, bonus2, bonus2x, bonus2y, fx+1, fy);
                            upright=false;
                            downleft=true;
                            next_interval=true;
                            iy-=2;
                            ix+=2;
                        }
                        else if(bricks[fy][fx+1]==' ') {
                            design=bricks[fy-1][fx]; increment_score(design,score); updatesetup(lifecount,level,score);
                            clear_top_block(bricks,fx,fy);
                            generate_lifepackage(design, qbonus, bonus1, bonus1x, bonus1y, bonus2, bonus2x, bonus2y, fx, fy-1);
                            upright=false;
                            downright=true;
                            next_interval=true;
                            iy-=2;
                        }
                    }
                    else {
                        if(bricks[fy][fx+1]!=' ') {
                            design=bricks[fy][fx+1]; increment_score(design,score); updatesetup(lifecount,level,score);
                            clear_right_block(bricks,fx,fy);
                            generate_lifepackage(design, qbonus, bonus1, bonus1x, bonus1y, bonus2, bonus2x, bonus2y, fx+1, fy);
                            upright=false;
                            upleft=true;
                            next_interval=true;
                            ix+=2;
                        }
                        else {
                            if(bricks[fy-1][fx+1]!=' ' && bricks[fy][fx+1]!='x') {
                                design=bricks[fy-1][fx+1]; increment_score(design,score); updatesetup(lifecount,level,score);
                                clear_topright_block(bricks,fx,fy);
                                generate_lifepackage(design, qbonus, bonus1, bonus1x, bonus1y, bonus2, bonus2x, bonus2y, fx+1, fy-1);
                                upright=false;
                                downleft=true;
                                next_interval=true;
                                iy-=2;
                                ix+=2;
                            }
                        }
                    }
                }
            }
        }
        if(!next_interval) {
            Sleep(100);
            fx++; fy--; ix++; iy--;
            gotoxy(ix,iy); cout<<' ';
            gotoxy(fx,fy); cout<<ball;
        }
    }
    else if(upleft) {
        if(fx==1) {
            if(fy<=6 && fy>=2) {
                if(bricks[fy-1][fx]=='x') {
                    upleft=false;
                    downright=true;
                    next_interval=true;
                    ix-=2;
                    iy-=2;
                }
                else {
                    if(bricks[fy-1][fx]!=' ') {
                        design=bricks[fy-1][fx]; increment_score(design,score); updatesetup(lifecount,level,score);
                        clear_top_block(bricks,fx,fy);
                        generate_lifepackage(design, qbonus, bonus1, bonus1x, bonus1y, bonus2, bonus2x, bonus2y, fx, fy-1);
                        upleft=false;
                        downright=true;
                        next_interval=true;
                        ix-=2;
                        iy-=2;
                    }
                    else {
                        upleft=false;
                        upright=true;
                        next_interval=true;
                        ix-=2;
                    }
                }
            }
            else {
                upleft=false;
                upright=true;
                next_interval=true;
                ix-=2;
            }
        }
        else {
            if(fy<=6 && fy>=2) {
                if(bricks[fy-1][fx]=='x') {
                    if(bricks[fy][fx-1]!=' ') {
                        design=bricks[fy][fx-1]; increment_score(design,score); updatesetup(lifecount,level,score);
                        clear_left_block(bricks, fx, fy);
                        generate_lifepackage(design, qbonus, bonus1, bonus1x, bonus1y, bonus2, bonus2x, bonus2y, fx-1, fy);
                        upleft=false;
                        downright=true;
                        next_interval=true;
                        ix-=2;
                        iy-=2;
                    }
                    else {
                        upleft=false;
                        downleft=true;
                        next_interval=true;
                        iy-=2;
                    }
                }
                else if(bricks[fy-1][fx]!='x') {
                    if(bricks[fy-1][fx]!=' ') {
                        if(bricks[fy][fx-1]!=' ') {
                            design=bricks[fy-1][fx]; increment_score(design,score); updatesetup(lifecount,level,score);
                            clear_top_block(bricks, fx, fy);
                            generate_lifepackage(design, qbonus, bonus1, bonus1x, bonus1y, bonus2, bonus2x, bonus2y, fx, fy-1);
                            design=bricks[fy][fx-1]; increment_score(design,score); updatesetup(lifecount,level,score);
                            clear_left_block(bricks, fx, fy);
                            generate_lifepackage(design, qbonus, bonus1, bonus1x, bonus1y, bonus2, bonus2x, bonus2y, fx-1, fy);
                            upleft=false;
                            downright=true;
                            next_interval=true;
                            ix-=2;
                            iy-=2;
                        }
                        else if(bricks[fy][fx-1]==' ') {
                            design=bricks[fy-1][fx]; increment_score(design,score); updatesetup(lifecount,level,score);
                            clear_top_block(bricks,fx,fy);
                            generate_lifepackage(design, qbonus, bonus1, bonus1x, bonus1y, bonus2, bonus2x, bonus2y, fx, fy-1);
                            upleft=false;
                            downleft=true;
                            next_interval=true;
                            iy-=2;
                        }
                    }
                    else {
                        if(bricks[fy][fx-1]!=' ') {
                            design=bricks[fy][fx-1]; increment_score(design,score); updatesetup(lifecount,level,score);
                            clear_left_block(bricks,fx,fy);
                            generate_lifepackage(design, qbonus, bonus1, bonus1x, bonus1y, bonus2, bonus2x, bonus2y, fx-1, fy);
                            upleft=false;
                            upright=true;
                            next_interval=true;
                            ix-=2;
                        }
                        else {
                            if(bricks[fy-1][fx-1]!=' ' && bricks[fy-1][fx-1]!='x') {
                                design=bricks[fy-1][fx-1]; increment_score(design,score); updatesetup(lifecount,level,score);
                                clear_topleft_block(bricks,fx,fy);
                                generate_lifepackage(design, qbonus, bonus1, bonus1x, bonus1y, bonus2, bonus2x, bonus2y, fx-1, fy-1);
                                upleft=false;
                                downright=true;
                                next_interval=true;
                                ix-=2;
                                iy-=2;
                            }
                        }
                    }
                }
            }
        }
        if(!next_interval) {
            Sleep(100);
            fx--; fy--; ix--; iy--;
            gotoxy(ix,iy); cout<<' ';
            gotoxy(fx,fy); cout<<ball;
        }
    }
    else if(downright) {
        if(fy==21) {
            if(fx>=left-1 && fx<=right+1) {
                if(fx<left+3 || fx>right-3) {
                    next_interval=true;
                    downright=false;
                    up=true;
                    iy+=2;
                    ix++;
                }
                else if(fx>=left+3 && fx<=right-3) {
                    next_interval=true;
                    downright=false;
                    upright=true;
                    iy+=2;
                }
            }
            else {
                Sleep(100);
                next_interval=true;
                ix++; iy++; fx++; fy++;
                gotoxy(ix,iy); cout<<' ';
                gotoxy(fx,fy); cout<<ball;
                downright=false;
                endgame=true;
            }
        }
        if(fy>=2 && fy<=5) {
            if (fx==63) {
                if(bricks[fy+1][fx]!=' ') {
                    design=bricks[fy+1][fx]; increment_score(design,score); updatesetup(lifecount,level,score);
                    clear_bottom_block(bricks,fx,fy);
                    generate_lifepackage(design, qbonus, bonus1, bonus1x, bonus1y, bonus2, bonus2x, bonus2y, fx, fy+1);
                    downright=false;
                    upleft=true;
                    next_interval=true;
                    ix+=2;
                    iy+=2;
                }
                else {
                    downright=false;
                    downleft=true;
                    next_interval=true;
                    ix+=2;
                }
            }
            else {
                if(bricks[fy+1][fx]!=' ') {
                    if(bricks[fy][fx+1]!=' ') {
                        design=bricks[fy][fx+1]; increment_score(design,score); updatesetup(lifecount,level,score);
                        clear_right_block(bricks,fx,fy);
                        generate_lifepackage(design, qbonus, bonus1, bonus1x, bonus1y, bonus2, bonus2x, bonus2y, fx+1, fy);
                        design=bricks[fy+1][fx]; increment_score(design,score); updatesetup(lifecount,level,score);
                        clear_bottom_block(bricks,fx,fy);
                        generate_lifepackage(design, qbonus, bonus1, bonus1x, bonus1y, bonus2, bonus2x, bonus2y, fx, fy+1);
                        downright=false;
                        upleft=true;
                        next_interval=true;
                        ix+=2;
                        iy+=2;
                    }
                    else {
                        design=bricks[fy+1][fx]; increment_score(design,score); updatesetup(lifecount,level,score);
                        clear_bottom_block(bricks,fx,fy);
                        generate_lifepackage(design, qbonus, bonus1, bonus1x, bonus1y, bonus2, bonus2x, bonus2y, fx, fy+1);
                        downright=false;
                        upright=true;
                        next_interval=true;
                        iy+=2;
                    }
                }
                else {
                    if(bricks[fy][fx+1]!=' ') {
                        design=bricks[fy][fx+1]; increment_score(design,score); updatesetup(lifecount,level,score);
                        clear_right_block(bricks,fx,fy);
                        generate_lifepackage(design, qbonus, bonus1, bonus1x, bonus1y, bonus2, bonus2x, bonus2y, fx+1, fy);
                        downright=false;
                        downleft=true;
                        next_interval=true;
                        ix+=2;
                    }
                    else {
                        if(bricks[fy+1][fx+1]!=' ') {
                            design=bricks[fy+1][fx+1]; increment_score(design,score); updatesetup(lifecount,level,score);
                            clear_bottomright_block(bricks,fx,fy);
                            generate_lifepackage(design, qbonus, bonus1, bonus1x, bonus1y, bonus2, bonus2x, bonus2y, fx+1, fy+1);
                            downright=false;
                            upleft=true;
                            next_interval=true;
                            ix+=2;
                            iy+=2;
                        }
                    }
                }
            }
        }
        else if(fy<21 && fy>5) {
            if(fx==63) {
                next_interval=true;
                downright=false;
                downleft=true;
                ix+=2;
            }
        }
        if(!next_interval) {
            Sleep(100);
            fx++; fy++; ix++; iy++;
            gotoxy(ix,iy); cout<<' ';
            gotoxy(fx,fy); cout<<ball;
        }
    }
    else if(downleft) {
        if(fy==21) {
            if(fx<=right+1 && fx>=left-1) {
                if(fx>right-3 || fx<left+3) {
                    next_interval=true;
                    downleft=false;
                    up=true;
                    iy+=2;
                    ix--;
                }
                else {
                    next_interval=true;
                    downleft=false;
                    upleft=true;
                    iy+=2;
                }
            }
            else {
                Sleep(100);
                next_interval=true;
                downleft=false;
                endgame=true;
                ix--; iy++; fx--; fy++;
                gotoxy(ix,iy); cout<<' ';
                gotoxy(fx,fy); cout<<ball;
            }
        }
        if(fy>=2 && fy<=5) {
            if (fx==1) {
                if(bricks[fy+1][fx]!=' ') {
                    design=bricks[fy+1][fx]; increment_score(design,score); updatesetup(lifecount,level,score);
                    clear_bottom_block(bricks,fx,fy);
                    generate_lifepackage(design, qbonus, bonus1, bonus1x, bonus1y, bonus2, bonus2x, bonus2y, fx, fy+1);
                    downleft=false;
                    upright=true;
                    next_interval=true;
                    ix-=2;
                    iy+=2;
                }
                else {
                    downleft=false;
                    downright=true;
                    next_interval=true;
                    ix-=2;
                }
            }
            else {
                if(bricks[fy+1][fx]!=' ') {
                    if(bricks[fy][fx-1]!=' ') {
                        design=bricks[fy][fx-1]; increment_score(design,score); updatesetup(lifecount,level,score);
                        clear_left_block(bricks,fx,fy);
                        generate_lifepackage(design, qbonus, bonus1, bonus1x, bonus1y, bonus2, bonus2x, bonus2y, fx-1, fy);
                        design=bricks[fy+1][fx]; increment_score(design,score); updatesetup(lifecount,level,score);
                        clear_bottom_block(bricks,fx,fy);
                        generate_lifepackage(design, qbonus, bonus1, bonus1x, bonus1y, bonus2, bonus2x, bonus2y, fx, fy+1);
                        downleft=false;
                        upright=true;
                        next_interval=true;
                        ix-=2;
                        iy+=2;
                    }
                    else {
                        design=bricks[fy+1][fx]; increment_score(design,score); updatesetup(lifecount,level,score);
                        clear_bottom_block(bricks,fx,fy);
                        generate_lifepackage(design, qbonus, bonus1, bonus1x, bonus1y, bonus2, bonus2x, bonus2y, fx, fy+1);
                        downleft=false;
                        upleft=true;
                        next_interval=true;
                        iy+=2;
                    }
                }
                else {
                    if(bricks[fy][fx-1]!=' ') {
                        design=bricks[fy][fx-1]; increment_score(design,score); updatesetup(lifecount,level,score);
                        clear_left_block(bricks,fx,fy);
                        generate_lifepackage(design, qbonus, bonus1, bonus1x, bonus1y, bonus2, bonus2x, bonus2y, fx-1, fy);
                        downleft=false;
                        downright=true;
                        next_interval=true;
                        ix-=2;
                    }
                    else {
                        if(bricks[fy+1][fx-1]!=' ') {
                            design=bricks[fy+1][fx-1]; increment_score(design,score); updatesetup(lifecount,level,score);
                            clear_bottomleft_block(bricks,fx,fy);
                            generate_lifepackage(design, qbonus, bonus1, bonus1x, bonus1y, bonus2, bonus2x, bonus2y, fx-1, fy+1);
                            downleft=false;
                            upright=true;
                            next_interval=true;
                            ix-=2;
                            iy+=2;
                        }
                    }
                }
            }
        }
        else if(fy<21 && fy>5) {
            if(fx==1) {
                next_interval=true;
                downleft=false;
                downright=true;
                ix-=2;
            }
        }
        if(!next_interval) {
            Sleep(100);
            fx--; fy++; ix--; iy++;
            gotoxy(ix,iy); cout<<' ';
            gotoxy(fx,fy); cout<<ball;
        }
    }
    SetColorAndBackground(15,0);
    drop_lifepackage(bonus1, bonus1x, bonus1y, left, right, lifecount, score, level, bricks, slowdown);
    drop_lifepackage(bonus2, bonus2x, bonus2y, left, right, lifecount, score, level, bricks, slowdown);
}

void generate_lifepackage(char design, int &qbonus, bool &bonus1, int &bonus1x, int &bonus1y, bool &bonus2, int &bonus2x, int &bonus2y, int x, int y) {
    if(design==bonus) {
        float sx=x/7.0;
        int dx=x/7;
        if(sx==dx) x-=3;
        else if(sx>dx) x=(dx*7)+4;
        if(bonus1x==0 && bonus1y==0) {
            bonus1x=x;
            bonus1y=y;
            bonus1=true;
            gotoxy(bonus1x, bonus1y);
            SetColorAndBackground(14,0);
            cout<<lifepackage;
            SetColorAndBackground(15,0);
        }
        else {
            bonus2x=x;
            bonus2y=y;
            bonus2=true;
            gotoxy(bonus2x, bonus2y);
            SetColorAndBackground(14,0);
            cout<<lifepackage;
            SetColorAndBackground(15,0);
        }
    }
}

void drop_lifepackage(bool &flag, int &x, int &y, int &left, int&right, int &lifecount, int &score, int &level, char bricks[][66], int &slowdown) {
    if(flag) {
        if(slowdown%4==0) {
            if(y==21) {
                if(x>=left and x<=right) {
                    lifecount++;
                    score+=10;
                    updatesetup(lifecount, level, score);
                    gotoxy(x, y);
                    cout<<' ';
                    flag=false;
                }
                else {
                    gotoxy(x, y);
                    cout<<' ';
                    y++;
                    gotoxy(x, y);
                    SetColorAndBackground(14,0);
                    cout<<lifepackage;
                    SetColorAndBackground(15,0);
                }
            }
            else if(y==22) {
                gotoxy(x, y);
                cout<<' ';
                flag=false;
            }
            else if(y<=5) {
                gotoxy(x,y);
                cout<<bricks[y][x];
                y++;
                gotoxy(x, y);
                SetColorAndBackground(14,0);
                cout<<lifepackage;
                SetColorAndBackground(15,0);
            }
            else {
                gotoxy(x,y);
                cout<<' ';
                y++;
                gotoxy(x, y);
                SetColorAndBackground(14,0);
                cout<<lifepackage;
                SetColorAndBackground(15,0);
            }
        }
        slowdown++;
    }
}

void increment_score(char design, int &score) {
    if(design==bonus) score+=5;
    else if(design==b1) score+=4;
    else if(design==b2) score+=3;
    else if(design==b3) score+=1;
    else if(design==b4) score+=2;
}//bonus=5, b1=4, b2=3, b3=1, b4=2, lifes=10

bool bringnewlevel(char bricks[][66], int totalbricks, bool bonus1, bool bonus2) {
    int counter=0;
    for(int i=2;i<=5;i++) {
        for(int j=1;j<64;j+=7) {
            if(bricks[i][j]==' ')
                counter++;
        }
    }
    if(counter==totalbricks) {
        if(!bonus1 && !bonus2)
            return true;
        else return false;
    }
    else return false;
}

void clear_line(int fromX, int toX, int Y) {
    for(int i=fromX;i<=toX;i++) {
        gotoxy(i,Y);
        cout<<' ';
    }
}

void clear_top_block(char bricks[][66], int fx, int fy) {
        float measure=fx/7.0; measure*=100.00;
        int measurejr=measure; measure/=100.00;
        int small=measure;
        small*=100;

        if(measurejr>small) {
            small=measure;
            int from=(small*7)+1, to=(small*7)+7, row=fy-1;
            for(int i=from; i<=to; i++) {
                bricks[row][i]=' ';
                gotoxy(i,row);
                cout<<" ";
            }
        }
        else if(measurejr==small) {
            small=measure;
            int from=(small*7)-6, to=(small*7), row=fy-1;
            for(int i=from; i<=to; i++) {
                bricks[row][i]=' ';
                gotoxy(i,row);
                cout<<" ";
            }
        }
}

void clear_right_block(char bricks[][66], int fx, int fy) {
    for(int i=fx+1;i<=fx+7;i++) {
        bricks[fy][i]=' ';
        gotoxy(i,fy);
        cout<<' ';
    }
}

void clear_left_block(char bricks[][66], int fx, int fy) {
    for(int i=fx-1;i>=fx-7;i--) {
        bricks[fy][i]=' ';
        gotoxy(i,fy);
        cout<<' ';
    }
}

void clear_topright_block(char bricks[][66], int fx, int fy) {
    for(int i=fx+1;i<=fx+7;i++) {
        bricks[fy-1][i]=' ';
        gotoxy(i,fy-1);
        cout<<' ';
    }
}

void clear_topleft_block(char bricks[][66], int fx, int fy) {
    for(int i=fx-1;i>=fx-7;i--) {
        bricks[fy-1][i]=' ';
        gotoxy(i,fy-1);
        cout<<' ';
    }
}

void clear_bottomright_block(char bricks[][66], int fx, int fy) {
    for(int i=fx+1;i<=fx+7;i++) {
        bricks[fy+1][i]=' ';
        gotoxy(i,fy+1);
        cout<<' ';
    }
}

void clear_bottomleft_block(char bricks[][66], int fx, int fy) {
    for(int i=fx-1;i>=fx-7;i--) {
        bricks[fy+1][i]=' ';
        gotoxy(i,fy+1);
        cout<<' ';
    }
}

void clear_bottom_block(char bricks[][66], int fx, int fy) {
        float measure=fx/7.0; measure*=100.00;
        int measurejr=measure; measure/=100.00;
        int small=measure;
        small*=100;

        if(measurejr>small) {
            small=measure;
            int from=(small*7)+1, to=(small*7)+7, row=fy+1;
            for(int i=from; i<=to; i++) {
                bricks[row][i]=' ';
                gotoxy(i,row);
                cout<<" ";
            }
        }
        else if(measurejr==small) {
            small=measure;
            int from=(small*7)-6, to=(small*7), row=fy+1;
            for(int i=from; i<=to; i++) {
                bricks[row][i]=' ';
                gotoxy(i,row);
                cout<<" ";
            }
        }
}
