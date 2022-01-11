#pragma once
#include "Colors.hpp"
#include "Mouse.hpp"
#include "Vec2Property.hpp"
#include <wx/propgrid/propgrid.h>

class Object{
public:
	Object()
		: selected(-1) {id[0]=0;}

	inline void Select(){selected=-1;}
	inline void Unselect(){selected=0;}
	inline bool IsSelected() const{return selected!=0;}
	inline bool IsSelectedPoint() const{return selected>0;}
	inline void UnselectPoints(){if(IsSelectedPoint())Select();}

	virtual bool Create(const Mouse &mouse);
	virtual bool CancelCreating(); //if false, object isn't valid
	
	virtual void UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const;
	virtual void OnPropertyGridChange(const wxString &name, const wxVariant &value);
protected:
	void DrawPoint(const Colors &colors, int index, const b2Vec2 &pos) const;
	bool UpdatePoint(const Mouse &mouse, int index, b2Vec2 &pos, bool grid=true);
	
	int selected;

	static const float activeSize;
	static const float inactiveSize;

	static const float borderAlpha;
	static const float fillAlpha;

	wxString id;
};
