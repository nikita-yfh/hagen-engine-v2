#pragma once
#include "Object.hpp"
#include "Rotatable.hpp"
#include "Texture.hpp"
#include "Body.hpp"

class Image : public Object, public Rotatable{
public:
	Image(const Texture *texture, const Body *body, const float &scale);

	virtual void Draw(const Colors &colors) const override;
	virtual void DrawPoints(const Colors &colors) const override;
	virtual bool UpdatePoints(const Mouse &mouse) override;
	virtual bool Create(const Mouse &mouse) override;
	virtual uint8_t GetObjectType() const override{return IMAGE;}
	virtual int8_t GetLayer() const override;

	virtual bool TryRemove(const void *object);

	virtual void UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const override;
	virtual void OnPropertyGridChange(const wxString &name, const wxVariant &value) override;

	void Save(rapidjson::Value &value, jsonutils::Allocator &allocator) const;
	bool Load(const rapidjson::Value &value);

	static bool enabled;
private:
	int8_t layer;
	b2Vec2 repeat;

	Mouse GetBodyMouse(const Mouse &mouse) const;
	void Transform() const;
	bool TestPoint(const b2Vec2 &point) const;

	const Texture *texture;
	const Body *bindBody;

	const float &scale;
};



