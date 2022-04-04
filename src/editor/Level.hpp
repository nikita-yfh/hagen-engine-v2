#pragma once
#include "Body.hpp"
#include "Joint.hpp"
#include "Mouse.hpp"
#include "Colors.hpp"
#include "Texture.hpp"
#include "Image.hpp"
#include "Directory.hpp"
#include <wx/gdicmn.h>

class Level : public Object{
public:
	Level(const Directory &dir);
	~Level(){
		Clear();
	}

	virtual void Draw(const Colors &colors) const override;
	virtual void DrawPoints(const Colors &colors) const override;
	virtual bool UpdatePoints(const Mouse &mouse) override;
	virtual uint8_t GetObjectType() const override{return LEVEL;}

	void UnselectAllPoints();
	void UnselectAll();
	void SelectAll();

	int GetSelectedBodyCount() const;
	int GetSelectedNotDynamicBodyCount() const;
	int GetSelectedJointCount() const;
	int GetSelectedCount() const;

	Object *GetFirstSelected();
	Object *GetFirstSelectedAll();

	void DeleteSelected();

	void AddObject(Object *object);
	void AddImage(const wxString &path);
	void AddFixture(Fixture *fixture);
	void AddJoint(Joint *joint);
	
	inline bool IsCreating() const{
		return create != nullptr;
	}
	bool CancelCreating();

	void Clear();

	Texture *AddTexture(const wxString&);
	void DeleteTexture(const wxString&);
	void ReloadTextures();
	inline const Texture *GetTextures() const{
		return textures;
	}

	void UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const;
	void OnPropertyGridChange(const wxString &name, const wxVariant &value);

	inline const Directory &GetGameDir() const{
		return gameDir;
	}
	void ResolveID(Object *object) const;

	void Save(rapidjson::Value &value, jsonutils::Allocator &allocator) const;
	bool Load(const rapidjson::Value &value);
private:
	void AddLoadObject(Object *object);
	void DeleteDeps(const void *object);
	bool SelectDeps(const void *object);

	Texture *GetTextureByID(const wxString &id);
	Body *GetBodyByID(const wxString &id);
	bool IsFreeID(const wxString &id, const Object *sel = nullptr) const;
	wxString GetFreeID() const;

	b2Vec2 gravity;
	float textureScale;
	bool allowSleep;

	Texture *textures;
	Object *objects;

	Object *create;

	Directory gameDir;
};
