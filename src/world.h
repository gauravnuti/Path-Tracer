#ifndef _WORLD_H_
#define _WORLD_H_

#include <vector>
#include "object.h"
#include "lightsource.h"
#include "color.h"
#include "ray.h"
#include "settings.h"

struct RayIntersectionInfo{
	Color emittance = Color(0);
	bool hit=false;
	int object_type=-1;
	float t = FLT_MAX;
};

class World
{
private:
	std::vector<Object*> objectList;
	std::vector<LightSource*> lightSourceList;

	Color ambient;
	Color background; //Background color to shade rays that miss all objects

public:
	World():
		objectList(0), lightSourceList(0), ambient(0), background(0)
	{}
	void setBackground(const Color& bk) { background = bk;}
	Color getbackground() { return background;}
	void setAmbient(const Color& amb) {ambient = amb;}
	Color getAmbient() {return ambient;}
	void addLight(LightSource* ls)
	{
		lightSourceList.push_back(ls);
	}
	std::vector<LightSource*> getLightSources(){
		return lightSourceList;
	};
	void addObject(Object *obj)
	{
		objectList.push_back(obj);
	}
	float firstIntersection(Ray& ray);
	Color lightIntersection(Ray& ray); // intersection with plane light sources
	Color shade_ray(Ray& ray, int flag = 0, const float cur_throughput = 0.0f);
	Color light_ray(const Ray& ray, float &solid_angle);
};
#endif
