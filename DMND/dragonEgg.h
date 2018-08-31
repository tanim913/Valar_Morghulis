#ifndef DRAGONEGG_H_INCLUDED
#define DRAGONEGG_H_INCLUDED
# include "variable.h"
typedef struct dragEgg dragEgg;
struct dragEgg
{
	char bImgLoc[100];
	int x, y;

	dragEgg()
	{
		;
	}
	dragEgg(int _y)
	{
		y = 500 - 52;
	}
	void setImgLoc()
	{
		strcpy_s(bImgLoc, "eg.png");
	}
	void eggCheck1(){//checks for collisions
	if (show == 2){
		if (y <= mcary + dragHeight){
			if ((egg[0].x >= mcarx && egg[0].x <= mcarx + mCarWid) || (mcarx + mCarWid >= egg[0].x + 30 && mcarx <= egg[0].x + 30)){
				point++;
				if (gamePoints)
					gamePoint += 3;
				// file e save 
				eggShow1 = false;
			}
		}
	}
}
}egg[4];


void eggCheck2(){
	if (show == 2){
		if (egg[1].y <= mcary + dragHeight){
			if ((egg[1].x >= mcarx && egg[1].x <= mcarx + mCarWid) || (mcarx + mCarWid >= egg[1].x + 30 && mcarx <= egg[1].x + 30)){
				point++;
				if (gamePoints)
					gamePoint += 3;
				eggShow2 = false;

			}
		}
	}
}
void eggCheck3(){
	if (show == 2){
		if (egg[2].y <= mcary + dragHeight){
			if ((egg[2].x >= mcarx && egg[2].x <= mcarx + mCarWid) || (mcarx + mCarWid >= egg[2].x + 30 && mcarx <= egg[2].x + 30)){
				point++;
				if (gamePoints)
					gamePoint += 3;
				eggShow3 = false;
			}
		}
	}
}
void eggCheck4(){
	if (show == 2){
		if (egg[3].y <= mcary + dragHeight){
			if ((egg[3].x >= mcarx && egg[3].x <= mcarx + mCarWid) || (mcarx + mCarWid >= egg[3].x + 30 && mcarx <= egg[3].x + 30)){
				point++;
				if (gamePoints)
					gamePoint += 3;
				eggShow4 = false;
			}
		}
	}
}
void eggMove(){

	for (int i = 0; i < 4; i++){
		egg[i].y -= rand() % 6;
	}

	if (egg[0].y + 52 <= 0)
	{
		egg[0].y = 500 - 52;
		eggShow1 = true;

	}

	if (egg[1].y + 52 <= 0)
	{
		egg[1].y = 500 - 52;
		eggShow2 = true;
	}
	if (egg[2].y + 52 <= 0)
	{
		eggShow3 = true;
		egg[2].y = 500 - 52;
	}
	if (egg[3].y + 52 <= 0)
	{
		eggShow4 = true;
		egg[3].y = 500 - 52;
	}
}
void dragLifebar(){//shows life bar movement

	if (dragLife <= tempLife && dragLife > 9 * (tempLife / 10)){
		iShowImage(10, 420, 200, 50, lifebar[0]);
	}
	if (dragLife <= 9 * (tempLife / 10) && dragLife > 8 * (tempLife / 10)){
		iShowImage(10, 420, 180, 50, lifebar[1]);
	}
	if (dragLife <= 8 * (tempLife / 10) && dragLife > 7 * (tempLife / 10)){
		iShowImage(10, 420, 160, 50, lifebar[2]);
	}
	if (dragLife <= 7 * (tempLife / 10) && dragLife > 6 * (tempLife / 10)){
		iShowImage(10, 420, 140, 50, lifebar[3]);
	}
	if (dragLife <= 6 * (tempLife / 10) && dragLife > 5 * (tempLife / 10)){
		iShowImage(10, 420, 120, 50, lifebar[4]);
	}
	if (dragLife <= 5 * (tempLife / 10) && dragLife >4 * (tempLife / 10)){
		iShowImage(10, 420, 100, 50, lifebar[5]);
	}
	if (dragLife <= 4 * (tempLife / 10) && dragLife > 3 * (tempLife / 10)){
		iShowImage(10, 420, 80, 50, lifebar[6]);
	}
	if (dragLife <= 3 * (tempLife / 10) && dragLife > 2 * (tempLife / 10)){
		iShowImage(10, 420, 60, 50, lifebar[7]);
	}
	if (dragLife <= 2 * (tempLife / 10) && dragLife > 1 * (tempLife / 10)){
		iShowImage(10, 420, 40, 50, lifebar[8]);
	}
	if (dragLife <= 1 * (tempLife / 10) && dragLife > 0 * (tempLife / 10)){
		iShowImage(10, 420, 40, 50, lifebar[9]);
	}
	if (dragLife <= 0){
		iShowImage(10, 420, 40, 50, lifebar[10]);
	}

}
void dragDying()
{

	if (dragLife <= 0){
	
		dragDieIndex++;
	}
	
}
void dragShoot(){
	if (shoot1Flag) {
		shoot1y += 3;// movement of the  shoot
		if (shoot1y >= boss1y){//collision check with the boss1
			if ((shoot1x >= boss1x && shoot1x <= boss1x + boss1wid) || (boss1x + boss1wid >= shoot1x + 25 && boss1x <= shoot1x + 25)){
				boss1Life -= 10;
				if (gamePoints)
				gamePoint += 10;
				shoot1Flag = false;
				shoot1y = shoot_y;
				shoot1x = mcarx + 40;
			}
		}
	}

	if (shoot1y >= height){//returning of the shoot
		shoot1Flag = false;
		shoot1y = shoot_y;
		shoot1x = mcarx + 40;
	}
	if (shoot2Flag) {
		shoot2y += 3;
		if (shoot2y >= boss1y){
			if ((shoot2x >= boss1x && shoot2x <= boss1x + boss1wid) || (boss1x + boss1wid >= shoot2x + 25 && boss1x <= shoot2x + 25)){
				boss1Life -= 10;
				if (gamePoints)
				gamePoint += 10;
				shoot2Flag = false;
				shoot2y = shoot_y;
				shoot2x = mcarx + 40;
			}
		}
	}

	if (shoot2y >= height){
		shoot2Flag = false;
		shoot2y = shoot_y;
		shoot2x = mcarx + 40;
	}
	if (shoot3Flag) {
		shoot3y += 3;
		if (shoot3y >= boss1y){
			if ((shoot3x >= boss1x && shoot3x <= boss1x + boss1wid) || (boss1x + boss1wid >= shoot3x + 25 && boss1x <= shoot3x + 25)){
				boss1Life -= 10;
				if (gamePoints)
				gamePoint += 10;
				shoot3Flag = false;
				shoot3y = shoot_y;
				shoot3x = mcarx + 40;
			}
		}
	}

	if (shoot3y >= height){
		shoot3Flag = false;
		shoot3y = shoot_y;
		shoot3x = mcarx + 40;
	}
	if (shoot4Flag) {
		shoot4y += 3;
		if (shoot4y >= boss1y){
			if ((shoot4x >= boss1x && shoot4x <= boss1x + boss1wid) || (boss1x + boss1wid >= shoot4x + 25 && boss1x <= shoot4x + 25)){
				boss1Life -= 10;
				if (gamePoints)
				gamePoint += 10;
				shoot4Flag = false;
				shoot4y = shoot_y;
				shoot4x = mcarx + 40;
			}
		}
	}

	if (shoot4y >= height){
		shoot4Flag = false;
		shoot4y = shoot_y;
		shoot4x = mcarx + 40;
	}
	if (shoot5Flag) {
		shoot5y += 3;
		if (shoot5y >= boss1y){
			if ((shoot5x >= boss1x && shoot5x <= boss1x + boss1wid) || (boss1x + boss1wid >= shoot5x + 25 && boss1x <= shoot5x + 25)){
				boss1Life -= 10;
				if (gamePoints)
				gamePoint += 10;
				shoot5Flag = false;
				shoot5y = shoot_y;
				shoot5x = mcarx + 40;
			}
		}
	}

	if (shoot5y >= height){
		shoot5Flag = false;
		shoot5y = shoot_y;
		shoot5x = mcarx + 40;
	}
	
} 
#endif // DRAGONEGG_H_INCLUDED
