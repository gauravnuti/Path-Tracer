#ifndef _LIGHTSOURCE_H_
#define _LIGHTSOURCE_H_

#include "world.h"
#include "color.h"
#include "vector3D.h"

class LightSource
{	
protected:
	World *world;
	Color intensity;
	bool isSolid = false;
public:
	LightSource(World *_world, const Color _intensity):
		world(_world), intensity(_intensity)   {}
	virtual Vector3D getPosition() const = 0;
	virtual bool intersect(Ray& ray) const = 0;
	Color getIntensity() const {return intensity;}
	bool getSolidity() const {return isSolid;}
	virtual Vector3D getNormal() const = 0;
	virtual float getcoso(Vector3D ray) const = 0;
};
#endif
