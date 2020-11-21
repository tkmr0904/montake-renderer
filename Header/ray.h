#ifndef RAY_H
#define RAY_H

#include "vec.h"
    

class Ray
{
    public:
    Vec3 origin;//視点
    Vec3 direction;//向き
    Ray ();
    Ray(Vec3 const& _origin, Vec3 const& _direction);
    Vec3 localdirection(double const& rands, double const& randf);
};

Ray rotateray_x(const Ray& ray, const double angle_x);
Ray rotateray_y(const Ray& ray, const double theta);
Ray rotateray_z(const Ray& ray, const double phi);
Ray rotateray  (const Ray& ray, const Matrix3& rotatemat);


bool operator==     (Ray& a, Ray& b);
bool operator!=     (Ray& a, Ray& b);
Ray mirrorreflectray(Vec3& predirection, Vec3& normal, Vec3& hitpos);


#endif

