#ifndef HITABLELISTH
#define HITABLELISTH

#include "Hitable.h"

class Hitable_list : public hitable
{
	public:
		Hitable_list() {}
		Hitable_list(hitable **l, int n) { list = l, list_size = n; }
		virtual bool hit(const Ray &r, float tmin, float tmax, hit_record &rec) const;
		hitable **list;
		int list_size;
};

bool Hitable_list::hit(const Ray &r, float tmin, float tmax, hit_record &rec) const
{
	hit_record temp_rec;
	bool hit_anything = false;
	double closest_so_far = tmax;

	for (int i = 0; i < list_size; i++)
	{
		if (list[i]->hit(r, tmin, closest_so_far, temp_rec))
		{
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}
	return hit_anything;
}


#endif // !HITABLELISTH

