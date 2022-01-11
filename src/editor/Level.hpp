#pragma once
#include "Body.hpp"
#include "Joint.hpp"
#include "Mouse.hpp"
#include "Colors.hpp"
#include <wx/gdicmn.h>

class Level : public Object{
public:
	Level();
	~Level();

	void Draw(const Colors &colors) const;

	void DrawPoints(const Colors &colors) const;
	bool UpdatePoints(const Mouse &mouse);

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

	void AddBody(Body *body);
	void AddFixture(Fixture *fixture);
	void AddJoint(Joint *joint);
	
	bool IsCreating() const;
	bool CancelCreating();

	void Clear();

	void UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const;
	void OnPropertyGridChange(const wxString &name, const wxVariant &value);
private:
	Body *GetFirstSelectedBody();
	Body *GetSecondSelectedBody();

	void DeleteSelectedBodies();
	void DeleteSelectedJoints();

	b2Vec2 gravity;
	bool allowSleep;

	Body *bodies;
	Joint *joints;

	Object *create;
};
