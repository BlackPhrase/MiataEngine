#include "Model.hpp"

void CModel::Init()
{
};

void CModel::ClearAll()
{
};

model_t *CModel::GetForName(const char *name, bool crash)
{
};

void *CModel::GetExtradata(model_t *mod)
{
};

void CModel::TouchModel(const char *name)
{
};

mleaf_t *CModel::PointInLeaf(float *p, model_t *model)
{
};

byte *CModel::LeafPVS(mleaf_t *leaf, model_t *model)
{
};