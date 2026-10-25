#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

// TrueType text. A font is baked once into a texture, then drawn glyph by glyph.
// Needs iGraphics.h first (OpenGL, assetPath).

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#include <vector>

struct iFont
{
	unsigned int tex = 0;
	stbtt_bakedchar glyph[96]; // ASCII 32..127
};

static const int I_FONT_ATLAS = 512;

// Loads assets/fonts/<name> at a pixel size. Call after iInitialize (it needs OpenGL).
inline bool iFontLoad(iFont &f, const char *name, float px)
{
	std::string path = assetPath(std::string("fonts/") + name);
	FILE *fp = fopen(path.c_str(), "rb");
	if (!fp)
	{
		fprintf(stderr, "Cannot load font %s\n", path.c_str());
		return false;
	}
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	std::vector<unsigned char> ttf(size);
	bool read = fread(ttf.data(), 1, size, fp) == (size_t)size;
	fclose(fp);

	std::vector<unsigned char> atlas(I_FONT_ATLAS * I_FONT_ATLAS);
	if (!read || stbtt_BakeFontBitmap(ttf.data(), 0, px, atlas.data(), I_FONT_ATLAS, I_FONT_ATLAS, 32, 96, f.glyph) <= 0)
	{
		fprintf(stderr, "Cannot bake font %s at %.0f px\n", path.c_str(), px);
		return false;
	}

	glGenTextures(1, &f.tex);
	glBindTexture(GL_TEXTURE_2D, f.tex);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, I_FONT_ATLAS, I_FONT_ATLAS, 0, GL_ALPHA, GL_UNSIGNED_BYTE, atlas.data());
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	return true;
}

inline float iFontWidth(const iFont &f, const char *s)
{
	float x = 0, y = 0;
	for (; *s; s++)
	{
		if (*s < 32)
			continue;
		stbtt_aligned_quad q;
		stbtt_GetBakedQuad(f.glyph, I_FONT_ATLAS, I_FONT_ATLAS, *s - 32, &x, &y, &q, 1);
	}
	return x;
}

// (x, y) is the left end of the baseline; y goes up, like the rest of iGraphics.
// A soft shadow keeps the letters readable over any picture.
inline void iFontDraw(const iFont &f, float x, float y, const char *s, float r, float g, float b, float a = 1.0f)
{
	if (!f.tex)
		return;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, f.tex);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	for (int pass = 0; pass < 2; pass++)
	{
		float shift = pass == 0 ? 1.5f : 0.0f;
		if (pass == 0)
			glColor4f(0, 0, 0, a * 0.7f);
		else
			glColor4f(r, g, b, a);
		float cx = x, cy = 0;
		glBegin(GL_QUADS);
		for (const char *p = s; *p; p++)
		{
			if (*p < 32)
				continue;
			stbtt_aligned_quad q;
			stbtt_GetBakedQuad(f.glyph, I_FONT_ATLAS, I_FONT_ATLAS, *p - 32, &cx, &cy, &q, 1);
			float top = y - q.y0 - shift, bottom = y - q.y1 - shift;
			glTexCoord2f(q.s0, q.t1); glVertex2f(q.x0 + shift, bottom);
			glTexCoord2f(q.s1, q.t1); glVertex2f(q.x1 + shift, bottom);
			glTexCoord2f(q.s1, q.t0); glVertex2f(q.x1 + shift, top);
			glTexCoord2f(q.s0, q.t0); glVertex2f(q.x0 + shift, top);
		}
		glEnd();
	}
	glColor4f(1, 1, 1, 1);
	glDisable(GL_TEXTURE_2D);
}

inline void iFontDrawCentered(const iFont &f, float cx, float y, const char *s, float r, float g, float b, float a = 1.0f)
{
	iFontDraw(f, cx - iFontWidth(f, s) / 2, y, s, r, g, b, a);
}

inline void iFontDrawRight(const iFont &f, float right, float y, const char *s, float r, float g, float b, float a = 1.0f)
{
	iFontDraw(f, right - iFontWidth(f, s), y, s, r, g, b, a);
}

#endif
