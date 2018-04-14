/// @file

struct IEntity;

struct IWorld
{
	///
	virtual void ChangeLevel(const char *mapname, const char *startspot) = 0;
	
	///
	virtual IEntity *CreateEntity(const char *name = "") = 0;
	
	///
	virtual void DestroyEntity(IEntity *entity) = 0;
	
	///
	virtual IEntity *GetEntity(int id) const = 0;
};