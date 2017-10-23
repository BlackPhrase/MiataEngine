

typedef struct
{
	
	
	
	int serverflags;             // episode completion information
	bool changelevel_issued; // cleared when at SV_SpawnServer
} server_static_t;

//=============================================================================

typedef enum { ss_loading,
	           ss_active } server_state_t;

typedef struct
{
	bool active; // false if only a net client

	bool paused;
	bool loadgame; // handle connections specially

	double time;

	int lastcheck; // used by PF_checkclient
	double lastchecktime;

	char name[64]; // map name
#ifdef QUAKE2
	char startspot[64];
#endif
	char modelname[64]; // maps/<name>.bsp, for model_precache[0]
	struct model_s *worldmodel;
	char *model_precache[MAX_MODELS]; // NULL terminated
	struct model_s *models[MAX_MODELS];
	char *sound_precache[MAX_SOUNDS]; // NULL terminated
	char *lightstyles[MAX_LIGHTSTYLES];
	int num_edicts;
	int max_edicts;
	edict_t *edicts;      // can NOT be array indexed, because
	                      // edict_t is variable sized, but can
	                      // be used to reference the world ent
	server_state_t state; // some actions are only valid during load

	CSizeBuffer datagram;
	byte datagram_buf[MAX_DATAGRAM];

	CSizeBuffer reliable_datagram; // copied to all clients at end of frame
	byte reliable_datagram_buf[MAX_DATAGRAM];

	CSizeBuffer signon;
	byte signon_buf[8192];
} server_t;

#define NUM_PING_TIMES 16
#define NUM_SPAWN_PARMS 16

typedef struct client_s
{
	bool active;     // false = client is free
	bool spawned;    // false = don't send datagrams
	bool dropasap;   // has been told to go to another level
	bool privileged; // can execute any host command
	bool sendsignon; // only valid before spawned

	double last_message; // reliable messages must be sent
	                     // periodically

	struct qsocket_s *netconnection; // communications handle

	usercmd_t cmd;  // movement
	vec3_t wishdir; // intended motion calced from cmd

	CSizeBuffer message; // can be added to at any time,
	                   // copied and clear once per frame
	byte msgbuf[MAX_MSGLEN];
	edict_t *edict; // EDICT_NUM(clientnum+1)
	char name[32];  // for printing to other people
	int colors;

	float ping_times[NUM_PING_TIMES];
	int num_pings; // ping_times[num_pings%NUM_PING_TIMES]

	// spawn parms are carried from level to level
	float spawn_parms[NUM_SPAWN_PARMS];

	// client known data for deltas
	int old_frags;
} client_t;

//=============================================================================

#include "server_const.hpp"

//============================================================================

extern cvar_t teamplay;
extern cvar_t skill;
extern cvar_t deathmatch;
extern cvar_t coop;
extern cvar_t fraglimit;
extern cvar_t timelimit;

extern server_static_t svs; // persistant server info
extern server_t sv;         // local server

extern client_t *host_client;

extern jmp_buf host_abortserver;

extern double host_time;

extern edict_t *sv_player;

//===========================================================

void SV_Init();

void SV_ClearDatagram();

int SV_ModelIndex(const char *name);

void SV_SetIdealPitch();

void SV_AddUpdates();

void SV_ClientThink();
void SV_AddClientToServer(struct qsocket_s *ret);

void SV_WriteClientdataToMessage(edict_t *ent, sizebuf_t *msg);

void SV_MoveToGoal();

void SV_RunClients();
void SV_SaveSpawnParms();

void SV_SpawnServer(const char *server, const char *startspot);