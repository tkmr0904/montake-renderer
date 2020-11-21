#include "../Header/image.h"


void hour_mimnute_second(double const& seconds, int& hour, int& minute, double& second)//第1引数の秒数から 何時間(整数), 何分(整数), 何秒(小数), をhour, minute, second に代入する
{
    hour = (int)(seconds/3600);
    minute = (int)fmod(seconds/60, 60);
    second = fmod(seconds, 60);
}

std::stringstream _0fill(int const& i, int const& digitnum)//数字の桁数(0を含む) が一定数になるように0を大きい位に追加する　例えば0001
{
    std::stringstream stream;//この時点では文字は入っていない
    int n;//0を除く桁数を求める

    if(i<0)
    {
        std::cout << "画像の番号が負の数であってはならない" << std::endl;
        exit(1);
    }

    //画像の番号が0以上の整数なら


    /*******0を除く桁数を求め, nに代入する******/
        for(n = 1; ; n++)
        {
            if(i < exp10(n))
            {
                break;
            }
        }
    /**************************************/
    //nは0を除く桁数

    /*******************0を除く部分の桁数がdigitnumより大きい場合*********************************/
    if(digitnum < n)
    {
        std::cout << "0を除く部分の桁数が指定された桁数" << digitnum << "を超えた" << std::endl;
        exit(1);
    }
    /***************************************************************************************/


    /***********0でdigitnum-n個埋める******************/
        for(int j = 0; j<(digitnum-n); j++)//0を埋める
        {
            stream << '0';
        }
    /************************************************/

    return stream;
}

void getfirstray_light(Ray& ray, Hit& hit, double& pdflast)//光源から発射される最初の光線rayを得る, ついでにhitLe, BSDF, cos_out, pdflastを求める
{
    Vec3 s, t;
    Vec3 wo;
    Vec3 firstdirection, firstdirection_1;

    orthonormalbasis(hit.hitnormal, s, t);    //正規直行系を得る

    hit.hitlight->sample_light(wo, pdflast);//光線サブパスの最初のrayの方向(ローカル座標)を得る   ついでに pdflastを求める

    hit.hitLe = hit.hitlight->Le(wo);

    hit.BSDF = Vec3(1,1,1);//もともとこれは計算不要なのだがweightを計算するときに掛け算されてしまう  なので1を代入しておく

    hit.cos_out = std::abs(costheta(wo)); //cosの値を得る

    firstdirection = local_to_world(wo, s, t, hit.hitnormal);//rayの方向を取り敢えず格納する woをワールド座標に直したもの

    firstdirection_1 = normalize(firstdirection);

    ray = Ray(hit.hitpos + firstdirection_1 * _d_ , firstdirection); //ワールド座標を得る   (少し衝突面から離す)
}

void getnextray_eye(Ray& ray, Hit& hit, Hit& prehit, double& pdflast)//wiが入射し, woが発射する
{
    Vec3 wi, wo, s, t;
    Vec3 nextdirection;//wiをワールド座標に直したもの
    Vec3 nextdirection_1;//単位ベクトルにしたもの

    orthonormalbasis(hit.hitnormal, s, t);       //正規直行系を得る
    wo = world_to_local(ray.direction, s, t, hit.hitnormal);     //入射rayのローカル座標の方向を得る
    hit.BSDF = hit.hitmaterial->sample_eye(wi, wo, pdflast);      //hitnormalの方向によってwo.zが正の値になったり負の値になったりすることを忘れずに        //ローカル座標のwiとpdfを得る
    hit.cos_out = std::abs(costheta(wi)); //wiが求まったのでcosの値を得る

    prehit.pdf_reverse = hit.hitmaterial->getpdf_light(-1*wi, -1*wo) / prehit.cos_out * (hit.hitdistance * hit.hitdistance);

    nextdirection =  local_to_world(wi, s, t, hit.hitnormal);//次のrayの方向を取り敢えず格納する wiをワールド座標に直したもの
    nextdirection_1 = normalize(nextdirection);//単位ベクトルを求める


    ray = Ray(hit.hitpos + nextdirection_1 * _d_ , nextdirection); //ワールド座標を得る   (少し衝突面から離す)
}

void getnextray_light(Ray& ray, Hit& hit, Hit& prehit, double& pdflast)//wiが入射し, woが発射する    rayの方向を求め, pdflast(rayの方向に関する確率密度関数)も求める, cos_out, BSDF, 前のpdf_reverseも求める
{
    Vec3 wi, wo, s, t;
    Vec3 nextdirection;//woをワールド座標に直したもの
    Vec3 nextdirection_1;//単位ベクトルにしたもの

    /*********************光線rayの入射rayのローカル座標系の方向を求める**************************************/
    orthonormalbasis(hit.hitnormal, s, t);    //正規直行系を得る
    wi = world_to_local(ray.direction, s, t, hit.hitnormal);      //入射rayのローカル座標の方向を得る  
    /***************************************************************************************************/

    /*****************wiとhitmaterialを用いてwo(ローカル座標)をサンプリングする***************************************************************/
    hit.BSDF = hit.hitmaterial->sample_light(wi, wo, pdflast);      //hitnormalの方向によってwi.zが正の値になったり負の値になったりすることを忘れずに 
    /***********************************************************************************************************************/


    /************************その他のhitの情報を得る************************************************************************************/
    hit.cos_out = std::abs(costheta(wo)); //cosの値を得る
    prehit.pdf_reverse = hit.hitmaterial->getpdf_eye(-1*wi, -1*wo) / prehit.cos_out * (hit.hitdistance * hit.hitdistance);
    /***********************************************************************************************************************/

    /***********************woをワールド座標系に直して発射rayを得る**************************************************************/
    nextdirection =  local_to_world(wo, s, t, hit.hitnormal);//次のrayの方向を取り敢えず格納する woをワールド座標に直したもの
    nextdirection_1 = normalize(nextdirection);//単位ベクトルを求める
    ray = Ray(hit.hitpos + nextdirection_1 * _d_ , nextdirection); //ワールド座標を得る   (少し衝突面から離す)
    /***********************************************************************************************************************/
}

void getposition_nee(Objects const& objects, Hit& hit)//hit.hitpos, hit.hitnormal, hit.pdf, hit.hitlight, hit.hitmaterialを求める hit.ifhitを1にする
{

    /********************乱数を求める***************************************/
    int num = rnd() * objects.objectnumbersum_nee;//乱数 //0からobjects.objectnumbersum_nee - 1が等確率で現れる

    if(num == objects.objectnumbersum_nee)//確率はほぼ0だが,この場合は 
    {
        num = objects.objectnumbersum_nee - 1;//objects.objectnumbersum_nee - 1に変更する
    }
    /********************************************************************/


    /*****************等確率で物体を選び, その点の情報をhitに格納する*************************************************************/
        for(int i = 0; i < (int)SHAPES::NUM_OF_SHAPES; i++)
        {
            if (num < objects.objectnumber_nee[i])
            {
                (objects.shapes_nee[i])[num]->getposition(hit);//hit.hitpos, hit.hitnormal, hit.pdf, hit.hitlight, hit.hitmaterialを求める
                return;
            }
            num -= objects.objectnumber_nee[i];
        }
    /***********************************************************************************************************************/

    //returnせずにここまで来たら
    std::cout << "getposition_neeでエラー発生" << std::endl;
    exit(1);//バグ回避
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*****************Imageの実装*********************************************************************************************************************************************************/
    Image::Image(int const& _width, int const& _height, double const& p_eye, double const& p_light, int const& p_eye_1_num, int const& p_light_1_num, bool const& ifnee, bool const& ifeye, bool const& iflight) : width(_width), height(_height), p_eye(p_eye), p_light(p_light), p_eye_1_num(p_eye_1_num), p_light_1_num(p_light_1_num), ifnee(ifnee), ifeye(ifeye), iflight(iflight)
    {
        data = new Vec3[width * height];
        data2 = new Vec3[width * height];
        data_temp = new Vec3[width * height];
        num_of_samples_each = new long[width * height];
        addednum_each = new long[width * height];


        for(int i = 0; i < width * height; i++)
        {
            addednum_each[i] = 0;//最初は0を代入する
        }
    }

    Image::~Image()
    {
        
        delete[] data;
        data = NULL;


        delete[] data2;
        data2 = NULL;


        delete[] data_temp;
        data_temp = NULL;


        delete[] num_of_samples_each;
        num_of_samples_each = NULL;


        delete[] addednum_each;
        addednum_each = NULL;
    }

    void Image::GettingReady()
    {
        seconds = 0;                        //次にサンプルを取る場合に初期化する
                                            
        for(int i = 0; i < threadsum; i++)  //
        {                                   //
            samplecounter[i] = 0;           //
        }                                   //
    }

    void Image::getSample(int const & i, int const & j, const Camera& camera, const Objects &objects, const Sky& sky, Subpath& subpath, const int& threadnumber)//サンプル1つの値を計算する
    {
        Accel accel;
        Vec3 col;
        double pdfsum_nee;//neeのrayの方向の立体角に関する確率密度関数の和
        
        int&& snum = camera.getnum_subpath();//簡単にするため

        Vec3 n;//入射rayとの内積が負になる法線ベクトルを代入する

        //サブパスの構築
        {
            /***************************************/
            double pdflast;//次の確率
            double roulettelast;//次の確率
            /***************************************/

            if(ifeye == 1)                     //視線サブパスを構築するなら
            {
                Hit hit_eye;
                Ray ray_eye;
                int k1 = 0;//視線サブパスのfor文で用いる

                /**********カメラの中でrayを飛ばし, subpathに情報を格納し, カメラから出るrayを得る**************/
                    if(k1 == 0)//最初だけ実行される
                    {
                        ray_eye = camera.getRay(i, j, subpath, pdflast); //カメラの中でrayを飛ばし, subpathに情報を格納し, カメラから出るrayを得る
                        roulettelast = 1;//必ず最初の点はサンプリングされるため
                        k1 = camera.getnum_subpath();
                    }
                /**************************************************************************************/

                for(; (k1<RAY_MAX); k1++) //rayを延長して繰り返す カメラの中で作ったサブパスの分を飛ばす
                {
                    /************************ロシアンルーレットの逆方向の確率とウェイトを計算する****ついでに入射rayを保存する*****************/
                        hit_eye.roulette = roulettelast;
                        hit_eye.weight = subpath.hit_eye[k1-1].weight * subpath.hit_eye[k1-1].cos_out * subpath.hit_eye[k1-1].BSDF / (pdflast * hit_eye.roulette);//pdflastが書き換わる前に用いる
                        hit_eye.raydirection_in = ray_eye.direction;
                    /*************************************************************************************************************/
                    
                    //全ての物体で衝突判定をする
                    { 
                        /**************************物体の衝突を考える**********************************************/
                            hit_eye.set(0, 0, hitdistance0);//初期化
                            accel.intersects(ray_eye, objects, hit_eye); //rayの衝突を判定し, 衝突したら衝突面の情報を保存する
                            accel.intersects_nee2(ray_eye, objects, hit_eye, pdfsum_nee); //neeの物体でも衝突判定 //ここではpdfsum_neeは使わない
                        /***************************************************************************************/
                        //この時点でhit.hitmaterial, hit.hitlight, hit.hitpos, hit.hitnormal, hit.hitdistance, hit.raydirection_in, weight, rouletteが保存されている

                        /********物体に衝突したかしていないかによって処理を分ける********/
                            if (hit_eye.ifhit == 1)//物体に衝突した場合
                            {
                                //rayの延長に進む
                                //その先でhit.hitLe, hit.BSDF, hit.cos_in, hit.cos_out, hit.pdf, pdf_reverse, 前のroulette_reverse, pdflast, roulettelastを求める
                            }
                            else //当たらなければ
                            {
                                //hit_eye.hitdistance = 1;//衝突点の周りに単位球の形の光源があると考える
                                //hit_eye.hitnormal = ray_eye.direction;
                                //hit_eye.cos_in = std::abs(dot(ray_eye.direction, hit_eye.hitnormal));
                                //hit_eye.pdf = pdflast * hit_eye.cos_in / (hit_eye.hitdistance * hit_eye.hitdistance);
                                //hit_eye.hitpos = ray_eye.origin + ray_eye.direction;
                                hit_eye.hitLe = sky.getradience(ray_eye);
                                //この時点でhit.hitLe, hit.hitpos, hit.hitnormal, hit.hitdistance, hit.raydirection_in, hit.cos_in, hit.pdfが保存されている(コメントアウトしたので保存されていない)

                                subpath.addeye(hit_eye, k1);//hitを保存する(意味があるのはhit.hitLe)
                                //脱出後にSubpathに保存しておいた値を用いてセンサーに入射する放射束を求める
                                
                                break;     //脱出(rayの延長をしない)
                            }
                        /********************************************************/
                    }

                    //ロシアンルーレットの確率を決める
                    roulettelast = getroulette_eye(k1);

                    if(rnd() < roulettelast) //rayを延長する
                    {

                        /*****************hitLeを求める******************************************/

                            /****************衝突点のLeを求める*************************/
                            Vec3 s,t, wi,wo;
                            orthonormalbasis(hit_eye.hitnormal, s, t);
                            wo = world_to_local(hit_eye.raydirection_in, s, t, hit_eye.hitnormal);
                            hit_eye.hitLe = hit_eye.hitlight->Le(wo);
                            /*********************************************************/
                            
                            /****************neeをする場合は加算しない*************************/
                            Hit& hitlast_eye = subpath.hit_eye[k1 - 1];//見づらいので簡単にする

                            if(((k1-1) >= (snum)) && (ifnee == 1) && (bool(hitlast_eye.hitmaterial)) && (hitlast_eye.hitmaterial->isSpecular() == 0))
                            //発射点がカメラの外にあって ifneeが1, materialがスペキュラーでなければ
                            {
                                hit_eye.hitLe = Vec3(0,0,0);//色を加算しない
                            }
                            /*************************************************************/
                        /**********************************************************************/

                        /*****************************cos_inとpdfを求める*******************************************/
                            hit_eye.cos_in = std::abs(dot(ray_eye.direction, hit_eye.hitnormal));
                            hit_eye.pdf = pdflast * hit_eye.cos_in / (hit_eye.hitdistance * hit_eye.hitdistance);
                        /*****************************************************************************************/

                        /*******1つ前のhit_eyeにroulette_reverse, pdf_reverseを代入し, ****今回のhit_eyeにBSDF, cos_outを代入し, pdflastを計算する**********/
                            subpath.hit_eye[k1-1].roulette_reverse = p_light;
                            getnextray_eye(ray_eye, hit_eye, subpath.hit_eye[k1-1], pdflast);//反射後のrayの方向を得て、さらにpdflast, hit.BSDF, hit.cos_out, 前のhit.pdf_reverseを計算する
                        /*********************************************************************************************************************/

                        subpath.addeye(hit_eye, k1);//hitを格納する
                        //rayの延長に進む
                    }
                    else // 延長しない
                    {

                        /*****************hitLeを求める******************************************/

                            /****************衝突点のLeを求める*************************/
                            Vec3 s,t, wi,wo;
                            orthonormalbasis(hit_eye.hitnormal, s, t);
                            wo = world_to_local(hit_eye.raydirection_in, s, t, hit_eye.hitnormal);
                            hit_eye.hitLe = hit_eye.hitlight->Le(wo);
                            /**************************************************/
                            
                            /****************neeをする場合は加算しない*************************/
                            Hit& hitlast_eye = subpath.hit_eye[k1 - 1];//見づらいので簡単にする

                            if(((k1-1) >= (snum)) && (ifnee == 1) && (bool(hitlast_eye.hitmaterial)) && (hitlast_eye.hitmaterial->isSpecular() == 0))
                            //発射点がカメラの外にあって ifneeが1, materialがスペキュラーでなければ
                            {
                                hit_eye.hitLe = Vec3(0,0,0);//色を加算しない
                            }
                            /*************************************************************/
                        /**********************************************************************/



                        /*****************************cos_inとpdfを求める*******************************************/
                            hit_eye.cos_in = std::abs(dot(ray_eye.direction, hit_eye.hitnormal));
                            hit_eye.pdf = pdflast * hit_eye.cos_in / (hit_eye.hitdistance * hit_eye.hitdistance);
                        /*****************************************************************************************/

                        /*********************1つ前のhit_eyeにroulette_reverseを代入する************************/
                            subpath.hit_eye[k1-1].roulette_reverse = p_light;
                        /************************************************************************************/

                        subpath.addeye(hit_eye, k1);//hitを格納する
                        //この時点でhit.hitLe, hit.hitpos, hit.hitnormal, hit.hitdistance, hit.raydirection_in, hit.cos_in, hit.pdf, hit.hitmaterial, roulette, 前のroulette_reverseが保存されている
                        //しかし現時点ではBSDF, cos_out, 前のpdf_reverse, 今のpdf_reverse, 今のroulette_reverseが求められないので後でサブパスの接続をするときに求める(配列には保存しない)
                        //pdflast, roulettelastは求める必要がない
                        break;
                    }

                }
            }
            
            if((ifnee == 1) || (iflight == 1) )//neeをするもしくは光線サブパスを構築するなら
            {
                Hit hit_light;
                Ray ray_light;
                int k2 = 0;//光線サブパスのfor文で用いる
                Vec3 s, t, wo, firstdirection, firstdirection_1;

                /***********光源から出るrayを得る*******************/
                    if(k2 == 0)//最初だけ実行される
                    {
                        getposition_nee(objects, hit_light);//neeの物体上の点を求める ついでにhit.hitnormal, hit.hitpos, hit.pdf, hit.hitlightを求める

                        hit_light.roulette = 1;//iflightが1である限り必ず最初の点は作られるため
                        roulettelast = 1;//iflightが1である限り必ず2番目の点が作られるため(無限遠の点も含む)

                        hit_light.weight = Vec3(1,1,1) * 1 / (hit_light.pdf * hit_light.roulette);

                        getfirstray_light(ray_light, hit_light, pdflast);//光源から発射される最初の光線rayを得る, ついでにhitLe, cos_out, pdflastを求める

                        subpath.addlight(hit_light, 0);//hitを保存する

                        k2 = 1;//光線サブパスの点を更に構築する場合は次の点は1番の点であるのでk2を1にする
                    }
                /************************************************/

                if(iflight == 1)
                {
                    for(; k2<RAY_MAX; k2++)  //光線rayを延長して繰り返す
                    {
                        /************************ロシアンルーレットの逆方向の確率とウェイトを計算する****ついでに入射rayを保存する*****************/
                            hit_light.roulette = roulettelast;
                            hit_light.weight = subpath.hit_light[k2-1].weight * subpath.hit_light[k2-1].cos_out * subpath.hit_light[k2-1].BSDF / (pdflast * hit_light.roulette);//pdflastが書き換わる前に用いる
                            hit_light.raydirection_in = ray_light.direction;//入射rayを保存する
                        /*************************************************************************************************************/
                        
                        
                        /**************************物体の衝突を考える**********************************************/
                            hit_light.set(0, 0, hitdistance0);//初期化
                            accel.intersects(ray_light, objects, hit_light);
                            accel.intersects_nee2(ray_light, objects, hit_light, pdfsum_nee);// pdfsum_neeは使わない
                        /***************************************************************************************/
                        //この時点でhit.hitmaterial, hit.hitlight, hit.hitpos, hit.hitnormal, hit.hitdistance, hit.raydirection_in, weight, rouletteが保存されている

                        /********物体に衝突したかしたいないかによって処理を分ける********/
                            if (hit_light.ifhit == 1)//物体に衝突した場合
                            {
                                //この時点でhit.hitmaterial, hit.hitlight, hit.hitpos, hit.hitnormal, hit.hitdistance, hit.raydirection_in, weight, rouletteが保存されている
                                //rayの延長に進む
                                //その先でhit.hitLe, hit.BSDF, hit.cos_in, hit.cos_out, hit.pdf, pdf_reverse, 前のroulette_reverse, pdflast, roulettelastを求める                            


                            }
                            else //当たらなければ
                            {
                                // この無限遠にある光線サブパスの点はサンプリングで意味をなさない
                                //前のpdf_reverse, 前のroulette_reverse, cos_out, BSDF, pdflast, roulettelastは求める必要がない


                                //subpath.addlight(hit_light, k2);//hitを保存する必要がない
                                //脱出後にSubpathに保存しておいた値を用いてセンサーに入射する放射束を求める
                                
                                break;     //脱出(rayの延長をしない)
                            }
                        /********************************************************/
                        
                        //ロシアンルーレットの確率を決める
                        roulettelast = getroulette_light(k2);

                        if(rnd() < roulettelast) //rayを延長する
                        {
                            /*****************************cos_inとpdfを求める*******************************************/
                                hit_light.cos_in = std::abs(dot(ray_light.direction, hit_light.hitnormal));
                                hit_light.pdf = pdflast * hit_light.cos_in / (hit_light.hitdistance * hit_light.hitdistance);
                            /*****************************************************************************************/

                            /*******1つ前のhit_eyeにroulette_reverse, pdf_reverseを代入し, ****今回のhit_eyeにBSDF, cos_outを代入し, pdflastを計算する**********/
                                subpath.hit_light[k2-1].roulette_reverse = p_eye;//この値を使うとは限らない 代わりに1を使うかもしれない
                                getnextray_light(ray_light, hit_light, subpath.hit_light[k2-1], pdflast);//反射後のrayの方向を得て、さらにpdflast, hit.BSDF, hit.cos_out, 前のhit.pdf_reverseを計算する
                            /*****************************************************************************************************************************/



                            /***********************hitLeを求める  rayの出射方向が分かってやっとhitLeが求められる*******************/
                                Vec3 s,t, wi,wo;
                                orthonormalbasis(hit_light.hitnormal, s, t);
                                wo = world_to_local(ray_light.direction, s, t, hit_light.hitnormal);//出射方向
                                hit_light.hitLe = hit_light.hitlight->Le(wo);
                            /**********************************************************************************************/


                            subpath.addlight(hit_light, k2);//hitを格納する

                            //rayの延長に進む
                        }
                        else // 延長しない
                        {
                            /*****************************cos_inとpdfを求める*******************************************/
                                hit_light.cos_in = std::abs(dot(ray_light.direction, hit_light.hitnormal));
                                hit_light.pdf = pdflast * hit_light.cos_in / (hit_light.hitdistance * hit_light.hitdistance);
                            /*****************************cos_inとpdfを求める*******************************************/

                            /*********************1つ前のhit_eyeにroulette_reverseを代入する**********/
                                subpath.hit_light[k2-1].roulette_reverse = p_eye;//この値を使うとは限らない 代わりに1を使うかもしれない
                            /**********************************************************************/
                    
                            subpath.addlight(hit_light, k2);
                            //この時点でhit.hitlight, hit.hitpos, hit.hitnormal, hit.hitdistance, hit.raydirection_in, hit.cos_in, hit.pdf, hit.hitmaterial, roulette, 前のroulette_reverseが保存されている
                            //しかし現時点ではhitLe, BSDF, cos_out, 前のpdf_reverse, 今のpdf_reverse, 今のroulette_reverseが求められないので後でサブパスの接続をするときに求める(配列には保存しない)
                            //pdflast, roulettelastは求める必要がない
                            break;
                        }


                    }
                }
            }
        }

        /***********************サブパスの個数がおかしい場合はcolの計算を中止*********************/
            if((subpath.num_eye < 0) || (subpath.num_light < 0))
            {
                std::cout << "エラー発生(num_eyeとnum_lightの少なくとも一方が負の値)" << std::endl;
                exit(1);
            }
            if((subpath.num_eye == 0) && (subpath.num_light = 0))
            {
                std::cout << "エラー発生(num_eyeとnum_lightの両方が0)" << std::endl;
                exit(1);
            }
        /**********************************************************************************/

        //colを計算する
        {
            /***********************************変数の宣言と初期化****************************************************************************************************/
                Hit hit;
                Ray ray;
                Vec3 raydirection_1;
                Vec3 BSDF;
                Vec3 throughput;
                Vec3 throughput_nee;
                Vec3 s, t, wi, wo;
                double cos; //衝突面またはセンサーとrayのなす角
                double misweight;

                Hit* phit_e = subpath.hit_eye;
                Hit* phit_l = subpath.hit_light;


                col = Vec3(0, 0, 0);        //初期化
            /******************************************************************************************************************************************************/

            /*********************************色のサンプルを取る******************************************************************************************************/
                if(iflight == 1)//光線サブパスを構築するなら
                {
                    /*********************最後に無限遠に飛んでいった場合(サブパスが構築できない場合)の寄与**********期待値は無限遠光源の光の積分になる*******************************************/
                        if(phit_e[subpath.num_eye-1].ifhit == 0)  //if else で完全に分けることができ,   下のmisweight計算で得られる積分には無限遠は含まれていないので足し算可能
                        {
                            col += phit_e[subpath.num_eye-1].hitLe * phit_e[subpath.num_eye-1].weight;
                        }
                        else
                        {
                            //加算しない
                        }            
                    /************************************************************************************************************************************/
                    
                    /*********************無限遠でない場合の寄与******************期待値は無限遠光源以外の光の積分になる******************************************/
                        int n_light;
                        int n_eye;
                        int min_light = 0;    //サブパスの最小の"個数"
                        int min_eye = snum+1; //サブパスの最小の"個数"

                        /************サブパスの個数がおかしい場合はcolの計算を中止*************/
                            if(subpath.num_eye < min_eye)
                            {
                                std::cout << "視線サブパスの個数が不足している" << std::endl;
                                exit(1);
                            }
                            if(subpath.num_light < min_light)
                            {
                                std::cout << "光線サブパスの個数が不足している" << std::endl;
                                exit(1);
                            }
                        /***************************************************************/

                        for(n_eye = min_eye; n_eye <= subpath.num_eye; n_eye++)//視線サブパスの個数を決める
                        {
                            for(n_light = min_light; n_light <= subpath.num_light; n_light++)//光線サブパスの個数を決める
                            {

                                /*****************無限遠に飛んでいった(サブパスが構築できない)場合を排除する***************************************************/
                                    if((n_eye > 0) && (phit_e[n_eye-1].ifhit == 0))//視線サブパスが存在してその端点が有限の位置に存在しなければ
                                    {
                                        //colを加算しない
                                        continue;
                                    }

                                    if((n_light > 0) && (phit_l[n_light-1].ifhit == 0))//光線サブパスが存在してその端点が有限の位置に存在しなければ
                                    {
                                        //colを加算しない
                                        continue;
                                    }
                                /************************************************************************************************/

                                //無限遠に飛んでいかなければ(サブパスが構築できれば)
        
                                /**************************サブパスの個数に応じてcolを計算する***************************/
                                    if((n_eye >= 1) && (n_light >= 1))//両サブパスがどちらも1つ以上の点を持つなら
                                    {
                                        /**************************変数名が長いので簡単にする***************************************/
                                        Hit& hit_end_e   = phit_e[n_eye   - 1];    //端点のhit  変数名が長いので簡単にする
                                        Hit& hit_end_l   = phit_l[n_light - 1];    //
                                        Hit& hit_end_1_e = phit_e[n_eye   - 2];      //端点の1つ前のhit  変数名が長いので簡単にする
                                        Hit& hit_end_1_l = phit_l[n_light - 2];      //
                                        /***************************************************************************************/

                                        /**************************光線サブパスと視線サブパスの端点が同じ場合を排除******************/
                                            if(hit_end_e.hitpos == hit_end_l.hitpos)//サブパスの端点が同じ点であれば 
                                            {
                                                //colは加算されない  理論的には起こる確率が0だからこの場合はサンプルにはほとんど影響しない
                                                continue;
                                            }
                                        /************************************************************************************/

                                        //サブパスの端点が重ならなければ

                                        /***********************サブパスの接続とcolの計算******************************/
                                        if//サブパスの端点の両方がスペキュラー以外の物体に衝突していれば 
                                        (
                                            (
                                                (
                                                    (n_eye   == 1)
                                                )  
                                                || 
                                                (
                                                    (   bool(hit_end_e.hitmaterial)              == 1)//ポインタがオブジェクトを所有しているか
                                                    &&    
                                                    (        hit_end_e.hitmaterial->isSpecular() == 0)//スペキュラーでないか
                                                )
                                            ) //スペキュラーでなければ(サブパスが1つしかない場合は判定が免除される) 
                                            &&   
                                            (
                                                (
                                                    (n_light == 1)
                                                )  
                                                || 
                                                (
                                                    (   bool(hit_end_l.hitmaterial)              == 1)//ポインタがオブジェクトを所有しているか
                                                    &&    
                                                    (        hit_end_l.hitmaterial->isSpecular() == 0)//スペキュラーでないか
                                                )
                                            ) //スペキュラーでなければ(サブパスが1つしかない場合は判定が免除される) 
                                        )
                                        {
                                            /*********************************サブパスの接続前に間に障害物がないか確認する**************************/
                                                raydirection_1 = normalize(hit_end_l.hitpos - hit_end_e.hitpos);//rayを向ける方向の単位ベクトル
                                                ray = Ray(hit_end_e.hitpos + _d_ * raydirection_1, raydirection_1 );
                                                
                                                hit.raydirection_in = ray.direction;//入射rayを保存する

                                                hit.set(0, 0, (hit_end_l.hitpos - hit_end_e.hitpos).length()-2*_d_);//初期値を2つの衝突点の距離とする 光線サブパスの衝突点に衝突しないように
                                                accel.intersects(ray, objects, hit);
                                                accel.intersects_nee2(ray, objects, hit, pdfsum_nee);// pdfsum_neeは使わない 
                                            /**************************************************************************************************/

                                            /*******************************colの加算を行う*******************************/
                                                if((hit.ifhit == 0))//視線サブパスの点と光線サブパスの点の間に障害物がなければ
                                                {
                                                    //サブパスの端点のBSDF, cos_out, pdf_reverse, roulette_reverse, 端点の1つ前のpdf_reverseが求められなかったので後でサブパスの接続をするときに求める(配列には保存しない)

                                                    double pdf_eye_reverse1;
                                                    double pdf_eye_reverse2;
                                                    double pdf_light_reverse1;
                                                    double pdf_light_reverse2;
                                                    double roulette_reverse_eye;
                                                    double roulette_reverse_light;

                                                    Vec3 BSDF_e;
                                                    Vec3 BSDF_l;

                                                    /********************視線(上), 光線(下)サブパスのcosを求める*************/
                                                        double&& cos1 = std::abs(dot(raydirection_1, hit_end_e.hitnormal));
                                                        double&& cos2 = std::abs(dot(raydirection_1, hit_end_l.hitnormal));
                                                    /*******************************************************************/     

                                                    /***************視線サブパスの端点で反射するrayのローカル座標を用いてのBSDF, 確率密度関数, ロシアンルーレットの確率を得る************/
                                                    
                                                        orthonormalbasis(hit_end_e.hitnormal, s, t);//視線サブパスの最後の点のローカル座標を得る

                                                        if(n_eye >= 2)//視線サブパスの個数が2以上なら
                                                        {
                                                            wi = world_to_local(-1*ray.direction, s, t, hit_end_e.hitnormal);            //光線サブパスの端点から視線サブパスの端点にrayが入射する方向のローカル座標           方向は実際に光が進む方向         
                                                            wo = world_to_local(-1*hit_end_e.raydirection_in, s, t, hit_end_e.hitnormal);//視線サブパスの端点から視線サブパスの端点の1つ前の点にrayが発射する方向のローカル座標  方向は実際に光が進む方向
                                                        
                                                            BSDF_e = hit_end_e.hitmaterial->getBSDF(-1*wi, -1*wo);//-wo, -wi    は視線rayの延長時のwo, wiに相当する  //

                                                            pdf_eye_reverse2 = hit_end_e.hitmaterial->getpdf_light(wi, wo)   * hit_end_1_e.cos_out / (hit_end_e.hitdistance * hit_end_e.hitdistance);// wi,  wo    は光線rayの延長時のwi, woに相当する  //視線サブパスの端点を反射するのでmaterialはどちらもhit_end_e.hitmaterial
                                                            pdf_light_reverse1 = hit_end_e.hitmaterial->getpdf_eye(-1*wi, -1*wo) * cos2                / (hit.hitdistance       * hit.hitdistance);  //-wo, -wi    は視線rayの延長時のwo, wiに相当する  

                                                            roulette_reverse_light = getroulette_eye(n_eye-1);//視線サブパスの端点から光線サブパスの端点への視線サブパスの延長が決定するロシアンルーレット(確率)
                                                        }
                                                        else//視線サブパスの個数が1なら
                                                        {
                                                            std::cout << "まだ実装していない" << std::endl;
                                                            exit(1);                                  

                                                            //wi = world_to_local(-1*ray.direction, s, t, hit_end_e.hitnormal);            //光線サブパスの端点から視線サブパスの端点(センサー)にrayが入射する方向のローカル座標           方向は実際に光が進む方向         

                                                            //BSDF_e = hit_end_e.BSDF;//センサーの感度 視線サブパスの最初の点のBSDFは例外的にセンサーの感度になる

                                                            //pdf_eye_reverse2 = 0;//使わない
                                                            //pdf_light_reverse1 = ;wiの方向に視線rayを発射する確率密度関数を求めて代入する

                                                            //roulette_reverse_light = p_eye;//今は定数だがrayの始点と発射方向に依存するようにするかもしれない
                                                            
                                                        }
                                                    
                                                    /********************************************************************************************************************/

                                                    /************************光線サブパスの端点で反射するrayのローカル座標を用いてのBSDF, 確率密度関数, ロシアンルーレットの確率を得る*************/
                                                    
                                                        orthonormalbasis(hit_end_l.hitnormal, s, t);//光線サブパスの最後の点のローカル座標を得る

                                                        if(n_light >= 2)//光線サブパスの個数が2以上なら
                                                        {
                                                            wi = world_to_local(hit_end_l.raydirection_in, s, t, hit_end_l.hitnormal);   //光線サブパスの端点の1つ前の点から光線サブパスの端点にrayが入射する方向のローカル座標           方向は実際に光が進む方向         
                                                            wo = world_to_local(-1*ray.direction, s, t, hit_end_l.hitnormal);            //光線サブパスの端点から視線サブパスの端点に向けて発射する方向のローカル座標           方向は実際に光が進む方向

                                                            
                                                            BSDF_l = hit_end_l.hitmaterial->getBSDF(wi, wo); // wi,  wo    は光線rayの延長時のwi, woに相当する         

                                                            pdf_light_reverse2 = hit_end_l.hitmaterial->getpdf_eye(-1*wi, -1*wo) * hit_end_1_l.cos_out / (hit_end_l.hitdistance * hit_end_l.hitdistance); //-wo, -wi     は視線rayの延長時のwo, wiに相当する
                                                            pdf_eye_reverse1 = hit_end_l.hitmaterial->getpdf_light(wi, wo)       * cos1                / (hit.hitdistance       * hit.hitdistance);       // wi,  wo     は光線rayの延長時のwi, woに相当する  //光線サブパスの端点を反射するのでmaterialはどちらもhit_end_l.hitmaterial
                                                            
                                                            roulette_reverse_eye = getroulette_light(n_light-1);//光線サブパスの端点から視線サブパスの端点への光線サブパスの延長が決定するロシアンルーレット(確率)
                                                        }
                                                        else//光線サブパスの個数が1なら
                                                        {
                                                            wo = world_to_local(-1*ray.direction, s, t, hit_end_l.hitnormal);//光線サブパスの端点(光源上)から視線サブパスの端点に向けて発射する方向のローカル座標           方向は実際に光が進む方向
                                                            BSDF_l = phit_l[0].BSDF;//光の放射の割合 光線サブパスの最初の点のBSDFは例外的に放射の割合になる

                                                            //pdf_light_reverse2は使われない
                                                            pdf_eye_reverse1 = phit_l[0].hitlight->getpdf_firstray(wo)   * cos1    / (hit.hitdistance * hit.hitdistance);//woの方向に光線rayを発射する確率密度関数を求めて面積に関するものに変換した代入する

                                                            roulette_reverse_eye = getroulette_light(1);//2つ目のサブパスが構築されるロシアンルーレット(確率)
                                                        }
                                                    
                                                    /******************************************************************************************************************************/


                                                    /*************************光線サブパス(の始点)から出る光を計算する***************************************************************/
                                                        throughput = hit_end_e.weight * hit_end_l.weight * BSDF_e * BSDF_l * cos1 * cos2 / (hit.hitdistance * hit.hitdistance);
                                                        misweight = subpath.misweight(n_eye, n_light, min_eye, min_light, pdf_eye_reverse1, pdf_eye_reverse2, pdf_light_reverse1, pdf_light_reverse2, roulette_reverse_eye, roulette_reverse_light);//ウェイトを計算する
                                                        col += misweight * throughput * phit_l[0].hitLe ;
                                                    /***********************************************************************************************************************/ 

                                                    
                                                }
                                                else//視線サブパスの点と光線サブパスの点の間に障害物があれば
                                                {
                                                    //colは加算されない
                                                }
                                            /***************************************************************************/
                                        }
                                        else//サブパスが1より大きくスペキュラーのものがあれば
                                        {
                                            //colは加算されない
                                        }
                                        /***************************************************************************/
                                    }
                                    else if((n_eye >= 2) && (n_light == 0))//光線サブパスを持たず, 視線サブパスを持つなら
                                    {
                                        //lightサブパスが0個のとき
                                        if(phit_e[n_eye-1].ifhit_light == 1)//視線サブパスがlight光源に当たった場合   
                                        {
                                            double pdf_eye_reverse1;
                                            double pdf_eye_reverse2;
                                            double roulette_reverse_eye;
                                            Vec3 BSDF_e;
                                            Vec3 BSDF_l;

                                            /************************光源から出た光の寄与を計算する***********************************/
                                                Hit& hit_end_e = phit_e[n_eye - 1];      //端点のhit  変数名が長いので簡単にする
                                                Hit& hit_end_1_e = phit_e[n_eye - 2];      //端点のhit  変数名が長いので簡単にする

                                                //サブパスの端点のBSDF, cos_out, pdf_reverse, roulette_reverse, 端点の1つ前のpdf_reverseが求められなかったので後でサブパスの接続をするときに求める(配列には保存しない)

                                                orthonormalbasis(hit_end_e.hitnormal, s, t);//視線サブパスの端点(今回は特別で光源上)のローカル座標
                                                wo = world_to_local(-1*hit_end_e.raydirection_in, s, t, hit_end_e.hitnormal);//光源面から発射されるrayの方向
                                                pdf_eye_reverse1 = hit_end_e.pdf_nee;//面積に関する
                                                pdf_eye_reverse2 = hit_end_e.hitlight->getpdf_firstray(wo) * hit_end_1_e.cos_out / (hit_end_e.hitdistance * hit_end_e.hitdistance);//面積に関する
                                                roulette_reverse_eye = getroulette_light(0); //光線サブパスの最初の点のロシアンルーレット(最初の点が生成される確率)

                                                misweight = subpath.misweight(n_eye, n_light, min_eye, min_light, pdf_eye_reverse1, pdf_eye_reverse2, 0, 0, roulette_reverse_eye, 0);//ウェイトを計算する
                                                throughput = hit_end_e.weight;
                                                col += misweight * throughput * hit_end_e.hitLe;//Leは例外的に光線サブパスの端点(この点は光源上にあるため)
                                            /************************************************************************************/
                                        }
                                        else//light光源以外に当たったら この範囲における積分は他に計算されることはないので直接足して(misweightは1)で良い
                                        {
                                            col += phit_e[n_eye-1].weight * phit_e[n_eye-1].hitLe;//視線サブパスの端点からの光を計算する
                                        }
                                    }
                                    else if((n_eye == 1) && (n_light == 0))
                                    {
                                        //colは加算されない
                                        //1回反射すらしないため
                                    }
                                    else if((n_eye == 0) && (n_light >= 2))//視線サブパスを持たず, 光線サブパスを持つなら
                                    {
                                        //視線サブパスが0個のとき
                                        //サブパスを逆算する処理を実装せよ
                                        //Cameraクラスに衝突判定用の物体をもたせるとよさそう

                                        std::cout << "まだ実装していない" << std::endl;
                                        exit(1);
                                    }
                                    else if((n_eye == 0) && (n_light == 1))
                                    {
                                        //colは加算されない
                                        //1回反射すらしないため
                                    }
                                    else if((n_eye < 0) || (n_light < 0))//少なくとも一方が負の値なら
                                    {
                                        std::cout << "サブパスの個数がありえない値(負の値)になった" << std::endl;
                                        exit(1);
                                    }
                                    else
                                    {
                                        std::cout << "まだ実装していない" << std::endl;
                                        exit(1);
                                    }
                                    
                                /***********************************************************************************/
                            }
                        }
                    /***********************************************************************************************************************************/
                }
                else//光線サブパスを構築しないなら
                {
                    /****視線サブパスが不足した場合はcolの計算を中止****/
                        if(subpath.num_eye < 2)//
                        {
                            std::cout << "視線サブパスの個数が不足している" << std::endl;
                            exit(1);
                        }
                    /********************************************/



                    //視線サブパスの点の個数が2個以上なら寄与を計算する
                    for(int k = 1; k <= (subpath.num_eye - 1); k++)//サブパスの最初から2番目から端点まで   k番の色を加算する
                    {
                        /**************************************neeの物体以外の色の加算(積分範囲の分割)********(k番目に格納されている色データにweightをかけて加算する)*********************/
                            //nee以外の処理(視線サブパスのなかで計算する)
                            //neeの物体に当たった場合の色は0に変えてあるので全てを足して構わない
                            col += phit_e[k].weight * phit_e[k].hitLe;//1番の点(視線パスが最初に衝突する点)から 視線パスの最後の点まで-----------[1]  [1]と[2]で積分範囲を分割している
                        /******************************************************************************************************************************************************/

                        /*******neeの物体の色の加算(積分範囲の分割)*****(k-1番の衝突点のデータを配列から受け取り, rayをランダムで選んだ光源の方向に飛ばし(これが実質k番の点となる), そしてそこの色を加算する*****/
                            hit = phit_e[k-1];//1つ前の衝突面の情報をhitにコピーする

                            if
                            (
                                (ifnee == 1)
                                        &&
                                (hit.ifhit == 1)
                                        &&
                                ((k-1) >= camera.getnum_subpath())//1つ前の衝突点がカメラの外にあれば
                                        &&  
                                (   (bool(hit.hitmaterial) == 1)  &&  (hit.hitmaterial->isSpecular() == 0)  )//スペキュラー面でないかどうか    左が偽なら右が実行されないので不正アクセスの心配がない    
                            ) //ifneeが1で, 1つ前の衝突点が有限の位置にあり, さらに スペキュラー面でなく, カメラの外にあれば必ずneeを行う
                            {
                                n = -sgn(dot(hit.hitnormal, hit.raydirection_in)) * hit.hitnormal;//法線ベクトルを入射ベクトルとの内積が負になる方向に補正する


                                if(subpath.hit_light[0].hitpos == hit.hitpos)//rayを向ける方向が前回の衝突点と等しいのなら
                                {
                                    continue;//バグ回避 今回の色情報は無視し, 次の点でneeを行う
                                }

                                //rayを向ける方向が前回の衝突点と異なるのなら
                            
                                raydirection_1 = normalize(subpath.hit_light[0].hitpos - hit.hitpos);//neeの物体への方向の単位ベクトル
                                
                                if
                                (
                                    (hit.hitmaterial->isTransparent() == 0)//Materialが透過せずに反射するもの
                                    && 
                                    (dot(n, raydirection_1) < 0)//rayが発射面の裏側へ突き抜ける
                                )
                                {
                                    col += Vec3(0, 0, 0); //加算しない(ありえない方向にrayを飛ばしてはいけない)                
                                    continue; //neeをやめ、次の衝突計算に進む
                                }

                                //Materialが透過するもの もしくは rayが発射面の裏側へ突き抜けないなら

                                ray = Ray(hit.hitpos + raydirection_1 * _d_, raydirection_1);//neeの物体へのrayを得る


                                /**************************************入射rayと出射rayからBSDFとcosを求める********************************************/
                                    orthonormalbasis(hit.hitnormal, s, t);      //hitnormalが必要                  //正規直行系を得る
                                    wi = world_to_local(ray.direction, s, t, hit.hitnormal);  //hitnormalが必要  //ローカル座標の方向を得る(反射後の方向)
                                    wo = world_to_local(hit.raydirection_in, s, t, hit.hitnormal); //ローカル座標の方向を得る(反射前の方向)
                                    BSDF = hit.hitmaterial->getBSDF(wi, wo);                //BSDFを得る//BSDFを得るにはwi(反射後の方向), wo(反射前の方向)が必要な場合がある
                                    cos = std::abs(costheta(wi));                           //  発射rayのcosの値を得る
                                /********************************************************************************************************************/


                                /*******************neeの物体との衝突を行い, pdfsum_nee(rayの指す方向の確率密度関数の総和)を求める*********************/
                                    hit.set(0, 0, hitdistance0);//初期化
                                    accel.intersects_nee2(ray, objects, hit, pdfsum_nee); //neeの物体で衝突判定 ここでpdfsum_neeを求める
                                /*************************************************************************************************/

                                //先程求めたBSDF, cos, pdfsum_neeと配列に格納されているウェイトを用いて今回のウェイトを求める

                                throughput_nee = phit_e[k-1].weight * (BSDF * cos / (pdfsum_nee * phit_e[k].roulette)); //throughput_neeを求める　ワールドでもローカルでもcosは不変   配列はk-1番であることに注意せよ nee以外の光と同様に確率がphit_e[k].rouletteだからphit_e[k].rouletteで割る
                                
                                /***********************衝突を計算した後でcolを求める******************************/
                                    //先程求めたifhitの値を確認する
                                    if (hit.ifhit == 1) //neeの物体に衝突した
                                    {
                                        hit.setbool(0, 0);//ifhitを初期化  hitdistanceを初期化してはいけない
                                        accel.intersects(ray, objects, hit); //nee以外の物体で衝突判定

                                        if (hit.ifhit == 1) //最も近くにnee以外の物体がある場合
                                        {
                                            
                                            col += Vec3(0, 0, 0); //加算しない
                                            continue;
                                            //加算せずに次の計算に進む
                                        }
                                        else //最も近くにneeの物体がある場合
                                        {
                                            if((exp10(-15) < pdfsum_nee)&&(pdfsum_nee < exp10(15)))//pdfsum_neeが小さすぎず大きすぎなければ
                                            {
                                                /******************hit.hitLeを求める******************/
                                                orthonormalbasis(hit.hitnormal, s, t);
                                                wo = world_to_local(ray.direction, s, t, hit.hitnormal);
                                                hit.hitLe = hit.hitlight->Le(wo);
                                                /****************************************************/

                                                col += throughput_nee  * hit.hitLe;//----------------------------------------------------------[2]  [1]と[2]で積分範囲を分割している
                                                continue;
                                                //次の計算に進む
                                            }
                                            else//pdfsum_neeが大きすぎるか小さすぎれば //
                                            {
                                                col += Vec3(0,0,0);//値を無視する  pdfsum_neeが非常に大きい領域の立体角は非常に小さいと考えられ、そしてその小さい立体角で放射輝度を積分してもほぼ0であると考えられるため
                                                    //もしそのまま加算すると計算での誤差が大きくなってしまうと考えられる(pdfsum_neeが非常に大きい領域の立体角は非常に小さいと考えられ,rayが少しずれただけで計算結果が大きくずれてしまう可能性がある)
                                                continue;
                                                //次の計算に進む
                                            }                              
                                        }
                                    }
                                    else //neeの物体に当たらなかったら (誤差などであたらない可能性がある)
                                    {
                                        col += Vec3(0, 0, 0); //加算しない 
                                        continue; 
                                        //加算せずに次の計算に進む
                                    }
                                /******************************************************************************/
                            }
                            else //上記を満たさない(ifneeが0もしくはSpecularもしくはサブパスの端点がカメラの内部にある)ならneeはしない(してはいけない)
                            {
                                //今回はneeはしない
                                //サブパスの長さを変更して次の計算に進む
                                continue;
                            }
                        /************************************************************************************************************************************************************/
                    }
                }
            /******************************************************************************************************************************************************/
        }
        data         [width * j + i] += col;       //和を取る
        data2        [width * j + i] += col * col; //2乗和を取る
        addednum_each[width * j + i] += 1;         //これまでに取ったサンプル数を加算する
    }

    void Image::addSample(const Camera& camera, const Objects &objects, const Sky& sky, int const& threadnumber)//サンプルを加算する　ついでにサンプルが加算された個数も足す
    {


        /************************バグ回避*********************************************************/
            if(num_of_samples < 0)
            {
                std::cout << "サンプル数が負の数なので不適切" << std::endl;
                exit(1);
            }

            if((objects.objectnumbersum_nee == 0) && (ifnee == 1))//neeの物体が存在せず、ifneeが1ならば
            {
                std::cout << "neeの物体がないのならifneeを0にせよ" << std::endl; 
                exit(1);//中断
            }

            if((ifnee == 1) && (iflight == 1))
            {
                std::cout << "ifneeとiflightが共に1であってはならない" << std::endl;
                exit(1);
            }
        /****************************************************************************************/

        /**********************視線サブパスを開始するセンサー上の点を選び, サンプルを取る*********************************************/
            Subpath subpath(RAY_MAX);//サブパスの生成
            Vec3 sample;//サンプルを格納する

            for (int j = height * threadnumber / threadsum; j < height * (threadnumber + 1) / threadsum; j++)
            {
                /********************上からj+1番目の行の横1行に沿う点それぞれから視線サブパスを開始する***********************************************************************/
                    for (int i = 0; i < width; i++) //横幅になるまで
                    {
                        /*****************指定された回数サンプルを取る**************************************/
                            for (int l = 0; l < num_of_samples_each[width * j + i]; l++) //サンプル数
                            {
                                subpath.reset();//サブパスの個数をリセット(0に)する
                                getSample(i, j, camera, objects, sky, subpath, threadnumber);//サンプルを1回取る
                            }
                        /******************************************************************************/
                        
                        samplecounter[threadnumber] += num_of_samples_each[width * j + i];//それぞれのスレッドでサンプルを何個取り終わったかの個数を更新する  これは何パーセント終わったかを求めるのに使われる
                    }
                /***************************************************************************************************************************************************/
            }
        /********************************************************************************************************************/
    }

    void Image::CalculateColor(double const& a_) const
    {
        std::cout << "▼色データの修正開始▼" << std::endl; //表示

        /******変数の宣言と初期化*************/
            int counter = 0;
            int counter_neg = 0;
            int counter_inf = 0;
            int counter_nan = 0;
        /**********************************/

        /***********2乗平均を求め, a_倍し(a_はcinで入力される), そして, inf, nan, 負の値, 255を超える値を除去する**************************/
            for (int j = 0; j < height; j++)
            {
                for (int i = 0; i < width; i++)
                {
                        data_temp[width * j + i] = data[width * j + i]/addednum_each[width * j + i] * a_; //平均を取ってその後でa_倍する
                        data_temp[width * j + i]._0to255(counter, counter_neg, counter_inf, counter_nan); //0から255までにする
                }
            }
        /***********************************************************************************************************************/
        std::cout << "▲色データの修正終了▲" << std::endl;       //表示

        /*********************************255以上の値, 負の値, nan, inf の個数と割合(%)を表示する*************************************************************************/
            std::cout << "255以上の値は" << counter << "個" << '(' << counter * 100.0 / (width * height) << '%' << ')' << "あった" << std::endl;     //表示
            std::cout << "負の値は" << counter_neg << "個" << '(' << counter_neg * 100.0 / (width * height) << '%' << ')' << "あった" << std::endl;  //表示
            std::cout << "nanの値は" << counter_nan << "個" << '(' << counter_nan * 100.0 / (width * height) << '%' << ')' << "あった" << std::endl; //表示
            std::cout << "infの値は" << counter_inf << "個" << '(' << counter_inf * 100.0 / (width * height) << '%' << ')' << "あった" << std::endl; //表示
            std::cout << std::endl; //1行空ける
        /**********************************************************************************************************************************************************/
    }

    #define digitnum_image 4//0埋めで使う 桁数

    void Image::Output_To_Ppm(const int &i) const //画像を1つ出力
    {
        std::cout << "▼書き込み開始▼" << std::endl; //表示
        std::cout << "ファイル名は" << "image-"s + _0fill(i, digitnum_image).str() + std::to_string(i) + ".ppm"s << std::endl;


        std::ofstream file("image-"s + _0fill(i, digitnum_image).str() + std::to_string(i) + ".ppm"s, std::ios::binary); //ファイル名を設定してファイルを作成

        /******************バグ回避**********************************/
            if(!file.is_open())
            {
                std::cout << "ファイルを作成できなかった" << std::endl;
                exit(1);
            }
        /**********************************************************/

        char c[10];//配列
        int n1, n2;//文字数を格納する

        /*************************ppmファイルの先頭の文字列を書き込む*************************************************/
            c[0] = 'P'; c[1] = '6'; c[2] = '\n';
            file.write(c, 3);                                   //書き込む "P6\n"

            n1 = sprintf(c, "%d", width);  c[n1] = ' ';  n2 = sprintf(c + n1 + 1, "%d", height);  c[n1+n2+1] = '\n';
            file.write(c, n1+n2+2);                             //書き込む "width height\n"

            n1 = sprintf(c, "%d", 255); c[n1] = '\n';
            file.write(c, n1+1);                                 //書き込む "255\n"
        /********************************************************************************************************/

        /***************************RGBを書き込む****************************************************************/
            for (int j = 0; j < height; j++) //ppmへの書き込み
            {
                for (int i = 0; i < width; i++)
                {
                    c[0] = (int)(data_temp[width * j + i]).x;  c[1] = (int)(data_temp[width * j + i]).y;  c[2] = (int)(data_temp[width * j + i]).z;

                    file.write(c, 3);  //色を書き込む
                }

                std::cout << (100 * (j+1) / height) << '%' << "\r" << std::flush; //進行状況を表示

            }
        /********************************************************************************************************/
        file.close(); //ファイルを閉じる

        std::cout << "▲書き込み終了▲" << std::endl; //表示
    }

    void Image::Output_Over255_To_Ppm(const int &i) const //画像を1つ出力
    {
        std::cout << "▼書き込み開始▼" << std::endl; //表示

        std::ofstream file("Over255-"s + _0fill(i, digitnum_image).str() + std::to_string(i) + ".ppm"s, std::ios::binary); //ファイル名を設定してファイルを作成


        /******************バグ回避**********************************/
            if(!file.is_open())
            {
                std::cout << "ファイルを作成できなかった" << std::endl;
                exit(1);
            }
        /**********************************************************/

        char c[10];//配列
        int n1, n2;//文字数を格納する

        /*************************ppmファイルの先頭の文字列を書き込む*************************************************/
            c[0] = 'P'; c[1] = '6'; c[2] = '\n';
            file.write(c, 3);                                   //書き込む "P6\n"

            n1 = sprintf(c, "%d", width);  c[n1] = ' ';  n2 = sprintf(c + n1 + 1, "%d", height);  c[n1+n2+1] = '\n';
            file.write(c, n1+n2+2);                             //書き込む "width height\n"

            n1 = sprintf(c, "%d", 255); c[n1] = '\n';
            file.write(c, n1+1);                                 //書き込む "255\n"
        /*******************************************************************************************************/

        /***************************RGBを書き込む****************************************************************/
            for (int j = 0; j < height; j++) //ppmへの書き込み
            {
                for (int i = 0; i < width; i++)
                {
                    /*************RGBのうちに255以上の値があれば白色, 全てが240未満なら黒色, 最も明るい色が240以上255未満なら青色を出力する*******/
                        if((data_temp[width * j + i]) < Vec3(240,240,240))//rgbが240未満なら
                        {
                            c[0] = c[1] = c[2] = 0;
                        }
                        else if(data_temp[width * j + i] < Vec3(255,255,255))//rgbが240以上かつ255未満なら
                        {
                            c[0] = c[1] = 0;  c[2] = 255;
                        }
                        else//rgbが255以上なら
                        {
                            c[0] = c[1] = c[2] = 255;
                        }
                    /***************************************************************************************************************/

                    file.write(c, 3);  //色を書き込む
                }

                std::cout << (100 * (j + 1) / height) << '%' << "\r" << std::flush; //進行状況を表示
            }
        /******************************************************************************************************/

        file.close(); //ファイルを閉じる

        std::cout << "▲書き込み終了▲" << std::endl; //表示
    }

    #undef digitnum_image

    void Image::CalculateSD_average(double const& a_) const
    {

        //この時点でdata2にはサンプルの2乗和が格納されている
        //dataにはサンプルの和が格納されている
        //data_tempは計算した値を格納するために用いる
        //こうすれば更にサンプルを追加しても追加後の平均や分散を求めることができる

        /***********SDデータ(標準偏差)を求める************/
            for (int j = 0; j < height; j++)
            {
                for (int i = 0; i < width; i++)
                {
                    //1行目: RGBの各々で 2乗平均 - 平均*平均を計算して, 個々のサンプルの標本分散を求める
                    //2行目: 標本分散に((サンプル数)/(サンプル数 - 1))をかけることで個々の不偏分散を求める
                    //3行目: 平均値の不偏分散を求めるには個々の不偏分散をサンプル数で割れば良い   なぜなら　　 独立なサンプルの和の分散は各々の分散の和  サンプルのn倍の分散は元の分散のnの2乗倍になるから
                    //4行目: 平均値の不偏分散は平均値の母分散の推定値に等しいとすると平均値のSD(標準偏差)はその平方根になる
                    //5行目: a_倍(定数倍)する

                    /************************************************************************************************************************************************************************************************************************/
                    data_temp[width * j + i] = data2[width * j + i]/addednum_each[width * j + i] - (data[width * j + i]/addednum_each[width * j + i]) * (data[width * j + i]/addednum_each[width * j + i]); //個々の分散(標本分散)を求める
                    data_temp[width * j + i] *= addednum_each[width * j + i] / (double)(addednum_each[width * j + i] - 1);                   //個々の不偏分散を求める
                    data_temp[width * j + i]  /=  (double)addednum_each[width * j + i];                                      //平均値の分散を求める
                    data_temp[width * j + i] = sqrt(data_temp[width * j + i]);                                      //平均値のSDを求める
                    data_temp[width * j + i] *= a_;                                                              //定数倍する
                    /************************************************************************************************************************************************************************************************************************/
                    
                }
            }
        /*********************************************/

        std::cout << "▼SDデータの修正開始▼" << std::endl; //表示

        /************************************SDデータ(標準偏差)を修正する********************************************************************************************/

            /******変数の宣言と初期化**********************************/
                Vec3 counter;//255を超える値の個数      //RGBの各々の場合がVec3のxyzの各々に格納される
                Vec3 counter_inf;//infの個数          //
                Vec3 counter_nan;//nanの個数          //
                Vec3 counter_neg;//負の数の個数        //
            /*******************************************************/

            /**************inf, nan, 負の値, 255を超える値を除去する***ついでにその個数も数える**************/
                for (int j = 0; j < height; j++)
                {
                    for (int i = 0; i < width; i++)
                    {
                            if (data_temp[width * j + i].x < 0)
                            {
                                data_temp[width * j + i].x = 0;
                                counter_neg.x++;
                            }

                            if (data_temp[width * j + i].y < 0)
                            {
                                data_temp[width * j + i].y = 0;
                                counter_neg.y++;
                            }

                            if (data_temp[width * j + i].z < 0)
                            {
                                data_temp[width * j + i].z = 0;
                                counter_neg.z++;
                            }

                            if (isnan(data_temp[width * j + i].x))
                            {
                                data_temp[width * j + i].x = 0;
                                counter_nan.x++;
                            }

                            if (isnan(data_temp[width * j + i].y))
                            {
                                data_temp[width * j + i].y = 0;
                                counter_nan.y++;
                            }

                            if (isnan(data_temp[width * j + i].z))
                            {
                                data_temp[width * j + i].z = 0;
                                counter_nan.z++;
                            }

                            if (isinf(data_temp[width * j + i].x))
                            {
                                data_temp[width * j + i].x = 255;
                                counter_inf.x++;
                            }

                            if (isinf(data_temp[width * j + i].y))
                            {
                                data_temp[width * j + i].y = 255;
                                counter_inf.y++;
                            }

                            if (isinf(data_temp[width * j + i].z))
                            {
                                data_temp[width * j + i].z = 255;
                                counter_inf.z++;
                            }

                            if (data_temp[width * j + i].x > 255)
                            {

                                data_temp[width * j + i].x = 255;

                                counter.x++;
                            }

                            if (data_temp[width * j + i].y > 255)
                            {

                                data_temp[width * j + i].y = 255;

                                counter.y++;
                            }

                            if (data_temp[width * j + i].z > 255)
                            {

                                data_temp[width * j + i].z = 255;

                                counter.z++;
                            }
                        
                    }
                }
            /****************************************************************************************/

        /*********************************************************************************************************************************************************/

        std::cout << "▲修正終了▲" << std::endl;
        std::cout << std::endl; //1行空ける

        /**************************************255以上の値, 負の値, nan, inf の個数と割合(%)を表示する********************************/
            std::cout << "255以上の値は" << counter << "個" << counter * 100 / (width * height) << "%あった" << std::endl;     //表示
            std::cout << "負の値は" << counter_neg << "個" << counter_neg * 100 / (width * height) << "%あった" << std::endl;  //表示
            std::cout << "nanの値は" << counter_nan << "個" << counter_nan * 100 / (width * height) << "%あった" << std::endl; //表示
            std::cout << "infの値は" << counter_inf << "個" << counter_inf * 100 / (width * height) << "%あった" << std::endl;
        /***********************************************************************************************************************/

        std::cout << std::endl; //1行空ける

        std::cout << "平均値のSDは大きくても" << 255 / a_ << "程度だと推測される" << std::endl;   //うまく255の値が最大値になるようにa_を決めれば 最大値は255/a_になる

        std::cout << std::endl; //1行空ける
    }

    void Image::CalculateSD_each1(double const& a_) const//個々のサンプルのSDデータ(標準偏差)を求める
    {

        /***********SDデータ(標準偏差)を求める************/
            for (int j = 0; j < height; j++)
            {
                for (int i = 0; i < width; i++)
                {
                    //1行目: RGBの各々で 2乗平均 - 平均*平均を計算して, 個々のサンプルの標本分散を求める
                    //2行目: 標本分散に((サンプル数)/(サンプル数 - 1))をかけることで個々の不偏分散を求める
                    //3行目: 個々のサンプルの不偏分散は個々のサンプルの母分散の推定値に等しいとすると平均値のSD(標準偏差)はその平方根になる
                    //4行目: a_倍(定数倍)する

                    data_temp[width * j + i] = data2[width * j + i]/addednum_each[width * j + i] - (data[width * j + i]/addednum_each[width * j + i]) * (data[width * j + i]/addednum_each[width * j + i]); //個々の分散(標本分散)を求める
                    data_temp[width * j + i] *= addednum_each[width * j + i] / (double)(addednum_each[width * j + i] - 1);                   //個々の不偏分散を求める
                    data_temp[width * j + i] = sqrt(data_temp[width * j + i]);                                      //個々のSDを求める
                    data_temp[width * j + i] *= a_;                                                              //定数倍する
                }
            }
        /*********************************************/

        std::cout << "▼SDデータの修正開始▼" << std::endl; //表示
        /******************************SDデータ(標準偏差)を修正する*********************/

            /******変数の宣言と初期化***********/
                Vec3 counter;
                Vec3 counter_inf;
                Vec3 counter_nan;
                Vec3 counter_neg;
            /********************************/

            /**************inf, nan, 負の値, 255を超える値を除去する***ついでにその個数も数える**************/
                for (int j = 0; j < height; j++)
                {
                    for (int i = 0; i < width; i++)
                    {

                        if (data_temp[width * j + i].x < 0)
                        {
                            data_temp[width * j + i].x = 0;
                            counter_neg.x++;
                        }

                        if (data_temp[width * j + i].y < 0)
                        {
                            data_temp[width * j + i].y = 0;
                            counter_neg.y++;
                        }

                        if (data_temp[width * j + i].z < 0)
                        {
                            data_temp[width * j + i].z = 0;
                            counter_neg.z++;
                        }

                        if (isnan(data_temp[width * j + i].x))
                        {
                            data_temp[width * j + i].x = 0;
                            counter_nan.x++;
                        }

                        if (isnan(data_temp[width * j + i].y))
                        {
                            data_temp[width * j + i].y = 0;
                            counter_nan.y++;
                        }

                        if (isnan(data_temp[width * j + i].z))
                        {
                            data_temp[width * j + i].z = 0;
                            counter_nan.z++;
                        }

                        if (isinf(data_temp[width * j + i].x))
                        {
                            data_temp[width * j + i].x = 255;
                            counter_inf.x++;
                        }

                        if (isinf(data_temp[width * j + i].y))
                        {
                            data_temp[width * j + i].y = 255;
                            counter_inf.y++;
                        }

                        if (isinf(data_temp[width * j + i].z))
                        {
                            data_temp[width * j + i].z = 255;
                            counter_inf.z++;
                        }

                        if (data_temp[width * j + i].x > 255)
                        {

                            data_temp[width * j + i].x = 255;

                            counter.x++;
                        }

                        if (data_temp[width * j + i].y > 255)
                        {

                            data_temp[width * j + i].y = 255;

                            counter.y++;
                        }

                        if (data_temp[width * j + i].z > 255)
                        {

                            data_temp[width * j + i].z = 255;

                            counter.z++;
                        }
                    }
                }
            /****************************************************************************************/

        /*********************************************************************************/
        std::cout << "▲修正終了▲" << std::endl;   //表示


        /**************************************255以上の値, 負の値, nan, inf の個数と割合(%)を表示する********************************/
            std::cout << "255以上の値は" << counter << "個" << counter * 100 / (width * height) << "%あった" << std::endl;     //表示
            std::cout << "負の値は" << counter_neg << "個" << counter_neg * 100 / (width * height) << "%あった" << std::endl;  //表示
            std::cout << "nanの値は" << counter_nan << "個" << counter_nan * 100 / (width * height) << "%あった" << std::endl; //表示
            std::cout << "infの値は" << counter_inf << "個" << counter_inf * 100 / (width * height) << "%あった" << std::endl;
        /***********************************************************************************************************************/

        std::cout << std::endl; //1行空ける

        std::cout << "個々のSDは大きくても" << 255 / a_ << "程度だと推測される" << std::endl; //うまく255の値が最大値になるようにa_を決めれば 最大値は255/a_になる

        std::cout << std::endl; //1行空ける
    }

    void Image::OutputSD_To_Ppm(const int &i) const //画像を3つ出力
    {
        std::cout << "▼SDデータの書き込み開始▼" << std::endl; //表示

        writeSD_to_ppm("SDx" + std::to_string(i), 0);
        writeSD_to_ppm("SDy" + std::to_string(i), 1);
        writeSD_to_ppm("SDz" + std::to_string(i), 2);

        std::cout << "▲書き込み終了▲" << std::endl; //表示
    }

    void Image::writeSD_to_ppm(std::__cxx11::string const& name, int const& num) const//numが0, 1, 2の場合それぞれx, y, zを表す
    {

        if((num != 0) && (num != 1) && (num != 2))
        {
            std::cout << "writeSD_to_ppmのnumでエラー発生" << std::endl;
            std::cout << "numが0, 1, 2の場合それぞれx, y, zを表すことになっているのだが, 今回はnumの値が" << num << "であり, これは0, 1, 2のいずれでもないのでx, y, zを表せない" << std::endl;
            exit(1);
        }

        std::ofstream file(name + ".ppm"s, std::ios::binary);          //ファイル名を設定してファイルを作成


        /****************バグ回避********************************/
            if(!file.is_open())
            {
                std::cout << "ファイルを作成できなかった" << std::endl;
                exit(1);
            }
        /*******************************************************/

        char c[10]; 
        int n1, n2;

        /*************************ppmファイルの先頭の文字列を書き込む*************************************************/
            c[0] = 'P'; c[1] = '6'; c[2] = '\n';
            file.write(c, 3);                                   //書き込む "P6\n"

            n1 = sprintf(c, "%d", width);  c[n1] = ' ';  n2 = sprintf(c + n1 + 1, "%d", height);  c[n1+n2+1] = '\n';
            file.write(c, n1+n2+2);                             //書き込む "width height\n"

            n1 = sprintf(c, "%d", 255); c[n1] = '\n';
            file.write(c, n1+1);                                 //書き込む "255\n"
        /********************************************************************************************************/


        /***************************RGBを書き込む****************************************************************/
            for (int j = 0; j < height; j++) //ppmへの書き込み
            {
                for (int i = 0; i < width; i++)
                {
                    c[0] = c[1] = c[2] = (int)(data_temp[width * j + i].a[num]);
                    
                    file.write(c, 3);
                }
                std::cout << (100 * (j + 1) / height) << '%' << "\r" << std::flush; //進行状況を表示
            }
        /*******************************************************************************************************/

        file.close(); //ファイルを閉じる

        std::cout << "RGB"[num] << "終了" << std::endl; //numの値によってR, G, Bを決める
    }

    void Image::setNumber_of_samples_each(bool const& abc)//個々のサンプル数を設定して総和を求めてnum_of_samples_pixelsに代入する
    {

        if(abc == 1)//個々の分散に比例させてサンプルを取るなら
        {
            for (int i = 0; i < width * height; i++)//個々のSDを求め、負の値,nanを0にする
            {
                /***************************************個々のSDを求める*********************************************************************/
                    data_temp[i] = data2[i]/addednum_each[i] - (data[i]/addednum_each[i]) * (data[i]/addednum_each[i]); //個々の分散(標本分散)を求める
                    data_temp[i] *= addednum_each[i] / (double)(addednum_each[i] - 1);                   //個々の不偏分散を求める
                    data_temp[i] = sqrt(data_temp[i]);                                      //個々のSDを求める 
                /**************************************************************************************************************************/ 

                /***負の値,nanを0にする*****/
                    if (data_temp[i].x < 0)
                    {
                        data_temp[i].x = 0;
                    }

                    if (data_temp[i].y < 0)
                    {
                        data_temp[i].y = 0;
                    }

                    if (data_temp[i].z < 0)
                    {
                        data_temp[i].z = 0;
                    }

                    if (isnan(data_temp[i].x))
                    {
                        data_temp[i].x = 0;
                    }

                    if (isnan(data_temp[i].y))
                    {
                        data_temp[i].y = 0;
                    }

                    if (isnan(data_temp[i].z))
                    {
                        data_temp[i].z = 0;
                    }
                /*************************/

                /************************************RGBの少なくとも1つがinfならサンプルを取らない************************************************/
                    if(isinf(data_temp[i].x) || isinf(data_temp[i].y) || isinf(data_temp[i].z))//RGBの少なくとも1つがinfなら
                    {
                        std::cout << "infを発見したので個々の分散に比例させてサンプルを取ることができないので次に取るサンプルの数を0にする" << std::endl;

                        for(int i = 0; i < width * height; i++)
                        {
                            num_of_samples_each[i] = 0;//サンプルを取ることができないので0で埋める(実質サンプルを取らないことになる)
                        }
                        num_of_samples_pixels = 0;//総和を代入する
                    }
                /**************************************************************************************************************************/
            }

            double sum = 0;//これから総和を求めるので0を代入する
            /********************RGBの分散の和を求める****************************/
                for(int i = 0; i < width * height; i++)//rgbの分散の和を求める
                {
                    sum += data_temp[i].length2();
                }
            /******************************************************************/

            /**************個々の分散に比例させてサンプル数を決める(サンプル数の合計が大体num_of_samples枚分になるように)***************/
                for(int i = 0; i < width * height; i++)
                {
                    num_of_samples_each[i] = (data_temp[i].length2() * (long)num_of_samples * (long)width * (long)height  / sum);
                }
            /**************************************************************************************************************/

            long sum_ = 0;//これから総和を求めるので0を代入する
            /****************進行状況を計算するためにサンプル数の総和を求める必要がある****(サンプル数がnum_of_samples枚分から微妙にずれる可能性があるため)************************/
                for(int i = 0; i < width * height; i++)//サンプル数の総和を求める
                {
                    sum_ += num_of_samples_each[i];
                }
                num_of_samples_pixels = sum_;//総和を代入する(サンプル数がnum_of_samples枚分から微妙にずれる可能性がある)
            /*****************************************************************************************************************************************************/
        }
        else//一様にサンプリングするなら
        {

            for(int i = 0; i < width * height; i++)//各々のサンプル数を一様にnum_of_samplesにする
            {
                num_of_samples_each[i] = (long)num_of_samples;
            }
            num_of_samples_pixels = (long)num_of_samples * (long)width * (long)height;//総和を代入する  一様なので掛け算で良い
        }
    }

    double Image::getpercent()const//終了のパーセントを求める
    {
        double sum = 0;

        for (int i = 0; i < threadsum; i++) //全てのスレッドの終了サンプル数の総和を求める
        {
            sum += samplecounter[i];
        }
        return  sum * 100 /(double)(num_of_samples_pixels);//終了のパーセントを求める
    }

    #define digitnum_time 2//printpercentで残り時間を表示するときに何桁に合わせるか

    void Image::printpercent()const //全てのスレッド全体の終了のパーセントと(サンプル取得完了までの)推定残り時間を表示する
    {
        double timeunit = 1;//推定残り時間を求めるときのpercent1, percent2を求める時間の差
        std::cout << "▼色の取得開始▼" << std::endl; //表示
        double percent0, percent1;//終了パーセントを格納する
        int remainingtime;//推測される残り時間
        
        while (getpercent() < 100)//サンプルを取り終わっていなければループ
        {
            percent0 = getpercent(); //timeunit秒前の終了率を保存

            usleep(1000000 * timeunit);             //timeunit秒間待機

            percent1 = getpercent();   //timeunit秒後の終了率を得る
            remainingtime = (100 - percent1) / ((percent1 - percent0)/timeunit); //推定残り時間を求める   (残り何%か)/(%減少速度)

            //表示


            /******************************推定残り時間を表示する***********************************************************************************/
            if(percent1 == percent0)
            {
                std::cout << "推定残り時間" << ' ' << "予想不可"
                << "  " << percent1 << "%終了"
                << "　　　　　　　　　　　"
                << "\r" << std::flush;
            }
            else
            {
                int hour;
                int minute;
                double second;
                hour_mimnute_second(remainingtime, hour, minute, second);//秒数remainingtimeから時間分秒を求めてhour, minute, secondに格納する

                std::cout << "推定残り時間" << ' ' << hour << "時間" << _0fill(minute, digitnum_time).str() + std::to_string(minute) << "分" << _0fill(second, digitnum_time).str() + std::to_string((int)second) << "秒"
                << "  " << percent1 << "%終了"
                << "　　　　　　　　　　　"
                << "\r" << std::flush;
            }
            /***********************************************************************************************************************************/
            
        }
        std::cout << std::endl << "▲色の取得終了▲" << std::endl << std::endl; //表示
    }

    #undef digitnum_time

    void Image::timecounter()//サンプル取得開始から完了までの時間を計測する  スレッドで並列処理する
    {
        double timeunit = 0.01;//時刻を数える細かさ
        double seconds_1_sam;//今回の画像のサンプルをを1枚分取るのにかかった時間
        double seconds_1_sam_1000_1000;//1000×1000のサンプルを1枚分取る場合にかかる時間
        

        /******ここにhour_minute_secondで求めた時間, 分, 秒を代入する**********/
            int hour;
            int minute;
            double second;
        /*****************************************************************/

        /**********画像をサンプリングしている間にループ(時間を計測)し続ける********/
            while (getpercent() < 100)//100%になるまで時間を数える
            {
                usleep(1000000*timeunit);//timeunit秒間待機
                seconds += timeunit;//加算
            }
        /*****************************************************************/
        usleep(100000);//0.1秒間待機 他のスレッドが文字を出力するかもしれないため

        /***********1枚分の時間, 1000×1000の場合の1枚分の時間を計算する****************************/
            seconds_1_sam = seconds / (num_of_samples_pixels/(double)(width * height));
            seconds_1_sam_1000_1000 = seconds_1_sam * (1000.0 * 1000.0 / (width * height));
        /*****************************************************************************/

        /**************サンプル取得開始から完了までの時間を表示*****************************/
            hour_mimnute_second(seconds, hour, minute, second);
            std::cout << "所要時間" << std::endl 
            << hour << "時間" << minute << "分" << second << "秒" << std::endl << std::endl;
        /*****************************************************************************/
        
        /************今回の画像のサンプルをを1枚分取るのにかかった時間を表示*******************/
            hour_mimnute_second(seconds_1_sam, hour, minute, second);
            std::cout << "1サンプルあたりの平均所要時間" << std::endl 
            << hour << "時間" << minute << "分" << second << "秒" << std::endl << std::endl;
        /*****************************************************************************/

        /***************1000×1000のサンプルを1枚分取る場合にかかる時間を表示*****************/
            hour_mimnute_second(seconds_1_sam_1000_1000, hour, minute, second);
            std::cout << "1000×1000あたりの1サンプルの平均所要時間" << std::endl 
            << hour << "時間" << minute << "分" << second << "秒" << std::endl << std::endl;
        /*****************************************************************************/
    }

    double Image::getroulette_eye(int const& k)const//視線サブパスを必ずp_eye_1_num個構築したい場合に用いられる
    {
        if(k+1+1 <= p_eye_1_num)//k+1番目のサブパスに衝突後に次のrayを飛ばす前にロシアンルーレット(次のrayを飛ばす確率)を決めるのだが, 次のサブパスすなわちk+1+1がp_eye_1_num以下なら
        {
            return 1;
        }
        else
        {
            return p_eye;
        }
    }

    double Image::getroulette_light(int const& k)const//光線サブパスを必ずp_light_1_num個構築したい場合に用いられる
    {
        if(k+1+1 <= p_light_1_num)//k+1番目のサブパスに衝突後に次のrayを飛ばす前にロシアンルーレット(次のrayを飛ばす確率)を決めるのだが, 次のサブパスすなわちk+1+1がp_light_1_num以下なら
        {
            return 1;
        }
        else
        {
            return p_light;
        }
    }
/*********************************************************************************************************************************************************************************/
