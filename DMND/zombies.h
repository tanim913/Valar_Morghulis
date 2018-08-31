#ifndef ZOMBIE_H_INCLUDED
#define ZOMBIE_H_INCLUDED
# include "variable.h"


typedef struct zom zom;

/**************Structer for zombie movement********************/
struct zom
{
	int x, y, life = 10;
	boolean zomShow = true;
	zom()
	{
		;
	}
	zom(int _y)
	{
		y = zombiePosition;
	}
	void zomMove()
	{
		if (show == 3)
		{

			y -= rand() % zomMoveFactor;
			if (y + 52 <= 0)
			{
				y = zombiePosition;
				zomShow = true;

			}

		}
	}
	void zomCheck(){//checks for collisions

		if (show == 3){
			if (y <= mcary + dragHeight){
				if ((x >= mcarx && x <= mcarx + mCarWid) || (mcarx + mCarWid >= x + zomWid && mcarx <= x + zomWid)){
					dragLife--;
					zomShow = false;
					y = zombiePosition;
				}
			}
		}
	}
	void place(){
		if (zomShow == true){
			zomCheck();
		}
	}

	void moveChange(){
		if (show == 3){
			zomMoveFactor++;
		}
	}

	void dragShootZom(){
		if (shoot1Flag) {
			shoot1y += 3;// movement of the  shoot
			if (shoot1y >= y){//collision check with zombie
				if ((shoot1x >= x && shoot1x <= x + zomWid) || (x + zomWid >= shoot1x + 25 && x <= shoot1x + 25)){
					life -= 10;
					y = zombiePosition;
					zomShow = true;
					if (gamePoints)
						gamePoint += 2;
					shoot1Flag = false;
					shoot1y = shoot_y;//repositioning the shoot
					shoot1x = mcarx + 40;//repositioning the shoot
				}
			}
		}

		if (shoot2Flag) {
			shoot2y += 3;// movement of the  shoot
			if (shoot2y >= y){//collision check with zombie
				if ((shoot2x >= x && shoot2x <= x + zomWid) || (x + zomWid >= shoot2x + 25 && x <= shoot2x + 25)){
					life -= 10;
					y = zombiePosition;
					zomShow = true;
					if (gamePoints)
						gamePoint += 2;
					shoot2Flag = false;
					shoot2y = shoot_y;
					shoot2x = mcarx + 40;
				}
			}
		}

		if (shoot3Flag) {
			shoot3y += 3;// movement of the  shoot
			if (shoot3y >= y){//collision check with zombie
				if ((shoot3x >= x && shoot3x <= x + zomWid) || (x + zomWid >= shoot3x + 25 && x <= shoot3x + 25)){
					life -= 10;
					y = zombiePosition;
					zomShow = true;
					if (gamePoints)
						gamePoint += 2;
					shoot3Flag = false;
					shoot3y = shoot_y;
					shoot3x = mcarx + 40;
				}
			}
		}

		if (shoot4Flag) {
			shoot4y += 3;// movement of the  shoot
			if (shoot4y >= y){//collision check with zombie
				if ((shoot4x >= x && shoot4x <= x + zomWid) || (x + zomWid >= shoot4x + 25 && x <= shoot4x + 25)){
					life -= 10;
					y = zombiePosition;
					zomShow = true;
					if (gamePoints)
						gamePoint += 2;
					shoot4Flag = false;
					shoot4y = shoot_y;
					shoot4x = mcarx + 40;
				}
			}
		}

		if (shoot5Flag) {
			shoot5y += 3;// movement of the  shoot
			if (shoot5y >= y){//collision check with zombie
				if ((shoot5x >= x && shoot5x <= x + zomWid) || (x + zomWid >= shoot5x + 25 && x <= shoot5x + 25)){
					life -= 10;
					y = zombiePosition;
					zomShow = true;
					if (gamePoints)
						gamePoint += 2;
					shoot5Flag = false;
					shoot5y = shoot_y;
					shoot5x = mcarx + 40;
				}
			}
		}

	}
};
zom zombie[5];
void ZombieMoveChange(){
	for (int i = 0; i < 5; i++){
		zombie[i].moveChange();
	}
}

void dragShootZombies()
{

	zombie[0].dragShootZom();
	zombie[1].dragShootZom();
	zombie[2].dragShootZom();
	zombie[3].dragShootZom();
	zombie[4].dragShootZom();

}

#endif // ZOMBIE_H_INCLUDED