#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#define threadsum 4

#include "camera.h"
#include "sky.h"
#include "shapes.h"


void hour_mimnute_second(double  const& seconds,    int      & hour    ,    int   & minute,     double& second );
std::stringstream _0fill(int     const& i      ,    int const& digitnum                                        );
void getfirstray_light  (Ray          & ray    ,    Hit      & hit     ,    double& pdflast                    );
void getnextray_eye     (Ray          & ray    ,    Hit      & hit     ,    Hit   & prehit,     double& pdflast);
void getnextray_light   (Ray          & ray    ,    Hit      & hit     ,    Hit   & prehit,     double& pdflast);
void getposition_nee    (Objects const& objects,    Hit      & hit);

class Image
{
    public:
    const int width;          //画像の横幅
    const int height;         //画像の縦幅

    /*******************サンプル取得前に設定される数値***************************************************/
        int   num_of_samples;       //これから取るサンプルの数は画像何枚分に相当するか(サンプルを取る前に設定する)
        long* num_of_samples_each;  //1つ1つの画素のサンプルを取る数                                  (num_of_samplesが設定された後で決まる)
        long  num_of_samples_pixels;//サンプルを取る個数の総和(画素数)(大きすぎる数値が入るのでlong)     (num_of_samplesが設定された後で決まる)
    /**********************************************************************************************/
    
    /********************サンプリング中に書き変わるもの******************************************************************************/

        /*****************サンプリングが何%終わっているかを表示するために計算するのに使われる***********************************/
        long samplecounter[threadsum];//それぞれのスレッドでサンプルを何個取り終わったか    //並列処理で同時にsamplecounterに加算処理が行われたら適切に加算されるとは思えないので分けておく
        /************************************************************************************************************/

        Vec3* data;         //各々の画素のサンプルの総和        サンプルが取られるたびに加算される
        Vec3* data2;        //各々の画素のサンプルの2乗の総和    サンプルが取られるたびに加算される
        long* addednum_each;//1つ1つの画素のサンプルを取り終わった数

        double seconds;//サンプル取得の秒数を数える サンプルを取り終わったら初期化される(更にサンプルを取り続ける場合は0から数え直し)

    /***************************************************************************************************************************/


    /*********************サブパスの構築に関するもの********************************/
    const double p_eye;     //視線rayを延長する確率
    const double p_light;   //光線rayを延長する確率
    const int    p_eye_1_num;  //視線サブパスの何個目までのロシアンルーレットを1にするかの個数
    const int    p_light_1_num;//光線サブパスの何個目までのロシアンルーレットを1にするかの個数

    const bool ifnee;   //neeをするかどうか
    const bool ifeye;   //視線サブパスを構築するか
    const bool iflight; //光線サブパスを構築するか
    /******************************************************************************/

    /*************サンプリングの後に書き換わるもの*******************************************************/
    double sample_sum;//画像何枚分に相当するサンプルを取り終わっているか(実数)(これは初期化されず, 加算され続ける)
    /**********************************************************************************************/
    
    /************画像の計算時に使われるもの*****************************************/
    Vec3* data_temp;//data, data2の値を用いて計算したものを, ここに一時的に保存しておく
    /**************************************************************************/

    Image(int const& _width, int const& _height, double const& p_eye, double const& p_light, int const& p_eye_1_num, int const& p_light_1_num, bool const& ifnee, bool const& ifeye, bool const& iflight);
    ~Image();

    void GettingReady             ();
    void getSample                (int                  const& i     ,  int     const& j      ,  Camera const& camera,  Objects const& objects     ,  Sky const& sky,  Subpath& subpath,  int const& threadnumber);
    void addSample                (Camera               const& camera,  Objects const& objects,  Sky    const& sky   ,  int     const& threadnumber);
    void CalculateColor           (double               const& a_                    ) const;
    void Output_To_Ppm            (int                  const& i                     ) const;
    void Output_Over255_To_Ppm    (int                  const& i                     ) const;
    void CalculateSD_average      (double               const& a_                    ) const;
    void CalculateSD_each1        (double               const& a_                    ) const;
    void OutputSD_To_Ppm          (int                  const& i                     ) const;
    void writeSD_to_ppm           (std::string const& name  , int const& num) const;
    void setNumber_of_samples_each(bool                 const& abc                   );
    double getpercent             ()                                                   const;
    void printpercent             ()                                                   const;
    void timecounter              ();
    double getroulette_eye        (int const& k)                                       const;
    double getroulette_light      (int const& k)                                       const;
};

#endif