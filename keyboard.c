#define KB_ALL
#include "keyboard.h"

char *keyidstr(KeyId id) {
    switch (id) {
    case KB_NoSymbol: return "NoSymbol";

    case KB_Escape: return "Escape";
    case KB_Return: return "Return";

    case KB_Left: return "Left";
    case KB_Up: return "Up";
    case KB_Right: return "Right";
    case KB_Down: return "Down";

    case KB_Shift_L: return "Left Shift";
    case KB_Shift_R: return "Right Shift";
    case KB_Control_L: return "Left Control";
    case KB_Control_R: return "Right Control";
    case KB_Caps_Lock: return "Caps Lock";
    case KB_Shift_Lock: return "Shift Lock";

    case KB_Meta_L: return "Left Meta";
    case KB_Meta_R: return "Right Meta";
    case KB_Alt_L: return "Left Alt";
    case KB_Alt_R: return "Right Alt";
    case KB_Super_L: return "Left Super";
    case KB_Super_R: return "Right Super";
    case KB_Hyper_L: return "Left Hyper";
    case KB_Hyper_R: return "Right Hyper";


    case KB_A: return "A";
    case KB_B: return "B";
    case KB_C: return "C";
    case KB_D: return "D";
    case KB_E: return "E";
    case KB_F: return "F";
    case KB_G: return "G";
    case KB_H: return "H";
    case KB_I: return "I";
    case KB_J: return "J";
    case KB_K: return "K";
    case KB_L: return "L";
    case KB_M: return "M";
    case KB_N: return "N";
    case KB_O: return "O";
    case KB_P: return "P";
    case KB_Q: return "Q";
    case KB_R: return "R";
    case KB_S: return "S";
    case KB_T: return "T";
    case KB_U: return "U";
    case KB_V: return "V";
    case KB_W: return "W";
    case KB_X: return "X";
    case KB_Y: return "Y";
    case KB_Z: return "Z";
    default: return "Unknown Id";
    }
}
