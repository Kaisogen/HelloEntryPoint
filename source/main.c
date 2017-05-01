#include "common.h"
#include "draw.h"
#include "hid.h"
#include "i2c.h"
// Constants
#define DISPLAY_TEXT "A9LH / Brahma payload!" //Theoretically, I should be able to change this message to anything else. I still haven't built it myself. (5/1/2017)
// Starting locations for cursor
#define START_X 10
#define START_Y 10


void Reboot()
{
    i2cWriteRegister(I2C_DEV_MCU, 0x20, 1 << 2);
    while(true);
}


void PowerOff()
{
    i2cWriteRegister(I2C_DEV_MCU, 0x20, 1 << 0);
    while (true);
}


int main()
{
    char clear_text[64] = { 0 };
    bool use_top = true; //Use top screen
    
    //Unsigned 32 bit ints start with the value of our constants.
    u32 cur_x = START_X; 
    u32 cur_y = START_Y;
    
    memset(clear_text, (int) ' ', strnlen(DISPLAY_TEXT, 63)); //Set clear_text to the value " " for the length of DISPLAY_TEXT constant. Basically clears the screen
    
    //I notice that the char clear_text has a length of 64, while this has a 63 stuck in here. Does this have anything to do with it? I'll experiment later.
    
    while( true ) {
        DrawStringF(cur_x, cur_y, use_top, DISPLAY_TEXT); //Write the string (constant) DISPLAY_TEXT at the location of the cursor.
        u32 pad_state = InputWait(); // Wait for input before proceeding?
        u32 increment = 1; //Speed of cursor
        DrawStringF(cur_x, cur_y, use_top, clear_text); //Clear screen after no input or time?
        if (pad_state & (BUTTON_B | BUTTON_START))
            break; //Leave after start or press b.
        if (pad_state & BUTTON_L1)
            increment = 10; //Speed Up
        if (pad_state & BUTTON_R1) {
            cur_x = START_X;
            cur_y = START_Y;
            use_top = !use_top;
            //Reset / Stop??
        }
        //Move cursor
        if (pad_state & BUTTON_UP)
            cur_y = (cur_y >= increment) ? cur_y - increment : 0;
        else if (pad_state & BUTTON_LEFT)
            cur_x = (cur_x >= increment) ? cur_x - increment : 0;
        else if (pad_state & BUTTON_DOWN)
            cur_y += increment;
        else if (pad_state & BUTTON_RIGHT)
            cur_x += increment;
    }
    
    Reboot();
    return 0;
}
