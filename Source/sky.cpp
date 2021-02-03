#include "../include/sky.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"//こうすることで定義の重複を避けることができる

/***********UniformSkyの実装****************************************/

    UniformSky::UniformSky(Vec3 const& _color):color(_color)
    {}

    Vec3 UniformSky::getradience(const Ray& ray) const
    {
        return color;
    }

/******************************************************************/

/***********SimpleSkyの実装****************************************/
    SimpleSky::SimpleSky(Vec3 const& _color):color(_color)
    {}

    Vec3 SimpleSky::getradience(const Ray& ray) const
    {

        double t = (ray.direction.z + 1.0)/2.0;




        return (1-t)*Vec3(255,255,255) + t * color;
    }

/******************************************************************/

/***********IBLの実装***********************************************/

    IBL::IBL(const std::string& filename)
    {
        int n;
        hdr_image = stbi_loadf(filename.c_str(), &width, &height, &n, 0);
    }

    IBL::~IBL()
    {
        stbi_image_free(hdr_image);
    }

    Vec3 IBL::getradience(const Ray& ray) const
    {
        double theta = gettheta(ray.direction);
        double phi   =   getphi(ray.direction);

        int i = phi  *   (width/(2*M_PI));
        int j = theta * (height/M_PI);

        int index = 3*(width*j + i);

        return Vec3(hdr_image[index], hdr_image[index+1], hdr_image[index+2]);
    }
/******************************************************************/