#pragma once

class CModel
{
public:
	void Init();
	
	void ClearAll();
	
	model_t *GetForName(const char *name, bool crash);
	void *GetExtradata(model_t *mod); // handles caching
	
	void TouchModel(const char *name);

	mleaf_t *PointInLeaf(float *p, model_t *model);
	byte *LeafPVS(mleaf_t *leaf, model_t *model);
};