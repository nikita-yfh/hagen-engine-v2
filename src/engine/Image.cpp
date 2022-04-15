#include "Image.hpp"

static void Transform(const b2Vec2 &position, float angle) {
	glTranslatef(position.x, position.y, 0.0f);
	glRotatef(angle/M_PI*180.0f, 0.0f, 0.0f, 1.0f);
}

void Image::Render(float scale) const {
	if(bindBody)
		Transform(bindBody->GetPosition(), bindBody->GetAngle());
	Transform(position, angle);

	glScalef(texture->GetWidth()*repeat.x/2.0f/scale,
			texture->GetHeight()*repeat.y/2.0f/scale, 1.0f);

	texture->Activate();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f,		0.0f);		glVertex2i(-1, -1);
	glTexCoord2f(0.0f,		repeat.y);	glVertex2i(-1,  1);
	glTexCoord2f(repeat.x,	repeat.y);	glVertex2i( 1,  1);
	glTexCoord2f(repeat.x,	0.0f);		glVertex2f( 1, -1);
	glEnd();
	texture->Deactivate();
}


