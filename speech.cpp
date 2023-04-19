#include "speech.h"

#include "globals.h"
#include "hardware.h"

/**
 * Draw the speech bubble background.
 * Use this file to draw speech bubbles, look at the uLCD libraries for printing
 * text to the uLCD (see graphics.cpp note for link)
 */
static void draw_speech_bubble();

/**
 * Erase the speech bubble.
 */
static void erase_speech_bubble();

/**
 * Draw a single line of the speech bubble.
 * @param line The text to display
 * @param which If TOP, the first line; if BOTTOM, the second line.
 */
#define TOP    0
#define BOTTOM 1
static void draw_speech_line(const char* line, int which);

/**
 * Draw a single line of the speech bubble with ints.
 */
static void draw_speech_adv(int line, int which);

/**
 * Delay until it is time to scroll.
 */
static void speech_bubble_wait();

void draw_speech_bubble()// method included
{
    uLCD.rectangle(0, 100, 127, 117, GREEN); // uLCD.printf() or text_string()?
    //uLCD.filled_rectangle(10, 50, 20, 60, RED);
    //uLCD.filled_rectangle(30, 50, 40, 60, RED);
    //uLCD.filled_rectangle(50, 50, 60, 60, RED);
    //uLCD.filled_rectangle(70, 50, 80, 60, RED);
}

void erase_speech_bubble()
{
    uLCD.filled_rectangle(0, 100, 127, 119, BLACK);
}

void draw_speech_line(const char* line, int which)
{
    draw_speech_bubble(); // need something to specify TOP & BOTTOM
    if (which == TOP) {
        uLCD.locate(0, 13);
    } else if (which == BOTTOM) {
        uLCD.locate(0, 14);
    }
    uLCD.printf(line);
}

void draw_speech_adv(int line, int which)
{
    draw_speech_bubble(); // need something to specify TOP & BOTTOM
    if (which == TOP) {
        uLCD.locate(0, 13);
    } else if (which == BOTTOM) {
        uLCD.locate(0, 14);
    }
    uLCD.printf("%d", line);
}

void speech_bubble_wait()
{
    // do something with timer
    wait_ms(2500);
}
// only for two lines
void speech(const char* line1, const char* line2)
{
    draw_speech_bubble();
    draw_speech_line(line1, TOP);
    draw_speech_line(line2, BOTTOM);
    speech_bubble_wait();
    erase_speech_bubble();
}
void speech_adv(const char* line1, int num){
    draw_speech_bubble();
    draw_speech_line(line1, TOP);
    draw_speech_adv(num, BOTTOM);
    speech_bubble_wait();
    erase_speech_bubble();
}
// call draw_speech in unique ways
void long_speech(const char* lines[], int n)
{
    // split lines[] into various lines
    // save various lines into variables and
    // call speech on them until gone
    // do your own approach
}

void orator(const char* talk)
{
    //char sub[talk.length] = (char[]) talk;
//    char line1 = (char) talk.substr(0, 16);
//    char line2 = (char) talk.substr(17, 33);
//    char left = (char) talk.substr(34, talk.length);
//    speech(line1, line2);
//    while(left.length != 0) {
//        speaker(left);
//    }
}