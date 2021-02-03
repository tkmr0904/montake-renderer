#ifndef ENUM_H
#define ENUM_H


typedef enum class enm1//Shapeと数字を1対1に対応付ける
{
   SPHERE, PIPE, TRIANGLE, DISK, CUBOID, NUM_OF_SHAPES, ANYTHING
} SHAPES;

//staticをつけてリンク時の重複定義を回避
static std::string const SHAPESNAME[(int)SHAPES::NUM_OF_SHAPES] = {"Sphere", "Pipe", "Triangle", "Disk", "Cuboid"};//順番はSHAPESと同じにしなければならない

typedef enum class enm2//Materialと数字を1対1に対応付ける
{
    DIFFUSE, MIRROR, GLASS, VACUUM
} MATERIAL;

typedef enum class enm3//真理値の0と1に対応させる
{
    NOTVACUUM, VACUUM
} ISVACUUM;

typedef enum class enm4//真理値の0と1に対応させる
{
    NOTSPECULAR, SPECULAR
} ISSPECULAR;

typedef enum class enm5//真理値の0と1に対応させる
{
    NOTTRANSPARENT, TRANSPARENT
} ISTRANSPARENT;

#endif