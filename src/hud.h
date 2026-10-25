#ifndef HUD_H_INCLUDED
#define HUD_H_INCLUDED

// Everything drawn on top of the game (score, life, boss bar, pause, hit flash,
// score pop-ups), and the sounds and shakes that go with what happens in it.
// The game logic is untouched: hudTick() only watches its numbers change.

#include "text.h"
#include "audio.h"

iFont fontS, fontM, fontL;
int uiHeart, uiStar, uiSkull, uiEgg;

float hudFlash = 0;  // 1 right after a hit, fades to 0
float hudShake = 0;  // pixels, fades to 0
float shakeX = 0, shakeY = 0;

struct Popup
{
	float x, y;
	int age;
	char text[12];
	float r, g, b;
};
const int MAX_POPUPS = 8;
const int POPUP_TICKS = 50;
Popup popups[MAX_POPUPS];
int popupCount = 0;

void hudInit()
{
	iFontLoad(fontS, "Cinzel.ttf", 18);
	iFontLoad(fontM, "Cinzel.ttf", 28);
	iFontLoad(fontL, "Cinzel.ttf", 54);
	uiHeart = iLoadImage("ui_heart.png");
	uiStar = iLoadImage("ui_star.png");
	uiSkull = iLoadImage("ui_skull.png");
	uiEgg = iLoadImage("eg.png");
}

void hudPopup(float x, float y, const char *text, float r, float g, float b)
{
	if (popupCount == MAX_POPUPS)// the oldest one makes room
	{
		for (int i = 1; i < MAX_POPUPS; i++)
			popups[i - 1] = popups[i];
		popupCount--;
	}
	Popup &p = popups[popupCount++];
	p.x = x; p.y = y; p.age = 0; p.r = r; p.g = g; p.b = b;
	snprintf(p.text, sizeof p.text, "%s", text);
}

// Called by a 16 ms timer.
void hudTick()
{
	static double pLife = 50;
	static int pPoint = 0, pBoss = 100, pGame = 0, pShow = -1;
	static bool pShot = false;

	bool paused = iAnimPause[0] != 0;
	bool playing = (show == 2 || show == 3) && !paused;
	char text[12];

	static bool recorded = false;
	if (show == 1 || show == 7)// back at the menu, or a new game
		recorded = false;
	if (show == 7)
		lastRank = -1;// until then the list keeps showing where the last score landed
	if (show == 8 && !recorded)// the dragon has fallen
	{
		recorded = true;
		if (scoreRank(gamePoint) >= 0)
			show = 10;// ask for a name before the game-over screen
	}

	if (show != pShow)
	{
		if (show == 9)
			audioPlaySfx("win.wav");
		if (show == 8)
			audioPlaySfx("lose.wav");
		pShow = show;
	}
	if (playing)
	{
		if (point > pPoint)// an egg was caught
		{
			audioPlaySfx("catch.wav");
			if (gamePoints)
				hudPopup(mcarx + 30, mcary + dragHeight + 20, "+3", 1.0f, 0.85f, 0.4f);
		}
		if (boss1Life < pBoss)// the Night King was hit
		{
			audioPlaySfx("hit.wav");
			hudPopup(boss1x + 20, boss1y - 10, "+10", 0.7f, 0.9f, 1.0f);
		}
		else if (gamePoint - pGame == 2 && point == pPoint)// a zombie went down
		{
			audioPlaySfx("hit.wav");
			hudPopup(mcarx + 30, mcary + dragHeight + 110, "+2", 1.0f, 0.85f, 0.4f);
		}
		if (dragLife < pLife)// the dragon was hurt
		{
			audioPlaySfx("hurt.wav");
			hudFlash = 1.0f;
			hudShake = 7.0f;
			snprintf(text, sizeof text, "-%d", (int)(pLife - dragLife));
			hudPopup(mcarx + 30, mcary + dragHeight + 20, text, 1.0f, 0.35f, 0.3f);
		}
		if (shoot1Flag && !pShot)
			audioPlaySfx("fire.wav");
	}
	pLife = dragLife; pPoint = point; pBoss = boss1Life; pGame = gamePoint; pShot = shoot1Flag;

	if (hudFlash > 0)
		hudFlash = hudFlash > 0.05f ? hudFlash - 0.05f : 0;
	hudShake = hudShake > 0.3f ? hudShake * 0.85f : 0;
	shakeX = (rand() % 2001 / 1000.0f - 1) * hudShake;
	shakeY = (rand() % 2001 / 1000.0f - 1) * hudShake;

	if (!paused)
		for (int i = 0; i < popupCount;)
		{
			if (++popups[i].age >= POPUP_TICKS)
			{
				for (int j = i + 1; j < popupCount; j++)
					popups[j - 1] = popups[j];
				popupCount--;
			}
			else
				i++;
		}
}

static void rectGrad(float x0, float y0, float x1, float y1, const float *top, const float *bottom)
{
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glColor4fv(bottom); glVertex2f(x0, y0); glVertex2f(x1, y0);
	glColor4fv(top);    glVertex2f(x1, y1); glVertex2f(x0, y1);
	glEnd();
	glColor4f(1, 1, 1, 1);
}

static void hudBar(float x, float y, float w, float h, float fraction, const float *top, const float *bottom)
{
	const float back[4] = {0, 0, 0, 0.6f};
	if (fraction < 0) fraction = 0;
	if (fraction > 1) fraction = 1;
	rectGrad(x - 2, y - 2, x + w + 2, y + h + 2, back, back);
	if (fraction > 0)
		rectGrad(x, y, x + w * fraction, y + h, top, bottom);
	glColor4f(0.85f, 0.65f, 0.3f, 0.9f);
	glLineWidth(1.5f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x - 2, y - 2); glVertex2f(x + w + 2, y - 2); glVertex2f(x + w + 2, y + h + 2); glVertex2f(x - 2, y + h + 2);
	glEnd();
	glColor4f(1, 1, 1, 1);
}

// The score, life and boss bar for level 1 and level 2.
void drawHud()
{
	const float clear[4] = {0, 0, 0, 0}, shade[4] = {0, 0, 0, 0.7f};
	const float redTop[4] = {1.0f, 0.45f, 0.35f, 1}, redBottom[4] = {0.7f, 0.08f, 0.1f, 1};
	const float iceTop[4] = {0.85f, 0.97f, 1.0f, 1}, iceBottom[4] = {0.3f, 0.6f, 0.9f, 1};
	char text[32];

	rectGrad(0, 440, 720, 500, shade, clear);

	// left: eggs (level 1, before the boss), then life
	if (show == 2 && !boss1Show)
	{
		iShowImage(12, 463, 28, 28, uiEgg);
		snprintf(text, sizeof text, "%d / 15", point > 15 ? 15 : point);
		iFontDraw(fontM, 46, 468, text, 1.0f, 0.96f, 0.88f);
	}
	int life = dragLife > 0 ? (int)dragLife : 0;
	iShowImage(12, 384, 28, 28, uiHeart);
	snprintf(text, sizeof text, "%d", life);
	iFontDraw(fontM, 46, 390, text, 1.0f, 0.96f, 0.88f);

	// right: score and best
	snprintf(text, sizeof text, "%d", gamePoint);
	iFontDrawRight(fontS, 708, 482, "SCORE", 0.8f, 0.65f, 0.4f);
	iFontDrawRight(fontM, 708, 452, text, 1.0f, 0.96f, 0.88f);
	iShowImage(708 - (int)iFontWidth(fontM, text) - 36, 449, 28, 28, uiStar);
	snprintf(text, sizeof text, "BEST %d", sscore > gamePoint ? sscore : gamePoint);
	iFontDrawRight(fontS, 708, 424, text, 0.8f, 0.65f, 0.4f);

	// centre: the level, or the Night King's life
	if (show == 2 && boss1Show && boss1Life > 0)
	{
		iFontDrawCentered(fontS, 360, 484, "THE NIGHT KING", 0.8f, 0.93f, 1.0f);
		hudBar(210, 458, 300, 14, boss1Life / 100.0f, iceTop, iceBottom);
		iShowImage(172, 448, 32, 32, uiSkull);
		if (boss1y > 550)// he is still on his way in
		{
			float pulse = 0.65f + 0.35f * sinf(glutGet(GLUT_ELAPSED_TIME) / 140.0f);
			iFontDrawCentered(fontM, 360, 250, "THE NIGHT KING IS COMING", 0.8f, 0.93f, 1.0f, pulse);
		}
	}
	else
		iFontDrawCentered(fontS, 360, 480, show == 2 ? "LEVEL 1" : "LEVEL 2", 0.8f, 0.65f, 0.4f);
}

// Hit flash, score pop-ups and the pause screen. Draw last.
void hudOverlay()
{
	if (hudFlash > 0)
	{
		const float red[4] = {0.85f, 0.05f, 0.05f, 0.35f * hudFlash};
		rectGrad(-20, -20, 740, 520, red, red);
	}
	for (int i = 0; i < popupCount; i++)
	{
		const Popup &p = popups[i];
		float k = p.age / (float)POPUP_TICKS;
		iFontDraw(fontM, p.x, p.y + 38 * k, p.text, p.r, p.g, p.b, k < 0.6f ? 1.0f : (1.0f - k) / 0.4f);
	}
	if (iAnimPause[0] && (show == 2 || show == 3))
	{
		const float dim[4] = {0, 0, 0, 0.6f};
		rectGrad(-20, -20, 740, 520, dim, dim);
		iFontDrawCentered(fontL, 360, 265, "PAUSED", 1.0f, 0.9f, 0.6f);
		iFontDrawCentered(fontS, 360, 225, "Press R to resume", 0.9f, 0.85f, 0.75f);
	}
}

// A line of text on the end-of-game screens: "Your Score is :  42".
void hudResultLine(const char *label, const char *value)
{
	char line[96];
	snprintf(line, sizeof line, "%s  %s", label, value);
	iFontDrawCentered(fontM, 360, 150, line, 1.0f, 0.9f, 0.6f);
}


static void panel(float x0, float y0, float x1, float y1)
{
	const float fill[4] = {0.05f, 0.03f, 0.03f, 0.78f};
	rectGrad(x0, y0, x1, y1, fill, fill);
	glColor4f(0.85f, 0.65f, 0.3f, 0.9f);
	glLineWidth(2.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x0, y0); glVertex2f(x1, y0); glVertex2f(x1, y1); glVertex2f(x0, y1);
	glEnd();
	glColor4f(1, 1, 1, 1);
}

// The ranked list of names and scores.
void hudScoreList()
{
	char text[32];
	panel(130, 40, 590, 440);
	iFontDrawCentered(fontM, 360, 400, "HIGH SCORES", 0.96f, 0.8f, 0.45f);
	const float rule[4] = {0.85f, 0.65f, 0.3f, 0.6f};
	rectGrad(150, 386, 570, 387, rule, rule);

	const float medal[3][3] = {{1.0f, 0.82f, 0.35f}, {0.85f, 0.88f, 0.92f}, {0.9f, 0.6f, 0.35f}};
	for (int i = 0; i < MAX_SCORES; i++)
	{
		float y = 352 - i * 31;
		if (i == lastRank)// the score just set
		{
			const float glow[4] = {1.0f, 0.75f, 0.3f, 0.22f};
			rectGrad(140, y - 8, 580, y + 24, glow, glow);
		}
		snprintf(text, sizeof text, "%d", i + 1);
		iFontDrawRight(fontS, 176, y, text, 0.8f, 0.65f, 0.4f);
		if (i < scoreCount)
		{
			const float *c = i < 3 ? medal[i] : NULL;
			float r = c ? c[0] : 1.0f, g = c ? c[1] : 0.96f, b = c ? c[2] : 0.88f;
			iFontDraw(fontM, 196, y, scores[i].name, r, g, b);
			snprintf(text, sizeof text, "%d", scores[i].points);
			iFontDrawRight(fontM, 570, y, text, r, g, b);
		}
		else
			iFontDraw(fontM, 196, y, "-", 0.5f, 0.42f, 0.35f);
	}
}

// "Enter your name" after a score that makes the list.
void hudNameEntry()
{
	char text[48];
	iShowImage(0, 0, width, height, go);
	const float dim[4] = {0, 0, 0, 0.6f};
	rectGrad(0, 0, 720, 500, dim, dim);
	panel(150, 130, 570, 400);

	iFontDrawCentered(fontS, 360, 366, "NEW HIGH SCORE", 0.96f, 0.8f, 0.45f);
	snprintf(text, sizeof text, "%d", gamePoint);
	iFontDrawCentered(fontL, 360, 308, text, 1.0f, 0.96f, 0.88f);
	snprintf(text, sizeof text, "Rank %d of %d", scoreRank(gamePoint) + 1, MAX_SCORES);
	iFontDrawCentered(fontS, 360, 270, text, 0.8f, 0.65f, 0.4f);

	iFontDrawCentered(fontS, 360, 232, "Your name", 0.9f, 0.85f, 0.75f);
	const float box[4] = {0, 0, 0, 0.55f};
	rectGrad(200, 180, 520, 218, box, box);
	glColor4f(0.85f, 0.65f, 0.3f, 0.9f);
	glLineWidth(1.5f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(200, 180); glVertex2f(520, 180); glVertex2f(520, 218); glVertex2f(200, 218);
	glEnd();
	glColor4f(1, 1, 1, 1);
	iFontDraw(fontM, 212, 189, playerName, 1.0f, 0.96f, 0.88f);
	if ((glutGet(GLUT_ELAPSED_TIME) / 500) % 2 == 0)// blinking caret
		iFontDraw(fontM, 212 + iFontWidth(fontM, playerName), 189, "_", 0.96f, 0.8f, 0.45f);

	iFontDrawCentered(fontS, 360, 148, "Enter to save      Esc to skip", 0.8f, 0.65f, 0.4f);
}

// Under the score on the game-over screen, once it has been saved.
void hudRankLine()
{
	if (lastRank < 0)
		return;
	char line[64];
	snprintf(line, sizeof line, "Saved as %s   Rank %d", playerName, lastRank + 1);
	iFontDrawCentered(fontS, 360, 116, line, 0.96f, 0.8f, 0.45f);
}

#endif
