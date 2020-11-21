#ifndef SKY_H
#define SKY_H


#include "ray.h"


class Sky//rayの向きから色を求める
{
    public:

    virtual Vec3 getradience(const Ray& ray) const = 0;

};


class UniformSky : public Sky//常に同じ色
{
    public:
    Vec3 color;

    UniformSky(Vec3 const& _color);

    Vec3 getradience(const Ray& ray) const;
};



class SimpleSky : public Sky//rayのz成分が1に近づくほど指定した色に近く, -1に近づくほど白色に近づく
{
    public:
    Vec3 color;
    SimpleSky(Vec3 const& _color);
    Vec3 getradience(const Ray& ray) const;
};



class IBL : public Sky//天球画像から色を取得する
{
    public:
    int width;
    int height;
    float* hdr_image;

    IBL(const std::string& filename);

    ~IBL();

    Vec3 getradience(const Ray& ray) const;
};

#endif