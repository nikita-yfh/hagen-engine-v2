#pragma once
#include "Object.hpp"
#include "Rotatable.hpp"
#include "Texture.hpp"
#include "Body.hpp"

class Level;

class Image : public Object, public Rotatable{
public:
	Image(const Texture *texture, const float &scale);

	virtual void Draw(const Colors &colors) const override;
	virtual void DrawPoints(const Colors &colors) const override;
	virtual bool UpdatePoints(const Mouse &mouse) override;
	virtual bool Create(const Mouse &mouse) override;
	virtual uint8_t GetType() const override{return IMAGE;}

	virtual bool TryRemove(const void *object);

	virtual void UpdatePropertyGrid(wxPropertyGrid *pg, bool n) const override;
	virtual void OnPropertyGridChange(const wxString &name, const wxVariant &value) override;
private:
	void Transform() const;
	bool TestPoint(const b2Vec2 &point) const;

	const Texture *texture;
	const Body *bindBody;

	const float &scale;

	friend class Level;
};



