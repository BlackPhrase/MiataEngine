#pragma once

#include "IClientGame.hpp"

/*
void CL_ParticleSteamEffect2(cl_sustain_t *self);

void CL_TeleporterParticles(entity_state_t *ent);
void CL_ParticleEffect(vec3_t org, vec3_t dir, int color, int count);
void CL_ParticleEffect2(vec3_t org, vec3_t dir, int color, int count);

// RAFAEL
void CL_ParticleEffect3(vec3_t org, vec3_t dir, int color, int count);

void CL_BlasterTrail(vec3_t start, vec3_t end);
void CL_QuadTrail(vec3_t start, vec3_t end);
void CL_RailTrail(vec3_t start, vec3_t end);
void CL_BubbleTrail(vec3_t start, vec3_t end);
void CL_FlagTrail(vec3_t start, vec3_t end, float color);

// RAFAEL
void CL_IonripperTrail(vec3_t start, vec3_t end);

// ========
// PGM
void CL_BlasterParticles2(vec3_t org, vec3_t dir, unsigned int color);
void CL_BlasterTrail2(vec3_t start, vec3_t end);
void CL_DebugTrail(vec3_t start, vec3_t end);
void CL_SmokeTrail(vec3_t start, vec3_t end, int colorStart, int colorRun, int spacing);
void CL_Flashlight(int ent, vec3_t pos);
void CL_ForceWall(vec3_t start, vec3_t end, int color);
void CL_FlameEffects(centity_t *ent, vec3_t origin);
void CL_GenericParticleEffect(vec3_t org, vec3_t dir, int color, int count, int numcolors, int dirspread, float alphavel);
void CL_BubbleTrail2(vec3_t start, vec3_t end, int dist);
void CL_Heatbeam(vec3_t start, vec3_t end);
void CL_ParticleSteamEffect(vec3_t org, vec3_t dir, int color, int count, int magnitude);
void CL_TrackerTrail(vec3_t start, vec3_t end, int particleColor);
void CL_Tracker_Explode(vec3_t origin);
void CL_TagTrail(vec3_t start, vec3_t end, float color);
void CL_ColorFlash(vec3_t pos, int ent, int intensity, float r, float g, float b);
void CL_Tracker_Shell(vec3_t origin);
void CL_MonsterPlasma_Shell(vec3_t origin);
void CL_ColorExplosionParticles(vec3_t org, int color, int run);
void CL_ParticleSmokeEffect(vec3_t org, vec3_t dir, int color, int count, int magnitude);
void CL_Widowbeamout(cl_sustain_t *self);
void CL_Nukeblast(cl_sustain_t *self);
void CL_WidowSplash(vec3_t org);
// PGM
// ========

void CL_ParseMuzzleFlash();
void CL_ParseMuzzleFlash2();
void SmokeAndFlash(vec3_t origin);

//
// cl_tent
//
void CL_SmokeAndFlash(vec3_t origin);

//
// cl_fx
//
cdlight_t *CL_AllocDlight(int key);
void CL_BigTeleportParticles(vec3_t org);
void CL_RocketTrail(vec3_t start, vec3_t end, centity_t *old);
void CL_DiminishingTrail(vec3_t start, vec3_t end, centity_t *old, int flags);
void CL_FlyEffect(centity_t *ent, vec3_t origin);
void CL_BfgParticles(entity_t *ent);
void CL_AddParticles(void);
void CL_EntityEvent(entity_state_t *ent);
// RAFAEL
void CL_TrapParticles(entity_t *ent);

//
// cl_inv
//
void CL_ParseInventory(void);
void CL_KeyInventory(int key);
void CL_DrawInventory(void);
*/

class CClientGame final : public IClientGame
{
public:
	CClientGame(IEngineInterface *apEngine);
	~CClientGame();
	
	bool Init() override;
	void Shutdown() override;
	
	void Frame() override;
private:
	IEngineInterface *mpEngine{nullptr};
};