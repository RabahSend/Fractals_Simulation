/*
 * ez-draw++.cpp: basic EZ-Draw++ module
 *
 * eric.remy@univ-amu.fr - 30/03/2022 - version 1.2-6
 *
 * This program is free software under the terms of the
 * GNU Lesser General Public License (LGPL) version 2.1.
 *
 * ez-draw++ is based on ez-draw (see below)
*/
/// @cond Document_C_source
#define EZ_PRIVATE_DEFS 1
/*
 * ez-draw.h: basic EZ-Draw module
 *
 * Edouard.Thiel@lif.univ-mrs.fr - 07/07/2013 - version 1.2
 *
 * This program is free software under the terms of the
 * GNU Lesser General Public License (LGPL) version 2.1.
*/

#ifndef EZ_DRAW__H
#define EZ_DRAW__H

#ifndef _WIN32
#define EZ_BASE_XLIB 1
#else
#define EZ_BASE_WIN32 1
#endif

/* To let vsnprintf be compatible with -ansi */
#if defined __STRICT_ANSI__ && !defined _GNU_SOURCE
#define _GNU_SOURCE
#include <stdio.h>
#undef _GNU_SOURCE
#else
#include <stdio.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#ifdef EZ_BASE_XLIB

#include <sys/time.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <X11/keysym.h>
#include <X11/extensions/Xdbe.h>

#elif defined EZ_BASE_WIN32

#ifdef __MINGW32__  /* MinGW underestimate Windows version */
#  undef WINVER
#  define WINVER 0x0501
#endif

#include <windows.h>
#include <windowsx.h>
#include <wingdi.h>

#endif /* EZ_BASE_ */

/* Miscellaneous constants */
#define EZ_FONT_MAX    16
#define EZ_WIN_MAX   1024

typedef   signed char  Ez_int8;
typedef unsigned char  Ez_uint8;
typedef unsigned short Ez_uint16;
typedef   signed short Ez_int16;
typedef unsigned int   Ez_uint32;
typedef   signed int   Ez_int32;
typedef unsigned int   Ez_uint;

/* Produce a compiler error if size is wrong */
typedef Ez_uint8 Ez_validate_uint32[sizeof (Ez_uint32)==4 ? 1 : -1];

#define EZ_ROUND(x) ((x)<0 ? (int)((x)-0.5) : (int)((x)+0.5))

/* Type Ez_window */
#ifdef EZ_BASE_XLIB
typedef Window Ez_window;
#elif defined EZ_BASE_WIN32
typedef HWND Ez_window;
#endif /* EZ_BASE_ */

/* Colors */
extern Ez_uint32 ez_black, ez_white, ez_grey, ez_red, ez_green, ez_blue,
              ez_yellow, ez_cyan, ez_magenta;

#ifdef EZ_BASE_XLIB
/* Colors handling on X11 */
typedef struct {
    Ez_uint32 mask, shift, length, max;
} Ez_channel;

typedef struct {
    Ez_channel green, red, blue;
} Ez_TrueColor;

typedef struct {
    Colormap colormap;
    Ez_uint32 palette[6][6][6];
    XColor samples[256];
} Ez_PseudoColor;
#endif /* EZ_BASE_ */

/* Timers handling */
#define EZ_TIMER_MAX 100

typedef struct {
    Ez_window win;
    struct timeval expiration;
} Ez_timer;

/* To display text */
typedef enum : int {
    EZ_AA = 183200,
    EZ_TL,  EZ_TC,  EZ_TR,     /* Top Left, Top Center, Top Right */
    EZ_ML,  EZ_MC,  EZ_MR,     /* Middle Left, Middle Center, Middle Right */
    EZ_BL,  EZ_BC,  EZ_BR,     /* Bottom Left, Bottom Center, Bottom Right */
    EZ_BB,
    EZ_TLF, EZ_TCF, EZ_TRF,    /* same, fill background */
    EZ_MLF, EZ_MCF, EZ_MRF,    /* same, fill background */
    EZ_BLF, EZ_BCF, EZ_BRF,    /* same, fill background */
    EZ_CC
} Ez_Align;

/* Convert Xlib types for Win32 */
#ifdef EZ_BASE_WIN32
typedef HDC XdbeBackBuffer;
typedef MSG XEvent;
typedef int KeySym;
typedef LPCTSTR XContext;
typedef POINT XPoint;
#define True  TRUE
#define False FALSE
#define None  NULL
#endif /* EZ_BASE_ */

/* Catchall type used by all functions */
typedef struct {
#ifdef EZ_BASE_XLIB
    Display *display;               /* The display */
    int screen_num;                 /* The screen number */
    GC gc;                          /* Graphical context */
    XdbeBackBuffer dbuf_pix;        /* Current double buffer */
    Ez_window dbuf_win;             /* Current double-buffered window */
    Atom atom_protoc, atom_delwin;  /* To handle windows deletion */
    XFontStruct *font[EZ_FONT_MAX]; /* To store the fonts */
    int depth;                      /* Depth: 8, 15, 16, 24, 32 */
    Visual *visual;                 /* For colors */
    Ez_PseudoColor pseudoColor;     /* Palette indexed on 256 colors */
    Ez_TrueColor   trueColor;       /* RGB channels stored in the pixels */
#elif defined EZ_BASE_WIN32
    HINSTANCE hand_prog;            /* Handle on the program */
    WNDCLASSEX wnd_class;           /* Extended window class */
    HDC hdc;                        /* Drawin context for dc_win */
    Ez_window dc_win;               /* Ez_window associated to hdc */
    HDC dbuf_dc;                    /* The dc of the current double buffer */
    Ez_window dbuf_win;             /* Current double-buffered window */
    int dbuf_w, dbuf_h;             /* Size of double buffer */
    HBITMAP hOldBmp, hMemBmp;       /* Bitmap of window and double buffer */
    HPEN hpen;                      /* Logical pen for color drawing */
    HBRUSH hbrush;                  /* Brush for color filling */
    HFONT font[EZ_FONT_MAX];        /* To store fonts */
    KeySym key_sym;                 /* Key symbol: XK_Space, XK_q, etc */
    const char  *key_name;          /* For printing: "XK_Space", "XK_q", .. */
    const char  *key_string;        /* Corresponding string: " ", "q", etc */
    struct timeval start_time;      /* Initial date */
    LARGE_INTEGER start_count;      /* Counter to compute time */
    double perf_freq;               /* Frequency to compute time */
#endif /* EZ_BASE_ */
    int display_width;              /* Display width */
    int display_height;             /* Display height */
    Ez_window root_win;             /* Root window */
    XContext info_prop;             /* xid or prop to store data in the window */
    int mv_x, mv_y;                 /* Coord to filter mouse moves */
    Ez_window mv_win;               /* Ez_window to filter mouse moves */
    Ez_uint32 color;                /* Current color */
    int thick;                      /* Current thickness */
    int nfont;                      /* Current font number */
    Ez_timer timer_l[EZ_TIMER_MAX]; /* Timers list */
    int timer_nb;                   /* Timers number */
    int main_loop;                  /* Main loop flag */
    int last_expose;                /* Last Expose flag */
    int auto_quit;                  /* Close button flag */
    int mouse_b;                    /* Mouse button pressed */
    Ez_window win_l[EZ_WIN_MAX];    /* Windows list */
    int win_nb;                     /* Windows number */
} Ez_X;

#ifdef EZ_BASE_WIN32
/* Events of X11/X.h for Win32 */
#define  KeyPress           2
#define  KeyRelease         3
#define  ButtonPress        4
#define  ButtonRelease      5
#define  MotionNotify       6
#define  Expose            12
#define  ConfigureNotify   22
#define  ClientMessage     33
#define  LASTEvent         35
/* Timer */
#define  EZ_TIMER1        208
/* Private messages */
enum { EZ_MSG_PAINT = WM_APP+1, EZ_MSG_LAST };
#endif /* EZ_BASE_ */

/* Additional events */
enum { WindowClose = LASTEvent+1, TimerNotify, EzLastEvent };


typedef struct sEz_event {
    int type;                       /* Expose, ButtonPress, etc */
    Ez_window win;                  /* Identifier of the window */
    int mx, my;                     /* Mouse coordinates */
    int mb;                         /* Mouse button number, 0 = None */
    int width, height;              /* Width and height */
    KeySym key_sym;                 /* Key symbol: XK_Space, XK_q, etc */
    char   key_name[80];            /* For printing: "XK_Space", "XK_q", .. */
    char   key_string[80];          /* Corresponding string: " ", "q", etc */
    int    key_count;               /* String length */
    XEvent xev;                     /* Original event */
} Ez_event;

/* Type of a callback */
typedef void (*Ez_func)(Ez_event *ev);

/* Data associated to a window using a xid or a property */
typedef struct {
    Ez_func func;                   /* Callback of window */
    void *data;                     /* User-data associated to window */
    XdbeBackBuffer dbuf;            /* Back-buffer of window */
    int show;                       /* For delayed display */
} Ez_win_info;


/* Public functions */

int ez_init (void) ;
Ez_window ez_window_create (int w, int h, const char *name, Ez_func func);
int ez_window_get_id (Ez_window win);
void ez_window_destroy (Ez_window win);
void ez_window_show (Ez_window win, int val);
void ez_window_set_size (Ez_window win, int w, int h);
void ez_window_get_size (Ez_window win, int *w, int *h);
void ez_window_reset (Ez_window win);
void ez_window_clear (Ez_window win,Ez_uint32 color);
void ez_window_dbuf (Ez_window win, int val);
void ez_set_data (Ez_window win, void *data);
void *ez_get_data (Ez_window win);
void ez_quit (void) ;
void ez_auto_quit (int val);
void ez_send_expose (Ez_window win);
void ez_start_timer (Ez_window win, int delay);
void ez_main_loop (void) ;
int ez_random (int n);
double ez_get_time (void) ;

extern Ez_uint32 (*ez_get_RGB)(Ez_uint8 r, Ez_uint8 g, Ez_uint8 b);
Ez_uint32 ez_get_grey (Ez_uint8 g);
void ez_HSV_to_RGB (double h, double s, double v,
    Ez_uint8 *r, Ez_uint8 *g, Ez_uint8 *b);
Ez_uint32 ez_get_HSV (double h, double s, double v);
void ez_set_color (Ez_uint32 color);
void ez_set_thick (int thick);
void ez_draw_point (Ez_window win, int x1, int y1);
void ez_draw_line (Ez_window win, int x1, int y1, int x2, int y2);
void ez_draw_rectangle (Ez_window win, int x1, int y1, int x2, int y2);
void ez_fill_rectangle (Ez_window win, int x1, int y1, int x2, int y2);
void ez_draw_triangle (Ez_window win, int x1, int y1, int x2, int y2, int x3, int y3);
void ez_fill_triangle (Ez_window win, int x1, int y1, int x2, int y2, int x3, int y3);
void ez_draw_circle (Ez_window win, int x1, int y1, int x2, int y2);
void ez_fill_circle (Ez_window win, int x1, int y1, int x2, int y2);

int ez_font_load (int num, const char *name);
void ez_set_nfont (int num);
void ez_draw_text (Ez_window win, Ez_Align align, int x1, int y1,
    const char *format, ...);


/* Private functions */
#ifdef EZ_PRIVATE_DEFS

int ez_draw_debug (void);

int ez_error_dfl (const char *fmt, va_list ap);
int ez_error_ign (const char *fmt, va_list ap);
extern int (*ez_error_handler) (const char *fmt, va_list ap);
int ez_error (const char *fmt, ...);
int ez_check_state (const char *funcname);

void ez_win_delete (Ez_window win);
void ez_win_delete_all (void) ;
extern int ez_win_delete_final;
void ez_close_disp (void) ;

#ifdef EZ_BASE_WIN32
void ez_init_timeofday (void);
void ez_get_systemtime (struct timeval *t);
#endif /* EZ_BASE_ */

void ez_gettimeofday (struct timeval *t);
int ez_timer_add (Ez_window win, int delay);
int ez_timer_remove (Ez_window win);
struct timeval *ez_timer_delay (void) ;

#ifdef EZ_BASE_XLIB
void ez_event_next (Ez_event *ev);
Bool ez_predicat_expose (Display *display, XEvent *xev, XPointer arg);
int ez_is_last_expose (XEvent *xev);
void ez_event_dispatch (Ez_event *ev);
#elif defined EZ_BASE_WIN32
void ez_msg_next (MSG *msg);
LRESULT CALLBACK ez_win_proc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
int ez_is_modifier (KeySym key_sym);
int ez_is_repetition (LPARAM lParam);
void ez_cur_win (Ez_window win);
void ez_update_pen (void) ;
#endif /* EZ_BASE_ */

void ez_random_init (void) ;

int ez_win_list_find (Ez_window win);
int ez_win_list_insert (Ez_window win);
int ez_win_list_remove (Ez_window win);

int ez_prop_set (Ez_window win, XContext prop, void *value);
int ez_prop_get (Ez_window win, XContext prop, void **value);
int ez_prop_destroy (Ez_window win, XContext prop);

int ez_info_get (Ez_window win, Ez_win_info **info);

int ez_func_set (Ez_window win, Ez_func func);
int ez_func_get (Ez_window win, Ez_func *func);
int ez_func_call (Ez_event *ev);

void ez_dbuf_init (void) ;
int ez_dbuf_set (Ez_window win, XdbeBackBuffer dbuf);
int ez_dbuf_get (Ez_window win, XdbeBackBuffer *dbuf);
void ez_dbuf_preswap (Ez_window win);
void ez_dbuf_swap (Ez_window win);

void ez_font_init (void) ;
void ez_font_delete (void) ;
int ez_color_init (void) ;
#ifdef EZ_BASE_XLIB
void ez_set_palette (Ez_uint32 pixel, int R, int G, int B,
    int max, int inCube);
void ez_init_PseudoColor (void) ;
void ez_init_TrueColor (void) ;
void ez_init_channel (Ez_channel *channel, Ez_uint32 mask);
Ez_uint32 ez_get_RGB_true_color   (Ez_uint8 r, Ez_uint8 g, Ez_uint8 b);
Ez_uint32 ez_get_RGB_pseudo_color (Ez_uint8 r, Ez_uint8 g, Ez_uint8 b);
#elif defined EZ_BASE_WIN32
Ez_uint32 ez_get_RGB_win32 (Ez_uint8 r, Ez_uint8 g, Ez_uint8 b);
int ez_keydown_convert (WPARAM wParam, LPARAM lParam, KeySym *k,const char **n,const char **s);
int ez_keychar_convert (WPARAM wParam, KeySym *k,const char **n,const char **s);
const char *ez_win_msg_name (unsigned int m);
#endif /* EZ_BASE_ */

#endif /* EZ_PRIVATE_DEFS */

/// @endcond Document_C_source
/* LATIN keyboard symbols from X11/keysymdef.h for Win32 */
#ifdef EZ_BASE_WIN32
#define XK_BackSpace                     0xff08  /* Back space, back char */
#define XK_Tab                           0xff09
#define XK_Return                        0xff0d  /* Return, enter */
#define XK_Pause                         0xff13  /* Pause, hold */
#define XK_Scroll_Lock                   0xff14
#define XK_Escape                        0xff1b
#define XK_Delete                        0xffff  /* Delete, rubout */
#define XK_Home                          0xff50
#define XK_Left                          0xff51  /* Move left, left arrow */
#define XK_Up                            0xff52  /* Move up, up arrow */
#define XK_Right                         0xff53  /* Move right, right arrow */
#define XK_Down                          0xff54  /* Move down, down arrow */
#define XK_Prior                         0xff55  /* Prior, previous */
#define XK_Next                          0xff56  /* Next */
#define XK_End                           0xff57  /* EOL */
#define XK_Insert                        0xff63  /* Insert, insert here */
#define XK_Menu                          0xff67
#define XK_Num_Lock                      0xff7f
#define XK_KP_Enter                      0xff8d  /* Enter */
#define XK_KP_Home                       0xff95
#define XK_KP_Left                       0xff96
#define XK_KP_Up                         0xff97
#define XK_KP_Right                      0xff98
#define XK_KP_Down                       0xff99
#define XK_KP_Prior                      0xff9a
#define XK_KP_Next                       0xff9b
#define XK_KP_End                        0xff9c
#define XK_KP_Begin                      0xff9d
#define XK_KP_Equal                      0xffbd  /* Equals */
#define XK_KP_Multiply                   0xffaa
#define XK_KP_Add                        0xffab
#define XK_KP_Separator                  0xffac  /* Separator, often comma */
#define XK_KP_Subtract                   0xffad
#define XK_KP_Divide                     0xffaf
#define XK_KP_0                          0xffb0
#define XK_KP_1                          0xffb1
#define XK_KP_2                          0xffb2
#define XK_KP_3                          0xffb3
#define XK_KP_4                          0xffb4
#define XK_KP_5                          0xffb5
#define XK_KP_6                          0xffb6
#define XK_KP_7                          0xffb7
#define XK_KP_8                          0xffb8
#define XK_KP_9                          0xffb9
#define XK_F1                            0xffbe
#define XK_F2                            0xffbf
#define XK_F3                            0xffc0
#define XK_F4                            0xffc1
#define XK_F5                            0xffc2
#define XK_F6                            0xffc3
#define XK_F7                            0xffc4
#define XK_F8                            0xffc5
#define XK_F9                            0xffc6
#define XK_F10                           0xffc7
#define XK_F11                           0xffc8
#define XK_F12                           0xffc9
#define XK_Shift_L                       0xffe1  /* Left shift */
#define XK_Shift_R                       0xffe2  /* Right shift */
#define XK_Control_L                     0xffe3  /* Left control */
#define XK_Control_R                     0xffe4  /* Right control */
#define XK_Caps_Lock                     0xffe5  /* Caps lock */
#define XK_Meta_L                        0xffe7  /* Left meta */
#define XK_Meta_R                        0xffe8  /* Right meta */
#define XK_Alt_L                         0xffe9  /* Left alt */
#define XK_Alt_R                         0xffea  /* Right alt */
#define XK_space                         0x0020  /* SPACE */
#define XK_exclam                        0x0021  /* EXCLAMATION MARK */
#define XK_quotedbl                      0x0022  /* QUOTATION MARK */
#define XK_numbersign                    0x0023  /* NUMBER SIGN */
#define XK_dollar                        0x0024  /* DOLLAR SIGN */
#define XK_percent                       0x0025  /* PERCENT SIGN */
#define XK_ampersand                     0x0026  /* AMPERSAND */
#define XK_apostrophe                    0x0027  /* APOSTROPHE */
#define XK_parenleft                     0x0028  /* LEFT PARENTHESIS */
#define XK_parenright                    0x0029  /* RIGHT PARENTHESIS */
#define XK_asterisk                      0x002a  /* ASTERISK */
#define XK_plus                          0x002b  /* PLUS SIGN */
#define XK_comma                         0x002c  /* COMMA */
#define XK_minus                         0x002d  /* HYPHEN-MINUS */
#define XK_period                        0x002e  /* FULL STOP */
#define XK_slash                         0x002f  /* SOLIDUS */
#define XK_0                             0x0030  /* DIGIT ZERO */
#define XK_1                             0x0031  /* DIGIT ONE */
#define XK_2                             0x0032  /* DIGIT TWO */
#define XK_3                             0x0033  /* DIGIT THREE */
#define XK_4                             0x0034  /* DIGIT FOUR */
#define XK_5                             0x0035  /* DIGIT FIVE */
#define XK_6                             0x0036  /* DIGIT SIX */
#define XK_7                             0x0037  /* DIGIT SEVEN */
#define XK_8                             0x0038  /* DIGIT EIGHT */
#define XK_9                             0x0039  /* DIGIT NINE */
#define XK_colon                         0x003a  /* COLON */
#define XK_semicolon                     0x003b  /* SEMICOLON */
#define XK_less                          0x003c  /* LESS-THAN SIGN */
#define XK_equal                         0x003d  /* EQUALS SIGN */
#define XK_greater                       0x003e  /* GREATER-THAN SIGN */
#define XK_question                      0x003f  /* QUESTION MARK */
#define XK_at                            0x0040  /* COMMERCIAL AT */
#define XK_A                             0x0041  /* CAPITAL LETTER A */
#define XK_B                             0x0042  /* CAPITAL LETTER B */
#define XK_C                             0x0043  /* CAPITAL LETTER C */
#define XK_D                             0x0044  /* CAPITAL LETTER D */
#define XK_E                             0x0045  /* CAPITAL LETTER E */
#define XK_F                             0x0046  /* CAPITAL LETTER F */
#define XK_G                             0x0047  /* CAPITAL LETTER G */
#define XK_H                             0x0048  /* CAPITAL LETTER H */
#define XK_I                             0x0049  /* CAPITAL LETTER I */
#define XK_J                             0x004a  /* CAPITAL LETTER J */
#define XK_K                             0x004b  /* CAPITAL LETTER K */
#define XK_L                             0x004c  /* CAPITAL LETTER L */
#define XK_M                             0x004d  /* CAPITAL LETTER M */
#define XK_N                             0x004e  /* CAPITAL LETTER N */
#define XK_O                             0x004f  /* CAPITAL LETTER O */
#define XK_P                             0x0050  /* CAPITAL LETTER P */
#define XK_Q                             0x0051  /* CAPITAL LETTER Q */
#define XK_R                             0x0052  /* CAPITAL LETTER R */
#define XK_S                             0x0053  /* CAPITAL LETTER S */
#define XK_T                             0x0054  /* CAPITAL LETTER T */
#define XK_U                             0x0055  /* CAPITAL LETTER U */
#define XK_V                             0x0056  /* CAPITAL LETTER V */
#define XK_W                             0x0057  /* CAPITAL LETTER W */
#define XK_X                             0x0058  /* CAPITAL LETTER X */
#define XK_Y                             0x0059  /* CAPITAL LETTER Y */
#define XK_Z                             0x005a  /* CAPITAL LETTER Z */
#define XK_bracketleft                   0x005b  /* LEFT SQUARE BRACKET */
#define XK_backslash                     0x005c  /* REVERSE SOLIDUS */
#define XK_bracketright                  0x005d  /* RIGHT SQUARE BRACKET */
#define XK_asciicircum                   0x005e  /* CIRCUMFLEX ACCENT */
#define XK_underscore                    0x005f  /* LOW LINE */
#define XK_grave                         0x0060  /* GRAVE ACCENT */
#define XK_a                             0x0061  /* SMALL LETTER A */
#define XK_b                             0x0062  /* SMALL LETTER B */
#define XK_c                             0x0063  /* SMALL LETTER C */
#define XK_d                             0x0064  /* SMALL LETTER D */
#define XK_e                             0x0065  /* SMALL LETTER E */
#define XK_f                             0x0066  /* SMALL LETTER F */
#define XK_g                             0x0067  /* SMALL LETTER G */
#define XK_h                             0x0068  /* SMALL LETTER H */
#define XK_i                             0x0069  /* SMALL LETTER I */
#define XK_j                             0x006a  /* SMALL LETTER J */
#define XK_k                             0x006b  /* SMALL LETTER K */
#define XK_l                             0x006c  /* SMALL LETTER L */
#define XK_m                             0x006d  /* SMALL LETTER M */
#define XK_n                             0x006e  /* SMALL LETTER N */
#define XK_o                             0x006f  /* SMALL LETTER O */
#define XK_p                             0x0070  /* SMALL LETTER P */
#define XK_q                             0x0071  /* SMALL LETTER Q */
#define XK_r                             0x0072  /* SMALL LETTER R */
#define XK_s                             0x0073  /* SMALL LETTER S */
#define XK_t                             0x0074  /* SMALL LETTER T */
#define XK_u                             0x0075  /* SMALL LETTER U */
#define XK_v                             0x0076  /* SMALL LETTER V */
#define XK_w                             0x0077  /* SMALL LETTER W */
#define XK_x                             0x0078  /* SMALL LETTER X */
#define XK_y                             0x0079  /* SMALL LETTER Y */
#define XK_z                             0x007a  /* SMALL LETTER Z */
#define XK_braceleft                     0x007b  /* LEFT CURLY BRACKET */
#define XK_bar                           0x007c  /* VERTICAL LINE */
#define XK_braceright                    0x007d  /* RIGHT CURLY BRACKET */
#define XK_asciitilde                    0x007e  /* TILDE */
#define XK_nobreakspace                  0x00a0  /* NO-BREAK SPACE */
#define XK_exclamdown                    0x00a1  /* INVERTED EXCLAMATION MARK */
#define XK_cent                          0x00a2  /* CENT SIGN */
#define XK_sterling                      0x00a3  /* POUND SIGN */
#define XK_currency                      0x00a4  /* CURRENCY SIGN */
#define XK_yen                           0x00a5  /* YEN SIGN */
#define XK_brokenbar                     0x00a6  /* BROKEN BAR */
#define XK_section                       0x00a7  /* SECTION SIGN */
#define XK_diaeresis                     0x00a8  /* DIAERESIS */
#define XK_copyright                     0x00a9  /* COPYRIGHT SIGN */
#define XK_ordfeminine                   0x00aa  /* FEMININE ORDINAL INDICATOR */
#define XK_guillemotleft                 0x00ab  /* LEFT-POINTING DOUBLE ANGLE QUOTATION MARK */
#define XK_notsign                       0x00ac  /* NOT SIGN */
#define XK_hyphen                        0x00ad  /* SOFT HYPHEN */
#define XK_registered                    0x00ae  /* REGISTERED SIGN */
#define XK_macron                        0x00af  /* MACRON */
#define XK_degree                        0x00b0  /* DEGREE SIGN */
#define XK_plusminus                     0x00b1  /* PLUS-MINUS SIGN */
#define XK_twosuperior                   0x00b2  /* SUPERSCRIPT TWO */
#define XK_threesuperior                 0x00b3  /* SUPERSCRIPT THREE */
#define XK_acute                         0x00b4  /* ACUTE ACCENT */
#define XK_mu                            0x00b5  /* MICRO SIGN */
#define XK_paragraph                     0x00b6  /* PILCROW SIGN */
#define XK_periodcentered                0x00b7  /* MIDDLE DOT */
#define XK_cedilla                       0x00b8  /* CEDILLA */
#define XK_onesuperior                   0x00b9  /* SUPERSCRIPT ONE */
#define XK_masculine                     0x00ba  /* MASCULINE ORDINAL INDICATOR */
#define XK_guillemotright                0x00bb  /* RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK */
#define XK_onequarter                    0x00bc  /* VULGAR FRACTION ONE QUARTER */
#define XK_onehalf                       0x00bd  /* VULGAR FRACTION ONE HALF */
#define XK_threequarters                 0x00be  /* VULGAR FRACTION THREE QUARTERS */
#define XK_questiondown                  0x00bf  /* INVERTED QUESTION MARK */
#define XK_Agrave                        0x00c0  /* CAPITAL LETTER A WITH GRAVE */
#define XK_Aacute                        0x00c1  /* CAPITAL LETTER A WITH ACUTE */
#define XK_Acircumflex                   0x00c2  /* CAPITAL LETTER A WITH CIRCUMFLEX */
#define XK_Atilde                        0x00c3  /* CAPITAL LETTER A WITH TILDE */
#define XK_Adiaeresis                    0x00c4  /* CAPITAL LETTER A WITH DIAERESIS */
#define XK_Aring                         0x00c5  /* CAPITAL LETTER A WITH RING ABOVE */
#define XK_AE                            0x00c6  /* CAPITAL LETTER AE */
#define XK_Ccedilla                      0x00c7  /* CAPITAL LETTER C WITH CEDILLA */
#define XK_Egrave                        0x00c8  /* CAPITAL LETTER E WITH GRAVE */
#define XK_Eacute                        0x00c9  /* CAPITAL LETTER E WITH ACUTE */
#define XK_Ecircumflex                   0x00ca  /* CAPITAL LETTER E WITH CIRCUMFLEX */
#define XK_Ediaeresis                    0x00cb  /* CAPITAL LETTER E WITH DIAERESIS */
#define XK_Igrave                        0x00cc  /* CAPITAL LETTER I WITH GRAVE */
#define XK_Iacute                        0x00cd  /* CAPITAL LETTER I WITH ACUTE */
#define XK_Icircumflex                   0x00ce  /* CAPITAL LETTER I WITH CIRCUMFLEX */
#define XK_Idiaeresis                    0x00cf  /* CAPITAL LETTER I WITH DIAERESIS */
#define XK_ETH                           0x00d0  /* CAPITAL LETTER ETH */
#define XK_Ntilde                        0x00d1  /* CAPITAL LETTER N WITH TILDE */
#define XK_Ograve                        0x00d2  /* CAPITAL LETTER O WITH GRAVE */
#define XK_Oacute                        0x00d3  /* CAPITAL LETTER O WITH ACUTE */
#define XK_Ocircumflex                   0x00d4  /* CAPITAL LETTER O WITH CIRCUMFLEX */
#define XK_Otilde                        0x00d5  /* CAPITAL LETTER O WITH TILDE */
#define XK_Odiaeresis                    0x00d6  /* CAPITAL LETTER O WITH DIAERESIS */
#define XK_multiply                      0x00d7  /* MULTIPLICATION SIGN */
#define XK_Oslash                        0x00d8  /* CAPITAL LETTER O WITH STROKE */
#define XK_Ugrave                        0x00d9  /* CAPITAL LETTER U WITH GRAVE */
#define XK_Uacute                        0x00da  /* CAPITAL LETTER U WITH ACUTE */
#define XK_Ucircumflex                   0x00db  /* CAPITAL LETTER U WITH CIRCUMFLEX */
#define XK_Udiaeresis                    0x00dc  /* CAPITAL LETTER U WITH DIAERESIS */
#define XK_Yacute                        0x00dd  /* CAPITAL LETTER Y WITH ACUTE */
#define XK_THORN                         0x00de  /* CAPITAL LETTER THORN */
#define XK_ssharp                        0x00df  /* SMALL LETTER SHARP S */
#define XK_agrave                        0x00e0  /* SMALL LETTER A WITH GRAVE */
#define XK_aacute                        0x00e1  /* SMALL LETTER A WITH ACUTE */
#define XK_acircumflex                   0x00e2  /* SMALL LETTER A WITH CIRCUMFLEX */
#define XK_atilde                        0x00e3  /* SMALL LETTER A WITH TILDE */
#define XK_adiaeresis                    0x00e4  /* SMALL LETTER A WITH DIAERESIS */
#define XK_aring                         0x00e5  /* SMALL LETTER A WITH RING ABOVE */
#define XK_ae                            0x00e6  /* SMALL LETTER AE */
#define XK_ccedilla                      0x00e7  /* SMALL LETTER C WITH CEDILLA */
#define XK_egrave                        0x00e8  /* SMALL LETTER E WITH GRAVE */
#define XK_eacute                        0x00e9  /* SMALL LETTER E WITH ACUTE */
#define XK_ecircumflex                   0x00ea  /* SMALL LETTER E WITH CIRCUMFLEX */
#define XK_ediaeresis                    0x00eb  /* SMALL LETTER E WITH DIAERESIS */
#define XK_igrave                        0x00ec  /* SMALL LETTER I WITH GRAVE */
#define XK_iacute                        0x00ed  /* SMALL LETTER I WITH ACUTE */
#define XK_icircumflex                   0x00ee  /* SMALL LETTER I WITH CIRCUMFLEX */
#define XK_idiaeresis                    0x00ef  /* SMALL LETTER I WITH DIAERESIS */
#define XK_eth                           0x00f0  /* SMALL LETTER ETH */
#define XK_ntilde                        0x00f1  /* SMALL LETTER N WITH TILDE */
#define XK_ograve                        0x00f2  /* SMALL LETTER O WITH GRAVE */
#define XK_oacute                        0x00f3  /* SMALL LETTER O WITH ACUTE */
#define XK_ocircumflex                   0x00f4  /* SMALL LETTER O WITH CIRCUMFLEX */
#define XK_otilde                        0x00f5  /* SMALL LETTER O WITH TILDE */
#define XK_odiaeresis                    0x00f6  /* SMALL LETTER O WITH DIAERESIS */
#define XK_division                      0x00f7  /* DIVISION SIGN */
#define XK_oslash                        0x00f8  /* SMALL LETTER O WITH STROKE */
#define XK_ugrave                        0x00f9  /* SMALL LETTER U WITH GRAVE */
#define XK_uacute                        0x00fa  /* SMALL LETTER U WITH ACUTE */
#define XK_ucircumflex                   0x00fb  /* SMALL LETTER U WITH CIRCUMFLEX */
#define XK_udiaeresis                    0x00fc  /* SMALL LETTER U WITH DIAERESIS */
#define XK_yacute                        0x00fd  /* SMALL LETTER Y WITH ACUTE */
#define XK_thorn                         0x00fe  /* SMALL LETTER THORN */
#define XK_ydiaeresis                    0x00ff  /* SMALL LETTER Y WITH DIAERESIS */
#define XK_EuroSign                      0x20ac  /* EURO SIGN */
#endif /* EZ_BASE_ */
/// @cond Document_C_source

#endif /* EZ_DRAW__H */
/// @endcond Document_C_source
/// @cond Document_C_source
/*
 * ez-draw.c: basic EZ-Draw module
 *
 * Edouard.Thiel@lif.univ-mrs.fr - 07/07/2013 - version 1.2
 *
 * This program is free software under the terms of the
 * GNU Lesser General Public License (LGPL) version 2.1.
*/

/* To debug, define environment variable EZ_IMAGE_DEBUG */

#define EZ_PRIVATE_DEFS 1
//REMOVED FOR EZ-DRAW++ : #include "ez-draw.h"

/* Contains internal parameters of ez-draw.c */
Ez_X ezx;

/* Allow to know if ez_init and ez_main_loop were called */
enum { EZ_PRE_INIT, EZ_INIT, EZ_POST_INIT, EZ_MAIN_LOOP, EZ_FINAL };
int ez_state = EZ_PRE_INIT;

/* Colors */
Ez_uint32 ez_black, ez_white, ez_grey, ez_red, ez_green, ez_blue,
          ez_yellow, ez_cyan, ez_magenta;


/*---------------------- P U B L I C   I N T E R F A C E --------------------*/

/*
 * Main initialization.
 * Return 0 on succes, -1 on error.
*/

int ez_init (void)
{
    if (ez_state != EZ_PRE_INIT) {
        ez_error ("ez_init: error, called several times\n");
        return -1;
    }
    ez_state = EZ_INIT;

    /* Set to zero all fields of ezx */
    memset (&ezx, 0, sizeof(Ez_X));

#ifdef EZ_BASE_XLIB

    /* Connect to the display which was set in environment variable DISPLAY,
       by default the local display. */
    ezx.display = XOpenDisplay ("");
    if (ezx.display == NULL) {
        ez_error ("ez_init: XOpenDisplay failed for \"%s\"\n",
            XDisplayName (""));
        return -1;
    }

    /* Store display parameters */
    ezx.screen_num = DefaultScreen (ezx.display);
    ezx.display_width = DisplayWidth (ezx.display, ezx.screen_num);
    ezx.display_height = DisplayHeight (ezx.display, ezx.screen_num);
    ezx.root_win = RootWindow (ezx.display, ezx.screen_num);
    ezx.depth = DefaultDepth (ezx.display, ezx.screen_num);

    /* Graphical context gc; suppress events NoExpose and GraphicsExpose */
    ezx.gc = DefaultGC (ezx.display, ezx.screen_num);
    XSetGraphicsExposures(ezx.display, ezx.gc, False);

    /* Create an xid to store data in a window */
    ezx.info_prop = XUniqueContext ();

    /* Atoms for the WM */
    ezx.atom_protoc = XInternAtom (ezx.display, "WM_PROTOCOLS", False);
    ezx.atom_delwin = XInternAtom (ezx.display, "WM_DELETE_WINDOW", False);

#elif defined EZ_BASE_WIN32

    /* Get the program Handle */
    ezx.hand_prog = GetModuleHandle(NULL);

    /* Prepare an extended window class */
    ezx.wnd_class.cbSize        = sizeof(WNDCLASSEX);
    ezx.wnd_class.style         = CS_CLASSDC | CS_HREDRAW | CS_VREDRAW;
    ezx.wnd_class.lpfnWndProc   = ez_win_proc;
    ezx.wnd_class.cbClsExtra    = 0;
    ezx.wnd_class.cbWndExtra    = 0;
    ezx.wnd_class.hInstance     = ezx.hand_prog;
    ezx.wnd_class.hIcon         = LoadIcon (NULL, IDI_APPLICATION);
    ezx.wnd_class.hIconSm       = LoadIcon (NULL, IDI_APPLICATION);
    ezx.wnd_class.hCursor       = LoadCursor (NULL, IDC_ARROW);
    ezx.wnd_class.hbrBackground = (HBRUSH) (COLOR_WINDOW+1);
    ezx.wnd_class.lpszMenuName  = NULL;
    ezx.wnd_class.lpszClassName = "EZ_WND_CLASS";

    /* Store the window class */
    if (!RegisterClassEx (&ezx.wnd_class)) {
        ez_error ("ez_init: RegisterClassEx failed for class \"%s\"\n",
            ezx.wnd_class.lpszClassName);
        return -1;
    }

    /* Store screen parameters */
    ezx.display_width  = GetSystemMetrics(SM_CXSCREEN);
    ezx.display_height = GetSystemMetrics(SM_CYSCREEN);
    ezx.root_win = NULL;

    /* Property to store data in a window */
    ezx.info_prop = "EZ_INFO_PROP";

    /* Events */
    ezx.key_sym = 0;                   /* For KeyRelease */
    ezx.key_name = ezx.key_string = "";

    /* For drawings */
    ezx.dc_win = None;                 /* Current window associated to dc */
    ezx.hpen = NULL;                   /* Pen for color drawing */
    ezx.hbrush = NULL;                 /* Brush for color filling */

    /* Handling of time */
    ez_init_timeofday ();

#endif /* EZ_BASE_ */

    /* Initialize the double buffer for windows displaying */
    ez_dbuf_init ();

    /* Initialize fonts and default font */
    ez_font_init ();
    ez_set_nfont (0);

    /* Initialize colors, default color and thickness */
    ez_color_init ();
    ez_set_color (ez_black);
    ez_set_thick (1);

    /* Timers list */
    ezx.timer_nb = 0;

    /* Configure event loop */
    ezx.main_loop = 1;    /* Set to 0 to break the event loop */
    ezx.last_expose = 1;  /* Set to 0 to deactivate waiting of last Expose */
    ezx.auto_quit = 1;    /* Button Close will exit program */
    ezx.mouse_b = 0;      /* Used for MotionNotify */
    ezx.win_nb = 0;       /* Break also event loop */
    ezx.mv_win = None;    /* To filter mouse moves */

    /* Initialize random numbers generator */
    ez_random_init ();

    /* For safe exit */
    atexit (ez_close_disp);

    ez_state = EZ_POST_INIT;

    return 0;
}


/*
 * Create a main window, having width w and height h, a title name, and a
 * callback func that will be called for each event.
 * Return the window.
*/

Ez_window ez_window_create (int w, int h, const char *name, Ez_func func)
{
    Ez_window win;
    Ez_win_info *info;

    if (ez_check_state ("ez_window_create") < 0) return None;

#ifdef EZ_BASE_XLIB

    /* Create a main window, child of the root window */
    win = XCreateSimpleWindow (ezx.display, ezx.root_win,
        0, 0, w, h, 0, ez_black, ez_white);

   /* Set an event mask */
    XSelectInput (ezx.display, win,
        ExposureMask | KeyPressMask | KeyReleaseMask |
        ButtonPressMask | ButtonReleaseMask | PointerMotionMask |
        StructureNotifyMask);

    /* Set the window title */
    XStoreName (ezx.display, win, name);

    /* Protect windows from destruction */
    XSetWMProtocols (ezx.display, win, &ezx.atom_delwin, 1);

    /* Palette of colors */
    if (ezx.visual->c_class == PseudoColor) {
        XSetWindowAttributes attr;
        attr.colormap = ezx.pseudoColor.colormap;
        XChangeWindowAttributes(ezx.display, win, CWColormap, &attr);
    }

#elif defined EZ_BASE_WIN32

    /* Create a main window = child of NULL on Windows */
    win = CreateWindowEx (
        WS_EX_CLIENTEDGE,
        ezx.wnd_class.lpszClassName,
        name,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        w + 4 + GetSystemMetrics (SM_CXSIZEFRAME)*2,
        h + 4 + GetSystemMetrics (SM_CYSIZEFRAME)*2
              + GetSystemMetrics (SM_CYCAPTION),
        NULL, NULL, ezx.hand_prog, NULL);

    if (win == NULL) {
        ez_error ("ez_window_create: failed for \"%s\"\n", name);
        return NULL;
    }

#endif /* EZ_BASE_ */

    /* To store data in the window */
    info = (Ez_win_info*) malloc (sizeof (Ez_win_info));
    if (info == NULL) {
        ez_error ("ez_window_create: malloc error for \"%s\"\n", name);
#ifdef EZ_BASE_XLIB
        XDestroyWindow (ezx.display, win);
#elif defined EZ_BASE_WIN32
        DestroyWindow (win);
#endif /* EZ_BASE_ */
        return None;
    }

    /* Associate data to the window by using a xid or a property */
    if (ez_prop_set (win, ezx.info_prop, info) < 0) {
        ez_error ("ez_window_create: malloc error for \"%s\"\n", name);
        free (info);
#ifdef EZ_BASE_XLIB
        XDestroyWindow (ezx.display, win);
#elif defined EZ_BASE_WIN32
        DestroyWindow (win);
#endif /* EZ_BASE_ */
        return None;
    }
    info->func = func;
    info->data = NULL;
    info->dbuf = None;
    ez_window_show (win, 1);

    /* Store the window */
    ez_win_list_insert (win);

    if (ez_draw_debug())
        printf ("ez_window_create 0x%x\n", ez_window_get_id(win));

    return win;
}


/*
 * Return the window id as an int.
*/

int ez_window_get_id (Ez_window win)
{
#ifdef EZ_BASE_XLIB
    return win;
#elif defined EZ_BASE_WIN32
    return PtrToInt (win);
#endif /* EZ_BASE_ */
}


/*
 * Destroy window win and all associated stuff.
*/

void ez_window_destroy (Ez_window win)
{
    if (ez_state < EZ_POST_INIT) {
        ez_error ("ez_window_destroy: error, called before ez_init\n");
        return;
    }

    if (ez_win_list_remove (win) < 0) return;

    ez_win_delete (win);
}


/*
 * Show (val = 1) or hide (val = 0) a window.
*/

void ez_window_show (Ez_window win, int val)
{
    if (ez_check_state ("ez_window_show") < 0) return;

    /* If not in main_loop, delay display */
    if (ez_state < EZ_MAIN_LOOP) {
        Ez_win_info *info;
        if (ez_info_get (win, &info) < 0) return;
        info->show = val;
        return;
    }

#ifdef EZ_BASE_XLIB
    if (val) {
        XMapRaised (ezx.display, win);
    } else {
        XUnmapWindow (ezx.display, win);
    }
#elif defined EZ_BASE_WIN32
    if (val) {
        ShowWindow (win, SW_SHOWNORMAL);
        PostMessage (win, WM_PAINT, 0, 0);
    } else {
        ShowWindow (win, SW_HIDE);
    }
#endif /* EZ_BASE_ */
}


/*
 * Change or retrieve the window size.
*/

void ez_window_set_size (Ez_window win, int w, int h)
{
#ifdef EZ_BASE_XLIB

 XResizeWindow(ezx.display, win, w, h);

#elif defined EZ_BASE_WIN32

    if (w < 1) w = 1;
    if (h < 1) h = 1;
    SetWindowPos (win, HWND_TOP, 0, 0,
        w + 4 + GetSystemMetrics (SM_CXSIZEFRAME)*2,
        h + 4 + GetSystemMetrics (SM_CYSIZEFRAME)*2
              + GetSystemMetrics (SM_CYCAPTION),
        SWP_NOMOVE | SWP_NOZORDER );

#endif /* EZ_BASE_ */
}

void ez_window_get_size (Ez_window win, int *w, int *h)
{
#ifdef EZ_BASE_XLIB

    Ez_window root_ret;
    unsigned int w_ret, h_ret, b_ret, d_ret;
    int x_ret, y_ret;

    XGetGeometry (ezx.display, win, &root_ret, &x_ret, &y_ret,
        &w_ret, &h_ret, &b_ret, &d_ret);

#elif defined EZ_BASE_WIN32

    int w_ret, h_ret;
    RECT rect;

    GetWindowRect (win, &rect);

    w_ret = rect.right - rect.left - 4
            - GetSystemMetrics (SM_CXSIZEFRAME)*2;
    if (w_ret < 0) w_ret = 0;
    h_ret = rect.bottom - rect.top - 4
            - GetSystemMetrics (SM_CYSIZEFRAME)*2
            - GetSystemMetrics (SM_CYCAPTION);
    if (h_ret < 0) h_ret = 0;

#endif /* EZ_BASE_ */

    if (w) *w = w_ret;
    if (h) *h = h_ret;
}


/*
 * Clear a window and initialize again drawings parameters.
*/

void ez_window_reset (Ez_window win)
{
    int w, h;

    ez_window_get_size (win, &w, &h);
    ez_set_color (ez_white);
    ez_fill_rectangle (win, 0, 0, w, h);
    ez_set_color (ez_black);
    ez_set_thick (1);
    ez_set_nfont (0);
}

/*
 * Clear a window with the given color and initialize again drawings parameters.
*/

void ez_window_clear (Ez_window win,Ez_uint32 color)
{
    int w, h;

    ez_window_get_size (win, &w, &h);
    ez_set_color (color);
    ez_fill_rectangle (win, 0, 0, w, h);
    ez_set_color (ez_black);
    ez_set_thick (1);
    ez_set_nfont (0);
}



/*
 * Activate or unactivate the double buffer for a window.
*/

void ez_window_dbuf (Ez_window win, int val)
{
    XdbeBackBuffer dbuf;

    if (ez_dbuf_get (win, &dbuf) < 0) return;

    if (val) {

        if (dbuf != None) return;
#ifdef EZ_BASE_XLIB
        dbuf = XdbeAllocateBackBufferName (ezx.display, win, XdbeUndefined);
#elif defined EZ_BASE_WIN32
        ez_cur_win (win);
        dbuf = CreateCompatibleDC (ezx.hdc);
#endif /* EZ_BASE_ */
        ez_dbuf_set (win, dbuf);

    } else {

        if (dbuf == None) return;
#ifdef EZ_BASE_XLIB
        XdbeDeallocateBackBufferName (ezx.display, dbuf);
#elif defined EZ_BASE_WIN32
        ez_cur_win (None);
        DeleteDC (dbuf);
#endif /* EZ_BASE_ */
        ez_dbuf_set (win, None);
    }
}


/*
 * Associate a client data to a window.
*/

void ez_set_data (Ez_window win, void *data)
{
    Ez_win_info *info;
    if (ez_info_get (win, &info) < 0) return;
    info->data = data;
}


/*
 * Retrieve the client data associated to a window.
*/

void *ez_get_data (Ez_window win)
{
    Ez_win_info *info;
    if (ez_info_get (win, &info) < 0) return NULL;
    return info->data;
}


/*
 * Break ez_main_loop().
*/

void ez_quit (void)
{
    if (ez_check_state ("ez_quit") < 0) return;

#ifdef EZ_BASE_WIN32
    PostQuitMessage (0);
#endif /* EZ_BASE_ */

    ezx.main_loop = 0;
}


/*
 * Determine the behavior when the user click on the Close button :
 *   val = 1  ==> the program will end (default).
 *   val = 0  ==> the program will receive a WindowClose event.
*/

void ez_auto_quit (int val)
{
    ezx.auto_quit = val;
}


/*
 * Send an Expose event to the window, to force redraw.
*/

void ez_send_expose (Ez_window win)
{
#ifdef EZ_BASE_XLIB

    XEvent ev;

    ev.type = Expose;
    ev.xexpose.window = win;
    ev.xexpose.count = 0;

    XSendEvent (ezx.display, win, False, 0L, &ev);

#elif defined EZ_BASE_WIN32

    PostMessage (win, EZ_MSG_PAINT, 0, 0);

#endif /* EZ_BASE_ */
}


/*
 * Start a timer for the window win with the delay expressed in millisecs.
 * Any recall before timer expiration will cancel and replace the timer with
 * the new delay. Moreover, if delay is -1 then the timer is deleted.
*/

void ez_start_timer (Ez_window win, int delay)
{
    if (delay < 0) {
        ez_timer_remove (win);
    } else {
        if (ez_timer_add (win, delay) < 0)
            ez_error ("ez_start_timer: could not set timer delay"
                " = %d ms for win 0x%x\n", delay, ez_window_get_id(win));
    }
}


/*
 * Main loop. To break, just call ez_quit().
 * This function displays the windows, then wait for events and dispatch them
 * by calling the corresponding callbacks.
 * Once returned from ez_main_loop(), no more graphic call should be done.
*/

void ez_main_loop (void)
{
    int i;
#ifdef EZ_BASE_XLIB
    Ez_event ev;
#elif defined EZ_BASE_WIN32
    MSG msg;
#endif /* EZ_BASE_ */

    if (ez_state == EZ_PRE_INIT) {
        ez_error ("ez_main_loop: error, ez_init must be called first\n");
        return;
    }
     if (ez_state >= EZ_MAIN_LOOP) {
        ez_error ("ez_main_loop: error, called several times\n");
        return;
    }
    ez_state = EZ_MAIN_LOOP;

    /* Display all windows whose displaying was delayed */
    for (i = 0; i < ezx.win_nb; i++) {
        Ez_win_info *info;
        if (ez_info_get (ezx.win_l[i], &info) < 0) continue;
        if (info->show) ez_window_show (ezx.win_l[i], 1);
    }

    /* Wait for next event then call the callback */
    while (ezx.main_loop != 0 && ezx.win_nb > 0) {
#ifdef EZ_BASE_XLIB
        ez_event_next (&ev);
        ez_event_dispatch (&ev);
#elif defined EZ_BASE_WIN32
        ez_msg_next (&msg);
        DispatchMessage (&msg);
#endif /* EZ_BASE_ */
    }

    ez_state = EZ_FINAL;
}


/*
 * Return a random number between 0 and n-1
*/

int ez_random (int n)
{
#ifdef EZ_BASE_XLIB
    return (random() / (RAND_MAX + 1.0)) * n;
#elif defined EZ_BASE_WIN32
    return (rand() / (RAND_MAX + 1.0)) * n;
#endif /* EZ_BASE_ */
}


/*
 * Return the elapsed time since the Epoch in seconds,microseconds
*/

double ez_get_time (void)
{
    struct timeval tp;
    ez_gettimeofday (&tp);
    return (double) tp.tv_sec + (double) tp.tv_usec / (double) 1E6;
}


/*
 * Return a color computed according to the levels r,g,b between 0 and 255.
*/

Ez_uint32 (*ez_get_RGB)(Ez_uint8 r, Ez_uint8 g, Ez_uint8 b) =
#ifdef EZ_BASE_XLIB
    ez_get_RGB_true_color;
#elif defined EZ_BASE_WIN32
    ez_get_RGB_win32;
#endif /* EZ_BASE_ */


/*
 * Return a grey color computed according to the level g between 0 and 255.
*/

Ez_uint32 ez_get_grey (Ez_uint8 g)
{
    return ez_get_RGB (g, g, g);
}


/*
 * Convert a color from HSV to RGB ; h in [0..360], s,v in [0..1]
 * see http://en.wikipedia.org/wiki/Hue/saturation/value
*/

void ez_HSV_to_RGB (double h, double s, double v,
    Ez_uint8 *r, Ez_uint8 *g, Ez_uint8 *b)
{
    int    ti = ((int) h / 60) % 6;
    double f = h / 60 - ti;
    v *= 255;
    switch (ti) {
        case 0 : *r = v; *b = v*(1-s); *g = v*(1-(1-f)*s); break;
        case 1 : *g = v; *b = v*(1-s); *r = v*(1-   f *s); break;
        case 2 : *g = v; *r = v*(1-s); *b = v*(1-(1-f)*s); break;
        case 3 : *b = v; *r = v*(1-s); *g = v*(1   -f *s); break;
        case 4 : *b = v; *g = v*(1-s); *r = v*(1-(1-f)*s); break;
        case 5 : *r = v; *g = v*(1-s); *b = v*(1-   f *s); break;
        default : *r = *g = *b = 0;
    }
}


/*
 * Return a color defined in space Hue, Saturation, Value.
 * h in [0..360], s,v in [0..1]
*/

Ez_uint32 ez_get_HSV (double h, double s, double v)
{
    Ez_uint8 r, g, b;

    ez_HSV_to_RGB (h, s, v, &r, &g, &b);
    return ez_get_RGB (r, g, b);
}


/*
 * Store the color for the next drawings, as well as for text displaying.
*/

void ez_set_color (Ez_uint32 color)
{
    ezx.color = color;

#ifdef EZ_BASE_XLIB

    XSetForeground (ezx.display, ezx.gc, ezx.color);

#elif defined EZ_BASE_WIN32

    /* Pen for drawings */
    ez_update_pen ();

    /* Brush for collor filling */
    if (ezx.hbrush != NULL) DeleteObject (ezx.hbrush);
    ezx.hbrush = CreateSolidBrush (ezx.color);
    if (ezx.dc_win != None) SelectObject (ezx.hdc, ezx.hbrush);

    /* To display text */
    if (ezx.dc_win != None) SetTextColor (ezx.hdc, ezx.color);

#endif /* EZ_BASE_ */
}


/*
 * Set the thickness for next drawings.
 * relates : ez_draw_point, ez_draw_line, ez_draw_rectangle, ez_draw_triangle,
 *           ez_draw_circle.
*/

void ez_set_thick (int thick)
{
    ezx.thick = (thick <= 0) ? 1 : thick;

#ifdef EZ_BASE_XLIB
    XSetLineAttributes (ezx.display, ezx.gc, (ezx.thick == 1) ? 0 : ezx.thick,
        LineSolid, CapRound, JoinRound);
#elif defined EZ_BASE_WIN32
    ez_update_pen ();
#endif /* EZ_BASE_ */
}


/*
 * Basic drawings. x1,y1 and y2,y2 are the top left and bottom right
 * coordinates of the bounding box.
*/

#define EZ_MIN(x,y) ((x)<(y)?(x):(y))
#define EZ_MAX(x,y) ((x)>(y)?(x):(y))

void ez_draw_point (Ez_window win, int x1, int y1)
{
#ifdef EZ_BASE_XLIB
    if (win == ezx.dbuf_win) win = ezx.dbuf_pix;
    if (ezx.thick == 1)
         XDrawPoint (ezx.display, win, ezx.gc, x1, y1);
    else XFillArc (ezx.display, win, ezx.gc, x1-ezx.thick/2, y1-ezx.thick/2,
                   ezx.thick+1, ezx.thick+1, 0, 360*64);
#elif defined EZ_BASE_WIN32
    ez_cur_win (win);
    MoveToEx (ezx.hdc, x1, y1, NULL);
    LineTo (ezx.hdc, x1+1, y1);  /* final point excluded */
#endif /* EZ_BASE_ */
}

void ez_draw_line (Ez_window win, int x1, int y1, int x2, int y2)
{
#ifdef EZ_BASE_XLIB
    if (win == ezx.dbuf_win) win = ezx.dbuf_pix;
    XDrawLine (ezx.display, win, ezx.gc, x1, y1, x2, y2);
#elif defined EZ_BASE_WIN32
    ez_cur_win (win);
    MoveToEx (ezx.hdc, x1, y1, NULL);
    LineTo (ezx.hdc, x2, y2);
    if (ezx.thick == 1) LineTo (ezx.hdc, x2+1, y2);   /* final point excluded */
#endif /* EZ_BASE_ */
}

void ez_draw_rectangle (Ez_window win, int x1, int y1, int x2, int y2)
{
#ifdef EZ_BASE_XLIB
    if (win == ezx.dbuf_win) win = ezx.dbuf_pix;
    XDrawRectangle (ezx.display, win, ezx.gc,
        EZ_MIN(x1,x2), EZ_MIN(y1,y2), abs(x2-x1), abs(y2-y1));
#elif defined EZ_BASE_WIN32
    ez_cur_win (win);
    MoveToEx (ezx.hdc, x1, y1, NULL);
    LineTo (ezx.hdc, x2, y1);
    LineTo (ezx.hdc, x2, y2);
    LineTo (ezx.hdc, x1, y2);
    LineTo (ezx.hdc, x1, y1);
#endif /* EZ_BASE_ */
}

void ez_fill_rectangle (Ez_window win, int x1, int y1, int x2, int y2)
{
#ifdef EZ_BASE_XLIB
    if (win == ezx.dbuf_win) win = ezx.dbuf_pix;
    XFillRectangle (ezx.display, win, ezx.gc,
        EZ_MIN(x1,x2), EZ_MIN(y1,y2), abs(x2-x1)+1, abs(y2-y1)+1);
#elif defined EZ_BASE_WIN32
    int old_thick = ezx.thick;
    ez_cur_win (win);
    if (ezx.thick != 1) ez_set_thick (1);
    Rectangle (ezx.hdc, EZ_MIN(x1,x2)  , EZ_MIN(y1,y2)   ,
                        EZ_MAX(x1,x2)+1, EZ_MAX(y1,y2)+1 );
    if (ezx.thick != old_thick) ez_set_thick (old_thick);
#endif /* EZ_BASE_ */
}

void ez_draw_triangle (Ez_window win, int x1, int y1, int x2, int y2, int x3, int y3)
{
#ifdef EZ_BASE_XLIB
    if (win == ezx.dbuf_win) win = ezx.dbuf_pix;
    XDrawLine (ezx.display, win, ezx.gc, x1, y1, x2, y2);
    XDrawLine (ezx.display, win, ezx.gc, x2, y2, x3, y3);
    XDrawLine (ezx.display, win, ezx.gc, x3, y3, x1, y1);
#elif defined EZ_BASE_WIN32
    ez_cur_win (win);
    MoveToEx (ezx.hdc, x1, y1, NULL);
    LineTo (ezx.hdc, x2, y2);
    LineTo (ezx.hdc, x3, y3);
    LineTo (ezx.hdc, x1, y1);
#endif /* EZ_BASE_ */
}

void ez_fill_triangle (Ez_window win, int x1, int y1, int x2, int y2, int x3, int y3)
{
#ifdef EZ_BASE_XLIB
    XPoint points[3];
    points[0].x = x1; points[1].x = x2; points[2].x = x3;
    points[0].y = y1; points[1].y = y2; points[2].y = y3;
    if (win == ezx.dbuf_win) win = ezx.dbuf_pix;
    XFillPolygon (ezx.display, win, ezx.gc, points, 3, Convex, CoordModeOrigin);
#elif defined EZ_BASE_WIN32
    POINT points[3];
    int old_thick = ezx.thick;
    points[0].x = x1; points[1].x = x2; points[2].x = x3;
    points[0].y = y1; points[1].y = y2; points[2].y = y3;
    ez_cur_win (win);
    if (ezx.thick != 1) ez_set_thick (1);
    Polygon (ezx.hdc, points, 3 );
    if (ezx.thick != old_thick) ez_set_thick (old_thick);
#endif /* EZ_BASE_ */
}

void ez_draw_circle (Ez_window win, int x1, int y1, int x2, int y2)
{
#ifdef EZ_BASE_XLIB
    if (win == ezx.dbuf_win) win = ezx.dbuf_pix;
    XDrawArc (ezx.display, win, ezx.gc,
        EZ_MIN(x1,x2), EZ_MIN(y1,y2), abs(x2-x1), abs(y2-y1), 0, 360*64);
#elif defined EZ_BASE_WIN32
    int xa = EZ_MIN(x1,x2), ya = EZ_MIN(y1,y2),
        xb = EZ_MAX(x1,x2), yb = EZ_MAX(y1,y2),
        xc = (xa+xb)/2;
    ez_cur_win (win);
    Arc (ezx.hdc, xa, ya, xb, yb, xc, ya, xc, ya);
#endif /* EZ_BASE_ */
}

void ez_fill_circle (Ez_window win, int x1, int y1, int x2, int y2)
{
#ifdef EZ_BASE_XLIB
    if (win == ezx.dbuf_win) win = ezx.dbuf_pix;
    XFillArc (ezx.display, win, ezx.gc,
        EZ_MIN(x1,x2), EZ_MIN(y1,y2), abs(x2-x1)+1, abs(y2-y1)+1, 0, 360*64);
#elif defined EZ_BASE_WIN32
    int old_thick = ezx.thick;
    ez_cur_win (win);
    if (ezx.thick != 1) ez_set_thick (1);
    Ellipse (ezx.hdc, EZ_MIN(x1,x2)  , EZ_MIN(y1,y2)   ,
                      EZ_MAX(x1,x2)+1, EZ_MAX(y1,y2)+1 );
    if (ezx.thick != old_thick) ez_set_thick (old_thick);
#endif /* EZ_BASE_ */
}


/*
 * Load a font from its name (e.g. "6x13") and store it in ezx.font[num].
 * Return 0 on succes, -1 on error.
 *
 * On X11, the name can be in any fashion but must correspond to an existing
 * font. On Windows, the name must be in the form widthxheight; a matching font
 * of fixed size is obtained.
*/

int ez_font_load (int num, const char *name)
{
#ifdef EZ_BASE_WIN32
    int w, h;
#endif /* EZ_BASE_ */

    if (num < 0 || num >= EZ_FONT_MAX) {
        ez_error ("ez_font_load: bad num\n");
        return -1;
    }

#ifdef EZ_BASE_XLIB

    ezx.font[num] = XLoadQueryFont (ezx.display, name);

#elif defined EZ_BASE_WIN32

    if (sscanf (name, "%dx%d", &w, &h) != 2) {
        ez_error ("ez_font_load: could not get wxh in \"%s\"\n", name);
        return -1;
    }

    ezx.font[num] = CreateFont (
        -h, -w, 0, 0,               /* nHeight,nWidth,nEscapement,nOrientation */
        h <= 18 ? 500 : 600,        /* fnWeight */
        FALSE, FALSE, FALSE,        /* fdwItalic, fdwUnderline, fdwStrikeOut */
        ANSI_CHARSET,               /* fdwCharset. DEFAULT_CHARSET ? */
        OUT_DEFAULT_PRECIS,         /* fdwOutputPrecision */
        CLIP_DEFAULT_PRECIS,        /* fdwClipPrecision */
        DEFAULT_QUALITY,            /* fdwQuality */
        FIXED_PITCH | FF_MODERN,    /* fdwPitchAndFamily */
        ""                          /* lpszFace */
    );

#endif /* EZ_BASE_ */

    if (ezx.font[num] == NULL)  {
        ez_error ("ez_font_load: could not load font \"%s\"\n", name);
        return -1;
    }
    return 0;
}


/*
 * Set the font number num for the next text drawings.
*/

void ez_set_nfont (int num)
{
    if (num < 0 || num >= EZ_FONT_MAX || ezx.font[num] == NULL) {
        ez_error ("ez_set_nfont: bad num\n");
        return;
    }
    ezx.nfont = num;

#ifdef EZ_BASE_XLIB
    XSetFont (ezx.display, ezx.gc, ezx.font[ezx.nfont]->fid);
#elif defined EZ_BASE_WIN32
    if (ezx.dc_win != None) SelectObject (ezx.hdc, ezx.font[ezx.nfont]);
#endif /* EZ_BASE_ */
}


/*
 * Display text; same usage as printf.
 *
 * Example : ez_draw_text (win, EZ_TL, x, y,
 *                         "Width = %d\nHeight = %d", w, h);
 *
 * The coordinates x1,y1 are relative to align, see Ez_Align definition.
 * align also specifies if the background is filled or not.
 *
 * By default:
 *  - the text is displayed with the font number 0 ("6x13") ;
 *    can be changed with ez_set_nfont().
 *  - text is displayed in black; can be changed with ez_set_color().
*/

void ez_draw_text (Ez_window win, Ez_Align align, int x1, int y1,
    const char *format, ...)
{
    int valign, halign, fillbg;
    va_list ap;
    char buf[16384];
    int i, j, k, n, x, y, a, b, c;

#ifdef EZ_BASE_XLIB
    XFontStruct *font = ezx.font[ezx.nfont];
#elif defined EZ_BASE_WIN32
    TEXTMETRIC text_metric;
#endif /* EZ_BASE_ */

    if (align <= EZ_AA || align == EZ_BB || align >= EZ_CC)
      { ez_error ("ez_draw_text: bad align\n"); return; }

    /* Decode align */
    fillbg = 0;
#ifdef __cplusplus
    if (align > EZ_BB) { fillbg = 1; align = static_cast<Ez_Align>(static_cast<int>(align) - 10); }
    align = static_cast<Ez_Align>(static_cast<int>(align) - static_cast<int>(EZ_AA) - 1);
#else
    if (align > EZ_BB) { fillbg = 1; align -= 10; }
    align -= EZ_AA + 1;
#endif
    halign = align % 3;
    valign = align / 3;

    /* Print the formated string in buf */
    va_start (ap, format);
    vsnprintf (buf, sizeof(buf)-1, format, ap);
    va_end (ap);
    buf[sizeof(buf)-1] = 0;
    if (buf[0] == 0) return;

    /* Count the number of lines */
    for (i = j = k = 0; ; i++)
    if (buf[i] == '\n' || buf[i] == 0) {
        k++; j = i+1;
        if (buf[i] == 0) break;
    }
    n = k;

#ifdef EZ_BASE_XLIB

    if (win == ezx.dbuf_win) win = ezx.dbuf_pix;
    a = font->ascent; b = font->descent; c = a+b+b;

    /* Display line by line */
    for (i = j = k = 0; ; i++)
    if (buf[i] == '\n' || buf[i] == 0) {
        x = x1 - XTextWidth (font, buf+j, i-j) * halign/2;
        y = y1 + a + c*k - (c*n-b) * valign/2;
        if (fillbg == 0)
             XDrawString      (ezx.display, win, ezx.gc, x, y, buf+j, i-j);
        else XDrawImageString (ezx.display, win, ezx.gc, x, y, buf+j, i-j);
        k++; j = i+1;
        if (buf[i] == 0) break;
    }

#elif defined EZ_BASE_WIN32

    ez_cur_win (win);
    GetTextMetrics (ezx.hdc, &text_metric);
    a = text_metric.tmAscent; b = text_metric.tmDescent; c = a+b;

    if (fillbg == 0) SetBkMode (ezx.hdc, TRANSPARENT);

    /* Display line by line */
    for (i = j = k = 0; ; i++)
    if (buf[i] == '\n' || buf[i] == 0) {
        x = x1 - (i-j)*text_metric.tmAveCharWidth * halign/2;
        y = y1 + c*k - (c*n-b) * valign/2 -2;
        TextOut (ezx.hdc, x, y, buf+j, i-j);
        k++; j = i+1;
        if (buf[i] == 0) break;
    }

    /* Restore background drawing mode */
    if (fillbg == 0) SetBkMode (ezx.hdc, OPAQUE);

#endif /* EZ_BASE_ */
}


/*-------------------- P R I V A T E   F U N C T I O N S --------------------*/

/*
 * Unique test of the definition of the environment variable EZ_DRAW_DEBUG
*/

int ez_draw_debug (void)
{
    static int debug = -1;
    if (debug < 0) debug = getenv ("EZ_DRAW_DEBUG") != NULL;
    return debug;
}


/*
 * Print errors.
 *
 * By default, ez_error prints on stderr. We can suppress error messages
 * by making: ez_error_handler = ez_error_ign
 * or intercept messages by changing ez_error_handler.
*/

int ez_error_dfl (const char *fmt, va_list ap)
{
    return vfprintf (stderr, fmt, ap);
}

int ez_error_ign (const char *fmt, va_list ap)
{
    (void) fmt; (void) ap;
    return 0;
}

int (*ez_error_handler) (const char *fmt, va_list ap) = ez_error_dfl;

int ez_error (const char *fmt, ...)
{
    va_list ap;
    int r;

    va_start (ap, fmt);
    r = (*ez_error_handler) (fmt, ap);
    va_end (ap);

    return r;
}


/*
 * Test if ez_state is EZ_POST_INIT or EZ_MAIN_LOOP
 * Return 0 on success, -1 on error. Verbose.
*/

int ez_check_state (const char *funcname)
{
    if (ez_state < EZ_POST_INIT) {
        ez_error ("%s: error, called before ez_init\n", funcname);
        return -1;
    }
    if (ez_state > EZ_MAIN_LOOP) {
        ez_error ("%s: error, called after ez_main_loop\n", funcname);
        return -1;
    }
    return 0;
}


/*
 * Delete a window (without suppressing it from the list).
 * Called by ez_window_destroy and ez_win_delete_all.
*/

void ez_win_delete (Ez_window win)
{
    Ez_win_info *info;

    if (ez_draw_debug())
        printf ("ez_win_delete 0x%x\n", ez_window_get_id(win));

    ez_window_dbuf (win, 0);
    ez_timer_remove (win);

    /* Destroy data _after_ ez_window_dbuf (which still uses them) */
    if (ez_info_get (win, &info) == 0) {
        free (info);
        ez_prop_destroy (win, ezx.info_prop);
    }

#ifdef EZ_BASE_XLIB

    XDestroyWindow (ezx.display, win);

#elif defined EZ_BASE_WIN32

    if (win == ezx.dc_win) ez_cur_win (None);
    if (win == ezx.mv_win) ezx.mv_win = None;
    DestroyWindow (win);

#endif /* EZ_BASE_ */
}


/* Delete remaining windows. Called at the end of the program by
 * ez_close_disp if ez_win_delete_final == 1.
*/

int ez_win_delete_final = 1;

void ez_win_delete_all (void)
{
    int i;

    for (i = 0; i < ezx.win_nb; i++)
        ez_win_delete (ezx.win_l[i]);
    ezx.win_nb = 0;
}


/*
 * Called after exit (atexit function set by ez_init)
*/

void ez_close_disp (void)
{
    if (ez_win_delete_final) ez_win_delete_all ();

    ez_font_delete ();

#ifdef EZ_BASE_XLIB
    if (ezx.visual->c_class == PseudoColor)
        XFreeColormap (ezx.display, ezx.pseudoColor.colormap);

    /* Close the display; from now on, do not call functions using it. */
    XCloseDisplay (ezx.display); ezx.display = NULL;
#endif /* EZ_BASE_ */
}


#ifdef EZ_BASE_WIN32

/*
 * Initialize time computation.
*/

void ez_init_timeofday (void)
{
    LARGE_INTEGER freq;

    ezx.perf_freq = 0;
    ez_get_systemtime (&ezx.start_time);
    QueryPerformanceCounter (&ezx.start_count);

    if (QueryPerformanceFrequency (&freq) && freq.QuadPart > 0)
         ezx.perf_freq = 1.0 / freq.QuadPart;
    if (ez_draw_debug())
        printf ("ez_init_timeofday: perf_freq = %.10f\n", ezx.perf_freq);
}


/*
 * Emulate gettimeofday() with GetSystemTimeAsFileTime().
 * GetSystemTimeAsFileTime() has an accuracy of 15ms (10ms on XP).
 * GetSystemTimePreciseAsFileTime() has an accuracy of 1ms but needs
 * windows >= 8, and seems rather expensive.
*/

void ez_get_systemtime (struct timeval *t)
{
    ULARGE_INTEGER ul;
    FILETIME ft;
    double dt;
    if (t == NULL) return;
    GetSystemTimeAsFileTime (&ft);
    ul.LowPart = ft.dwLowDateTime;
    ul.HighPart = ft.dwHighDateTime;
    dt = ul.QuadPart / 10000000.0 - 11644473600.0;
    t->tv_sec = dt;
    t->tv_usec = (dt - t->tv_sec) * 1000000.0;
}


/*
 * Emulate gettimeofday() with QueryPerformanceCounter().
 * This method seems to be more accurate than ez_get_systemtime()
 * but leads to a drift.
*/

void ez_get_countertime (struct timeval *t)
{
    LARGE_INTEGER now;
    double dt;
    QueryPerformanceCounter (&now);
    dt = (now.QuadPart - ezx.start_count.QuadPart) * ezx.perf_freq;
    *t = ezx.start_time;
    t->tv_sec += dt;
    t->tv_usec += (dt - (int)dt) * 1000000;
    t->tv_sec += t->tv_usec / 1000000;
    t->tv_usec %= 1000000;
}

#endif /* EZ_BASE_ */


/*
 * Compute elapsed time since the Epoch.
*/

void ez_gettimeofday (struct timeval *t)
{
    if (t == NULL) return;
#ifdef EZ_BASE_XLIB
    gettimeofday (t, NULL);
#elif defined EZ_BASE_WIN32
    if (ezx.perf_freq == 0)
         ez_get_systemtime (t);
    else ez_get_countertime (t);
#endif /* EZ_BASE_ */
}


/*
 * Insert a timer in the list. Return 0 on success, -1 on error.
*/

int ez_timer_add (Ez_window win, int delay)
{
    int m1, m2, mid;
    struct timeval t, *mt;

    ez_timer_remove (win);

    if (ezx.timer_nb >= EZ_TIMER_MAX) {
        ez_error ("ez_timer_add: too many timers\n");
        return -1;
    }

    /* Retrieve current date */
    ez_gettimeofday (&t);

    /* Compute expiration date */
    t.tv_usec += delay * 1000;   /* delay in milliseconds */
    if (t.tv_usec > 1000000) {
        t.tv_sec += t.tv_usec / 1000000;
        t.tv_usec %= 1000000;
    }

    /* Dichotomic search */
    m1 = 0; m2 = ezx.timer_nb;
    while (m2-m1 > 0) {
        mid = (m1+m2) / 2;    /* Middle s.t. m1 <= mid < m2 */
        mt = &ezx.timer_l[mid].expiration;
        if ( mt->tv_sec < t.tv_sec ||
            (mt->tv_sec == t.tv_sec && mt->tv_usec < t.tv_usec) )
             m1 = mid+1;   /* t goes in [mid+1 .. m2] */
        else m2 = mid;     /* t goes in [m1 .. mid]   */
    }

    /* Insert in position m1 */
    if (m1 < ezx.timer_nb)
        memmove ( ezx.timer_l+m1+1, ezx.timer_l+m1,
                 (ezx.timer_nb-m1)*sizeof(Ez_timer) );
    ezx.timer_nb++;
    ezx.timer_l[m1].win = win;
    ezx.timer_l[m1].expiration = t;

    return 0;
}


/*
 * Suppress a timer from the list. Return 0 on success, -1 on error.
*/

int ez_timer_remove (Ez_window win)
{
    int i;
    if (win == None) return 0;
    for (i = 0; i < ezx.timer_nb; i++)
        if (ezx.timer_l[i].win == win) {
            memmove ( ezx.timer_l+i, ezx.timer_l+i+1,
                     (ezx.timer_nb-i-1)*sizeof(Ez_timer) );
            ezx.timer_nb--;
            return 0;
        }
    return -1;
}


/*
 * Return delay between current date and next timer.
 * To pass directly to select().
*/

struct timeval *ez_timer_delay (void)
{
    static struct timeval t;

    /* No timer */
    if (ezx.timer_nb == 0) return NULL;

    /* Retrieve current date */
    ez_gettimeofday (&t);

    /* The next timer is ezx.timer_list[0].expiration ;
       we compute the difference with the current date */
    t.tv_sec  = ezx.timer_l[0].expiration.tv_sec  - t.tv_sec;
    t.tv_usec = ezx.timer_l[0].expiration.tv_usec - t.tv_usec;
    if (t.tv_usec < 0) { t.tv_usec += 1000000; t.tv_sec -= 1;}
    if (t.tv_sec  < 0) t.tv_sec = t.tv_usec = 0;

    /* printf ("Timeout in %d s  %6d us\n", (int)t.tv_sec, (int)t.tv_usec); */

    /* Return static address of the struct */
    return &t;
}


#ifdef EZ_BASE_XLIB

/*
 * Blocking waiting of en event.
 * Replace XNextEvent and add events (TimerNotify).
*/

void ez_event_next (Ez_event *ev)
{
    int n, res, fdx = ConnectionNumber(ezx.display);
    fd_set set1;

    /* Initialize ev */
    memset (ev, 0, sizeof(Ez_event));
    ev->type = EzLastEvent;
    ev->win = None;

    /* Label allowing to ignore an event and start again waiting */
    start_waiting:

    /* Do a XFlush and retrieve the number of events in the queue,
     * without reading and without blocking.
    */
    n = XEventsQueued (ezx.display, QueuedAfterFlush);

    /* If there is at least one event in the queue, we can read it without
     * blocking with XNextEvent(). We must do it here since a waiting with
     * select() would be blocking if the server did already send all events.
    */
    if (n > 0) {
        XNextEvent (ezx.display, &ev->xev);
        if ( (ev->xev.type == Expose) &&
             ezx.last_expose && ! ez_is_last_expose (&ev->xev))
            goto start_waiting;
        return;
    }

    /* The queue on the client side is empty, we start waiting */
    FD_ZERO (&set1);
    FD_SET (fdx, &set1);

    res = select (fdx+1, &set1, NULL, NULL, ez_timer_delay ());

    if (res > 0) {
        if (FD_ISSET (fdx, &set1)) {
            XNextEvent (ezx.display, &ev->xev);
            if ( (ev->xev.type == Expose) &&
                 ezx.last_expose && ! ez_is_last_expose (&ev->xev) )
                goto start_waiting;
            return;
        }

    } else if (res == 0) {

        if (ezx.timer_nb == 0) {
            ez_error ("ez_event_next: select() should not return 0\n");
            goto start_waiting;
        }
        ev->type = TimerNotify;
        ev->win = ezx.timer_l[0].win;
        ez_timer_remove (ev->win);

    } else {
        perror ("ez_event_next: select()");
    }
}


/* Type for XCheckIfEvent */
typedef struct { int later; Ez_window win; } Ez_predicat_Expose;

/* Prototype imposed by XCheckIfEvent */
Bool ez_predicat_expose (Display *display, XEvent *xev, XPointer arg)
{
    Ez_predicat_Expose *p = (Ez_predicat_Expose *) arg;
    (void) display;  /* unused variable. */
    if (xev->type == Expose && xev->xexpose.window == p->win)
        p->later = 1;
    return 0;
}

/*
 * Check if this is the last Expose in the queue, without modifying it to
 * preserve temporal order. Return boolean.
*/

int ez_is_last_expose (XEvent *xev)
{
    Ez_predicat_Expose p;
    XEvent ev_return;

    if (xev->xexpose.count > 0) return 0;

    p.later = 0;
    p.win = xev->xexpose.window;

    /* Check the whole queue without alteration, because the predicate
     * function ez_predicat_expose always returns FALSE */
    XCheckIfEvent (ezx.display, &ev_return, ez_predicat_expose, (XPointer) &p);

    return ! p.later;
}


/*
 * Analyse an event then call the callback.
*/

void ez_event_dispatch (Ez_event *ev)
{
    /* Double buffer */
    ezx.dbuf_pix = None; ezx.dbuf_win = None;

    /* We decode the event. To ignore, just make return */
    if (ev->type == EzLastEvent)
    switch (ev->xev.type) {

        /* The window must be redrawn. */
        case Expose :
            /* Some Expose will be ignored. */
            if (ezx.last_expose && ! ez_is_last_expose (&ev->xev))
                return;
            ev->type = ev->xev.type;
            ev->win  = ev->xev.xexpose.window;
            ez_dbuf_get (ev->win, &ezx.dbuf_pix);
            if (ezx.dbuf_pix != None) ez_dbuf_preswap (ev->win);
            ez_window_reset (ev->win);
            break;

        /* A mouse button was pressed or released. */
        case ButtonPress :
            ev->type = ev->xev.type;
            ev->win  = ev->xev.xbutton.window;
            ev->mx   = ev->xev.xbutton.x;
            ev->my   = ev->xev.xbutton.y;
            ev->mb   = ev->xev.xbutton.button;
            /* If another button is pressed, it is ignored. */
            if (ezx.mouse_b != 0) return;
            ezx.mouse_b = ev->mb;
            break;

        case ButtonRelease :
            ev->type = ev->xev.type;
            ev->win  = ev->xev.xbutton.window;
            ev->mx   = ev->xev.xbutton.x;
            ev->my   = ev->xev.xbutton.y;
            ev->mb   = ev->xev.xbutton.button;
            /* If another button is released, it is ignored. */
            if (ezx.mouse_b != ev->mb) return;
            ezx.mouse_b = 0;
            break;

        /* The mouse was moved in the window. */
        case MotionNotify :
            ev->type = ev->xev.type;
            ev->win  = ev->xev.xmotion.window;
            ev->mx   = ev->xev.xmotion.x;
            ev->my   = ev->xev.xmotion.y;
            ev->mb   = ezx.mouse_b;  /* because no xmotion.button ! */
            /* If the same event has already been sent, it is ignored. */
            if (ezx.mv_win == ev->win && ezx.mv_x == ev->mx && ezx.mv_y == ev->my)
                    return;
            ezx.mv_win = ev->win; ezx.mv_x = ev->mx; ezx.mv_y = ev->my;
            break;

        /* A key was pressed or released. */
        case KeyPress :
        case KeyRelease :
            ev->type = ev->xev.type;
            ev->win  = ev->xev.xkey.window;
            ev->mx   = ev->xev.xkey.x;
            ev->my   = ev->xev.xkey.y;

            ev->key_count = XLookupString (&ev->xev.xkey, ev->key_string,
                sizeof(ev->key_string)-1, &ev->key_sym, NULL);
            ev->key_string[ev->key_count] = 0;  /* add terminal '\0' */
            sprintf (ev->key_name, "XK_%s", XKeysymToString(ev->key_sym));
            break;

        /* Get new window size */
        case ConfigureNotify  :
            ev->type   = ev->xev.type;
            ev->win    = ev->xev.xconfigure.window;
            ev->width  = ev->xev.xconfigure.width;
            ev->height = ev->xev.xconfigure.height;
            break;

        /* Intercept window close: see ez_auto_quit() */
        case ClientMessage :
            if ((Atom) ev->xev.xclient.message_type == ezx.atom_protoc &&
                (Atom) ev->xev.xclient.data.l[0]    == ezx.atom_delwin)
            {
                if (ezx.auto_quit) {
                    ez_quit ();
                    return;
                } else {
                    ev->type  = WindowClose;
                    ev->win   = ev->xev.xclient.window;
                }
            } else return;
            break;

        default : return;

    } /* End event decoding */

    /* Call the window callback. */
    ez_func_call (ev);

    /* Swap double buffer */
    if (ezx.dbuf_pix != None) ez_dbuf_swap (ev->win);
}

#elif defined EZ_BASE_WIN32

/*
 * Blocking waiting of a message.
 * Replace GetMessage by adding the message WM_TIMER with a better accuracy
 * than with native timers (whose have an accuracy of 10 or 15ms).
*/

void ez_msg_next (MSG *msg)
{
    struct timeval *tv;
    double dt_ms;
    int k;

    start_waiting:
    tv = ez_timer_delay ();
    if (tv == NULL) dt_ms = INFINITE;
    else {
        dt_ms = tv->tv_sec*1000 + tv->tv_usec/1000;
        if (dt_ms < 0) dt_ms = 0;
    }

    k = MsgWaitForMultipleObjectsEx (0, NULL, dt_ms, QS_ALLINPUT,
            MWMO_INPUTAVAILABLE);  /* <-- very important! */

    if (k == WAIT_TIMEOUT) {
        memset (msg, 0, sizeof(MSG));
        msg->message = WM_TIMER;
        msg->hwnd = ezx.timer_l[0].win;
        ez_timer_remove (msg->hwnd);
    } else {
        if (! PeekMessage (msg, NULL, 0, 0, PM_REMOVE)) goto start_waiting;
        /* Add message WM_CHAR after a WM_KEYDOWN */
        TranslateMessage (msg);
    }
}


/*
 * The winproc called by DispatchMessage or by the system.
 * Return 0L to tell that the message is processed.
*/
LRESULT CALLBACK ez_win_proc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Ez_event ev;

    /* Initialize ev */
    memset (&ev, 0, sizeof(Ez_event));
    ev.win = None;  /* It it stays to None, the callback is not called */
    ev.xev.hwnd = hwnd; ev.xev.message = msg;
    ev.xev.wParam = wParam; ev.xev.lParam = lParam;

    /* Double buffer */
    ezx.dbuf_dc = None; ezx.dbuf_win = None;

    if (ez_draw_debug())
        if (msg != WM_SETCURSOR && msg != WM_NCHITTEST)
            printf ("    ez_win_proc  win = 0x%08x  MSG = 0x%04x = %-20s  wP = 0x%08x  lP = 0x%08x\n",
            PtrToInt(hwnd), (int) msg, ez_win_msg_name (msg), (int) wParam, (int) lParam);

    /* Decode message */
    switch (msg) {

        case WM_PAINT :
            ValidateRect (hwnd, NULL);
#ifdef __cplusplus
			[[fallthrough]];
#endif
        case EZ_MSG_PAINT :
            ev.type = Expose;
            ev.win = hwnd;
            ez_dbuf_get (ev.win, &ezx.dbuf_dc);
            if (ezx.dbuf_dc != None) ez_dbuf_preswap (ev.win);
            if (ez_draw_debug())
                printf ("Expose  win 0x%x  dbuf 0x%x\n",
                    ez_window_get_id(ev.win), PtrToInt(ezx.dbuf_dc));
            ez_window_reset (ev.win);
            break;

        /* A mouse button was pressed or released. */
        case WM_LBUTTONDOWN :
        case WM_MBUTTONDOWN :
        case WM_RBUTTONDOWN :
            ev.type = ButtonPress;
            ev.win  = hwnd;
            ev.mx   = GET_X_LPARAM (lParam);
            ev.my   = GET_Y_LPARAM (lParam);
            ev.mb   = msg == WM_LBUTTONDOWN ? 1 :
                      msg == WM_MBUTTONDOWN ? 2 :
                      msg == WM_RBUTTONDOWN ? 3 : 4;
            /* If another button is pressed, it is ignored. */
            if (ezx.mouse_b != 0) return 0L;
            ezx.mouse_b = ev.mb;
            /* Capture the mouse to get the MotionNotify events when the
               mouse comes out while button pressed. */
            SetCapture (hwnd);
            break;

        case WM_LBUTTONUP :
        case WM_MBUTTONUP :
        case WM_RBUTTONUP :
            ev.type = ButtonRelease;
            ev.win  = hwnd;
            ev.mx   = GET_X_LPARAM (lParam);
            ev.my   = GET_Y_LPARAM (lParam);
            ev.mb   = msg == WM_LBUTTONUP ? 1 :
                      msg == WM_MBUTTONUP ? 2 :
                      msg == WM_RBUTTONUP ? 3 : 4;
            /* If another button is pressed, it is ignored. */
            if (ezx.mouse_b != ev.mb) return 0L;
            ezx.mouse_b = 0;
            ReleaseCapture ();
            break;

        /* The mouse has moved in the window. */
        case WM_MOUSEMOVE :
            ev.type = MotionNotify;
            ev.win  = hwnd;
            ev.mx   = GET_X_LPARAM (lParam);
            ev.my   = GET_Y_LPARAM (lParam);
            ev.mb   = ezx.mouse_b;
            /* If the same event has already been sent, it is ignored. */
            if (ezx.mv_win == ev.win && ezx.mv_x == ev.mx && ezx.mv_y == ev.my)
                    return 0L;
            ezx.mv_win = ev.win; ezx.mv_x = ev.mx; ezx.mv_y = ev.my;
            break;

        /* A key was pressed or released. */
        case WM_SYSKEYDOWN :
        case WM_KEYDOWN :
        case WM_CHAR :
        {
            KeySym ksym;
            const char *kname, *kstring;

            /* Some keys generate two events, we filter. */
            if (msg == WM_CHAR) {
                if (ez_keychar_convert (wParam, &ksym, &kname, &kstring) < 0)
                    return 0L;
                /* Store the key for the next WM_KEYUP */
                ezx.key_sym = ksym;
                ezx.key_name = kname;
                ezx.key_string = kstring;
            } else {
                if (ez_keydown_convert (wParam, lParam, &ksym,&kname, &kstring) < 0)
                   return 0L;
            }

            ev.type    = KeyPress;
            ev.win     = hwnd;
            ev.mx      = ezx.mv_x;
            ev.my      = ezx.mv_y;
            ev.key_sym = ksym;
            strcpy (ev.key_name  , kname);
            strcpy (ev.key_string, kstring);
            ev.key_count = strlen (kstring);

            if (ez_is_repetition (lParam)) {
                if (ez_is_modifier (ksym)) {
                    /* Suppress repetitions for keys Ctrl, ALt and Shift */
                    return 0L;
                } else {
                    /* Simulate KeyRelease before the KeyPress */
                    ev.type = KeyRelease; ez_func_call (&ev); ev.type = KeyPress;
                }
            }
            break;
        }

        case WM_KEYUP :
        case WM_SYSKEYUP :
        {
            KeySym ksym;
            const char *kname, *kstring;

            if (ez_keydown_convert (wParam, lParam, &ksym, &kname, &kstring) < 0) {
               /* This message contains the description of the corresponding
                  WM_KEYDOWN, but that is the description of the previous
                  WM_CHAR which is relevant. */
                ksym = ezx.key_sym;
                kname = ezx.key_name;
                kstring = ezx.key_string;
                /* Reset */
                ezx.key_sym = 0;
                ezx.key_name = ezx.key_string = "";
            }
            if (ksym == 0) return 0L;

            ev.type    = KeyRelease;
            ev.win     = hwnd;
            ev.mx      = ezx.mv_x;
            ev.my      = ezx.mv_y;
            ev.key_sym = ksym;
            strcpy (ev.key_name  , kname);
            strcpy (ev.key_string, kstring);
            ev.key_count = strlen (kstring);
            break;
        }

        case WM_SIZE :
            ev.type   = ConfigureNotify;
            ev.win    = hwnd;
            ev.width  = LOWORD(lParam);
            ev.height = HIWORD(lParam);
            break;

     case WM_TIMER :
            ev.type   = TimerNotify;
            ev.win    = hwnd;
            break;

        case WM_CLOSE :
            if (ezx.auto_quit) {
                ez_quit ();
                return 0L;
            } else {
                ev.type  = WindowClose;
                ev.win   = hwnd;
            }
            break;

        default:
            return DefWindowProc (hwnd, msg, wParam, lParam);

    } /* End message decoding */

    /* Call the window callback. */
    ez_func_call (&ev);

    /* Swap Double buffer */
    if (ezx.dbuf_dc != None) ez_dbuf_swap (ev.win);

    return 0L;
}


/*
 * Return 1 if key_sim is a Ctrl, Shift or Alt key, else 0.
*/

int ez_is_modifier (KeySym key_sym)
{
    switch (key_sym) {
        case XK_Control_L :
        case XK_Control_R :
        case XK_Shift_L :
        case XK_Shift_R :
        case XK_Alt_L :
        case XK_Alt_R :
            return 1;
        default : break;
    }
    return 0;
}


/*
 * Return 1 if the key is repeated, else 0.
*/

int ez_is_repetition (LPARAM lParam)
{
    return (lParam & (1L << 30)) ? 1 : 0;
}


/*
 * Set this window as the current window, and associate or free the
 * drawing Handles if necessary.
*/

void ez_cur_win (Ez_window win)
{
    if (ezx.dc_win == win) return;
    if (ezx.dc_win != None && ezx.dc_win != ezx.dbuf_win)
        ReleaseDC (ezx.dc_win, ezx.hdc);

    ezx.dc_win = win;
    if (ezx.dc_win == None) return;

    if (ezx.dc_win != ezx.dbuf_win)
         ezx.hdc = GetDC (ezx.dc_win);
    else ezx.hdc = ezx.dbuf_dc;

    if (ezx.hpen   != NULL) SelectObject (ezx.hdc, ezx.hpen);
    if (ezx.hbrush != NULL) SelectObject (ezx.hdc, ezx.hbrush);
    if (ezx.font[ezx.nfont] != NULL)
        SelectObject (ezx.hdc, ezx.font[ezx.nfont]);
    SetTextColor (ezx.hdc, ezx.color);
}


/*
 * Create a pen to draw with the current color and thickness.
*/

void ez_update_pen (void)
{
    if (ezx.hpen != NULL) DeleteObject (ezx.hpen);
    ezx.hpen = CreatePen (PS_SOLID, (ezx.thick == 1) ? 0 : ezx.thick, ezx.color);
    if (ezx.dc_win != None) SelectObject (ezx.hdc, ezx.hpen);
}

#endif /* EZ_BASE_ */


/*
 * Initialize the random numbers generator
*/

void ez_random_init (void)
{
#ifdef EZ_BASE_XLIB
    srandom ((int) time (NULL));
#elif defined EZ_BASE_WIN32
    srand ((int) time (NULL));
#endif /* EZ_BASE_ */
}


/*
 * Return insertion point in the sorted list ezx.win_l
*/

int ez_win_list_find (Ez_window win)
{
    int m1, m2, mid;

    m1 = 0; m2 = ezx.win_nb;
    while (m2-m1 > 0) {
        mid = (m1+m2) / 2;    /* Middle s.t. m1 <= mid < m2 */

        if (win > ezx.win_l[mid])
             m1 = mid+1;   /* Search in [mid+1 .. m2] */
        else m2 = mid;     /* Search in [m1 .. mid]   */
    }

    return m1;
}


/*
 * Insert a window in the sorted list ezx.win_l
 * (win != None, and win not already member of the list)
 * Return 0 on success, -1 on error.
*/

int ez_win_list_insert (Ez_window win)
{
    int k;

    if (ezx.win_nb >= EZ_WIN_MAX) {
        ez_error ("ez_win_list_insert: too many windows\n");
        return -1;
    }
    k = ez_win_list_find (win);

    /* Insert in position k */
    if (k < ezx.win_nb)
        memmove ( ezx.win_l+k+1, ezx.win_l+k,
                 (ezx.win_nb-k)*sizeof(Ez_window) );
    ezx.win_nb++;
    ezx.win_l[k] = win;
    return 0;
}


/*
 * Suppress the window from the sorted list ezx.win_l
 * Return 0 on success, -1 on error.
*/

int ez_win_list_remove (Ez_window win)
{
    int k = ez_win_list_find (win);

    if (k >= ezx.win_nb || ezx.win_l[k] != win) {
        ez_error ("ez_win_list_remove: can't find window\n");
        return -1;
    }

    ezx.win_nb--;
    if (k < ezx.win_nb)
        memmove ( ezx.win_l+k, ezx.win_l+k+1,
                 (ezx.win_nb-k)*sizeof(Ez_window) );
    return 0;
}


/*
 * Associate a property to a window.
 * Return 0 on success, -1 on error.
*/

int ez_prop_set (Ez_window win, XContext prop, void *value)
{
#ifdef EZ_BASE_XLIB
    if (XSaveContext (ezx.display, win, prop, (XPointer) value) != 0) {
        ez_error ("ez_prop_set: XSaveContext failed\n");
        return -1;
    }
#elif defined EZ_BASE_WIN32
    if (SetProp (win, prop, (HANDLE) value) == 0) {
        ez_error ("ez_prop_set: SetProp failed\n");
        return -1;
    }
#endif /* EZ_BASE_ */
    return 0;
}


/*
 * Retrieve a property associated to a window.
 * Return 0 on success, -1 on error.
*/

int ez_prop_get (Ez_window win, XContext prop, void **value)
{
    if (value == NULL) return -1;

#ifdef EZ_BASE_XLIB
    *value = NULL;
    if (XFindContext (ezx.display, win, prop, (XPointer*) value) != 0)
        return -1;
#elif defined EZ_BASE_WIN32
    *value = GetProp (win, prop);
#endif /* EZ_BASE_ */
    return 0;
}


/*
 * Delete the property associated to the window.
 * Return 0 on success, -1 on error.
*/

int ez_prop_destroy (Ez_window win, XContext prop)
{
#ifdef EZ_BASE_XLIB
    return XDeleteContext (ezx.display, win, prop) == 0 ? 0 : -1;
#elif defined EZ_BASE_WIN32
    return RemoveProp (win, prop) == NULL ? -1 : 0;
#endif /* EZ_BASE_ */
}


/*
 * Retrieve the data associated to a window.
 * Return 0 on success, -1 on error.
*/

int ez_info_get (Ez_window win, Ez_win_info **info)
{
    if (ez_prop_get (win, ezx.info_prop, (void **) info) < 0) return -1;
    if (*info == NULL) {
        ez_error ("ez_info_get: can't get Ez_win_info\n");
        return -1;
    }
    return 0;
}


/*
 * Associate a callback func to a window.
 * A new call overwrite the previous callback.
 * To deactivate the callback, call with func = NULL.
 * Return 0 on success, -1 on error.
*/

int ez_func_set (Ez_window win, Ez_func func)
{
    Ez_win_info *info;
    if (ez_info_get (win, &info) < 0) return -1;
    info->func = func;
    return 0;
}


/*
 * Retrieve the callback associated to the window.
 * Return 0 on success, -1 on error.
*/

int ez_func_get (Ez_window win, Ez_func *func)
{
    Ez_win_info *info;
    if (ez_info_get (win, &info) < 0) return -1;
    *func = info->func;
    return 0;
}


/*
 * Check if a callback has been defined for this window.
 * If yes, call the callback and return 0, else return -1.
*/

int ez_func_call (Ez_event *ev)
{
    Ez_func func;

    /* No drawable */
    if (ev->win == None) return -1;

#ifdef EZ_BASE_XLIB
    /* NoExpose and GraphicsExpose does not involve pixmaps */
    if (ev->type == NoExpose || ev->type == GraphicsExpose) return -1;
#endif /* EZ_BASE_ */

    /* Is there a callback? */
    if (ez_func_get (ev->win, &func) < 0) return -1;
    if (func == NULL) return -1;

    /* Call the callback */
    func (ev);

    return 0;
}


/*
 * Initialize the double buffer mode, which allows to redraw the pixels without
 * blinking.
*/

void ez_dbuf_init (void)
{
#ifdef EZ_BASE_XLIB
    int m1, m2;
    /* Load the DBE extension */
    if (XdbeQueryExtension (ezx.display, &m1, &m2) == 0)
        ez_error ("ez_dbuf_init: DBE extension failed\n");
    ezx.dbuf_pix = None;
#elif defined EZ_BASE_WIN32
    ezx.dbuf_dc  = None;
#endif /* EZ_BASE_ */
    ezx.dbuf_win = None;
}


/*
 * Store dbuf in win.
 * Return 0 on success, -1 on error.
*/

int ez_dbuf_set (Ez_window win, XdbeBackBuffer dbuf)
{
    Ez_win_info *info;
    if (ez_info_get (win, &info) < 0) return -1;
    info->dbuf = dbuf;
    return 0;
}


/*
 * Retrieve dbuf from win.
 * Return 0 on success, -1 on error.
*/

int ez_dbuf_get (Ez_window win, XdbeBackBuffer *dbuf)
{
    Ez_win_info *info;
    if (ez_info_get (win, &info) < 0) return -1;
    *dbuf = info->dbuf;
    return 0;
}


/*
 * Prepare the double buffer for the swap.
*/

void ez_dbuf_preswap (Ez_window win)
{
#ifdef EZ_BASE_XLIB
    ezx.dbuf_win = win;
#elif defined EZ_BASE_WIN32
    ez_cur_win (win);
    ez_window_get_size (win, &ezx.dbuf_w, &ezx.dbuf_h);
    ezx.hMemBmp = CreateCompatibleBitmap (ezx.hdc, ezx.dbuf_w, ezx.dbuf_h);
    ezx.hOldBmp = (HBITMAP) SelectObject (ezx.dbuf_dc, ezx.hMemBmp);
    ez_cur_win (None);
    ezx.dbuf_win = win;
#endif /* EZ_BASE_ */
}


/*
 * Swap the buffers for the window.
*/

void ez_dbuf_swap (Ez_window win)
{
#ifdef EZ_BASE_XLIB
    XdbeSwapInfo swap_info[1];
    swap_info[0].swap_window = win;
    swap_info[0].swap_action = XdbeUndefined;
    XdbeSwapBuffers (ezx.display, swap_info, 1);
#elif defined EZ_BASE_WIN32
    ez_cur_win (None);
    ezx.dbuf_win = None;
    ez_cur_win (win);
    BitBlt (ezx.hdc, 0, 0, ezx.dbuf_w, ezx.dbuf_h, ezx.dbuf_dc, 0, 0, SRCCOPY);
    SelectObject (ezx.dbuf_dc, ezx.hOldBmp);
    DeleteObject (ezx.hMemBmp);
#endif /* EZ_BASE_ */
}


/*
 * Initialize the fonts.
*/

void ez_font_init (void)
{
    int i;
    for (i = 0; i < EZ_FONT_MAX; i++)
        ezx.font[i] = NULL;
    ez_font_load (0, "6x13");
    ez_font_load (1, "8x16");
    ez_font_load (2, "10x20");
    ez_font_load (3, "12x24");
}


/*
 * Free the fonts
*/

void ez_font_delete (void)
{
#ifdef EZ_BASE_XLIB
    int i;
    for (i = 0; i < EZ_FONT_MAX; i++)
    if (ezx.font[i] != NULL)
      { XFreeFont (ezx.display, ezx.font[i]); ezx.font[i] = NULL; }
#elif defined EZ_BASE_WIN32
    int i;
    for (i = 0; i < EZ_FONT_MAX; i++)
    if (ezx.font[i] != 0)
       { DeleteObject (ezx.font[i]); ezx.font[i] = NULL; }
#endif /* EZ_BASE_ */
}


/*
 * Initialize colors.
 *
 * Author: Regis.Barbanchon@lif.univ-mrs.fr
 * This code was extracted from MyX_Color.c
*/

int ez_color_init (void)  {

#ifdef EZ_BASE_XLIB

    ezx.visual = DefaultVisual (ezx.display, ezx.screen_num);

    switch (ezx.visual->c_class) {
        case PseudoColor : ez_init_PseudoColor (); break;
        case TrueColor   : ez_init_TrueColor   (); break;
        default : ez_error ("ez_init_color: unsupported Visual\n");
                  return -1;
    }

#endif /* EZ_BASE_ */

    /* Predefined colors */
    ez_black   = ez_get_RGB (0, 0, 0);
    ez_white   = ez_get_RGB (255, 255, 255);
    ez_grey    = ez_get_RGB (150, 150, 150);
    ez_red     = ez_get_RGB (255, 0, 0);
    ez_green   = ez_get_RGB (0, 255, 0);
    ez_blue    = ez_get_RGB (0, 0, 255);
    ez_yellow  = ez_get_RGB (255, 255, 0);
    ez_cyan    = ez_get_RGB (0, 255, 255);
    ez_magenta = ez_get_RGB (255, 0, 255);
    return 0;
}


#ifdef EZ_BASE_XLIB

/*
 * Store a color in the palette.
*/

void ez_set_palette (Ez_uint32 pixel, int R, int G, int B,
    int max, int inCube)
{
    XColor c;

    c.pixel = pixel;
    c.red   = R / (double) max * 0xFFFF ;
    c.green = G / (double) max * 0xFFFF ;
    c.blue  = B / (double) max * 0xFFFF ;
    c.flags = DoRed | DoGreen | DoBlue;
    XStoreColor (ezx.display, ezx.pseudoColor.colormap, &c);
    if (inCube) ezx.pseudoColor.palette [R][G][B] = c.pixel;
    ezx.pseudoColor.samples[c.pixel] = c;
}


/*
 * Initialize color palette.
*/

void ez_init_PseudoColor (void)
{
    int i, j, k;
    Ez_uint32 pixel;

    ez_get_RGB = ez_get_RGB_pseudo_color;

    ezx.pseudoColor.colormap =
        XCreateColormap (ezx.display, ezx.root_win, ezx.visual, AllocAll);

    for (i = 0, pixel = 0; i < 3; i++)
    for (j = 0; j < 6; j++)
    for (k = 0; k < 6; k++, pixel++)
        ez_set_palette (pixel, i, j, k, 5, 1);

    for (i = 0 ; i < 40; i++, pixel++)
        ez_set_palette (pixel, i, i, i, 39, 0);

    for (i = 3; i < 6; i++)
    for (j = 0; j < 6; j++)
    for (k = 0; k < 6; k++, pixel++)
        ez_set_palette (pixel, i, j, k, 5, 1);
}


/*
 * Initialize the true color mode.
*/

void ez_init_TrueColor (void)
{
    ez_init_channel (&ezx.trueColor.blue , ezx.visual-> blue_mask);
    ez_init_channel (&ezx.trueColor.green, ezx.visual->green_mask);
    ez_init_channel (&ezx.trueColor.red  , ezx.visual->  red_mask);
}


/*
 * Initialize a color channel.
*/

void ez_init_channel (Ez_channel *channel, Ez_uint32 mask)
{
    channel->mask = mask;
    channel->shift = channel->length = channel->max = 0;
    if (! mask) return;
    while ( (mask & 1) == 0 ) { channel->shift  ++; mask >>= 1; }
    while ( (mask & 1) == 1 ) { channel->length ++; mask >>= 1; }
    channel->max = channel->mask >> channel->shift;
}


/*
 * Compute a color for R,G,B levels between 0 and 255.
*/

Ez_uint32 ez_get_RGB_true_color (Ez_uint8 r, Ez_uint8 g, Ez_uint8 b)
{
    return r >> (8 - ezx.trueColor.red  .length) << ezx.trueColor.red  .shift |
           g >> (8 - ezx.trueColor.green.length) << ezx.trueColor.green.shift |
           b >> (8 - ezx.trueColor.blue .length) << ezx.trueColor.blue .shift ;
}

Ez_uint32 ez_get_RGB_pseudo_color (Ez_uint8 r, Ez_uint8 g, Ez_uint8 b)
{
     if (r == g && g == b)
         return ezx.pseudoColor.samples[108 + (int)r * 40/256].pixel;
     return ezx.pseudoColor.palette[r / 51][g / 51][b / 51];
}

#elif defined EZ_BASE_WIN32

Ez_uint32 ez_get_RGB_win32 (Ez_uint8 r, Ez_uint8 g, Ez_uint8 b)
{
     return RGB (r, g, b);
}


/*
 * Convert keyboard events from win32 to X11
*/

int ez_keydown_convert (WPARAM wParam, LPARAM lParam, KeySym *k,const char **n, const char **s)
{
    const char *es = "";

    switch (wParam) {
        case VK_PAUSE   : *k = XK_Pause  ; *n = "XK_Pause"  ; *s = es ; break;
        case VK_PRIOR   : *k = XK_Prior  ; *n = "XK_Prior"  ; *s = es ; break;
        case VK_NEXT    : *k = XK_Next   ; *n = "XK_Next"   ; *s = es ; break;
        case VK_END     : *k = XK_End    ; *n = "XK_End"    ; *s = es ; break;
        case VK_HOME    : *k = XK_Home   ; *n = "XK_Home"   ; *s = es ; break;
        case VK_LEFT    : *k = XK_Left   ; *n = "XK_Left"   ; *s = es ; break;
        case VK_UP      : *k = XK_Up     ; *n = "XK_Up"     ; *s = es ; break;
        case VK_RIGHT   : *k = XK_Right  ; *n = "XK_Right"  ; *s = es ; break;
        case VK_DOWN    : *k = XK_Down   ; *n = "XK_Down"   ; *s = es ; break;
        case VK_INSERT  : *k = XK_Insert ; *n = "XK_Insert" ; *s = es ; break;
        case VK_DELETE  : *k = XK_Delete ; *n = "XK_Delete" ; *s = es ; break;
        case VK_F1      : *k = XK_F1     ; *n = "XK_F1"     ; *s = es ; break;
        case VK_F2      : *k = XK_F2     ; *n = "XK_F2"     ; *s = es ; break;
        case VK_F3      : *k = XK_F3     ; *n = "XK_F3"     ; *s = es ; break;
        case VK_F4      : *k = XK_F4     ; *n = "XK_F4"     ; *s = es ; break;
        case VK_F5      : *k = XK_F5     ; *n = "XK_F5"     ; *s = es ; break;
        case VK_F6      : *k = XK_F6     ; *n = "XK_F6"     ; *s = es ; break;
        case VK_F7      : *k = XK_F7     ; *n = "XK_F7"     ; *s = es ; break;
        case VK_F8      : *k = XK_F8     ; *n = "XK_F8"     ; *s = es ; break;
        case VK_F9      : *k = XK_F9     ; *n = "XK_F9"     ; *s = es ; break;
        case VK_F10     : *k = XK_F10    ; *n = "XK_F10"    ; *s = es ; break;

        case VK_SHIFT   :
                if (lParam & (1L << 20))
                    { *k = XK_Shift_R   ; *n = "XK_Shift_R"   ; *s = es; }
               else { *k = XK_Shift_L   ; *n = "XK_Shift_L"   ; *s = es; } break;
        case VK_CONTROL :
                if (lParam & (1L << 24))
                    { *k = XK_Control_R ; *n = "XK_Control_R" ; *s = es; }
               else { *k = XK_Control_L ; *n = "XK_Control_L" ; *s = es; } break;
        case VK_MENU    :
                if (lParam & (1L << 24))
                    { *k = XK_Alt_R     ; *n = "XK_Alt_R"     ; *s = es; }
               else { *k = XK_Alt_L     ; *n = "XK_Alt_L"     ; *s = es; } break;
        /* BUG: AltGr generates Control_L + ALt_R */

        /* Symbols still to be processed, please help! */
     /* case VK_CAPITAL  : *k = XK_ ; *n = "XK_"; *s = es; break; */
     /* case VK_NUMLOCK  : *k = XK_ ; *n = "XK_"; *s = es; break; */
     /* case VK_SCROLL   : *k = XK_ ; *n = "XK_"; *s = es; break; */

        default : *k = 0; *n = es; *s = es; return -1;
    }

    return 0;
}

int ez_keychar_convert (WPARAM wParam, KeySym *k, const char **n, const char **s)
{
    const char *es = "";

    switch (wParam) {
        case VK_BACK   : *k = XK_BackSpace ; *n = "XK_BackSpace" ; *s = es  ; break;
        case VK_TAB    : *k = XK_Tab       ; *n = "XK_Tab"       ; *s = es  ; break;
        case VK_RETURN : *k = XK_Return    ; *n = "XK_Return"    ; *s = es  ; break;
        case VK_ESCAPE : *k = XK_Escape    ; *n = "XK_Escape"    ; *s = es  ; break;
        case VK_SPACE  : *k = XK_space     ; *n = "XK_space"     ; *s = " " ; break;

        case 0x0021    : *k = XK_exclam    ; *n = "XK_exclam"    ; *s = "!" ; break;
        case 0x0022    : *k = XK_quotedbl  ; *n = "XK_quotedbl"  ; *s = "\""; break;
        case 0x0023    : *k = XK_numbersign; *n = "XK_numbersign"; *s = "#" ; break;
        case 0x0024    : *k = XK_dollar    ; *n = "XK_dollar"    ; *s = "$" ; break;
        case 0x0025    : *k = XK_percent   ; *n = "XK_percent"   ; *s = "%" ; break;
        case 0x0026    : *k = XK_ampersand ; *n = "XK_ampersand" ; *s = "&" ; break;
        case 0x0027    : *k = XK_apostrophe; *n = "XK_apostrophe"; *s = "'" ; break;
        case 0x0028    : *k = XK_parenleft ; *n = "XK_parenleft" ; *s = "(" ; break;
        case 0x0029    : *k = XK_parenright; *n = "XK_parenright"; *s = ")" ; break;
        case 0x002a    : *k = XK_asterisk  ; *n = "XK_asterisk"  ; *s = "*" ; break;
        case 0x002b    : *k = XK_plus      ; *n = "XK_plus"      ; *s = "+" ; break;
        case 0x002c    : *k = XK_comma     ; *n = "XK_comma"     ; *s = "," ; break;
        case 0x002d    : *k = XK_minus     ; *n = "XK_minus"     ; *s = "-" ; break;
        case 0x002e    : *k = XK_period    ; *n = "XK_period"    ; *s = "." ; break;
        case 0x002f    : *k = XK_slash     ; *n = "XK_slash"     ; *s = "/" ; break;

        case 0x0030 : *k = XK_0        ; *n = "XK_0"        ; *s = "0" ; break;
        case 0x0031 : *k = XK_1        ; *n = "XK_1"        ; *s = "1" ; break;
        case 0x0032 : *k = XK_2        ; *n = "XK_2"        ; *s = "2" ; break;
        case 0x0033 : *k = XK_3        ; *n = "XK_3"        ; *s = "3" ; break;
        case 0x0034 : *k = XK_4        ; *n = "XK_4"        ; *s = "4" ; break;
        case 0x0035 : *k = XK_5        ; *n = "XK_5"        ; *s = "5" ; break;
        case 0x0036 : *k = XK_6        ; *n = "XK_6"        ; *s = "6" ; break;
        case 0x0037 : *k = XK_7        ; *n = "XK_7"        ; *s = "7" ; break;
        case 0x0038 : *k = XK_8        ; *n = "XK_8"        ; *s = "8" ; break;
        case 0x0039 : *k = XK_9        ; *n = "XK_9"        ; *s = "9" ; break;
        case 0x003a : *k = XK_colon    ; *n = "XK_colon"    ; *s = ":" ; break;
        case 0x003b : *k = XK_semicolon; *n = "XK_semicolon"; *s = ";" ; break;
        case 0x003c : *k = XK_less     ; *n = "XK_less"     ; *s = "<" ; break;
        case 0x003d : *k = XK_equal    ; *n = "XK_equal"    ; *s = "=" ; break;
        case 0x003e : *k = XK_greater  ; *n = "XK_greater"  ; *s = ">" ; break;
        case 0x003f : *k = XK_question ; *n = "XK_question" ; *s = "?" ; break;
        case 0x0040 : *k = XK_at       ; *n = "XK_at"       ; *s = "@" ; break;
        case 0x0041 : *k = XK_A        ; *n = "XK_A"        ; *s = "A" ; break;
        case 0x0042 : *k = XK_B        ; *n = "XK_B"        ; *s = "B" ; break;
        case 0x0043 : *k = XK_C        ; *n = "XK_C"        ; *s = "C" ; break;
        case 0x0044 : *k = XK_D        ; *n = "XK_D"        ; *s = "D" ; break;
        case 0x0045 : *k = XK_E        ; *n = "XK_E"        ; *s = "E" ; break;
        case 0x0046 : *k = XK_F        ; *n = "XK_F"        ; *s = "F" ; break;
        case 0x0047 : *k = XK_G        ; *n = "XK_G"        ; *s = "G" ; break;
        case 0x0048 : *k = XK_H        ; *n = "XK_H"        ; *s = "H" ; break;
        case 0x0049 : *k = XK_I        ; *n = "XK_I"        ; *s = "I" ; break;
        case 0x004a : *k = XK_J        ; *n = "XK_J"        ; *s = "J" ; break;
        case 0x004b : *k = XK_K        ; *n = "XK_K"        ; *s = "K" ; break;
        case 0x004c : *k = XK_L        ; *n = "XK_L"        ; *s = "L" ; break;
        case 0x004d : *k = XK_M        ; *n = "XK_M"        ; *s = "M" ; break;
        case 0x004e : *k = XK_N        ; *n = "XK_N"        ; *s = "N" ; break;
        case 0x004f : *k = XK_O        ; *n = "XK_O"        ; *s = "O" ; break;
        case 0x0050 : *k = XK_P        ; *n = "XK_P"        ; *s = "P" ; break;
        case 0x0051 : *k = XK_Q        ; *n = "XK_Q"        ; *s = "Q" ; break;
        case 0x0052 : *k = XK_R        ; *n = "XK_R"        ; *s = "R" ; break;
        case 0x0053 : *k = XK_S        ; *n = "XK_S"        ; *s = "S" ; break;
        case 0x0054 : *k = XK_T        ; *n = "XK_T"        ; *s = "T" ; break;
        case 0x0055 : *k = XK_U        ; *n = "XK_U"        ; *s = "U" ; break;
        case 0x0056 : *k = XK_V        ; *n = "XK_V"        ; *s = "V" ; break;
        case 0x0057 : *k = XK_W        ; *n = "XK_W"        ; *s = "W" ; break;
        case 0x0058 : *k = XK_X        ; *n = "XK_X"        ; *s = "X" ; break;
        case 0x0059 : *k = XK_Y        ; *n = "XK_Y"        ; *s = "Y" ; break;
        case 0x005a : *k = XK_Z        ; *n = "XK_Z"        ; *s = "Z" ; break;

        case 0x005b : *k = XK_bracketleft  ; *n = "XK_bracketleft"  ; *s = "[" ; break;
        case 0x005c : *k = XK_backslash    ; *n = "XK_backslash"    ; *s = "\\"; break;
        case 0x005d : *k = XK_bracketright ; *n = "XK_bracketright" ; *s = "]" ; break;
        case 0x005e : *k = XK_asciicircum  ; *n = "XK_asciicircum"  ; *s = "^" ; break;
        case 0x005f : *k = XK_underscore   ; *n = "XK_underscore"   ; *s = "_" ; break;
        case 0x0060 : *k = XK_grave        ; *n = "XK_grave"        ; *s = "`" ; break;

        case 0x0061 : *k = XK_a        ; *n = "XK_a"        ; *s = "a" ; break;
        case 0x0062 : *k = XK_b        ; *n = "XK_b"        ; *s = "b" ; break;
        case 0x0063 : *k = XK_c        ; *n = "XK_c"        ; *s = "c" ; break;
        case 0x0064 : *k = XK_d        ; *n = "XK_d"        ; *s = "d" ; break;
        case 0x0065 : *k = XK_e        ; *n = "XK_e"        ; *s = "e" ; break;
        case 0x0066 : *k = XK_f        ; *n = "XK_f"        ; *s = "f" ; break;
        case 0x0067 : *k = XK_g        ; *n = "XK_g"        ; *s = "g" ; break;
        case 0x0068 : *k = XK_h        ; *n = "XK_h"        ; *s = "h" ; break;
        case 0x0069 : *k = XK_i        ; *n = "XK_i"        ; *s = "i" ; break;
        case 0x006a : *k = XK_j        ; *n = "XK_j"        ; *s = "j" ; break;
        case 0x006b : *k = XK_k        ; *n = "XK_k"        ; *s = "k" ; break;
        case 0x006c : *k = XK_l        ; *n = "XK_l"        ; *s = "l" ; break;
        case 0x006d : *k = XK_m        ; *n = "XK_m"        ; *s = "m" ; break;
        case 0x006e : *k = XK_n        ; *n = "XK_n"        ; *s = "n" ; break;
        case 0x006f : *k = XK_o        ; *n = "XK_o"        ; *s = "o" ; break;
        case 0x0070 : *k = XK_p        ; *n = "XK_p"        ; *s = "p" ; break;
        case 0x0071 : *k = XK_q        ; *n = "XK_q"        ; *s = "q" ; break;
        case 0x0072 : *k = XK_r        ; *n = "XK_r"        ; *s = "r" ; break;
        case 0x0073 : *k = XK_s        ; *n = "XK_s"        ; *s = "s" ; break;
        case 0x0074 : *k = XK_t        ; *n = "XK_t"        ; *s = "t" ; break;
        case 0x0075 : *k = XK_u        ; *n = "XK_u"        ; *s = "u" ; break;
        case 0x0076 : *k = XK_v        ; *n = "XK_v"        ; *s = "v" ; break;
        case 0x0077 : *k = XK_w        ; *n = "XK_w"        ; *s = "w" ; break;
        case 0x0078 : *k = XK_x        ; *n = "XK_x"        ; *s = "x" ; break;
        case 0x0079 : *k = XK_y        ; *n = "XK_y"        ; *s = "y" ; break;
        case 0x007a : *k = XK_z        ; *n = "XK_z"        ; *s = "z" ; break;

        case 0x007b : *k = XK_braceleft  ; *n = "XK_braceleft"  ; *s = "{" ; break;
        case 0x007c : *k = XK_bar        ; *n = "XK_bar"        ; *s = "|" ; break;
        case 0x007d : *k = XK_braceright ; *n = "XK_braceright" ; *s = "}" ; break;
        case 0x007e : *k = XK_asciitilde ; *n = "XK_asciitilde" ; *s = "~" ; break;

        case 0x0080 : *k = XK_EuroSign   ; *n = "XK_EuroSign"   ; *s = "\200" ; break;

        case 0x00a0 : *k = XK_nobreakspace   ; *n = "XK_nobreakspace"   ; *s = "\240"; break;
        case 0x00a1 : *k = XK_exclamdown     ; *n = "XK_exclamdown"     ; *s = "\241"; break;
        case 0x00a2 : *k = XK_cent           ; *n = "XK_cent"           ; *s = "\242"; break;
        case 0x00a3 : *k = XK_sterling       ; *n = "XK_sterling"       ; *s = "\243"; break;
        case 0x00a4 : *k = XK_currency       ; *n = "XK_currency"       ; *s = "\244"; break;
        case 0x00a5 : *k = XK_yen            ; *n = "XK_yen"            ; *s = "\245"; break;
        case 0x00a6 : *k = XK_brokenbar      ; *n = "XK_brokenbar"      ; *s = "\246"; break;
        case 0x00a7 : *k = XK_section        ; *n = "XK_section"        ; *s = "\247"; break;
        case 0x00a8 : *k = XK_diaeresis      ; *n = "XK_diaeresis"      ; *s = "\250"; break;
        case 0x00a9 : *k = XK_copyright      ; *n = "XK_copyright"      ; *s = "\251"; break;
        case 0x00aa : *k = XK_ordfeminine    ; *n = "XK_ordfeminine"    ; *s = "\252"; break;
        case 0x00ab : *k = XK_guillemotleft  ; *n = "XK_guillemotleft"  ; *s = "\253"; break;
        case 0x00ac : *k = XK_notsign        ; *n = "XK_notsign"        ; *s = "\254"; break;
        case 0x00ad : *k = XK_hyphen         ; *n = "XK_hyphen"         ; *s = "\255"; break;
        case 0x00ae : *k = XK_registered     ; *n = "XK_registered"     ; *s = "\256"; break;
        case 0x00af : *k = XK_macron         ; *n = "XK_macron"         ; *s = "\257"; break;
        case 0x00b0 : *k = XK_degree         ; *n = "XK_degree"         ; *s = "\260"; break;
        case 0x00b1 : *k = XK_plusminus      ; *n = "XK_plusminus"      ; *s = "\261"; break;
        case 0x00b2 : *k = XK_twosuperior    ; *n = "XK_twosuperior"    ; *s = "\262"; break;
        case 0x00b3 : *k = XK_threesuperior  ; *n = "XK_threesuperior"  ; *s = "\263"; break;
        case 0x00b4 : *k = XK_acute          ; *n = "XK_acute"          ; *s = "\264"; break;
        case 0x00b5 : *k = XK_mu             ; *n = "XK_mu"             ; *s = "\265"; break;
        case 0x00b6 : *k = XK_paragraph      ; *n = "XK_paragraph"      ; *s = "\266"; break;
        case 0x00b7 : *k = XK_periodcentered ; *n = "XK_periodcentered" ; *s = "\267"; break;
        case 0x00b8 : *k = XK_cedilla        ; *n = "XK_cedilla"        ; *s = "\270"; break;
        case 0x00b9 : *k = XK_onesuperior    ; *n = "XK_onesuperior"    ; *s = "\271"; break;
        case 0x00ba : *k = XK_masculine      ; *n = "XK_masculine"      ; *s = "\272"; break;
        case 0x00bb : *k = XK_guillemotright ; *n = "XK_guillemotright" ; *s = "\273"; break;
        case 0x00bc : *k = XK_onequarter     ; *n = "XK_onequarter"     ; *s = "\274"; break;
        case 0x00bd : *k = XK_onehalf        ; *n = "XK_onehalf"        ; *s = "\275"; break;
        case 0x00be : *k = XK_threequarters  ; *n = "XK_threequarters"  ; *s = "\276"; break;
        case 0x00bf : *k = XK_questiondown   ; *n = "XK_questiondown"   ; *s = "\277"; break;
        case 0x00c0 : *k = XK_Agrave         ; *n = "XK_Agrave"         ; *s = "\300"; break;
        case 0x00c1 : *k = XK_Aacute         ; *n = "XK_Aacute"         ; *s = "\301"; break;
        case 0x00c2 : *k = XK_Acircumflex    ; *n = "XK_Acircumflex"    ; *s = "\302"; break;
        case 0x00c3 : *k = XK_Atilde         ; *n = "XK_Atilde"         ; *s = "\303"; break;
        case 0x00c4 : *k = XK_Adiaeresis     ; *n = "XK_Adiaeresis"     ; *s = "\304"; break;
        case 0x00c5 : *k = XK_Aring          ; *n = "XK_Aring"          ; *s = "\305"; break;
        case 0x00c6 : *k = XK_AE             ; *n = "XK_AE"             ; *s = "\306"; break;
        case 0x00c7 : *k = XK_Ccedilla       ; *n = "XK_Ccedilla"       ; *s = "\307"; break;
        case 0x00c8 : *k = XK_Egrave         ; *n = "XK_Egrave"         ; *s = "\310"; break;
        case 0x00c9 : *k = XK_Eacute         ; *n = "XK_Eacute"         ; *s = "\311"; break;
        case 0x00ca : *k = XK_Ecircumflex    ; *n = "XK_Ecircumflex"    ; *s = "\312"; break;
        case 0x00cb : *k = XK_Ediaeresis     ; *n = "XK_Ediaeresis"     ; *s = "\313"; break;
        case 0x00cc : *k = XK_Igrave         ; *n = "XK_Igrave"         ; *s = "\314"; break;
        case 0x00cd : *k = XK_Iacute         ; *n = "XK_Iacute"         ; *s = "\315"; break;
        case 0x00ce : *k = XK_Icircumflex    ; *n = "XK_Icircumflex"    ; *s = "\316"; break;
        case 0x00cf : *k = XK_Idiaeresis     ; *n = "XK_Idiaeresis"     ; *s = "\317"; break;
        case 0x00d0 : *k = XK_ETH            ; *n = "XK_ETH"            ; *s = "\320"; break;
        case 0x00d1 : *k = XK_Ntilde         ; *n = "XK_Ntilde"         ; *s = "\321"; break;
        case 0x00d2 : *k = XK_Ograve         ; *n = "XK_Ograve"         ; *s = "\322"; break;
        case 0x00d3 : *k = XK_Oacute         ; *n = "XK_Oacute"         ; *s = "\323"; break;
        case 0x00d4 : *k = XK_Ocircumflex    ; *n = "XK_Ocircumflex"    ; *s = "\324"; break;
        case 0x00d5 : *k = XK_Otilde         ; *n = "XK_Otilde"         ; *s = "\325"; break;
        case 0x00d6 : *k = XK_Odiaeresis     ; *n = "XK_Odiaeresis"     ; *s = "\326"; break;
        case 0x00d7 : *k = XK_multiply       ; *n = "XK_multiply"       ; *s = "\327"; break;
        case 0x00d8 : *k = XK_Oslash         ; *n = "XK_Oslash"         ; *s = "\330"; break;
        case 0x00d9 : *k = XK_Ugrave         ; *n = "XK_Ugrave"         ; *s = "\331"; break;
        case 0x00da : *k = XK_Uacute         ; *n = "XK_Uacute"         ; *s = "\332"; break;
        case 0x00db : *k = XK_Ucircumflex    ; *n = "XK_Ucircumflex"    ; *s = "\333"; break;
        case 0x00dc : *k = XK_Udiaeresis     ; *n = "XK_Udiaeresis"     ; *s = "\334"; break;
        case 0x00dd : *k = XK_Yacute         ; *n = "XK_Yacute"         ; *s = "\335"; break;
        case 0x00de : *k = XK_THORN          ; *n = "XK_THORN"          ; *s = "\336"; break;
        case 0x00df : *k = XK_ssharp         ; *n = "XK_ssharp"         ; *s = "\337"; break;
        case 0x00e0 : *k = XK_agrave         ; *n = "XK_agrave"         ; *s = "\340"; break;
        case 0x00e1 : *k = XK_aacute         ; *n = "XK_aacute"         ; *s = "\341"; break;
        case 0x00e2 : *k = XK_acircumflex    ; *n = "XK_acircumflex"    ; *s = "\342"; break;
        case 0x00e3 : *k = XK_atilde         ; *n = "XK_atilde"         ; *s = "\343"; break;
        case 0x00e4 : *k = XK_adiaeresis     ; *n = "XK_adiaeresis"     ; *s = "\344"; break;
        case 0x00e5 : *k = XK_aring          ; *n = "XK_aring"          ; *s = "\345"; break;
        case 0x00e6 : *k = XK_ae             ; *n = "XK_ae"             ; *s = "\346"; break;
        case 0x00e7 : *k = XK_ccedilla       ; *n = "XK_ccedilla"       ; *s = "\347"; break;
        case 0x00e8 : *k = XK_egrave         ; *n = "XK_egrave"         ; *s = "\350"; break;
        case 0x00e9 : *k = XK_eacute         ; *n = "XK_eacute"         ; *s = "\351"; break;
        case 0x00ea : *k = XK_ecircumflex    ; *n = "XK_ecircumflex"    ; *s = "\352"; break;
        case 0x00eb : *k = XK_ediaeresis     ; *n = "XK_ediaeresis"     ; *s = "\353"; break;
        case 0x00ec : *k = XK_igrave         ; *n = "XK_igrave"         ; *s = "\354"; break;
        case 0x00ed : *k = XK_iacute         ; *n = "XK_iacute"         ; *s = "\355"; break;
        case 0x00ee : *k = XK_icircumflex    ; *n = "XK_icircumflex"    ; *s = "\356"; break;
        case 0x00ef : *k = XK_idiaeresis     ; *n = "XK_idiaeresis"     ; *s = "\357"; break;
        case 0x00f0 : *k = XK_eth            ; *n = "XK_eth"            ; *s = "\360"; break;
        case 0x00f1 : *k = XK_ntilde         ; *n = "XK_ntilde"         ; *s = "\361"; break;
        case 0x00f2 : *k = XK_ograve         ; *n = "XK_ograve"         ; *s = "\362"; break;
        case 0x00f3 : *k = XK_oacute         ; *n = "XK_oacute"         ; *s = "\363"; break;
        case 0x00f4 : *k = XK_ocircumflex    ; *n = "XK_ocircumflex"    ; *s = "\364"; break;
        case 0x00f5 : *k = XK_otilde         ; *n = "XK_otilde"         ; *s = "\365"; break;
        case 0x00f6 : *k = XK_odiaeresis     ; *n = "XK_odiaeresis"     ; *s = "\366"; break;
        case 0x00f7 : *k = XK_division       ; *n = "XK_division"       ; *s = "\367"; break;
        case 0x00f8 : *k = XK_oslash         ; *n = "XK_oslash"         ; *s = "\370"; break;
        case 0x00f9 : *k = XK_ugrave         ; *n = "XK_ugrave"         ; *s = "\371"; break;
        case 0x00fa : *k = XK_uacute         ; *n = "XK_uacute"         ; *s = "\372"; break;
        case 0x00fb : *k = XK_ucircumflex    ; *n = "XK_ucircumflex"    ; *s = "\373"; break;
        case 0x00fc : *k = XK_udiaeresis     ; *n = "XK_udiaeresis"     ; *s = "\374"; break;
        case 0x00fd : *k = XK_yacute         ; *n = "XK_yacute"         ; *s = "\375"; break;
        case 0x00fe : *k = XK_thorn          ; *n = "XK_thorn"          ; *s = "\376"; break;
        case 0x00ff : *k = XK_ydiaeresis     ; *n = "XK_ydiaeresis"     ; *s = "\377"; break;

        /* Symbols still to be processed, please help! */
     /* case ''     : *k = XK_Scroll_Lock    ; *n = "XK_Scroll_Lock"    ; *s = ""; break; */
     /* case ''     : *k = XK_Menu           ; *n = "XK_Menu"           ; *s = ""; break; */
     /* case ''     : *k = XK_Num_Lock       ; *n = "XK_Num_Lock"       ; *s = ""; break; */
     /* case ''     : *k = XK_KP_Enter       ; *n = "XK_KP_Enter"       ; *s = ""; break; */
     /* case ''     : *k = XK_KP_Home        ; *n = "XK_KP_Home"        ; *s = ""; break; */
     /* case ''     : *k = XK_KP_Left        ; *n = "XK_KP_Left"        ; *s = ""; break; */
     /* case ''     : *k = XK_KP_Up          ; *n = "XK_KP_Up"          ; *s = ""; break; */
     /* case ''     : *k = XK_KP_Right       ; *n = "XK_KP_Right"       ; *s = ""; break; */
     /* case ''     : *k = XK_KP_Down        ; *n = "XK_KP_Down"        ; *s = ""; break; */
     /* case ''     : *k = XK_KP_Prior       ; *n = "XK_KP_Prior"       ; *s = ""; break; */
     /* case ''     : *k = XK_KP_Next        ; *n = "XK_KP_Next"        ; *s = ""; break; */
     /* case ''     : *k = XK_KP_End         ; *n = "XK_KP_End"         ; *s = ""; break; */
     /* case ''     : *k = XK_KP_Begin       ; *n = "XK_KP_Begin"       ; *s = ""; break; */
     /* case ''     : *k = XK_KP_Equal       ; *n = "XK_KP_Equal"       ; *s = "="; break; */
     /* case ''     : *k = XK_KP_Multiply    ; *n = "XK_KP_Multiply"    ; *s = "*"; break; */
     /* case ''     : *k = XK_KP_Add         ; *n = "XK_KP_Add"         ; *s = "+"; break; */
     /* case ''     : *k = XK_KP_Separator   ; *n = "XK_KP_Separator"   ; *s = ""; break; */
     /* case ''     : *k = XK_KP_Subtract    ; *n = "XK_KP_Subtract"    ; *s = "-"; break; */
     /* case ''     : *k = XK_KP_Divide      ; *n = "XK_KP_Divide"      ; *s = "/"; break; */
     /* case ''     : *k = XK_KP_0           ; *n = "XK_KP_0"           ; *s = "0"; break; */
     /* case ''     : *k = XK_KP_1           ; *n = "XK_KP_1"           ; *s = "1"; break; */
     /* case ''     : *k = XK_KP_2           ; *n = "XK_KP_2"           ; *s = "2"; break; */
     /* case ''     : *k = XK_KP_3           ; *n = "XK_KP_3"           ; *s = "3"; break; */
     /* case ''     : *k = XK_KP_4           ; *n = "XK_KP_4"           ; *s = "4"; break; */
     /* case ''     : *k = XK_KP_5           ; *n = "XK_KP_5"           ; *s = "5"; break; */
     /* case ''     : *k = XK_KP_6           ; *n = "XK_KP_6"           ; *s = "6"; break; */
     /* case ''     : *k = XK_KP_7           ; *n = "XK_KP_7"           ; *s = "7"; break; */
     /* case ''     : *k = XK_KP_8           ; *n = "XK_KP_8"           ; *s = "8"; break; */
     /* case ''     : *k = XK_KP_9           ; *n = "XK_KP_9"           ; *s = "9"; break; */

     /* case ''     : *k = XK_Caps_Lock      ; *n = "XK_Caps_Lock"      ; *s = ""; break; */
     /* case ''     : *k = XK_Meta_L         ; *n = "XK_Meta_L"         ; *s = ""; break; */
     /* case ''     : *k = XK_Meta_R         ; *n = "XK_Meta_R"         ; *s = ""; break; */
     /* case ''     : *k = XK_Alt_L          ; *n = "XK_Alt_L"          ; *s = ""; break; */
     /* case ''     : *k = XK_Alt_R          ; *n = "XK_Alt_R"          ; *s = ""; break; */

        default :
            if (ez_draw_debug())
                printf ("KEYCODE 0x%x\n", (unsigned) wParam);
            *k = 0; *n = es; *s = es; return -1;
    }

    return 0;
}


const char *ez_win_msg_name (unsigned int m)
{
    switch (m) {
        case 0x0000 : return "WM_NULL";
        case 0x0001 : return "WM_CREATE";
        case 0x0002 : return "WM_DESTROY";
        case 0x0003 : return "WM_MOVE";
        case 0x0005 : return "WM_SIZE";
        case 0x0006 : return "WM_ACTIVATE";
        case 0x0007 : return "WM_SETFOCUS";
        case 0x0008 : return "WM_KILLFOCUS";
        case 0x000A : return "WM_ENABLE";
        case 0x000B : return "WM_SETREDRAW";
        case 0x000C : return "WM_SETTEXT";
        case 0x000D : return "WM_GETTEXT";
        case 0x000E : return "WM_GETTEXTLENGTH";
        case 0x000F : return "WM_PAINT";
        case 0x0010 : return "WM_CLOSE";
        case 0x0011 : return "WM_QUERYENDSESSION";
        case 0x0013 : return "WM_QUERYOPEN";
        case 0x0016 : return "WM_ENDSESSION";
        case 0x0012 : return "WM_QUIT";
        case 0x0014 : return "WM_ERASEBKGND";
        case 0x0015 : return "WM_SYSCOLORCHANGE";
        case 0x0018 : return "WM_SHOWWINDOW";
        case 0x001A : return "WM_WININICHANGE";
        case 0x001B : return "WM_DEVMODECHANGE";
        case 0x001C : return "WM_ACTIVATEAPP";
        case 0x001D : return "WM_FONTCHANGE";
        case 0x001E : return "WM_TIMECHANGE";
        case 0x001F : return "WM_CANCELMODE";
        case 0x0020 : return "WM_SETCURSOR";
        case 0x0021 : return "WM_MOUSEACTIVATE";
        case 0x0022 : return "WM_CHILDACTIVATE";
        case 0x0023 : return "WM_QUEUESYNC";
        case 0x0024 : return "WM_GETMINMAXINFO";
        case 0x0026 : return "WM_PAINTICON";
        case 0x0027 : return "WM_ICONERASEBKGND";
        case 0x0028 : return "WM_NEXTDLGCTL";
        case 0x002A : return "WM_SPOOLERSTATUS";
        case 0x002B : return "WM_DRAWITEM";
        case 0x002C : return "WM_MEASUREITEM";
        case 0x002D : return "WM_DELETEITEM";
        case 0x002E : return "WM_VKEYTOITEM";
        case 0x002F : return "WM_CHARTOITEM";
        case 0x0030 : return "WM_SETFONT";
        case 0x0031 : return "WM_GETFONT";
        case 0x0032 : return "WM_SETHOTKEY";
        case 0x0033 : return "WM_GETHOTKEY";
        case 0x0037 : return "WM_QUERYDRAGICON";
        case 0x0039 : return "WM_COMPAREITEM";
        case 0x003D : return "WM_GETOBJECT";
        case 0x0041 : return "WM_COMPACTING";
        case 0x0044 : return "WM_COMMNOTIFY";
        case 0x0046 : return "WM_WINDOWPOSCHANGING";
        case 0x0047 : return "WM_WINDOWPOSCHANGED";
        case 0x0048 : return "WM_POWER";
        case 0x004A : return "WM_COPYDATA";
        case 0x004B : return "WM_CANCELJOURNAL";
        case 0x004E : return "WM_NOTIFY";
        case 0x0050 : return "WM_INPUTLANGCHANGEREQUEST";
        case 0x0051 : return "WM_INPUTLANGCHANGE";
        case 0x0052 : return "WM_TCARD";
        case 0x0053 : return "WM_HELP";
        case 0x0054 : return "WM_USERCHANGED";
        case 0x0055 : return "WM_NOTIFYFORMAT";
        case 0x007B : return "WM_CONTEXTMENU";
        case 0x007C : return "WM_STYLECHANGING";
        case 0x007D : return "WM_STYLECHANGED";
        case 0x007E : return "WM_DISPLAYCHANGE";
        case 0x007F : return "WM_GETICON";
        case 0x0080 : return "WM_SETICON";
        case 0x0081 : return "WM_NCCREATE";
        case 0x0082 : return "WM_NCDESTROY";
        case 0x0083 : return "WM_NCCALCSIZE";
        case 0x0084 : return "WM_NCHITTEST";
        case 0x0085 : return "WM_NCPAINT";
        case 0x0086 : return "WM_NCACTIVATE";
        case 0x0087 : return "WM_GETDLGCODE";
        case 0x0088 : return "WM_SYNCPAINT";
        case 0x00A0 : return "WM_NCMOUSEMOVE";
        case 0x00A1 : return "WM_NCLBUTTONDOWN";
        case 0x00A2 : return "WM_NCLBUTTONUP";
        case 0x00A3 : return "WM_NCLBUTTONDBLCLK";
        case 0x00A4 : return "WM_NCRBUTTONDOWN";
        case 0x00A5 : return "WM_NCRBUTTONUP";
        case 0x00A6 : return "WM_NCRBUTTONDBLCLK";
        case 0x00A7 : return "WM_NCMBUTTONDOWN";
        case 0x00A8 : return "WM_NCMBUTTONUP";
        case 0x00A9 : return "WM_NCMBUTTONDBLCLK";
        case 0x00AB : return "WM_NCXBUTTONDOWN";
        case 0x00AC : return "WM_NCXBUTTONUP";
        case 0x00AD : return "WM_NCXBUTTONDBLCLK";
        case 0x00FF : return "WM_INPUT";
        case 0x0100 : return "WM_KEYDOWN";
        case 0x0101 : return "WM_KEYUP";
        case 0x0102 : return "WM_CHAR";
        case 0x0103 : return "WM_DEADCHAR";
        case 0x0104 : return "WM_SYSKEYDOWN";
        case 0x0105 : return "WM_SYSKEYUP";
        case 0x0106 : return "WM_SYSCHAR";
        case 0x0107 : return "WM_SYSDEADCHAR";
        case 0x0109 : return "WM_UNICHAR";
        case 0x010D : return "WM_IME_STARTCOMPOSITION";
        case 0x010E : return "WM_IME_ENDCOMPOSITION";
        case 0x010F : return "WM_IME_COMPOSITION";
        case 0x0110 : return "WM_INITDIALOG";
        case 0x0111 : return "WM_COMMAND";
        case 0x0112 : return "WM_SYSCOMMAND";
        case 0x0113 : return "WM_TIMER";
        case 0x0114 : return "WM_HSCROLL";
        case 0x0115 : return "WM_VSCROLL";
        case 0x0116 : return "WM_INITMENU";
        case 0x0117 : return "WM_INITMENUPOPUP";
        case 0x011F : return "WM_MENUSELECT";
        case 0x0120 : return "WM_MENUCHAR";
        case 0x0121 : return "WM_ENTERIDLE";
        case 0x0122 : return "WM_MENURBUTTONUP";
        case 0x0123 : return "WM_MENUDRAG";
        case 0x0124 : return "WM_MENUGETOBJECT";
        case 0x0125 : return "WM_UNINITMENUPOPUP";
        case 0x0126 : return "WM_MENUCOMMAND";
        case 0x0127 : return "WM_CHANGEUISTATE";
        case 0x0128 : return "WM_UPDATEUISTATE";
        case 0x0129 : return "WM_QUERYUISTATE";
        case 0x0132 : return "WM_CTLCOLORMSGBOX";
        case 0x0133 : return "WM_CTLCOLOREDIT";
        case 0x0134 : return "WM_CTLCOLORLISTBOX";
        case 0x0135 : return "WM_CTLCOLORBTN";
        case 0x0136 : return "WM_CTLCOLORDLG";
        case 0x0137 : return "WM_CTLCOLORSCROLLBAR";
        case 0x0138 : return "WM_CTLCOLORSTATIC";
        case 0x0200 : return "WM_MOUSEMOVE";
        case 0x0201 : return "WM_LBUTTONDOWN";
        case 0x0202 : return "WM_LBUTTONUP";
        case 0x0203 : return "WM_LBUTTONDBLCLK";
        case 0x0204 : return "WM_RBUTTONDOWN";
        case 0x0205 : return "WM_RBUTTONUP";
        case 0x0206 : return "WM_RBUTTONDBLCLK";
        case 0x0207 : return "WM_MBUTTONDOWN";
        case 0x0208 : return "WM_MBUTTONUP";
        case 0x0209 : return "WM_MBUTTONDBLCLK";
        case 0x020A : return "WM_MOUSEWHEEL";
        case 0x020B : return "WM_XBUTTONDOWN";
        case 0x020C : return "WM_XBUTTONUP";
        case 0x020D : return "WM_XBUTTONDBLCLK";
        case 0x0210 : return "WM_PARENTNOTIFY";
        case 0x0211 : return "WM_ENTERMENULOOP";
        case 0x0212 : return "WM_EXITMENULOOP";
        case 0x0213 : return "WM_NEXTMENU";
        case 0x0214 : return "WM_SIZING";
        case 0x0215 : return "WM_CAPTURECHANGED";
        case 0x0216 : return "WM_MOVING";
        case 0x0218 : return "WM_POWERBROADCAST";
        case 0x0220 : return "WM_MDICREATE";
        case 0x0221 : return "WM_MDIDESTROY";
        case 0x0222 : return "WM_MDIACTIVATE";
        case 0x0223 : return "WM_MDIRESTORE";
        case 0x0224 : return "WM_MDINEXT";
        case 0x0225 : return "WM_MDIMAXIMIZE";
        case 0x0226 : return "WM_MDITILE";
        case 0x0227 : return "WM_MDICASCADE";
        case 0x0228 : return "WM_MDIICONARRANGE";
        case 0x0229 : return "WM_MDIGETACTIVE";
        case 0x0230 : return "WM_MDISETMENU";
        case 0x0231 : return "WM_ENTERSIZEMOVE";
        case 0x0232 : return "WM_EXITSIZEMOVE";
        case 0x0233 : return "WM_DROPFILES";
        case 0x0234 : return "WM_MDIREFRESHMENU";
        case 0x0281 : return "WM_IME_SETCONTEXT";
        case 0x0282 : return "WM_IME_NOTIFY";
        case 0x0283 : return "WM_IME_CONTROL";
        case 0x0284 : return "WM_IME_COMPOSITIONFULL";
        case 0x0285 : return "WM_IME_SELECT";
        case 0x0286 : return "WM_IME_CHAR";
        case 0x0288 : return "WM_IME_REQUEST";
        case 0x0290 : return "WM_IME_KEYDOWN";
        case 0x0291 : return "WM_IME_KEYUP";
        case 0x02A1 : return "WM_MOUSEHOVER";
        case 0x02A3 : return "WM_MOUSELEAVE";
        case 0x02A0 : return "WM_NCMOUSEHOVER";
        case 0x02A2 : return "WM_NCMOUSELEAVE";
        case 0x02B1 : return "WM_WTSSESSION_CHANGE";
        case 0x02c0 : return "WM_TABLET_FIRST";
        case 0x02df : return "WM_TABLET_LAST";
        case 0x0300 : return "WM_CUT";
        case 0x0301 : return "WM_COPY";
        case 0x0302 : return "WM_PASTE";
        case 0x0303 : return "WM_CLEAR";
        case 0x0304 : return "WM_UNDO";
        case 0x0305 : return "WM_RENDERFORMAT";
        case 0x0306 : return "WM_RENDERALLFORMATS";
        case 0x0307 : return "WM_DESTROYCLIPBOARD";
        case 0x0308 : return "WM_DRAWCLIPBOARD";
        case 0x0309 : return "WM_PAINTCLIPBOARD";
        case 0x030A : return "WM_VSCROLLCLIPBOARD";
        case 0x030B : return "WM_SIZECLIPBOARD";
        case 0x030C : return "WM_ASKCBFORMATNAME";
        case 0x030D : return "WM_CHANGECBCHAIN";
        case 0x030E : return "WM_HSCROLLCLIPBOARD";
        case 0x030F : return "WM_QUERYNEWPALETTE";
        case 0x0310 : return "WM_PALETTEISCHANGING";
        case 0x0311 : return "WM_PALETTECHANGED";
        case 0x0312 : return "WM_HOTKEY";
        case 0x0317 : return "WM_PRINT";
        case 0x0318 : return "WM_PRINTCLIENT";
        case 0x0319 : return "WM_APPCOMMAND";
        case 0x031A : return "WM_THEMECHANGED";
        case 0x0358 : return "WM_HANDHELDFIRST";
        case 0x035F : return "WM_HANDHELDLAST";
        case 0x0360 : return "WM_AFXFIRST";
        case 0x037F : return "WM_AFXLAST";
        case 0x0380 : return "WM_PENWINFIRST";
        case 0x038F : return "WM_PENWINLAST";
        case 0x8000 : return "WM_APP";
        case EZ_MSG_PAINT : return "EZ_MSG_PAINT";
    }
    return "*** UNKNOWN ***";
}

#endif /* EZ_BASE_ */
/// @endcond Document_C_source
#include "ez-draw++.hpp"

// Beginning of ez-draw++.cpp

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <cstdlib>

static EZDraw *ezDraw=nullptr;

static void nettoyage()
{
 delete ezDraw; // detruit l'objet EZDraw s'il existe encore (aucun effet sinon).
 ezDraw=nullptr;
}

std::ostream& operator<<(std::ostream& os,const EZKeySym& k)
{
 os << (int) k;
 return os;
}

////////////////////////// class EZDrawPrivate

 /// @cond Private_implantation

class EZDrawPrivate {
 static std::map<Ez_window,EZWindow*> *ctocpp;
 static std::map<const EZWindow*,Ez_window> *cpptoc;
 static void newMaps()
  {
   ctocpp = new std::map<Ez_window,EZWindow*>;
   cpptoc = new std::map<const EZWindow*,Ez_window>;
  }
 static void deleteMaps()
  {
   delete ctocpp; ctocpp = nullptr;
   delete cpptoc; cpptoc = nullptr;
  }
 static void associate(Ez_window ezw,EZWindow* EZw)
  {
 // std::cerr << "EZWindow::EZWindow() { window=" << window << " this=" << this << std::endl << "Before:" << std::endl;
 // for (auto it = ezDraw->windows.cbegin(); it != ezDraw->windows.cend(); ++it)
 //   std::cout << " [" << (*it).first << ':' << (*it).second << ']' << std::endl;
 (*ctocpp)[ezw]=EZw;
 (*cpptoc)[EZw]=ezw;
 // std::pair<EZDraw::WindowMap::iterator,bool> ret = ezDraw->windows.insert(EZDraw::WindowMap::value_type(window,this));
 // std::cerr << "ret.second=" << ret.second << std::endl;
 // for (auto it = ezDraw->windows.cbegin(); it != ezDraw->windows.cend(); ++it)
 //   std::cout << " [" << (*it).first << ':' << (*it).second << ']' << std::endl;
 // std::cerr << "}" << std::endl;
  }
 static Ez_window remove(EZWindow* w)
  {
   auto it = cpptoc->find(w);
   if(it != cpptoc->end() )
    {
     Ez_window res = it->second;
     ctocpp->erase(res);
     cpptoc->erase(it);
     return res;
    }
   else
    throw EZDrawError("EZDrawPrivate::remove(): Ceci ne devrait jamais arriver.");
  }
public:
 static Ez_window recover(const EZWindow* const w)
  {
   auto it = cpptoc->find(w);
   if(it != cpptoc->end() ) return it->second;
   else throw EZDrawError("Ez_window EZDrawPrivate::recoverEZWindow(EZWindow* w): Ceci ne devrait jamais arriver.");
  }
 static EZWindow* recover(Ez_window w)
  {
   auto it = ctocpp->find(w);
   if(it != ctocpp->end() ) return it->second;
   else return nullptr;
  }

 friend class EZDraw;
 friend class EZWindow;
};

std::map<Ez_window,EZWindow*>   *EZDrawPrivate::ctocpp(nullptr);
std::map<const EZWindow*,Ez_window>   *EZDrawPrivate::cpptoc(nullptr);

EZEvent::EZEvent(const Ez_event *e)
 : ez_event(e)
{}
 /// @endcond

int          EZEvent::mouseX() const      { return ez_event->mx; }
int          EZEvent::mouseY() const      { return ez_event->my; }
int          EZEvent::mouseButton() const { return ez_event->mb; }
EZKeySym     EZEvent::keySym() const      { return (EZKeySym)ez_event->key_sym; }
std::string  EZEvent::keyName() const
{
 std::string res(ez_event->key_name);
 std::size_t start = res.find("XK_");
 if(start!=std::string::npos)
   res.replace(0,3,"EZEvent::");
 return res;
}

size_t       EZEvent::keyCount() const   { return ez_event->key_count; }
const char * EZEvent::keyString() const   { return ez_event->key_string; }


/// Construit une nouvelle fenêtre.
///
EZWindow::EZWindow(int width,int height,const char *title)
 : current_event(nullptr),_isVisible(true)
{
 if(ezDraw==nullptr)
  {
   std::cerr <<
    "Vous n'avez pas cree l'instance de la classe EZDraw avant de creer votre premiere EZWindow !"
    " RELISEZ la documentation !" << std::endl;
   ezDraw = new EZDraw();
   atexit(nettoyage);
  }
 Ez_window window = ez_window_create (width, height, title,EZWindow::dispatch);
 // std::cerr << "EZWindow::EZWindow() { window=" << window << " this=" << this << std::endl << "Before:" << std::endl;
 // for (auto it = ezDraw->windows.cbegin(); it != ezDraw->windows.cend(); ++it)
 //   std::cout << " [" << (*it).first << ':' << (*it).second << ']' << std::endl;
 EZDrawPrivate::associate(window,this);
 // std::pair<EZDraw::WindowMap::iterator,bool> ret = ezDraw->windows.insert(EZDraw::WindowMap::value_type(window,this));
 // std::cerr << "ret.second=" << ret.second << std::endl;
 // for (auto it = ezDraw->windows.cbegin(); it != ezDraw->windows.cend(); ++it)
 //   std::cout << " [" << (*it).first << ':' << (*it).second << ']' << std::endl;
 // std::cerr << "}" << std::endl;
}

EZWindow::~EZWindow()
{
 Ez_window window = EZDrawPrivate::remove(this);
 ez_window_destroy(window);
}

void          EZWindow::setDoubleBuffer (bool state)
 { ez_window_dbuf(EZDrawPrivate::recover(this), state); }
void          EZWindow::setWidth     (int w)
 { ez_window_set_size(EZDrawPrivate::recover(this),w,getHeight()); }
void          EZWindow::setHeight    (int h)
 { ez_window_set_size(EZDrawPrivate::recover(this),getWidth(),h); }
void          EZWindow::setWidthHeight(int w,int h)
 { ez_window_set_size(EZDrawPrivate::recover(this),w,h); }
int           EZWindow::getWidth     () const
 { int w,h; ez_window_get_size(EZDrawPrivate::recover(this),&w,&h); return w; }
int           EZWindow::getHeight    () const
 { int w,h; ez_window_get_size(EZDrawPrivate::recover(this),&w,&h); return h; }
void          EZWindow::setVisible   (bool visible)
 { ez_window_show(EZDrawPrivate::recover(this),(visible?1:0)); _isVisible = ! _isVisible; }
void          EZWindow::setColor     (EZColor color)
 { ez_set_color(color.value); }
void          EZWindow::setThick     (int thick)
 { ez_set_thick(thick); }
void          EZWindow::loadFont     (int num,const char *name)
 { if( ez_font_load(num,name)!=0) throw EZDrawError("EZWindow::loadFont() a rencontré un problème en chargeant la police demandé."); }
void          EZWindow::setFont      (int num)
 { ez_set_nfont(num); }

void          EZWindow::reset        () const
 { ez_window_reset(EZDrawPrivate::recover(this)); }
void          EZWindow::clear        (EZColor color) const
 { ez_window_clear(EZDrawPrivate::recover(this),color.value); }
void          EZWindow::drawPoint    (int x1, int y1) const
 { ez_draw_point(EZDrawPrivate::recover(this),x1,y1); }
void          EZWindow::drawLine     (int x1, int y1, int x2, int y2) const
 { ez_draw_line(EZDrawPrivate::recover(this),x1,y1,x2,y2); }
void          EZWindow::drawRectangle(int x1, int y1, int x2, int y2) const
 { ez_draw_rectangle(EZDrawPrivate::recover(this),x1,y1,x2,y2); }
void          EZWindow::fillRectangle(int x1, int y1, int x2, int y2) const
 { ez_fill_rectangle(EZDrawPrivate::recover(this),x1,y1,x2,y2); }
void          EZWindow::drawCircle   (int x1, int y1, int x2, int y2) const
 { ez_draw_circle(EZDrawPrivate::recover(this),x1,y1,x2,y2); }
void          EZWindow::fillCircle   (int x1, int y1, int x2, int y2) const
 { ez_fill_circle(EZDrawPrivate::recover(this),x1,y1,x2,y2); }
void          EZWindow::drawTriangle (int x1, int y1, int x2, int y2, int x3, int y3     ) const
 { ez_draw_triangle(EZDrawPrivate::recover(this),x1,y1,x2,y2,x3,y3); }
void          EZWindow::fillTriangle (int x1, int y1, int x2, int y2, int x3, int y3     ) const
 { ez_fill_triangle(EZDrawPrivate::recover(this),x1,y1,x2,y2,x3,y3); }
void          EZWindow::drawText     (EZAlign align,int x1,int y1,const std::string &str) const
 { ez_draw_text(EZDrawPrivate::recover(this),(Ez_Align)align,x1,y1,str.c_str()); }
void          EZWindow::drawText     (EZAlign align,int x1,int y1,const char *str)        const
 { ez_draw_text(EZDrawPrivate::recover(this),(Ez_Align)align,x1,y1,str); }
void          EZWindow::startTimer   (unsigned int delay) const
 { ez_start_timer(EZDrawPrivate::recover(this),(int) delay); }
void          EZWindow::stopTimer    () const
 { ez_start_timer(EZDrawPrivate::recover(this),-1);}

 // Evenements
void          EZWindow::sendExpose   () const
 { ez_send_expose(EZDrawPrivate::recover(this)); } // Force le rafraichissement du contenu de la fenetre

void          EZWindow::expose       ()                                               {}
void          EZWindow::close        ()                                               {}
void          EZWindow::buttonPress  (int /*mouse_x*/,int /*mouse_y*/,int /*button*/) {}
void          EZWindow::buttonRelease(int /*mouse_x*/,int /*mouse_y*/,int /*button*/) {}
void          EZWindow::motionNotify (int /*mouse_x*/,int /*mouse_y*/,int /*button*/) {}
void          EZWindow::keyPress     (EZKeySym /*keysym*/)                            {}
void          EZWindow::keyRelease   (EZKeySym /*keysym*/)                            {}
void          EZWindow::configureNotify(int /*with*/,int /*height*/)                  {}
void          EZWindow::timerNotify  ()                                               {}

////////////////////////// class EZDraw
EZDrawError::EZDrawError(const char *errmsg)
 : std::runtime_error(errmsg)
{}

EZColor EZColor::black,
        EZColor::white,
        EZColor::grey,
        EZColor::red,
        EZColor::green,
        EZColor::blue,
        EZColor::yellow,
        EZColor::cyan,
        EZColor::magenta;

void EZColor::init()
{
 EZColor::black.value   = ez_black;
 EZColor::white.value   = ez_white;
 EZColor::grey.value    = ez_grey;
 EZColor::red.value     = ez_red;
 EZColor::green.value   = ez_green;
 EZColor::blue.value    = ez_blue;
 EZColor::cyan.value    = ez_cyan;
 EZColor::magenta.value = ez_magenta;
 EZColor::yellow.value  = ez_yellow;

#if defined EZ_BASE_XLIB
  if(!(EZColor::red.value==0xff0000 && EZColor::blue.value==0x0000ff))
    std::cerr << "Vous avez réussi à trouver une architecture X11 où les couleurs ne sont pas stockées en 24 bits RGB big-endian. Les accesseurs de consultation et modification des composantes de couleur ne vont pas marcher correctement. Prévenez-moi (E. Remy)." << std::endl;
#elif defined EZ_BASE_WIN32
  if(!(EZColor::red.value==0x0000ff && EZColor::blue.value==0xff0000))
    std::cerr << "Vous avez réussi à trouver une architecture Windows où les couleurs ne sont pas stockées en 24 bits RGB little-endian. Les accesseurs de consultation et modification des composantes de couleur ne vont pas marcher correctement. Prévenez-moi (E. Remy). red:" << std::hex << EZColor::red.value << " blue:" << std::hex << EZColor::blue.value << std::endl;
#else
    std::cerr << "Contactez-moi (E. Remy) pour m'indiquer sur quelle machine (architecture+système) vous tentez de compiler EZDraw++." << std::endl;
#endif
}

EZColor::EZColor(EZuint8 grey)
 : value(ez_get_grey(grey))
{}

EZColor::EZColor(EZuint8 red,EZuint8 green,EZuint8 blue)
 : value(ez_get_RGB(red,green,blue))
{}

EZColor::EZColor(double hue,double saturation,double _value)
 : value(ez_get_HSV(hue,saturation,_value))
{}

#if defined EZ_BASE_XLIB
EZuint8 EZColor::getRed()   const { return (value&0xFF0000)>>16; }
EZuint8 EZColor::getGreen() const { return (value&0x00FF00)>> 8; }
EZuint8 EZColor::getBlue()  const { return (value&0x0000FF)    ; }
void EZColor::setRed  (EZuint8 red)   { value &= 0x00FFFF; value |= (  red<<16); }
void EZColor::setGreen(EZuint8 green) { value &= 0xFF00FF; value |= (green<< 8); }
void EZColor::setBlue (EZuint8 blue)  { value &= 0xFFFF00; value |= ( blue    ); }
#elif defined EZ_BASE_WIN32
EZuint8 EZColor::getRed()   const { return (value&0x0000FF)    ; }
EZuint8 EZColor::getGreen() const { return (value&0x00FF00)>> 8; }
EZuint8 EZColor::getBlue()  const { return (value&0xFF0000)>>16; }
void EZColor::setRed  (EZuint8 red)   { value &= 0xFFFF00; value |= ( red     ); }
void EZColor::setGreen(EZuint8 green) { value &= 0xFF00FF; value |= (green<< 8); }
void EZColor::setBlue (EZuint8 blue)  { value &= 0x00FFFF; value |= ( blue<<16); }
#endif

std::ostream& operator<<(std::ostream& os,const EZColor& col)
{
  std::ostringstream oss;
  oss << "#"
     << std::hex << std::setfill('0')
     << std::setw(2) << int(col.getRed())
     << std::setw(2) << int(col.getGreen())
     << std::setw(2) << int(col.getBlue());
  os << oss.str();
  return os;
}

static inline EZuint8 convert_char_pair(char c1,char c2)
{
  EZuint8 res=0;
  res|=('0'<=c1&&c1<='9'?c1-'0':('a'<=c1&&c1<='f'?10+c1-'a':('A'<=c1&&c1<='F'?10+c1-'A':0)));
  res<<=4;
  res|=('0'<=c2&&c2<='9'?c2-'0':('a'<=c2&&c2<='f'?10+c2-'a':('A'<=c2&&c2<='F'?10+c2-'A':0)));
  return res;
}
std::istream& operator>>(std::istream& is,      EZColor& col)
{
  char c=' ';
  is >> c;
  if(c=='#')
  {
    std::string str;
    is>>str;
    if(str.length()!=6)
      throw EZDrawError("La chaîne de caractère décrivant la couleur devrait être formée de 6 caractères !");
    col.setRed  (convert_char_pair(str[0],str[1]));
    col.setGreen(convert_char_pair(str[2],str[3]));
    col.setBlue (convert_char_pair(str[4],str[5]));
  }
  else
  { // old behavior
    is.putback(c);
    is >> col.value;
  }
  return is;
}

EZDraw::EZDraw()
{
 if(ezDraw != nullptr)
  { const char * errmsg="Ne creez pas plus d'une instance de EZDraw !\nRELISEZ la documentation ! " ; std::cerr << errmsg << std::endl; throw EZDrawError(errmsg); }

 if(ez_init()<0)
  throw EZDrawError("Can't initialize EZ-Draw (error during ez_init()).");

 ezDraw = this;
 EZDrawPrivate::newMaps();
 ez_error_handler = EZDraw::error_handler;
 EZColor::init();
}

EZDraw::~EZDraw()
{
 if(ezDraw != this) std::cerr << "Comment diable avez-vous pu produire ce message ! Envoyez d'urgence votre programme source a eric.remy@univ-amu.fr ! Merci." << std::endl;
 EZDrawPrivate::deleteMaps();
 ezDraw=nullptr;
}

 /// @cond Private_implantation
int EZDraw::error_handler(const char *fmt, va_list ap)
{
  constexpr size_t ERRMAX = 1024;
  char errmsg[ERRMAX];
  int res = vsnprintf (errmsg,ERRMAX, fmt, ap);
  std::string errmsg2="Erreur dans la sous-couche ez-draw : ";
  errmsg2+=errmsg;
  throw EZDrawError(errmsg2.c_str());
  return res;
}

void EZWindow::dispatch(struct sEz_event *ev)
{
 EZWindow *pwin = EZDrawPrivate::recover(ev->win);

 if(pwin == nullptr)
  {
    //std::cerr << "Aborting: pwin==NULL in " <<  __func__ << std::endl;
    //std::cerr << "ev->win=" << ev->win << std::endl;
    // Ne pas afficher d'erreur ici car sous Windows la fenêtre peut recevoir des évènements avant même qu'elle ait été créée et rajouter au std::map qui sert à lui attacher son adresse : on passe donc une fois par ici avant même d'avoir enregistré le pointeur dans le map au début de l'existance de la fenêtre en question !
    return ;
  }
 pwin->current_event.setEvent(ev);
 switch(ev->type)
  {
   case Expose:          pwin->expose(); break;
   case WindowClose:     pwin->close();  break;
   case ButtonPress:     pwin->buttonPress(ev->mx,ev->my,ev->mb);   break;
   case ButtonRelease:   pwin->buttonRelease(ev->mx,ev->my,ev->mb); break;
   case MotionNotify:    pwin->motionNotify(ev->mx,ev->my,ev->mb);  break;
   case KeyPress:        pwin->keyPress((EZKeySym)ev->key_sym);     break;
   case KeyRelease:      pwin->keyRelease((EZKeySym)ev->key_sym);   break;
   case ConfigureNotify: pwin->configureNotify(ev->width,ev->height); break;
   case TimerNotify:     pwin->timerNotify(); break;
   default :
     {
      std::ostringstream oss;
      oss << __FILE__<<":"<<__LINE__<<": Evenement non gere : "<< ev->type;
      throw EZDrawError(oss.str().c_str());
     }
  }
}
 /// @endcond

void   EZDraw::quit() { ez_quit(); }

void   EZDraw::setAutoQuit(bool value) { ez_auto_quit(int(value)); }

void   EZDraw::mainLoop() { ez_main_loop(); }

int    EZDraw::random(int n) { return ez_random(n); }

double EZDraw::getTime() { return ez_get_time(); }
/// @cond Document_C_source
/*
 * ez-image.h: EZ-Draw module to load and draw images
 *
 * Edouard.Thiel@lif.univ-mrs.fr - 12/05/2013 - version 1.2
 * Benoit.Favre@lif.univ-mrs.fr
 *
 * This program is free software under the terms of the
 * GNU Lesser General Public License (LGPL) version 2.1.
*/

#ifndef EZ_IMAGE__H
#define EZ_IMAGE__H

//REMOVED FOR EZ-DRAW++ : #include "ez-draw.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


typedef struct sEz_image {
    int width, height;
    EZuint8 *pixels_rgba;
    int has_alpha;
    int opacity;
} Ez_image;

typedef struct sEz_pixmap {
    int width, height;
#ifdef EZ_BASE_XLIB
    Pixmap map, mask;
#elif defined EZ_BASE_WIN32
    HBITMAP hmap;
    int has_alpha;
#endif /* EZ_BASE_ */
} Ez_pixmap;


/* Public functions */

Ez_image *ez_image_new (void);
void ez_image_destroy (Ez_image *img);
Ez_image *ez_image_create (int w, int h);
Ez_image *ez_image_dup (Ez_image *img);
Ez_image *ez_image_load (const char *filename);

void ez_image_set_alpha (Ez_image *img, int has_alpha);
int  ez_image_has_alpha (Ez_image *img);
void ez_image_set_opacity (Ez_image *img, int opacity);
int  ez_image_get_opacity (Ez_image *img);

void ez_image_paint (Ez_window win, Ez_image *img, int x, int y);
void ez_image_paint_sub (Ez_window win, Ez_image *img, int x, int y,
    int src_x, int src_y, int w, int h);
void ez_image_print (Ez_image *img, int src_x, int src_y, int w, int h);

void ez_image_fill_rgba (Ez_image *img, EZuint8 r, EZuint8 g, EZuint8 b, EZuint8 a);
void ez_image_blend (Ez_image *dst, Ez_image *src, int dst_x, int dst_y);
void ez_image_blend_sub (Ez_image *dst, Ez_image *src, int dst_x, int dst_y,
    int src_x, int src_y, int w, int h);

Ez_image *ez_image_extract (Ez_image *img, int src_x, int src_y, int w, int h);
Ez_image *ez_image_sym_ver (Ez_image *img);
Ez_image *ez_image_sym_hor (Ez_image *img);
Ez_image *ez_image_scale (Ez_image *img, double factor);
Ez_image *ez_image_rotate (Ez_image *img, double theta, int quality);
void ez_image_rotate_point (Ez_image *img, double theta, int src_x, int src_y,
    int *dst_x, int *dst_y);

Ez_pixmap *ez_pixmap_new (void);
void ez_pixmap_destroy (Ez_pixmap *pix);
Ez_pixmap *ez_pixmap_create_from_image (Ez_image *img);
void ez_pixmap_paint (Ez_window win, Ez_pixmap *pix, int x, int y);
void ez_pixmap_tile (Ez_window win, Ez_pixmap *pix, int x, int y, int w, int h);


/* Private functions */
#ifdef EZ_PRIVATE_DEFS

int ez_image_debug (void);

int ez_image_confine_sub_coords (Ez_image *img, int *src_x, int *src_y,
    int *w, int *h);
int ez_confine_coord (int *t, int *r, int tmax);

#ifdef EZ_BASE_XLIB

void ez_image_draw_pict (Ez_window win, Ez_image *img, int x, int y,
    int src_x, int src_y, int w, int h);

typedef void (*ez_xi_func)(XImage *, Ez_image *, int, int, int, int);

void ez_image_draw_xi (Ez_window win, Ez_image *img, int x, int y,
    int src_x, int src_y, int w, int h);
XImage *ez_xi_create (Ez_image *img, int src_x, int src_y, int w, int h,
    ez_xi_func xi_func);
ez_xi_func ez_xi_get_func (void);
void ez_xi_fill_default (XImage *xi, Ez_image *img, int src_x, int src_y,
    int w, int h);
void ez_xi_fill_24 (XImage *xi, Ez_image *img, int src_x, int src_y,
    int w, int h);
Ez_image *ez_xi_test_create (void);
int ez_xi_diff (XImage *xi1, XImage *xi2);

Pixmap ez_xmask_create (Ez_window win, Ez_image *img, int src_x, int src_y,
    int w, int h);
void ez_xmask_fill (EZuint8 *data, Ez_image *img,
    int src_x, int src_y, int w, int h);

#elif defined EZ_BASE_WIN32

void ez_image_draw_dib (HDC hdc_dst, Ez_image *img, int x, int y,
    int src_x, int src_y, int w, int h);
void ez_dib_fill_noalpha (EZuint8 *data, Ez_image *img, int src_x, int src_y,
    int w, int h);
void ez_dib_fill_opacity (EZuint8 *data, Ez_image *img, int src_x, int src_y,
    int w, int h);
void ez_dib_fill_truealpha (EZuint8 *data, Ez_image *img, int src_x, int src_y,
    int w, int h);

#endif /* EZ_BASE_ */

void ez_image_print_rgba (Ez_image *img, int src_x, int src_y, int w, int h);
void ez_image_comp_fill_rgba (Ez_image *img, EZuint8 r, EZuint8 g, EZuint8 b,
    EZuint8 a);
void ez_image_comp_over (Ez_image *dst, Ez_image *src, int dst_x, int dst_y,
    int src_x, int src_y, int w, int h);
void ez_image_comp_blend (Ez_image *dst, Ez_image *src, int dst_x, int dst_y,
    int src_x, int src_y, int w, int h);

void ez_image_copy_sub (Ez_image *src, Ez_image *dest , int src_x, int src_y);
void ez_image_comp_symv (Ez_image *src, Ez_image *dst);
void ez_image_comp_symh (Ez_image *src, Ez_image *dst);
void ez_image_expand (Ez_image *src, Ez_image *dst, double factor);
void ez_image_shrink (Ez_image *src, Ez_image *dst, double factor);
void ez_rotate_get_size (double theta, int src_w, int src_h, int *dst_w, int *dst_h);
void ez_rotate_get_coords (double theta, int src_w, int src_h, int src_x, int src_y,
    int *dst_x, int *dst_y);
void ez_image_rotate_nearest (Ez_image *src, Ez_image *dst, double theta);
void ez_image_rotate_bilinear (Ez_image *src, Ez_image *dst, double theta);
void ez_bilinear_4points (EZuint8 *src_p, EZuint8 *dst_p,
    int src_w, int src_h, double sx, double sy, int t);
void ez_bilinear_pane (EZuint8 *src_p, EZuint8 *dst_p,
    int src_w, int src_h, double sx, double sy, int t, double factor);

#ifdef EZ_BASE_XLIB
int ez_pixmap_build_map (Ez_pixmap *pix, Ez_image *img);
void ez_pixmap_draw_area (Ez_window win, Ez_pixmap *pix, int x, int y);
void ez_pixmap_tile_area (Ez_window win, Ez_pixmap *pix, int x, int y, int w, int h);
#elif defined EZ_BASE_WIN32
int ez_pixmap_build_hmap (Ez_pixmap *pix, Ez_image *img);
void ez_pixmap_draw_hmap (HDC hdc_dst, Ez_pixmap *pix, int x, int y);
void ez_pixmap_tile_hmap (HDC hdc_dst, Ez_pixmap *pix, int x, int y, int w, int h);
#endif /* EZ_BASE_ */

#endif /* EZ_PRIVATE_DEFS */


/*---------------------------------------------------------------------------
 *
 * Image files loading.
 *
 * Code extracted from stb_image.c version 1.33 - public domain JPEG/PNG reader
 * Original source and authors:  http://nothings.org/stb_image.c
 *
 * Supported formats:
 *    JPEG baseline (no JPEG progressive)
 *    PNG 8-bit-per-channel only
 *    BMP non-1bpp, non-RLE
 *    GIF (*comp always reports as 4-channel)
 *
 * Main contributors (see original sources):
 *    Sean Barrett (jpeg, png, bmp)
 *    Jean-Marc Lienher (gif)
*/

/* To test with the original file stb_image.c , define USE_ORIGINAL_STBI */
#ifndef USE_ORIGINAL_STBI

enum {
    EZ_STBI_DEFAULT    = 0, /* only used for req_comp */
    EZ_STBI_GREY       = 1,
    EZ_STBI_GREY_ALPHA = 2,
    EZ_STBI_RGB        = 3,
    EZ_STBI_RGB_ALPHA  = 4
};

/* Load image by filename, open file, or memory buffer */
EZuint8 *ez_stbi_load_from_memory (EZuint8 const *buffer, int len, int *x, int *y, int *comp, int req_comp);
EZuint8 *ez_stbi_load (char const *filename, int *x, int *y, int *comp, int req_comp);
EZuint8 *ez_stbi_load_from_file (FILE *f, int *x, int *y, int *comp, int req_comp);
/* for ez_stbi_load_from_file, file pointer is left pointing immediately after image */

typedef struct {
    /* Fill 'data' with 'size' bytes. Return number of bytes actually read */
    int (*read) (void *user, char *data, int size);
    /* Skip the next 'n' bytes */
    void (*skip) (void *user, unsigned n);
    /* Return nonzero if we are at end of file/data */
    int (*eof) (void *user);
} Ez_stbi_io_callbacks;

EZuint8 *ez_stbi_load_from_callbacks (Ez_stbi_io_callbacks const *clbk, void *user, int *x, int *y, int *comp, int req_comp);

/* Free the loaded image -- this is just free () */
void ez_stbi_image_free (void *retval_from_Ez_stbi_load);

/* Get image dimensions and components without fully decoding */
int ez_stbi_info_from_memory (EZuint8 const *buffer, int len, int *x, int *y, int *comp);
int ez_stbi_info_from_callbacks (Ez_stbi_io_callbacks const *clbk, void *user, int *x, int *y, int *comp);
int ez_stbi_info (char const *filename, int *x, int *y, int *comp);
int ez_stbi_info_from_file (FILE *f, int *x, int *y, int *comp);

/* Zlib client - used by PNG, available for other purposes */
char *ez_stbi_zlib_decode_malloc_guesssize (const char *buffer, int len, int initial_size, int *outlen);
char *ez_stbi_zlib_decode_malloc (const char *buffer, int len, int *outlen);
int   ez_stbi_zlib_decode_buffer (char *obuffer, int olen, const char *ibuffer, int ilen);
char *ez_stbi_zlib_decode_noheader_malloc (const char *buffer, int len, int *outlen);
int   ez_stbi_zlib_decode_noheader_buffer (char *obuffer, int olen, const char *ibuffer, int ilen);

#else /* USE_ORIGINAL_STBI */

#define STBI_HEADER_FILE_ONLY
#include "stb_image.c"
#undef STBI_HEADER_FILE_ONLY

#define EZ_STBI_DEFAULT     STBI_default
#define EZ_STBI_GREY        STBI_grey
#define EZ_STBI_GREY_ALPHA  STBI_grey_alpha
#define EZ_STBI_RGB         STBI_rgb
#define EZ_STBI_RGB_ALPHA   STBI_rgb_alpha

#define ez_stbi_load_from_memory    stbi_load_from_memory
#define ez_stbi_load                stbi_load
#define ez_stbi_load_from_file      stbi_load_from_file

#define Ez_stbi_io_callbacks        stbi_io_callbacks
#define ez_stbi_load_from_callbacks stbi_load_from_callbacks

#define ez_stbi_image_free          stbi_image_free

#define ez_stbi_info_from_memory    stbi_info_from_memory
#define ez_stbi_info_from_callbacks stbi_info_from_callbacks
#define ez_stbi_info                stbi_info
#define ez_stbi_info_from_file      stbi_info_from_file

#define ez_stbi_zlib_decode_malloc_guesssize    stbi_zlib_decode_malloc_guesssize
#define ez_stbi_zlib_decode_malloc              stbi_zlib_decode_malloc
#define ez_stbi_zlib_decode_buffer              stbi_zlib_decode_buffer
#define ez_stbi_zlib_decode_noheader_malloc     stbi_zlib_decode_noheader_malloc
#define ez_stbi_zlib_decode_noheader_buffer     stbi_zlib_decode_noheader_buffer

#endif /* USE_ORIGINAL_STBI */

#endif /* EZ_IMAGE__H */
/// @endcond Document_C_source
/// @cond Document_C_source
/*
 * ez-image.c: EZ-Draw module to load and draw images
 *
 * Edouard.Thiel@lif.univ-mrs.fr - 12/05/2013 - version 1.2
 * Benoit.Favre@lif.univ-mrs.fr
 *
 * This program is free software under the terms of the
 * GNU Lesser General Public License (LGPL) version 2.1.
*/

/* To debug, define environment variable EZ_IMAGE_DEBUG */

#define EZ_PRIVATE_DEFS 1
//REMOVED #include "ez-image.h"

/* Contains internal parameters of ez-draw.c */
extern Ez_X ezx;

/* Counters for debugging */
int ez_image_count = 0, ez_pixmap_count = 0;


/*---------------------- P U B L I C   I N T E R F A C E --------------------*/

/*
 * Allocate an image, initialized with default values.
 * Return the image, else NULL.
*/

Ez_image *ez_image_new (void)
{
    Ez_image *img = (Ez_image *) malloc (sizeof(Ez_image));
    if (img == NULL) {
        ez_error ("ez_image_new: out of memory\n");
        return NULL;
    }
    ez_image_count++;

    img->width = img->height = 0;
    img->pixels_rgba = NULL;
    img->has_alpha = 0;
    img->opacity = 128;

    return img;
}


/*
 * Destroy image and free memory.
*/

void ez_image_destroy (Ez_image *img)
{
    if (img == NULL) return;
    if (img->pixels_rgba != NULL) free (img->pixels_rgba);
    free (img);

    ez_image_count--;
    if (ez_image_debug ())
        printf ("ez_image_destroy  count = %d\n", ez_image_count);
}


/*
 * Create an image having width w and height h.
 * Return the image, else NULL.
*/

Ez_image *ez_image_create (int w, int h)
{
    Ez_image *img;

    if (w < 0 || h < 0) {
        ez_error ("ez_image_create: bad size\n");
        return NULL;
    }

    img = ez_image_new ();
    if (img == NULL) return NULL;

    img->pixels_rgba = (EZuint8*) calloc (w*h*4, 1);
    if (img->pixels_rgba == NULL) {
        ez_error ("ez_image_create: out of memory\n");
        return NULL;
    }
    img->width = w; img->height = h;

    return img;
}


/*
 * Create a deep copy of image img.
 * Return the new image, else NULL.
*/

Ez_image *ez_image_dup (Ez_image *img)
{
    Ez_image *res;

    if (img == NULL) return NULL;
    res = ez_image_create (img->width, img->height);
    if (res == NULL) return NULL;
    memcpy (res->pixels_rgba, img->pixels_rgba, img->width * img->height * 4);
    res->has_alpha = img->has_alpha;
    res->opacity   = img->opacity;
    return res;
}


/*
 * Load an image from a file filename.
 * Return the image, else NULL.
 */

Ez_image *ez_image_load (const char *filename)
{
    Ez_image *img;
    int nbytes;
    double time1 = 0, time2 = 0;

    img = ez_image_new ();
    if (img == NULL) return NULL;

    if (ez_image_debug()) time1 = ez_get_time ();

    /* Loading with stbi */
    img->pixels_rgba = ez_stbi_load (filename, &img->width, &img->height,
        &nbytes, EZ_STBI_RGB_ALPHA);
    if (img->pixels_rgba == NULL) {
        ez_error ("ez_load_image: can't load file \"%s\"\n", filename);
        ez_image_destroy (img);
        return NULL;
    }
    /* An alpha channel is present in the file? */
    img->has_alpha = (nbytes == 4 || nbytes==2);

    if (ez_image_debug()) {
        time2 = ez_get_time ();
        printf ("ez_image_load  file \"%s\"  in %.3f ms  w = %d  h = %d  "
                "n = %d  has_alpha = %d\n",
                filename, (time2-time1)*1000, img->width, img->height,
                nbytes, img->has_alpha);
    }

    return img;
}


/*
 * Properties has_alpha and opacity
*/

void ez_image_set_alpha (Ez_image *img, int has_alpha)
{
    if (img == NULL) return;
    img->has_alpha = has_alpha ? 1 : 0;
}


int ez_image_has_alpha (Ez_image *img)
{
    if (img == NULL) return 0;
    return img->has_alpha ? 1 : 0;
}


void ez_image_set_opacity (Ez_image *img, int opacity)
{
    if (img == NULL) return;
    img->opacity = opacity;
}


int ez_image_get_opacity (Ez_image *img)
{
    if (img == NULL) return 0;
    return img->opacity;
}


/*
 * Display an image or a rectangular region of the image img in the
 * window win, with the upper left corner of the image at the x,y
 * coordinates in the window.
 *
 * If img->has_alpha is true, apply transparency.
*/

void ez_image_paint (Ez_window win, Ez_image *img, int x, int y)
{
    if (img == NULL) return;
    ez_image_paint_sub (win, img, x, y, 0, 0, img->width, img->height);
}


void ez_image_paint_sub (Ez_window win, Ez_image *img, int x, int y,
    int src_x, int src_y, int w, int h)
{
    int src_x_old = src_x, src_y_old = src_y;

    if (win == None) return;

    if (ez_image_confine_sub_coords (img, &src_x, &src_y, &w, &h) < 0) return;
    x += src_x - src_x_old;
    y += src_y - src_y_old;

#ifdef EZ_BASE_XLIB
    if (win == ezx.dbuf_win) win = ezx.dbuf_pix;
    ez_image_draw_xi (win, img, x, y, src_x, src_y, w, h);
#elif defined EZ_BASE_WIN32
    ez_cur_win (win);
    ez_image_draw_dib (ezx.hdc, img, x, y, src_x, src_y, w, h);
#endif /* EZ_BASE_ */
}


/*
 * Print a rectangular region of the image img in the terminal.
*/

void ez_image_print (Ez_image *img, int src_x, int src_y, int w, int h)
{
    if (ez_image_confine_sub_coords (img, &src_x, &src_y, &w, &h) < 0) return;

    printf ("Image: width %d  height %d  has_alpha %s  opacity %d\n",
        img->width, img->height, img->has_alpha?"true":"false", img->opacity);
    printf ("Region: src_x %d  src_y %d  width %d  height %d\n",
        src_x, src_y, w, h);

    ez_image_print_rgba (img, src_x, src_y, w, h);
}


/*
 * Fill an image with RGBA color
*/

void ez_image_fill_rgba (Ez_image *img, EZuint8 r, EZuint8 g, EZuint8 b, EZuint8 a)
{
    if (img == NULL) return;

    ez_image_comp_fill_rgba (img, r, g, b, a);
}


/*
 * Superimpose a region of image src into the image dst. If src image has an
 * alpha channel, apply transparency.
 * If the coordinates go beyond the images src or dst, just the common region
 * is superimposed.
*/

void ez_image_blend (Ez_image *dst, Ez_image *src, int dst_x, int dst_y)
{
    if (src == NULL) return;
    ez_image_blend_sub (dst, src, dst_x, dst_y, 0, 0, src->width, src->height);
}


void ez_image_blend_sub (Ez_image *dst, Ez_image *src, int dst_x, int dst_y,
    int src_x, int src_y, int w, int h)
{
    int src_x_old = src_x, src_y_old = src_y, dst_x_old, dst_y_old;

    if (ez_image_confine_sub_coords (src, &src_x, &src_y, &w, &h) < 0) return;
    dst_x += src_x - src_x_old; dst_x_old = dst_x;
    dst_y += src_y - src_y_old; dst_y_old = dst_y;
    if (ez_image_confine_sub_coords (dst, &dst_x, &dst_y, &w, &h) < 0) return;
    src_x += dst_x - dst_x_old;
    src_y += dst_y - dst_y_old;

    if (src->has_alpha)
         ez_image_comp_blend (dst, src, dst_x, dst_y, src_x, src_y, w, h);
    else ez_image_comp_over  (dst, src, dst_x, dst_y, src_x, src_y, w, h);
}


/*
 * Extract a rectangular region of an image.
 * Return new image, else NULL.
*/

Ez_image *ez_image_extract (Ez_image *img, int src_x, int src_y, int w, int h)
{
    Ez_image *res;

    if (ez_image_confine_sub_coords (img, &src_x, &src_y, &w, &h) < 0)
        return NULL;

    res = ez_image_create (w, h);
    if (res == NULL) return NULL;
    res->has_alpha = img->has_alpha;
    res->opacity   = img->opacity;

    ez_image_copy_sub (img, res, src_x, src_y);

    return res;
}


/*
 * Vertical or horizontal symmetry.
 * Return new image, else NULL.
*/

Ez_image *ez_image_sym_ver (Ez_image *img)
{
    Ez_image *res;

    if (img == NULL) return NULL;

    res = ez_image_create (img->width, img->height);
    if (res == NULL) return NULL;
    res->has_alpha = img->has_alpha;
    res->opacity   = img->opacity;

    ez_image_comp_symv (img, res);
    return res;
}


Ez_image *ez_image_sym_hor (Ez_image *img)
{
    Ez_image *res;

    if (img == NULL) return NULL;

    res = ez_image_create (img->width, img->height);
    if (res == NULL) return NULL;
    res->has_alpha = img->has_alpha;
    res->opacity   = img->opacity;

    ez_image_comp_symh (img, res);
    return res;
}


/*
 * Scale an image.
 * Return new image, else NULL.
*/

Ez_image *ez_image_scale (Ez_image *img, double factor)
{
    Ez_image *res;

    if (img == NULL) return NULL;

    if (factor <= 0) {
        ez_error ("ez_image_scale: bad scale factor %f\n", factor);
        return NULL;
    }
    if (factor == 1)
        return ez_image_dup (img);

    res = ez_image_create (img->width*factor, img->height*factor);
    if (res == NULL) return NULL;
    res->has_alpha = 1;
    res->opacity   = img->opacity;

    if (factor > 1)
         ez_image_expand (img, res, factor);
    else ez_image_shrink (img, res, factor);
    return res;
}


/*
 * Rotate image img for angle theta in degrees.
 * Return a new image whose size is adjusted to contain the result, or NULL
 * on error. The outer parts of the original image become transparent.
*/

Ez_image *ez_image_rotate (Ez_image *img, double theta, int quality)
{
    int w, h;
    Ez_image *res;

    if (img == NULL) return NULL;

    ez_rotate_get_size (theta, img->width, img->height, &w, &h);

    res = ez_image_create (w, h);
    if (res == NULL) return NULL;
    res->has_alpha = 1;
    res->opacity   = img->opacity;

    if (quality)
         ez_image_rotate_bilinear (img, res, theta);
    else ez_image_rotate_nearest  (img, res, theta);
    return res;
}


/*
 * Compute for a point having coordinates src_x,src_y in the source image, the
 * corresponding coordinates dst_x,dst_y of the point in the destination image.
*/

void ez_image_rotate_point (Ez_image *img, double theta, int src_x, int src_y,
    int *dst_x, int *dst_y)
{
    if (img == NULL || dst_x == NULL || dst_y == NULL) return;

    ez_rotate_get_coords (theta, img->width, img->height, src_x, src_y,
        dst_x, dst_y);
}


/*
 * Allocate a pixmap, initialized to default value.
 * Return the pixmap, else NULL.
*/

Ez_pixmap *ez_pixmap_new (void)
{
    Ez_pixmap *pix = (Ez_pixmap *) malloc (sizeof(Ez_pixmap));
    if (pix == NULL) {
        ez_error ("ez_pixmap_new: out of memory\n");
        return NULL;
    }
    ez_pixmap_count++;

    pix->width = pix->height = 0;
#ifdef EZ_BASE_XLIB
    pix->map = None;
    pix->mask = None;
#elif defined EZ_BASE_WIN32
    pix->hmap = NULL;
    pix->has_alpha = 0;
#endif /* EZ_BASE_ */

    return pix;
}


/*
 * Destroy the pixmap and free memory.
*/

void ez_pixmap_destroy (Ez_pixmap *pix)
{
    if (pix == NULL) return;

#ifdef EZ_BASE_XLIB
    if (pix->map  != None) XFreePixmap (ezx.display, pix->map );
    if (pix->mask != None) XFreePixmap (ezx.display, pix->mask);
#elif defined EZ_BASE_WIN32
    if (pix->hmap != NULL) DeleteObject (pix->hmap);
#endif /* EZ_BASE_ */
    free (pix);

    ez_pixmap_count--;
    if (ez_image_debug ())
        printf ("ez_pixmap_destroy  count = %d\n", ez_pixmap_count);
}


/*
 * Create a pixmap from an image img.
 * Return the pixmap, else NULL.
*/

Ez_pixmap *ez_pixmap_create_from_image (Ez_image *img)
{
    Ez_pixmap *pix;

    if (img == NULL) return NULL;
    pix = ez_pixmap_new ();
    if (pix == NULL) return NULL;

    pix->width  = img->width;
    pix->height = img->height;

#ifdef EZ_BASE_XLIB
    if (ez_pixmap_build_map (pix, img) < 0) {
        ez_error ("ez_pixmap_create_from_image: can't create map\n");
        goto free_pix;
    }
    if (img->has_alpha) {
        pix->mask = ez_xmask_create (ezx.root_win, img, 0, 0,
            img->width, img->height);
        if (pix->mask == None) {
            ez_error ("ez_pixmap_create_from_image: can't create mask\n");
            goto free_pix;
        }
    }
#elif defined EZ_BASE_WIN32
    if (ez_pixmap_build_hmap (pix, img) < 0) {
        ez_error ("ez_pixmap_create_from_image: can't create hmap\n");
        goto free_pix;
    }
    pix->has_alpha = img->has_alpha;

#endif /* EZ_BASE_ */

    return pix;

  free_pix:
    ez_pixmap_destroy (pix);
    return NULL;
}


/*
 * Display the pixmap pix in the window win.
 * The top left corner of the pixmap is displayed at the x,y coordinates
 * in the window.
*/

void ez_pixmap_paint (Ez_window win, Ez_pixmap *pix, int x, int y)
{
    if (win == None) return;

#ifdef EZ_BASE_XLIB
    if (win == ezx.dbuf_win) win = ezx.dbuf_pix;
    ez_pixmap_draw_area (win, pix, x, y);
#elif defined EZ_BASE_WIN32
    ez_cur_win (win);
    ez_pixmap_draw_hmap (ezx.hdc, pix, x, y);
#endif /* EZ_BASE_ */
}


/*
 * Display the pixmap pix repeatedly in the window win.
 * The pixmap is displayed as a wallpaper in the window region bounded
 * by coordinates x,y (top left corner) and x+w-1,y+h-1 (bottom right corner).
*/

void ez_pixmap_tile (Ez_window win, Ez_pixmap *pix, int x, int y, int w, int h)
{
    if (win == None || w <= 0 || h <= 0) return;

#ifdef EZ_BASE_XLIB
    if (win == ezx.dbuf_win) win = ezx.dbuf_pix;
    ez_pixmap_tile_area (win, pix, x, y, w, h);
#elif defined EZ_BASE_WIN32
    ez_cur_win (win);
    ez_pixmap_tile_hmap (ezx.hdc, pix, x, y, w, h);
#endif /* EZ_BASE_ */
}


/*-------------------- P R I V A T E   F U N C T I O N S --------------------*/

/*
 * Unique test of the definition of the environment variable EZ_IMAGE_DEBUG
*/

int ez_image_debug (void)
{
    static int debug = -1;
    if (debug < 0) debug = getenv ("EZ_IMAGE_DEBUG") != NULL;
    return debug;
}


/*
 * Confine coordinates of sub-image inside img
*/

int ez_image_confine_sub_coords (Ez_image *img, int *src_x, int *src_y,
    int *w, int *h)
{
    if (img == NULL) return -1;

    if (ez_confine_coord (src_x, w, img->width) < 0 ||
        ez_confine_coord (src_y, h, img->height) < 0) {
        if (ez_image_debug ())
            printf ("ez_image_confine_sub_coords empty region "
                    "src_x = %d  src_y = %d  w = %d  h = %d\n",
                *src_x, *src_y, *w, *h);
        return -1;
    }
    return 0;
}

int ez_confine_coord (int *t, int *r, int tmax)
{
    if (*t < 0) { *r += *t; *t = 0; }
    if (*t+*r > tmax) *r = tmax -*t;
    return *r > 0 ? 0 : -1;
}


#ifdef EZ_BASE_XLIB

/*
 * Display sub-image with transparency.
*/

void ez_image_draw_xi (Ez_window win, Ez_image *img, int x, int y,
    int src_x, int src_y, int w, int h)
{
    XImage *xi = NULL;
    Pixmap mask = None;
    ez_xi_func xi_func;

    xi_func = ez_xi_get_func ();
    xi = ez_xi_create (img, src_x, src_y, w, h, xi_func);
    if (xi == NULL) return;

    if (img->has_alpha) {
        mask = ez_xmask_create (win, img, src_x, src_y, w, h);
        if (mask == None) goto free_xi;
        XSetClipOrigin (ezx.display, ezx.gc, x, y);
        XSetClipMask (ezx.display, ezx.gc, mask);
    }

    XPutImage (ezx.display, win, ezx.gc, xi, 0, 0, x, y, w, h);

    if (img->has_alpha) {
        XSetClipOrigin (ezx.display, ezx.gc, 0, 0);
        XSetClipMask (ezx.display, ezx.gc, None);
        XFreePixmap (ezx.display, mask);
    }

  free_xi:
    XDestroyImage (xi);
}


/*
 * Creation of ximage
*/

XImage *ez_xi_create (Ez_image *img, int src_x, int src_y, int w, int h,
    ez_xi_func xi_func)
{
    XImage *xi = NULL;

    if (xi_func == NULL) {
        ez_error ("ez_xi_create: NULL xi_func\n");
        return NULL;
    }

    /* We must first create xi to obtain informations to create data */
    xi = XCreateImage (ezx.display, ezx.visual, ezx.depth, ZPixmap, 0,
        NULL, w, h, 32, 0);
    if (xi == NULL) {
        ez_error ("ez_xi_create: can't create XImage\n");
        return NULL;
    }

    if (ez_image_debug())
        printf ("ez_xi_create  w = %d  h = %d  depth = %d  bpp = %d\n",
            w, h, ezx.depth, xi->bits_per_pixel);

    xi->data = (char*) calloc (xi->bytes_per_line * h, 1);
    if (xi->data == NULL)  {
        ez_error ("ez_xi_create: out of memory\n");
        XDestroyImage (xi);
        return NULL;
    }
    /* xi->data will be freed by XDestroyImage */

    /* Draw pixels in xi->data */
    xi_func (xi, img, src_x, src_y, w, h);

    return xi;
}


ez_xi_func ez_xi_get_func (void)
{
    static ez_xi_func xi_func = NULL;

    if (xi_func != NULL) return xi_func;
    xi_func = ez_xi_fill_default;

    if (ezx.depth == 24)
    {
        Ez_image *img = ez_xi_test_create ();
        XImage *xi1 = ez_xi_create (img, 0, 0, img->width, img->height,
            ez_xi_fill_default);
        XImage *xi2 = ez_xi_create (img, 0, 0, img->width, img->height,
            ez_xi_fill_24);

        if (ez_xi_diff (xi1, xi2) == 0)
            xi_func = ez_xi_fill_24;

        XDestroyImage (xi1);
        XDestroyImage (xi2);
        ez_image_destroy (img);
    }

    return xi_func;
}


void ez_xi_fill_default (XImage *xi, Ez_image *img, int src_x, int src_y,
    int w, int h)
{
    int x, y, tx, ty, img_w = img->width;
    EZuint8 cr, cg, cb;
    double time1 = 0;

    if (ez_image_debug()) time1 = ez_get_time ();

    for (y = 0, ty = src_y * img_w; y < h; y++, ty += img_w)
    for (x = 0, tx = (ty + src_x)*4; x < w; x++, tx += 4) {
        cr = img->pixels_rgba[tx];
        cg = img->pixels_rgba[tx+1];
        cb = img->pixels_rgba[tx+2];
        XPutPixel (xi, x, y, ez_get_RGB (cr, cg, cb));
    }

    if (ez_image_debug())
        printf ("ez_xi_fill_default %.3f ms\n", (ez_get_time() - time1)*1000);
}


/* Speed gain compared to fill_default: 50% */

void ez_xi_fill_24 (XImage *xi, Ez_image *img, int src_x, int src_y,
    int w, int h)
{
    int x, y, tx, ty, i, j, img_w = img->width,
        dj = xi->bytes_per_line,
        di = xi->bits_per_pixel / 8,
        ir = ezx.trueColor.red.shift   / 8,
        ig = ezx.trueColor.green.shift / 8,
        ib = ezx.trueColor.blue.shift  / 8;
    EZuint8 *data = (EZuint8 *) xi->data;
    double time1 = 0;

    if (ez_image_debug()) time1 = ez_get_time ();

    for (y = 0, ty = src_y * img_w, j = 0; y < h; y++, ty += img_w, j += dj)
    for (x = 0, tx = (ty + src_x)*4, i = j; x < w; x++, tx += 4, i += di) {
        data[i+ir] = img->pixels_rgba[tx];
        data[i+ig] = img->pixels_rgba[tx+1];
        data[i+ib] = img->pixels_rgba[tx+2];
    }

    if (ez_image_debug())
        printf ("ez_xi_fill_24 %.3f ms\n", (ez_get_time() - time1)*1000);
}


Ez_image *ez_xi_test_create (void)
{
    int w = 9, h = 13, t, tmax = w*h*4;
    Ez_image *img = ez_image_create (w, h);
    if (img == NULL) return NULL;

    for (t = 0; t < tmax; t += 4) {
        img->pixels_rgba[t  ] = t*11/256;
        img->pixels_rgba[t+1] = t*31/256;
        img->pixels_rgba[t+2] = t* 7/256;
    }

    return img;
}


int ez_xi_diff (XImage *xi1, XImage *xi2)
{
    int ntot, i;
    if (xi1 == NULL || xi2 == NULL) return -1;

    ntot = xi1->bytes_per_line * xi1->height;
    for (i = 0; i < ntot; i++)
        if (xi1->data[i] != xi2->data[i]) return -1;

    return 0;
}


/*
 * Create a cutting mask from alpha channel and opacity threshold
*/

Pixmap ez_xmask_create (Ez_window win, Ez_image *img, int src_x, int src_y,
    int w, int h)
{
    Pixmap mask = None;
    EZuint8 *data = NULL;
    int bytes_per_line = (w+7)/8;
    double time1 = 0, time2 = 0, time3 = 0;

    data = (EZuint8*) calloc (bytes_per_line*h, 1);
    if (data == NULL) {
        ez_error ("ez_xmask_create: out of memory\n");
        return None;
    }

    if (ez_image_debug()) time1 = ez_get_time ();

    ez_xmask_fill (data, img, src_x, src_y, w, h);

    if (ez_image_debug()) time2 = ez_get_time ();

    mask = XCreateBitmapFromData (ezx.display, win, (char*) data, w, h);
    if (mask == None)
        ez_error ("ez_xmask_create: can't create bitmap");

    if (ez_image_debug()) {
        time3 = ez_get_time ();
        printf ("ez_xmask_create   fill %.3f ms   bitmap %.3f ms\n",
            (time2-time1)*1000, (time3-time2)*1000);
    }

    free (data);
    return mask;
}


void ez_xmask_fill (EZuint8 *data, Ez_image *img,
    int src_x, int src_y, int w, int h)
{

    int tx, mx, y, ty, i, j, bpl = (w+7)/8, img_w = img->width;

    for (y = 0, ty = src_y * img_w ; y < h ; y++, ty += img_w)
    for (tx = (ty+src_x)*4+3, mx = tx+w*4, i = y*bpl, j = 0 ; tx < mx ; tx += 4, j++) {
        if (j == 8) { i++; j = 0; }
        if (img->pixels_rgba[tx] < img->opacity) continue;
        data[i] |= 1 << j;
    }
}

#elif defined EZ_BASE_WIN32

/*
 * Display a sub-image with transparency.
 * DIB = Device Independent Bitmap
*/

void ez_image_draw_dib (HDC hdc_dst, Ez_image *img, int x, int y,
    int src_x, int src_y, int w, int h)
{
    HDC hdc = NULL;
    HBITMAP hbitmap = NULL;
    BITMAPINFO bmi;
    EZuint8 *data;

    /* Create a DC for the bitmap */
    hdc = CreateCompatibleDC(hdc_dst);
    if (hdc == NULL) {
        ez_error ("ez_image_draw_dib: can't create compatible DC\n");
        return;
    }

    ZeroMemory(&bmi, sizeof(BITMAPINFO));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = w;
    bmi.bmiHeader.biHeight = -h;  /* negative height for Origin at top left */
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = w * h * 4;

    hbitmap = CreateDIBSection (hdc_dst, &bmi, DIB_RGB_COLORS, (void **) &data, NULL, 0);
    if (hbitmap == NULL) {
        ez_error ("ez_image_draw_dib: can't create DIB Section\n");
        goto final;
    }
    SelectObject (hdc, hbitmap);

    if (!img->has_alpha)
         ez_dib_fill_noalpha   (data, img, src_x, src_y, w, h);
    else if (img->opacity >= 0)
         ez_dib_fill_opacity   (data, img, src_x, src_y, w, h);
    else ez_dib_fill_truealpha (data, img, src_x, src_y, w, h);

    if (img->has_alpha) {
        BLENDFUNCTION bf;

        bf.BlendOp = AC_SRC_OVER;
        bf.BlendFlags = 0;
        bf.AlphaFormat = AC_SRC_ALPHA;  /* alpha channel premultiplied */
        bf.SourceConstantAlpha = 0xff;

        if (AlphaBlend (hdc_dst, x, y, w, h, hdc, 0, 0, w, h, bf) == FALSE)
            ez_error ("ez_image_draw_dib: AlphaBlend failed\n");
    } else {
        if (BitBlt (hdc_dst, x, y, w, h, hdc, 0, 0, SRCCOPY ) == 0)
            ez_error ("ez_image_draw_dib: bitblt failed\n");
    }

  final:
    if (hbitmap != NULL) DeleteObject (hbitmap);
    if (hdc != NULL) DeleteDC (hdc);
}


void ez_dib_fill_noalpha (EZuint8 *data, Ez_image *img, int src_x, int src_y,
    int w, int h)
{
    int x, y, tx, ty, i, j, img_w = img->width, dj = w*4;
    double time1 = 0;

    if (ez_image_debug()) time1 = ez_get_time ();

    for (y = 0, ty = src_y * img_w,  j = 0; y < h; y++, ty += img_w, j += dj)
    for (x = 0, tx = (ty + src_x)*4, i = j; x < w; x++, tx += 4, i += 4) {
        data[i+3] = 0xff;
        data[i+2] = img->pixels_rgba[tx];
        data[i+1] = img->pixels_rgba[tx+1];
        data[i  ] = img->pixels_rgba[tx+2];
    }

    if (ez_image_debug())
        printf ("ez_dib_fill_noalpha %.3f ms\n", (ez_get_time() - time1)*1000);
}


void ez_dib_fill_opacity (EZuint8 *data, Ez_image *img, int src_x, int src_y,
    int w, int h)
{
    int x, y, tx, ty, i, j, img_w = img->width, dj = w*4;
    double time1 = 0;

    if (ez_image_debug()) time1 = ez_get_time ();

    for (y = 0, ty = src_y * img_w,  j = 0; y < h; y++, ty += img_w, j += dj)
    for (x = 0, tx = (ty + src_x)*4, i = j; x < w; x++, tx += 4, i += 4) {
        if (img->pixels_rgba[tx+3] >= img->opacity) {
            data[i+3] = 0xff;
            data[i+2] = img->pixels_rgba[tx];
            data[i+1] = img->pixels_rgba[tx+1];
            data[i  ] = img->pixels_rgba[tx+2];
        } else {
            data[i+3] = 0;
            data[i+2] = 0;
            data[i+1] = 0;
            data[i  ] = 0;
        }
    }

    if (ez_image_debug())
        printf ("ez_dib_fill_opacity %.3f ms\n", (ez_get_time() - time1)*1000);
}


void ez_dib_fill_truealpha (EZuint8 *data, Ez_image *img, int src_x, int src_y,
    int w, int h)
{
    int x, y, tx, ty, i, j, img_w = img->width, dj = w*4, a;
    double time1 = 0;

    if (ez_image_debug()) time1 = ez_get_time ();

    for (y = 0, ty = src_y * img_w,  j = 0; y < h; y++, ty += img_w, j += dj)
    for (x = 0, tx = (ty + src_x)*4, i = j; x < w; x++, tx += 4, i += 4) {
        data[i+3] = a = img->pixels_rgba[tx+3];
        data[i+2] = img->pixels_rgba[tx  ]*a/255;
        data[i+1] = img->pixels_rgba[tx+1]*a/255;
        data[i  ] = img->pixels_rgba[tx+2]*a/255;
    }

    if (ez_image_debug())
        printf ("ez_dib_fill_truealpha %.3f ms\n", (ez_get_time() - time1)*1000);
}

#endif /* EZ_BASE_ */


/*
 * Print a rectangular region of the image img in the terminal.
*/

void ez_image_print_rgba (Ez_image *img, int src_x, int src_y, int w, int h)
{
    int x, y;
    EZuint8 *p;

    printf (" y,x ");
    for (x = src_x; x < src_x+w; x++)
        printf ("|      %4d      ", x);
    printf ("\n");

    for (y = src_y; y < src_y+h; y++) {
        printf ("%4d ", y);
        for (x = src_x; x < src_x+w; x++) {
            p = img->pixels_rgba + (y*img->width+x)*4;
            printf ("| %3d %3d %3d %3d", p[0], p[1], p[2], p[3]);
        }
        printf ("\n");
    }
}


/*
 * Fill image with a RGBA color
*/

void ez_image_comp_fill_rgba (Ez_image *img, EZuint8 r, EZuint8 g, EZuint8 b,
    EZuint8 a)
{
    int t, tmax4 = img->width * img->height * 4;

    for (t = 0; t < tmax4; t += 4) {
        img->pixels_rgba[t  ] = r;
        img->pixels_rgba[t+1] = g;
        img->pixels_rgba[t+2] = b;
        img->pixels_rgba[t+3] = a;
    }
}


/*
 * Superimpose src into dst, without transparency
*/

void ez_image_comp_over (Ez_image *dst, Ez_image *src, int dst_x, int dst_y,
    int src_x, int src_y, int w, int h)
{
    int x, y, t_src, t_dst;

    for (y = 0; y < h; y++)
    for (x = 0; x < w; x++) {
        t_src = ((y+src_y)*src->width+x+src_x)*4;
        t_dst = ((y+dst_y)*dst->width+x+dst_x)*4;

        dst->pixels_rgba[t_dst  ] = src->pixels_rgba[t_src  ];
        dst->pixels_rgba[t_dst+1] = src->pixels_rgba[t_src+1];
        dst->pixels_rgba[t_dst+2] = src->pixels_rgba[t_src+2];
        dst->pixels_rgba[t_dst+3] = src->pixels_rgba[t_src+3];
    }
}


/*
 * Superimpose src into dst, with transparency
*/

void ez_image_comp_blend (Ez_image *dst, Ez_image *src, int dst_x, int dst_y,
    int src_x, int src_y, int w, int h)
{
    int x, y, t_src, t_dst, a_src, a_dst, a_res;

    for (y = 0; y < h; y++)
    for (x = 0; x < w; x++) {
        t_src = ((y+src_y)*src->width+x+src_x)*4;
        t_dst = ((y+dst_y)*dst->width+x+dst_x)*4;

        a_src = src->pixels_rgba[t_src+3];
        a_dst = dst->pixels_rgba[t_dst+3];
        a_res = a_src + a_dst * (255-a_src) / 255;

        if (a_res == 0) {
            dst->pixels_rgba[t_dst  ] = 0;
            dst->pixels_rgba[t_dst+1] = 0;
            dst->pixels_rgba[t_dst+2] = 0;
            dst->pixels_rgba[t_dst+3] = 0;
            continue;
        }

        dst->pixels_rgba[t_dst  ] =
            ( src->pixels_rgba[t_src  ] * a_src +
              dst->pixels_rgba[t_dst  ] * a_dst * (255-a_src) / 255 ) / a_res;
        dst->pixels_rgba[t_dst+1] =
            ( src->pixels_rgba[t_src+1] * a_src +
              dst->pixels_rgba[t_dst+1] * a_dst * (255-a_src) / 255 ) / a_res;
        dst->pixels_rgba[t_dst+2] =
            ( src->pixels_rgba[t_src+2] * a_src +
              dst->pixels_rgba[t_dst+2] * a_dst * (255-a_src) / 255 ) / a_res;
        dst->pixels_rgba[t_dst+3] = a_res;
    }
}


/*
 * Extract a rectangular region from an image
*/

void ez_image_copy_sub (Ez_image *src, Ez_image *dest , int src_x, int src_y)
{
    int tw4  = dest->width*4,
        sw4  = src ->width*4,
        sxy4 = (src_y*src->width + src_x)*4,
        y, ty, sy;

    for (y = ty = 0, sy = sxy4; y < dest->height; y++, ty += tw4, sy += sw4)
        memcpy (dest->pixels_rgba + ty, src ->pixels_rgba + sy, tw4);
}


/*
 * Transformations
*/

void ez_image_comp_symv (Ez_image *src, Ez_image *dst)
{
    int dst_h = dst->height, dst_w = dst->width,
        x, y, ts, td, tw = dst_w;
    Ez_uint32 *src_p = (Ez_uint32 *)src->pixels_rgba,
              *dst_p = (Ez_uint32 *)dst->pixels_rgba;

    for (y = 0, td = 0      ; y < dst_h; y++)
    for (x = 0, ts = td+tw-1; x < dst_w; x++, td++, ts--)
        dst_p[td] = src_p[ts];
}


void ez_image_comp_symh (Ez_image *src, Ez_image *dst)
{
    int dst_h = dst->height, dst_w = dst->width,
        y, ts, td, tw4 = dst_w*4, tdm4 = (dst_h-1)*tw4;

    for (td = y = 0, ts = tdm4; y < dst_h; y++, td += tw4, ts -= tw4)
        memcpy (dst->pixels_rgba + td, src->pixels_rgba + ts, tw4);
}


void ez_image_expand (Ez_image *src, Ez_image *dst, double factor)
{
    int x, y, t;

    for (t = y = 0; y < dst->height; y++)
    for (    x = 0; x < dst->width ; x++, t+=4)
        ez_bilinear_4points (src->pixels_rgba, dst->pixels_rgba,
            src->width, src->height, x/factor, y/factor, t);
}


void ez_image_shrink (Ez_image *src, Ez_image *dst, double factor)
{
    int x, y, t;

    for (t = y = 0; y < dst->height; y++)
    for (    x = 0; x < dst->width ; x++, t+=4)
        ez_bilinear_pane (src->pixels_rgba, dst->pixels_rgba,
            src->width, src->height, x/factor, y/factor, t, factor);
}


void ez_rotate_get_size (double theta, int src_w, int src_h, int *dst_w, int *dst_h)
{
    double a = theta*M_PI/180, c = cos(a), s = sin(a),
           w1 = fabs (c*src_w - s*src_h), h1 = fabs (s*src_w + c*src_h),
           w2 = fabs (c*src_w + s*src_h), h2 = fabs (s*src_w - c*src_h);
    *dst_w = EZ_ROUND (w1 > w2 ? w1 : w2);
    *dst_h = EZ_ROUND (h1 > h2 ? h1 : h2);
}


void ez_rotate_get_coords (double theta, int src_w, int src_h, int src_x, int src_y,
    int *dst_x, int *dst_y)
{
    double a = theta*M_PI/180, c = cos(a), s = sin(a);
    double sx[4], sy[4], tx[4], ty[4], minx, miny;
    int i;

    /* Corners coordinates relative to the rotation center src */
    sx[0] = 0     - src_x; sy[0] = 0     - src_y;
    sx[1] = src_w - src_x; sy[1] = 0     - src_y;
    sx[2] = src_w - src_x; sy[2] = src_h - src_y;
    sx[3] = 0     - src_x; sy[3] = src_h - src_y;

    /* Corners rotation */
    for (i = 0; i < 4; i++) {
        tx[i] = c*sx[i] - s*sy[i];
        ty[i] = s*sx[i] + c*sy[i];
    }

    /* Research of min */
    minx = tx[0];
    miny = ty[0];
    for (i = 1; i < 4; i++) {
        if (tx[i] < minx) minx = tx[i];
        if (ty[i] < miny) miny = ty[i];
    }

    *dst_x = EZ_ROUND (-minx);
    *dst_y = EZ_ROUND (-miny);
}


void ez_image_rotate_nearest (Ez_image *src, Ez_image *dst, double theta)
{
    double a = theta*M_PI/180, c = cos(-a), s = sin(-a), sx, sy;
    int x, y, t, dx, dy, xn, yn, dst_x, dst_y,
        src_w = src->width, src_h = src->height,
        dst_w = dst->width, dst_h = dst->height;
    Ez_uint32 *src_p = (Ez_uint32 *)src->pixels_rgba,
              *dst_p = (Ez_uint32 *)dst->pixels_rgba;

    /* We set the rotation center to 0,0 and we retrieve the center
       coordinates dst_x,dst_y in dst */
    ez_rotate_get_coords (theta, src_w, src_h, 0, 0, &dst_x, &dst_y);

    for (t = y = 0, dy = -dst_y; y < dst_h; y++, dy++)
    for (    x = 0, dx = -dst_x; x < dst_w; x++, dx++, t++)
    {
        /* Coordinates of real antecedent and closest integer */
        sx = c*dx - s*dy; xn = EZ_ROUND(sx);
        sy = s*dx + c*dy; yn = EZ_ROUND(sy);

        /* Antecedent outside? */
        if (xn < 0 || xn >= src_w || yn < 0 || yn >= src_h)
             dst_p[t] = 0;
        else dst_p[t] = src_p[yn * src_w + xn];
    }
}


void ez_image_rotate_bilinear (Ez_image *src, Ez_image *dst, double theta)
{
    double a = theta*M_PI/180, c = cos(-a), s = sin(-a), sx, sy;
    int x, y, t, dx, dy, dst_x, dst_y,
        src_w = src->width, src_h = src->height,
        dst_w = dst->width, dst_h = dst->height;
    EZuint8 *src_p = src->pixels_rgba, *dst_p = dst->pixels_rgba;

    /* We set the rotation center to 0,0 and we retrieve the center
       coordinates dst_x,dst_y in dst */
    ez_rotate_get_coords (theta, src_w, src_h, 0, 0, &dst_x, &dst_y);

    for (t = y = 0, dy = -dst_y; y < dst_h; y++, dy++)
    for (    x = 0, dx = -dst_x; x < dst_w; x++, dx++, t+=4)
    {
        /* Coordinates of real antecedent */
        sx = c*dx - s*dy;
        sy = s*dx + c*dy;

        ez_bilinear_4points (src_p, dst_p, src_w, src_h, sx, sy, t);
    }
}


void ez_bilinear_4points (EZuint8 *src_p, EZuint8 *dst_p,
    int src_w, int src_h, double sx, double sy, int t)
{
    double rx0, rx1, ry0, ry1, r00, r01, r10, r11;
    int x0, y0, x1, y1, k00, k01, k10, k11;

    /* Antecedent outside? */
    if (sx < -0.5 || sx >= src_w-0.5 || sy < -0.5 || sy >= src_h-0.5) {
        dst_p[t] = dst_p[t+1] = dst_p[t+2] = dst_p[t+3] = 0;
        return;
    }

    /* Neighbors of real antecedent reel on the grid */
    x0 = sx < 0 ? sx-1 : sx; x1 = x0+1;
    y0 = sy < 0 ? sy-1 : sy; y1 = y0+1;

    /* Real distance */
    rx0 = sx-x0; rx1 = x1-sx;
    ry0 = sy-y0; ry1 = y1-sy;
    r00 = ry0*rx0; r01 = ry0*rx1;
    r10 = ry1*rx0; r11 = ry1*rx1;

    /* Neighbours offsets */
    k00 = (y0 * src_w + x0)*4; k01 = k00 + 4;
    k10 = k00 + src_w*4;       k11 = k10 + 4;

    /* Neighbour outside? We take the neighbour inside */
    if      (x0 < 0     ) k00 = k01, k10 = k11;
    else if (x1 >= src_w) k01 = k00, k11 = k10;
    if      (y0 < 0     ) k00 = k10, k01 = k11;
    else if (y1 >= src_h) k10 = k00, k11 = k01;

    /* Bilinear interpolation of colors and alpha channel */
    dst_p[t  ] = r11*src_p[k00  ] + r10*src_p[k01  ] + r01*src_p[k10  ] + r00*src_p[k11  ];
    dst_p[t+1] = r11*src_p[k00+1] + r10*src_p[k01+1] + r01*src_p[k10+1] + r00*src_p[k11+1];
    dst_p[t+2] = r11*src_p[k00+2] + r10*src_p[k01+2] + r01*src_p[k10+2] + r00*src_p[k11+2];
    dst_p[t+3] = r11*src_p[k00+3] + r10*src_p[k01+3] + r01*src_p[k10+3] + r00*src_p[k11+3];
}


void ez_bilinear_pane (EZuint8 *src_p, EZuint8 *dst_p,
    int src_w, int src_h, double sx, double sy, int t, double factor)
{
    double d = 1/factor, d2 = d*d, r = d/2, v, rx, ry,
           mx0 = sx-r, my0 = sy-r, mx1 = sx+r, my1 = sy+r;
    int x, y, i,
        x0 = EZ_ROUND (mx0), y0 = EZ_ROUND (my0),
        x1 = EZ_ROUND (mx1), y1 = EZ_ROUND (my1);

    if (x0 < 0) x0 = 0; else if (x1 > src_w-1) x1 = src_w-1;
    if (y0 < 0) y0 = 0; else if (y1 > src_h-1) y1 = src_h-1;

    if (x0 >= x1 || y0 >= y1) {
        dst_p[t] = dst_p[t+1] = dst_p[t+2] = dst_p[t+3] = 0;
        return;
    }

    for (i = 0; i < 4; i++) {
        v = 0;
        for (x = x0; x <= x1; x++)
        for (y = y0; y <= y1; y++) {
            rx = x == x0 ? x0+0.5-mx0 : x == x1 ? mx1-x1+0.5 : 1;
            ry = y == y0 ? y0+0.5-my0 : y == y1 ? my1-y1+0.5 : 1;
            v += src_p[(y*src_w+x)*4+i] * rx * ry;
        }
        dst_p[t+i] = v / d2;
    }
}


/*
 * Operations on Ez_pixmap
*/

#ifdef EZ_BASE_XLIB

int ez_pixmap_build_map (Ez_pixmap *pix, Ez_image *img)
{
    XImage *xi = NULL;
    ez_xi_func xi_func;

    pix->map = XCreatePixmap (ezx.display, ezx.root_win,
        img->width, img->height, ezx.depth);
    if (pix->map == None) return -1;

    xi_func = ez_xi_get_func ();
    xi = ez_xi_create (img, 0, 0, img->width, img->height, xi_func);
    if (xi == NULL) return -1;

    XPutImage (ezx.display, pix->map, ezx.gc, xi, 0, 0, 0, 0,
        img->width, img->height);
    XDestroyImage (xi);

    return 0;
}


void ez_pixmap_draw_area (Ez_window win, Ez_pixmap *pix, int x, int y)
{
    if (pix->mask != None) {
        XSetClipOrigin (ezx.display, ezx.gc, x, y);
        XSetClipMask (ezx.display, ezx.gc, pix->mask);
    }

    XCopyArea(ezx.display, pix->map, win, ezx.gc, 0, 0,
        pix->width, pix->height, x, y);

    if (pix->mask != None) {
        XSetClipOrigin (ezx.display, ezx.gc, 0, 0);
        XSetClipMask (ezx.display, ezx.gc, None);
    }
}


void ez_pixmap_tile_area (Ez_window win, Ez_pixmap *pix, int x, int y, int w, int h)
{
    int nx, ny;

    for (ny = 0; ny < h; ny += pix->height)
    for (nx = 0; nx < w; nx += pix->width)
    {
        if (pix->mask != None) {
            XSetClipOrigin (ezx.display, ezx.gc, x+nx, y+ny);
            XSetClipMask (ezx.display, ezx.gc, pix->mask);
        }

        XCopyArea(ezx.display, pix->map, win, ezx.gc, 0, 0,
            nx+pix->width  <= w ? pix->width  : w-nx,
            ny+pix->height <= h ? pix->height : h-ny,
            x+nx, y+ny);
    }

    if (pix->mask != None) {
        XSetClipOrigin (ezx.display, ezx.gc, 0, 0);
        XSetClipMask (ezx.display, ezx.gc, None);
    }
}

#elif defined EZ_BASE_WIN32

int ez_pixmap_build_hmap (Ez_pixmap *pix, Ez_image *img)
{
    HDC root_dc = NULL, pix_dc = NULL;
    int status = -1;

    root_dc = GetDC (NULL);
    if (root_dc == NULL) return -1;

    pix->hmap = CreateCompatibleBitmap (root_dc, img->width, img->height);
    if (pix->hmap == NULL) goto final;

    pix_dc = CreateCompatibleDC (root_dc);
    if (pix_dc == NULL) goto final;
    SelectObject (pix_dc, pix->hmap);

    ez_image_draw_dib (pix_dc, img, 0, 0, 0, 0, img->width, img->height);
    status = 0;

  final :
    if (root_dc != NULL) ReleaseDC (NULL, root_dc);
    if (pix_dc != NULL) DeleteDC (pix_dc);

    return status;
}


void ez_pixmap_draw_hmap (HDC hdc_dst, Ez_pixmap *pix, int x, int y)
{
    HDC hdc = NULL;

    /* Create a DC for the bitmap */
    hdc = CreateCompatibleDC(hdc_dst);
    if (hdc == NULL) {
        ez_error ("ez_pixmap_draw_hmap: can't create compatible DC\n");
        return;
    }
    SelectObject (hdc, pix->hmap);

    if (pix->has_alpha) {
        BLENDFUNCTION bf;

        bf.BlendOp = AC_SRC_OVER;
        bf.BlendFlags = 0;
        bf.AlphaFormat = AC_SRC_ALPHA;  /* Alpha channel premultiplied */
        bf.SourceConstantAlpha = 0xff;

        if (AlphaBlend (hdc_dst, x, y, pix->width, pix->height, hdc,
                0, 0, pix->width, pix->height, bf) == FALSE)
            ez_error ("ez_pixmap_draw_hmap: AlphaBlend failed\n");
    } else {
        if (BitBlt (hdc_dst, x, y, pix->width, pix->height, hdc,
                0, 0, SRCCOPY ) == 0)
            ez_error ("ez_pixmap_draw_hmap: bitblt failed\n");
    }

    if (hdc != NULL) DeleteDC (hdc);
}


void ez_pixmap_tile_hmap (HDC hdc_dst, Ez_pixmap *pix, int x, int y, int w, int h)
{
    HDC hdc = NULL;
    int nx, ny, nw, nh;
    BLENDFUNCTION bf;

    /* Create a DC for the bitmap */
    hdc = CreateCompatibleDC(hdc_dst);
    if (hdc == NULL) {
        ez_error ("ez_pixmap_tile_hmap: can't create compatible DC\n");
        return;
    }
    SelectObject (hdc, pix->hmap);

    if (pix->has_alpha) {
        bf.BlendOp = AC_SRC_OVER;
        bf.BlendFlags = 0;
        bf.AlphaFormat = AC_SRC_ALPHA;  /* Alpha channel premultiplied */
        bf.SourceConstantAlpha = 0xff;
    }

    for (ny = 0; ny < h; ny += pix->height)
    for (nx = 0; nx < w; nx += pix->width)
    {
        nw = nx+pix->width  <= w ? pix->width  : w-nx;
        nh = ny+pix->height <= h ? pix->height : h-ny;

        if (pix->has_alpha) {
            if (AlphaBlend (hdc_dst, x+nx, y+ny, nw, nh, hdc,
                    0, 0, nw, nh, bf) == FALSE) {
                ez_error ("ez_pixmap_tile_hmap: AlphaBlend failed\n");
                goto final;
            }
        } else {
            if (BitBlt (hdc_dst, x+nx, y+ny, nw, nh, hdc, 0, 0, SRCCOPY ) == 0) {
                ez_error ("ez_pixmap_tile_hmap: bitblt failed\n");
                goto final;
            }
        }
    }

  final:
    if (hdc != NULL) DeleteDC (hdc);
}

#endif /* EZ_BASE_ */


/*---------------------------------------------------------------------------
 *
 * Image files loading.
 *
 * Code extracted from stb_image.c version 1.33 - public domain JPEG/PNG reader
 * Original source and authors:  http://nothings.org/stb_image.c
 *
 * Supported formats:
 *    JPEG baseline (no JPEG progressive)
 *    PNG 8-bit-per-channel only
 *    BMP non-1bpp, non-RLE
 *    GIF (*comp always reports as 4-channel)
 *
 * Main contributors (see original sources):
 *    Sean Barrett (jpeg, png, bmp)
 *    Jean-Marc Lienher (gif)
*/

/* To test with the original file stb_image.c , define USE_ORIGINAL_STBI */

#ifndef USE_ORIGINAL_STBI

/*
 *  Basic usage:
 *     int x,y,n;
 *     EZuint8 *data = ez_stbi_load (filename, &x, &y, &comp, req_comp);
 *     if (data != NULL) ...
 *     stbi_image_free (data)
 *
 *  Standard parameters:
 *     int *x       -- outputs image width in pixels
 *     int *y       -- outputs image height in pixels
 *     int *comp    -- outputs # of image components in image file
 *     int req_comp -- if non-zero, # of image components requested in result
 *
 *  The return value from an image loader is an 'EZuint8 *' which points
 *  to the pixel data. The pixel data consists of *y scanlines of *x pixels,
 *  with each pixel consisting of N interleaved 8-bit components; the first
 *  pixel pointed to is top-left-most in the image. There is no padding between
 *  image scanlines or between pixels, regardless of format. The number of
 *  components N is 'req_comp' if req_comp is non-zero, or *comp otherwise.
 *  If req_comp is non-zero, *comp has the number of components that _would_
 *  have been output otherwise. E.g. if you set req_comp to 4, you will always
 *  get RGBA output, but you can check *comp to easily see if it's opaque.
 *
 *  An output image with N components has the following components interleaved
 *  in this order in each pixel:
 *
 *      N=#comp     components
 *        1           grey
 *        2           grey, alpha
 *        3           red, green, blue
 *        4           red, green, blue, alpha
 *
 *  If image loading fails for any reason, the return value will be NULL,
 *  and *x, *y, *comp will be unchanged.
 *
 *  Paletted PNG, BMP and GIF images are automatically depalettized.
*/


#if defined (_MSC_VER) && _MSC_VER >= 0x1400
#define _CRT_SECURE_NO_WARNINGS /* suppress bogus warnings about fopen () */
#endif

#ifndef _MSC_VER
  #ifdef __cplusplus
  #define EZ_INLINE inline
  #else
  #define EZ_INLINE
  #endif
#else
  #define EZ_INLINE __forceinline
#endif

#ifdef _MSC_VER
#define EZ_STBI_HAS_LROTL
#endif

#ifdef EZ_STBI_HAS_LROTL
  #define EZ_LROT(x,y)  _lrotl(x,y)
#else
  #define EZ_LROT(x,y)  (((x) << (y)) | ((x) >> (32 - (y))))
#endif


/*
 *  Ez_stbi struct and ez_stbi_start_xxx functions
 *
 *  Ez_stbi structure is our basic context used by all images, so it
 *  contains all the IO context, plus some basic image information
*/

typedef struct {
    Ez_uint32 img_x, img_y;
    int img_n, img_out_n;

    Ez_stbi_io_callbacks io;
    void *io_user_data;

    int read_from_callbacks;
    int buflen;
    EZuint8 buffer_start[128];

    EZuint8 *img_buffer, *img_buffer_end;
    EZuint8 *img_buffer_original;
} Ez_stbi;


void ez_refill_buffer (Ez_stbi *s);


/* Initialize a memory-decode context */

void ez_stbi_start_mem (Ez_stbi *s, EZuint8 const *buffer, int len)
{
    s->io.read = NULL;
    s->read_from_callbacks = 0;
    s->img_buffer = s->img_buffer_original = (EZuint8 *) buffer;
    s->img_buffer_end = (EZuint8 *) buffer+len;
}


/* Initialize a callback-based context */

void ez_stbi_start_callbacks (Ez_stbi *s, Ez_stbi_io_callbacks *c, void *user)
{
    s->io = *c;
    s->io_user_data = user;
    s->buflen = sizeof (s->buffer_start);
    s->read_from_callbacks = 1;
    s->img_buffer_original = s->buffer_start;
    ez_refill_buffer (s);
}


int ez_io_read (void *user, char *data, int size)
{
    return (int) fread (data, 1, size, (FILE*) user);
}

void ez_io_skip (void *user, unsigned n)
{
    fseek ((FILE*) user, n, SEEK_CUR);
}

int ez_io_eof (void *user)
{
    return feof ((FILE*) user);
}

Ez_stbi_io_callbacks ez_stbi_io_callbacks =
{
    ez_io_read,
    ez_io_skip,
    ez_io_eof,
};

void ez_start_file (Ez_stbi *s, FILE *f)
{
    ez_stbi_start_callbacks (s, &ez_stbi_io_callbacks, (void *) f);
}


void ez_stbi_rewind (Ez_stbi *s)
{
    /* Conceptually rewind SHOULD rewind to the beginning of the stream, but
       we just rewind to the beginning of the initial buffer, because we only
       use it after doing 'test', which only ever looks at at most 92 bytes */
    s->img_buffer = s->img_buffer_original;
}


int       ez_stbi_jpeg_test (Ez_stbi *s);
EZuint8 *ez_stbi_jpeg_load (Ez_stbi *s, int *x, int *y, int *comp, int req_comp);
int       ez_stbi_jpeg_info (Ez_stbi *s, int *x, int *y, int *comp);
int       ez_stbi_png_test  (Ez_stbi *s);
EZuint8 *ez_stbi_png_load  (Ez_stbi *s, int *x, int *y, int *comp, int req_comp);
int       ez_stbi_png_info  (Ez_stbi *s, int *x, int *y, int *comp);
int       ez_stbi_bmp_test  (Ez_stbi *s);
EZuint8 *ez_stbi_bmp_load  (Ez_stbi *s, int *x, int *y, int *comp, int req_comp);
int       ez_stbi_bmp_info  (Ez_stbi *s, int *x, int *y, int *comp);
int       ez_stbi_gif_test  (Ez_stbi *s);
EZuint8 *ez_stbi_gif_load  (Ez_stbi *s, int *x, int *y, int *comp, int req_comp);
int       ez_stbi_gif_info  (Ez_stbi *s, int *x, int *y, int *comp);


void ez_stbi_image_free (void *retval_from_stbi_load)
{
    free (retval_from_stbi_load);
}


EZuint8 *ez_stbi_load_main (Ez_stbi *s, int *x, int *y, int *comp,
    int req_comp)
{
    if (ez_stbi_jpeg_test (s)) return ez_stbi_jpeg_load (s, x, y, comp, req_comp);
    if (ez_stbi_png_test (s))  return ez_stbi_png_load  (s, x, y, comp, req_comp);
    if (ez_stbi_bmp_test (s))  return ez_stbi_bmp_load  (s, x, y, comp, req_comp);
    if (ez_stbi_gif_test (s))  return ez_stbi_gif_load  (s, x, y, comp, req_comp);

    ez_error ("ez_stbi_load_main: image not of any known type, or corrupt\n");
    return NULL;
}


EZuint8 *ez_stbi_load (char const *filename, int *x, int *y, int *comp,
    int req_comp)
{
    FILE *f = fopen (filename, "rb");
    EZuint8 *result;
    if (!f) {
        ez_error ("ez_stbi_load: unable to open file \"%s\"\n", filename);
        return NULL;
    }
    result = ez_stbi_load_from_file (f, x, y, comp, req_comp);
    fclose (f);
    return result;
}


EZuint8 *ez_stbi_load_from_file (FILE *f, int *x, int *y, int *comp,
    int req_comp)
{
    Ez_stbi s;
    ez_start_file (&s, f);
    return ez_stbi_load_main (&s, x, y, comp, req_comp);
}


EZuint8 *ez_stbi_load_from_memory (EZuint8 const *buffer, int len,
    int *x, int *y, int *comp, int req_comp)
{
    Ez_stbi s;
    ez_stbi_start_mem (&s, buffer, len);
    return ez_stbi_load_main (&s, x, y, comp, req_comp);
}


EZuint8 *ez_stbi_load_from_callbacks (Ez_stbi_io_callbacks const *clbk,
    void *user, int *x, int *y, int *comp, int req_comp)
{
    Ez_stbi s;
    ez_stbi_start_callbacks (&s, (Ez_stbi_io_callbacks *) clbk, user);
    return ez_stbi_load_main (&s, x, y, comp, req_comp);
}


/*
 * Common code used by all image loaders
*/

enum {
    EZ_SCAN_LOAD = 0,
    EZ_SCAN_TYPE,
    EZ_SCAN_HEADER
};


void ez_refill_buffer (Ez_stbi *s)
{
    int n = (s->io.read) (s->io_user_data, (char*)s->buffer_start, s->buflen);
    if (n == 0) {
        /* At end of file, treat same as if from memory */
        s->read_from_callbacks = 0;
        s->img_buffer = s->img_buffer_end-1;
        *s->img_buffer = 0;
    } else {
        s->img_buffer = s->buffer_start;
        s->img_buffer_end = s->buffer_start + n;
    }
}


EZ_INLINE int ez_buffer_get8 (Ez_stbi *s)
{
    if (s->img_buffer < s->img_buffer_end)
        return *s->img_buffer++;
    if (s->read_from_callbacks) {
        ez_refill_buffer (s);
        return *s->img_buffer++;
    }
    return 0;
}


EZ_INLINE int ez_buffer_at_eof (Ez_stbi *s)
{
    if (s->io.read) {
        if (! (s->io.eof) (s->io_user_data)) return 0;
        /* If feof () is true, check if buffer = end.
           Special case: we've only got the special 0 character at the end */
        if (s->read_from_callbacks == 0) return 1;
    }

    return s->img_buffer >= s->img_buffer_end;
}


EZ_INLINE EZuint8 ez_buffer_get8u (Ez_stbi *s)
{
    return (EZuint8) ez_buffer_get8 (s);
}


void ez_buffer_skip (Ez_stbi *s, int n)
{
    if (s->io.read) {
        int blen = s->img_buffer_end - s->img_buffer;
        if (blen < n) {
            s->img_buffer = s->img_buffer_end;
            (s->io.skip) (s->io_user_data, n - blen);
            return;
        }
    }
    s->img_buffer += n;
}


int ez_buffer_getn (Ez_stbi *s, EZuint8 *buffer, int n)
{
    if (s->io.read) {
        int blen = s->img_buffer_end - s->img_buffer;
        if (blen < n) {
            int res, count;

            memcpy (buffer, s->img_buffer, blen);
            count = (s->io.read) (s->io_user_data,
                        (char*) buffer + blen, n - blen);
            res = (count == (n-blen));
            s->img_buffer = s->img_buffer_end;
            return res;
        }
    }

    if (s->img_buffer+n <= s->img_buffer_end) {
        memcpy (buffer, s->img_buffer, n);
        s->img_buffer += n;
        return 1;
    } else
        return 0;
}


int ez_buffer_get16 (Ez_stbi *s)
{
    int z = ez_buffer_get8 (s);
    return (z << 8) + ez_buffer_get8 (s);
}


Ez_uint32 ez_buffer_get32 (Ez_stbi *s)
{
    Ez_uint32 z = ez_buffer_get16 (s);
    return (z << 16) + ez_buffer_get16 (s);
}


int ez_buffer_get16le (Ez_stbi *s)
{
    int z = ez_buffer_get8 (s);
    return z + (ez_buffer_get8 (s) << 8);
}


Ez_uint32 ez_buffer_get32le (Ez_stbi *s)
{
    Ez_uint32 z = ez_buffer_get16le (s);
    return z + (ez_buffer_get16le (s) << 16);
}


/*
 * Generic converter from built-in img_n to req_comp.
 *
 * Individual types do this automatically as much as possible (e.g. jpeg
 * does all cases internally since it needs to colorspace convert anyway,
 * and it never has alpha, so very few cases). png can automatically
 * interleave an alpha=255 channel, but falls back to this for other cases.
 *
 * Assume data buffer is malloced, so malloc a new one and free that one
 * only failure mode is malloc failing.
*/

EZuint8 ez_convert_comp_y (int r, int g, int b)
{
    return (EZuint8) (( (r*77) + (g*150) + (29*b)) >> 8);
}


EZuint8 *ez_convert_format (EZuint8 *data, int img_n, int req_comp,
    Ez_uint x, Ez_uint y)
{
    int i, j;
    EZuint8 *good;

    if (req_comp == img_n) return data;
    if (req_comp < 1 || req_comp > 4) {
        ez_error ("ez_convert_format: req_comp %d not in range 1..4\n", req_comp);
        return NULL;
    }

    good = (EZuint8*) malloc (req_comp * x * y);
    if (good == NULL) {
        free (data);
        ez_error ("ez_convert_format: out of memory\n");
        return NULL;
    }

    for (j=0; j < (int) y; ++j) {
        EZuint8 *src  = data + j * x * img_n   ;
        EZuint8 *dest = good + j * x * req_comp;

        #define COMBO(a,b)  ((a)*8+ (b))
        #define CASE(a,b)   case COMBO(a,b): \
            for (i=x-1; i >= 0; --i, src += a, dest += b)

        /* Convert source image with img_n components to one with req_comp
           components; avoid switch per pixel, so use switch per scanline
           and massive macros */
        switch (COMBO (img_n, req_comp)) {

            CASE (1,2) dest[0]=src[0], dest[1]=255;
             break;
            CASE (1,3) dest[0]=dest[1]=dest[2]=src[0];
             break;
            CASE (1,4) dest[0]=dest[1]=dest[2]=src[0], dest[3]=255;
             break;
            CASE (2,1) dest[0]=src[0];
             break;
            CASE (2,3) dest[0]=dest[1]=dest[2]=src[0];
             break;
            CASE (2,4) dest[0]=dest[1]=dest[2]=src[0], dest[3]=src[1];
             break;
            CASE (3,4) dest[0]=src[0], dest[1]=src[1], dest[2]=src[2],
                       dest[3]=255;
             break;
            CASE (3,1) dest[0]=ez_convert_comp_y (src[0], src[1], src[2]);
             break;
            CASE (3,2) dest[0]=ez_convert_comp_y (src[0], src[1], src[2]),
                       dest[1] = 255;
             break;
            CASE (4,1) dest[0]=ez_convert_comp_y (src[0], src[1], src[2]);
             break;
            CASE (4,2) dest[0]=ez_convert_comp_y (src[0], src[1], src[2]),
                       dest[1] = src[3];
             break;
            CASE (4,3) dest[0]=src[0], dest[1]=src[1], dest[2]=src[2];
             break;

            default:
                ez_error ("ez_convert_format: internal error for case (%d, %d)\n",
                    img_n, req_comp);
                return NULL;
        }
        #undef CASE
    }

    free (data);
    return good;
}


/*
 * "baseline" JPEG/JFIF decoder (not actually fully baseline implementation).
 *
 * Simple implementation:
 *     - channel subsampling of at most 2 in each dimension
 *     - doesn't support delayed output of y-dimension
 *     - simple interface (only one output format: 8-bit interleaved RGB)
 *     - doesn't try to recover corrupt jpegs
 *     - doesn't allow partial loading, loading multiple at once
 *     - still fast on x86 (copying globals into locals doesn't help x86)
 *     - allocates lots of intermediate memory (full size of all components)
 *     - non-interleaved case requires this anyway
 *     - allows good upsampling (see next)
 * High-quality:
 *     - upsampled channels are bilinearly interpolated, even across blocks
 *     - quality integer IDCT derived from IJG's 'slow'
 * Performance
 *     - fast huffman; reasonable integer IDCT
 *     - uses a lot of intermediate memory, could cache poorly
 *     - load http://nothings.org/remote/anemones.jpg 3 times on 2.8Ghz P4
 *         stb_jpeg:   1.34 seconds (MSVC6, default release build)
 *         stb_jpeg:   1.06 seconds (MSVC6, processor = Pentium Pro)
 *         IJL11.dll:  1.08 seconds (compiled by intel)
 *         IJG 1998:   0.98 seconds (MSVC6, makefile provided by IJG)
 *         IJG 1998:   0.95 seconds (MSVC6, makefile + proc=PPro)
*/

/* Huffman decoding acceleration */
#define EZ_FAST_BITS 9  /* larger handles more cases; smaller stomps less cache */

typedef struct {
    EZuint8  fast[1 << EZ_FAST_BITS];
    /* Weirdly, repacking this into AoS is a 10% speed loss, instead of a win */
    Ez_uint16 code[256];
    EZuint8  values[256];
    EZuint8  size[257];
    unsigned int maxcode[18];
    int       delta[17];   /* old 'firstsymbol' - old 'firstcode' */
} Ez_huffman;

typedef struct {
    Ez_stbi *s;
    Ez_huffman huff_dc[4];
    Ez_huffman huff_ac[4];
    EZuint8 dequant[4][64];

    /* Sizes for components, interleaved MCUs */
    int img_h_max, img_v_max;
    int img_mcu_x, img_mcu_y;
    int img_mcu_w, img_mcu_h;

    /* Definition of jpeg image component */
    struct {
        int id;
        int h, v;
        int tq;
        int hd, ha;
        int dc_pred;

        int x, y, w2, h2;
        EZuint8 *data;
        void *raw_data;
        EZuint8 *linebuf;
    } img_comp[4];

    Ez_uint32  code_buffer; /* jpeg entropy-coded buffer */
    int        code_bits;   /* number of valid bits */
    EZuint8   marker;      /* marker seen while filling entropy buffer */
    int        nomore;      /* flag if we saw a marker so must stop */

    int scan_n, order[4];
    int restart_interval, todo;
} Ez_jpeg;


int ez_build_huffman (Ez_huffman *h, int *count)
{
    int i, j, k=0, code;
    /* Build size list for each symbol (from JPEG spec) */
    for (i=0; i < 16; ++i)
        for (j=0; j < count[i]; ++j)
            h->size[k++] = (EZuint8) (i+1);
    h->size[k] = 0;

    /* Compute actual symbols (from jpeg spec) */
    code = 0;
    k = 0;
    for (j=1; j <= 16; ++j) {
        /* Compute delta to add to code to compute symbol id */
        h->delta[j] = k - code;
        if (h->size[k] == j) {
            while (h->size[k] == j)
                h->code[k++] = (Ez_uint16) (code++);
            if (code-1 >= (1 << j)) {
               ez_error ("ez_build_huffman: corrupt JPEG: bad code lengths\n");
               return 0;
            }
        }
        /* compute largest code + 1 for this size, preshifted as needed later */
        h->maxcode[j] = code << (16-j);
        code <<= 1;
    }
    h->maxcode[j] = 0xffffffff;

    /* Build non-spec acceleration table; 255 is flag for not-accelerated */
    memset (h->fast, 255, 1 << EZ_FAST_BITS);
    for (i=0; i < k; ++i) {
        int s = h->size[i];
        if (s <= EZ_FAST_BITS) {
            int c = h->code[i] << (EZ_FAST_BITS-s);
            int m = 1 << (EZ_FAST_BITS-s);
            for (j=0; j < m; ++j) {
                h->fast[c+j] = (EZuint8) i;
            }
        }
    }
    return 1;
}


void ez_grow_buffer_unsafe (Ez_jpeg *j)
{
    do {
        int b = j->nomore ? 0 : ez_buffer_get8 (j->s);
        if (b == 0xff) {
            int c = ez_buffer_get8 (j->s);
            if (c != 0) {
                j->marker = (EZuint8) c;
                j->nomore = 1;
                return;
            }
        }
        j->code_buffer |= b << (24 - j->code_bits);
        j->code_bits += 8;
    } while (j->code_bits <= 24);
}


/* (1 << n) - 1 */
Ez_uint32 bmask[17] = {0, 1, 3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095,
    8191, 16383, 32767, 65535};


/* Decode a jpeg huffman value from the bitstream */

EZ_INLINE int ez_jpeg_decode (Ez_jpeg *j, Ez_huffman *h)
{
    unsigned int temp;
    int c, k;

    if (j->code_bits < 16) ez_grow_buffer_unsafe (j);

    /* Look at the top EZ_FAST_BITS and determine what symbol ID it is,
       if the code is <= EZ_FAST_BITS */
    c = (j->code_buffer >> (32 - EZ_FAST_BITS)) & ((1 << EZ_FAST_BITS)-1);
    k = h->fast[c];
    if (k < 255) {
        int s = h->size[k];
        if (s > j->code_bits)
            return -1;
        j->code_buffer <<= s;
        j->code_bits -= s;
        return h->values[k];
    }

    /* Naive test is to shift the code_buffer down so k bits are
       valid, then test against maxcode. To speed this up, we've
       preshifted maxcode left so that it has (16-k) 0s at the
       end; in other words, regardless of the number of bits, it
       wants to be compared against something shifted to have 16;
       that way we don't need to shift inside the loop. */
    temp = j->code_buffer >> 16;
    for (k=EZ_FAST_BITS+1 ; ; ++k)
        if (temp < h->maxcode[k])
            break;
    if (k == 17) {
        /* error! code not found */
        j->code_bits -= 16;
        return -1;
    }

    if (k > j->code_bits)
        return -1;

    /* Convert the huffman code to the symbol id */
    c = ((j->code_buffer >> (32 - k)) & bmask[k]) + h->delta[k];
    if (( ((j->code_buffer) >> (32-h->size[c])) & bmask[h->size[c]]) != h->code[c]) {
        ez_error ("ez_jpeg_decode: internal error while converting huffman code\n");
        return 0;
    }

    /* Convert the id to a symbol */
    j->code_bits -= k;
    j->code_buffer <<= k;
    return h->values[c];
}


/* Combined JPEG 'receive' and JPEG 'extend', since baseline
   always extends everything it receives. */

EZ_INLINE int ez_jpeg_extend_receive (Ez_jpeg *j, int n)
{
    unsigned int m = 1 << (n-1);
    unsigned int k;
    if (j->code_bits < n) ez_grow_buffer_unsafe (j);

    k = EZ_LROT (j->code_buffer, n);
    j->code_buffer = k & ~bmask[n];
    k &= bmask[n];
    j->code_bits -= n;

    /* The following test is probably a random branch that won't
       predict well. I tried to table accelerate it but failed.
       maybe it's compiling as a conditional move? */
    if (k < m)
         return ( ((unsigned int)-1) << n) + k + 1;
    else return k;
}


/* Given a value that's at position X in the zigzag stream,
   where does it appear in the 8x8 matrix coded as row-major? */

EZuint8 dezigzag[64+15] =
{
    0,  1,  8, 16,  9,  2,  3, 10,
   17, 24, 32, 25, 18, 11,  4,  5,
   12, 19, 26, 33, 40, 48, 41, 34,
   27, 20, 13,  6,  7, 14, 21, 28,
   35, 42, 49, 56, 57, 50, 43, 36,
   29, 22, 15, 23, 30, 37, 44, 51,
   58, 59, 52, 45, 38, 31, 39, 46,
   53, 60, 61, 54, 47, 55, 62, 63,
   /* let corrupt input sample past end */
   63, 63, 63, 63, 63, 63, 63, 63,
   63, 63, 63, 63, 63, 63, 63
};

/* Decode one 64-entry block */

int ez_jpeg_decode_block (Ez_jpeg *j, short data[64], Ez_huffman *hdc,
    Ez_huffman *hac, int b)
{
    int diff, dc, k;
    int t = ez_jpeg_decode (j, hdc);
    if (t < 0) {
        ez_error ("ez_jpeg_decode_block: corrupt JPEG: bad huffman code\n");
        return 0;
    }

    /* 0 all the ac values now so we can do it 32-bits at a time */
    memset (data, 0, 64*sizeof (data[0]));

    diff = t ? ez_jpeg_extend_receive (j, t) : 0;
    dc = j->img_comp[b].dc_pred + diff;
    j->img_comp[b].dc_pred = dc;
    data[0] = (short) dc;

    /* Decode AC components, see JPEG spec */
    k = 1;
    do {
        int r, s;
        int rs = ez_jpeg_decode (j, hac);
        if (rs < 0)  {
            ez_error ("ez_jpeg_decode_block: corrupt JPEG: bad huffman code\n");
            return 0;
        }
        s = rs & 15;
        r = rs >> 4;
        if (s == 0) {
            if (rs != 0xf0) break; /* end block */
            k += 16;
        } else {
            k += r;
            /* decode into unzigzag'd location */
            data[dezigzag[k++]] = (short) ez_jpeg_extend_receive (j, s);
        }
    } while (k < 64);
    return 1;
}


/* Take a -128..127 value and clamp it and convert to 0..255 */

EZ_INLINE EZuint8 ez_jpeg_clamp (int x)
{
    /* Trick to use a single test to catch both cases */
    if ((unsigned int) x > 255) {
        if (x < 0) return 0;
        if (x > 255) return 255;
    }
    return (EZuint8) x;
}


#define Ez_f2f(x)  (int)(((x) * 4096 + 0.5))
#define Ez_fsh(x)  ((x) << 12)

/* derived from jidctint -- DCT_ISLOW */
#define EZ_IDCT_1D(s0,s1,s2,s3,s4,s5,s6,s7)     \
    int t0, t1, t2, t3, p1, p2, p3, p4, p5, x0, x1, x2, x3; \
    p2 = s2;                                    \
    p3 = s6;                                    \
    p1 = (p2+p3) * Ez_f2f (0.5411961f);         \
    t2 = p1 + p3*Ez_f2f (-1.847759065f);        \
    t3 = p1 + p2*Ez_f2f ( 0.765366865f);        \
    p2 = s0;                                    \
    p3 = s4;                                    \
    t0 = Ez_fsh (p2+p3);                        \
    t1 = Ez_fsh (p2-p3);                        \
    x0 = t0+t3;                                 \
    x3 = t0-t3;                                 \
    x1 = t1+t2;                                 \
    x2 = t1-t2;                                 \
    t0 = s7;                                    \
    t1 = s5;                                    \
    t2 = s3;                                    \
    t3 = s1;                                    \
    p3 = t0+t2;                                 \
    p4 = t1+t3;                                 \
    p1 = t0+t3;                                 \
    p2 = t1+t2;                                 \
    p5 = (p3+p4)*Ez_f2f ( 1.175875602f);        \
    t0 = t0*Ez_f2f ( 0.298631336f);             \
    t1 = t1*Ez_f2f ( 2.053119869f);             \
    t2 = t2*Ez_f2f ( 3.072711026f);             \
    t3 = t3*Ez_f2f ( 1.501321110f);             \
    p1 = p5 + p1*Ez_f2f (-0.899976223f);        \
    p2 = p5 + p2*Ez_f2f (-2.562915447f);        \
    p3 = p3*Ez_f2f (-1.961570560f);             \
    p4 = p4*Ez_f2f (-0.390180644f);             \
    t3 += p1+p4;                                \
    t2 += p2+p3;                                \
    t1 += p2+p4;                                \
    t0 += p1+p3;

typedef EZuint8 Ez_stbi_dequantize_t;


/* .344 seconds on 3*anemones.jpg */

void ez_jpeg_idct_block (EZuint8 *out, int out_stride, short data[64],
    Ez_stbi_dequantize_t *dequantize)
{
    int i, val[64], *v=val;
    Ez_stbi_dequantize_t *dq = dequantize;
    EZuint8 *o;
    short *d = data;

    /* Columns */
    for (i=0; i < 8; ++i, ++d, ++dq, ++v) {
        /* If all zeroes, shortcut -- this avoids dequantizing 0s and IDCTing */
        if (d[ 8]==0 && d[16]==0 && d[24]==0 && d[32]==0 &&
            d[40]==0 && d[48]==0 && d[56]==0)
        {
            /* no shortcut                 0     seconds
               (1|2|3|4|5|6|7)==0          0     seconds
               all separate               -0.047 seconds
               1 && 2|3 && 4|5 && 6|7:    -0.047 seconds */
            int dcterm = d[0] * dq[0] << 2;
            v[0] = v[8] = v[16] = v[24] = v[32] = v[40] = v[48] = v[56] = dcterm;
        } else {
            EZ_IDCT_1D (d[ 0]*dq[ 0], d[ 8]*dq[ 8], d[16]*dq[16], d[24]*dq[24],
                     d[32]*dq[32], d[40]*dq[40], d[48]*dq[48], d[56]*dq[56])
            /* Constants scaled things up by 1<<12; let's bring them back
               down, but keep 2 extra bits of precision */
            x0 += 512; x1 += 512; x2 += 512; x3 += 512;
            v[ 0] = (x0+t3) >> 10;
            v[56] = (x0-t3) >> 10;
            v[ 8] = (x1+t2) >> 10;
            v[48] = (x1-t2) >> 10;
            v[16] = (x2+t1) >> 10;
            v[40] = (x2-t1) >> 10;
            v[24] = (x3+t0) >> 10;
            v[32] = (x3-t0) >> 10;
        }
    }

    for (i=0, v=val, o=out; i < 8; ++i, v+=8, o+=out_stride) {
        /* No fast case since the first 1D IDCT spread components out */
        EZ_IDCT_1D (v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7])
        /* Constants scaled things up by 1<<12, plus we had 1<<2 from first
           loop, plus horizontal and vertical each scale by sqrt (8) so together
           we've got an extra 1<<3, so 1<<17 total we need to remove.
           so we want to round that, which means adding 0.5 * 1<<17,
           aka 65536. Also, we'll end up with -128 to 127 that we want to
           encode as 0..255 by adding 128, so we'll add that before the shift */
        x0 += 65536 + (128<<17);
        x1 += 65536 + (128<<17);
        x2 += 65536 + (128<<17);
        x3 += 65536 + (128<<17);
        /* Tried computing the shifts into temps, or'ing the temps to see
           if any were out of range, but that was slower */
        o[0] = ez_jpeg_clamp ((x0+t3) >> 17);
        o[7] = ez_jpeg_clamp ((x0-t3) >> 17);
        o[1] = ez_jpeg_clamp ((x1+t2) >> 17);
        o[6] = ez_jpeg_clamp ((x1-t2) >> 17);
        o[2] = ez_jpeg_clamp ((x2+t1) >> 17);
        o[5] = ez_jpeg_clamp ((x2-t1) >> 17);
        o[3] = ez_jpeg_clamp ((x3+t0) >> 17);
        o[4] = ez_jpeg_clamp ((x3-t0) >> 17);
    }
}


#define EZ_MARKER_NONE  0xff

/* If there's a pending marker from the entropy stream, return that
   otherwise, fetch from the stream and get a marker. if there's no
   marker, return 0xff, which is never a valid marker value */

EZuint8 ez_jpeg_get_marker (Ez_jpeg *j)
{
    EZuint8 x;
    if (j->marker != EZ_MARKER_NONE) {
        x = j->marker; j->marker = EZ_MARKER_NONE; return x;
    }
    x = ez_buffer_get8u (j->s);
    if (x != 0xff) return EZ_MARKER_NONE;
    while (x == 0xff)
        x = ez_buffer_get8u (j->s);
    return x;
}


/* In each scan, we'll have scan_n components, and the order
   of the components is specified by order[] */
#define EZ_RESTART(x)     ((x) >= 0xd0 && (x) <= 0xd7)


/* After a restart interval, reset the entropy decoder and
   the dc prediction */

void ez_jpeg_reset (Ez_jpeg *j)
{
    j->code_bits = 0;
    j->code_buffer = 0;
    j->nomore = 0;
    j->img_comp[0].dc_pred = j->img_comp[1].dc_pred = j->img_comp[2].dc_pred = 0;
    j->marker = EZ_MARKER_NONE;
    j->todo = j->restart_interval ? j->restart_interval : 0x7fffffff;
    /* No more than 1<<31 MCUs if no restart_interal? that's plenty safe,
       since we don't even allow 1<<30 pixels */
}


int ez_jpeg_parse_entropy_coded_data (Ez_jpeg *z)
{
    ez_jpeg_reset (z);
    if (z->scan_n == 1) {
        int i, j;
        short data[64];
        int n = z->order[0];
        /* Non-interleaved data, we just need to process one block at a time,
           in trivial scanline order
           number of blocks to do just depends on how many actual "pixels" this
           component has, independent of interleaved MCU blocking and such */
        int w = (z->img_comp[n].x+7) >> 3;
        int h = (z->img_comp[n].y+7) >> 3;
        for (j=0; j < h; ++j) {
            for (i=0; i < w; ++i) {
                if (!ez_jpeg_decode_block (z, data, z->huff_dc+z->img_comp[n].hd,
                    z->huff_ac+z->img_comp[n].ha, n)) return 0;
                ez_jpeg_idct_block (z->img_comp[n].data+z->img_comp[n].w2*j*8+i*8,
                    z->img_comp[n].w2, data, z->dequant[z->img_comp[n].tq]);

                /* Every data block is an MCU, so countdown the restart interval */
                if (--z->todo <= 0) {
                    if (z->code_bits < 24) ez_grow_buffer_unsafe (z);
                    /* If it's NOT a restart, then just bail, so we get corrupt
                       data rather than no data */
                    if (!EZ_RESTART (z->marker)) return 1;
                    ez_jpeg_reset (z);
                }
            }
        }
    } else { /* interleaved! */
        int i, j, k, x, y;
        short data[64];
        for (j=0; j < z->img_mcu_y; ++j) {
            for (i=0; i < z->img_mcu_x; ++i) {
                /* Scan an interleaved mcu... process scan_n components in order */
                for (k=0; k < z->scan_n; ++k) {
                    int n = z->order[k];
                    /* Scan out an mcu's worth of this component; that's just
                       determined by the basic H and V specified for the component */
                    for (y=0; y < z->img_comp[n].v; ++y) {
                        for (x=0; x < z->img_comp[n].h; ++x) {
                            int x2 = (i*z->img_comp[n].h + x)*8;
                            int y2 = (j*z->img_comp[n].v + y)*8;
                            if (!ez_jpeg_decode_block (z, data,
                                z->huff_dc+z->img_comp[n].hd,
                                z->huff_ac+z->img_comp[n].ha, n)) return 0;

                            ez_jpeg_idct_block (
                                z->img_comp[n].data+z->img_comp[n].w2*y2+x2,
                                z->img_comp[n].w2, data,
                                z->dequant[z->img_comp[n].tq]);
                        }
                    }
                }
                /* After all interleaved components, that's an interleaved MCU,
                   so now count down the restart interval */
                if (--z->todo <= 0) {
                    if (z->code_bits < 24) ez_grow_buffer_unsafe (z);
                    /* If it's NOT a restart, then just bail, so we get corrupt
                       data rather than no data */
                    if (!EZ_RESTART (z->marker)) return 1;
                    ez_jpeg_reset (z);
                }
            }
        }
    }
    return 1;
}


int ez_jpeg_process_marker (Ez_jpeg *z, int m)
{
    int L;
    switch (m) {
        case EZ_MARKER_NONE: /* no marker found */
            ez_error ("ez_jpeg_process_marker: corrupt JPEG: expected marker\n");
            return 0;

        case 0xC2: /* SOF - progressive */
            ez_error ("ez_jpeg_process_marker: JPEG format not supported (progressive)\n");
            return 0;

        case 0xDD: /* DRI - specify restart interval */
            if (ez_buffer_get16 (z->s) != 4) {
                ez_error ("ez_jpeg_process_marker: corrupt JPEG: bad DRI len\n");
                return 0;
            }
            z->restart_interval = ez_buffer_get16 (z->s);
            return 1;

        case 0xDB: /* DQT - define quantization table */
            L = ez_buffer_get16 (z->s)-2;
            while (L > 0) {
                int q = ez_buffer_get8 (z->s);
                int p = q >> 4;
                int t = q & 15, i;
                if (p != 0) {
                    ez_error ("ez_jpeg_process_marker: corrupt JPEG: bad DQT type\n");
                    return 0;
                }
                if (t > 3) {
                    ez_error ("ez_jpeg_process_marker: corrupt JPEG: bad DQT table\n");
                    return 0;
                }
                for (i=0; i < 64; ++i)
                    z->dequant[t][dezigzag[i]] = ez_buffer_get8u (z->s);
                L -= 65;
            }
            return L==0;

        case 0xC4: /* DHT - define huffman table */
            L = ez_buffer_get16 (z->s)-2;
            while (L > 0) {
                EZuint8 *v;
                int sizes[16], i, m=0;
                int q = ez_buffer_get8 (z->s);
                int tc = q >> 4;
                int th = q & 15;
                if (tc > 1 || th > 3) {
                    ez_error ("ez_jpeg_process_marker: corrupt JPEG: bad DHT header\n");
                    return 0;
                }
                for (i=0; i < 16; ++i) {
                    sizes[i] = ez_buffer_get8 (z->s);
                    m += sizes[i];
                }
                L -= 17;
                if (tc == 0) {
                    if (!ez_build_huffman (z->huff_dc+th, sizes)) return 0;
                    v = z->huff_dc[th].values;
                } else {
                    if (!ez_build_huffman (z->huff_ac+th, sizes)) return 0;
                    v = z->huff_ac[th].values;
                }
                for (i=0; i < m; ++i)
                    v[i] = ez_buffer_get8u (z->s);
                L -= m;
            }
            return L==0;
    }

    /* check for comment block or APP blocks */
    if ((m >= 0xE0 && m <= 0xEF) || m == 0xFE) {
        ez_buffer_skip (z->s, ez_buffer_get16 (z->s)-2);
        return 1;
    }
   return 0;
}


/* after we see SOS */

int ez_jpeg_process_scan_header (Ez_jpeg *z)
{
    int i;
    int Ls = ez_buffer_get16 (z->s);
    z->scan_n = ez_buffer_get8 (z->s);
    if (z->scan_n < 1 || z->scan_n > 4 || z->scan_n > (int) z->s->img_n) {
        ez_error ("ez_jpeg_process_scan_header: corrupt JPEG: bad SOS component count\n");
        return 0;
    }
    if (Ls != 6+2*z->scan_n) {
        ez_error ("ez_jpeg_process_scan_header: corrupt JPEG: bad SOS len\n");
        return 0;
    }
    for (i=0; i < z->scan_n; ++i) {
        int id = ez_buffer_get8 (z->s), which;
        int q = ez_buffer_get8 (z->s);
        for (which = 0; which < z->s->img_n; ++which)
            if (z->img_comp[which].id == id)
                break;
        if (which == z->s->img_n) return 0;
        z->img_comp[which].hd = q >> 4;
        if (z->img_comp[which].hd > 3) {
            ez_error ("ez_jpeg_process_scan_header: corrupt JPEG: bad DC huff\n");
            return 0;
        }
        z->img_comp[which].ha = q & 15;
        if (z->img_comp[which].ha > 3) {
            ez_error ("ez_jpeg_process_scan_header: corrupt JPEG: bad AC huff\n");
            return 0;
        }
        z->order[i] = which;
    }
    if (ez_buffer_get8 (z->s) != 0) {
        ez_error ("ez_jpeg_process_scan_header: corrupt JPEG: bad SOS\n");
        return 0;
    }
    ez_buffer_get8 (z->s); /* should be 63, but might be 0 */
    if (ez_buffer_get8 (z->s) != 0) {
        ez_error ("ez_jpeg_process_scan_header: corrupt JPEG: bad SOS\n");
        return 0;
    }

    return 1;
}

int ez_jpeg_process_frame_header (Ez_jpeg *z, int scan)
{
    Ez_stbi *s = z->s;
    int Lf, p, i, q, h_max=1, v_max=1, c;
    Lf = ez_buffer_get16 (s); if (Lf < 11) {
        ez_error ("ez_jpeg_process_frame_header: corrupt JPEG: bad SOF len\n");
        return 0;
    }
    p = ez_buffer_get8 (s); if (p != 8) {
        ez_error ("ez_jpeg_process_frame_header: JPEG format not supported: 8-bit only\n");
        return 0;
    }
    s->img_y = ez_buffer_get16 (s); if (s->img_y == 0) {
        ez_error ("ez_jpeg_process_frame_header: JPEG format not supported: delayed height\n");
        return 0;
    }
    s->img_x = ez_buffer_get16 (s); if (s->img_x == 0) {
        ez_error ("ez_jpeg_process_frame_header: corrupt JPEG: 0 width\n");
        return 0;
    }
    c = ez_buffer_get8 (s);
    if (c != 3 && c != 1) {
        ez_error ("ez_jpeg_process_frame_header: corrupt JPEG: bad component count\n");
        return 0;
    }
    s->img_n = c;
    for (i=0; i < c; ++i) {
        z->img_comp[i].data = NULL;
        z->img_comp[i].linebuf = NULL;
    }

    if (Lf != 8+3*s->img_n) {
        ez_error ("ez_jpeg_process_frame_header: corrupt JPEG: bad SOF len\n");
        return 0;
    }

    for (i=0; i < s->img_n; ++i) {
        z->img_comp[i].id = ez_buffer_get8 (s);
        if (z->img_comp[i].id != i+1)   /* JFIF requires */
            if (z->img_comp[i].id != i) {
                /* Some version of jpegtran outputs non-JFIF-compliant files! */
                ez_error ("ez_jpeg_process_frame_header: corrupt JPEG: bad component ID\n");
                return 0;
            }
        q = ez_buffer_get8 (s);
        z->img_comp[i].h = (q >> 4);
        if (!z->img_comp[i].h || z->img_comp[i].h > 4) {
            ez_error ("ez_jpeg_process_frame_header: corrupt JPEG: bad H\n");
            return 0;
        }
        z->img_comp[i].v = q & 15;
        if (!z->img_comp[i].v || z->img_comp[i].v > 4)  {
            ez_error ("ez_jpeg_process_frame_header: corrupt JPEG: bad V\n");
            return 0;
        }
        z->img_comp[i].tq = ez_buffer_get8 (s);
        if (z->img_comp[i].tq > 3)  {
            ez_error ("ez_jpeg_process_frame_header: corrupt JPEG: bad TQ\n");
            return 0;
        }
    }

    if (scan != EZ_SCAN_LOAD) return 1;

    if ((1 << 30) / s->img_x / s->img_n < s->img_y)  {
        ez_error ("ez_jpeg_process_frame_header: JPEG image too large to decode\n");
        return 0;
    }

    for (i=0; i < s->img_n; ++i) {
        if (z->img_comp[i].h > h_max) h_max = z->img_comp[i].h;
        if (z->img_comp[i].v > v_max) v_max = z->img_comp[i].v;
    }

    /* Compute interleaved mcu info */
    z->img_h_max = h_max;
    z->img_v_max = v_max;
    z->img_mcu_w = h_max * 8;
    z->img_mcu_h = v_max * 8;
    z->img_mcu_x = (s->img_x + z->img_mcu_w-1) / z->img_mcu_w;
    z->img_mcu_y = (s->img_y + z->img_mcu_h-1) / z->img_mcu_h;

    for (i=0; i < s->img_n; ++i) {
        /* Number of effective pixels (e.g. for non-interleaved MCU) */
        z->img_comp[i].x = (s->img_x * z->img_comp[i].h + h_max-1) / h_max;
        z->img_comp[i].y = (s->img_y * z->img_comp[i].v + v_max-1) / v_max;
        /* To simplify generation, we'll allocate enough memory to decode
           the bogus oversized data from using interleaved MCUs and their
           big blocks (e.g. a 16x16 iMCU on an image of width 33); we won't
           discard the extra data until colorspace conversion */
        z->img_comp[i].w2 = z->img_mcu_x * z->img_comp[i].h * 8;
        z->img_comp[i].h2 = z->img_mcu_y * z->img_comp[i].v * 8;
        z->img_comp[i].raw_data = malloc (z->img_comp[i].w2 * z->img_comp[i].h2+15);
        if (z->img_comp[i].raw_data == NULL) {
            for (--i; i >= 0; --i) {
                free (z->img_comp[i].raw_data);
                z->img_comp[i].data = NULL;
            }
            ez_error ("ez_jpeg_process_frame_header: out of memory\n");
            return 0;
        }
        /* Align blocks for installable-idct using mmx/sse */
        z->img_comp[i].data =
            (EZuint8*) (( (size_t) z->img_comp[i].raw_data + 15) & ~15);
        z->img_comp[i].linebuf = NULL;
    }

    return 1;
}


/* Use comparisons since in some cases we handle more than one case (e.g. SOF) */
#define EZ_DNL(x)         ((x) == 0xdc)
#define EZ_SOI(x)         ((x) == 0xd8)
#define EZ_EOI(x)         ((x) == 0xd9)
#define EZ_SOF(x)         ((x) == 0xc0 || (x) == 0xc1)
#define EZ_SOS(x)         ((x) == 0xda)


int ez_jpeg_decode_header (Ez_jpeg *z, int scan, int verbose)
{
    int m;
    z->marker = EZ_MARKER_NONE; /* initialize cached marker to empty */
    m = ez_jpeg_get_marker (z);
    if (!EZ_SOI (m)) {
        if (verbose) ez_error ("ez_jpeg_decode_header: corrupt JPEG: no SOI\n");
        return 0;
    }
    if (scan == EZ_SCAN_TYPE) return 1;
    m = ez_jpeg_get_marker (z);
    while (!EZ_SOF (m)) {
        if (!ez_jpeg_process_marker (z, m)) return 0;
        m = ez_jpeg_get_marker (z);
        while (m == EZ_MARKER_NONE) {
            /* Some files have extra padding after their blocks, so ok, we'll scan */
            if (ez_buffer_at_eof (z->s)) {
                if (verbose) ez_error ("ez_jpeg_decode_header: corrupt JPEG: no SOF\n");
                return 0;
            }
            m = ez_jpeg_get_marker (z);
        }
    }
    if (!ez_jpeg_process_frame_header (z, scan)) return 0;
    return 1;
}


int ez_jpeg_decode_image (Ez_jpeg *j)
{
    int m;
    j->restart_interval = 0;
    if (!ez_jpeg_decode_header (j, EZ_SCAN_LOAD, 1)) return 0;
    m = ez_jpeg_get_marker (j);
    while (!EZ_EOI (m)) {
        if (EZ_SOS (m)) {
            if (!ez_jpeg_process_scan_header (j)) return 0;
            if (!ez_jpeg_parse_entropy_coded_data (j)) return 0;
            if (j->marker == EZ_MARKER_NONE ) {
                /* Handle 0s at the end of image data from IP Kamera 9060 */
                while (!ez_buffer_at_eof (j->s)) {
                    int x = ez_buffer_get8 (j->s);
                    if (x == 255) {
                        j->marker = ez_buffer_get8u (j->s);
                        break;
                    } else if (x != 0) {
                        return 0;
                    }
                }
                /* If we reach eof without hitting a marker, ez_jpeg_get_marker ()
                   below will fail and we'll eventually return 0 */
            }
        } else {
            if (!ez_jpeg_process_marker (j, m)) return 0;
        }
        m = ez_jpeg_get_marker (j);
    }
    return 1;
}


/* jfif-centered resampling (across block boundaries) */

typedef EZuint8* (*Ez_resample_row_func)
    (EZuint8 *out, EZuint8 *in0, EZuint8 *in1, int w, int hs);

#define div4(x) ((EZuint8) ((x) >> 2))


EZuint8 *ez_jpeg_resample_row_1 (EZuint8 *out, EZuint8 *in_near,
    EZuint8 *in_far, int w, int hs)
{
    (void) out;
    (void) in_far;
    (void) w;
    (void) hs;
    return in_near;
}

EZuint8* ez_jpeg_resample_row_v_2 (EZuint8 *out, EZuint8 *in_near,
    EZuint8 *in_far, int w, int hs)
{
    /* Need to generate two samples vertically for every one in input */
    int i;
    (void) hs;
    for (i=0; i < w; ++i)
        out[i] = div4 (3*in_near[i] + in_far[i] + 2);
    return out;
}


EZuint8*  ez_jpeg_resample_row_h_2 (EZuint8 *out, EZuint8 *in_near,
    EZuint8 *in_far, int w, int hs)
{
    /* Need to generate two samples horizontally for every one in input */
    int i;
    EZuint8 *input = in_near;

    if (w == 1) {
        /* If only one sample, can't do any interpolation */
        out[0] = out[1] = input[0];
        return out;
    }

    out[0] = input[0];
    out[1] = div4 (input[0]*3 + input[1] + 2);
    for (i=1; i < w-1; ++i) {
        int n = 3*input[i]+2;
        out[i*2+0] = div4 (n+input[i-1]);
        out[i*2+1] = div4 (n+input[i+1]);
    }
    out[i*2+0] = div4 (input[w-2]*3 + input[w-1] + 2);
    out[i*2+1] = input[w-1];

    (void) in_far;
    (void) hs;

    return out;
}


#define div16(x) ((EZuint8) ((x) >> 4))

EZuint8 *ez_jpeg_resample_row_hv_2 (EZuint8 *out, EZuint8 *in_near,
    EZuint8 *in_far, int w, int hs)
{
    /* Need to generate 2x2 samples for every one in input */
    int i, t0, t1;
    if (w == 1) {
        out[0] = out[1] = div4 (3*in_near[0] + in_far[0] + 2);
        return out;
    }

    t1 = 3*in_near[0] + in_far[0];
    out[0] = div4 (t1+2);
    for (i=1; i < w; ++i) {
        t0 = t1;
        t1 = 3*in_near[i]+in_far[i];
        out[i*2-1] = div16 (3*t0 + t1 + 8);
        out[i*2  ] = div16 (3*t1 + t0 + 8);
    }
    out[w*2-1] = div4 (t1+2);

    (void) hs;

    return out;
}


EZuint8 *ez_jpeg_resample_row_generic (EZuint8 *out, EZuint8 *in_near,
    EZuint8 *in_far, int w, int hs)
{
    /* Resample with nearest-neighbor */
    int i, j;
    (void) in_far;
    for (i=0; i < w; ++i)
        for (j=0; j < hs; ++j)
            out[i*hs+j] = in_near[i];
    return out;
}


#define Ez_float2fixed(x)  ((int) ((x) * 65536 + 0.5))

/* 0.38 seconds on 3*anemones.jpg   (0.25 with processor = Pro)
   VC6 without processor=Pro is generating multiple LEAs per multiply! */

void ez_jpeg_YCbCr_to_RGB_row (EZuint8 *out, const EZuint8 *y,
    const EZuint8 *pcb, const EZuint8 *pcr, int count, int step)
{
    int i;
    for (i=0; i < count; ++i) {
        int y_fixed = (y[i] << 16) + 32768; /* rounding */
        int r, g, b;
        int cr = pcr[i] - 128;
        int cb = pcb[i] - 128;
        r = y_fixed + cr*Ez_float2fixed (1.40200f);
        g = y_fixed - cr*Ez_float2fixed (0.71414f) - cb*Ez_float2fixed (0.34414f);
        b = y_fixed                                + cb*Ez_float2fixed (1.77200f);
        r >>= 16;
        g >>= 16;
        b >>= 16;
        if ((unsigned) r > 255) { if (r < 0) r = 0; else r = 255; }
        if ((unsigned) g > 255) { if (g < 0) g = 0; else g = 255; }
        if ((unsigned) b > 255) { if (b < 0) b = 0; else b = 255; }
        out[0] = (EZuint8)r;
        out[1] = (EZuint8)g;
        out[2] = (EZuint8)b;
        out[3] = 255;
        out += step;
    }
}


/* clean up the temporary component buffers */

void ez_jpeg_cleanup (Ez_jpeg *j)
{
    int i;
    for (i=0; i < j->s->img_n; ++i) {
        if (j->img_comp[i].data) {
            free (j->img_comp[i].raw_data);
            j->img_comp[i].data = NULL;
        }
        if (j->img_comp[i].linebuf) {
            free (j->img_comp[i].linebuf);
            j->img_comp[i].linebuf = NULL;
        }
    }
}


typedef struct {
    Ez_resample_row_func resample;
    EZuint8 *line0, *line1;
    int hs, vs;   /* expansion factor in each axis */
    int w_lores;  /* horizontal pixels pre-expansion  */
    int ystep;    /* how far through vertical expansion we are */
    int ypos;     /* which pre-expansion row we're on */
} Ez_stbi_resample;


EZuint8 *ez_jpeg_load_image (Ez_jpeg *z, int *out_x, int *out_y, int *comp,
    int req_comp)
{
    int n, decode_n;
    /* Validate req_comp */
    if (req_comp < 0 || req_comp > 4) {
        ez_error ("ez_jpeg_load_image: internal error: bad req_comp\n");
        return NULL;
    }
    z->s->img_n = 0;

    /* Load a jpeg image from whichever source */
    if (!ez_jpeg_decode_image (z)) { ez_jpeg_cleanup (z); return NULL; }

    /* Determine actual number of components to generate */
    n = req_comp ? req_comp : z->s->img_n;

    if (z->s->img_n == 3 && n < 3)
        decode_n = 1;
    else
        decode_n = z->s->img_n;

    /* Resample and color-convert */
    {
        int k;
        Ez_uint i, j;
        EZuint8 *output;
        EZuint8 *coutput[4];

        Ez_stbi_resample res_comp[4];

        for (k=0; k < decode_n; ++k) {
            Ez_stbi_resample *r = &res_comp[k];

            /* Allocate line buffer big enough for upsampling off the edges
               with upsample factor of 4 */
            z->img_comp[k].linebuf = (EZuint8*) malloc (z->s->img_x + 3);
            if (!z->img_comp[k].linebuf) {
                ez_jpeg_cleanup (z);
                ez_error ("ez_jpeg_load_image: out of memory\n");
                return NULL;
            }

            r->hs      = z->img_h_max / z->img_comp[k].h;
            r->vs      = z->img_v_max / z->img_comp[k].v;
            r->ystep   = r->vs >> 1;
            r->w_lores = (z->s->img_x + r->hs-1) / r->hs;
            r->ypos    = 0;
            r->line0   = r->line1 = z->img_comp[k].data;

            if      (r->hs == 1 && r->vs == 1) r->resample = ez_jpeg_resample_row_1;
            else if (r->hs == 1 && r->vs == 2) r->resample = ez_jpeg_resample_row_v_2;
            else if (r->hs == 2 && r->vs == 1) r->resample = ez_jpeg_resample_row_h_2;
            else if (r->hs == 2 && r->vs == 2) r->resample = ez_jpeg_resample_row_hv_2;
            else                               r->resample = ez_jpeg_resample_row_generic;
        }

        /* can't error after this so, this is safe */
        output = (EZuint8*) malloc (n * z->s->img_x * z->s->img_y + 1);
        if (!output) {
            ez_jpeg_cleanup (z);
            ez_error ("ez_jpeg_load_image: out of memory\n");
            return NULL;
        }

        /* Now go ahead and resample */
        for (j=0; j < z->s->img_y; ++j) {
            EZuint8 *out = output + n * z->s->img_x * j;
            for (k=0; k < decode_n; ++k) {
                Ez_stbi_resample *r = &res_comp[k];
                int y_bot = r->ystep >= (r->vs >> 1);
                coutput[k] = r->resample (z->img_comp[k].linebuf,
                                          y_bot ? r->line1 : r->line0,
                                          y_bot ? r->line0 : r->line1,
                                          r->w_lores, r->hs);
                if (++r->ystep >= r->vs) {
                    r->ystep = 0;
                    r->line0 = r->line1;
                    if (++r->ypos < z->img_comp[k].y)
                        r->line1 += z->img_comp[k].w2;
                }
            }
            if (n >= 3) {
                EZuint8 *y = coutput[0];
                if (z->s->img_n == 3) {
                    ez_jpeg_YCbCr_to_RGB_row (out, y, coutput[1], coutput[2],
                        z->s->img_x, n);
                } else
                    for (i=0; i < z->s->img_x; ++i) {
                        out[0] = out[1] = out[2] = y[i];
                        out[3] = 255; /* not used if n==3 */
                        out += n;
                    }
            } else {
                EZuint8 *y = coutput[0];
                if (n == 1)
                    for (i=0; i < z->s->img_x; ++i) out[i] = y[i];
                else
                    for (i=0; i < z->s->img_x; ++i) *out++ = y[i], *out++ = 255;
            }
        }
        ez_jpeg_cleanup (z);
        *out_x = z->s->img_x;
        *out_y = z->s->img_y;
        if (comp) *comp  = z->s->img_n; /* report original components, not output */
        return output;
    }
}


EZuint8 *ez_stbi_jpeg_load (Ez_stbi *s, int *x, int *y, int *comp, int req_comp)
{
    Ez_jpeg j;
    j.s = s;
    return ez_jpeg_load_image (&j, x, y, comp, req_comp);
}


int ez_stbi_jpeg_test (Ez_stbi *s)
{
    int r;
    Ez_jpeg j;
    j.s = s;
    r = ez_jpeg_decode_header (&j, EZ_SCAN_TYPE, 0);  /* 0 for silent */
    ez_stbi_rewind (s);
    return r;
}


int ez_stbi_jpeg_info_raw (Ez_jpeg *j, int *x, int *y, int *comp)
{
    if (!ez_jpeg_decode_header (j, EZ_SCAN_HEADER, 1)) {
        ez_stbi_rewind ( j->s );
        return 0;
    }
    if (x) *x = j->s->img_x;
    if (y) *y = j->s->img_y;
    if (comp) *comp = j->s->img_n;
    return 1;
}


int ez_stbi_jpeg_info (Ez_stbi *s, int *x, int *y, int *comp)
{
    Ez_jpeg j;
    j.s = s;
    return ez_stbi_jpeg_info_raw (&j, x, y, comp);
}


/*
 * Public domain zlib decode    v0.2  Sean Barrett 2006-11-18
 *   Simple implementation:
 *    - all input must be provided in an upfront buffer
 *    - all output is written to a single output buffer (can malloc/realloc)
 *   Performance:
 *    - fast huffman
*/

/* fast-way is faster to check than jpeg huffman, but slow way is slower */
#define EZ_ZFAST_BITS  9 /* accelerate all cases in default tables */
#define EZ_ZFAST_MASK  ((1 << EZ_ZFAST_BITS) - 1)


/* zlib-style huffman encoding */
/* (jpegs packs from left, zlib from right, so can't share code) */

typedef struct {
    Ez_uint16 fast[1 << EZ_ZFAST_BITS];
    Ez_uint16 firstcode[16];
    int maxcode[17];
    Ez_uint16 firstsymbol[16];
    EZuint8  size[288];
    Ez_uint16 value[288];
} Ez_zhuffman;


EZ_INLINE int ez_zlib_bitreverse16 (int n)
{
    n = ((n & 0xAAAA) >>  1) | ((n & 0x5555) << 1);
    n = ((n & 0xCCCC) >>  2) | ((n & 0x3333) << 2);
    n = ((n & 0xF0F0) >>  4) | ((n & 0x0F0F) << 4);
    n = ((n & 0xFF00) >>  8) | ((n & 0x00FF) << 8);
    return n;
}


EZ_INLINE int ez_zlib_bit_reverse (int v, int bits)
{
    if (bits > 16) {
        ez_error ("ez_zlib_bit_reverse: bits %d > 16\n", bits);
        return 0;
    }
    /* To bit reverse n bits, reverse 16 and shift
       e.g. 11 bits, bit reverse and shift away 5 */
    return ez_zlib_bitreverse16 (v) >> (16-bits);
}


int ez_zlib_build_huffman (Ez_zhuffman *z, EZuint8 *sizelist, int num)
{
    int i, k=0;
    int code, next_code[16], sizes[17];

    /* DEFLATE spec for generating codes */
    memset (sizes, 0, sizeof (sizes));
    memset (z->fast, 255, sizeof (z->fast));
    for (i=0; i < num; ++i)
        ++sizes[sizelist[i]];
    sizes[0] = 0;
    for (i=1; i < 16; ++i)
        if (sizes[i] > (1 << i)) {
            ez_error ("ez_zlib_build_huffman: internal error size[%d] = %d\n",
                i, sizes[i]);
            return 0;
        }
    code = 0;
    for (i=1; i < 16; ++i) {
        next_code[i] = code;
        z->firstcode[i] = (Ez_uint16) code;
        z->firstsymbol[i] = (Ez_uint16) k;
        code = (code + sizes[i]);
        if (sizes[i])
            if (code-1 >= (1 << i)) {
                ez_error ("ez_zlib_build_huffman: corrupt JPEG: bad codelengths\n");
            return 0;
            }
        z->maxcode[i] = code << (16-i); /* preshift for inner loop */
        code <<= 1;
        k += sizes[i];
    }
    z->maxcode[16] = 0x10000; /* sentinel */
    for (i=0; i < num; ++i) {
        int s = sizelist[i];
        if (s) {
            int c = next_code[s] - z->firstcode[s] + z->firstsymbol[s];
            z->size[c] = (EZuint8)s;
            z->value[c] = (Ez_uint16)i;
            if (s <= EZ_ZFAST_BITS) {
                int k = ez_zlib_bit_reverse (next_code[s], s);
                while (k < (1 << EZ_ZFAST_BITS)) {
                    z->fast[k] = (Ez_uint16) c;
                    k += (1 << s);
                }
            }
            ++next_code[s];
        }
    }
    return 1;
}


/*
 * Zlib-from-memory implementation for PNG reading
*
 * Because PNG allows splitting the zlib stream arbitrarily,
 * and it's annoying structurally to have PNG call ZLIB call PNG,
 * we require PNG read all the IDATs and combine them into a single
 * memory buffer.
*/

typedef struct {
    EZuint8 *zbuffer, *zbuffer_end;
    int num_bits;
    Ez_uint32 code_buffer;

    char *zout;
    char *zout_start;
    char *zout_end;
    int   z_expandable;

    Ez_zhuffman z_length, z_distance;
} Ez_zbuf;


EZ_INLINE int ez_zlib_get8 (Ez_zbuf *z)
{
    if (z->zbuffer >= z->zbuffer_end) return 0;
    return *z->zbuffer++;
}


void ez_zlib_fill_bits (Ez_zbuf *z)
{
    do {
        if (z->code_buffer >= (1U << z->num_bits)) {
            ez_error ("ez_zlib_fill_bits: internal error for numbits = %s\n",
                z->num_bits);
            return;
        }
        z->code_buffer |= ez_zlib_get8 (z) << z->num_bits;
        z->num_bits += 8;
    } while (z->num_bits <= 24);
}


EZ_INLINE unsigned int ez_zlib_receive (Ez_zbuf *z, int n)
{
    unsigned int k;
    if (z->num_bits < n) ez_zlib_fill_bits (z);
    k = z->code_buffer & ((1 << n) - 1);
    z->code_buffer >>= n;
    z->num_bits -= n;
    return k;
}


EZ_INLINE int ez_zlib_huffman_decode (Ez_zbuf *a, Ez_zhuffman *z)
{
    int b, s, k;
    if (a->num_bits < 16) ez_zlib_fill_bits (a);
    b = z->fast[a->code_buffer & EZ_ZFAST_MASK];
    if (b < 0xffff) {
        s = z->size[b];
        a->code_buffer >>= s;
        a->num_bits -= s;
        return z->value[b];
    }

    /* Not resolved by fast table, so compute it the slow way
       use jpeg approach, which requires MSbits at top */
    k = ez_zlib_bit_reverse (a->code_buffer, 16);
    for (s=EZ_ZFAST_BITS+1; ; ++s)
        if (k < z->maxcode[s])
            break;
    if (s == 16) return -1; /* invalid code! */
    /* code size is s, so: */
    b = (k >> (16-s)) - z->firstcode[s] + z->firstsymbol[s];
    if (z->size[b] != s) {
        ez_error ("ez_zlib_huffman_decode: internal error\n");
        return 0;
    }
    a->code_buffer >>= s;
    a->num_bits -= s;
    return z->value[b];
}


int ez_zlib_expand (Ez_zbuf *z, int n)  /* need to make room for n bytes */
{
    char *q;
    int cur, limit;
    if (!z->z_expandable) {
        ez_error ("ez_zlib_expand: corrupt PNG: output buffer limit\n");
        return 0;
    }
    cur   = (int) (z->zout     - z->zout_start);
    limit = (int) (z->zout_end - z->zout_start);
    while (cur + n > limit)
        limit *= 2;
    q = (char*) realloc (z->zout_start, limit);
    if (q == NULL)  {
        ez_error ("ez_zlib_expand: out of memory\n");
        return 0;
    }
    z->zout_start = q;
    z->zout       = q + cur;
    z->zout_end   = q + limit;
    return 1;
}


int length_base[31] = {
    3, 4, 5, 6, 7, 8, 9, 10, 11, 13,
    15, 17, 19, 23, 27, 31, 35, 43, 51, 59,
    67, 83, 99, 115, 131, 163, 195, 227, 258, 0, 0 };

int length_extra[31] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
    3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0, 0, 0 };

int dist_base[32] = { 1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129,
    193, 257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289,
    16385, 24577, 0, 0};

int dist_extra[32] = { 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7,
    8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13};


int ez_zlib_parse_huffman_block (Ez_zbuf *a)
{
    for (;;) {
        int z = ez_zlib_huffman_decode (a, &a->z_length);
        if (z < 256) {
            if (z < 0) {
                ez_error ("ez_zlib_parse_huffman_block: corrupt PNG: bad huffman code\n");
                return 0;
            }
            if (a->zout >= a->zout_end) if (!ez_zlib_expand (a, 1)) return 0;
            *a->zout++ = (char) z;
        } else {
            EZuint8 *p;
            int len, dist;
            if (z == 256) return 1;
            z -= 257;
            len = length_base[z];
            if (length_extra[z]) len += ez_zlib_receive (a, length_extra[z]);
            z = ez_zlib_huffman_decode (a, &a->z_distance);
            if (z < 0) {
                ez_error ("ez_zlib_parse_huffman_block: corrupt PNG: bad huffman code\n");
                return 0;
            }
            dist = dist_base[z];
            if (dist_extra[z]) dist += ez_zlib_receive (a, dist_extra[z]);
            if (a->zout - a->zout_start < dist) {
                ez_error ("ez_zlib_parse_huffman_block: corrupt PNG: bad dist\n");
                return 0;
            }
            if (a->zout + len > a->zout_end) if (!ez_zlib_expand (a, len)) return 0;
            p = (EZuint8 *) (a->zout - dist);
            while (len--)
                *a->zout++ = *p++;
        }
    }
}


int ez_zlib_compute_huffman_codes (Ez_zbuf *a)
{
    EZuint8 length_dezigzag[19] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12,
        3, 13, 2, 14, 1, 15 };
    Ez_zhuffman z_codelength;
    EZuint8 lencodes[286+32+137];/* padding for maximum single op */
    EZuint8 codelength_sizes[19];
    int i, n;

    int hlit  = ez_zlib_receive (a, 5) + 257;
    int hdist = ez_zlib_receive (a, 5) + 1;
    int hclen = ez_zlib_receive (a, 4) + 4;

    memset (codelength_sizes, 0, sizeof (codelength_sizes));
    for (i=0; i < hclen; ++i) {
        int s = ez_zlib_receive (a, 3);
        codelength_sizes[length_dezigzag[i]] = (EZuint8) s;
    }
    if (!ez_zlib_build_huffman (&z_codelength, codelength_sizes, 19)) return 0;

    n = 0;
    while (n < hlit + hdist) {
        int c = ez_zlib_huffman_decode (a, &z_codelength);
        if (c < 0 || c >= 19) {
            ez_error ("ez_zlib_compute_huffman_codes: internal error c = %d\n", c);
            return 0;
        }
        if (c < 16)
            lencodes[n++] = (EZuint8) c;
        else if (c == 16) {
            c = ez_zlib_receive (a, 2)+3;
            memset (lencodes+n, lencodes[n-1], c);
            n += c;
        } else if (c == 17) {
            c = ez_zlib_receive (a, 3)+3;
            memset (lencodes+n, 0, c);
            n += c;
        } else {
            if (c != 18) {
                ez_error ("ez_zlib_compute_huffman_codes: internal error c = %d\n", c);
                return 0;
            }
            c = ez_zlib_receive (a, 7)+11;
            memset (lencodes+n, 0, c);
            n += c;
        }
    }
    if (n != hlit+hdist) {
        ez_error ("ez_zlib_compute_huffman_codes: corrupt PNG: bad code lengths\n");
        return 0;
    }
    if (!ez_zlib_build_huffman (&a->z_length, lencodes, hlit)) return 0;
    if (!ez_zlib_build_huffman (&a->z_distance, lencodes+hlit, hdist)) return 0;
    return 1;
}


int ez_zlib_parse_uncompressed_block (Ez_zbuf *a)
{
    EZuint8 header[4];
    int len, nlen, k;
    if (a->num_bits & 7)
        ez_zlib_receive (a, a->num_bits & 7); /* discard */
    /* Drain the bit-packed data into header */
    k = 0;
    while (a->num_bits > 0) {
        header[k++] = (EZuint8) (a->code_buffer & 255); /* wtf this warns? */
        a->code_buffer >>= 8;
        a->num_bits -= 8;
    }
    if (a->num_bits != 0) {
        ez_error ("ez_zlib_parse_uncompressed_block: internal error num_bits = %d\n",
            a->num_bits);
        return 0;
    }
    /* Now fill header the normal way */
    while (k < 4)
        header[k++] = (EZuint8) ez_zlib_get8 (a);
    len  = header[1] * 256 + header[0];
    nlen = header[3] * 256 + header[2];
    if (nlen != (len ^ 0xffff)) {
        ez_error ("ez_zlib_parse_uncompressed_block: corrupt PNG: zlib corrupt\n");
        return 0;
    }
    if (a->zbuffer + len > a->zbuffer_end) {
        ez_error ("ez_zlib_parse_uncompressed_block: corrupt PNG: read past buffer\n");
        return 0;
    }
    if (a->zout + len > a->zout_end)
        if (!ez_zlib_expand (a, len)) return 0;
    memcpy (a->zout, a->zbuffer, len);
    a->zbuffer += len;
    a->zout += len;
    return 1;
}


int ez_zlib_parse_header (Ez_zbuf *a)
{
    int cmf   = ez_zlib_get8 (a);
    int cm    = cmf & 15;
    /* int cinfo = cmf >> 4; */
    int flg   = ez_zlib_get8 (a);
    if ((cmf*256+flg) % 31 != 0) { /* zlib spec */
        ez_error ("ez_zlib_parse_header: corrupt PNG: bad zlib header\n");
        return 0;
    }
    if (flg & 32) { /* preset dictionary not allowed in png */
        ez_error ("ez_zlib_parse_header: corrupt PNG: no preset dict\n");
        return 0;
    }
    if (cm != 8)  { /* DEFLATE required for png */
        ez_error ("ez_zlib_parse_header: corrupt PNG: bad compression\n");
        return 0;
    }

    /* window = 1 << (8 + cinfo)... but who cares, we fully buffer output */
    return 1;
}


/* @TODO: should statically initialize these for optimal thread safety */
EZuint8 default_length[288], default_distance[32];

void ez_zlib_init_defaults (void)
{
    int i;   /* use <= to match clearly with spec */
    for (i=0; i <= 143; ++i)     default_length[i]   = 8;
    for (   ; i <= 255; ++i)     default_length[i]   = 9;
    for (   ; i <= 279; ++i)     default_length[i]   = 7;
    for (   ; i <= 287; ++i)     default_length[i]   = 8;

    for (i=0; i <=  31; ++i)     default_distance[i] = 5;
}


/* A quick hack to only allow decoding some of a PNG... I should implement
   real streaming support instead */
int ez_stbi_png_partial;

int ez_zlib_parse (Ez_zbuf *a, int parse_header)
{
    int final, type;
    if (parse_header)
        if (!ez_zlib_parse_header (a)) return 0;
    a->num_bits = 0;
    a->code_buffer = 0;
    do {
        final = ez_zlib_receive (a, 1);
        type  = ez_zlib_receive (a, 2);
        if (type == 0) {
            if (!ez_zlib_parse_uncompressed_block (a)) return 0;
        } else if (type == 3) {
            return 0;
        } else {
            if (type == 1) {
                /* use fixed code lengths */
                if (!default_distance[31]) ez_zlib_init_defaults ();
                if (!ez_zlib_build_huffman (&a->z_length  , default_length  , 288))
                    return 0;
                if (!ez_zlib_build_huffman (&a->z_distance, default_distance,  32))
                    return 0;
            } else {
                if (!ez_zlib_compute_huffman_codes (a)) return 0;
            }
            if (!ez_zlib_parse_huffman_block (a)) return 0;
        }
        if (ez_stbi_png_partial && a->zout - a->zout_start > 65536)
            break;
    } while (!final);
    return 1;
}


int ez_zlib_do (Ez_zbuf *a, char *obuf, int olen, int exp, int parse_header)
{
    a->zout_start = obuf;
    a->zout       = obuf;
    a->zout_end   = obuf + olen;
    a->z_expandable = exp;

    return ez_zlib_parse (a, parse_header);
}

char *ez_stbi_zlib_decode_malloc_guesssize (const char *buffer, int len,
    int initial_size, int *outlen)
{
    Ez_zbuf a;
    char *p = (char*) malloc (initial_size);
    if (p == NULL) return NULL;
    a.zbuffer = (EZuint8 *) buffer;
    a.zbuffer_end = (EZuint8 *) buffer + len;
    if (ez_zlib_do (&a, p, initial_size, 1, 1)) {
        if (outlen) *outlen = (int) (a.zout - a.zout_start);
        return a.zout_start;
    } else {
        free (a.zout_start);
        return NULL;
    }
}

char *ez_stbi_zlib_decode_malloc (char const *buffer, int len, int *outlen)
{
    return ez_stbi_zlib_decode_malloc_guesssize (buffer, len, 16384, outlen);
}


char *ez_stbi_zlib_decode_malloc_guesssize_headerflag (const char *buffer,
    int len, int initial_size, int *outlen, int parse_header)
{
    Ez_zbuf a;
    char *p = (char*) malloc (initial_size);
    if (p == NULL) return NULL;
    a.zbuffer = (EZuint8 *) buffer;
    a.zbuffer_end = (EZuint8 *) buffer + len;
    if (ez_zlib_do (&a, p, initial_size, 1, parse_header)) {
        if (outlen) *outlen = (int) (a.zout - a.zout_start);
        return a.zout_start;
    } else {
        free (a.zout_start);
        return NULL;
    }
}


int ez_stbi_zlib_decode_buffer (char *obuffer, int olen, char const *ibuffer,
    int ilen)
{
    Ez_zbuf a;
    a.zbuffer = (EZuint8 *) ibuffer;
    a.zbuffer_end = (EZuint8 *) ibuffer + ilen;
    if (ez_zlib_do (&a, obuffer, olen, 0, 1))
        return (int) (a.zout - a.zout_start);
    else
        return -1;
}


char *ez_stbi_zlib_decode_noheader_malloc (char const *buffer, int len,
    int *outlen)
{
    Ez_zbuf a;
    char *p = (char*) malloc (16384);
    if (p == NULL) return NULL;
    a.zbuffer = (EZuint8 *) buffer;
    a.zbuffer_end = (EZuint8 *) buffer+len;
    if (ez_zlib_do (&a, p, 16384, 1, 0)) {
        if (outlen) *outlen = (int) (a.zout - a.zout_start);
        return a.zout_start;
    } else {
        free (a.zout_start);
        return NULL;
    }
}


int ez_stbi_zlib_decode_noheader_buffer (char *obuffer, int olen,
    const char *ibuffer, int ilen)
{
    Ez_zbuf a;
    a.zbuffer = (EZuint8 *) ibuffer;
    a.zbuffer_end = (EZuint8 *) ibuffer + ilen;
    if (ez_zlib_do (&a, obuffer, olen, 0, 0))
        return (int) (a.zout - a.zout_start);
    else
        return -1;
}


/*
 * Public domain "baseline" PNG decoder   v0.10  Sean Barrett 2006-11-18
 *
 * Simple implementation:
 *   - only 8-bit samples
 *   - no CRC checking
 *   - allocates lots of intermediate memory
 *   - avoids problem of streaming data between subsystems
 *   - avoids explicit window management
 * Performance:
 *   - uses stb_zlib, a PD zlib implementation with fast huffman decoding
*/

typedef struct {
    Ez_uint32 length;
    Ez_uint32 type;
} Ez_chunk;

#define EZ_PNG_TYPE(a,b,c,d)  (((a) << 24) + ((b) << 16) + ((c) << 8) + (d))


Ez_chunk ez_png_get_chunk_header (Ez_stbi *s)
{
    Ez_chunk c;
    c.length = ez_buffer_get32 (s);
    c.type   = ez_buffer_get32 (s);
    return c;
}


int ez_png_check_header (Ez_stbi *s, int verbose)
{
    static EZuint8 png_sig[8] = { 137, 80, 78, 71, 13, 10, 26, 10 };
    int i;
    for (i=0; i < 8; ++i)
        if (ez_buffer_get8u (s) != png_sig[i]) {
            if (verbose) ez_error ("ez_png_check_header: not a PNG: bad signature\n");
            return 0;
        }
    return 1;
}


typedef struct {
    Ez_stbi *s;
    EZuint8 *idata, *expanded, *out;
} Ez_png;


enum {
    EZ_F_NONE = 0, EZ_F_SUB = 1, EZ_F_UP = 2, EZ_F_AVG = 3, EZ_F_PAETH = 4,
    EZ_F_AVG_FIRST, EZ_F_PAETH_FIRST
};

EZuint8 first_row_filter[5] = {
    EZ_F_NONE, EZ_F_SUB, EZ_F_NONE, EZ_F_AVG_FIRST, EZ_F_PAETH_FIRST
};


int ez_png_paeth (int a, int b, int c)
{
    int p = a + b - c;
    int pa = abs (p-a);
    int pb = abs (p-b);
    int pc = abs (p-c);
    if (pa <= pb && pa <= pc) return a;
    if (pb <= pc) return b;
    return c;
}


/* Create the png data from post-deflated data */

int ez_png_create_image_raw (Ez_png *a, EZuint8 *raw, Ez_uint32 raw_len,
    int out_n, Ez_uint32 x, Ez_uint32 y)
{
    Ez_stbi *s = a->s;
    Ez_uint32 i, j, stride = x*out_n;
    int k;
    int img_n = s->img_n; /* copy it into a local for later */
    if (out_n != s->img_n && out_n != s->img_n+1) {
        ez_error ("ez_png_create_image_raw: internal error out_n = %d\n", out_n);
        return 0;
    }
    if (ez_stbi_png_partial) y = 1;
    a->out = (EZuint8*) malloc (x * y * out_n);
    if (!a->out) {
        ez_error ("ez_png_create_image_raw: out of memory\n");
        return 0;
    }
    if (!ez_stbi_png_partial) {
        if (s->img_x == x && s->img_y == y) {
            if (raw_len != (img_n * x + 1) * y) {
                ez_error ("ez_png_create_image_raw: corrupt PNG: not enough pixels\n");
            return 0;
            }
        } else { /* interlaced: */
            if (raw_len < (img_n * x + 1) * y) {
                ez_error ("ez_png_create_image_raw: corrupt PNG: not enough pixels\n");
                return 0;
            }
        }
    }
    for (j=0; j < y; ++j) {
        EZuint8 *cur = a->out + stride*j;
        EZuint8 *prior = cur - stride;
        int filter = *raw++;
        if (filter > 4) {
            ez_error ("ez_png_create_image_raw: corrupt PNG: invalid filter %d\n",
                filter);
            return 0;
        }
        /* If first row, use special filter that doesn't sample previous row */
        if (j == 0) filter = first_row_filter[filter];
        /* Handle first pixel explicitly */
        for (k=0; k < img_n; ++k) {
            switch (filter) {
                case EZ_F_NONE       : cur[k] = raw[k]; break;
                case EZ_F_SUB        : cur[k] = raw[k]; break;
                case EZ_F_UP         : cur[k] = raw[k] + prior[k]; break;
                case EZ_F_AVG        : cur[k] = raw[k] + (prior[k]>>1); break;
                case EZ_F_PAETH      : cur[k] = (EZuint8) (raw[k] + ez_png_paeth (
                                            0, prior[k], 0)); break;
                case EZ_F_AVG_FIRST  : cur[k] = raw[k]; break;
                case EZ_F_PAETH_FIRST: cur[k] = raw[k]; break;
            }
        }
        if (img_n != out_n) cur[img_n] = 255;
        raw += img_n;
        cur += out_n;
        prior += out_n;
        /* This is a little gross, so that we don't switch per-pixel or per-
           component */
        if (img_n == out_n) {
            #define CASE(f) \
                case f:     \
                    for (i=x-1; i >= 1; --i, raw+=img_n, cur+=img_n, prior+=img_n) \
                        for (k=0; k < img_n; ++k)
            switch (filter) {
                CASE(EZ_F_NONE)
                    cur[k] = raw[k];
                 break;
                CASE(EZ_F_SUB)
                    cur[k] = raw[k] + cur[k-img_n];
                 break;
                CASE(EZ_F_UP)
                    cur[k] = raw[k] + prior[k];
                 break;
                CASE(EZ_F_AVG)
                    cur[k] = raw[k] + ((prior[k] + cur[k-img_n])>>1);
                 break;
                CASE(EZ_F_PAETH)
                    cur[k] = (EZuint8) (raw[k] + ez_png_paeth (
                        cur[k-img_n], prior[k], prior[k-img_n]));
                 break;
                CASE(EZ_F_AVG_FIRST)
                    cur[k] = raw[k] + (cur[k-img_n] >> 1);
                 break;
                CASE(EZ_F_PAETH_FIRST)
                    cur[k] = (EZuint8) (raw[k] + ez_png_paeth (
                        cur[k-img_n], 0, 0));
                 break;
            }
            #undef CASE
        } else {
            if (img_n+1 != out_n) {
                ez_error ("ez_png_create_image_raw: internal error out_n = %d\n",
                    out_n);
                return 0;
            }
            #define CASE(f) \
                case f:     \
                    for (i=x-1; i >= 1; --i, cur[img_n]=255, raw+=img_n, \
                        cur+=out_n, prior+=out_n) \
                      for (k=0; k < img_n; ++k)
            switch (filter) {
                CASE(EZ_F_NONE)
                    cur[k] = raw[k];
                 break;
                CASE(EZ_F_SUB)
                    cur[k] = raw[k] + cur[k-out_n];
                 break;
                CASE(EZ_F_UP)
                    cur[k] = raw[k] + prior[k];
                 break;
                CASE(EZ_F_AVG)
                    cur[k] = raw[k] + ((prior[k] + cur[k-out_n])>>1);
                 break;
                CASE(EZ_F_PAETH)
                    cur[k] = (EZuint8) (raw[k] + ez_png_paeth (cur[k-out_n],
                        prior[k], prior[k-out_n]));
                 break;
                CASE(EZ_F_AVG_FIRST)
                    cur[k] = raw[k] + (cur[k-out_n] >> 1);
                 break;
                CASE(EZ_F_PAETH_FIRST)
                    cur[k] = (EZuint8) (raw[k] + ez_png_paeth (cur[k-out_n], 0, 0));
                 break;
            }
            #undef CASE
        }
    }
    return 1;
}


int ez_png_create_image (Ez_png *a, EZuint8 *raw, Ez_uint32 raw_len, int out_n,
    int interlaced)
{
    EZuint8 *final;
    int p;
    int save;
    if (!interlaced)
        return ez_png_create_image_raw (a, raw, raw_len, out_n, a->s->img_x,
            a->s->img_y);
    save = ez_stbi_png_partial;
    ez_stbi_png_partial = 0;

    /* De-interlacing */
    final = (EZuint8*) malloc (a->s->img_x * a->s->img_y * out_n);
    for (p=0; p < 7; ++p) {
        int xorig[] = { 0, 4, 0, 2, 0, 1, 0 };
        int yorig[] = { 0, 0, 4, 0, 2, 0, 1 };
        int xspc[]  = { 8, 8, 4, 4, 2, 2, 1 };
        int yspc[]  = { 8, 8, 8, 4, 4, 2, 2 };
        int i, j, x, y;
        /* pass1_x[4] = 0, pass1_x[5] = 1, pass1_x[12] = 1 */
        x = (a->s->img_x - xorig[p] + xspc[p]-1) / xspc[p];
        y = (a->s->img_y - yorig[p] + yspc[p]-1) / yspc[p];
        if (x && y) {
            if (!ez_png_create_image_raw (a, raw, raw_len, out_n, x, y)) {
                free (final);
                return 0;
            }
            for (j=0; j < y; ++j)
            for (i=0; i < x; ++i)
                memcpy (final + (j*yspc[p]+yorig[p])*a->s->img_x*out_n +
                    (i*xspc[p]+xorig[p])*out_n, a->out + (j*x+i)*out_n, out_n);
            free (a->out);
            raw += (x*out_n+1)*y;
            raw_len -= (x*out_n+1)*y;
        }
    }
    a->out = final;

    ez_stbi_png_partial = save;
    return 1;
}


int ez_png_compute_transparency (Ez_png *z, EZuint8 tc[3], int out_n)
{
    Ez_stbi *s = z->s;
    Ez_uint32 i, pixel_count = s->img_x * s->img_y;
    EZuint8 *p = z->out;

    /* Compute color-based transparency, assuming we've
       already got 255 as the alpha value in the output */
    if (out_n != 2 && out_n != 4) {
        ez_error ("ez_png_compute_transparency: internal error out_n = %d\n",
            out_n);
        return 0;
    }

    if (out_n == 2) {
        for (i=0; i < pixel_count; ++i) {
            p[1] = (p[0] == tc[0] ? 0 : 255);
            p += 2;
        }
    } else {
        for (i=0; i < pixel_count; ++i) {
            if (p[0] == tc[0] && p[1] == tc[1] && p[2] == tc[2])
                p[3] = 0;
            p += 4;
        }
    }
    return 1;
}


int ez_png_expand_palette (Ez_png *a, EZuint8 *palette, int len, int pal_img_n)
{
    Ez_uint32 i, pixel_count = a->s->img_x * a->s->img_y;
    EZuint8 *p, *temp_out, *orig = a->out;

    p = (EZuint8*) malloc (pixel_count * pal_img_n);
    if (p == NULL) {
        ez_error ("ez_png_expand_palette: out of memory\n");
        return 0;
    }

    /* Between here and free (out) below, exitting would leak */
    temp_out = p;

    if (pal_img_n == 3) {
        for (i=0; i < pixel_count; ++i) {
            int n = orig[i]*4;
            p[0] = palette[n  ];
            p[1] = palette[n+1];
            p[2] = palette[n+2];
            p += 3;
        }
    } else {
        for (i=0; i < pixel_count; ++i) {
            int n = orig[i]*4;
            p[0] = palette[n  ];
            p[1] = palette[n+1];
            p[2] = palette[n+2];
            p[3] = palette[n+3];
            p += 4;
        }
    }
    free (a->out);
    a->out = temp_out;

    (void) len;

    return 1;
}


int ez_png_parse_file (Ez_png *z, int scan, int req_comp)
{
    EZuint8 palette[1024], pal_img_n=0;
    EZuint8 has_trans=0, tc[3];
    Ez_uint32 ioff=0, idata_limit=0, i, pal_len=0;
    int first=1, k, interlace=0;
    Ez_stbi *s = z->s;

    z->expanded = NULL;
    z->idata = NULL;
    z->out = NULL;

    if (!ez_png_check_header (s, 1)) return 0;

    if (scan == EZ_SCAN_TYPE) return 1;

    for (;;) {
        Ez_chunk c = ez_png_get_chunk_header (s);
        switch (c.type) {
            case EZ_PNG_TYPE ('C', 'g', 'B', 'I'):
                ez_buffer_skip (s, c.length);
                break;
            case EZ_PNG_TYPE ('I', 'H', 'D', 'R'): {
                int depth, color, comp, filter;
                if (!first) {
                    ez_error ("ez_png_parse_file: corrupt PNG: multiple IHDR\n");
                    return 0;
                }
                first = 0;
                if (c.length != 13) {
                    ez_error ("ez_png_parse_file: corrupt PNG: bad IHDR len\n");
                    return 0;
                }
                s->img_x = ez_buffer_get32 (s); if (s->img_x > (1 << 24)) {
                    ez_error ("ez_png_parse_file: corrupt PNG: too large image\n");
                    return 0;
                }
                s->img_y = ez_buffer_get32 (s); if (s->img_y > (1 << 24)) {
                    ez_error ("ez_png_parse_file: corrupt PNG: too large image\n");
                    return 0;
                }
                depth = ez_buffer_get8 (s); if (depth != 8) {
                    ez_error ("ez_png_parse_file: PNG not supported: 8-bit only\n");
                    return 0;
                }
                color = ez_buffer_get8 (s); if (color > 6) {
                    ez_error ("ez_png_parse_file: corrupt PNG: bad ctype\n");
                    return 0;
                }
                if (color == 3) pal_img_n = 3; else if (color & 1) {
                    ez_error ("ez_png_parse_file: corrupt PNG: bad ctype\n");
                    return 0;
                }
                comp  = ez_buffer_get8 (s); if (comp) {
                    ez_error ("ez_png_parse_file: corrupt PNG: bad comp method\n");
                    return 0;
                }
                filter= ez_buffer_get8 (s);  if (filter) {
                    ez_error ("ez_png_parse_file: corrupt PNG: bad filter method\n");
                    return 0;
                }
                interlace = ez_buffer_get8 (s); if (interlace>1) {
                    ez_error ("ez_png_parse_file: corrupt PNG: bad interlace method\n");
                return 0;
                }
                if (!s->img_x || !s->img_y) {
                    ez_error ("ez_png_parse_file: corrupt PNG: 0 pixel image\n");
                    return 0;
                }
                if (!pal_img_n) {
                    s->img_n = (color & 2 ? 3 : 1) + (color & 4 ? 1 : 0);
                    if ((1 << 30) / s->img_x / s->img_n < s->img_y) {
                        ez_error ("ez_png_parse_file: PNG image too large to decode\n");
                        return 0;
                    }
                    if (scan == EZ_SCAN_HEADER) return 1;
                } else {
                    /* If paletted, then pal_n is our final components, and
                       img_n is # components to decompress/filter. */
                    s->img_n = 1;
                    if ((1 << 30) / s->img_x / 4 < s->img_y) {
                        ez_error ("ez_png_parse_file: corrupt PNG: too large\n");
                        return 0;
                    }
                    /* if EZ_SCAN_HEADER, have to scan to see if we have a tRNS */
                }
                break;
            }

            case EZ_PNG_TYPE ('P', 'L', 'T', 'E'):  {
                if (first) {
                    ez_error ("ez_png_parse_file: corrupt PNG: first not IHDR\n");
                    return 0;
                }
                if (c.length > 256*3) {
                    ez_error ("ez_png_parse_file: corrupt PNG: invalid PLTE\n");
                    return 0;
                }
                pal_len = c.length / 3;
                if (pal_len * 3 != c.length) {
                    ez_error ("ez_png_parse_file: corrupt PNG: invalid PLTE\n");
                    return 0;
                }
                for (i=0; i < pal_len; ++i) {
                    palette[i*4+0] = ez_buffer_get8u (s);
                    palette[i*4+1] = ez_buffer_get8u (s);
                    palette[i*4+2] = ez_buffer_get8u (s);
                    palette[i*4+3] = 255;
                }
                break;
            }

            case EZ_PNG_TYPE ('t', 'R', 'N', 'S'): {
                if (first) {
                    ez_error ("ez_png_parse_file: corrupt PNG: first not IHDR\n");
                    return 0;
                }
                if (z->idata) {
                    ez_error ("ez_png_parse_file: corrupt PNG: tRNS after IDAT\n");
                    return 0;
                }
                if (pal_img_n) {
                    if (scan == EZ_SCAN_HEADER) { s->img_n = 4; return 1; }
                    if (pal_len == 0) {
                        ez_error ("ez_png_parse_file: corrupt PNG: tRNS before PLTE\n");
                        return 0;
                    }
                    if (c.length > pal_len) {
                        ez_error ("ez_png_parse_file: corrupt PNG: bad tRNS len\n");
                        return 0;
                    }
                    pal_img_n = 4;
                    for (i=0; i < c.length; ++i)
                        palette[i*4+3] = ez_buffer_get8u (s);
                } else {
                    if (! (s->img_n & 1)) {
                        ez_error ("ez_png_parse_file: corrupt PNG: tRNS with alpha\n");
                        return 0;
                    }
                    if (c.length != (Ez_uint32) s->img_n*2) {
                        ez_error ("ez_png_parse_file: corrupt PNG: bad tRNS len\n");
                        return 0;
                    }
                    has_trans = 1;
                    for (k=0; k < s->img_n; ++k)
                        tc[k] = (EZuint8) ez_buffer_get16 (s);
                        /* non 8-bit images will be larger */
                }
                break;
            }

            case EZ_PNG_TYPE ('I', 'D', 'A', 'T'): {
                if (first) {
                    ez_error ("ez_png_parse_file: corrupt PNG: first not IHDR\n");
                    return 0;
                }
                if (pal_img_n && !pal_len) {
                    ez_error ("ez_png_parse_file: corrupt PNG: no PLTE\n");
                    return 0;
                }
                if (scan == EZ_SCAN_HEADER) { s->img_n = pal_img_n; return 1; }
                if (ioff + c.length > idata_limit) {
                    EZuint8 *p;
                    if (idata_limit == 0)
                        idata_limit = c.length > 4096 ? c.length : 4096;
                    while (ioff + c.length > idata_limit)
                        idata_limit *= 2;
                    p = (EZuint8*) realloc (z->idata, idata_limit); if (p == NULL) {
                        ez_error ("ez_png_parse_file: out of memory\n");
                        return 0;
                    }
                    z->idata = p;
                }
                if (!ez_buffer_getn (s, z->idata+ioff, c.length)) {
                    ez_error ("ez_png_parse_file: corrupt PNG: out of data\n");
                    return 0;
                }
                ioff += c.length;
                break;
            }

            case EZ_PNG_TYPE ('I', 'E', 'N', 'D'): {
                Ez_uint32 raw_len;
                if (first) {
                    ez_error ("ez_png_parse_file: corrupt PNG: first not IHDR\n");
                    return 0;
                }
                if (scan != EZ_SCAN_LOAD) return 1;
                if (z->idata == NULL) {
                    ez_error ("ez_png_parse_file: corrupt PNG: no IDAT\n");
                    return 0;
                }
                z->expanded = (EZuint8 *)
                    ez_stbi_zlib_decode_malloc_guesssize_headerflag (
                        (char *) z->idata, ioff, 16384, (int *) &raw_len, 1);
                if (z->expanded == NULL) return 0; /* zlib should set error */
                free (z->idata); z->idata = NULL;
                if ((req_comp == s->img_n+1 && req_comp != 3 && !pal_img_n) ||
                    has_trans) s->img_out_n = s->img_n+1;
                else
                    s->img_out_n = s->img_n;
                if (!ez_png_create_image (z, z->expanded, raw_len, s->img_out_n,
                    interlace)) return 0;
                if (has_trans)
                    if (!ez_png_compute_transparency (z, tc, s->img_out_n)) return 0;
                if (pal_img_n) {
                    /* pal_img_n == 3 or 4 */
                    s->img_n = pal_img_n; /* record the actual colors we had */
                    s->img_out_n = pal_img_n;
                    if (req_comp >= 3) s->img_out_n = req_comp;
                    if (!ez_png_expand_palette (z, palette, pal_len, s->img_out_n))
                        return 0;
                }
                free (z->expanded); z->expanded = NULL;
                return 1;
            }

            default:
                /* if critical, fail */
                if (first) {
                    ez_error ("ez_png_parse_file: corrupt PNG: first not IHDR\n");
                    return 0;
                }
                if ((c.type & (1 << 29)) == 0) {
                    ez_error ("ez_png_parse_file: PNG not supported: invalid chunk"
                        " 0x%x\n", c.type);
                    return 0;
                }
                ez_buffer_skip (s, c.length);
                break;
        }
        /* End of chunk, read and skip CRC */
        ez_buffer_get32 (s);
    }
}


EZuint8 *ez_png_do (Ez_png *p, int *x, int *y, int *n, int req_comp)
{
    EZuint8 *result=NULL;
    if (req_comp < 0 || req_comp > 4) {
        ez_error ("ez_png_do: internal error : bad req_comp\n");
        return NULL;
    }
    if (ez_png_parse_file (p, EZ_SCAN_LOAD, req_comp)) {
        result = p->out;
        p->out = NULL;
        if (req_comp && req_comp != p->s->img_out_n) {
            result = ez_convert_format (result, p->s->img_out_n, req_comp,
                p->s->img_x, p->s->img_y);
            p->s->img_out_n = req_comp;
            if (result == NULL) return result;
        }
        *x = p->s->img_x;
        *y = p->s->img_y;
        if (n) *n = p->s->img_n;
    }
    free (p->out);      p->out      = NULL;
    free (p->expanded); p->expanded = NULL;
    free (p->idata);    p->idata    = NULL;

    return result;
}


EZuint8 *ez_stbi_png_load (Ez_stbi *s, int *x, int *y, int *comp, int req_comp)
{
    Ez_png p;
    p.s = s;
    return ez_png_do (&p, x, y, comp, req_comp);
}


int ez_stbi_png_test (Ez_stbi *s)
{
    int r;
    r = ez_png_check_header (s, 0);  /* 0 for silent */
    ez_stbi_rewind (s);
    return r;
}


int ez_stbi_png_info_raw (Ez_png *p, int *x, int *y, int *comp)
{
    if (!ez_png_parse_file (p, EZ_SCAN_HEADER, 0)) {
        ez_stbi_rewind ( p->s );
        return 0;
    }
    if (x) *x = p->s->img_x;
    if (y) *y = p->s->img_y;
    if (comp) *comp = p->s->img_n;
    return 1;
}


int ez_stbi_png_info (Ez_stbi *s, int *x, int *y, int *comp)
{
    Ez_png p;
    p.s = s;
    return ez_stbi_png_info_raw (&p, x, y, comp);
}


/*
 * Microsoft/Windows BMP image
*/

int ez_bmp_test (Ez_stbi *s)
{
    int sz;
    if (ez_buffer_get8 (s) != 'B') return 0;
    if (ez_buffer_get8 (s) != 'M') return 0;
    ez_buffer_get32le (s); /* discard filesize */
    ez_buffer_get16le (s); /* discard reserved */
    ez_buffer_get16le (s); /* discard reserved */
    ez_buffer_get32le (s); /* discard data offset */
    sz = ez_buffer_get32le (s);
    if (sz == 12 || sz == 40 || sz == 56 || sz == 108) return 1;
    return 0;
}


int ez_stbi_bmp_test (Ez_stbi *s)
{
    int r = ez_bmp_test (s);
    ez_stbi_rewind (s);
    return r;
}



/* Returns 0..31 for the highest set bit */

int ez_bmp_highest_bit (unsigned int z)
{
    int n=0;
    if (z == 0) return -1;
    if (z >= 0x10000) n += 16, z >>= 16;
    if (z >= 0x00100) n +=  8, z >>=  8;
    if (z >= 0x00010) n +=  4, z >>=  4;
    if (z >= 0x00004) n +=  2, z >>=  2;
    if (z >= 0x00002) n +=  1, z >>=  1;
    return n;
}


int ez_bmp_bitcount (unsigned int a)
{
    a = (a & 0x55555555) + ((a >>  1) & 0x55555555); /* max 2 */
    a = (a & 0x33333333) + ((a >>  2) & 0x33333333); /* max 4 */
    a = (a + (a >> 4)) & 0x0f0f0f0f; /* max 8 per 4, now 8 bits */
    a = (a + (a >> 8));  /* max 16 per 8 bits */
    a = (a + (a >> 16)); /* max 32 per 8 bits */
    return a & 0xff;
}


int ez_bmp_shiftsigned (int v, int shift, int bits)
{
    int result;
    int z=0;

    if (shift < 0) v <<= -shift;
    else v >>= shift;
    result = v;

    z = bits;
    while (z < 8) {
        result += v >> z;
        z += bits;
    }
    return result;
}


EZuint8 *ez_bmp_load (Ez_stbi *s, int *x, int *y, int *comp, int req_comp)
{
    EZuint8 *out;
    unsigned int mr=0, mg=0, mb=0, ma=0;
    EZuint8 pal[256][4];
    int psize=0, i, j, compress=0, width;
    int bpp, flip_vertically, pad, target, offset, hsz;
    if (ez_buffer_get8 (s) != 'B' || ez_buffer_get8 (s) != 'M') {
        ez_error ("ez_bmp_load: corrupt BMP : not BMP\n");
        return NULL;
    }
    ez_buffer_get32le (s); /* discard filesize */
    ez_buffer_get16le (s); /* discard reserved */
    ez_buffer_get16le (s); /* discard reserved */
    offset = ez_buffer_get32le (s);
    hsz = ez_buffer_get32le (s);
    if (hsz != 12 && hsz != 40 && hsz != 56 && hsz != 108) {
        ez_error ("ez_bmp_load: BMP type not supported: unknown\n");
        return NULL;
    }
    if (hsz == 12) {
        s->img_x = ez_buffer_get16le (s);
        s->img_y = ez_buffer_get16le (s);
    } else {
        s->img_x = ez_buffer_get32le (s);
        s->img_y = ez_buffer_get32le (s);
    }
    if (ez_buffer_get16le (s) != 1) {
        ez_error ("ez_bmp_load: bad BMP\n");
        return NULL;
    }
    bpp = ez_buffer_get16le (s);
    if (bpp == 1) {
        ez_error ("ez_bmp_load: BMP type not supported: 1-bit monochrome\n");
        return NULL;
    }
    flip_vertically = ((int) s->img_y) > 0;
    s->img_y = abs ((int) s->img_y);
    if (hsz == 12) {
        if (bpp < 24)
            psize = (offset - 14 - 24) / 3;
    } else {
        compress = ez_buffer_get32le (s);
        if (compress == 1 || compress == 2) {
            ez_error ("ez_bmp_load: BMP type not supported: RLE\n");
            return NULL;
        }
        ez_buffer_get32le (s); /* discard sizeof */
        ez_buffer_get32le (s); /* discard hres */
        ez_buffer_get32le (s); /* discard vres */
        ez_buffer_get32le (s); /* discard colorsused */
        ez_buffer_get32le (s); /* discard max important */
        if (hsz == 40 || hsz == 56) {
            if (hsz == 56) {
                ez_buffer_get32le (s);
                ez_buffer_get32le (s);
                ez_buffer_get32le (s);
                ez_buffer_get32le (s);
            }
            if (bpp == 16 || bpp == 32) {
                mr = mg = mb = 0;
                if (compress == 0) {
                    if (bpp == 32) {
                        mr = 0xffu << 16;
                        mg = 0xffu <<  8;
                        mb = 0xffu <<  0;
                        ma = 0xffu << 24;
                        /* Fake_a = 1; @TODO: check for cases like alpha value
                           is all 0 and switch it to 255 */
                    } else {
                        mr = 31u << 10;
                        mg = 31u <<  5;
                        mb = 31u <<  0;
                    }
                } else if (compress == 3) {
                    mr = ez_buffer_get32le (s);
                    mg = ez_buffer_get32le (s);
                    mb = ez_buffer_get32le (s);
                    /* Not documented, but generated by photoshop and handled
                        by mspaint */
                    if (mr == mg && mg == mb) {
                        /* ?!?!? */
                        ez_error ("ez_bmp_load: bad BMP\n");
                        return NULL;
                    }
                } else {
                    ez_error ("ez_bmp_load: bad BMP\n");
                    return NULL;
                }
            }
        } else {
            if (hsz != 108) {
                ez_error ("ez_bmp_load: internal error hsz = %d\n", hsz);
                return NULL;
            }
            mr = ez_buffer_get32le (s);
            mg = ez_buffer_get32le (s);
            mb = ez_buffer_get32le (s);
            ma = ez_buffer_get32le (s);
            ez_buffer_get32le (s); /* discard color space */
            for (i=0; i < 12; ++i)
                ez_buffer_get32le (s); /* discard color space parameters */
        }
        if (bpp < 16)
            psize = (offset - 14 - hsz) >> 2;
    }
    s->img_n = ma ? 4 : 3;
    if (req_comp && req_comp >= 3) /* we can directly decode 3 or 4 */
        target = req_comp;
    else
        target = s->img_n; /* if they want monochrome, we'll post-convert */
    out = (EZuint8*) malloc (target * s->img_x * s->img_y);
    if (!out) {
        ez_error ("ez_bmp_load: out of memory\n");
        return NULL;
    }
    if (bpp < 16) {
        int z=0;
        if (psize == 0 || psize > 256) {
            free (out);
            ez_error ("ez_bmp_load: corrupt BMP: bad psize\n");
            return NULL;
        }
        for (i=0; i < psize; ++i) {
            pal[i][2] = ez_buffer_get8u (s);
            pal[i][1] = ez_buffer_get8u (s);
            pal[i][0] = ez_buffer_get8u (s);
            if (hsz != 12) ez_buffer_get8 (s);
            pal[i][3] = 255;
        }
        ez_buffer_skip (s, offset - 14 - hsz - psize * (hsz == 12 ? 3 : 4));
        if (bpp == 4) width = (s->img_x + 1) >> 1;
        else if (bpp == 8) width = s->img_x;
        else {
            free (out);
            ez_error ("ez_bmp_load: corrupt BMP: bad pbb\n");
            return NULL;
        }
        pad = (-width)&3;
        for (j=0; j < (int) s->img_y; ++j) {
            for (i=0; i < (int) s->img_x; i += 2) {
                int v=ez_buffer_get8 (s), v2=0;
                if (bpp == 4) {
                    v2 = v & 15;
                    v >>= 4;
                }
                out[z++] = pal[v][0];
                out[z++] = pal[v][1];
                out[z++] = pal[v][2];
                if (target == 4) out[z++] = 255;
                if (i+1 == (int) s->img_x) break;
                v = (bpp == 8) ? ez_buffer_get8 (s) : v2;
                out[z++] = pal[v][0];
                out[z++] = pal[v][1];
                out[z++] = pal[v][2];
                if (target == 4) out[z++] = 255;
            }
            ez_buffer_skip (s, pad);
        }
    } else {
        int rshift=0, gshift=0, bshift=0, ashift=0, rcount=0, gcount=0,
            bcount=0, acount=0;
        int z = 0;
        int easy=0;
        ez_buffer_skip (s, offset - 14 - hsz);
        if (bpp == 24) width = 3 * s->img_x;
        else if (bpp == 16) width = 2*s->img_x;
        else /* bpp = 32 and pad = 0 */ width=0;
        pad = (-width) & 3;
        if (bpp == 24) {
            easy = 1;
        } else if (bpp == 32) {
            if (mb == 0xff       && mg == 0xff00 &&
                mr == 0x00ff0000 && ma == 0xff000000)
                easy = 2;
        }
        if (!easy) {
            if (!mr || !mg || !mb) {
                free (out);
                ez_error ("ez_bmp_load: corrupt BMP: bad masks\n");
                return NULL;
            }
            /* Right shift amt to put high bit in position #7 */
            rshift = ez_bmp_highest_bit (mr)-7; rcount = ez_bmp_bitcount (mr);
            gshift = ez_bmp_highest_bit (mg)-7; gcount = ez_bmp_bitcount (mr);
            bshift = ez_bmp_highest_bit (mb)-7; bcount = ez_bmp_bitcount (mr);
            ashift = ez_bmp_highest_bit (ma)-7; acount = ez_bmp_bitcount (mr);
        }
        for (j=0; j < (int) s->img_y; ++j) {
            if (easy) {
                for (i=0; i < (int) s->img_x; ++i) {
                    int a;
                    out[z+2] = ez_buffer_get8u (s);
                    out[z+1] = ez_buffer_get8u (s);
                    out[z+0] = ez_buffer_get8u (s);
                    z += 3;
                    a = (easy == 2 ? ez_buffer_get8 (s) : 255);
                    if (target == 4) out[z++] = (EZuint8) a;
                }
            } else {
                for (i=0; i < (int) s->img_x; ++i) {
                    Ez_uint32 v = (bpp == 16 ? (Ez_uint32) ez_buffer_get16le (s)
                                             : ez_buffer_get32le (s));
                    int a;
                    out[z++] = (EZuint8) ez_bmp_shiftsigned (v & mr, rshift, rcount);
                    out[z++] = (EZuint8) ez_bmp_shiftsigned (v & mg, gshift, gcount);
                    out[z++] = (EZuint8) ez_bmp_shiftsigned (v & mb, bshift, bcount);
                    a = (ma ? ez_bmp_shiftsigned (v & ma, ashift, acount) : 255);
                    if (target == 4) out[z++] = (EZuint8) a;
                }
            }
            ez_buffer_skip (s, pad);
        }
    }
    if (flip_vertically) {
        EZuint8 t;
        for (j=0; j < (int) s->img_y>>1; ++j) {
            EZuint8 *p1 = out +      j     *s->img_x*target;
            EZuint8 *p2 = out + (s->img_y-1-j)*s->img_x*target;
            for (i=0; i < (int) s->img_x*target; ++i) {
                t = p1[i], p1[i] = p2[i], p2[i] = t;
            }
        }
    }

    if (req_comp && req_comp != target) {
        out = ez_convert_format (out, target, req_comp, s->img_x, s->img_y);
        if (out == NULL) return out; /* ez_convert_format frees input on failure */
    }

    *x = s->img_x;
    *y = s->img_y;
    if (comp) *comp = s->img_n;
    return out;
}


EZuint8 *ez_stbi_bmp_load (Ez_stbi *s, int *x, int *y, int *comp, int req_comp)
{
    return ez_bmp_load (s, x, y, comp, req_comp);
}


int ez_stbi_bmp_info (Ez_stbi *s, int *x, int *y, int *comp)
{
    int hsz;
    if (ez_buffer_get8 (s) != 'B' || ez_buffer_get8 (s) != 'M') {
        ez_stbi_rewind ( s );
        return 0;
    }
    ez_buffer_skip (s, 12);
    hsz = ez_buffer_get32le (s);
    if (hsz != 12 && hsz != 40 && hsz != 56 && hsz != 108) {
        ez_stbi_rewind ( s );
        return 0;
    }
    if (hsz == 12) {
        *x = ez_buffer_get16le (s);
        *y = ez_buffer_get16le (s);
    } else {
        *x = ez_buffer_get32le (s);
        *y = ez_buffer_get32le (s);
    }
    if (ez_buffer_get16le (s) != 1) {
        ez_stbi_rewind ( s );
        return 0;
    }
    *comp = ez_buffer_get16le (s) / 8;
    return 1;
}


/*
 * GIF loader -- public domain by Jean-Marc Lienher -- simplified/shrunk by stb
*/

typedef struct Ez_stbi_gif_lzw_struct {
    Ez_int16 prefix;
    EZuint8 first;
    EZuint8 suffix;
} Ez_stbi_gif_lzw;


typedef struct Ez_stbi_gif_struct {
    int w, h;
    EZuint8 *out;                 /* output buffer (always 4 components) */
    int flags, bgindex, ratio, transparent, eflags;
    EZuint8  pal[256][4];
    EZuint8 lpal[256][4];
    Ez_stbi_gif_lzw codes[4096];
    EZuint8 *color_table;
    int parse, step;
    int lflags;
    int start_x, start_y;
    int max_x, max_y;
    int cur_x, cur_y;
    int line_size;
} Ez_stbi_gif;


int ez_gif_test (Ez_stbi *s)
{
    int sz;
    if (ez_buffer_get8 (s) != 'G' || ez_buffer_get8 (s) != 'I' ||
        ez_buffer_get8 (s) != 'F' || ez_buffer_get8 (s) != '8') return 0;
    sz = ez_buffer_get8 (s);
    if (sz != '9' && sz != '7') return 0;
    if (ez_buffer_get8 (s) != 'a') return 0;
    return 1;
}


int ez_stbi_gif_test (Ez_stbi *s)
{
    int r = ez_gif_test (s);
    ez_stbi_rewind (s);
    return r;
}


void ez_stbi_gif_parse_colortable (Ez_stbi *s, EZuint8 pal[256][4],
    int num_entries, int transp)
{
    int i;
    for (i=0; i < num_entries; ++i) {
        pal[i][2] = ez_buffer_get8u (s);
        pal[i][1] = ez_buffer_get8u (s);
        pal[i][0] = ez_buffer_get8u (s);
        pal[i][3] = transp ? 0 : 255;
    }
}


int ez_stbi_gif_header (Ez_stbi *s, Ez_stbi_gif *g, int *comp, int is_info)
{
    EZuint8 version;
    if (ez_buffer_get8 (s) != 'G' || ez_buffer_get8 (s) != 'I' ||
        ez_buffer_get8 (s) != 'F' || ez_buffer_get8 (s) != '8') {
        ez_error ("ez_stbi_gif_header: corrupt GIF: not GIF\n");
        return 0;
    }

    version = ez_buffer_get8u (s);
    if (version != '7' && version != '9') {
        ez_error ("ez_stbi_gif_header: corrupt GIF: not GIF\n");
        return 0;
    }
    if (ez_buffer_get8 (s) != 'a') {
        ez_error ("ez_stbi_gif_header: corrupt GIF: not GIF\n");
        return 0;
    }

    g->w = ez_buffer_get16le (s);
    g->h = ez_buffer_get16le (s);
    g->flags = ez_buffer_get8 (s);
    g->bgindex = ez_buffer_get8 (s);
    g->ratio = ez_buffer_get8 (s);
    g->transparent = -1;

    /* can't actually tell whether it's 3 or 4 until we parse the comments */
    if (comp != 0) *comp = 4;

    if (is_info) return 1;

    if (g->flags & 0x80)
        ez_stbi_gif_parse_colortable (s, g->pal, 2 << (g->flags & 7), -1);

    return 1;
}


int ez_stbi_gif_info_raw (Ez_stbi *s, int *x, int *y, int *comp)
{
    Ez_stbi_gif g;
    if (!ez_stbi_gif_header (s, &g, comp, 1)) {
        ez_stbi_rewind ( s );
        return 0;
    }
    if (x) *x = g.w;
    if (y) *y = g.h;
    return 1;
}


void ez_stbi_out_gif_code (Ez_stbi_gif *g, Ez_uint16 code)
{
    EZuint8 *p, *c;

   /* Recurse to decode the prefixes, since the linked-list is backwards,
      and working backwards through an interleaved image would be nasty */
    if (g->codes[code].prefix >= 0)
        ez_stbi_out_gif_code (g, g->codes[code].prefix);

    if (g->cur_y >= g->max_y) return;

    p = &g->out[g->cur_x + g->cur_y];
    c = &g->color_table[g->codes[code].suffix * 4];

    if (c[3] >= 128) {
        p[0] = c[2];
        p[1] = c[1];
        p[2] = c[0];
        p[3] = c[3];
    }
    g->cur_x += 4;

    if (g->cur_x >= g->max_x) {
        g->cur_x = g->start_x;
        g->cur_y += g->step;

        while (g->cur_y >= g->max_y && g->parse > 0) {
            g->step = (1 << g->parse) * g->line_size;
            g->cur_y = g->start_y + (g->step >> 1);
            --g->parse;
        }
    }
}


EZuint8 *ez_stbi_process_gif_raster (Ez_stbi *s, Ez_stbi_gif *g)
{
    EZuint8 lzw_cs;
    Ez_int32 len, code;
    Ez_uint32 first;
    Ez_int32 codesize, codemask, avail, oldcode, bits, valid_bits, clear;
    Ez_stbi_gif_lzw *p;

    lzw_cs = ez_buffer_get8u (s);
    clear = 1 << lzw_cs;
    first = 1;
    codesize = lzw_cs + 1;
    codemask = (1 << codesize) - 1;
    bits = 0;
    valid_bits = 0;
    for (code = 0; code < clear; code++) {
        g->codes[code].prefix = -1;
        g->codes[code].first = (EZuint8) code;
        g->codes[code].suffix = (EZuint8) code;
    }

   /* Support no starting clear code */
    avail = clear+2;
    oldcode = -1;

    len = 0;
    for (;;) {
        if (valid_bits < codesize) {
            if (len == 0) {
                len = ez_buffer_get8 (s); /* start new block */
                if (len == 0)
                    return g->out;
            }
            --len;
            bits |= (Ez_int32) ez_buffer_get8 (s) << valid_bits;
            valid_bits += 8;
        } else {
            Ez_int32 code = bits & codemask;
            bits >>= codesize;
            valid_bits -= codesize;
            /* @OPTIMIZE: is there some way we can accelerate the non-clear path? */
            if (code == clear) {  /* clear code */
                codesize = lzw_cs + 1;
                codemask = (1 << codesize) - 1;
                avail = clear + 2;
                oldcode = -1;
                first = 0;
            } else if (code == clear + 1) { /* end of stream code */
                ez_buffer_skip (s, len);
                while ((len = ez_buffer_get8 (s)) > 0)
                    ez_buffer_skip (s, len);
                return g->out;
            } else if (code <= avail) {
                if (first) {
                    ez_error ("ez_stbi_process_gif_raster: corrupt GIF:"
                        " no clear code\n");
                    return NULL;
                }
                if (oldcode >= 0) {
                    p = &g->codes[avail++];
                    if (avail > 4096) {
                        ez_error ("ez_stbi_process_gif_raster: corrupt GIF:"
                            " too many codes\n");
                        return NULL;
                    }
                    p->prefix = (Ez_int16) oldcode;
                    p->first = g->codes[oldcode].first;
                    p->suffix = (code == avail) ? p->first : g->codes[code].first;
                } else if (code == avail) {
                    ez_error ("ez_stbi_process_gif_raster: corrupt GIF:"
                        " illegal code in raster\n");
                    return NULL;
                }

                ez_stbi_out_gif_code (g, (Ez_uint16) code);

                if ((avail & codemask) == 0 && avail <= 0x0FFF) {
                    codesize++;
                    codemask = (1 << codesize) - 1;
                }

                oldcode = code;
            } else {
                ez_error ("ez_stbi_process_gif_raster: corrupt GIF:"
                    " illegal code in raster\n");
                return NULL;
            }
        }
    }
}


void ez_stbi_fill_gif_background (Ez_stbi_gif *g)
{
    int i;
    EZuint8 *c = g->pal[g->bgindex];
    /* @OPTIMIZE: write a dword at a time */
    for (i = 0; i < g->w * g->h * 4; i += 4) {
        EZuint8 *p  = &g->out[i];
        p[0] = c[2];
        p[1] = c[1];
        p[2] = c[0];
        p[3] = c[3];
    }
}

/* This function is designed to support animated gifs,
   although stb_image doesn't support it */

EZuint8 *ez_stbi_gif_load_next (Ez_stbi *s, Ez_stbi_gif *g, int *comp,
    int req_comp)
{
    int i;
    EZuint8 *old_out = 0;

    if (g->out == 0) {
        if (!ez_stbi_gif_header (s, g, comp, 0)) return 0;
        g->out = (EZuint8*) malloc (4 * g->w * g->h);
        if (g->out == 0) {
            ez_error ("ez_stbi_gif_load_next: out of memory\n");
            return NULL;
        }
        ez_stbi_fill_gif_background (g);
    } else {
        /* Animated-gif-only path */
        if (( (g->eflags & 0x1C) >> 2) == 3) {
            old_out = g->out;
            g->out = (EZuint8*) malloc (4 * g->w * g->h);
            if (g->out == 0) {
                ez_error ("ez_stbi_gif_load_next: out of memory\n");
                return NULL;
            }
            memcpy (g->out, old_out, g->w*g->h*4);
        }
    }

    for (;;) {
        switch (ez_buffer_get8 (s)) {
            case 0x2C: /* Image Descriptor */
            {
                Ez_int32 x, y, w, h;
                EZuint8 *o;

                x = ez_buffer_get16le (s);
                y = ez_buffer_get16le (s);
                w = ez_buffer_get16le (s);
                h = ez_buffer_get16le (s);
                if (( (x + w) > (g->w)) || ((y + h) > (g->h))) {
                    ez_error ("ez_stbi_gif_load_next: corrupt GIF:"
                        " bad image descriptor\n");
                    return NULL;
                }

                g->line_size = g->w * 4;
                g->start_x = x * 4;
                g->start_y = y * g->line_size;
                g->max_x   = g->start_x + w * 4;
                g->max_y   = g->start_y + h * g->line_size;
                g->cur_x   = g->start_x;
                g->cur_y   = g->start_y;

                g->lflags = ez_buffer_get8 (s);

                if (g->lflags & 0x40) {
                    g->step = 8 * g->line_size; /* first interlaced spacing */
                    g->parse = 3;
                } else {
                    g->step = g->line_size;
                    g->parse = 0;
                }

                if (g->lflags & 0x80) {
                    ez_stbi_gif_parse_colortable (s, g->lpal,
                        2 << (g->lflags & 7),
                        g->eflags & 0x01 ? g->transparent : -1);
                    g->color_table = (EZuint8 *) g->lpal;
                } else if (g->flags & 0x80) {
                    /* @OPTIMIZE: reset only the previous transparent */
                    for (i=0; i < 256; ++i)
                        g->pal[i][3] = 255;
                    if (g->transparent >= 0 && (g->eflags & 0x01))
                        g->pal[g->transparent][3] = 0;
                    g->color_table = (EZuint8 *) g->pal;
                } else {
                    ez_error ("ez_stbi_gif_load_next: corrupt GIF:"
                        " missing color table\n");
                    return NULL;
                }

                o = ez_stbi_process_gif_raster (s, g);
                if (o == NULL) return NULL;

                if (req_comp && req_comp != 4)
                    o = ez_convert_format (o, 4, req_comp, g->w, g->h);
                return o;
            }

            case 0x21: /* Comment Extension. */
            {
                int len;
                if (ez_buffer_get8 (s) == 0xF9) { /* Graphic Control Extension. */
                    len = ez_buffer_get8 (s);
                    if (len == 4) {
                        g->eflags = ez_buffer_get8 (s);
                        ez_buffer_get16le (s); /* delay */
                        g->transparent = ez_buffer_get8 (s);
                    } else {
                        ez_buffer_skip (s, len);
                        break;
                    }
                }
                while ((len = ez_buffer_get8 (s)) != 0)
                    ez_buffer_skip (s, len);
                break;
            }

            case 0x3B: /* gif stream termination code */
                return (EZuint8 *) 1;

            default:
                ez_error ("ez_stbi_gif_load_next: corrupt GIF: unkbown code\n");
                return NULL;
        }
    }
}


EZuint8 *ez_stbi_gif_load (Ez_stbi *s, int *x, int *y, int *comp, int req_comp)
{
    EZuint8 *u = 0;
    Ez_stbi_gif g;  /* = {0}; gcc complains about it; we use memset instead */
    memset (&g, 0, sizeof (g));

    u = ez_stbi_gif_load_next (s, &g, comp, req_comp);
    if (u == (void *) 1) u = 0;  /* end of animated gif marker */
    if (u) {
        *x = g.w;
        *y = g.h;
    }

    return u;
}


int ez_stbi_gif_info (Ez_stbi *s, int *x, int *y, int *comp)
{
    return ez_stbi_gif_info_raw (s, x, y, comp);
}


/*
 * Get image dimensions and components without fully decoding
*/

int ez_stbi_info_main (Ez_stbi *s, int *x, int *y, int *comp)
{
    if (ez_stbi_jpeg_info (s, x, y, comp)) return 1;
    if (ez_stbi_png_info  (s, x, y, comp)) return 1;
    if (ez_stbi_gif_info  (s, x, y, comp)) return 1;
    if (ez_stbi_bmp_info  (s, x, y, comp)) return 1;

    ez_error ("ez_stbi_info_main: image not of any known type, or corrupt\n");
    return 0;
}


int ez_stbi_info (char const *filename, int *x, int *y, int *comp)
{
    FILE *f = fopen (filename, "rb");
    int result;
    if (!f) {
        ez_error ("ez_stbi_info: unable to open file \"%s\"\n", filename);
        return 0;
    }
    result = ez_stbi_info_from_file (f, x, y, comp);
    fclose (f);
    return result;
}


int ez_stbi_info_from_file (FILE *f, int *x, int *y, int *comp)
{
    int r;
    Ez_stbi s;
    long pos = ftell (f);
    ez_start_file (&s, f);
    r = ez_stbi_info_main (&s, x, y, comp);
    fseek (f, pos, SEEK_SET);
    return r;
}


int ez_stbi_info_from_memory (EZuint8 const *buffer, int len, int *x, int *y,
    int *comp)
{
    Ez_stbi s;
    ez_stbi_start_mem (&s, buffer, len);
    return ez_stbi_info_main (&s, x, y, comp);
}


int ez_stbi_info_from_callbacks (Ez_stbi_io_callbacks const *c, void *user,
    int *x, int *y, int *comp)
{
    Ez_stbi s;
    ez_stbi_start_callbacks (&s, (Ez_stbi_io_callbacks *) c, user);
    return ez_stbi_info_main (&s, x, y, comp);
}

#endif /* !USE_ORIGINAL_STBI */
/// @endcond Document_C_source
// Beginning of ez-image++.cpp

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cstdlib>


// EZImage

/// @cond Private_implantation
EZImage::EZImage(Ez_image *i)
 : image(i)
{}
/// @endcond Private_implantation

EZImage::EZImage(int w, int h)
 : image(nullptr)
{ image = ez_image_create(w,h); }

EZImage::EZImage(const EZImage& o)
 : image(nullptr)
{ image = ez_image_dup(o.image); }

EZImage::EZImage(const char *filename)
 : image(nullptr)
{
 image = ez_image_load(filename);
 if(image == nullptr)
   throw std::runtime_error("Unable to build a new EZImage from this image file.");
}

EZImage::~EZImage()
{ ez_image_destroy(image); }

int EZImage::getWidth()  const
 { return image->width; }

int EZImage::getHeight() const
 { return image->height; }

void EZImage::setAlpha(bool has_alpha)
{ ez_image_set_alpha(image,(has_alpha?1:0)); }

bool  EZImage::getAlpha() const
{ return (ez_image_has_alpha(image)==1?true:false); }

void EZImage::setOpacity(int opacity)
{ ez_image_set_opacity(image,opacity); }

int  EZImage::getOpacity() const
{ return ez_image_get_opacity(image); }

EZPixel EZImage::getPixel(int x,int y)
{
 if(x>=image->width || y>=image->height)
   throw std::runtime_error("Au moins une des coordonnées x ou y dépasse les dimensions de l'image dans EZImage::getPixel(int x,int y) !");
 else if(x<0 || y<0)
   throw std::runtime_error("Au moins une des coordonnées x ou y est inférieure à zéro !");
 else return EZPixel(&image->pixels_rgba[(y*image->width+x)*4]);
}

void EZImage::paint(EZWindow& win,int x,int y) const
{ ez_image_paint (EZDrawPrivate::recover(&win), image, x, y); }

void EZImage::paintSubimage(EZWindow& win, int x, int y,int src_x, int src_y, int w, int h) const
{ ez_image_paint_sub(EZDrawPrivate::recover(&win),image,x,y,src_x,src_y,w,h); }

void EZImage::print(int src_x, int src_y, int w, int h) const
{ ez_image_print (image, src_x, src_y, w, h);}

void EZImage::fillRGBA(EZuint8 r, EZuint8 g, EZuint8 b, EZuint8 a)
{ ez_image_fill_rgba(image,r,g,b,a); }

void EZImage::blend(const EZImage& src, int dst_x, int dst_y)
{ ez_image_blend(image,src.image,dst_x,dst_y); }

void EZImage::blendSubimage(const EZImage& src, int dst_x, int dst_y,int src_x, int src_y, int w, int h)
{ ez_image_blend_sub(image,src.image,dst_x,dst_y,src_x, src_y, w, h); }

EZImage *EZImage::extract(int src_x, int src_y, int w, int h) const
{
 Ez_image *res = ez_image_extract(image,src_x,src_y,w,h);
 if(res != nullptr) return new EZImage(res);
 else return nullptr;
}

EZImage* EZImage::scale(double factor) const
{
 Ez_image *res = ez_image_scale(image,factor);
 if(res != nullptr) return new EZImage(res);
 else return nullptr;
}

EZImage *EZImage::verticalSymmetry() const
{
 Ez_image *res = ez_image_sym_ver(image);
 if(res != nullptr) return new EZImage(res);
 else return nullptr;
}

EZImage *EZImage::horizontalSymmetry() const
{
 Ez_image *res = ez_image_sym_hor(image);
 if(res != nullptr) return new EZImage(res);
 else return nullptr;
}

EZImage* EZImage::rotate(double theta,int quality) const
{
 Ez_image *res = ez_image_rotate(image,theta,quality);
 if(res != nullptr) return new EZImage(res);
 else return nullptr;
}

void     EZImage::rotatePoint(double theta, int src_x, int src_y,int& dst_x, int& dst_y)
{ ez_image_rotate_point(image,theta,src_x,src_y,&dst_x,&dst_y); }

// EZPixmap
#if 0
EZPixmap::EZPixmap()
{ pixmap = ez_pixmap_new(); }
#endif

EZPixmap::EZPixmap(const EZImage& image)
{ pixmap = ez_pixmap_create_from_image(image.image); }

EZPixmap::~EZPixmap()
{ ez_pixmap_destroy(pixmap); }

void EZPixmap::paint(EZWindow& win,int x, int y) const
{ ez_pixmap_paint(EZDrawPrivate::recover(&win),pixmap,x,y); }

void EZPixmap::tile(EZWindow& win,int x, int y, int w, int h) const
{ ez_pixmap_tile(EZDrawPrivate::recover(&win),pixmap,x,y,w,h); }
