//物体表面の発光と光の反射などに関する
#ifndef TEXTURES_H
#define TEXTURES_H

#include "ray.h"
#include "enum.h"

/**************************ppmファイル(画像)の読み込みに関する処理***********************************************************************/
    int  nexttoken (std::ifstream     & file, unsigned char c[], int arraynum);
    void loadppm_P6(std::ifstream     & file, int   & imagewidth, int& imageheight, int& maxbrightness,  std::shared_ptr<Vec3[]>& color);
    void loadppm   (std::string   const str , int& mode, int& imagewidth, int& imageheight, int& maxbrightness,  std::shared_ptr<Vec3[]>& color);
/******************************************************************************************************************************/

//MaterialとLightでPatternの名前が衝突してしまうのでnamespaceで分ける

namespace M     //MaterialのM
{
    /**************物体上の表面のある点での光の反射のしかたを設定する*****************************/
        class Material //物体上の表面のある点での光の反射のしかたを設定する
        {
            public:
            
            virtual Vec3   sample_eye   (Vec3      & wi,  Vec3 const& wo,  double& pdf)   const = 0;
            virtual Vec3   sample_light (Vec3 const& wi,  Vec3      & wo,  double& pdf)   const = 0;
            virtual Vec3   getBSDF      (Vec3 const& wi,  Vec3 const& wo              )   const = 0;
            virtual double getpdf_eye   (Vec3        wi,  Vec3 const& wo              )   const = 0;
            virtual double getpdf_light (Vec3 const& wi,  Vec3        wo              )   const = 0;
            virtual bool   isSpecular   ()                                                const = 0;
            virtual bool   isVacuum     ()                                                const = 0;
            virtual bool   isTransparent()                                                const = 0;
            virtual void   name         ()                                                const = 0;
            virtual int    Materialnum  ()                                                const = 0;  
        };

        class Diffuse : public Material  //光が反射するときの放射輝度が一様になるように設定する
        {
            public:

            Vec3 rate;

            Diffuse(const Vec3 _rate);
            Diffuse();
            Vec3   sample_eye   (Vec3      & wi,  Vec3 const& wo,  double& pdf)   const;
            Vec3   sample_light (Vec3 const& wi,  Vec3      & wo,  double& pdf)   const;
            Vec3   getBSDF      (Vec3 const& wi,  Vec3 const& wo              )   const;
            double getpdf_eye   (Vec3        wi,  Vec3 const& wo              )   const;
            double getpdf_light (Vec3 const& wi,  Vec3        wo              )   const;
            bool   isSpecular   ()                                                const;
            bool   isVacuum     ()                                                const;
            bool   isTransparent()                                                const;
            void   name         ()                                                const;
            int    Materialnum  ()                                                const;
        };

        class Mirror : public Material   //入射角と反射角が等しくなるように設定する
        {
            public:
            Vec3 rate;

            Mirror(const Vec3 _rate);
            Mirror();
            Vec3   sample_eye   (Vec3      & wi,  const Vec3& wo,  double& pdf)       const;
            Vec3   sample_light (Vec3 const& wi,  Vec3      & wo,  double& pdf)       const;
            Vec3   getBSDF      (Vec3 const& wi,  Vec3 const& wo              )       const;
            double getpdf_eye   (Vec3        wi,  Vec3 const& wo              )       const;
            double getpdf_light (Vec3 const& wi,  Vec3        wo              )       const;
            bool   isSpecular   ()                                                    const;
            bool   isVacuum     ()                                                    const;
            bool   isTransparent()                                                    const;  
            void   name         ()                                                    const;
            int    Materialnum  ()                                                    const;
        };

        /***************Glassで使われる関数たち*************************************************************/
            double fresnel(const Vec3& wi, const Vec3& n, const double& ni, const double& no);
            bool refract(const Vec3& in, Vec3& out, Vec3 const& n, const double& no, const double& ni);
        /***********************************************************************************************/

        class Glass : public Material    //無色透明で表面が滑らかな場合の光の反射を設定する
        {
            public:
            double n_inside;
            double n_outside;
            double p_reflect;

            Glass(double n_inside, double n_outside, double p_reflect);
            Glass();

            Vec3   sample_eye   (Vec3      & wi,  Vec3 const& wo,  double& pdf)   const;
            Vec3   sample_light (Vec3 const& wi,  Vec3      & wo,  double& pdf)   const;
            Vec3   getBSDF      (Vec3 const& wi,  Vec3 const& wo              )   const;
            double getpdf_eye   (Vec3        wi,  Vec3 const& wo              )   const;
            double getpdf_light (Vec3 const& wi,  Vec3        wo              )   const;
            bool   isSpecular   ()                                                const;
            bool   isVacuum     ()                                                const;
            bool   isTransparent()                                                const;
            void   name         ()                                                const;
            int    Materialnum  ()                                                const;
        };

        class Vacuum : public Material   //光が反射ぜず直進するように設定する 何もないかのように振る舞う
        {
            public:

            Vacuum();


            Vec3   sample_eye   (Vec3      & wi,  Vec3 const& wo,  double& pdf)   const;
            Vec3   sample_light (Vec3 const& wi,  Vec3      & wo,  double& pdf)   const;
            Vec3   getBSDF      (Vec3 const& wi,  Vec3 const& wo              )   const;
            double getpdf_eye   (Vec3        wi,  Vec3 const& wo              )   const;
            double getpdf_light (Vec3 const& wi,  Vec3        wo              )   const;
            bool   isSpecular   ()                                                const;
            bool   isVacuum     ()                                                const;
            bool   isTransparent()                                                const;
            void   name         ()                                                const;
            int    Materialnum  ()                                                const;
        };
    /*************************************************************************************/

    /**************Materialが物体表面上の位置に関してどのように分布するかを設定する****************************************************************/
        class Pattern//Materialの物体表面上の位置に関する分布を設定する
        {
            public:
            
            virtual std::shared_ptr<Material> getmaterial(double const & theta, double const & phi)const = 0;  //物体表面上の位置に関する情報を受け取ってMaterialを返す
            virtual int patternnum()const = 0;
        };

        /*************適当に作ってみた**************************************************/
            class Plain :public Pattern //物体の表面のMaterialを位置に依存しなくする
            {
                public:

                std::shared_ptr<Material> material;
                Plain(std::shared_ptr<Material> material_);
                std::shared_ptr<Material> getmaterial(double const & x, double const & y)const;//materialに格納したMaterialを衝突点に依存せず, 同じMaterialを渡す
                int patternnum()const;
            };
    
            class Colorful_sphere :public Pattern //sphereでしか使ってはいけない
            {
                public:

                Colorful_sphere();

                std::shared_ptr<Material> getmaterial(double const & theta, double const & phi)const;
                int patternnum()const;

            };

            class Colorful_pipe :public Pattern //pipeでしか使ってはいけない
            {
                public:

                Colorful_pipe();

                std::shared_ptr<Material> getmaterial(double const & phi, double const & z)const;
                int patternnum()const;
            };

            class Colorful_triangle :public Pattern //triangleでしか使ってはいけない
            {
                public:

                Colorful_triangle();

                std::shared_ptr<Material> getmaterial(double const & xp1, double const & xp2)const;
                int patternnum()const;
            };

            class Colorful_disk :public Pattern  //diskでしか使ってはいけない
            {
                public:

                Colorful_disk();

                std::shared_ptr<Material> getmaterial(double const & r, double const & phi)const;
                int patternnum()const;
            };

            class Colorful_cuboid :public Pattern  //cuboidでしか使ってはいけない
            {
                public:

                Colorful_cuboid();

                std::shared_ptr<Material> getmaterial(double const & x, double const & y)const;
                int patternnum()const;
            };

            class DiskHole_cuboid: public Pattern  //cuboidでしか使ってはいけない
            {
                public:
                double holeradius;

                DiskHole_cuboid(double holeradius_);   
                std::shared_ptr<Material> getmaterial(double const & x, double const & y)const;

                int patternnum()const;

            };

            /*************物体表面に画像(.ppm)を貼り付ける**************************************/
                class Picture_sphere :public Pattern    //sphereでしか使ってはいけない
                {
                    public:

                    const std::string str;//colorに読み込まれた画像のファイル名
                    int mode;
                    int imagewidth;
                    int imageheight;
                    int maxbrightness;
                    std::shared_ptr<Vec3[]> color;

                    Picture_sphere(const std::string str);

                    std::shared_ptr<Material> getmaterial(double const & theta, double const & phi)const;
                    int patternnum()const;
                };

                class Picture_pipe:public Pattern  //pipeでしか使ってはいけない
                {
                    public:

                    const std::string str;
                    int mode;
                    int imagewidth;
                    int imageheight;
                    int maxbrightness;
                    std::shared_ptr<Vec3[]> color;

                    Picture_pipe(const std::string str);

                    std::shared_ptr<Material> getmaterial(double const & phi, double const & z)const;
                    int patternnum()const;
                };

                class Picture_disk:public Pattern  //diskでしか使ってはいけない
                {
                    public:

                    const std::string str;
                    int mode;
                    int imagewidth;
                    int imageheight;
                    int maxbrightness;
                    std::shared_ptr<Vec3[]> color;

                    Picture_disk(const std::string str);

                    std::shared_ptr<Material> getmaterial(double const & r, double const & phi)const;
                    int patternnum ()const;
                };

                class Picture_rectangle:public Pattern  //Cuboidでしか使ってはいけない
                {
                    public:

                    const std::string str;
                    int mode;
                    int imagewidth;
                    int imageheight;
                    int maxbrightness;
                    std::shared_ptr<Vec3[]> color;

                    Picture_rectangle(const std::string str);

                    std::shared_ptr<Material> getmaterial(double const& x, double const& y)const;
                    int patternnum ()const;
                };


            /*******************************************************************************/

        /*****************************************************************************/

        /**********Patternをshareポインタに変える関数たち***************************************************************************************/
            /************テンプレート関数の定義はヘッダーに書くべし(コンパイル時に完全な関数が作られていなければならないため, 後でリンクすることができない)****/
                template<typename a> std::shared_ptr<Pattern> pPlain(a material)//物体表面の光の反射を位置に依存しなくする       material(Materialの継承クラスのオブジェクト)をメンバとするPlain(M::Patternの派生クラス)のオブジェクトのshareポインタを返す
                //ポインタがshared_ptrなら関数内でそれらを宣言しても構わない(もし生ポインタなら関数の外に出ると参照先が解放されてしまうのでセグメント違反が起こる)
                {
                    return std::make_shared<Plain> (Plain( std::make_shared<a>   (material)   ));
                }
            /**************************************************************************************************************************/

            std::shared_ptr<Pattern> pColorful_sphere();
            std::shared_ptr<Pattern> pColorful_pipe();
            std::shared_ptr<Pattern> pColorful_triangle();
            std::shared_ptr<Pattern> pColorful_disk();
            std::shared_ptr<Pattern> pColorful_cuboid();
            std::shared_ptr<Pattern> pPicture_sphere(std::string str);
            std::shared_ptr<Pattern> pDiskHole_cuboid(double holeradius);
            std::shared_ptr<Pattern> pPicture_rectangle(std::string str);
        /***********************************************************************************************************************************/

        /**************実際に物体を作成するときに書きやすくする****************************************/
            typedef std::shared_ptr<Pattern>      sharePattern;//Patternクラスををshareポインタにしたもの
        /***************************************************************************************/

    /*************************************************************************************************************************************/
}

namespace L     //LightのL
{
    /************物体上の表面のある点での発光のしかたを設定する**************************/
        class Light//物体上の表面のある点での発光のしかたを設定する
        {
            public:

            virtual Vec3 Le(Vec3 const&) const = 0;//発光の方向(引数)に放射される光の放射輝度を返す
            virtual void sample_light(Vec3& wo, double& pdflast) const = 0;//光線rayが光源から発射される方向をサンプリングする
            virtual double getpdf_firstray(Vec3 const& wo) const = 0;//光線rayが光源から発射される方向に関する確率密度
            //sample_lightとgetpdf_firstrayのつじつまを合わせなければならない
        };

        class Uniform : public Light//発光の放射輝度が発光する方向に依存しないように設定
        {
            public:

            Vec3 color;//どんな色で発光しているのかをRGBで設定する
            Uniform(Vec3 const& _color);
            Vec3 Le(Vec3 const&)const;
            void sample_light(Vec3& wo, double& pdflast) const;
            double getpdf_firstray(Vec3 const& wo) const;
        };
    /****************************************************************************/

    /*******************Lightが物体表面上の位置に関してどのように分布するかを設定する********************************************************************/
        class Pattern //Lightの物体表面上の位置に関する分布を設定する
        {
            public:

            virtual std::shared_ptr<Light> getlight(double const&, double const&) const = 0;
            virtual int patternnum(void)const = 0;
        };

        /********適当に作ってみた***********************************************/
            class Plain : public Pattern//物体の表面のLightを位置に依存しなくする
            {
                public:
                std::shared_ptr<Light> light;

                Plain(std::shared_ptr<Light> light_);

                std::shared_ptr<Light> getlight(double const& x, double const& y) const;
                int patternnum() const;
            };

            class pattern_disk : public Pattern
            {
                public:

                pattern_disk();

                std::shared_ptr<Light> getlight(double const& r, double const& phi) const;
                int patternnum() const;
            };
        /*********************************************************************/

        /**************Patternをshareポインタに変える関数たち*************************************************************************************/
            /************テンプレート関数の定義はヘッダーに書くべし(コンパイル時に完全な関数が作られていなければならないため, 後でリンクすることができない)****/
                template<typename a> std::shared_ptr<Pattern> pPlain(a light)//物体表面の発光を位置に依存しなくする     light(Lightの継承クラスのオブジェクト)をメンバとするPlain(L::Patternの派生クラス)のオブジェクトのshareポインタを返す
                //ポインタがshared_ptrなら関数内でそれらを宣言しても構わない(もし生ポインタなら関数の外に出ると参照先が解放されてしまうのでセグメント違反が起こる)
                {
                    return std::make_shared<Plain>(   Plain(  std::make_shared<a>(light)  )   );
                }
            /*****************************************************************************************************************************/

            std::shared_ptr<Pattern> ppattern_disk();
        /*************************************************************************************************************************************/

        /**************実際に物体を作成するときに書きやすくする****************************************/
            typedef std::shared_ptr<L::Pattern> sharePattern;//Patternクラスををshareポインタにしたもの
        /***************************************************************************************/
    /*******************************************************************************************************************************************/
}

#endif