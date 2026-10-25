#include "audio.h"
#include "paths.h"
#include <stdio.h>

#define MA_NO_ENCODING
#define MA_NO_GENERATION
#define MA_NO_FLAC
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

static ma_engine engine;
static ma_sound sound;
static bool engineReady = false;
static bool soundReady = false;
static bool engineTried = false;

static void audioShutdown()
{
	if (soundReady)
		ma_sound_uninit(&sound);
	if (engineReady)
		ma_engine_uninit(&engine);
	soundReady = engineReady = false;
}

static bool muted = false;

static bool audioInit()
{
	if (!engineTried)
	{
		engineTried = true;
		engineReady = ma_engine_init(NULL, &engine) == MA_SUCCESS;
		if (engineReady)
			atexit(audioShutdown);
		else
			fprintf(stderr, "No sound device, playing without sound.\n");
	}
	return engineReady;
}

void audioPlayLoop(const char *name)
{
	if (!audioInit())
		return;
	muted = false;

	if (!soundReady)
	{
		std::string path = assetPath(std::string("audio/") + name);
		if (ma_sound_init_from_file(&engine, path.c_str(), MA_SOUND_FLAG_STREAM, NULL, NULL, &sound) != MA_SUCCESS)
		{
			fprintf(stderr, "Cannot load music %s\n", path.c_str());
			return;
		}
		ma_sound_set_looping(&sound, MA_TRUE);
		ma_sound_set_volume(&sound, 0.55f); // leave room for the effects
		soundReady = true;
	}
	ma_sound_seek_to_pcm_frame(&sound, 0);
	ma_sound_start(&sound);
}

void audioPlaySfx(const char *name)
{
	if (muted || !audioInit())
		return;
	std::string path = assetPath(std::string("audio/") + name);
	ma_engine_play_sound(&engine, path.c_str(), NULL);
}

void audioStop()
{
	muted = true;
	if (soundReady)
		ma_sound_stop(&sound);
}
