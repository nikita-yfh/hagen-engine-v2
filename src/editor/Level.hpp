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
	~Level();

	virtual void Draw(const Colors &colors) const override;
	virtual void DrawPoints(const Colors &colors) const override;
	virtual bool UpdatePoints(const Mouse &mouse) override;
	virtual uint8_t GetType() const override{return LEVEL;}

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

	void AddImage(const wxString &path);
	void AddBody(Body *body);
	void AddFixture(Fixture *fixture);
	void AddJoint(Joint *joint);
	
	bool IsCreating() const;
	bool CancelCreating();

	void Clear();

	void AddTexture(const wxString&);
	void DeleteTexture(const wxString&);

	void UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const;
	void OnPropertyGridChange(const wxString &name, const wxVariant &value);

	const Directory &GetGameDir() const;
private:
	void AddObject(Object *object);
	void DeleteObject(const void *object);

	b2Vec2 gravity;
	float textureScale;
	bool allowSleep;

	Texture *textures;
	Object *objects;

	Object *create;

	Directory gameDir;
};
