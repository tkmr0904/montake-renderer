#ifndef CAMERA_H
#define CAMERA_H

#include "hit.h"

#define RAY_MAX 100000//視線, 光線サブパスのそれぞれの個数の最大数

class Camera
{
    public:
    virtual Ray getRay(const int& i, const int& j, Subpath& subpath, double& pdflast) const = 0;//仮想空間上のrayを得る
    virtual Vec3 rechangeposition(Vec3 const& v)const = 0;
    virtual Vec3 rechangedirection(Vec3 const& v)const = 0;
    virtual Vec3 sensor_2_camera(Vec2 const& ij) const = 0;
    virtual int getnum_subpath() const = 0;
};


class PinholeCamera : public Camera
{

    public:
    const int width;//画像の横の画素数
    const int height;//画像の縦の画素数

    Vec3 camPos;//仮想空間(ワールド座標)上のカメラのセンサーの0,0座標の位置

    //最初はカメラから見たとき   前がz軸(ワールド座標)の正の方向, 右がx軸(ワールド座標)の正の方向, 下がy軸(ワールド座標)の正の方向 であるとする
    const Vec3 camForward0 = Vec3(0, 0, 1); //ワールド座標で表す
    const Vec3 camRight0   = Vec3(1, 0, 0); //ワールド座標
    const Vec3 camDown0    = Vec3(0, 1, 0); //ワールド座標
    //          ||
    //          ||
    //          ||             回転行列で動かしてから使う
    //         \  /
    //          \/
    //カメラの前,右,下の方向
    Vec3 camForward;
    Vec3 camRight;
    Vec3 camDown;

    Matrix3 rechangemat;//カメラ座標の方向ベクトルに左からこれをかけるとワールド座標の方向ベクトルになる

    double alpha;            //
    double theta;            //  カメラを回転させる角度 
    double phi;              //


    const double holedistance;//ピンホールとセンサーの距離
    const double holearea;//ピンホールの面積(微小と見なす   この面積に立体角が比例するとみなせるくらいに微小であると見なす)
    const double sensorsensitivity;//センサーに入った放射束の何倍が色の明るさとなるかの値
    double sensorarea;//センサーの1画素の面積
    const int num_subpath = 1;//視線サブパスを構築するときにカメラ内部にできるサブパスの点の個数
    /*ピンホールカメラではセンサー上に1個のみ*/

    PinholeCamera(int const& _width, int const& _height, Vec3 const& camPos, double const& alpha, double const& theta, double const& phi, double const& holedistance, double const& holearea, double const& sensorsensitivity);

    Ray getRay(const int& i, const int& j, Subpath& subpath, double& pdflast) const;
    Vec3 rechangeposition(Vec3 const& v)const;
    Vec3 rechangedirection(Vec3 const& v)const;
    Vec3 sensor_2_camera(Vec2 const& ij) const;
    int getnum_subpath() const;
};


class ThinLensCamera : public Camera
{
    public:
    const int width;//画像の横の画素数
    const int height;//画像の縦の画素数

    Vec3 camPos;//仮想空間上のカメラのセンサーの0,0座標の位置

    //最初はカメラから見たとき   前がz軸(ワールド座標)の正の方向, 右がx軸(ワールド座標)の正の方向, 下がy軸(ワールド座標)の正の方向 であるとする
    const Vec3 camForward0 = Vec3(0, 0, 1); //ワールド座標で表す
    const Vec3 camRight0   = Vec3(1, 0, 0); //ワールド座標
    const Vec3 camDown0    = Vec3(0, 1, 0); //ワールド座標
    //          ||
    //          ||
    //          ||             回転行列で動かしてから使う
    //         \  /
    //          \/
    Vec3 camForward;
    Vec3 camRight;
    Vec3 camDown;

    Matrix3 rechangemat;//カメラ座標の方向ベクトルに左からこれをかけるとワールド座標の方向ベクトルになる

    double alpha;            //
    double theta;            //  カメラを回転させる角度 
    double phi;              //

    double sensorarea;//センサーの1画素の面積
    const double sensorsensitivity;//センサーに入った放射束の何倍が色の明るさとなるかの値
    double lensradius;//レンズの半径                                           
    double lensdistance;//センサーとレンズの距離                                          
    double focallength;//レンズの焦点距離
    const int num_subpath = 2;//視線サブパスを構築するときにカメラ内部にできるサブパスの点の個数  
    /*薄レンズカメラではセンサー上に1個, レンズ上に1個  合計2個*/   /*薄レンズなのでレンズ内部で1回光が曲がると考えても良いため、レンズ上に1個しか点を取らないことにする*/


    ThinLensCamera(int const& width, int const& height, Vec3 const& camPos, double const& alpha, double const& theta, double const& phi, double const& lensradius, double const& lensdistance, double const& focallength, double const& sensorsensitivity);


    Ray getRay(const int& i, const int& j, Subpath& subpath, double& pdflast) const;
    Vec3 rechangeposition(Vec3 const& v)const;
    Vec3 rechangedirection(Vec3 const& v)const;
    Vec3 sensor_2_camera(Vec2 const& ij) const;//センサー座標からカメラ座標に直す
    int getnum_subpath() const;
};

#endif