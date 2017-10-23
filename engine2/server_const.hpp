#pragma once

// edict->movetype values
#define MOVETYPE_NONE 0 // never moves
#define MOVETYPE_ANGLENOCLIP 1
#define MOVETYPE_ANGLECLIP 2
#define MOVETYPE_WALK 3 // gravity
#define MOVETYPE_STEP 4 // gravity, special edge handling
#define MOVETYPE_FLY 5
#define MOVETYPE_TOSS 6 // gravity
#define MOVETYPE_PUSH 7 // no clip to world, push and crush
#define MOVETYPE_NOCLIP 8
#define MOVETYPE_FLYMISSILE 9 // extra size to monsters
#define MOVETYPE_BOUNCE 10
#ifdef QUAKE2
#define MOVETYPE_BOUNCEMISSILE 11 // bounce w/o gravity
#define MOVETYPE_FOLLOW 12        // track movement of aiment
#endif

// edict->solid values
#define SOLID_NOT 0      // no interaction with other objects
#define SOLID_TRIGGER 1  // touch on edge, but not blocking
#define SOLID_BBOX 2     // touch on edge, block
#define SOLID_SLIDEBOX 3 // touch on edge, but not an onground
#define SOLID_BSP 4      // bsp clip, touch on edge, block

// edict->deadflag values
#define DEAD_NO 0
#define DEAD_DYING 1
#define DEAD_DEAD 2

#define DAMAGE_NO 0
#define DAMAGE_YES 1
#define DAMAGE_AIM 2

// edict->flags
#define FL_FLY 1
#define FL_SWIM 2
//#define	FL_GLIMPSE				4
#define FL_CONVEYOR 4
#define FL_CLIENT 8
#define FL_INWATER 16
#define FL_MONSTER 32
#define FL_GODMODE 64
#define FL_NOTARGET 128
#define FL_ITEM 256
#define FL_ONGROUND 512
#define FL_PARTIALGROUND 1024 // not all corners are valid
#define FL_WATERJUMP 2048     // player jumping out of water
#define FL_JUMPRELEASED 4096  // for jump debouncing
#ifdef QUAKE2
#define FL_FLASHLIGHT 8192
#define FL_ARCHIVE_OVERRIDE 1048576
#endif

// entity effects

#define EF_BRIGHTFIELD 1
#define EF_MUZZLEFLASH 2
#define EF_BRIGHTLIGHT 4
#define EF_DIMLIGHT 8
#ifdef QUAKE2
#define EF_DARKLIGHT 16
#define EF_DARKFIELD 32
#define EF_LIGHT 64
#define EF_NODRAW 128
#endif

#define SPAWNFLAG_NOT_EASY 256
#define SPAWNFLAG_NOT_MEDIUM 512
#define SPAWNFLAG_NOT_HARD 1024
#define SPAWNFLAG_NOT_DEATHMATCH 2048

#ifdef QUAKE2
// server flags
#define SFL_EPISODE_1 1
#define SFL_EPISODE_2 2
#define SFL_EPISODE_3 4
#define SFL_EPISODE_4 8
#define SFL_NEW_UNIT 16
#define SFL_NEW_EPISODE 32
#define SFL_CROSS_TRIGGERS 65280
#endif