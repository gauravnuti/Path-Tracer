#ifndef _PLANELIGHTSOURCE_H_
#define _PLANELIGHTSOURCE_H_

#include "lightsource.h"
#include "triangle.h"
#include "color.h"

class PlaneLightSource : public LightSource
{
private:
	Vector3D p1;
	Vector3D p2;
	Vector3D p3;
	Vector3D p4 = p3 + p1 - p2;
	Triangle top = Triangle(p1,p2,p3,NULL);
	Triangle bottom = Triangle(p1,p3,p4,NULL);

public:
	PlaneLightSource(World *_world, const Vector3D& _p1, const Vector3D& _p2, const Vector3D& _p3, const Color& _intensity):
		LightSource(_world, _intensity), p1(_p1), p2(_p2), p3(_p3) 
		{
			isSolid = true;
		}
	Vector3D getPosition() const {return  p1 + (p2-p1)*((float)rand()/RAND_MAX) + (p3-p2)*((float)rand()/RAND_MAX);}
	bool intersect(Ray& r) const {
		bool intersection = top.intersect(r) || bottom.intersect(r);
		if (intersection){
			Color emittance = getIntensity();
			r.setIntensity(emittance);
		}
		return intersection;
	} 
	Vector3D getNormal() const { return crossProduct(p2-p1,p3-p1);}
	float getcoso(Vector3D ray) const { return (getNormal()).length();}
	// float getcoso(Vector3D ray) const { return dotProduct(ray,getNormal());}
};
#endif