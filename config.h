/* See LICENSE file for copyright and license details. */
#include <X11/keysym.h>
#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Verdana Nerd Font:size=11" };
static const char dmenufont[]       = "Verdana Nerd Font:size=11";
/*static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";*/
#include "/home/kali/.cache/wal/colors-wal-dwm.h"

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
	{ "nsxiv",     NULL,       NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands 
 dmenu setup (conflicts with pywal) static char dmenumon[2] = "0";  component of dmenucmd, manipulated in spawn() 
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL }; */

/* add alongside dmenucmd: */
static const char *roficmd[] = { "rofi", "-show", "drun", NULL };
static const char *termcmd[]  = { "kitty", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = roficmd } },
	{ MODKEY,             		XK_Return, spawn,          {.v = termcmd } },
	
	/* F2 capture full screenshot to clipboard */
 	{ 0,				XK_F2,     spawn,	   SHCMD("maim | xclip -selection clipboard -t image/png") },

	/* shift F2 capture selection screenshot to clipboard */
	{ ShiftMask,                    XK_F2,      spawn,            SHCMD("maim -s | xclip -selection clipboard -t image/png") },

	/* F20 capture full screenshot to clipboard */
 	{ 0,				XF86XK_Launch1,    spawn,	   SHCMD("maim | xclip -selection clipboard -t image/png") }, 
	
	/* Shift + rog button on laptop, selection screenshot to clipboard */
	{ ShiftMask,                    XF86XK_Launch1,    spawn,          SHCMD("maim -s | xclip -selection clipboard -t image/png") },
    	
	/* Control+ rog button on laptop    → full-screen to file */
    	{ ControlMask,                  XF86XK_Launch1,    spawn,          SHCMD("maim ~/Pictures/shot_$(date +%Y%m%d_%H%M%S).png") },
    	
	/* Ctrl+Shift+ rog button on laptop → region to file */
    	{ ControlMask|ShiftMask,        XF86XK_Launch1,    spawn,          SHCMD("maim -s ~/Pictures/shot_$(date +%Y%m%d_%H%M%S).png") },


	/* volume controls via pactl */
	{ 0,                  	        XF86XK_AudioLowerVolume, spawn, SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -5%") },
	{ 0,                       	XF86XK_AudioRaiseVolume, spawn, SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +5%") },
	{ 0,                       	XF86XK_AudioMute,        spawn, SHCMD("pactl set-sink-mute @DEFAULT_SINK@ toggle") },

	/* volume controls via pactl for normal keyboard (fkeys) */
	{ 0,                  	        XK_F9, spawn, SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -5%") },
	{ 0,                       	XK_F10, spawn, SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +5%") },
	{ 0,                       	XK_F8, spawn, SHCMD("pactl set-sink-mute @DEFAULT_SINK@ toggle") },

	/* brightness controls */
	{ 0,                       	XF86XK_MonBrightnessUp,   spawn, SHCMD("brightnessctl set +10%") },
	{ 0,                        	XF86XK_MonBrightnessDown, spawn, SHCMD("brightnessctl set 10%-") },
	

	/* keyboard backlight */
	{ 0,			 	XF86XK_KbdBrightnessUp,   spawn, SHCMD("asusctl --next-kbd-bright") },
	{ 0, 				XF86XK_KbdBrightnessDown, spawn, SHCMD("asusctl --prev-kbd-bright") },
	

	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

