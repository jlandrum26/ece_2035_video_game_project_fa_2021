#ifndef SPEECH_H
#define SPEECH_H

/**
 * Display a speech bubble.
 */
void speech(const char* line1, const char* line2);

/**
 * Display a speech bubble with an int.
 */
void speech_adv(const char* line1, int num);

/**
 * Display a long speech bubble (more than 2 lines).
 * 
 * @param lines The actual lines of text to display
 * @param n The number of lines to display.
 */
void long_speech(const char* lines[], int n);

/**
 * Custom long_speech.
 *
 * @param talk Text displayed
 */
void orator(const char* talk);

#endif // SPEECH_H