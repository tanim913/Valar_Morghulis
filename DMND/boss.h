#ifndef BOSS_H_INCLUDED
#define BOSS_H_INCLUDED
# include "variable.h"

void boss1move(){
	if (point >= 15){
		if (boss1y > 400){//gradual decending of boss1
			boss1y -= 3;
			boss1shoot1y -= 3;
		}
	}
	if (boss1y == 400 && boss1Show)
	{
		boss1x += dx;//horizontal movement of boss1
		if (boss1shoot1x<645 && boss1shoot1 == false)
			boss1shoot1x += dx;
		if (boss1x + boss1height >= 700 || boss1x <= 0)
			dx = -dx;
	}
	if (countt==1){//after boss is dead
		boss1y -= 8;//boss decending
		bossDieIndex++;//inndex increasing
		if (bossDieIndex > 9)
			bossDieIndex = 9;
	}
}
void boss1shooter()
{
	if (boss1shoot1) {
		boss1shoot1y -= 10;// movement of boss1 shoot
		if (boss1shoot1y <= mcary + dragHeight){// collision check with drag
			if ((boss1shoot1x >= mcarx && boss1shoot1x <= mcarx + mCarWid) || (mcarx + mCarWid >= boss1shoot1x + 25 && mcarx <= boss1shoot1x + 25)){
				dragLife -= 30;
				//gamePoint += 5;
				boss1shoot1 = false;
				boss1shoot1y = shoot__y;
				boss1shoot1x = boss1x + 40;
			}
		}
		boss1shootIndex++;// rendering of boss1 shoot
		if (boss1shootIndex == 3)
			boss1shootIndex = 0;
	}
}
void boolBossShoot(){
	if (boss1y == 400 && bossLife>0)
		boss1shoot1 = true;	
}
void bossVerticalMove(){

	if (boss1shoot1y <= 0)
	{
		boss1shoot1y = 400;
		boss1shoot1x = boss1x;
	}
}

#endif // BOSS_H_INCLUDED