#ifndef VEC_H
#define VEC_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <algorithm>
#include <stdio.h>
#include <memory>
#include <vector>
#include <string>
#include <unistd.h>
#include "rand.h"
using namespace std::literals::string_literals;
#define _d_ exp10(-10)


class Vec3
{
    public:

    double a[3];//データはここに格納する

    /*******分かりやすくするためにxyzで表す***/
    double& x = a[0];
    double& y = a[1];
    double& z = a[2];
    /************************************/

    Vec3();
    Vec3(double const & _x, double const & _y, double const & _z);
    Vec3(Vec3 const& v);
    Vec3 operator=(Vec3 const& v)
    {
        x = v.x;
        y = v.y;
        z = v.z;

        return *this;
    }

    double length() const;
    double length2() const;

    Vec3 operator -() const
    {
        return Vec3(-x, -y, -z);
    }
    
    void _0to255(int& counter, int&counter_neg, int& counter_inf, int& counter_nan);

};


Vec3 operator+(const Vec3& v1, const Vec3& v2);
void operator+=(Vec3& v1, const Vec3& v2);
Vec3 operator-(const Vec3& v1, const Vec3& v2);
void operator-=(Vec3& v1, const Vec3& v2);
Vec3 operator*(const double& k, const Vec3& v);
Vec3 operator*(const Vec3& v, const double& k);
void operator*=(Vec3& v, double const & k);
Vec3 operator*(const Vec3& v1, const Vec3& v2);
void operator*=(Vec3& v1, const Vec3& v2);
Vec3 operator/(Vec3 const & v1, Vec3 const & v2);
Vec3 operator/(double const & k, Vec3 const & v);
Vec3 operator/(Vec3 const & v, double const & k);
void operator/=(Vec3& v, double const & k);
std::ostream& operator<<(std::ostream& stream, Vec3 const & v);
bool operator==(Vec3 const & a, Vec3 const & b);
bool operator!=(Vec3 const & a, Vec3 const & b);
bool operator<(Vec3 const & a, Vec3 const & b);
bool operator>(Vec3 const & a, Vec3 const & b);
double dot(const Vec3& v1, const Vec3& v2);
Vec3 cross(const Vec3& v1, const Vec3& v2) ;
Vec3 normalize(const Vec3& v) ;
Vec3 vabs(Vec3 const & v);
Vec3 _x0(Vec3 const & v);
Vec3 _y0(Vec3 const & v);
Vec3 _z0(Vec3 const & v);
Vec3 _z1(Vec3 const& v);
Vec3 mirrorreflect(Vec3 const & predirection, Vec3 const & normal);
void orthonormalbasis(const Vec3& n, Vec3& s, Vec3& t);
Vec3 sqrt(Vec3 const & v);

//////////////////////////////////////////////////////////////////////////////////


class Vec2 
{
    public:
    double x;
    double y;

    Vec2();
 
    Vec2(double const & _x, double const & _y);



    double length() const;
 
    double length2() const;
};


Vec2 operator+(const Vec2& v1, const Vec2& v2) ;
void operator+=(Vec2& v1, const Vec2& v2) ;
Vec2 operator-(const Vec2& v1, const Vec2& v2); 
Vec2 operator*(const double& k, const Vec2& v) ;
Vec2 operator*(const Vec2& v, const double& k) ;
void operator*=(Vec2& v, double const & k);
Vec2 operator*(const Vec2& v1, const Vec2& v2);
void operator*=(Vec2& v1, const Vec2& v2);
Vec2 operator/(double const & k, Vec2 const & v); 
Vec2 operator/(Vec2 const & v, double const& k) ;
void operator/=(Vec2& v, double const & k);
std::ostream& operator<<(std::ostream& stream, const Vec2& v); 
bool operator==(Vec2 const& a, Vec2 const& b);
bool operator!=(Vec2 const& a, Vec2 const & b);
double dot(const Vec2& v1, const Vec2& v2) ;
Vec2 normalize(const Vec2& v) ;
Vec2 vabs(Vec2 const& v);
Vec2 z0(Vec3 const& v);
Vec3 Vec2_to_Vec3(Vec2 const& v);
int sgn(double const& x);


////////////////////////////////////////////////////////////////////////


class Matrix2
{
    public:
    Vec2 m2[2];//縦ベクトル2個
    Matrix2();

    Matrix2(Vec2 const& v1, Vec2 const& v2);

    Matrix2(double const& _1_1, double const& _1_2,  
           double const& _2_1, double const& _2_2 );
};

Matrix2 transpose(Matrix2 const& m);
Matrix2 operator*(Matrix2 const& a, double const& k);
Matrix2 operator*(double const& k, Matrix2 const& a);
Matrix2 operator/(Matrix2 const& a, double const& k);
Matrix2 operator*(Matrix2 const& a, Matrix2 const& b);
Vec2 operator*(Matrix2 const& a, Vec2 const& b);
std::ostream& operator<<(std::ostream& stream, Matrix2 const& Matrix2);
Vec2 rotate(Vec2 const& X, double const& angle);
double determinant(Matrix2 const& M);
Matrix2 inverse2(Matrix2 const& M);


//////////////////////////////////////////////////////////////////////////////////////////////////




class Matrix3
{
    public:
    Vec3 m3[3];//縦ベクトル3個
    Matrix3();

    Matrix3(Vec3 const& v1, Vec3 const& v2, Vec3 const& v3);

    Matrix3(double const& _1_1, double const& _1_2, double const& _1_3, 
            double const& _2_1, double const& _2_2, double const& _2_3, 
            double const& _3_1, double const& _3_2, double const& _3_3 );
};

Matrix3 transpose(const Matrix3& m);
Matrix3 operator*(const Matrix3 a, const Matrix3 b);
Vec3 operator*(const Matrix3& a, const Vec3& b);
std::ostream& operator<<(std::ostream& stream, Matrix3 const& Matrix3);
Matrix3 getmatx(double const& angle);
Matrix3 getmaty(double const& angle);
Matrix3 getmatz(double const& angle);
Vec3 rotatex(Vec3 const& X, double const& angle);
Vec3 rotatey(Vec3 const& X, double const& angle);
Vec3 rotatez(Vec3 const& X, double const& angle);
Matrix3 rotatexM(Matrix3 const& M, double const& angle);
Matrix3 rotateyM(Matrix3 const& M, double const& angle);
Matrix3 rotatezM(Matrix3 const& M, double const& angle);
double triangle_inverse(double const& cos, double const& sin);
double costheta(Vec3 const& direction);
double abscostheta(const Vec3& direction);
double sintheta(const Vec3& direction);
double cosphi(const Vec3& direction);
double sinphi(const Vec3& direction);
double gettheta(Vec3 const& direction);
double getphi(Vec3 const& direction);
Vec3 world_to_local(const Vec3& v, const Vec3& s, const Vec3& t, const Vec3& n);
Vec3 local_to_world(const Vec3& v, const Vec3& s, const Vec3& t, const Vec3& n);
Vec3 angle_to_direction(double const& theta, double const& phi);


#endif