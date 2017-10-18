#pragma once

struct IPhysicsBody;

class CEdict final
{
public:
	CEdict(const char *asName, int anID);
	CEdict() = default;
	~CEdict() = default;
	
	bool CheckBottom(); // move to physbody?
	bool MoveStep(vec3_t move, bool relink); // move to physbody?
	
	void SetName(const char *name);
	const char *GetName() const;
	
	int GetID() const;
	
	void SetPhysBody(IPhysicsBody *apBody);
	IPhysicsBody *GetPhysBody() const {return mpPhysBody;}
private:
	edict_t *ent{nullptr};
	IPhysicsBody *mpPhysBody{nullptr};
};