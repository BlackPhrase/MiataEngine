#pragma once

#define MAX_TEMP_ENTITIES 64    // lightning bolts, etc
#define MAX_STATIC_ENTITIES 128 // torches, etc

dlight_t *CL_AllocDlight(int key);
void CL_DecayLights();

//
// cl_tent
//
void CL_InitTEnts();
void CL_ClearTEnts();
void CL_UpdateTEnts();

//
// cl_ents.c
//
void CL_SetSolidPlayers(int playernum);
void CL_SetUpPlayerPrediction(bool dopred);
void CL_EmitEntities();
void CL_ClearProjectiles();
void CL_ParseProjectiles();
void CL_ParsePacketEntities(bool delta);
void CL_SetSolidEntities();
void CL_ParsePlayerinfo();

//
// cl_pred.c
//
void CL_InitPrediction();
void CL_PredictMove();
void CL_PredictUsercmd(player_state_t *from, player_state_t *to, usercmd_t *u, bool spectator);

class CClientWorld final
{
public:
	CClientWorld() = default;
	~CClientWorld() = default;
	
	void Update();
private:
	// FIXME, allocate dynamically
	//extern efrag_t *cl_efrags;           //[MAX_EFRAGS];
	extern entity_t *cl_entities;        //[MAX_EDICTS];
	extern entity_t *cl_static_entities; //[MAX_STATIC_ENTITIES];
	extern lightstyle_t *cl_lightstyle;  //[MAX_LIGHTSTYLES];
	extern dlight_t *cl_dlights;         //[MAX_DLIGHTS];
	extern entity_t *cl_temp_entities;   //[MAX_TEMP_ENTITIES];
	extern beam_t *cl_beams;             //[MAX_BEAMS];
};