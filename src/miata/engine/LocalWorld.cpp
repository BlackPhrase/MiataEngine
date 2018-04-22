/// @file

#include "quakedef.hpp"
#include "LocalWorld.hpp"

int num_temp_entities;
entity_t cl_temp_entities[MAX_TEMP_ENTITIES];
beam_t cl_beams[MAX_BEAMS];

sfx_t *cl_sfx_wizhit;
sfx_t *cl_sfx_knighthit;
sfx_t *cl_sfx_tink1;
sfx_t *cl_sfx_ric1;
sfx_t *cl_sfx_ric2;
sfx_t *cl_sfx_ric3;
sfx_t *cl_sfx_r_exp3;
#ifdef QUAKE2
sfx_t *cl_sfx_imp;
sfx_t *cl_sfx_rail;
#endif

/*
==============
CL_PrintEntities_f
==============
*/
void CL_PrintEntities_f()
{
	entity_t *ent;
	int i;

	for(i = 0, ent = cl_entities; i < cl.num_entities; i++, ent++)
	{
		Con_Printf("%3i:", i);
		if(!ent->model)
		{
			Con_Printf("EMPTY\n");
			continue;
		}
		Con_Printf("%s:%2i  (%5.1f,%5.1f,%5.1f) [%5.1f %5.1f %5.1f]\n", ent->model->name, ent->frame, ent->origin[0], ent->origin[1], ent->origin[2], ent->angles[0], ent->angles[1], ent->angles[2]);
	}
};

CLocalWorld::CLocalWorld() = default;
CLocalWorld::~CLocalWorld() = default;

void CLocalWorld::Init()
{
	Cmd_AddCommand("entities", CL_PrintEntities_f);
	
	InitTEnts();
};

void CLocalWorld::Update(float afTimeStep)
{
	CL_DecayLights();
	CL_UpdateTEnts();
};

/*
=================
CL_ParseTEnt
=================
*/
void CLocalWorld::InitTEnts()
{
	cl_sfx_wizhit = mpSoundManager->PrecacheSound("wizard/hit.wav");
	cl_sfx_knighthit = mpSoundManager->PrecacheSound("hknight/hit.wav");
	cl_sfx_tink1 = mpSoundManager->PrecacheSound("weapons/tink1.wav");
	cl_sfx_ric1 = mpSoundManager->PrecacheSound("weapons/ric1.wav");
	cl_sfx_ric2 = mpSoundManager->PrecacheSound("weapons/ric2.wav");
	cl_sfx_ric3 = mpSoundManager->PrecacheSound("weapons/ric3.wav");
	cl_sfx_r_exp3 = mpSoundManager->PrecacheSound("weapons/r_exp3.wav");
#ifdef QUAKE2
	cl_sfx_imp = mpSoundManager->PrecacheSound("shambler/sattck1.wav");
	cl_sfx_rail = mpSoundManager->PrecacheSound("weapons/lstart.wav");
#endif
};

void CLocalWorld::StartSound()
{
};

void CLocalWorld::SpawnParticle()
{
};

/*
===============
CL_AllocDlight

===============
*/
dlight_t *CL_AllocDlight(int key)
{
	int i;
	dlight_t *dl;

	// first look for an exact key match
	if(key)
	{
		dl = cl_dlights;
		for(i = 0; i < MAX_DLIGHTS; i++, dl++)
		{
			if(dl->key == key)
			{
				memset(dl, 0, sizeof(*dl));
				dl->key = key;
				return dl;
			}
		}
	}

	// then look for anything else
	dl = cl_dlights;
	for(i = 0; i < MAX_DLIGHTS; i++, dl++)
	{
		if(dl->die < cl.time)
		{
			memset(dl, 0, sizeof(*dl));
			dl->key = key;
			return dl;
		}
	}

	dl = &cl_dlights[0];
	memset(dl, 0, sizeof(*dl));
	dl->key = key;
	return dl;
}

/*
=================
CL_NewTempEntity
=================
*/
entity_t *CLocalWorld::NewTempEntity()
{
	entity_t *ent;

	if(cl_numvisedicts == MAX_VISEDICTS)
		return NULL;
	if(num_temp_entities == MAX_TEMP_ENTITIES)
		return NULL;
	ent = &cl_temp_entities[num_temp_entities];
	memset(ent, 0, sizeof(*ent));
	num_temp_entities++;
	cl_visedicts[cl_numvisedicts] = ent;
	cl_numvisedicts++;

	ent->colormap = vid.colormap;
	return ent;
}

/*
===============
CL_DecayLights

===============
*/
void CL_DecayLights(void)
{
	int i;
	dlight_t *dl;
	float time;

	time = cl.time - cl.oldtime;

	dl = cl_dlights;
	for(i = 0; i < MAX_DLIGHTS; i++, dl++)
	{
		if(dl->die < cl.time || !dl->radius)
			continue;

		dl->radius -= time * dl->decay;
		if(dl->radius < 0)
			dl->radius = 0;
	}
}

/*
=================
CL_UpdateTEnts
=================
*/
void CL_UpdateTEnts(void)
{
	int i;
	beam_t *b;
	vec3_t dist, org;
	float d;
	entity_t *ent;
	float yaw, pitch;
	float forward;

	num_temp_entities = 0;

	// update lightning
	for(i = 0, b = cl_beams; i < MAX_BEAMS; i++, b++)
	{
		if(!b->model || b->endtime < cl.time)
			continue;

		// if coming from the player, update the start position
		if(b->entity == cl.viewentity)
		{
			VectorCopy(cl_entities[cl.viewentity].origin, b->start);
		}

		// calculate pitch and yaw
		VectorSubtract(b->end, b->start, dist);

		if(dist[1] == 0 && dist[0] == 0)
		{
			yaw = 0;
			if(dist[2] > 0)
				pitch = 90;
			else
				pitch = 270;
		}
		else
		{
			yaw = (int)(atan2(dist[1], dist[0]) * 180 / M_PI);
			if(yaw < 0)
				yaw += 360;

			forward = sqrt(dist[0] * dist[0] + dist[1] * dist[1]);
			pitch = (int)(atan2(dist[2], forward) * 180 / M_PI);
			if(pitch < 0)
				pitch += 360;
		}

		// add new entities for the lightning
		VectorCopy(b->start, org);
		d = VectorNormalize(dist);
		while(d > 0)
		{
			ent = CL_NewTempEntity();
			if(!ent)
				return;
			VectorCopy(org, ent->origin);
			ent->model = b->model;
			ent->angles[0] = pitch;
			ent->angles[1] = yaw;
			ent->angles[2] = rand() % 360;

			for(i = 0; i < 3; i++)
				org[i] += dist[i] * 30;
			d -= 30;
		}
	}
}

/*
===============
CL_RelinkEntities
===============
*/
void CL_RelinkEntities(void)
{
	entity_t *ent;
	int i, j;
	float frac, f, d;
	vec3_t delta;
	float bobjrotate;
	vec3_t oldorg;
	dlight_t *dl;

	// determine partial update time
	frac = CL_LerpPoint();

	cl_numvisedicts = 0;

	//
	// interpolate player info
	//
	for(i = 0; i < 3; i++)
		cl.velocity[i] = cl.mvelocity[1][i] +
		frac * (cl.mvelocity[0][i] - cl.mvelocity[1][i]);

	if(cls.demoplayback)
	{
		// interpolate the angles
		for(j = 0; j < 3; j++)
		{
			d = cl.mviewangles[0][j] - cl.mviewangles[1][j];
			if(d > 180)
				d -= 360;
			else if(d < -180)
				d += 360;
			cl.viewangles[j] = cl.mviewangles[1][j] + frac * d;
		}
	}

	bobjrotate = anglemod(100 * cl.time);

	// start on the entity after the world
	for(i = 1, ent = cl_entities + 1; i < cl.num_entities; i++, ent++)
	{
		if(!ent->model)
		{ // empty slot
			if(ent->forcelink)
				R_RemoveEfrags(ent); // just became empty
			continue;
		}

		// if the object wasn't included in the last packet, remove it
		if(ent->msgtime != cl.mtime[0])
		{
			ent->model = NULL;
			continue;
		}

		VectorCopy(ent->origin, oldorg);

		if(ent->forcelink)
		{ // the entity was not updated in the last message
			// so move to the final spot
			VectorCopy(ent->msg_origins[0], ent->origin);
			VectorCopy(ent->msg_angles[0], ent->angles);
		}
		else
		{ // if the delta is large, assume a teleport and don't lerp
			f = frac;
			for(j = 0; j < 3; j++)
			{
				delta[j] = ent->msg_origins[0][j] - ent->msg_origins[1][j];
				if(delta[j] > 100 || delta[j] < -100)
					f = 1; // assume a teleportation, not a motion
			}

			// interpolate the origin and angles
			for(j = 0; j < 3; j++)
			{
				ent->origin[j] = ent->msg_origins[1][j] + f * delta[j];

				d = ent->msg_angles[0][j] - ent->msg_angles[1][j];
				if(d > 180)
					d -= 360;
				else if(d < -180)
					d += 360;
				ent->angles[j] = ent->msg_angles[1][j] + f * d;
			}
		}

		// rotate binary objects locally
		if(ent->model->flags & EF_ROTATE)
			ent->angles[1] = bobjrotate;

		if(ent->effects & EF_BRIGHTFIELD)
			R_EntityParticles(ent);
#ifdef QUAKE2
		if(ent->effects & EF_DARKFIELD)
			R_DarkFieldParticles(ent);
#endif
		if(ent->effects & EF_MUZZLEFLASH)
		{
			vec3_t fv, rv, uv;

			dl = CL_AllocDlight(i);
			VectorCopy(ent->origin, dl->origin);
			dl->origin[2] += 16;
			AngleVectors(ent->angles, fv, rv, uv);

			VectorMA(dl->origin, 18, fv, dl->origin);
			dl->radius = 200 + (rand() & 31);
			dl->minlight = 32;
			dl->die = cl.time + 0.1;
		}
		if(ent->effects & EF_BRIGHTLIGHT)
		{
			dl = CL_AllocDlight(i);
			VectorCopy(ent->origin, dl->origin);
			dl->origin[2] += 16;
			dl->radius = 400 + (rand() & 31);
			dl->die = cl.time + 0.001;
		}
		if(ent->effects & EF_DIMLIGHT)
		{
			dl = CL_AllocDlight(i);
			VectorCopy(ent->origin, dl->origin);
			dl->radius = 200 + (rand() & 31);
			dl->die = cl.time + 0.001;
		}
#ifdef QUAKE2
		if(ent->effects & EF_DARKLIGHT)
		{
			dl = CL_AllocDlight(i);
			VectorCopy(ent->origin, dl->origin);
			dl->radius = 200.0 + (rand() & 31);
			dl->die = cl.time + 0.001;
			dl->dark = true;
		}
		if(ent->effects & EF_LIGHT)
		{
			dl = CL_AllocDlight(i);
			VectorCopy(ent->origin, dl->origin);
			dl->radius = 200;
			dl->die = cl.time + 0.001;
		}
#endif

		if(ent->model->flags & EF_GIB)
			R_RocketTrail(oldorg, ent->origin, 2);
		else if(ent->model->flags & EF_ZOMGIB)
			R_RocketTrail(oldorg, ent->origin, 4);
		else if(ent->model->flags & EF_TRACER)
			R_RocketTrail(oldorg, ent->origin, 3);
		else if(ent->model->flags & EF_TRACER2)
			R_RocketTrail(oldorg, ent->origin, 5);
		else if(ent->model->flags & EF_ROCKET)
		{
			R_RocketTrail(oldorg, ent->origin, 0);
			dl = CL_AllocDlight(i);
			VectorCopy(ent->origin, dl->origin);
			dl->radius = 200;
			dl->die = cl.time + 0.01;
		}
		else if(ent->model->flags & EF_GRENADE)
			R_RocketTrail(oldorg, ent->origin, 1);
		else if(ent->model->flags & EF_TRACER3)
			R_RocketTrail(oldorg, ent->origin, 6);

		ent->forcelink = false;

		if(i == cl.viewentity && !chase_active.value)
			continue;

#ifdef QUAKE2
		if(ent->effects & EF_NODRAW)
			continue;
#endif
		if(cl_numvisedicts < MAX_VISEDICTS)
		{
			cl_visedicts[cl_numvisedicts] = ent;
			cl_numvisedicts++;
		}
	}
}