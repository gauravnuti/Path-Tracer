#ifndef _POINTLIGHTSOURCE_H_
#define _POINTLIGHTSOURCE_H_

#include "lightsource.h"

class PointLightSource : public LightSource
{
private:
	Vector3D position;
public:
	PointLightSource(World *_world, const Vector3D& _pos, const Color& _intensity):
		LightSource(_world, _intensity), position(_pos) {}
	Vector3D getPosition() const {return position;}
	Vector3D getNormal() const { return position;}
	float getcoso(Vector3D ray) const { return 1;}
};
#endif
