//ray.h
#ifndef _RAY_H_
#define _RAY_H_

#include <float.h>
#include "vector3D.h"
#include "settings.h"
#include "color.h"

class Object;

// const float SMALLEST_DIST = 1e-4; //Constant used to dismiss intersections very close to previous
class Ray
{
private:
	const Vector3D origin;
	Vector3D direction;
	float t; //Distance travelled alogn the Ray
	bool hit; //has the ray hit something?
	int object_type; // -1 if nothing hit else stores whether we hit a light or a non light source
	Color intensity = Color(0); // intensity of intersected light source
	const Object *object;//The object that has been hit
	int level;//Number of times the ray has been traced recursively
	float refractive_index;
	Vector3D normal; //Normal of the hit object

public:  
	Ray(const Vector3D& o, const Vector3D& d, int _level = 0, float _ref_idx = 1.0):
    		origin(o), direction(d), t(FLT_MAX), hit(false), level(_level), refractive_index(_ref_idx), object_type(-1)
	{
		direction.normalize();	
	}
	Vector3D getOrigin() const  {return origin;}
	Vector3D getDirection() const  {return direction;}
	Vector3D getPosition() const {return origin + t*direction;}
	Vector3D getNormal() const {return normal;}
	int getObjectType() const {return object_type;}
	void setObjectType(bool isLight);
	int getLevel() {return level;}
	void setLevel(int newLevel){level = newLevel;}
	float getRefractiveIndex() const {return refractive_index;}
	void setRefractiveIndex(float eta){refractive_index = eta;}
	float getParameter() const {return t;}
	bool setParameter(const float par, const Vector3D& norm, const Object *obj);
	bool didHit() const {return hit;}
	const Object* intersected() const {return object;}
	int getLevel() const {return level;}
	void setIntensity(Color internsity) {intensity = intensity;}
	Color getIntensity() const {return intensity;}

};
#endif
