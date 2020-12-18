#include "ray.h"

bool Ray::setParameter(const float par, const  Vector3D& norm, const Object *obj)
{
	if(par < t && par > EPSILON)
	{
		hit = true;
		t = par;
		object = obj;
		normal = norm;
		normal.normalize();
		return true;
	}
	return false;
}

void Ray::setObjectType(bool isLight){
	if(isLight){
		object_type=LIGHT_SOURCE;
	} 
	else{
		object_type=NON_LIGHT;
	}
}

