#ifndef SPHEREH
#define SPHEREH

#include "Hitable.h"

class Sphere : public hitable
{
	public:
		Sphere() {}
		Sphere(vec3 cent, float r, Material *material) : center(cent), radius(r), mat(material) {};
		virtual bool hit(const Ray &r, float tmin, float tmax, hit_record &rec) const;

		vec3 center;
		float radius;
		Material *mat;
};

bool Sphere::hit(const Ray &r, float tmin, float tmax, hit_record &rec) const
{
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - a * c;

	if (discriminant > 0)
	{
		float temp = (-b - sqrt(discriminant)) / a;
		if (temp < tmax && temp > tmin)
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat;
			return true;
		}

		temp = (-b + sqrt(discriminant)) / a;
		if (temp < tmax && temp > tmin)
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat;
			return true;
		}
	}

	return false;
}

#endif // !SPHEREH

