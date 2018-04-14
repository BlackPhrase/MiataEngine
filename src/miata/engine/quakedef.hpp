/*
Copyright (C) 1996-1997 Id Software, Inc.
Copyright (C) 2017-2018 BlackPhrase

This program is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file
/// @brief defs common to client and server

#pragma once

#include <cmath>
#include <cstring>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <csetjmp>

//#define GLTEST // experimental stuff

#define QUAKE_GAME // as opposed to utilities

#define VERSION 0.1

//define PARANOID // speed sapping error checking

#define GAMENAME "id1" // directory to look in by default

#if defined(_WIN32) && !defined(WINDED)

#if defined(_M_IX86)
	#define __i386__ 1
#endif

void VID_LockBuffer();
void VID_UnlockBuffer();

#else

#define VID_LockBuffer()
#define VID_UnlockBuffer()

#endif

#if defined __i386__ // && !defined __sun__
#define id386 1
#else
#define id386 0
#endif

#if id386
#define UNALIGNED_OK 1 // set to 0 if unaligned accesses are not supported
#else
#define UNALIGNED_OK 0
#endif

// !!! if this is changed, it must be changed in d_ifacea.h too !!!
constexpr auto CACHE_SIZE = 32; // used to align key data structures

#define UNUSED(x) (x = x) // for pesky compiler / lint warnings

#define MINIMUM_MEMORY 0x550000
//#define MINIMUM_MEMORY_LEVELPAK (MINIMUM_MEMORY + 0x100000)

constexpr auto MAX_NUM_ARGVS = 50;

enum
{
	PITCH = 0, ///< up / down
	YAW,       ///< left / right
	ROLL       ///< fall over
};

constexpr auto MAX_QPATH = 64;   // max length of a quake game pathname
constexpr auto MAX_OSPATH = 128; // max length of a filesystem pathname

constexpr auto ON_EPSILON = 0.1f; // point on plane side epsilon

constexpr auto MAX_MSGLEN = 8000;   // max length of a reliable message
constexpr auto MAX_DATAGRAM = 1024; // max length of unreliable message

//
// per-level limits
//
constexpr auto MAX_EDICTS = 600; // FIXME: ouch! ouch! ouch!
constexpr auto MAX_LIGHTSTYLES = 64;
constexpr auto MAX_MODELS = 1024; // these are sent over the net as bytes
constexpr auto MAX_SOUNDS = 1024; // so they cannot be blindly increased

constexpr auto SAVEGAME_COMMENT_LENGTH = 39;

constexpr auto MAX_STYLESTRING = 64;

//===========================================

constexpr auto MAX_SCOREBOARD = 16; // max numbers of players
constexpr auto MAX_SCOREBOARDNAME = 32;

constexpr auto SOUND_CHANNELS = 8;

#include "commontypes.hpp"
#include "common.hpp"
//#include "bspfile.hpp"
//#include "vid.hpp"
//#include "sys.hpp"
#include "zone.hpp"
#include "mathlib.hpp"

typedef struct
{
	vec3_t origin;
	vec3_t angles;
	int modelindex;
	int frame;
	int colormap;
	int skin;
	int effects;
} entity_state_t;

//#include "wad.hpp"
#include "cvar.hpp"
//#include "screen.hpp"
//#include "net.hpp"
//#include "protocol.hpp"
#include "cmd.hpp"
//#include "sbar.hpp"
//#include "sound.hpp"
//#include "ref.hpp"
//#include "client.hpp"
//#include "progs.hpp"
//#include "server.hpp"

#ifdef GLQUAKE
#include "gl_model.hpp"
#else
//#include "model.hpp"
#endif

//#include "input.hpp"
//#include "world.hpp"
//#include "keys.hpp"
//#include "console.hpp"
//#include "view.hpp"
//#include "menu.hpp"
//#include "crc.hpp"
//#include "cdaudio.hpp"

#ifdef GLQUAKE
#include "glquake.hpp"
#endif

//=============================================================================

extern bool noclip_anglehack;

//
// host
//

extern cvar_t sys_ticrate;
extern cvar_t sys_nostdout;
extern cvar_t developer;

extern double host_frametime;
extern byte *host_basepal;
extern byte *host_colormap;
extern int host_framecount; // incremented every frame, never reset
extern double realtime;     // not bounded in any way, changed at
                            // start of every frame, never reset

void Host_ServerFrame(); // SV_Frame

void Host_Frame(float time);

void Host_Quit_f();


extern bool msg_suppress_1; // suppresses resolution and cache size console output
                                //  an fullscreen DIB focus gain/loss
extern int current_skill;       // skill level for currently loaded level (in case
                                //  the user changes the cvar while the level is
                                //  running, this reflects the level actually in use)

extern bool isDedicated;

extern int minimum_memory;

//
// chase
//
extern cvar_t chase_active;

void Chase_Init();
void Chase_Reset();
void Chase_Update();