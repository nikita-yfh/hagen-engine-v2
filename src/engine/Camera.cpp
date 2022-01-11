#include "Camera.hpp"

Camera::Camera(){
	position=b2Vec2(0.0f,0.0f);
	lowerBounds=b2Vec2(0.0f,0.0f);
	upperBounds=b2Vec2(0.0f,0.0f);
}
void Camera::SetBounds(const b2Vec2 &lower, const b2Vec2 &upper){
	lowerBounds=lower;
	upperBounds=upper;
}
void Camera::Apply() const{

}
