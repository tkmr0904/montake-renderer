#ifndef HIT_H
#define HIT_H

#include "texture.h"

extern double const hitdistance0;//様々なcppファイルでconstの数値を用いてそれらを分割コンパイルしたいが,ヘッターに数値の定義を書きたくない...   そのような場合にはextern宣言をヘッダーに書いてcppにインクルードさせ, 1つのcppで数値を定義すれば良い


class Hit  /*衝突点の情報などを記録するために用いられる*/
{
    public:

    std::shared_ptr<M::Material> hitmaterial;  //Materialの情報
    std::shared_ptr<L::Light>    hitlight;     //Lightの情報


    Vec3             hitnormal;         //単位法線ベクトル
    Vec3             hitpos;            //位置
    Vec3             hitLe;             //放射輝度
    double           hitdistance;       //前回に衝突した点と今回衝突した点の距離
    Vec3             raydirection_in;   //今回の衝突点に入射してきたrayの方向 
    double           pdf;               //面積に関する
    double           pdf_reverse;       //面積に関する 逆方向から
    double           pdf_nee;           //面積に関する
    double           cos_in;            //今回の衝突点に入射してきたrayと法線ベクトルのなす角に関するcos
    double           cos_out;           //これから発射するrayと法線ベクトルのなす角に関するcos
    Vec3             BSDF;
    bool             ifhit;             //衝突点(有限の位置にある点)とみなすかどうか
    bool             ifhit_light;       //lightの物体に当たったかどうか
    Vec3             weight;//
    double           roulette;          //これからray発射する確率
    double           roulette_reverse;  //入射、出射が逆であったと仮定した場合のrayが発射する確率

    Hit();

    void set    (bool const& ifhit_,  bool const& ifhit_light_,  double const& hitdistance_);
    void setbool(bool const& ifhit_,  bool const& ifhit_light_                             );
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define abc(a) ((a)*(a)) //バランスヒューリスティックに現れるべき乗

class Subpath
{
    public:

    Hit* hit_eye;
    Hit* hit_light;

    int num_eye;
    int num_light;

    int const num;

    Subpath(int const& num_);
    ~Subpath();
    void addeye(Hit const& hit, int const& num_);
    void addlight(Hit const& hit, int const& num_);
    void reset();
    double misweight(int const& length_eye, int const& length_light, int const& min_eye, int const& min_light, double const& pdf_eye_reverse1, double const& pdf_eye_reverse2, double const& pdf_light_reverse1, double const& pdf_light_reverse2, double const& roulette_reverse_eye, double const& roulette_reverse_light);
   
};

#endif