#ifndef SCORE_H_INCLUDED
#define SCORE_H_INCLUDED

#include <fstream>
#include <iostream>
using namespace std;

int sscore; // the best score so far, kept in the player's data folder

void loadHighscore()
{
	ifstream fp(dataPath("highscore.txt"));
	if (!(fp >> sscore))
		sscore = 0;
}

void insertHighscore()
{
	if (gamePoint > sscore)// compares the current score with the highest score
	{
		sscore = gamePoint;
		ofstream fp(dataPath("highscore.txt"));
		fp << sscore << endl;
	}
}

#endif
