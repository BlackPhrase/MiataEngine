#pragma once

#include <vector>
#include "IEntity.hpp"

struct IPhysicsBody;

using tEntityVec = std::vector<IEntity*>;
using tEntityComponentVec = std::vector<IEntityComponent*>;

using edict_t = struct edict_s;

class CEdict final : public IEntity
{
public:
	CEdict(const char *asName, int anID);
	CEdict() = default;
	~CEdict() = default;
	
	bool CheckBottom(); // move to physbody?
	bool MoveStep(vec3_t move, bool relink); // move to physbody/charbody?
	
	void SetName(const char *asName) override;
	const char *GetName() const override;
	
	int GetID() const override;
	
	void SetPos(const vec3_t &avPos) override;
	const vec3_t &GetPos() const override {return mvPos;}
	
	void SetPhysBody(IPhysicsBody *apBody);
	IPhysicsBody *GetPhysBody() const {return mpPhysBody;}
	
	//void SetParent(IEntity *apParent) override {mpParent = apParent;}
	//IEntity *GetParent() const override {return mpParent;}
	
	//void AddChild(IEntity *apEntity) override;
	//void RemoveChild(IEntity *apEntity) override;
	//IEntity *GetChild(int anID) override;
	
	//void AddComponent(IEntityComponent *apComponent) override;
	//void RemoveComponent(IEntityComponent *apComponent) override;
	
	//IEntityComponent *GetComponentByIndex(int anID) const override;
	//IEntityComponent *GetComponentByName(const char *asName) const override;
private:
	tEntityVec mvChilds;
	tEntityComponentVec mvComponents;
	
	vec3_t mvPos{};
	
	edict_t *ent{nullptr};
	IEntity *mpParent{nullptr};
	IPhysicsBody *mpPhysBody{nullptr};
};