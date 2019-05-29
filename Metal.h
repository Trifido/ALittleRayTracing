#ifndef METAL_H
#define METAL_H

#include "Material.h"

class Metal : public Material
{
	public:
		Metal(const vec3 & a, float f) : albedo(a) 
		{
			if (f < 1.0f)
				fuzz = f;
			else
				fuzz = 1.0f;
		}

		virtual bool scatter(const Ray & r_in, const hit_record & record, vec3 & attenuation, Ray & scattered) const
		{
			vec3 reflected = Reflect(unit_vector(r_in.direction()), record.normal);
			scattered = Ray(record.p, reflected + fuzz*random_in_unit_sphere());
			attenuation = albedo;
			return (dot(scattered.direction(), record.normal) > 0);
		}

		vec3 albedo;
		float fuzz;
};

#endif // !METAL_H

