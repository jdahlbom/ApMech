#ifndef AP_KEYEVENT
#define AP_KEYEVENT

namespace Ap {

typedef unsigned short Uint16;

/*
    Key codes matching those in SDL.h
*/
typedef enum {
    AP_K_UNDEF          = 0,
    AP_K_BACKSPACE      = 8,
    AP_K_TAB            = 9,
    AP_K_CLEAR          = 12,
    AP_K_RETURN         = 13,
    AP_K_PAUSE          = 19,
    AP_K_ESCAPE         = 27,
    AP_K_SPACE          = 32,
    AP_K_EXCLAIM        = 33,
    AP_K_QUOTEDBL       = 34,
    AP_K_HASH           = 35,
    AP_K_DOLLAR         = 36,
    AP_K_AMPERSAND      = 38,
    AP_K_QUOTE          = 39,
    AP_K_LEFTPAREN      = 40,
    AP_K_RIGHTPAREN     = 41,
    AP_K_ASTERISK       = 42,
    AP_K_PLUS           = 43,
    AP_K_COMMA          = 44,
    AP_K_MINUS          = 45,
    AP_K_PERIOD         = 46,
    AP_K_SLASH          = 47,
    AP_K_0              = 48,
    AP_K_1              = 49,
    AP_K_2              = 50,
    AP_K_3              = 51,
    AP_K_4              = 52,
    AP_K_5              = 53,
    AP_K_6              = 54,
    AP_K_7              = 55,
    AP_K_8              = 56,
    AP_K_9              = 57,
    AP_K_COLON          = 58,
    AP_K_SEMICOLON      = 59,
    AP_K_LESS           = 60,
    AP_K_EQUALS         = 61,
    AP_K_GREATER        = 62,
    AP_K_QUESTION       = 63,
    AP_K_AT             = 64,
    /*
        Skip uppercase letters
    */
    AP_K_LEFTBRACKET    = 91,
    AP_K_BACKSLASH      = 92,
    AP_K_RIGHTBRACKET   = 93,
    AP_K_CARET          = 94,
    AP_K_UNDERSCORE     = 95,
    AP_K_BACKQUOTE      = 96,
    AP_K_a              = 97,
    AP_K_b              = 98,
    AP_K_c              = 99,
    AP_K_d              = 100,
    AP_K_e              = 101,
    AP_K_f              = 102,
    AP_K_g              = 103,
    AP_K_h              = 104,
    AP_K_i              = 105,
    AP_K_j              = 106,
    AP_K_k              = 107,
    AP_K_l              = 108,
    AP_K_m              = 109,
    AP_K_n              = 110,
    AP_K_o              = 111,
    AP_K_p              = 112,
    AP_K_q              = 113,
    AP_K_r              = 114,
    AP_K_s              = 115,
    AP_K_t              = 116,
    AP_K_u              = 117,
    AP_K_v              = 118,
    AP_K_w              = 119,
    AP_K_x              = 120,
    AP_K_y              = 121,
    AP_K_z              = 122,
    AP_K_DELETE         = 127,
    /* End of ASCII mapped keysyms */

    /* International keyboard syms */
    AP_K_WORLD_0        = 160,        /* 0xA0 */
    AP_K_WORLD_1        = 161,
    AP_K_WORLD_2        = 162,
    AP_K_WORLD_3        = 163,
    AP_K_WORLD_4        = 164,
    AP_K_WORLD_5        = 165,
    AP_K_WORLD_6        = 166,
    AP_K_WORLD_7        = 167,
    AP_K_WORLD_8        = 168,
    AP_K_WORLD_9        = 169,
    AP_K_WORLD_10       = 170,
    AP_K_WORLD_11       = 171,
    AP_K_WORLD_12       = 172,
    AP_K_WORLD_13       = 173,
    AP_K_WORLD_14       = 174,
    AP_K_WORLD_15       = 175,
    AP_K_WORLD_16       = 176,
    AP_K_WORLD_17       = 177,
    AP_K_WORLD_18       = 178,
    AP_K_WORLD_19       = 179,
    AP_K_WORLD_20       = 180,
    AP_K_WORLD_21       = 181,
    AP_K_WORLD_22       = 182,
    AP_K_WORLD_23       = 183,
    AP_K_WORLD_24       = 184,
    AP_K_WORLD_25       = 185,
    AP_K_WORLD_26       = 186,
    AP_K_WORLD_27       = 187,
    AP_K_WORLD_28       = 188,
    AP_K_WORLD_29       = 189,
    AP_K_WORLD_30       = 190,
    AP_K_WORLD_31       = 191,
    AP_K_WORLD_32       = 192,
    AP_K_WORLD_33       = 193,
    AP_K_WORLD_34       = 194,
    AP_K_WORLD_35       = 195,
    AP_K_WORLD_36       = 196,
    AP_K_WORLD_37       = 197,
    AP_K_WORLD_38       = 198,
    AP_K_WORLD_39       = 199,
    AP_K_WORLD_40       = 200,
    AP_K_WORLD_41       = 201,
    AP_K_WORLD_42       = 202,
    AP_K_WORLD_43       = 203,
    AP_K_WORLD_44       = 204,
    AP_K_WORLD_45       = 205,
    AP_K_WORLD_46       = 206,
    AP_K_WORLD_47       = 207,
    AP_K_WORLD_48       = 208,
    AP_K_WORLD_49       = 209,
    AP_K_WORLD_50       = 210,
    AP_K_WORLD_51       = 211,
    AP_K_WORLD_52       = 212,
    AP_K_WORLD_53       = 213,
    AP_K_WORLD_54       = 214,
    AP_K_WORLD_55       = 215,
    AP_K_WORLD_56       = 216,
    AP_K_WORLD_57       = 217,
    AP_K_WORLD_58       = 218,
    AP_K_WORLD_59       = 219,
    AP_K_WORLD_60       = 220,
    AP_K_WORLD_61       = 221,
    AP_K_WORLD_62       = 222,
    AP_K_WORLD_63       = 223,
    AP_K_WORLD_64       = 224,
    AP_K_WORLD_65       = 225,
    AP_K_WORLD_66       = 226,
    AP_K_WORLD_67       = 227,
    AP_K_WORLD_68       = 228,
    AP_K_WORLD_69       = 229,
    AP_K_WORLD_70       = 230,
    AP_K_WORLD_71       = 231,
    AP_K_WORLD_72       = 232,
    AP_K_WORLD_73       = 233,
    AP_K_WORLD_74       = 234,
    AP_K_WORLD_75       = 235,
    AP_K_WORLD_76       = 236,
    AP_K_WORLD_77       = 237,
    AP_K_WORLD_78       = 238,
    AP_K_WORLD_79       = 239,
    AP_K_WORLD_80       = 240,
    AP_K_WORLD_81       = 241,
    AP_K_WORLD_82       = 242,
    AP_K_WORLD_83       = 243,
    AP_K_WORLD_84       = 244,
    AP_K_WORLD_85       = 245,
    AP_K_WORLD_86       = 246,
    AP_K_WORLD_87       = 247,
    AP_K_WORLD_88       = 248,
    AP_K_WORLD_89       = 249,
    AP_K_WORLD_90       = 250,
    AP_K_WORLD_91       = 251,
    AP_K_WORLD_92       = 252,
    AP_K_WORLD_93       = 253,
    AP_K_WORLD_94       = 254,
    AP_K_WORLD_95       = 255,        /* 0xFF */

    /* Numeric keypad */
    AP_K_KP0            = 256,
    AP_K_KP1            = 257,
    AP_K_KP2            = 258,
    AP_K_KP3            = 259,
    AP_K_KP4            = 260,
    AP_K_KP5            = 261,
    AP_K_KP6            = 262,
    AP_K_KP7            = 263,
    AP_K_KP8            = 264,
    AP_K_KP9            = 265,
    AP_K_KP_PERIOD      = 266,
    AP_K_KP_DIVIDE      = 267,
    AP_K_KP_MULTIPLY    = 268,
    AP_K_KP_MINUS       = 269,
    AP_K_KP_PLUS        = 270,
    AP_K_KP_ENTER       = 271,
    AP_K_KP_EQUALS      = 272,

    /* Arrows + Home/End pad */
    AP_K_UP             = 273,
    AP_K_DOWN           = 274,
    AP_K_RIGHT          = 275,
    AP_K_LEFT           = 276,
    AP_K_INSERT         = 277,
    AP_K_HOME           = 278,
    AP_K_END            = 279,
    AP_K_PAGEUP         = 280,
    AP_K_PAGEDOWN       = 281,

    /* Function keys */
    AP_K_F1             = 282,
    AP_K_F2             = 283,
    AP_K_F3             = 284,
    AP_K_F4             = 285,
    AP_K_F5             = 286,
    AP_K_F6             = 287,
    AP_K_F7             = 288,
    AP_K_F8             = 289,
    AP_K_F9             = 290,
    AP_K_F10            = 291,
    AP_K_F11            = 292,
    AP_K_F12            = 293,
    AP_K_F13            = 294,
    AP_K_F14            = 295,
    AP_K_F15            = 296,

    /* Key state modifier keys */
    AP_K_NUMLOCK        = 300,
    AP_K_CAPSLOCK       = 301,
    AP_K_SCROLLOCK      = 302,
    AP_K_RSHIFT         = 303,
    AP_K_LSHIFT         = 304,
    AP_K_RCTRL          = 305,
    AP_K_LCTRL          = 306,
    AP_K_RALT           = 307,
    AP_K_LALT           = 308,
    AP_K_RMETA          = 309,
    AP_K_LMETA          = 310,
    AP_K_MODE           = 313,        /* "Alt Gr" key */


    /* Miscellaneous function keys */
    AP_K_HELP           = 315,
    AP_K_PRINT          = 316,
    AP_K_SYSREQ         = 317,
    AP_K_BREAK          = 318,
    AP_K_MENU           = 319,
    AP_K_POWER          = 320,        /* Power Macintosh power key */
    AP_K_EURO           = 321,        /* Some european keyboards */
    AP_K_UNDO           = 322,        /* Atari keyboard has Undo */

} KeySymbol;

typedef enum {
    AP_E_KEYDOWN,
    AP_E_KEYUP
} KeyEventType;

typedef struct{
    KeyEventType  type;
    KeySymbol     key;
    Uint16        unicode;
} KeyEvent;

} // namespace Ap

#endif