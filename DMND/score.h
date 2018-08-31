#ifndef SCORE_H_INCLUDED
#define SCORE_H_INCLUDED

# include <string>
#include <cstring>
#include <fstream>
#include <iostream>
using namespace std;



int sscore;
int sc;

char fname[50] = "SCR.txt"; // file name

// to creat file.................
void creatFile(){

	
	ofstream fp;// opening file stream

	fp.open("SCR.txt");//opening a file 
	
	void close();//closing a file

}


void insertHighscore()
{
	if (gamePoint > sscore)//compares the current score with the highest score

	{
		sscore = gamePoint;//assigning the total game score to temporary variable
		
		ofstream fp;

	
		fp.open("SCR.txt", ios::out);//printing the highest score into file
		
		fp << sscore << endl;

	
		void close();//closing the file
	}
		


}


#endif 