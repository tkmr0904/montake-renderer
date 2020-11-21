
#include "../Header/vec.h"


/***********Vec3の実装**************************************************/


    Vec3::Vec3() 
    {
        x = y = z = 0;
    }
    Vec3::Vec3(double const & _x, double const & _y, double const & _z) 
    {
        x = _x;
        y = _y;
        z = _z;
    }

    Vec3::Vec3(Vec3 const& v)  //なぜかこれがないと正しく計算できない
    {
        x = v.x;
        y = v.y;
        z = v.z;
    }


    double Vec3::length() const 
    {
        return sqrt(x*x + y*y + z*z);
    };
    double Vec3::length2() const 
    {
        return x*x + y*y + z*z;
    };


    void Vec3::_0to255(int& counter, int&counter_neg, int& counter_inf, int& counter_nan)
    {


        /*if (x < 0) {x = 0;}
        if (y < 0) {y = 0;}
        if (z < 0) {z = 0;}

        if(isnan(x)) {x = 0;}
        if(isnan(y)) {y = 0;}
        if(isnan(z)) {z = 0;}*/

        /*if(isinf(x)) {x = DBL_MAX;}
        if(isinf(y)) {y = DBL_MAX;}
        if(isinf(z)) {z = DBL_MAX;}*/

        if(isnan(x)||isnan(y)||isnan(z))
        {
            x = 0;
            y = 0;
            z = 0;
            counter_nan++;
        }

        if(isinf(x)||isinf(y)||isinf(z))
        {
            x = 255;
            y = 255;
            z = 255;
            counter_inf++;
        }
        
        if((x<0)||(y<0)||(z<0))
        {
            x = 0;
            y = 0;
            z = 0;
            counter_neg++;
        }



        double k = std::max({x,y,z});

        if(k>255)
        {
            counter++;
            x = x/k;
            y = y/k;
            z = z/k;

            x = x*255;
            y = y*255;
            z = z*255;            
        }


        /*if (x < 0) {x = 0;}
        if (y < 0) {y = 0;}
        if (z < 0) {z = 0;}

        if(x != x) {x = 0;}
        if(y != y) {y = 0;}
        if(z != z) {z = 0;}*/


        /*if (x > 255) {x = 255; y = 0; z = 0;}
        if (y > 255) {x = 255; y = 0; z = 0;}
        if (z > 255) {x = 255; y = 0; z = 0;}
        if (x < 0) {x = 0; y = 255; z = 0;}
        if (y < 0) {x = 0; y = 255; z = 0;}
        if (z < 0) {x = 0; y = 255; z = 0;}


        if(x != x) {x = 0; y = 0; z = 255; }
        if(y != y) {x = 0; y = 0; z = 255; }
        if(z != z) {x = 0; y = 0; z = 255; }*/


        /*if (x > 255) {x = 255;}
        if (y > 255) {y = 255;}
        if (z > 255) {z = 255;}
        if (x < 0) {x = 0;}
        if (y < 0) {y = 0;}
        if (z < 0) {z = 0;}


        if(x != x) {x = 0; y = 0; z = 255; }
        if(y != y) {x = 0; y = 0; z = 255; }
        if(z != z) {x = 0; y = 0; z = 255; }*/


        

    }

/**********************************************************************/


Vec3 operator+(const Vec3& v1, const Vec3& v2) 
{
    return Vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

void operator+=(Vec3& v1, const Vec3& v2) 
{
    v1 = v1 + v2;
}


Vec3 operator-(const Vec3& v1, const Vec3& v2) 
{
    return Vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}


void operator-=(Vec3& v1, const Vec3& v2) 
{
    v1 = v1 - v2;
}



Vec3 operator*(const double& k, const Vec3& v) 
{
    return Vec3(k * v.x, k * v.y, k * v.z);
}


Vec3 operator*(const Vec3& v, const double& k) 
{
    return k*v;
}

void operator*=(Vec3& v, double const & k)
{
    v = v*k;
}


Vec3 operator*(const Vec3& v1, const Vec3& v2)
{
    return Vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

void operator*=(Vec3& v1, const Vec3& v2)
{
    v1 = v1*v2;
}


Vec3 operator/(Vec3 const & v1, Vec3 const & v2)
{
    return Vec3(v1.x/v2.x, v1.y/v2.y, v1.z/v2.z);
}

Vec3 operator/(double const & k, Vec3 const & v) 
{
    return Vec3(k / v.x, k / v.y, k / v.z);
}
Vec3 operator/(Vec3 const & v, double const & k) 
{
    return Vec3(v.x / k, v.y / k, v.z / k);
}

void operator/=(Vec3& v, double const & k)
{
    v = Vec3(v.x / k, v.y / k, v.z / k);
}

std::ostream& operator<<(std::ostream& stream, Vec3 const & v) 
{
    stream << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return stream;
}

bool operator==(Vec3 const & a, Vec3 const & b)
{
    if((a.x == b.x)&&(a.y == b.y)&&(a.z == b.z))
    return 1;
    else 
    return 0;
}

bool operator!=(Vec3 const & a, Vec3 const & b)
{
    return !(a == b);
}

bool operator<(Vec3 const & a, Vec3 const & b)
{
    return (a.x<b.x)&&(a.y<b.y)&&(a.z<b.z);
}

bool operator>(Vec3 const & a, Vec3 const & b)
{
    return (a.x>b.x)&&(a.y>b.y)&&(a.z>b.z);
}

double dot(const Vec3& v1, const Vec3& v2) 
{
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}
Vec3 cross(const Vec3& v1, const Vec3& v2) 
{
    return Vec3(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}

Vec3 normalize(const Vec3& v) 
{
    return v/v.length();
}

Vec3 vabs(Vec3 const & v)
{
    return Vec3(std::abs(v.x), std::abs(v.y), std::abs(v.z));
}

Vec3 _x0(Vec3 const & v)
{
    return Vec3(0, v.y, v.z);
}

Vec3 _y0(Vec3 const & v)
{
    return Vec3(v.x, 0, v.z);
}

Vec3 _z0(Vec3 const & v)
{
    return Vec3(v.x, v.y, 0);
}

Vec3 _z1(Vec3 const& v)//z成分が1になるように定数倍する　このようにすることで直線の傾き(空間バージョン)を得やすくできる
{
    return v/v.z;
}

Vec3 mirrorreflect(Vec3 const & predirection, Vec3 const & normal)//反射方向を得る
{
    Vec3 direction = normalize(predirection - 2.0*normal*dot(predirection,normal));//反射方向を得る

    return direction;
}

void orthonormalbasis(const Vec3& n, Vec3& s, Vec3& t)
{
    while(1)
    {
        s = Vec3(rnd(), rnd(), rnd());
        if(s.length2() > 0.001)
        {
            s = normalize(s);

            if(dot(n,s)*dot(n,s)<0.9)
            {
                s = s - dot(n,s)*n;//平行な成分を除く
                s = normalize(s);
                break;
            }
            else
            continue;
        }
        else
        continue;
    }
    
    t = cross(n,s);
}

Vec3 sqrt(Vec3 const & v)
{
    return Vec3(sqrt(v.x), sqrt(v.y), sqrt(v.z));
}



//-------------------------------------------------------------------------------------------------------------------

/**************Vec2の実装************************************/ 
    Vec2::Vec2() 
    {
        x = y = 0;
    }

    Vec2::Vec2(double const & _x, double const & _y) 
    {
        x = _x;
        y = _y;
    }


    double Vec2::length() const 
    {
        return sqrt(x*x + y*y);
    };
    double Vec2::length2() const 
    {
        return x*x + y*y;
    };
/***********************************************************/


Vec2 operator+(const Vec2& v1, const Vec2& v2) 
{
    return Vec2(v1.x + v2.x, v1.y + v2.y);
}

void operator+=(Vec2& v1, const Vec2& v2) 
{
    v1 = v1 + v2;
}

Vec2 operator-(const Vec2& v1, const Vec2& v2) 
{
    return Vec2(v1.x - v2.x, v1.y - v2.y);
}

Vec2 operator*(const double& k, const Vec2& v) 
{
    return Vec2(k * v.x, k * v.y);
}


Vec2 operator*(const Vec2& v, const double& k) 
{
    return k*v;
}

void operator*=(Vec2& v, double const & k)
{
    v = v*k;
}


Vec2 operator*(const Vec2& v1, const Vec2& v2)
{
    return Vec2(v1.x * v2.x, v1.y * v2.y);
}

void operator*=(Vec2& v1, const Vec2& v2)
{
    v1 = v1*v2;
}




Vec2 operator/(double const & k, Vec2 const & v) 
{
    return Vec2(k / v.x, k / v.y);
}
Vec2 operator/(Vec2 const & v, double const& k) 
{
    return Vec2(v.x / k, v.y / k);
}

void operator/=(Vec2& v, double const & k)
{
    v = Vec2(v.x / k, v.y / k);
}

std::ostream& operator<<(std::ostream& stream, const Vec2& v) 
{
    stream << "(" << v.x << ", " << v.y <<  ")";
    return stream;
}

bool operator==(Vec2 const& a, Vec2 const& b)
{
    if((a.x == b.x)&&(a.y == b.y))
    return 1;
    else 
    return 0;
}

bool operator!=(Vec2 const& a, Vec2 const & b)
{
    return !(a == b);
}

double dot(const Vec2& v1, const Vec2& v2) 
{
    return v1.x*v2.x + v1.y*v2.y;
}


Vec2 normalize(const Vec2& v) 
{
    return v/v.length();
}

Vec2 vabs(Vec2 const& v)
{
    return Vec2(std::abs(v.x), std::abs(v.y));
}


Vec2 z0(Vec3 const& v)
{
    return Vec2(v.x, v.y);
}

Vec3 Vec2_to_Vec3(Vec2 const& v)
{
    return Vec3(v.x, v.y, 0);
}


int sgn(double const& x)//正なら1  負なら0  0なら0  nanなら終了
{
    if(x>0)
    return 1;

    if(x<0)
    return -1;

    if(x==0)
    return 0;

    
    //ここまで達したらnan
    std::cout << "sgnでNAN発生" << std::endl; 
    exit(1);
    
    
}

////////////////////////////////////////////////////////////////////////////////////////






//////////////////////////////////////////////////////////////////////////////

/************Matrix2の実装******************************************************/

    Matrix2::Matrix2(){}

    Matrix2::Matrix2(Vec2 const& v1, Vec2 const& v2)//縦ベクトルを2個代入
    {
        m2[0] = v1;
        m2[1] = v2;
    }

    Matrix2::Matrix2(double const& _1_1, double const& _1_2,  
                     double const& _2_1, double const& _2_2 )//成分を代入
    {
        m2[0] = Vec2(_1_1, _2_1); 
        m2[1] = Vec2(_1_2, _2_2);
    }
/*****************************************************************************/


Matrix2 transpose(Matrix2 const& m)//転置
{
        


        return Matrix2( m.m2[0].x, m.m2[0].y,
                       m.m2[1].x, m.m2[1].y  );
}


Matrix2 operator*(Matrix2 const& a, double const& k)
{
    return Matrix2(a.m2[0]*k, a.m2[1]*k);
}


Matrix2 operator*(double const& k, Matrix2 const& a)
{
    return a*k;
}

Matrix2 operator/(Matrix2 const& a, double const& k)
{
    return a*(1/k);
}




Matrix2 operator*(Matrix2 const& a, Matrix2 const& b)//22行列同士の積
{
        

    return Matrix2(  dot((transpose(a).m2[0]),(b.m2[0])),    dot((transpose(a).m2[0]),(b.m2[1]))   ,                           
                    dot((transpose(a).m2[1]),(b.m2[0])),    dot((transpose(a).m2[1]),(b.m2[1]))    );
}

Vec2 operator*(Matrix2 const& a, Vec2 const& b)//22行列と縦ベクトルの積
{
    return Vec2(dot((transpose(a).m2[0]),b),   dot((transpose(a).m2[1]),b)  );
}

std::ostream& operator<<(std::ostream& stream, Matrix2 const& Matrix2)
{
    stream << transpose(Matrix2).m2[0] << std::endl << transpose(Matrix2).m2[1] ;
    return stream;
}


Vec2 rotate(Vec2 const& X, double const& angle)//回転
{
    Matrix2 m = Matrix2(  cos(angle), -1*sin(angle) ,
                          sin(angle),    cos(angle) );
    return m * X;
}


double determinant(Matrix2 const& M)
{
    return M.m2[0].x * M.m2[1].y - M.m2[1].x * M.m2[0].y;
}


Matrix2 inverse2(Matrix2 const& M)
{
    return Matrix2(   M.m2[1].y, -1*M.m2[1].x,  
                   -1*M.m2[0].y,    M.m2[0].x )/determinant(M);
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------




/************Matrix3の実装********************************************************/
    Matrix3::Matrix3(){}

    Matrix3::Matrix3(Vec3 const& v1, Vec3 const& v2, Vec3 const& v3)//縦ベクトルを3個代入
    {
        m3[0] = v1;
        m3[1] = v2;
        m3[2] = v3;
    }

    Matrix3::Matrix3(double const& _1_1, double const& _1_2, double const& _1_3, 
                     double const& _2_1, double const& _2_2, double const& _2_3, 
                     double const& _3_1, double const& _3_2, double const& _3_3 )//成分を代入
    {
        m3[0] = Vec3(_1_1, _2_1, _3_1); 
        m3[1] = Vec3(_1_2, _2_2, _3_2);
        m3[2] = Vec3(_1_3, _2_3, _3_3);
    }
    
    
/*******************************************************************************/

Matrix3 transpose(const Matrix3& m)//転置
{
        


        return Matrix3( m.m3[0].x, m.m3[0].y, m.m3[0].z ,
                        m.m3[1].x, m.m3[1].y, m.m3[1].z ,
                        m.m3[2].x, m.m3[2].y, m.m3[2].z   );
}




Matrix3 operator*(const Matrix3 a, const Matrix3 b)//33行列同士の積
{
        

    return Matrix3(  dot((transpose(a).m3[0]),(b.m3[0])),    dot((transpose(a).m3[0]),(b.m3[1]))  ,    dot((transpose(a).m3[0]),(b.m3[2]))    ,                           
                    dot((transpose(a).m3[1]),(b.m3[0])),    dot((transpose(a).m3[1]),(b.m3[1]))  ,    dot((transpose(a).m3[1]),(b.m3[2]))    ,
                    dot((transpose(a).m3[2]),(b.m3[0])),    dot((transpose(a).m3[2]),(b.m3[1]))  ,    dot((transpose(a).m3[2]),(b.m3[2]))      );
}

Vec3 operator*(const Matrix3& a, const Vec3& b)//33行列と縦ベクトルの積
{
    return Vec3(dot((transpose(a).m3[0]),b),   dot((transpose(a).m3[1]),b),   dot((transpose(a).m3[2]),b)  );
}

std::ostream& operator<<(std::ostream& stream, Matrix3 const& Matrix3)
{
    stream << transpose(Matrix3).m3[0] << std::endl << transpose(Matrix3).m3[1] << std::endl << transpose(Matrix3).m3[2];
    return stream;
}

Matrix3 getmatx(double const& angle)
{
    return Matrix3(  1,          0,             0,
            0, cos(angle), -1*sin(angle),
            0, sin(angle),    cos(angle)   );
}

Matrix3 getmaty(double const& angle)
{
    return Matrix3(  cos(angle), 0,   sin(angle),
                                 0, 1,            0,
                        -1*sin(angle), 0, cos(angle)   );
}

Matrix3 getmatz(double const& angle)
{
    return Matrix3(  cos(angle), -1*sin(angle),   0    ,
                    sin(angle),    cos(angle),   0    ,
                     0,             0,   1     );
}



Vec3 rotatex(Vec3 const& X, double const& angle)//x軸を軸に回転
{
    Matrix3 m = Matrix3(  1,          0,             0,
                        0, cos(angle), -1*sin(angle),
                        0, sin(angle),    cos(angle)   );
    return m * X;
}

Vec3 rotatey(Vec3 const& X, double const& angle)//y軸を軸に回転
{
    Matrix3 m = Matrix3(  cos(angle), 0,   sin(angle),
                                 0, 1,            0,
                        -1*sin(angle), 0, cos(angle)   );
    return m * X;
}


Vec3 rotatez(Vec3 const& X, double const& angle)//z軸を軸に回転
{
    Matrix3 m = Matrix3(  cos(angle), -1*sin(angle),   0    ,
                        sin(angle),    cos(angle),   0    ,
                                 0,             0,   1     );
    return m * X;
}





Matrix3 rotatexM(Matrix3 const& M, double const& angle)//x軸を軸に(3つの縦ベクトルを一気に)回転
{
    Matrix3 m = Matrix3(  1,          0,             0,
                        0, cos(angle), -1*sin(angle),
                        0, sin(angle),    cos(angle)   );

    return m * M;
}

Matrix3 rotateyM(Matrix3 const& M, double const& angle)//y軸を軸に(3つの縦ベクトルを一気に)回転
{
    Matrix3 m = Matrix3(  cos(angle), 0,   sin(angle),
                                 0, 1,            0,
                        -1*sin(angle), 0, cos(angle)   );
    return m * M;
}


Matrix3 rotatezM(Matrix3 const& M, double const& angle)//z軸を軸に(3つの縦ベクトルを一気に)回転
{
    Matrix3 m = Matrix3(  cos(angle), -1*sin(angle),   0    ,
                        sin(angle),    cos(angle),   0    ,
                                 0,             0,   1     );
    return m * M;
}

double triangle_inverse(double const& cos, double const& sin)//sin cosの値から角度を求める
{
    if(sin>=0)
    return acos(cos);
    else
    return 2*M_PI - acos(cos);
}



double costheta(Vec3 const& direction)//3次元極座標
{
    if(direction.length() != 0)
    {
        Vec3 norm = normalize(direction);
        return norm.z;
    }
    else
    {
        return 0;//長さが0だから方向なんてどうでもいい
    }
}


double abscostheta(const Vec3& direction)
{
    return std::abs(costheta(direction));
}

double sintheta(const Vec3& direction)//3次元極座標
{

    if(direction.length() != 0)
    {
        Vec3 norm = normalize(direction);
        Vec3 z0 = _z0(norm);
        return z0.length();
    }
    else
    {
        return 0;//長さが0だから方向なんてどうでもいい
    }
    

    
}

double cosphi(const Vec3& direction)//3次元極座標
{
    Vec3 z0 = _z0(direction);
    if(z0.length() != 0)
    {
        z0 = normalize(z0);
        return z0.x;
    }
    else if(direction.length2() != 0)
    {
        return 1;//sin*sin+cos*cos=1が成立すれば値は何でもいい
    }
    else
    {
        return 0;//長さが0だから方向なんてどうでもいい
    }
    
    
    
    
}

double sinphi(const Vec3& direction)//3次元極座標
{
    Vec3 z0 = _z0(direction);
    if(z0.length() != 0)
    {
        z0 = normalize(z0);
        return z0.y;
    }
    else if(direction.length2() != 0)
    {
        return 0;//sin*sin+cos*cos=1が成立すれば値は何でもいい
    }
    else
    {
        return 0;//長さが0だから方向なんてどうでもいい
    }
}

double gettheta(Vec3 const& direction)//3次元極座標
{
    return triangle_inverse(costheta(direction), sintheta(direction));
}

double getphi(Vec3 const& direction)//3次元極座標
{
    return triangle_inverse(cosphi(direction), sinphi(direction));
}




Vec3 world_to_local(const Vec3& v, const Vec3& s, const Vec3& t, const Vec3& n)
{
    return Vec3(dot(v, s), dot(v, t), dot(v, n));
}

Vec3 local_to_world(const Vec3& v, const Vec3& s, const Vec3& t, const Vec3& n)
{

    Matrix3 m = Matrix3(s,t,n);


    return m*v;
}


 Vec3 angle_to_direction(double const& theta, double const& phi)
{
    Vec3 forward = Vec3(0,0,1);

    forward = rotatey(forward, theta);
    forward = rotatez(forward, phi);

    return forward;

}