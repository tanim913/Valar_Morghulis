#ifndef AUDIO_H_INCLUDED
#define AUDIO_H_INCLUDED

// Looping background music from assets/audio/<name>. Does nothing (no error)
// when the machine has no sound device.
void audioPlayLoop(const char *name); // starts from the beginning
void audioStop();

#endif
