#ifndef HITABLEH
#define HITABLEH

#include "Ray.h"

class Material;

struct hit_record
{
	float t;
	vec3 p;
	vec3 normal;
	Material *mat_ptr;
};

class hitable
{
public:
	virtual bool hit(const Ray &r, float t_min, float t_max, hit_record &rec) const = 0;
};

#endif // !

