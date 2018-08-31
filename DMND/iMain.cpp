
# include "iGraphics.h"
#include <string.h>
# include "variable.h"
# include "dragonEgg.h"
# include "clouds.h"
# include "boss.h"
# include "zombies.h"
#include <iostream>
#include "score.h"
# include <string>
#include <cstring>
#include <fstream>
//using namespace std;



void pauseGame(){
	/*pausing the game on different gameState*/
		iPauseTimer(0);
		iPauseTimer(1);
		iPauseTimer(2);
		iPauseTimer(3);
		iPauseTimer(4);
		iPauseTimer(5);


}
void resumeGame(){
	/*Resuming the game on different gamestate*/
		iResumeTimer(0);
		iResumeTimer(1);
		iResumeTimer(2);
		iResumeTimer(3);
		iResumeTimer(4);
		iResumeTimer(5);
}


void objectChange() {

	dragShoot();//Dragon's firing 

	if (show==2 || show ==3)
	poisonCloud();//for the movement and collsions for poison clouds
	bossVerticalMove();
	if (show == 3){
		zombie[0].place();
		zombie[1].place();
		zombie[2].place();
		zombie[3].place();
		zombie[4].place();
	}
	if (!boss1Show && show==2){
		eggMove();

		if (eggShow1)

			egg[0].eggCheck1();
		if (eggShow2)

			eggCheck2();
		if (eggShow3)

			eggCheck3();
		if (eggShow4)

			eggCheck4();
	}
	if (point == 15){
		if (bossLife>0)
		boss1Show = true;
		eggShow1 = false;	eggShow2 = false;	eggShow3 = false;	eggShow4 = false;
	}
	if (dragLife <= 0 && show != 1 && show != 4 && show != 5 && show != 6 && dragDieIndex >= 5)
	{
		show = 8;//gameOver
		
	}
	if (boss1Life <= 0){
		countt = 1;
	}
	if (boss1y <= 300 && show != 1 && show != 4 && show != 5 && show != 6 && show!=3&&dragLife>0)
	{
		show = 9;//1stLevelCompleted
	}
}

void iDraw()
{
	iClear();
	iSetColor(0, 0, 0);
	/*converting integer to String*/
	_itoa_s(point, score, 10);
	_itoa_s(gamePoint, gameScore, 10);
	_itoa_s(boss1Life, bossLife, 10);
	_itoa_s(dragLife, drLife, 10);

	insertHighscore();// Inserting the highscore
	
	if (show == 0){//openning page
		iShowImage(0, 0, width, height, start);
		iSetColor(0, 0, 0);
		iRectangle(320, 70, 80, 80);
		iShowImage(mouse_x-25, mouse_y-25, 50, 50, cursor);	
	}
	if (show == 4){ //instruction
		iShowImage(0, 0, width+10, height, insw);
		iShowImage(10, 450, 40, 40, backbutton);
		iShowImage(mouse_x-25, mouse_y-25, 50, 50, cursor);
	}
	if (show == 5){//highscore page 
		
		iSetColor(255, 255, 255);
		
		
		ifstream  fpp;//creating filestream to read from the file


		fpp.open("SCR.txt", ios::in);//taking the highscore from the file
		fpp >> sscore;

		void close();//closing the file 

		printf("SScore == %d\n", sscore);

		_itoa_s(sscore, highScore, 10);
		iShowImage(0, 0, width, height, hsw);
		iShowImage(10, 450, 40, 40, backbutton);
		iText(250, 150, "Highest Score is :  ", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(425, 150, (highScore), GLUT_BITMAP_TIMES_ROMAN_24);
		iShowImage(mouse_x-25, mouse_y-25, 50, 50, cursor);
	}
	if (show == 6){//credits page 
		iShowImage(0, 0, width, height, cw);
		iShowImage(10, 450, 40, 40, backbutton);
		iShowImage(mouse_x-25, mouse_y-25, 50, 50, cursor);
	}
	
	if (show == 8)//hero defeated
	{
		gamePoints = false;//stops counting points
		iShowImage(0, 0, width, height, go);
		iSetColor(255, 255, 255);
		iText(250, 150, "Your Score is :  ", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(400, 150, (gameScore), GLUT_BITMAP_TIMES_ROMAN_24);
		//iText(250, 50, "Press 'q' to return to menu.", GLUT_BITMAP_TIMES_ROMAN_24);

	}
	if (show == 9)//completing level
	{
		gamePoints = false;//stops counting points
		iShowImage(0, -20, width, height+20, flc);
		iSetColor(255, 255, 255);
		iText(250, 150, "Your Score is :  ", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(400, 150, (gameScore), GLUT_BITMAP_TIMES_ROMAN_24);
		//iText(250, 50, "Press 'q' to return to menu.", GLUT_BITMAP_TIMES_ROMAN_24);
		iShowImage(10, 450, 40, 40, backbutton);
		iShowImage(mouse_x-25, mouse_y-25, 50, 50, cursor);
		
	}
	if (show == 1){//menu
		
		iShowImage(0, 0, width, height, menu);
		iShowImage(550, 400, 80, 60, play);
		iShowImage(500, 305, 180, 70, instruction);
		iShowImage(490, 240, 200, 60, highestscore);
		iShowImage(540, 160, 100, 60, credit);
		iSetColor(255,255,255);
		if(returning == 1 && dragLife>0)
		iShowImage(530, 90, 120, 60, resume);
		iShowImage(mouse_x-25, mouse_y-25, 45, 45, cursor);
	}
	if (show == 7)//staring instructions 
	{
		iShowImage(0, 0, width, height, fifeteen);
	}

	if (show == 2 ){//level 1 background
		
		iShowImage(0, bg1_y, width, height, bg1);
		iShowImage(0, bg2_y, width, height, bg2);
		
		/*controlling the image of dragon fire*/
		 if (shoot5Flag) {
			 iShowImage(shoot5x, shoot5y, 35, 25, shoot[shootIndex]);
		}if (shoot4Flag) {
			iShowImage(shoot4x, shoot4y, 35, 25, shoot[shootIndex]);
		}if (shoot3Flag) {
			iShowImage(shoot3x, shoot3y, 35, 25, shoot[shootIndex]);
		}if (shoot2Flag) {
			iShowImage(shoot2x, shoot2y, 35, 25, shoot[shootIndex]);
		}if (shoot1Flag) {
			iShowImage(shoot1x, shoot1y, 35, 25, shoot[shootIndex]);
		}

		iSetColor(0, 0, 0);
		
		if (dragLife>0)
			iShowImage(mcarx, mcary, mCarWid + 30, 100, drag[dragIndex]);//Showing the Dragon
		if (eggShow1){
			iShowImage(egg[0].x = 150, egg[0].y, 30, 30, eggPic[0]);//img1 for egg1
		}
		if (eggShow2){
			iShowImage(egg[1].x = 300, egg[1].y, 30, 30, eggPic[0]);//img4 for egg2
		}
		if (eggShow3){
			iShowImage(egg[2].x = 450, egg[2].y, 30, 30, eggPic[0]);//img5 for egg3
		}
		if (eggShow4){
			iShowImage(egg[3].x = 600, egg[3].y, 30, 30, eggPic[0]);//img6 for egg4	
		}
		if (dragLife <= 0){
			iShowImage(mcarx, mcary, mCarWid + 30, dragHeight, dragDie[dragDieIndex]);//rending of drag dying
		}
		/*Showing the clouds*/
		iShowImage(cloud1x, cloud1y, cloudWidth, cloudHeight, cloud);
		iShowImage(cloud2x, cloud2y, cloudWidth, cloudHeight, cloud);
		iShowImage(cloud3x, cloud3y, cloudWidth, cloudHeight, cloud);
		if (boss1Show && boss1Life > 0){//Blue firing from boss at the end of the first level
			if (boss1shoot1){
				iShowImage(boss1shoot1x +40, boss1shoot1y+10, 30, 35, boss1shoot[boss1shootIndex]);
			}
			iShowImage(boss1x, boss1y, boss1wid, boss1height, boss1[boss1Index]);
		}
		dragLifebar();//showing the dragon lifebar
		if (countt == 1){
			boss1Show = false;
			boss1shoot1 = false;
			iShowImage(boss1x, boss1y, boss1wid - 30, boss1height - 30, bossDead[bossDieIndex]);
		}
		if (!boss1Show && bossLife>0){//scoreboard
			iSetColor(255,255,255);
			iText(10, 480, "Collected Eggs:  ", GLUT_BITMAP_TIMES_ROMAN_24);
			iText(180, 480, (score), GLUT_BITMAP_TIMES_ROMAN_24);
			iText(10, 380, "Score:  ", GLUT_BITMAP_TIMES_ROMAN_24);
			iText(100, 380, (gameScore), GLUT_BITMAP_TIMES_ROMAN_24);
			iText(10, 400, "Dragon Life:  ", GLUT_BITMAP_TIMES_ROMAN_24);
			iText(150, 400, (drLife), GLUT_BITMAP_TIMES_ROMAN_24);
		}
		
		if (boss1Show){//scoreboard during game 
			iSetColor(255,255,255);
			iText(10, 480, "Score:  ", GLUT_BITMAP_TIMES_ROMAN_24);
			iText(100, 480, (gameScore), GLUT_BITMAP_TIMES_ROMAN_24);
			iText(10, 380, "Boss Life:  ", GLUT_BITMAP_TIMES_ROMAN_24);
			iText(190, 380, (bossLife), GLUT_BITMAP_TIMES_ROMAN_24);
			iText(10, 400, "Dragon Life:  ", GLUT_BITMAP_TIMES_ROMAN_24);
			iText(190, 400, (drLife), GLUT_BITMAP_TIMES_ROMAN_24);
			if (boss1y > 550){
				iSetColor(rand() % 255, rand()%255, 0);
				iText(240, 235, "NIGHT KING IS COMING!", GLUT_BITMAP_TIMES_ROMAN_24);

			}
		}
		
	}

	if (show == 3){// 2nd levels image rendaring

		iShowImage(0, secbg1_y, width, height, secbg1);
		iShowImage(0, secbg2_y, width, height, secbg2);
		if (dragLife>0)
		iShowImage(mcarx, mcary, mCarWid + 30, 100, dan[danIndex]);

		/******************************dragon firing of 2nd level**************************/
		if (shoot5Flag) {
			iShowImage(shoot5x, shoot5y, 35, 25, shoot[shootIndex]);
		}if (shoot4Flag) {
			iShowImage(shoot4x, shoot4y, 35, 25, shoot[shootIndex]);
		}if (shoot3Flag) {
			iShowImage(shoot3x, shoot3y, 35, 25, shoot[shootIndex]);
		}if (shoot2Flag) {
			iShowImage(shoot2x, shoot2y, 35, 25, shoot[shootIndex]);
		}if (shoot1Flag) {
			iShowImage(shoot1x, shoot1y, 35, 25, shoot[shootIndex]);
		}

		/*********************controlling visibility of the imgae of zombies in 2nd level*************************/
		if (zombie[0].zomShow== true)
			iShowImage(zombie[0].x = 150, zombie[0].y, zomWid, zomHeight, zom1[zom1Index]);
		if (zombie[1].zomShow == true)
			iShowImage(zombie[1].x = 300, zombie[1].y, zomWid, zomHeight, zom1[zom1Index]);
		if (zombie[2].zomShow == true)
			iShowImage(zombie[2].x = 450, zombie[2].y, zomWid, zomHeight, zom1[zom1Index]);
		if (zombie[3].zomShow == true)
			iShowImage(zombie[3].x = 600, zombie[3].y, zomWid, zomHeight, zom1[zom1Index]);
		if (zombie[4].zomShow == true)
			iShowImage(zombie[4].x = 10, zombie[4].y, zomWid, zomHeight, zom1[zom1Index]);

		/**********************controlling the image of poison cloud****************************/
		iShowImage(cloud1x, cloud1y, cloudWidth, cloudHeight, cloud);
		iShowImage(cloud2x, cloud2y, cloudWidth, cloudHeight, cloud);
		iShowImage(cloud3x, cloud3y, cloudWidth, cloudHeight, cloud);
		if (dragLife <= 0){
			iShowImage(mcarx, mcary, mCarWid + 30, dragHeight, dragDie[dragDieIndex]);
			}

		iText(10, 380, "Score:  ", GLUT_BITMAP_TIMES_ROMAN_24);// showing the score on the screen
		iText(100, 380, (gameScore), GLUT_BITMAP_TIMES_ROMAN_24);
		if (dragLife > 0)/***********Stop showing the score when the dragon is dead*************/
		{
			iText(10, 400, "Dragon Life:  ", GLUT_BITMAP_TIMES_ROMAN_24);
			iText(150, 400, (drLife), GLUT_BITMAP_TIMES_ROMAN_24);
		}
		dragLifebar();
	}

}


void iMouseMove(int mx, int my)
{

}

void iPassiveMouse(int mx, int my)
{
	mouse_x = mx;/*************mouse cursor*****************/
	mouse_y = my;

}


void iMouse(int button, int state, int mx, int my)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (mx >= 320 && mx <= 400 && my >= 70 && my <= 150 && show == 0)//from openinng page to menu
		{
			/******************Playing Musics*******************/
			show = 1;
			PlaySound("music\\opening", NULL, SND_LOOP | SND_ASYNC);
		}
	
		if (mx >= 550 && mx <= 630 && my >= 400 && my <= 460 && show == 1)//from menu to play
		{
			newInitialise();/*Starts the game from the very beginning*/
			show = 7;
			/******************Playing Musics*******************/
			PlaySound("music\\opening", NULL, SND_LOOP | SND_ASYNC);
		}
	
		if (mx >= 500 && mx <= 680 && my >= 305 && my <= 375 && show == 1)//from menu to instruction
		{
			show = 4;
		}
	
		if (mx >= 490 && mx <= 690 && my >= 240 && my <= 300 && show == 1)//to highscore
		{
			show = 5;
		}
	
		if (mx >= 540 && mx <= 640 && my >= 160 && my <= 220 && show == 1)// to credits
		{
			show = 6;
		}
	
		if (mx >= 10 && mx <= 50 && my >= 430 && my <= 470 && show == 4) //backbutton of instruction
		{
			show = 1;
		}
		if (mx >= 10 && mx <= 50 && my >= 430 && my <= 470 && show == 5)//backbutton of highscore
		{
			show = 1;
		}
		if (mx >= 10 && mx <= 50 && my >= 430 && my <= 470 && show == 6)//backbutton of credits
		{
			show = 1;
		}
		
		if (mx >= 530 && mx <= 660 && my >= 90 && my <= 150 && show == 1 && returning== 1  && dragLife>0)// resuming game
		{
			if (level==1)
				show = 2;
			if (level == 2)
				show = 3;
		}
		if (mx >= 10 && mx <= 50 && my >= 430 && my <= 470 && show == 9)//backbutton of main menu
		{
			show = 1;
		}
	
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		iResumeTimer(0);
	}
}


void iKeyboard(unsigned char key)
{
	if (key == 'm')//muting music
	{
		PlaySound(0, 0, 0);
	}
	if (key == '\r')//starting game with enter
	{   
		if (show == 7){
			show = 2;
			resumeGame();
		}
	}
	if (key == 'p')//playing game
	{
		PlaySound("music\\opening", NULL, SND_LOOP | SND_ASYNC);
	}
	if (key == 'q')//returninng to menu
	{
		if (show == 2){
			level = 1;
			show = 1;
			returning = 1;
			pauseGame();
		}
		if (show == 3){
			level = 2;
			show = 1;
			returning = 1;
			pauseGame();
		}
		if (show == 8 || show == 9)
			show = 1;
	}
	if (key == ' '){/*.............dragon shooting..............*/
		if ((shoot5y >= mcary + dragHeight + 100 ) || shoot5y == shoot_y)//positioning the shoot
			shoot1Flag = true;
		if (shoot1Flag && shoot1y >= mcary + dragHeight + 100)
			shoot2Flag = true;
		if (shoot2Flag && shoot2y >= mcary + dragHeight + 100)
			shoot3Flag = true;
		if (shoot3Flag && shoot3y >= mcary + dragHeight + 100)
			shoot4Flag = true;
		if (shoot4Flag && shoot4y >= mcary + dragHeight + 100)
			shoot5Flag = true;
	}
	if (key == 's'){//pause
		pauseGame();
	}
	if (key == 'r'){//resume
		resumeGame();
	}
	if (key == 'n'){//next level
		if (show == 9)
		{
			show = 3;
			gamePoints = true;
		}
	}
}
void iSpecialKeyboard(unsigned char key)
{

	if(key==GLUT_KEY_RIGHT)
	{
		if (mcarx<600)//moving drag
			mcarx += 10;
		if (shoot1x<600 && shoot1Flag == false)//moving the shoots aswell
			shoot1x += 10;
		if (shoot2x<600 && shoot2Flag == false)
			shoot2x += 10;
		if (shoot3x<600 && shoot3Flag == false)
			shoot3x += 10;
		if (shoot4x<600 && shoot4Flag == false)
			shoot4x += 10;
		if (shoot5x<600 && shoot5Flag == false)
			shoot5x += 10;
	}


	if(key==GLUT_KEY_LEFT)
	{
		if (mcarx>0)
			mcarx -= 10;//moving drag
		if (shoot1x>30 && shoot1Flag == false)// moving the shoots with it
			shoot1x -= 10;
		if (shoot2x>30 && shoot2Flag == false)
			shoot2x -= 10;
		if (shoot3x>30 && shoot3Flag == false)
			shoot3x -= 10;
		if (shoot4x>30 && shoot4Flag == false)
			shoot4x -= 10;
		if (shoot5x>30 && shoot5Flag == false)
			shoot5x -= 10;
	}

}

void bgMove1(){
	if (show == 2){// background rendering for lvl1
		bg1_y--;
		bg2_y--;
		if (bg2_y == 0)
			bg1_y = 500;
		if (bg1_y == 0)
			bg2_y = 500;
	}
	if (show == 3){//background rendering for level 2
		secbg1_y--;
		secbg2_y--;
		if (secbg2_y == 0)
			secbg1_y = 500;
		if (secbg1_y == 0)
			secbg2_y = 500;
	}
}

void zombieMove(){/*movement of zombies in 2nd level*/
	zom1Index++;

	if (zom1Index == 3)
		zom1Index = 0;
	zom2Index++;
	if (zom2Index == 3)
		zom2Index = 0;
	zom3Index++;
	if (zom3Index == 3)
		zom3Index = 0;

	zombie[0].zomMove();
	zombie[1].zomMove();
	zombie[2].zomMove();
	zombie[3].zomMove();
	zombie[4].zomMove();
}

void dragMove(){
	dragIndex++;
	danIndex++;
	if (dragIndex == 3)//dragon movement render
		dragIndex = 0;
	if (danIndex == 3)//danny movement render level 2
		danIndex = 0;
	boss1Index++;
	if (boss1Index == 3)//boss1 movement render
		boss1Index = 0;
	shootIndex++;
	if (shootIndex == 3)//drag shoot render
		shootIndex = 0;
	zombieMove();
	if (show == 2){
		boss1move();
		boss1shooter();
	}
}

int main()
{

	for (int i = 0; i < 4; i++){//setting image of eggs
		egg[i].setImgLoc();
	}


	iInitialize(width,height, "All Men Must Die");

	iSetTimer(1, objectChange);/*timer for egg movement*/
	iSetTimer(10, bgMove1);/*timer for1st level's background image rendering*/
	iSetTimer(80, dragMove);/*Timer for 1st level dragon movement*/
	iSetTimer(3000, boolBossShoot); /*timer for boss firing*/
	iSetTimer(10000, ZombieMoveChange);/*timer for zombie movement*/
	iSetTimer(80, dragShootZombies);/*timer for dragon shoot of 2nd level*/
	iSetTimer(800, dragDying); /*Timer for the animation of dragon death*/

	tempLife = dragLife;
	cloud = iLoadImage("cloudt.png");
	cursor = iLoadImage("cursor.png");
	
	shoot[0] = iLoadImage("drags1.png");
	shoot[1] = iLoadImage("drags3.png");
	shoot[2] = iLoadImage("drags2.png");
	shoot[3] = iLoadImage("drags3.png");
	start = iLoadImage("start.jpg");
	menu = iLoadImage("menu.jpg");
	play = iLoadImage("play.png");
	fifeteen = iLoadImage("cEggs.png");
	instruction = iLoadImage("instruction.png");
	highestscore = iLoadImage("highestscore.png");
	insw = iLoadImage("bg1.png");
	credit = iLoadImage("credit.png");
	hsw = iLoadImage("hsw.jpg");
	cw = iLoadImage("bg2.jpg");
	backbutton = iLoadImage("backbutton.png");
	resume = iLoadImage("resume.png");

	/*Animation of 1st level dragon*/
	drag[0] = iLoadImage("bdrag1.png");
	drag[1] = iLoadImage("bdrag2.png");
	drag[2] = iLoadImage("bdrag3.png");
	drag[3] = iLoadImage("bdrag2.png");

	/*Animation of 2nd level hero*/
	dan[0] = iLoadImage("dan2.png");
	dan[1] = iLoadImage("dan3.png");
	dan[2] = iLoadImage("dan4.png");
	dan[3] = iLoadImage("dan3.png");

	/*Animation of dragon death*/
	dragDie[0] = iLoadImage("db1.png");
	dragDie[1] = iLoadImage("db2.png");
	dragDie[2] = iLoadImage("db3.png");
	dragDie[3] = iLoadImage("db4.png");
	dragDie[4] = iLoadImage("db5.png");
	dragDie[5] = iLoadImage("db6.png");
	/*image movement of zombie*/
	zom1[0] = iLoadImage("zom1.png");
	zom1[1] = iLoadImage("zom3.png");
	zom1[2] = iLoadImage("zom4.png");
	zom1[3] = iLoadImage("zom3.png");

	/*Image movement of boss*/

	boss1[0] = iLoadImage("kk2.png");
	boss1[1] = iLoadImage("kk1.png");
	boss1[2] = iLoadImage("kk3.png");
	boss1[3] = iLoadImage("kk1.png");

	go = iLoadImage("go.jpg");//image of game over
	flc = iLoadImage("flc.jpg");//image of First level completed

	/*Setting the image location of the egg*/
	for (int i = 0,j=0; i < 4; j++,i++){
		eggPic[j] = iLoadImage(egg[i].bImgLoc);
	}
	image7 = iLoadImage("gameover.png");
	bg1 = iLoadImage("fire3.png");
	bg2 = iLoadImage("fire2.png");
	secbg1 = iLoadImage("bgr2.jpg");
	secbg2 = iLoadImage("bgr3.jpg");

	/*Boss's blue flame image rendaring*/
	boss1shoot[0] = iLoadImage("wd1.png");
	boss1shoot[1] = iLoadImage("wd2.png");
	boss1shoot[2] = iLoadImage("wd3.png");
	boss1shoot[3] = iLoadImage("wd2.png");

	/*Images of the life bar*/
	lifebar[0] = iLoadImage("9.png");
	lifebar[1] = iLoadImage("8.png");
	lifebar[2] = iLoadImage("7.png");
	lifebar[3] = iLoadImage("6.png");
	lifebar[4] = iLoadImage("5.png");
	lifebar[5] = iLoadImage("4.png");
	lifebar[6] = iLoadImage("3.png");
	lifebar[7] = iLoadImage("2.png");
	lifebar[8] = iLoadImage("1.png");
	lifebar[9] = iLoadImage("0(2).png");
	lifebar[10] = iLoadImage("0(0).png");

	/*The destruction animation after the boss death*/
	bossDead[0] = iLoadImage("kdie0.png");
	bossDead[1] = iLoadImage("kdie1.png");
	bossDead[2] = iLoadImage("kdie2.png");
	bossDead[3] = iLoadImage("kdie3.png");
	bossDead[4] = iLoadImage("kdie4.png");
	bossDead[5] = iLoadImage("kdie5.png");
	bossDead[6] = iLoadImage("kdie6.png");
	bossDead[7] = iLoadImage("kdie7.png");
	bossDead[8] = iLoadImage("kdie8.png");
	bossDead[9] = iLoadImage("kdie9.png");

	iStart(); // it will start drawing

	

	return 0;
}

