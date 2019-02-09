#include "quakedef.h"
#include "Model.hpp"

void CModel::Init()
{
	Mod_Init();
};

void CModel::ClearAll()
{
	Mod_ClearAll();
};

model_t *CModel::GetForName(const char *name, bool crash)
{
	return Mod_ForName(name, crash);
};

void *CModel::GetExtradata(model_t *mod)
{
	return Mod_Extradata(mod);
};

void CModel::TouchModel(const char *name)
{
	Mod_TouchModel(name);
};

mleaf_t *CModel::PointInLeaf(float *p, model_t *model)
{
	return Mod_PointInLeaf(p, model);
};

byte *CModel::LeafPVS(mleaf_t *leaf, model_t *model)
{
	return Mod_LeafPVS(leaf, model);
};