#ifndef DIELECTRIC_H
#define DIELECTRIC_H

#include "Material.h"



class Dielectric : public Material
{
	public:
		Dielectric(float ri) : ref_idx(ri) {}
		virtual bool scatter(const Ray &rayIn, const hit_record &rec, vec3 &attenuation, Ray &scattered) const
		{
			vec3 outward_normal;
			vec3 reflected = Reflect(rayIn.direction(), rec.normal);

			float ni_over_nt;
			attenuation = vec3(1.0f, 1.0f, 1.0f);
			vec3 refracted;
			float reflect_prob;
			float cosine;

			if (dot(rayIn.direction(), rec.normal) > 0)
			{
				outward_normal = -rec.normal;
				ni_over_nt = ref_idx;
				cosine = ref_idx * dot(rayIn.direction(), rec.normal) / rayIn.direction().length();
			}
			else
			{
				outward_normal = rec.normal;
				ni_over_nt = 1.0f / ref_idx;
				cosine = -dot(rayIn.direction(), outward_normal) / rayIn.direction().length();
			}

			if (Refract(rayIn.direction(), outward_normal, ni_over_nt, refracted))
			{
				reflect_prob = schlick(cosine, ref_idx);
			}
			else
			{
				//scattered = Ray(rec.p, reflected);
				reflect_prob = 1.0f;
			}

			if ((rand() / (RAND_MAX + 1.0)) < reflect_prob)
			{
				scattered = Ray(rec.p, reflected);
			}
			else
			{
				scattered = Ray(rec.p, refracted);
			}
			return true;
		}

		float ref_idx;
};

#endif // !DIELECTRIC_H
