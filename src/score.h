#ifndef SCORE_H_INCLUDED
#define SCORE_H_INCLUDED

// The top ten scores, each with the player's name, kept in the player's data
// folder (highscores.txt, one "points name" line per score, best first).

#include <cstdio>
#include <cstring>
#include <cctype>

const int MAX_SCORES = 10;
const int NAME_LEN = 13; // 12 letters and the end of the string

struct Score
{
	char name[NAME_LEN];
	int points;
};

Score scores[MAX_SCORES];
int scoreCount = 0;
int sscore = 0;          // the best score so far
int lastRank = -1;       // row of the score just added, or -1
char playerName[NAME_LEN] = "";// what is being typed, and the name used last time

void loadHighscores()
{
	scoreCount = 0;
	FILE *fp = fopen(dataPath("highscores.txt").c_str(), "r");
	if (fp)
	{
		char line[64];
		while (scoreCount < MAX_SCORES && fgets(line, sizeof line, fp))
		{
			Score &s = scores[scoreCount];
			if (sscanf(line, "%d %12[^\n]", &s.points, s.name) == 2 && s.points > 0)
				scoreCount++;
		}
		fclose(fp);
	}
	sscore = scoreCount ? scores[0].points : 0;
}

void saveHighscores()
{
	FILE *fp = fopen(dataPath("highscores.txt").c_str(), "w");
	if (!fp)
		return;
	for (int i = 0; i < scoreCount; i++)
		fprintf(fp, "%d %s\n", scores[i].points, scores[i].name);
	fclose(fp);
}

// Where a score would land, 0 for first place; -1 when it would not make the list.
int scoreRank(int points)
{
	if (points <= 0)
		return -1;
	int rank = 0;
	while (rank < scoreCount && scores[rank].points >= points)// an older equal score stays ahead
		rank++;
	return rank < MAX_SCORES ? rank : -1;
}

// Adds a score and saves. Returns its row, or -1 if it did not make the list.
int addHighscore(const char *name, int points)
{
	int rank = scoreRank(points);
	if (rank < 0)
		return -1;
	int last = scoreCount < MAX_SCORES ? scoreCount : MAX_SCORES - 1;
	for (int i = last; i > rank; i--)
		scores[i] = scores[i - 1];
	snprintf(scores[rank].name, NAME_LEN, "%s", name);
	scores[rank].points = points;
	if (scoreCount < MAX_SCORES)
		scoreCount++;
	sscore = scores[0].points;
	saveHighscores();
	return rank;
}

// One key typed on the "enter your name" screen. Returns true when it is over.
bool nameKey(unsigned char key, int points)
{
	int n = (int)strlen(playerName);
	if (key == 27)// Esc: skip, nothing is saved
	{
		lastRank = -1;
		return true;
	}
	if (key == '\r' || key == '\n')
	{
		while (n > 0 && playerName[n - 1] == ' ')// trailing spaces
			playerName[--n] = 0;
		const char *start = playerName;
		while (*start == ' ')
			start++;
		char name[NAME_LEN];
		snprintf(name, sizeof name, "%s", *start ? start : "Player");
		snprintf(playerName, sizeof playerName, "%s", name);
		lastRank = addHighscore(name, points);
		return true;
	}
	if ((key == 8 || key == 127) && n > 0)
		playerName[n - 1] = 0;
	else if (n < NAME_LEN - 1 && (isalnum(key) || key == ' ' || key == '-' || key == '_' || key == '.'))
	{
		playerName[n] = (char)key;
		playerName[n + 1] = 0;
	}
	return false;
}

#endif
