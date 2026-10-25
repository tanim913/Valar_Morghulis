// Checks the high-score list: ordering, the top-ten limit, ties, saving and loading,
// and the name typing keys. Run by CTest (cmake --build build && ctest --test-dir build).
#include <assert.h>
#include <stdlib.h>
#include <filesystem>
#include <string>

std::string testDir;
std::string dataPath(const std::string &file) { return testDir + "/" + file; }

#include "score.h"

static void typeName(const char *s)
{
	for (; *s; s++)
		nameKey((unsigned char)*s, 0);
}

int main()
{
	testDir = (std::filesystem::temp_directory_path() / "amm_score_test").string();
	std::filesystem::remove_all(testDir);
	std::filesystem::create_directories(testDir);

	loadHighscores();                                  // no file yet
	assert(scoreCount == 0 && sscore == 0);
	assert(scoreRank(0) == -1);                        // a zero score never counts
	assert(addHighscore("Ann", 50) == 0);
	assert(addHighscore("Bob", 80) == 0 && scores[1].points == 50);   // new best goes first
	assert(addHighscore("Cy", 50) == 2);               // a tie goes after the older score
	assert(sscore == 80);

	for (int i = 1; i <= 10; i++)                      // fill the list past ten
		addHighscore("Fill", 100 + i);
	assert(scoreCount == MAX_SCORES);
	assert(scores[0].points == 110 && scores[9].points == 101);
	assert(scoreRank(100) == -1);                      // too low for a full list
	assert(scoreRank(105) == 6);                       // an older 105 stays ahead of it

	loadHighscores();                                  // what was saved comes back the same
	assert(scoreCount == MAX_SCORES && scores[0].points == 110 && strcmp(scores[0].name, "Fill") == 0);

	playerName[0] = 0;                                 // typing a name
	typeName("  Tan!m  ");                             // "!" is not allowed
	assert(strcmp(playerName, "  Tanm  ") == 0);
	nameKey(8, 0);                                     // backspace
	assert(strcmp(playerName, "  Tanm ") == 0);
	assert(nameKey('\r', 0) && strcmp(playerName, "Tanm") == 0);   // Enter trims it
	playerName[0] = 0;
	nameKey('\r', 0);                                  // an empty name becomes "Player"
	assert(strcmp(playerName, "Player") == 0);
	playerName[0] = 0;
	typeName("abcdefghijklmnopqrstuvwxyz");
	assert(strlen(playerName) == 12);                  // longest name
	assert(nameKey(27, 0) && lastRank == -1);          // Esc skips

	std::filesystem::remove_all(testDir);
	return 0;
}
