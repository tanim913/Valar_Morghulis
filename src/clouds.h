#ifndef CLOUDS_H_INCLUDED
#define CLOUDS_H_INCLUDED
# include "variable.h"
void poisonCloud(){
	if ((show == 3) || (show ==2) ){
		cloud1y -= 3;//vertical movement of clouds
		cloud2y -= 3;
		cloud3y -= 3;

		if (cloud1y + cloudHeight <= 0)
		{
			cloud1y = cloud_y;
			cloud1x = rand() % 580;// random horizontal movement of clouds
		}
		if (cloud2y + cloudHeight <= 0)
		{
			cloud2y = cloud_y;
			cloud2x = rand() % 580;
		}
		if (cloud3y + cloudHeight <= 0)
		{
			cloud3y = cloud_y;
			cloud3x = rand() % 580;
		}
		
		if (cloud1y <= mcary + dragHeight){//checking for collision of clouds and dragon
			if ((cloud1x >= mcarx && cloud1x <= mcarx + mCarWid) || (mcarx + mCarWid >= cloud1x + cloudWidth && mcarx <= cloud1x + cloudWidth)){
				dragLife -= 5;
				cloud1y = cloud_y;

			}
		}
		if (cloud2y <= mcary + dragHeight){
			if ((cloud2x >= mcarx && cloud2x <= mcarx + mCarWid) || (mcarx + mCarWid >= cloud2x + cloudWidth && mcarx <= cloud2x + cloudWidth)){
				dragLife -= 5;
				cloud2y = cloud_y;
			}
		}
		if (cloud3y <= mcary + dragHeight){
			if ((cloud3x >= mcarx && cloud3x <= mcarx + mCarWid) || (mcarx + mCarWid >= cloud3x + cloudWidth && mcarx <= cloud3x + cloudWidth)){
				dragLife -= 5;
				cloud3y = cloud_y;
			}
		}
	}
}

#endif // CLOUDS_H_INCLUDED
