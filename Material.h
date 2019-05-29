#ifndef MATERIALH
#define MATERIALH

#include "Hitable.h"

vec3 random_in_unit_sphere()
{
	vec3 p;
	do
	{
		p = 2.0f * vec3(rand() / (RAND_MAX + 1.0), rand() / (RAND_MAX + 1.0), rand() / (RAND_MAX + 1.0)) - vec3(1.0f, 1.0f, 1.0f);
	} while (p.squared_length() >= 1.0f);
	return p;
}

bool Refract(const vec3 &v, const vec3 &n, float ni_over_nt, vec3 &refracted)
{
	vec3 uv = unit_vector(v);
	float dt = dot(uv, n);
	float discriminant = 1.0f - ni_over_nt * ni_over_nt * (1.0f - dt * dt);

	if (discriminant > 0.0f)
	{
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
		return true;
	}
	else
	{
		return false;
	}
}

vec3 Reflect(const vec3 & v, const vec3 & n)
{
	return v - 2 * dot(v, n) * n;
}

float schlick(float cosine, float ref_idx)
{
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

class Material
{
public:
	virtual bool scatter(const Ray &rayIn, const hit_record &rec, vec3 &attenuation, Ray &scattered) const = 0;
};

#endif // !MATERIALH
