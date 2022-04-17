#pragma once
#include "Colors.hpp"
#include "JSONUtils.hpp"
#include "Mouse.hpp"
#include "Vec2Property.hpp"
#include "FilterProperty.hpp"
#include <wx/propgrid/propgrid.h>

class Level;

class Object{
public:
	Object();
	virtual ~Object() {}

	enum{
		LEVEL,
		BODY,
		JOINT,
		IMAGE,
		LIGHT,
		FIXTURE
	};

	inline void Select(){selected=-1;}
	inline void Unselect(){selected=0;}
	inline bool IsSelected() const{return selected!=0;}
	inline bool IsSelectedPoint() const{return selected>0;}
	inline void UnselectPoints(){if(IsSelectedPoint())Select();}

	virtual void Draw(const Colors &colors) const = 0;
	virtual void DrawPreview() const;
	virtual void DrawPoints(const Colors &colors) const = 0;
	virtual bool UpdatePoints(const Mouse &mouse) = 0;
	virtual bool Create(const Mouse &mouse);
	virtual bool CancelCreating(); //if false, object isn't valid
	virtual bool TryRemove(const void *object); //true = success
	virtual uint8_t GetObjectType() const = 0;
	virtual int8_t GetLayer() const;
	
	virtual void UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const;
	virtual void OnPropertyGridChange(const wxString &name, const wxVariant &value);

	void ToJSON(rapidjson::Value &value, jsonutils::Allocator &allocator) const;
	bool FromJSON(const rapidjson::Value &value);

	const wxString &GetID() const;
protected:
	void DrawPoint(const Colors &colors, int index, const b2Vec2 &pos) const;
	bool UpdatePoint(const Mouse &mouse, int index, b2Vec2 &pos, bool grid=true);

	int selected;

	static constexpr float activeSize=6.0f;
	static constexpr float inactiveSize=4.0f;
	static constexpr float borderAlpha=1.0f;
	static constexpr float fillAlpha=0.4f;

	static b2Vec2 delta;

	wxString id;

	Object *next;

	friend class Level;
};
