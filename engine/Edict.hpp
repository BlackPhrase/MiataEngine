#pragma once

#include "IEntity.hpp"

struct IPhysicsBody;

class CEdict final : public IEntity
{
public:
	CEdict(const char *asName, int anID);
	CEdict() = default;
	~CEdict() = default;
	
	bool CheckBottom(); // move to physbody?
	bool MoveStep(vec3_t move, bool relink); // move to physbody?
	
	void SetName(const char *asName) override;
	const char *GetName() const override;
	
	int GetID() const override;
	
	void SetPos(vec3_t avPos) override;
	vec3_t GetPos() const override {return mvPos;}
	
	void SetPhysBody(IPhysicsBody *apBody);
	IPhysicsBody *GetPhysBody() const {return mpPhysBody;}
private:
	vec3_t mvPos{vec3_t::origin};
	
	edict_t *ent{nullptr};
	IPhysicsBody *mpPhysBody{nullptr};
};