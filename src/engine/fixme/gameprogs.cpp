#include "quakedef.h"
#include "game.h"

game_export_t gGameProgs{};

void GameProgs_Init()
{
	//PR_ExecuteProgram(pr_global_struct->main);
};

void GameProgs_Shutdown()
{
};

void GameProgs_SpawnEntities(char *mapname, char *entstring, char *spawnpoint)
{
	// TODO: wrong place
	//PR_ExecuteProgram(pr_global_struct->SetNewParms);
	//PR_ExecuteProgram(pr_global_struct->SetChangeParms);
};

void GameProgs_WriteGame(char *filename, qboolean autosave)
{
};

void GameProgs_ReadGame(char *filename)
{
};

void GameProgs_WriteLevel(char *filename)
{
};

void GameProgs_ReadLevel(char *filename)
{
};

qboolean GameProgs_ClientConnect(edict_t *ent, char *userinfo)
{
	pr_global_struct->self = EDICT_TO_PROG(ent);
	// TODO: userinfo
	PR_ExecuteProgram(pr_global_struct->ClientConnect);
	return true;
};

void GameProgs_ClientBegin(edict_t *ent)
{
	pr_global_struct->self = EDICT_TO_PROG(ent);
	PR_ExecuteProgram(pr_global_struct->PutClientInServer);
};

void GameProgs_ClientUserinfoChanged(edict_t *ent, char *userinfo)
{
};

void GameProgs_ClientDisconnect(edict_t *ent)
{
	pr_global_struct->self = EDICT_TO_PROG(ent);
	PR_ExecuteProgram(pr_global_struct->ClientDisconnect);
};

void GameProgs_ClientCommand(edict_t *ent)
{
	pr_global_struct->self = EDICT_TO_PROG(ent);
	
	if(Q_strcmp(cmdname, "kill"))
		PR_ExecuteProgram(pr_global_struct->ClientKill);
};

void GameProgs_ClientThink(edict_t *ent, usercmd_t *cmd)
{
	pr_global_struct->self = EDICT_TO_PROG(ent);
	// TODO: cmd support
	
	PR_ExecuteProgram(pr_global_struct->PlayerPreThink);
	
	PR_ExecuteProgram(pr_global_struct->PlayerPostThink);
};

void GameProgs_RunFrame()
{
	PR_ExecuteProgram(pr_global_struct->StartFrame);
};

void GameProgs_ServerCommand()
{
};