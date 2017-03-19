//@StarLeague instructors - I am using ncurses because conio.h and mac don't go together
//so please run this in a terminal window or something and link ncurses.h when compiling
//g++ pacman.cpp =lncurses (my libncurses.a/dylib is located in the same folder as pacman)
//more faithful to actual pacman than assignment (grade differently)

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <ncurses.h>
//reads map from "pacman.txt"
//assumes height is no larger than 50
//assumes monsters are less than 20

//NEEDS WORK: Player oriented code / monsters actually go to player
using namespace std;
int main(){
	//backend init
	ifstream fin("pacman.txt");
	int height,ex,ey,px,py,mons=0,gems=0,lives=1; fin>>height; string map[50],monsMap[50]; //not initialized because unnecessary and lazy //monsMap - where user changes are recorded but not monster changes
	bool running=true;
	for(int i=0;i<height;i++){
		fin>>map[i];
		monsMap[i]=map[i];
		for(int j=0;j<map[i].length();j++){
			if(map[i][j]=='E'){
				ex=j;ey=i;
			}
			if(map[i][j]=='P'){
				px=j;py=i;
			}
			if(map[i][j]=='o'){
				gems++;
			}
			if(map[i][j]=='M'){
				mons++;
				monsMap[i][j]='.';
			}
		}
	}

	//screen init
	int row,col,startX,startY,width=map[0].length(),ch;
	initscr();
	curs_set(0);
	start_color();
	raw();
	keypad(stdscr,TRUE); //arrows
	getmaxyx(stdscr,row,col);
	startY=row/2-height/2;
	startX=col/2-width/2;

	//print gimmicks
	bool isHash=false;
	bool isP=false;
	bool isE=false;
	bool isUnlock=false;
	bool isG=false;
	bool isM=false;
	init_pair(1, COLOR_YELLOW, COLOR_BLACK); //Pacman
	init_pair(2, COLOR_BLUE, COLOR_BLACK); //Wall
	init_pair(3, COLOR_BLACK, COLOR_GREEN); //Exit
	init_pair(4, COLOR_WHITE, COLOR_BLACK); //Gem (changed graphically from G to 'o')
	init_pair(5, COLOR_GREEN, COLOR_BLACK); //Unlocked exit
	init_pair(6, COLOR_RED, COLOR_BLACK); //Monster
	init_pair(7, COLOR_RED, COLOR_RED); //Dead
	init_pair(8, COLOR_BLACK, COLOR_RED); //Dead text
	string dead="oops, you died";
	bool deadprint=false;

	//game loop
	while(running){
		for(int i=0;i<height;i++){
			move(startY+i,startX);
			for(int j=0;j<width;j++){
				if(map[i][j]=='P'){attron(A_BOLD);attron(COLOR_PAIR(1));isP=true;}
				if(map[i][j]=='#'){attron(COLOR_PAIR(2));isHash=true;}
				if(map[i][j]=='E'){
					if(gems==0){
						attron(A_BOLD);attron(COLOR_PAIR(5));attron(A_BLINK);isUnlock=true;
					}else{
						attron(A_BOLD);attron(COLOR_PAIR(3));isE=true;
					}
				}
				if(map[i][j]=='o'){attron(A_BOLD);attron(COLOR_PAIR(4));isG=true;}
				if(map[i][j]=='M'){attron(A_BOLD);attron(COLOR_PAIR(6));isM=true;}
				addch(map[i][j]); //@eclipse fuck off I'm only using one window
				if(isP){attroff(A_BOLD);attroff(COLOR_PAIR(1));isP=false;}
				if(isHash){attroff(COLOR_PAIR(2));isHash=false;}
				if(isUnlock){attroff(A_BOLD);attroff(COLOR_PAIR(5));attroff(A_BLINK);isUnlock=false;}
				if(isE){attroff(A_BOLD);attroff(COLOR_PAIR(3));isE=false;}
				if(isG){attroff(A_BOLD);attroff(COLOR_PAIR(4));isG=false;}
				if(isM){attroff(A_BOLD);attroff(COLOR_PAIR(6));isM=false;}
			}
			refresh();
		}
		//player movement validation and map updates
		ch=getch();
		switch(ch){
			case KEY_UP:
				if(py==0){
					switch(map[height-1][px]){
						case ' ':
							if(px==ex&&py==ey){
								map[py][px]='E';
								monsMap[py][px]='E';
								py=height-1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}else{
								map[py][px]=' ';
								monsMap[py][px]=' ';
								py=height-1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}
							break;
						case '.':
							if(px==ex&&py==ey){
								map[py][px]='E';
								monsMap[py][px]='E';
								py=height-1;
								monsMap[py][px]='P';
								map[py][px]='P';
							}else{
								map[py][px]=' ';
								monsMap[py][px]=' ';
								py=height-1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}
						case 'o':
							if(px==ex&&py==ey){
								map[py][px]='E';
								monsMap[py][px]='E';
								py=height-1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}else{
								map[py][px]=' ';
								monsMap[py][px]=' ';
								py=height-1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}
							gems-=1;
							break;
						case 'E':
							map[py][px]=' ';
							monsMap[py][px]=' ';
							py=height-1;
							map[py][px]='P';
							monsMap[py][px]='P';
							break;
						case 'M':
							lives-=1;
							map[py][px]=' ';
							monsMap[py][px]=' ';
							py=height-1;
							map[py][px]='P';
							monsMap[py][px]='P';
						default:
							break;
					}
				}else{
					switch(map[py-1][px]){
						case ' ':
							if(px==ex&&py==ey){
								map[py][px]='E';
								monsMap[py][px]='E';
								py-=1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}else{
								map[py][px]=' ';
								monsMap[py][px]=' ';
								py-=1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}
							break;
						case '.':
							if(px==ex&&py==ey){
								map[py][px]='E';
								monsMap[py][px]='E';
								py-=1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}else{
								map[py][px]=' ';
								monsMap[py][px]=' ';
								py-=1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}
							break;
						case 'o':
							if(px==ex&&py==ey){
								map[py][px]='E';
								monsMap[py][px]='E';
								py-=1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}else{
								map[py][px]=' ';
								monsMap[py][px]=' ';
								py-=1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}
							gems-=1;
							break;
						case 'E':
							map[py][px]=' ';
							monsMap[py][px]=' ';
							py-=1;
							map[py][px]='P';
							monsMap[py][px]='P';
							break;
						case 'M':
							lives-=1;
							map[py][px]=' ';
							monsMap[py][px]=' ';
							py-=1;
							map[py][px]='P';
							monsMap[py][px]='P';
						default:
							break;
					}
				}
				break;
			case KEY_DOWN:
				if(py==height-1){
					switch(map[0][px]){
						case ' ':
							if(px==ex&&py==ey){
								map[py][px]='E';
								monsMap[py][px]='E';
								py=0;
								map[py][px]='P';
								monsMap[py][px]='P';
							}else{
								map[py][px]=' ';
								monsMap[py][px]=' ';
								py=0;
								map[py][px]='P';
								monsMap[py][px]='P';
							}
							break;
						case '.':
							if(px==ex&&py==ey){
								map[py][px]='E';
								monsMap[py][px]='E';
								py=0;
								map[py][px]='P';
								monsMap[py][px]='P';
							}else{
								map[py][px]=' ';
								monsMap[py][px]=' ';
								py=0;
								map[py][px]='P';
								monsMap[py][px]='P';
							}
							break;
						case 'o':
							if(px==ex&&py==ey){
								map[py][px]='E';
								monsMap[py][px]='E';
								py=0;
								map[py][px]='P';
								monsMap[py][px]='P';
							}else{
								map[py][px]=' ';
								monsMap[py][px]=' ';
								py=0;
								map[py][px]='P';
								monsMap[py][px]='P';
							}
							gems-=1;
							break;
						case 'E':
							map[py][px]=' ';
							monsMap[py][px]=' ';
							py=0;
							map[py][px]='P';
							monsMap[py][px]='P';
							break;
						case 'M':
							lives-=1;
							map[py][px]=' ';
							monsMap[py][px]=' ';
							py=0;
							map[py][px]='P';
							monsMap[py][px]='P';
						default:
							break;
					}
				}else{
					switch(map[py+1][px]){
						case ' ':
							if(px==ex&&py==ey){
								map[py][px]='E';
								monsMap[py][px]='E';
								py+=1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}else{
								map[py][px]=' ';
								monsMap[py][px]=' ';
								py+=1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}
							break;
						case '.':
							if(px==ex&&py==ey){
								map[py][px]='E';
								monsMap[py][px]='E';
								py+=1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}else{
								map[py][px]=' ';
								monsMap[py][px]=' ';
								py+=1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}
							break;
						case 'o':
							if(px==ex&&py==ey){
								map[py][px]='E';
								monsMap[py][px]='E';
								py+=1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}else{
								map[py][px]=' ';
								monsMap[py][px]=' ';
								py+=1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}
							gems-=1;
							break;
						case 'E':
							map[py][px]=' ';
							monsMap[py][px]=' ';
							py+=1;
							map[py][px]='P';
							monsMap[py][px]='P';
							break;
						case 'M':
							lives-=1;
							map[py][px]=' ';
							monsMap[py][px]=' ';
							py+=1;
							map[py][px]='P';
							monsMap[py][px]='P';
						default:
							break;
					}
				}
				break;
			case KEY_LEFT:
				if(px==0){
					switch(map[py][width-1]){
						case ' ':
							if(px==ex&&py==ey){
								map[py][px]='E';
								monsMap[py][px]='E';
								px=width-1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}else{
								map[py][px]=' ';
								monsMap[py][px]=' ';
								px=width-1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}
							break;
						case '.':
							if(px==ex&&py==ey){
								map[py][px]='E';
								monsMap[py][px]='E';
								px=width-1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}else{
								map[py][px]=' ';
								monsMap[py][px]=' ';
								px=width-1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}
							break;
						case 'o':
							if(px==ex&&py==ey){
								map[py][px]='E';
								monsMap[py][px]='E';
								px=width-1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}else{
								map[py][px]=' ';
								monsMap[py][px]=' ';
								px=width-1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}
							gems-=1;
							break;
						case 'E':
							map[py][px]=' ';
							monsMap[py][px]=' ';
							px=width-1;
							map[py][px]='P';
							monsMap[py][px]='P';
							break;
						case 'M':
							lives-=1;
							map[py][px]=' ';
							monsMap[py][px]=' ';
							px=width-1;
							map[py][px]='P';
							monsMap[py][px]='P';
						default:
							break;
					}
				}else{
					switch(map[py][px-1]){
						case ' ':
							if(px==ex&&py==ey){
								map[py][px]='E';
								monsMap[py][px]='E';
								px-=1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}else{
								map[py][px]=' ';
								monsMap[py][px]=' ';
								px-=1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}
							break;
						case '.':
							if(px==ex&&py==ey){
								map[py][px]='E';
								monsMap[py][px]='E';
								px-=1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}else{
								map[py][px]=' ';
								monsMap[py][px]=' ';
								px-=1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}
							break;
						case 'o':
							if(px==ex&&py==ey){
								map[py][px]='E';
								monsMap[py][px]='E';
								px-=1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}else{
								map[py][px]=' ';
								monsMap[py][px]=' ';
								px-=1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}
							gems-=1;
							break;
						case 'E':
							map[py][px]=' ';
							monsMap[py][px]=' ';
							px-=1;
							map[py][px]='P';
							monsMap[py][px]='P';
							break;
						case 'M':
							lives-=1;
							map[py][px]=' ';
							monsMap[py][px]=' ';
							px-=1;
							map[py][px]='P';
							monsMap[py][px]='P';
						default:
							break;
					}
				}
				break;
			case KEY_RIGHT:
				if(px==width-1){
					switch(map[py][0]){
						case ' ':
							if(px==ex&&py==ey){
								map[py][px]='E';
								monsMap[py][px]='E';
								px=0;
								map[py][px]='P';
								monsMap[py][px]='P';
							}else{
								map[py][px]=' ';
								monsMap[py][px]=' ';
								px=0;
								map[py][px]='P';
								monsMap[py][px]='P';
							}
							break;
						case '.':
							if(px==ex&&py==ey){
								map[py][px]='E';
								monsMap[py][px]='E';
								px=0;
								map[py][px]='P';
								monsMap[py][px]='P';
							}else{
								map[py][px]=' ';
								monsMap[py][px]=' ';
								px=0;
								map[py][px]='P';
								monsMap[py][px]='P';
							}
							break;
						case 'o':
							if(px==ex&&py==ey){
								map[py][px]='E';
								monsMap[py][px]='E';
								px=0;
								map[py][px]='P';
								monsMap[py][px]='P';
							}else{
								map[py][px]=' ';
								monsMap[py][px]=' ';
								px=0;
								map[py][px]='P';
								monsMap[py][px]='P';
							}
							gems-=1;
							break;
						case 'E':
							map[py][px]=' ';
							monsMap[py][px]=' ';
							px=0;
							map[py][px]='P';
							monsMap[py][px]='P';
							break;
						case 'M':
							lives-=1;
							map[py][px]=' ';
							monsMap[py][px]=' ';
							px=0;
							map[py][px]='P';
							monsMap[py][px]='P';
						default:
							break;
					}
				}else{
					switch(map[py][px+1]){
						case ' ':
							if(px==ex&&py==ey){
								map[py][px]='E';
								monsMap[py][px]='E';
								px+=1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}else{
								map[py][px]=' ';
								monsMap[py][px]=' ';
								px+=1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}
							break;
						case '.':
							if(px==ex&&py==ey){
								map[py][px]='E';
								monsMap[py][px]='E';
								px+=1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}else{
								map[py][px]=' ';
								monsMap[py][px]=' ';
								px+=1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}
							break;
						case 'o':
							if(px==ex&&py==ey){
								map[py][px]='E';
								monsMap[py][px]='E';
								px+=1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}else{
								map[py][px]=' ';
								monsMap[py][px]=' ';
								px+=1;
								map[py][px]='P';
								monsMap[py][px]='P';
							}
							gems-=1;
							break;
						case 'E':
							map[py][px]=' ';
							monsMap[py][px]=' ';
							px+=1;
							map[py][px]='P';
							monsMap[py][px]='P';
							break;
						case 'M':
							lives-=1;
							map[py][px]=' ';
							monsMap[py][px]=' ';
							px+=1;
							map[py][px]='P';
							monsMap[py][px]='P';
						default:
							break;
					}
				}
				break;
			case 81: //Q
				running=false;
				break;
			case 113: //q
				running=false;
				break;
			default:
				refresh();
				break;
		}

		int dir;
		bool monsInvalid;
		//monster movements
		srand(time(NULL));
		for(int i=0;i<mons;i++){
			monsInvalid=true;
			for(int j=0;j<height;j++){
				if(monsInvalid==false)break;
				for(int k=0;k<width;k++){
					if(monsInvalid==false)break;
					if(map[j][k]=='M'){
						while(monsInvalid){
							dir=rand()%5;
							switch(dir){
								case 1:
									if(k==0){
										if(map[j][width-1]!='#'&&map[j][width-1]!='M'&&map[j][width-1]!='N'){
											if(map[j][width-1]=='P')lives-=1;
											map[j][k]=monsMap[j][k];
											map[j][width-1]='N';
											monsInvalid=false;
										}
									}else{
										if(map[j][k-1]!='#'&&map[j][k-1]!='M'&&map[j][k-1]!='N'){
											if(map[j][k-1]=='P')lives-=1;
											map[j][k]=monsMap[j][k];
											map[j][k-1]='N';
											monsInvalid=false;
										}
									}
									break;
								case 2:
									if(j==0){
										if(map[height-1][k]!='#'&&map[height-1][k]!='M'&&map[height-1][k]!='N'){
											if(map[height-1][k]=='P')lives-=1;
											map[j][k]=monsMap[j][k];
											map[height-1][k]='N';
											monsInvalid=false;
										}
									}else{
										if(map[j-1][k]!='#'&&map[j-1][k]!='M'&&map[j-1][k]!='N'){
											if(map[j-1][k]=='P')lives-=1;
											map[j][k]=monsMap[j][k];
											map[j-1][k]='N';
											monsInvalid=false;
										}
									}
									break;
								case 3:
									if(k==width-1){
										if(map[j][0]!='#'&&map[j][0]!='M'&&map[j][0]!='N'){
											if(map[j][0]=='P')lives-=1;
											map[j][k]=monsMap[j][k];
											map[j][0]='N';
											monsInvalid=false;
										}
									}else{
										if(map[j][k+1]!='#'&&map[j][k+1]!='M'&&map[j][k+1]!='N'){
											if(map[j][k+1]=='P')lives-=1;
											map[j][k]=monsMap[j][k];
											map[j][k+1]='N';
											monsInvalid=false;
										}
									}
									break;
								case 4:
									if(j==height-1){
										if(map[0][k]!='#'&&map[0][k]!='M'&&map[0][k]!='N'){
											if(map[0][k]=='P')lives-=1;
											map[j][k]=monsMap[j][k];
											map[0][k]='N';
											monsInvalid=false;
										}
									}else{
										if(map[j+1][k]!='#'&&map[j+1][k]!='M'&&map[j+1][k]!='N'){
											if(map[j+1][k]=='P')lives-=1;
											map[j][k]=monsMap[j][k];
											map[j+1][k]='N';
											monsInvalid=false;
										}
									}
									break;
								default:
									break;
							}
						}
					}
				}
			}
		}
		//quick map update N>>M
		for(int i=0;i<height;i++){
			for(int j=0;j<width;j++){
				if(map[i][j]=='N'){
					map[i][j]='M';
				}
			}
		}

		//check if game finished
		if(lives==0){
			for(int i=0;i<14;i++){
				map[height/2][((width/2)-7)+i]=dead[i];
			}
			for(int i=0;i<height;i++){
				move(startY+i,startX);
				for(int j=0;j<width;j++){
					if(i==height/2&&(j>=width/2-7&&j<=width/2+7)){
						attron(A_BOLD);
						attron(COLOR_PAIR(8));
						deadprint=true;
					}else{
						attron(COLOR_PAIR(7));
					}
					addch(map[i][j]);
					if(deadprint){
						attroff(A_BOLD);
						attroff(COLOR_PAIR(8));
						deadprint=false;
					}else{
						attroff(COLOR_PAIR(7));
					}
				}
				refresh();
			}
			getch();
			running=false;
		}
		if((px==ex&&py==ey)&&gems==0){
			running=false;
		}
	}

	endwin();
}
