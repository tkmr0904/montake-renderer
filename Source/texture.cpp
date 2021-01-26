#include"../Header/texture.h"

using namespace std::literals::string_literals;

/**************************ppmファイル(画像)の読み込みに関する処理***********************************************************************/
    int nexttoken(std::ifstream& file, unsigned char c[], int arraynum)//戻り値は文字数 見つからなかった場合は0
    {
        int i;


        while(1)//見つからない限り続ける
        {
            file.read(reinterpret_cast<char*>(c), 1);//読み込む

            if(file.eof() == 1)//ファイル終端の1つ先を読み込んだ
            {
                return 0;//見つからなかった
            }
            else if((c[0] != ' ') && (c[0] != '\n'))//ファイル内のデータを読み込んで文字は改行空白以外の文字だった
            {
                break;//見つかった 
            }
        }


        //見つかった場合
        

        for(i = 1; i < arraynum; i++)//何個先まで格納されているか調べる
        {

            file.read(reinterpret_cast<char*>(c+i), 1);//読み込む
            if((c[i] == ' ') || (c[i] == '\n'))//文字は改行か空白だった
            {
                c[i] = '\0';
                return i;//i個の文字列を発見した
            }
        }
        //配列の全てが改行空白以外の文字で埋まった
        std::cout << "配列が短すぎる" << std::endl;
        exit(1);
    }

    void loadppm_P6(std::ifstream& file, int& imagewidth, int& imageheight, int& maxbrightness,  std::shared_ptr<Vec3[]>& color)
    {
        unsigned char c[20];
        
        for(int j = 0; j<imageheight; j++)
        {
            for(int i = 0; i<imagewidth; i++)
            {
                file.read(reinterpret_cast<char*>(c), 3);


                if(file.eof() == 1)
                {
                    std::cout << "ファイルの終端を過ぎてしまった" << std::endl;
                    exit(1);
                }
                //ファイルの終端でなければ
                color[(long)imagewidth*j + i].x = ((int)c[0])/255.0;
                color[(long)imagewidth*j + i].y = ((int)c[1])/255.0;
                color[(long)imagewidth*j + i].z = ((int)c[2])/255.0;                                                                                
            }
            std::cout << (100 * j / imageheight) << '%' << "\r" << std::flush; //進行状況を表示
        }
        file.close();

        if(file.is_open())
        {
            std::cout << "ファイルを閉じることができなかった"s << std::endl;
            exit(1);
        }

        std::cout << "読み込み終了" << std::endl;
    }

    void loadppm(const std::string str, int& mode, int& imagewidth, int& imageheight, int& maxbrightness,  std::shared_ptr<Vec3[]>& color)
    {
        std::ifstream file(str, std::ios::binary);

        if(!file.is_open())
        {
            std::cout << str + "を開けなかった"s << std::endl;
            exit(1);
        }

        std::cout << str + "の読み込み開始"s << std::endl;

        unsigned char c[20];
        int tokennum;


        file.read(reinterpret_cast<char*>(c), 2);
        mode = c[1] - '0';

        if(c[0] != 'P')
        {
            std::cout << "ファイルが壊れている" << std::endl;
            exit(1);
        }

        if((mode == 1) || (mode == 4))
        {
            std::cout << "このファイルはビットマップ形式なので非対応" << std::endl;
            exit(1);
        }
        else if((mode == 2) || (mode == 5))
        {
            std::cout << "このファイルはモノクロ形式なので非対応" << std::endl;
            exit(1);
        }
        else if((mode == 3) || (mode == 6))
        {
            if(mode == 3)
            {

                std::cout << "このファイルは文字形式(P3)で非対応" << std::endl;
                exit(1);


            }
            if(mode == 6)
            {
                std::cout << "このファイルはP6" << std::endl;
            }
        }
        else
        {
            std::cout << "ファイルが壊れている" << std::endl;
        }


        //画像の横幅情報を得る
        tokennum = nexttoken(file, c, sizeof(c)/sizeof(c[0]));

        if(tokennum > 0)
        {
            imagewidth = (int)strtol((char*)(c), NULL, 10);
            std::cout << "横の画素数は" << imagewidth << std::endl;
        }
        else //エラー発生
        {
            std::cout << "エラー発生" << std::endl;
            exit(1);
        }
        if(imagewidth == 0)
        {
            std::cout << "エラー発生" << std::endl;
            exit(1);
        }
        //imagewidthが無事取得された

        //画像の横幅情報を得る
        tokennum = nexttoken(file, c, sizeof(c)/sizeof(c[0]));

        if(tokennum > 0)
        {
            imageheight = (int)strtol((char*)(c), NULL, 10);
            std::cout << "縦の画素数は" << imageheight << std::endl;
        }
        else //エラー発生
        {
            std::cout << "エラー発生" << std::endl;
            exit(1);
        }
        if(imageheight == 0)
        {
            std::cout << "エラー発生" << std::endl;
            exit(1);
        }
        //imageheightが無事取得された

        


        //画像の最大輝度を得る
        tokennum = nexttoken(file, c, sizeof(c)/sizeof(c[0]));

        if(tokennum > 0)
        {
            maxbrightness = (int)strtol((char*)(c), NULL, 10);
        }
        else //エラー発生
        {
            std::cout << "エラー発生" << std::endl;
            exit(1);
        }
        if(maxbrightness != 255)
        {
            std::cout << "maxbrightnessが255でなく" << maxbrightness << std::endl;
            exit(1);
        }

        std::shared_ptr <Vec3[]> col {new Vec3[imagewidth*imageheight]};//imagewidth, imageheightによって配列の大きさが確定する
        color = std::move(col);                                         //

        if((mode == 1) || (mode == 4))
        {
            exit(1);
        }
        else if((mode == 2) || (mode == 5))
        {
            exit(1);
        }
        else if((mode == 3) || (mode == 6))
        {
            if(mode == 3)
            {
                exit(1);


            }
            if(mode == 6)
            {
                loadppm_P6(file, imagewidth, imageheight, maxbrightness, color);
            }
        }
        else
        {
            std::cout << "ファイルが壊れている" << std::endl;
        }

    }
/******************************************************************************************************************************/

namespace M     //MaterialのM
{

    /*************Diffuseの実装******************************************************/

        Diffuse::Diffuse(const Vec3 _rate):rate(_rate)
        {}

        Diffuse::Diffuse(){}

        Vec3 Diffuse::sample_eye(Vec3& wi, Vec3 const& wo, double& pdf) const//woが入射し, wiが発射する
        {
            double u = rnd();
            double v = rnd();

            double theta = (1/2.0)*acos(1-2*u);         //確率密度関数がcosに比例するようサンプリングにする
            double phi   = 2*M_PI*v;                    //

            double x = sin(theta)*cos(phi);
            double y = sin(theta)*sin(phi);
            double z = cos(theta);

            if(costheta(wo) < 0)//rayが上から入射してくる場合
            {
                wi = Vec3(x,y,z);//上方向に反射させる
            }
            else//rayが下から入射してくる場合
            {
                wi = Vec3(x,y,-z);//下方向に反射させる
            }
            
            
            pdf = cos(theta)/M_PI;//立体角に関する確率密度関数

            return rate/M_PI;//BSDFを返す
        }


        Vec3 Diffuse::sample_light(Vec3 const& wi, Vec3& wo, double& pdf) const//wiが入射し, woが発射する
        {
            double u = rnd();
            double v = rnd();

            double theta = (1/2.0)*acos(1-2*u);         //確率密度関数がcosに比例するようサンプリングにする
            double phi   = 2*M_PI*v;                    //

            double x = sin(theta)*cos(phi);
            double y = sin(theta)*sin(phi);
            double z = cos(theta);

            if(costheta(wi) < 0)//rayが上から入射してくる場合
            {
                wo = Vec3(x,y,z);//上方向に反射させる
            }
            else//rayが下から入射してくる場合
            {
                wo = Vec3(x,y,-z);//下方向に反射させる
            }
            
        

            pdf = cos(theta)/M_PI;//立体角に関する確率密度関数  cosが負になる心配はない

            return rate/M_PI;//BSDFを返す
        }

        Vec3 Diffuse::getBSDF(const Vec3& wi, const Vec3& wo) const
        {
            if((wi.z*wo.z < 0))//rayが衝突面を突き抜けないなら
            {
                return rate/M_PI;//BSDFを返す
            }
            else
            {
                return Vec3(0,0,0);//BSDFは0
            }
        }

        double Diffuse::getpdf_eye(Vec3 wi, Vec3 const& wo) const//視線rayを延長する確率密度(立体角に関する)
        {
            if(wi.z*wo.z < 0)//rayが衝突面を突き抜けないなら
            {
                return abscostheta(wi) / M_PI;
            }
            else//rayが衝突面を突き抜けるなら
            {
                return 0;//このような方向にサンプルを取れることはないのでpdfは0
                //std::cout << "Diffuse面でrayが突き抜けた" << std::endl;
                //exit(1);
            }
        }


        double Diffuse::getpdf_light(Vec3 const& wi, Vec3 wo) const//光線rayを延長する確率密度(立体角に関する)
        {
            if(wi.z*wo.z < 0)//rayが衝突面を突き抜けないなら
            {
                return abscostheta(wo) / M_PI;
            }
            else//rayが衝突面を突き抜けるなら
            {
                return 0;//このような方向にサンプルを取れることはないのでpdfは0
                //std::cout << "Diffuse面でrayが突き抜けた" << std::endl;
                //exit(1);
            }
        }


        bool Diffuse::isSpecular() const//BSDFに少なくとも1つのデルタ関数を持つかどうか
        {
            return (bool)ISSPECULAR::NOTSPECULAR;
        }

        bool Diffuse::isVacuum() const//Materialの種類がVacuumかどうか
        {
            return (bool)ISVACUUM::NOTVACUUM;
        }

        bool Diffuse::isTransparent()const//光が衝突面を透過することがある(裏面のBSDFに0でないものが存在する)のかどうか
        {
            return (bool)ISTRANSPARENT::NOTTRANSPARENT;
        }    


        void Diffuse::name() const
        {
            std::cout << "Diffuse" << std::endl;
        }

        int  Diffuse::Materialnum() const //enumの番号
        {
            return (int)MATERIAL::DIFFUSE;
        }

    /*******************************************************************************/

    /*************Mirrorの実装*******************************************************/

        Mirror::Mirror(const Vec3 _rate):rate(_rate)
        {}

        Mirror::Mirror(){}

        Vec3 Mirror::sample_eye(Vec3& wi, const Vec3& wo, double& pdf) const//woが入射し, wiが発射する
        {
            
            wi = mirrorreflect(wo, Vec3(0,0,1));//法線ベクトルはどちら向きでも良い

            pdf = 1.0;//確率密度関数


            return rate/abscostheta(wi) * Vec3(1,1,1);//BSDFを返す
        }

        Vec3 Mirror::sample_light(Vec3 const& wi, Vec3& wo, double& pdf) const//wiが入射し, woが発射する
        {
            
            wo = mirrorreflect(wi, Vec3(0,0,1));

            pdf = 1.0;//確率密度関数


            return rate/abscostheta(wo) * Vec3(1,1,1);//BSDFを返す
        }



        Vec3 Mirror::getBSDF(const Vec3& wi, const Vec3& wo) const
        {
            return Vec3(0,0,0);
            //実質この関数は使われない(デルタ関数をもつから)
            //サンプルをとるときにデルタ関数の方向にrayを飛ばす確率は0 言い換えればデルタ関数でない方向に必ずrayが飛ぶのでデルタ関数の部分が積分できない
            //スペキュラー面では絶対にneeをしてはいけない
        }

        double Mirror::getpdf_eye(Vec3 wi, Vec3 const& wo) const//視線rayを延長する確率密度(立体角に関する)
        {
            return 1;
        }

        double Mirror::getpdf_light(Vec3 const& wi, Vec3 wo) const//光線rayを延長する確率密度(立体角に関する)
        {
            return 1;
        }



        bool Mirror::isSpecular() const//BSDFに少なくとも1つのデルタ関数を持つかどうか
        {
            return (bool)ISSPECULAR::SPECULAR;
        }

        bool Mirror::isVacuum() const//Materialの種類がVacuumかどうか
        {
            return (bool)ISVACUUM::NOTVACUUM;
        }

        bool Mirror::isTransparent()const//光が衝突面を透過することがある(裏面のBSDFに0でないものが存在する)のかどうか
        {
            return (bool)ISTRANSPARENT::NOTTRANSPARENT;
        }    


        void Mirror::name() const
        {
            std::cout << "Mirror" << std::endl;
        }

        int  Mirror::Materialnum() const //enumの番号
        {
            return (int)MATERIAL::MIRROR;
        }

    /*******************************************************************************/

    /**************************Glassで使う処理****************************************/
        double fresnel(const Vec3& wi, const Vec3& n, const double& ni, const double& no)
        {
            double f0 = std::pow((ni-no)/(ni+no), 2);
            double cos = abscostheta(wi);

            return f0 + (1-f0)*std::pow((1-cos), 5);
        }

        bool refract(const Vec3& in, Vec3& out, Vec3 const& n, const double& no, const double& ni)//nは入射方向との内積が負になる方向になっていなければならない    入射するray(in)側の屈折率をno, 屈折後のray(out)側の屈折率をniとする
        {
            //  光は逆向きにしても同じ経路を進む

            double sin1 = sintheta(in);//in側のsin  //sinthetaは必ず0以上
            double cos1 = sqrt(1-sin1*sin1);
            double sin2 = (no/ni)*sin1;//in側のsin　//必ず0以上
            double cos2 = sqrt(1-sin2*sin2);


            Vec3 h;//法線ベクトルに垂直な方向の単位ベクトルをこれから代入する


            if(sin2>=1)//全反射するなら(臨界角以上なら)
            {
                //outを求める必要がない
                return 0;//屈折しない場合を排除する     //屈折しない
            }

            //屈折する場合は
            if(sin1 == 0)//法線ベクトルとinが平行なら
            {
                out = in;//直進する 
                return 1;//直進だがこれも屈折に含める   //屈折する
            }
            else//法線ベクトルとinが平行でなければ 
            {
                h = (cos1*n + in)/sin1;//法線ベクトルに垂直な方向の単位ベクトルを取り敢えず求めておく

                out = sin2*h - cos2*n;
                return 1;                            //屈折する
            }


            

        }
    /********************************************************************************/

    /************Glassの実装********************************************************/

        Glass::Glass(double n_inside, double n_outside, double p_reflect):n_inside(n_inside), n_outside(n_outside), p_reflect(p_reflect)
        {}

        Glass::Glass(){}
        

        Vec3 Glass::sample_eye(Vec3& wi, const Vec3& wo, double& pdf) const
        {
            Vec3 n;//法線ベクトル
            bool ifrefract;//真理値
            double fr; //フレネル反射率 部分反射する割合
            double rate; //屈折による光の疎密による放射輝度の変化率    (全部屈折すると仮定した場合に屈折後の放射輝度は入射する光の放射輝度の何倍になるのかの値  BSDFとは違うものだということに注意せよ)

            if(costheta(wo) < 0)//上から下向きに入射する場合  rayが物体外部から内部に向かって入射する場合
            {
                n = Vec3(0,0,1);//法線ベクトルは上向き 屈折方向を求めるのに必要
                ifrefract = refract(wo, wi, n, n_outside, n_inside);//屈折するのかどうか確認する
                rate = std::pow((n_outside/n_inside), 2);//実際はwiが入射光でwoが屈折後の光であることを忘れずに
            }
            else//下から上向きに入射する場合  rayが物体内部から外部に向かって入射する場合
            {
                n = Vec3(0,0,-1);//法線ベクトルは下向き 屈折方向を求めるのに必要     
                ifrefract = refract(wo, wi, n, n_inside, n_outside);//屈折するのかどうか確認する
                rate = std::pow((n_inside/n_outside), 2);//実際はwiが入射光でwoが屈折後の光であることを忘れずに
            }

            

            if(ifrefract == 0)//屈折せずに全反射する場合
            { 
                n = Vec3(0,0,1);//法線ベクトルはどちら向きでも良い
                wi = mirrorreflect(wo, n);//鏡面反射方向    法線ベクトルはどちら向きでも計算結果は同じになる             //
                pdf = 1;                 //確率密度関数          デルタ関数が結局BSDFと打ち消し合うのでこれで良い        //Mirrorと同様
                return 1/abscostheta(wi) * Vec3(1,1,1);//BSDFを返す                                             //
            }
            else
            {
                    //wiには屈折方向が代入されている
                fr = fresnel(wi, n, n_inside, n_outside); //フレネル反射率            //両者の屈折率を入れ替えても戻り値は変わらない  法線ベクトルnは代入されるが使われない


                //臨界角未満の場合
                if(rnd() < /*p_reflect*/fr)//反射する場合    確率p_reflectで部分反射       p_reflectをfrと等しくすると良いらしい
                {
                    wi = mirrorreflect(wo, Vec3(0,0,1));//反射方向
                    pdf = /*p_reflect*/fr;//確率密度関数          デルタ関数が結局BSDFと打ち消し合うのでこれで良い
                    return fr /abscostheta(wi) * Vec3(1,1,1);//BSDFを返す
                }           


                else//屈折する場合　　　　　　　　　　　　　確率1-p_reflect
                {
                    //wiには屈折方向が代入されている
                    pdf = 1 - /*p_reflect*/fr;//確率密度関数              デルタ関数が結局BSDFと打ち消し合うのでこれで良い
                    return rate * (1 - fr) /abscostheta(wi) * Vec3(1,1,1);//BSDFを返す
                }      
            }
        }


        Vec3 Glass::sample_light(Vec3 const& wi, Vec3& wo, double& pdf) const
        {
            Vec3 n;//法線ベクトル
            bool ifrefract;//真理値
            double fr; //フレネル反射率 部分反射する割合
            double rate; //屈折による光の疎密による放射輝度の変化率 (全部屈折すると仮定した場合に屈折後の放射輝度は入射する光の放射輝度の何倍になるのかの値  BSDFとは違うものだということに注意せよ)  

            if(costheta(wi) < 0)//上から下向きに入射する場合  rayが物体外部から内部に向かって入射する場合
            {
                n = Vec3(0,0,1);//法線ベクトルは上向き 屈折方向を求めるのに必要
                ifrefract = refract(wi, wo, n, n_outside, n_inside);//屈折するのかどうか確認し, 屈折するなら屈折方向を求める
                rate = std::pow((n_outside/n_inside), 2);//実際はwiが入射光でwoが屈折後の光であることを忘れずに
            }
            else//下から上向きに入射する場合   rayが物体内部から外部に向かって入射する場合
            {
                n = Vec3(0,0,-1);//法線ベクトルは下向き 屈折方向を求めるのに必要    
                ifrefract = refract(wi, wo, n, n_inside, n_outside);//屈折するのかどうか確認し, 屈折するなら屈折方向を求める
                rate = std::pow((n_inside/n_outside), 2);//実際はwiが入射光でwoが屈折後の光であることを忘れずに
            }

            

            if(ifrefract == 0)//屈折せずに全反射する場合
            {   
                n = Vec3(0,0,1);//法線ベクトルはどちら向きでも良い
                wo = mirrorreflect(wi, n);//鏡面反射方向    法線ベクトルはどちら向きでも計算結果は同じになる              //
                pdf = 1;                 //確率密度関数          デルタ関数が結局BSDFと打ち消し合うのでこれで良い        //Mirrorと同様
                return 1/abscostheta(wo) * Vec3(1,1,1);//BSDFを返す                                             //
            }
            else
            { 
                    //woには屈折方向が代入されている
                fr = fresnel(wi, n, n_inside, n_outside); //フレネル反射率            //両者の屈折率を入れ替えても戻り値は変わらない  法線ベクトルnは代入されるが使われない


                //臨界角未満の場合
                if(rnd() < /*p_reflect*/fr)//反射する場合    確率p_reflectで部分反射   p_reflectをfrと等しくすると良いらしい
                {   
                    wo = mirrorreflect(wi, Vec3(0,0,1));//鏡面反射方向    法線ベクトルはどちら向きでも計算結果は同じになる
                    pdf = /*p_reflect*/fr;//確率密度関数          デルタ関数が結局BSDFと打ち消し合うのでこれで良い      //Mirrorとは少し違う(デルタ関数が複数ある場合はこのようにpdfは反射・屈折割合になる)
                    return fr /abscostheta(wo) * Vec3(1,1,1);//BSDFを返す
                }           


                else//屈折する場合　　　　　　　　　　　　　確率1-p_reflect
                {
                    //woには屈折方向が代入されている
                    pdf = 1 - /*p_reflect*/fr;//確率密度関数       デルタ関数が結局BSDFと打ち消し合うのでこれで良い     //Mirrorとは少し違う(デルタ関数が複数ある場合はこのようにpdfは反射・屈折割合になる)
                    return rate * (1 - fr) /abscostheta(wo) * Vec3(1,1,1);//BSDFを返す
                }      
            }
        }


        Vec3 Glass::getBSDF(const Vec3& wi, const Vec3& wo) const
        {
            return Vec3(0,0,0);
            //実質この関数は使われない(デルタ関数をもつから)
            //サンプルをとるときにデルタ関数の方向にrayを飛ばす確率は0 言い換えればデルタ関数でない方向に必ずrayが飛ぶのでデルタ関数の部分が積分できない
            //スペキュラー面では絶対にneeをしてはいけない
        }

        double Glass::getpdf_eye(Vec3 wi, Vec3 const& wo) const//視線rayを延長する確率密度(立体角に関する)
        {

            double fr;
            Vec3 n;
            bool ifrefract;

            if(wi.z*wo.z < 0)//衝突面を突き抜けずに跳ね返った場合
            {
                //反射しているので部分反射か全反射か調べる
                
                if(costheta(wo) < 0)//上から下向きに入射する場合  rayが物体外部から内部に向かって入射する場合
                {
                    n = Vec3(0,0,1);//法線ベクトルは上向き 屈折方向を求めるのに必要
                    ifrefract = refract(wo, wi, n, n_outside, n_inside);//屈折するのかどうか確認し, 屈折するなら屈折方向を求める  wiは不要
                }
                else//下から上向きに入射する場合   rayが物体内部から外部に向かって入射する場合
                {
                    n = Vec3(0,0,-1);//法線ベクトルは下向き 屈折方向を求めるのに必要    
                    ifrefract = refract(wo, wi, n, n_inside, n_outside);//屈折するのかどうか確認し, 屈折するなら屈折方向を求める  wiは不要
                }
                
                if(ifrefract == 1)//部分反射の場合
                {
                    fr = fresnel(wi, n, n_inside, n_outside); //フレネル反射率            //両者の屈折率を入れ替えても戻り値は変わらない  法線ベクトルnは代入されるが使われない
                    return fr;
                }
                else//全反射の場合
                {
                    return 1;//Mirrorと同様
                }
            }
            else
            {
                //屈折しているので
                fr = fresnel(wi, n, n_inside, n_outside); //フレネル反射率     //両者の屈折率を入れ替えても戻り値は変わらない  法線ベクトルnは代入されるが使われない
                return 1-fr;
            }  
        }

        double Glass::getpdf_light(Vec3 const& wi, Vec3 wo) const//光線rayを延長する確率密度(立体角に関する)
        {

            double fr;
            Vec3 n;
            bool ifrefract;

            if(wi.z*wo.z < 0)//衝突面を突き抜けずに跳ね返った場合
            {
                //反射しているので部分反射か全反射か調べる
                
                if(costheta(wi) < 0)//上から下向きに入射する場合  rayが物体外部から内部に向かって入射する場合
                {
                    n = Vec3(0,0,1);//法線ベクトルは上向き 屈折方向を求めるのに必要
                    ifrefract = refract(wi, wo, n, n_outside, n_inside);//屈折するのかどうか確認し, 屈折するなら屈折方向を求める  woは不要
                }
                else//下から上向きに入射する場合   rayが物体内部から外部に向かって入射する場合
                {
                    n = Vec3(0,0,-1);//法線ベクトルは下向き 屈折方向を求めるのに必要    
                    ifrefract = refract(wi, wo, n, n_inside, n_outside);//屈折するのかどうか確認し, 屈折するなら屈折方向を求める  woは不要
                }
                
                if(ifrefract == 1)//部分反射の場合
                {
                    fr = fresnel(wi, n, n_inside, n_outside); //フレネル反射率            //両者の屈折率を入れ替えても戻り値は変わらない  法線ベクトルnは代入されるが使われない
                    return fr;
                }
                else//全反射の場合
                {
                    return 1;//Mirrorと同様
                }
            }
            else
            {
                //屈折しているので
                fr = fresnel(wi, n, n_inside, n_outside); //フレネル反射率     //両者の屈折率を入れ替えても戻り値は変わらない  法線ベクトルnは代入されるが使われない
                return 1-fr;
            }  
        }



        bool Glass::isSpecular() const//BSDFに少なくとも1つのデルタ関数を持つかどうか
        {
            return (bool)ISSPECULAR::SPECULAR;
        }

        bool Glass::isVacuum() const//Materialの種類がVacuumかどうか
        {
            return (bool)ISVACUUM::NOTVACUUM;
        }

        bool Glass::isTransparent()const//光が衝突面を透過することがある(裏面のBSDFに0でないものが存在する)のかどうか
        {
            return (bool)ISTRANSPARENT::TRANSPARENT;
        }    

        void Glass::name() const
        {
            std::cout << "Glass" << std::endl;
        }

        int  Glass::Materialnum() const //enumの番号
        {
            return (int)MATERIAL::GLASS;
        }

    /*******************************************************************************/

    /************Vacuumの実装*******************************************************/

        Vacuum::Vacuum()
        {}

        Vec3 Vacuum::sample_eye(Vec3& wi, const Vec3& wo, double& pdf) const
        {
            wi = wo;

            pdf = 1;//デルタ関数が打ち消し合うので結局1で良い
            
            return 1/abscostheta(wi) * Vec3(1,1,1);//BSDFを返す
        }


        Vec3 Vacuum::sample_light(Vec3 const& wi, Vec3& wo, double& pdf) const
        {
            wo = wi;

            pdf = 1;//デルタ関数が打ち消し合うので結局1で良い
            
            return 1/abscostheta(wo) * Vec3(1,1,1);//BSDFを返す
        }


        Vec3 Vacuum::getBSDF(const Vec3& wi, const Vec3& wo) const
        {
            return Vec3(0,0,0);
            //実質この関数は使われない(デルタ関数をもつから)
            //サンプルをとるときにデルタ関数の方向にrayを飛ばす確率は0 言い換えればデルタ関数でない方向に必ずrayが飛ぶのでデルタ関数の部分が積分できない
            //スペキュラー面では絶対にneeをしてはいけない
        }

        double Vacuum::getpdf_eye(Vec3 wi, Vec3 const& wo) const//視線rayを延長する確率密度(立体角に関する)
        {
            return 1;
        }

        double Vacuum::getpdf_light(Vec3 const& wi, Vec3 wo) const//光線rayを延長する確率密度(立体角に関する)
        {
            return 1;
        }

        bool Vacuum::isSpecular() const//BSDFに少なくとも1つのデルタ関数を持つかどうか
        {
            return (bool)ISSPECULAR::SPECULAR;
        }

        bool Vacuum::isVacuum() const//Materialの種類がVacuumかどうか
        {
            return (bool)ISVACUUM::VACUUM;
        }

        bool Vacuum::isTransparent()const//光が衝突面を透過することがある(裏面のBSDFに0でないものが存在する)のかどうか
        {
            return (bool)ISTRANSPARENT::TRANSPARENT;
        }    

        void Vacuum::name() const
        {
            std::cout << "Vacuum" << std::endl;
        }

        int Vacuum::Materialnum() const//enumの番号
        {
            return (int)MATERIAL::VACUUM;
        }

        


    /*******************************************************************************/


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    /*********Plainの実装************************************************************/

        Plain::Plain(std::shared_ptr<Material> material_)
        {
            material = material_;
        }

        std::shared_ptr<Material> Plain::getmaterial(double const & x, double const & y)const//使わない変数
        {
            return material;
        }

        int Plain::patternnum()const//どんな種類のShapesで使えるか
        {
            return (int)SHAPES::ANYTHING;//何でも使える
        }

    /*******************************************************************************/

    /*********Colorful_sphereの実装**************************************************/


        Colorful_sphere::Colorful_sphere(){}

        std::shared_ptr<Material> Colorful_sphere::getmaterial(double const & theta, double const & phi)const
        {
            double a = phi;
            double b = theta;
            double k = 10;

            Diffuse diffuse;
            std::shared_ptr<Material> material;


            diffuse.rate = Vec3(0, 0, 1)*((sin(k*phi)+3)/4.0) + Vec3(0, 1, 0)*((sin(k*phi + M_PI)+3)/4.0) ;//+ Vec3(1, 0, 0) * ((sin(k*theta)+3)/4.0);

            material = std::make_shared<Diffuse>(diffuse);

            return material;


        }

        int Colorful_sphere::patternnum()const//どんな種類のShapesで使えるか
        {
            return (int)SHAPES::SPHERE;
        }
    /*******************************************************************************/

    /*********Colorful_pipeの実装****************************************************/

        Colorful_pipe::Colorful_pipe(){}

        std::shared_ptr<Material> Colorful_pipe::getmaterial(double const & phi, double const & z)const
        {
            int a = phi/2/M_PI*50;

            Diffuse diffuse;
            std::shared_ptr<Material> material;

            //diffuse.rate = Vec3(0, 0, 1)*((sin(10*phi)+3)/4.0) + Vec3(0, 1, 0)*((sin(10*(phi + M_PI/10.0))+3)/4.0);

            if(a%2 == 0)
            {
                material = std::make_shared<Diffuse>(Diffuse(Vec3(1,1,1)));
            }
            else
            {
                material = std::make_shared<Vacuum>(Vacuum());
            }
            

            

            return material;
        }

        int Colorful_pipe::patternnum()const//どんな種類のShapesで使えるか
        {
            return (int)SHAPES::PIPE;
        }

    /*******************************************************************************/

    /*********Colorful_triangleの実装************************************************/

        Colorful_triangle::Colorful_triangle(){}

        std::shared_ptr<Material> Colorful_triangle::getmaterial(double const & xp1, double const & xp2)const
        {

            Diffuse diffuse;
            std::shared_ptr<Material> material;

            diffuse.rate = Vec3(0, xp1, xp2);

            material = std::make_shared<Diffuse>(diffuse);

            return material;
        }

        int Colorful_triangle::patternnum()const//どんな種類のShapesで使えるか
        {
            return (int)SHAPES::TRIANGLE;
        }

    /*******************************************************************************/

    /*********Colorful_disk*********************************************************/

        Colorful_disk::Colorful_disk(){}

        std::shared_ptr<Material> Colorful_disk::getmaterial(double const & r, double const & phi)const
        {
            double a = r/30.0;
            double b = phi;

            Diffuse diffuse;
            std::shared_ptr<Material> material;

            diffuse.rate = Vec3(0, 0, 1)*(a*a/2.0 + 0.5)*((sin(10*phi)+3)/4.0) + Vec3(0, 1, 0)*(a*a/2.0 + 0.5)*((sin(10*phi + M_PI)+3)/4.0);

            material = std::make_shared<Diffuse>(diffuse);

            return material;
        }

        int Colorful_disk::patternnum()const//どんな種類のShapesで使えるか
        {
            return (int)SHAPES::DISK;
        }


    /*******************************************************************************/

    /*********Colorful_cuboid*******************************************************/

        Colorful_cuboid::Colorful_cuboid(){}

        std::shared_ptr<Material> Colorful_cuboid::getmaterial(double const & x, double const & y)const
        {
            int a = (x+1)*10;
            int b = (y+1)*10;

            Diffuse diffuse;
            std::shared_ptr<Material> material;

            if((a+b)%2 == 0)
            {
                
                diffuse.rate = Vec3(1, 1, 1)*0;
                material = std::make_shared<Diffuse>(diffuse);

                return material;
            }
            else
            {
                diffuse.rate = Vec3(1, 1, 1);

                material = std::make_shared<Diffuse>(diffuse);

                return material;
            }

            
            
        }

        int Colorful_cuboid::patternnum()const//どんな種類のShapesで使えるか
        {
            return (int)SHAPES::CUBOID;
        }

    /*******************************************************************************/

    /*********DiskHole_cuboid******************************************************/

        DiskHole_cuboid::DiskHole_cuboid(double holeradius_):holeradius(holeradius_)
        {}
        
        std::shared_ptr<Material> DiskHole_cuboid::getmaterial(double const & x, double const & y)const
        {
            if(sqrt(x*x + y*y) < holeradius)
            {
                return std::make_shared<Vacuum>(Vacuum());//どんな種類のShapesで使えるか
            }
            else
            {
                return std::make_shared<Diffuse>(Diffuse(Vec3(0,0,1)));
            }
            
        }

        int DiskHole_cuboid::patternnum()const
        {
            return (int)SHAPES::CUBOID;
        }

    /*******************************************************************************/

    /*********Picture_sphere*******************************************************/

        Picture_sphere::Picture_sphere(const std::string str):str(str)
        {
            loadppm(str, mode, imagewidth, imageheight, maxbrightness, color);//画像のファイル名をstrで受け取り, その名前の画像の色データを0から1になるように正規化してcolorに格納する

            if(imagewidth != 2*imageheight)//縦と横の画素数の比が1対2でないものを受け付けない
            {
                std::cout << "縦と横の画素数の比が1対2でない" << '(' << str << ')' << std::endl;
                exit(1);
            }
        }

        std::shared_ptr<Material> Picture_sphere::getmaterial(double const & theta, double const & phi)const
        {
            /***************球面座標のphi, thetaがそれぞれ画像の座標の縦, 横の座標になるように変換する*************例えばメルカトル図法の画像を用いれば地球儀ができる****************/
            int i =   phi/ (2.0*M_PI)*imagewidth;
            int j = theta/ (M_PI)    *imageheight;
            /****************************************************************************************************************************************************/

            /************配列の外のデータを読んでしまうのを防ぐ*************************/
                if((i>imagewidth-1) || (i<0))//画像の横の座標がはみ出れば
                {
                    std::cout << "画像の横の座標が" << "\"" << str + "\"" << "の範囲を超えてしまった" << std::endl;  //   "\""のバックスラッシュはダブルクオーテーションを表示するために必要  """とすることはできない
                    exit(1);
                }
                
                if((j>imageheight-1) || (j<0))//画像の縦の座標がはみ出れば
                {
                    std::cout << "画像の縦の座標が" << "\"" << str + "\"" << "の範囲を超えてしまった" << std::endl;
                    exit(1);
                }
            /********************************************************************/

            //座標が画像の範囲を超えなければ


            /****************戻り値のmaterialを求める******************************************************/
                Diffuse diffuse;
                std::shared_ptr<Material> material;

                diffuse.rate = color[(long)2*imageheight*j + i];
                                                                                                            
                material = std::make_shared<Diffuse>(diffuse);

                return material;
            /*******************************************************************************************/


        }

        int Picture_sphere::patternnum()const//どんな種類のShapesで使えるか
        {
            return (int)SHAPES::SPHERE;
        }
    /*******************************************************************************/

    /*********Picture_pipe**********************************************************/
        Picture_pipe::Picture_pipe(const std::string str): str(str)
        {
            loadppm(str, mode, imagewidth, imageheight, maxbrightness, color); //画像のファイル名をstrで受け取り, その名前の画像の色データを0から1になるように正規化してcolorに格納する
        }

        std::shared_ptr<Material> Picture_pipe::getmaterial(double const & phi, double const & z)const//phi: 円筒の側面をぐるっと回る角度  //z: 円筒の底面からの高さを0から1に正規化したもの
        {
            /***************円筒を展開した長方形のz, phi方向を伸ばしたり縮めたりして画像の縦, 横方向に一致するようにする*************************************/
            int i = phi/  (2.0*M_PI) * imagewidth;
            int j = z  /  (M_PI)     * imageheight;
            /****************************************************/


            /************配列の外のデータを読んでしまうのを防ぐ*************************/
                if((i>imagewidth-1) || (i<0))//画像の横の座標がはみ出れば
                {
                    std::cout << "画像の横の座標が" << "\"" << str + "\"" << "の範囲を超えてしまった" << std::endl;  //   "\""のバックスラッシュはダブルクオーテーションを表示するために必要  """とすることはできない
                    exit(1);
                }
                
                if((j>imageheight-1) || (j<0))//画像の縦の座標がはみ出れば
                {
                    std::cout << "画像の縦の座標が" << "\"" << str + "\"" << "の範囲を超えてしまった" << std::endl;
                    exit(1);
                }
            /********************************************************************/

            //座標が画像の範囲を超えなければ

            /****************戻り値のmaterialを求める******************************************************/
                Diffuse diffuse;
                std::shared_ptr<Material> material;

                diffuse.rate = color[(long)2*imageheight*j + i];
                                                                                                            
                material = std::make_shared<Diffuse>(diffuse);

                return material;
            /*******************************************************************************************/


        }


        int Picture_pipe::patternnum()const//どんな種類のShapesで使えるか
        {
            return (int)SHAPES::PIPE;
        }
    /*******************************************************************************/

    /*********Picture_disk**********************************************************/

        Picture_disk::Picture_disk(const std::string str): str(str)//ここでは画像の縦と横の画素数が同じもの以外受け付けない
        {
            loadppm(str, mode, imagewidth, imageheight, maxbrightness, color);//画像のファイル名をstrで受け取り, その名前の画像の色データを0から1になるように正規化してcolorに格納する
            if(imagewidth != imageheight)//縦と横の画素数が異なるものを受け付けない
            {
                std::cout << "縦と横が違う" << '(' << str << ')' << std::endl;
                exit(1);
            }
        }

        std::shared_ptr<Material> Picture_disk::getmaterial(double const & r, double const & phi)const//r: 円盤を単位円とみたときの中心からの距離  //phi: 円盤の周りをぐるっと回る角度
        {
            /******************円盤を正方形の画像の内接円になるように伸ばしたり縮めたりしたときの衝突点の座標を求める(円盤の中心を原点とする)*********************/
            int x = r*imagewidth/2*cos(phi);
            int y = r*imagewidth/2*sin(phi);
            /********************************************************************************************************/

            /*****************上で求めたx, yを画像の画素の座標i, jに変換する***********************/
            int i = x+imagewidth/2;
            int j = imageheight/2-y;
            /********************************************************************************/


            /************配列の外のデータを読んでしまうのを防ぐ*************************/
            if((i>imagewidth-1) || (i<0))//画像の横の座標がはみ出れば
                {
                    std::cout << "画像の横の座標が" << "\"" << str + "\"" << "の範囲を超えてしまった" << std::endl;  //   "\""のバックスラッシュはダブルクオーテーションを表示するために必要  """とすることはできない
                    exit(1);
                }
                
                if((j>imageheight-1) || (j<0))//画像の縦の座標がはみ出れば
                {
                    std::cout << "画像の縦の座標が" << "\"" << str + "\"" << "の範囲を超えてしまった" << std::endl;
                    exit(1);
                }
            /********************************************************************/

            //座標が画像の範囲を超えなければ

            /****************戻り値のmaterialを求める******************************************************/
                if(color[imagewidth*j + i] < Vec3(0.5,0.5,0.5))
                {
                    return std::make_shared<Diffuse>(Diffuse(Vec3(1,1,1)));//衝突点をDiffuseとする
                }
                else
                {
                    return std::make_shared<Vacuum>(Vacuum());//何もないのと同様に扱う
                }
            /*******************************************************************************************/
        }

        int Picture_disk::patternnum ()const//どんな種類のShapesで使えるか
        {
            return (int)SHAPES::DISK;
        }

    /*******************************************************************************/

    /*********Picture_rectangle*****************************************************/
        Picture_rectangle::Picture_rectangle(const std::string str): str(str)
        {
            loadppm(str, mode, imagewidth, imageheight, maxbrightness, color);//画像のファイル名をstrで受け取り, その名前の画像の色データを0から1になるように正規化してcolorに格納する
        }

        std::shared_ptr<Material> Picture_rectangle::getmaterial(double const& x, double const& y)const
        {

            /**********************************/
                //x, yは長方形に衝突したときの座標を辺の長さが2の正方形となるように縦横伸ばしたり縮めたりしたときの座標(中心を(0,0)とする)
                //したがって-1<=x<=1, -1<=y<=1だからこれを画像の形に変形するには以下の式を用いる
                int i = (x+1)*imagewidth/2;
                int j = (y+1)*imageheight/2;
            /**********************************/

            /******配列からはみ出たら中に収まるようにする************/
            if(i<0)
            {
                i = 0;
            }
            else if(imagewidth<=i)
            {
                i = imagewidth-1;
            }

            if(j<0)
            {
                j = 0;
            }
            else if(imageheight<=j)
            {
                j = imageheight-1;
            }
            /**************************************************/
            
            return std::make_shared<Diffuse>(Diffuse(color[j*imagewidth + i]));
        }

        int Picture_rectangle::patternnum ()const//どんな種類のShapesで使えるか
        {
            return (int)SHAPES::CUBOID;
        }
    /*******************************************************************************/


    std::shared_ptr<Pattern> pColorful_sphere()//patternのポインタを生成する
    {
        return std::make_shared<Colorful_sphere> (Colorful_sphere());
    }

    std::shared_ptr<Pattern> pColorful_pipe()
    {
        return std::make_shared<Colorful_pipe> (Colorful_pipe());
    }

    std::shared_ptr<Pattern> pColorful_triangle()
    {
        return std::make_shared<Colorful_triangle> (Colorful_triangle());
    }

    std::shared_ptr<Pattern> pColorful_disk()
    {
        return std::make_shared<Colorful_disk> (Colorful_disk());
    }

    std::shared_ptr<Pattern> pColorful_cuboid()
    {
        return std::make_shared<Colorful_cuboid> (Colorful_cuboid());
    }

    std::shared_ptr<Pattern> pPicture_sphere(std::string str)
    {
        return std::make_shared<Picture_sphere> (Picture_sphere(str));
    }

    std::shared_ptr<Pattern> pDiskHole_cuboid(double holeradius)
    {
        return std::make_shared<DiskHole_cuboid>(DiskHole_cuboid(holeradius));
    }

    std::shared_ptr<Pattern> pPicture_rectangle(std::string str)
    {
        return std::make_shared<Picture_rectangle>(Picture_rectangle(str));
    }
}

namespace L     //LightのL
{
    /*********Uniform**********************************************************************/

        Uniform::Uniform(Vec3 const& _color):color(_color)
        {};

        Vec3 Uniform::Le(Vec3 const&)const// 変数は使われない
        {
            return color;
        }

        void Uniform::sample_light(Vec3& wo, double& pdflast) const//rayの方向を求め, pdflast(rayの方向に関する確率密度関数), cos_out, BSDF(EDF)も求める 
        {
            Vec3 firstdirection;//rayを発射する方向(ワールド座標)
            Vec3 firstdirection_1;//単位ベクトルにしたもの

            double a;//乱数
            double b;//乱数
            double theta;
            double phi;
            double x;
            double y;
            double z;
            bool ifoutside = 1;

            /******************rayを発射する方向を求める**************************/
                a = rnd();//乱数を求める
                b = rnd();//乱数を求める

                theta = (1/2.0) * acos(1-2*a);
                phi = 2 * M_PI * b;

                x = sin(theta) * cos(phi);
                y = sin(theta) * sin(phi);
                z = cos(theta);


                if(ifoutside == 1)
                {
                    wo = Vec3(x,y,z);//rayを発射する方向(ローカル座標)を求める
                }
                else
                {
                    wo = Vec3(x,y,-z);//rayを発射する方向(ローカル座標)を求める
                }
            /*****************************************************************/

            pdflast = abscostheta(wo)/M_PI;
        }

        double Uniform::getpdf_firstray(Vec3 const& wo) const//光源から光線サブパスの最初のrayが発射される方向に関する確率密度関数
        {
            return abscostheta(wo)/(M_PI);
        }
    /*******************************************************************************/

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /*********Plain**********************************************************************/

        Plain::Plain(std::shared_ptr<Light> light_): light(light_)
        {}

        std::shared_ptr<Light> Plain::getlight(double const& x, double const& y) const
        {
            return light;
        }

        int Plain::patternnum() const//どんな種類のShapesで使えるか
        {
            return (int)SHAPES::ANYTHING;
        }

    /*******************************************************************************/

    /*********pattern_disk**********************************************************************/

        pattern_disk::pattern_disk(){}

        std::shared_ptr<Light> pattern_disk::getlight(double const& r, double const& phi) const
        {
            double a = r*2*M_PI;
            return std::make_shared<Uniform> (Uniform(      20*(     Vec3(0, 255, 0)*(cos(a*1)+1)/2 + Vec3(0, 0, 255)*(cos(a*1+2*M_PI/3)+1)/2 + Vec3(255, 0, 0)*(cos(a*1+4*M_PI/3)+1)/2       )       ));
        }

        int pattern_disk::patternnum() const//どんな種類のShapesで使えるか
        {
            return (int)SHAPES::DISK;
        }
        
    /*******************************************************************************/

    std::shared_ptr<Pattern> ppattern_disk()
    {
        return std::make_shared<pattern_disk>(   pattern_disk()   );
    }
}