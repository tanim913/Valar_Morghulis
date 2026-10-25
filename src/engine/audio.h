#ifndef AUDIO_H_INCLUDED
#define AUDIO_H_INCLUDED

// Sound from assets/audio/. Does nothing (no error) when the machine has no
// sound device.
void audioPlayLoop(const char *name); // looping music, starts from the beginning
void audioPlaySfx(const char *name);  // a short effect, may overlap itself
void audioStop();                     // stops the music and silences the effects

#endif
