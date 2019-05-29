#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "Material.h"

class Lambertian : public Material
{
public:
	Lambertian(const vec3 &a) : albedo(a) {}

	virtual bool scatter(const Ray & r_in, const hit_record & record, vec3 & attenuation, Ray & scattered) const
	{
		vec3 target = record.p + record.normal + random_in_unit_sphere();
		scattered = Ray(record.p, target - record.p);
		attenuation = albedo;
		return true;
	}

	vec3 albedo;
};

#endif // !LAMBERTIAN_H




