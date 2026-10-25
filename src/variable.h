#ifndef VARIABLE_H_INCLUDED
#define VARIABLE_H_INCLUDED

int mouse_x, mouse_y, cursor, image1, image2, go, flc, image3, shoot[4], shootIndex = 0, bg1, bg2, cloud, bg1_y = 0, bg2_y = 500, image7, boss1[4], start, menu, play, instruction, insw, highestscore, credit, cw, hsw, secbg1_y = 0, secbg2_y = 500, secbg1, secbg2, backbutton;//picture names
double width = 720;
double height = 500;
double dragHeight = 90, mcarx = 160, mcary = 5, mCarWid = 80, dragLife = 50, tempLife;//variable related to dragon
int dan[4], danIndex;
int shoot1x = 200, shoot2x = 200, shoot3x = 200, shoot4x = 200, shoot5x = 200;
int shoot1y = 80, shoot2y = 80, shoot3y = 80, shoot4y = 80, shoot5y = 80, shoot_y = 80, shoot__y = 400;
int boss1x = 305, boss1y = 700, boss1height = 110, boss1wid = 100, boss1Life = 100, boss1shoot[4], boss1Index = 0, boss1shootIndex = 0, boss1shoot1x = 305, boss1shoot1y = 700,bossDieIndex=0,bossDead[10];
int show = 0, fifeteen, resume;
int drag[4], dragIndex = 0,dragDie[6],dragDieIndex=0;
int zom1[4], zom1Index = 0, zom2[4], zom2Index = 0, zom3[4], zom3Index = 0,zomb,zombiePosition=500,zomMoveFactor=3,zomWid=50,zomHeight=60;
int cloud_y = 600, cloud1x = 80, cloud1y = 600, cloud2x = 330, cloud2y = 600, cloud3x = 580, cloud3y = 600, cloudWidth = 40, cloudHeight = 50;
int eggPic[4];
char score[100];
char gameScore[100];
char bossLife[100];
char drLife[100];
char highScore[100];
int returning=0;
int point = 0;//egg counnt
int gamePoint = 0;//actual points
int countt = 0;//decides the death of boss
/*booleans for eggShow*/
bool eggShow1 = true;
bool eggShow2 = true;
bool eggShow3 = true;
bool eggShow4 = true;
bool boss1Show = false;
bool  shoot1Flag = false, shoot2Flag = false, shoot3Flag = false, shoot4Flag = false, shoot5Flag = false;
bool boss1shoot1 = false, gamePoints = true;
//bool music_on = true;
int dx = 3;
int level = 1;//tells us which level
int lifebar[11];
int highestScore=50;
int Fscore;
FILE *filePointer;
void newInitialise(){//newly initialises the values

	dragLife = 50;
	boss1x = 305; boss1y = 700; boss1Life = 100; boss1shoot1x = 305; boss1shoot1y = 700;

	point = 0; gamePoint = 0;
	countt = 0;
	returning=0;
	eggShow1 = true;
	eggShow2 = true;
	eggShow3 = true;
	eggShow4 = true;
	boss1Show = false;
	shoot1Flag = false, shoot2Flag = false, shoot3Flag = false, shoot4Flag = false, shoot5Flag = false;
	boss1shoot1 = false, gamePoints = true;
	zomMoveFactor = 3;
	dragDieIndex = 0, bossDieIndex = 0;
}


#endif // VARIABLE_H_INCLUDED
