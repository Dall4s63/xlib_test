#ifndef KEYBOARD_H
#define KEYBOARD_H

/* This file will hold generic preprocessor definitions for 
 * keyboard keys for the game engine.
 *
 * This will be based on what I think is usefel at the time 
 * I suppose.
 */

#ifdef KB_ALL

#define KB_MISC
#define KB_LATIN

#endif

#ifdef KB_MY_DEFAULTS

#define KB_MISC
#define KB_LATIN

#endif

#define KB_NoSymbol     0x0000

#ifdef KB_MISC

#define KB_Escape       0xff01
#define KB_Return       0xff0d 

#define KB_Left         0xff51
#define KB_Up           0xff52
#define KB_Right        0xff53
#define KB_Down         0xff54

#define KB_Shift_L      0xffe1
#define KB_Shift_R      0xffe2
#define KB_Control_L    0xffe3
#define KB_Control_R    0xffe4
#define KB_Caps_Lock     0xffe5
#define KB_Shift_Lock    0xffe6

#define KB_Meta_L       0xffe7
#define KB_Meta_R       0xffe8
#define KB_Alt_L        0xffe9
#define KB_Alt_R        0xffea
#define KB_Super_L      0xffeb
#define KB_Super_R      0xffec
#define KB_Hyper_L      0xffed
#define KB_Hyper_R      0xffee

#endif


#ifdef KB_LATIN

#define KB_A            0x0041
#define KB_B            0x0042
#define KB_C            0x0043
#define KB_D            0x0044
#define KB_E            0x0045
#define KB_F            0x0046
#define KB_G            0x0047
#define KB_H            0x0048
#define KB_I            0x0049
#define KB_J            0x004a
#define KB_K            0x004b
#define KB_L            0x004c
#define KB_M            0x004d
#define KB_N            0x004e
#define KB_O            0x004f
#define KB_P            0x0050
#define KB_Q            0x0051
#define KB_R            0x0052
#define KB_S            0x0053
#define KB_T            0x0054
#define KB_U            0x0055
#define KB_V            0x0056
#define KB_W            0x0057
#define KB_X            0x0058
#define KB_Y            0x0059
#define KB_Z            0x005a

#endif

typedef unsigned int KeyId;

char *keyidstr(KeyId id);

#endif
