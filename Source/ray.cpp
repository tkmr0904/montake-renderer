#include "../Header/ray.h"

/***************Rayの実装***************************************************************/
    Ray::Ray () {}
    Ray::Ray(Vec3 const& _origin, Vec3 const& _direction) : origin(_origin)
    {
        #ifdef DEBUG
        if(_direction == Vec3(0,0,0))
        {
            std::cout << "rayのdirectionが0,0,0" << std::endl;
            exit(1);
        }
        #endif
        
        direction = normalize(_direction);
    }

    
    Vec3 Ray::localdirection(double const& rands, double const& randf)//反射方向を得る
    {
        Vec3 vec = Vec3(sin(rands)*cos(randf), sin(rands)*sin(randf), cos(rands));//ランダム反射方向を得る
    
        return vec;
    }
/**************************************************************************************/

Ray rotateray_x(const Ray& ray, const double angle_x)
{ 
    return Ray(rotatex(ray.origin, angle_x), rotatex(ray.direction, angle_x));
}

Ray rotateray_y(const Ray& ray, const double theta)
{ 
    return Ray(rotatey(ray.origin, theta), rotatey(ray.direction, theta));
}

Ray rotateray_z(const Ray& ray, const double phi)
{ 
    return Ray(rotatez(ray.origin, phi), rotatez(ray.direction, phi));
}

Ray rotateray(const Ray& ray, const Matrix3& rotatemat)
{
    return Ray(rotatemat * ray.origin, rotatemat * ray.direction);
}

bool operator==(Ray& a, Ray& b)
{
    if((a.origin == b.origin)&&(a.direction == b.direction))
    return 1;
    else
    return 0;
}

bool operator!=(Ray& a, Ray& b)
{
    return !(a == b);
}

Ray mirrorreflectray(Vec3& predirection, Vec3& normal, Vec3& hitpos)//反射方向を得る
{
    return Ray(hitpos, mirrorreflect(predirection, normal));
}
