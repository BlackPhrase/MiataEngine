/*
Copyright (C) 1996-1997 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

/// @file

#pragma once

typedef struct
{
	vec3_t viewangles;

	/// intended velocities
	float forwardmove{0.0f};
	float sidemove{0.0f};
	float upmove{0.0f};

	byte lightlevel{0};
} usercmd_t;

// player_state_t is the information needed by a player entity
// to do move prediction and to generate a drawable entity
typedef struct
{
	int			messagenum;		// all player's won't be updated each frame

	double		state_time;		// not the same as the packet time,
								// because player commands come asyncronously
	usercmd_t	command;		// last command for prediction

	vec3_t		origin;
	vec3_t		viewangles;		// only for demos, not from server
	vec3_t		velocity;
	int			weaponframe;

	int			modelindex;
	int			frame;
	int			skinnum;
	int			effects;

	int			flags;			// dead, gib, etc

	float		waterjumptime;
	int			onground;		// -1 = in air, else pmove entity number
	int			oldbuttons;
} player_state_t;

typedef struct
{
	char name[MAX_SCOREBOARDNAME];
	float entertime;
	int frags;
	int colors; // two 4 bit fields
	byte translations[VID_GRADES * 256];
} scoreboard_t;

typedef struct
{
	int destcolor[3];
	int percent; // 0-256
} cshift_t;

#define CSHIFT_CONTENTS 0
#define CSHIFT_DAMAGE 1
#define CSHIFT_BONUS 2
#define CSHIFT_POWERUP 3
#define NUM_CSHIFTS 4

#define NAME_LENGTH 64

//
// client_state_t should hold all pieces of the client state
//

#define SIGNONS 4 // signon messages to receive before connected

#define MAX_EFRAGS 640

#define MAX_MAPSTRING 2048

#define MAX_DEMOS 8
#define MAX_DEMONAME 16

typedef enum {
	ca_dedicated,    // a dedicated server with no ability to start a client
	ca_disconnected, // full screen console with no connection
	ca_connected     // valid netcon, talking to a server
} cactive_t;

//
// the client_static_t structure is persistant through an arbitrary number
// of server connections
//
typedef struct
{
	cactive_t state;

	// personalization data sent to server
	char mapstring[MAX_QPATH];
	char spawnparms[MAX_MAPSTRING]; // to restart a level

	// demo loop control
	int demonum;                         // -1 = don't play demos
	char demos[MAX_DEMOS][MAX_DEMONAME]; // when not playing

	// demo recording info must be here, because record is started before
	// entering a map (and clearing client_state_t)
	bool demorecording;
	bool demoplayback;
	bool timedemo;
	int forcetrack; // -1 = use normal cd track
	FILE *demofile;
	int td_lastframe;   // to meter out one message a frame
	int td_startframe;  // host_framecount at start
	float td_starttime; // realtime at second frame of timedemo

	// connection information
	int signon; // 0 to SIGNONS
	struct qsocket_s *netcon;
	sizebuf_t message; // writing buffer to send to server

} client_static_t;

extern client_static_t cls;

//
// the client_state_t structure is wiped completely at every
// server signon
//
typedef struct
{
	int movemessages; // since connecting to this server
	                  // throw out the first couple, so the player
	                  // doesn't accidentally do something the
	                  // first frame
	usercmd_t cmd;    // last command sent to the server

	// information for local display
	int stats[MAX_CL_STATS]; // health, etc
	int items;               // inventory bit flags
	float item_gettime[32];  // cl.time of aquiring item, for blinking
	float faceanimtime;      // use anim frame if cl.time < this

	cshift_t cshifts[NUM_CSHIFTS];      // color shifts for damage, powerups
	cshift_t prev_cshifts[NUM_CSHIFTS]; // and content types

	// the client maintains its own idea of view angles, which are
	// sent to the server each frame.  The server sets punchangle when
	// the view is temporarliy offset, and an angle reset commands at the start
	// of each level and after teleporting.
	vec3_t mviewangles[2]; // during demo playback viewangles is lerped
	                       // between these
	vec3_t viewangles;

	vec3_t mvelocity[2]; // update by server, used for lean+bob
	                     // (0 is newest)
	vec3_t velocity;     // lerped between mvelocity[0] and [1]

	vec3_t punchangle; // temporary offset

	// pitch drifting vars
	float idealpitch;
	float pitchvel;
	bool nodrift;
	float driftmove;
	double laststop;

	float viewheight;
	float crouch; // local amount for smoothing stepups

	bool paused; // send over by server
	bool onground;
	bool inwater;

	int intermission;   // don't change view angle, full screen, etc
	int completed_time; // latched at intermission start

	double mtime[2]; // the timestamp of last two messages
	double time;     // clients view of time, should be between
	                 // servertime and oldservertime to generate
	                 // a lerp point for other data
	double oldtime;  // previous cl.time, time-oldtime is used
	                 // to decay light values and smooth step ups

	float last_received_message; // (realtime) for net trouble icon

	//
	// information that is static for the entire time connected to a server
	//
	struct model_s *model_precache[MAX_MODELS];
	struct sfx_s *sound_precache[MAX_SOUNDS];

	char levelname[40]; // for display on solo scoreboard
	int viewentity;     // cl_entitites[cl.viewentity] = player
	int maxclients;
	int gametype;

	// refresh related state
	struct model_s *worldmodel; // cl_entitites[0].model
	struct efrag_s *free_efrags;
	int num_entities; // held in cl_entities array
	int num_statics;  // held in cl_staticentities array
	entity_t viewent; // the gun model

	int cdtrack, looptrack; // cd audio

	// frag scoreboard
	scoreboard_t *scores; // [cl.maxclients]

#ifdef QUAKE2
	// light level at player's position including dlights
	// this is sent back to the server each frame
	// architectually ugly but it works
	int light_level;
#endif
} client_state_t;

//
// cvars
//
extern cvar_t *cl_name;
extern cvar_t *cl_color;

extern cvar_t *cl_upspeed;
extern cvar_t *cl_forwardspeed;
extern cvar_t *cl_backspeed;
extern cvar_t *cl_sidespeed;

//extern	cvar_t	*cl_movespeedkey;

extern cvar_t *cl_yawspeed;
extern cvar_t *cl_pitchspeed;

extern cvar_t *cl_anglespeedkey;

//extern	cvar_t	*cl_autofire;

extern cvar_t *cl_shownet;
//extern	cvar_t	*cl_nolerp;

//extern	cvar_t	*cl_pitchdriftspeed;

extern cvar_t *lookspring;
extern cvar_t *lookstrafe;
extern cvar_t *sensitivity;

extern cvar_t *m_pitch;
extern cvar_t *m_yaw;
extern cvar_t *m_forward;
extern cvar_t *m_side;

extern client_state_t cl;

//=============================================================================

//
// cl_main
//

void CL_Init();

void CL_Disconnect();
void CL_Disconnect_f();
void CL_NextDemo();
bool CL_DemoBehind();

void CL_BeginServerConnect();

//
// cl_input
//
typedef struct
{
	int down[2]; // key nums holding it down
	int state;   // low bit is down state
} kbutton_t;

extern kbutton_t in_mlook, in_klook;
extern kbutton_t in_strafe;
extern kbutton_t in_speed;

void CL_InitInput();
void CL_SendCmd();
void CL_SendMove(usercmd_t *cmd);

void CL_ParseTEnt();

void CL_ClearState();

void CL_ReadPackets();

void CL_WriteToServer(usercmd_t *cmd);
void CL_BaseMove(usercmd_t *cmd);

float CL_KeyState(kbutton_t *key);

//
// cl_demo.c
//
void CL_StopPlayback();
int CL_GetMessage();

void CL_Stop_f();
void CL_Record_f();
//void CL_ReRecord_f ();
void CL_PlayDemo_f();
void CL_TimeDemo_f();

//
// cl_parse.c
//
void CL_ParseServerMessage();
void CL_NewTranslation(int slot);

void CL_SignonReply();

//
// cl_cam.c
//
#define CAM_NONE 0
#define CAM_TRACK 1

extern int autocam;
extern int spec_track; // player# of who we are tracking

bool Cam_DrawViewModel();
bool Cam_DrawPlayer(int playernum);
void Cam_Track(usercmd_t *cmd);
void Cam_FinishMove(usercmd_t *cmd);
void Cam_Reset();
void CL_InitCam();