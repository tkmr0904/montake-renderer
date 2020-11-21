#include "../Header/camera.h"

/*************PinholeCameraの実装*******************************************************************************************************************************************/
    PinholeCamera::PinholeCamera(int const& _width, int const& _height, Vec3 const& camPos, double const& alpha, double const& theta, double const& phi, double const& holedistance, double const& holearea, double const& sensorsensitivity):width(_width), height(_height), camPos(camPos), alpha(alpha), theta(theta), phi(phi), holedistance(holedistance), holearea(holearea), sensorsensitivity(sensorsensitivity)
    {
        rechangemat = Matrix3(1,0,0,
                              0,1,0,
                              0,0,1);//単位ベクトル  次に回転する


        /******************変換用の行列(カメラ座標の方向ベクトルに左からかければワールド座標の方向ベクトルに変換できる)を求める*******************************************/
            //カメラと一緒にベクトルを回転させるようにイメージすれば理解しやすいと思われる
            rechangemat = rotatezM(rechangemat, alpha);//alpha回転
            rechangemat = rotateyM(rechangemat, theta);//theta回転
            rechangemat = rotatezM(rechangemat, phi);//phi回転
        /****************************************************************************************************************************************************/

        /************上で求めた行列をカメラ座標の方向ベクトルに左からかけることでそれぞれのワールド座標の方向ベクトルを求める******************************/
            camForward = rechangemat * camForward0;//CamForward0を回転させたもの
            camRight   = rechangemat * camRight0;//CamRight0を回転させたもの　　　(ワールド座標)
            camDown      = rechangemat * camDown0;//CamDown0を回転させたもの
        /********************************************************************************************************************************/

        sensorarea = std::pow((2.0/(double)(width)), 2);//センサーの面積を求める   センサーのx方向の辺の長さが2なのでそれをwidth等分すれば画素(正方形)の辺の長さがわかる 正方形なので2乗すれば面積が出る

        #ifdef DEBUG
        if(holedistance <= 0)//holedistanceは0以上でないとカメラとして機能しない バグ防止
        {
            std::cout << "holedistanceが0以下" << std::endl;
            exit(1);
        }
        #endif
    }

    Ray PinholeCamera::getRay(const int& i, const int& j, Subpath& subpath, double& pdflast) const//仮想空間上のrayを得る ついでにpdflastを得る  カメラの中でもrayを飛ばす
    {
        #ifdef DEBUG
        if(RAY_MAX <= num_subpath)//バグ防止
        {
            std::cout << "サブパス最大個数が少なすぎるのでサブパスに配列数を超えてアクセスしてしまう." << std::endl;
            exit(1);
        }
        #endif

        Hit hit;//衝突点の情報を格納する
        Ray ray;//カメラの中でもrayを飛ばす

        /*********************     rayを得る   ***************************************************************/
            Vec3 xy = sensor_2_camera(Vec2(i,j));//センサー上の点をカメラ座標で表す
            Vec3 sensorPos = rechangeposition(xy);//ワールド座標上のセンサーの座標    カメラ座標では(x,y,0)      //
            Vec3 pinhole = camPos + camForward * holedistance;//ワールド座標上のピンホールの座標             //2つともワールド座標

            ray = Ray(sensorPos, pinhole - sensorPos);//センサーから飛ばすrayを得る  rayの方向はピンホールを向く方向
        /***************************************************************************************************/

        /*********************     rayの最初の始点の情報を得る   ***************************************************************/
            hit.hitnormal = camForward;//センサーはcamForwardと垂直になるように決められている
            //hit.hitdistanceは不要(しかも求められないし、存在しない)
            //hit.raydirection_inは不要(しかも求められないし、存在しない)
            //hit.cos_inは不要(しかも求められないし、存在しない)
            hit.cos_out = std::abs(dot(ray.direction, hit.hitnormal));//発射されるrayとセンサーの法線ベクトルのなす角のcos
            hit.BSDF = Vec3(1,1,1) * sensorsensitivity;//最初だけ例外的にセンサーの感度を代入する
            hit.hitpos = sensorPos;//発射元の点はこの点  (センサーの面積は微小であると見なせるため)  この微小なセンサー上の点が選ばれる確率は1
            hit.pdf = 1/sensorarea;//発射元の点が選ばれる確率密度(面積に関する)  センサーの面積が微小だから hit.pdf * sensorarea = 1(確率) と見なして良い
            hit.hitLe = Vec3(0,0,0);//センサー自体は発光していない
            hit.ifhit = 1;//有限の位置にサブパスが存在することを表す
            hit.ifhit_light = 0;//センサーは光源ではない
            hit.roulette = 1;//必ずサブパスが追加されるため 必ずセンサーの外にrayが飛ぶようにするため
        /******************************************************************************************************************/

        /******************************   weightの計算   ****************************************************************************/
            hit.weight = Vec3(1,1,1) / hit.pdf;//最初だけ特別
        /************************************************************************************************************************/

        /**********************    pdflastを求め, 代入する    ***********************************************************************/
            double distance = (pinhole - sensorPos).length();//センサーとピンホールの距離
            double solidangle = holearea * (hit.cos_out/(distance * distance));//発射する方向に関する立体角(微小) その方向に発射される確率は1
            pdflast = 1/solidangle;//発射する方向に関するpdf    solidangleは微小だから pdflast * solidangleは = 1(確率) と見なして良い
        /************************************************************************************************************************/

        /*****************************    subpathのセット    ***************************************************************************/
            subpath.addeye(hit, 0);//センサー上の情報を0番目にセットする
        /************************************************************************************************************************/

        /*******************************rayを渡す********************************************/
            return ray;   //rayを渡す(ワールド座標)
        /************************************************************************************************************************/

        //この後にrayを飛ばし, そこで衝突した点は1番に格納される
    }

    Vec3 PinholeCamera::rechangeposition(Vec3 const& v)const  //カメラ座標をワールド座標に変換する
    {
        return camPos + rechangemat * v;
    }

    Vec3 PinholeCamera::rechangedirection(Vec3 const& v)const  //カメラ座標での方向ベクトルをワールド座標での方向ベクトルに変換する
    {
        return rechangemat * v;
    }

    Vec3 PinholeCamera::sensor_2_camera(Vec2 const& ij) const  //センサー座標をカメラ座標に変換する
    {
        Vec3 xy;
        xy.x = (2.0*ij.x-width) / width;//スケールの変換        //ここでのx,yは長さのスケールをワールド座標系と等しくしてcamRight方向にx, camDown方向にy
        xy.y = (2.0*ij.y - height) / width;//スケールの変換     //
        xy.z = 0; //センサー面はカメラ座標上の平面(z=0)の一部だからカメラ座標でのzの値は0
        //  x方向の長さが2になるように形を変えずに縮める
        
        return xy;
    }

    int PinholeCamera::getnum_subpath() const //constメンバ変数(num_subpath)の値を得る
    {
        return num_subpath;
    }
/**************************************************************************************************************************************************************************/

/******************ThinLensCameraの実装*************************************************************************************************************************************/
    ThinLensCamera::ThinLensCamera(int const& width, int const& height, Vec3 const& camPos, double const& alpha, double const& theta, double const& phi, double const& lensradius, double const& lensdistance, double const& focallength, double const& sensorsensitivity):width(width), height(height), camPos(camPos), alpha(alpha), theta(theta), phi(phi), lensradius(lensradius), lensdistance(lensdistance), focallength(focallength), sensorsensitivity(sensorsensitivity)
    {
        rechangemat = Matrix3(1,0,0,
                              0,1,0,
                              0,0,1);//単位ベクトル  次に回転する


        /******************変換用の行列(カメラ座標の方向ベクトルに左からかければワールド座標の方向ベクトルに変換できる)を求める*******************************************/
            //カメラと一緒にベクトルを回転させるようにイメージすれば理解しやすいと思われる
            rechangemat = rotatezM(rechangemat, alpha);//alpha回転
            rechangemat = rotateyM(rechangemat, theta);//theta回転
            rechangemat = rotatezM(rechangemat, phi);//phi回転
        /****************************************************************************************************************************************************/

        /************上で求めた行列をカメラ座標の方向ベクトルに左からかけることでそれぞれのワールド座標の方向ベクトルを求める******************************/
            camForward = rechangemat * camForward0;//CamForward0を回転させたもの
            camRight   = rechangemat * camRight0;//CamRight0を回転させたもの　　　(ワールド座標)
            camDown    = rechangemat * camDown0;//CamDown0を回転させたもの
        /********************************************************************************************************************************/

        sensorarea = std::pow((2/(double)(width)), 2);//1画素のセンサーの面積  センサーのx方向の辺の長さが2なのでそれをwidth等分すれば画素(正方形)の辺の長さがわかる 正方形なので2乗すれば面積が出る

        #ifdef DEBUG
        if(lensradius <= 0)//バグ防止
        {
            std::cout << "lensradiusが0以下" << std::endl;
            exit(1);
        }
        #endif
    }

    Ray ThinLensCamera::getRay(const int& i, const int& j, Subpath& subpath, double& pdflast) const//仮想空間上のrayを得る ついでにweightを得る  カメラの中でもrayを飛ばす
    {
        #ifdef DEBUG
        if(RAY_MAX <= num_subpath)
        {
            std::cout << "サブパス最大個数が少なすぎるのでサブパスに配列数を超えてアクセスしてしまう." << std::endl;
            exit(1);
        }
        #endif

        double x;
        double y;

        Hit hit;//衝突点の情報を格納する
        Hit hitlast;//前回のhitを格納する
        Ray ray;//カメラの中でもrayを飛ばす
        Ray raylast;//前回のrayを格納する


        ////////////////////////センサー上///////////////////////////////////////////////////////////////////////////////////////////////

        /*****************************センサーから発射されるrayを得る(ワールド座標)****************************************************************************/
            SampleDisk(x,y, lensradius);//円盤上の点を選ぶ
            Vec3 lenspoint = Vec3(x,y,lensdistance);//カメラ座標上
            Vec3 sensorPos = sensor_2_camera(Vec2(i,j));//センサー上の点をカメラ座標で表す
            ray = Ray(rechangeposition(sensorPos), rechangedirection(lenspoint - sensorPos)); //rayの始点、方向はワールド座標で表される
        /****************************************************************************************************************************************/

        /**************************     rayの最初の始点の情報を得る ************************************************/
            hit.hitnormal = camForward;//センサーはcamForwardと垂直になるように決められている
            //hit.hitdistanceは不要(しかも求められないし、存在しない)
            //hit.raydirection_inは不要(しかも求められないし、存在しない)
            //hit.cos_inは不要(しかも求められないし、存在しない)
            hit.cos_out = std::abs(dot(ray.direction, hit.hitnormal));//発射されるrayとセンサーの法線ベクトルのなす角のcos
            hit.BSDF = Vec3(1,1,1) * sensorsensitivity;//最初だけ例外的にセンサーの感度を代入する
            hit.hitpos = rechangeposition(sensorPos);//発射元の点はこの1点  (センサーの面積は微小であると見なせるため)  この微小なセンサー上の点が選ばれる確率は1
            hit.pdf = 1/sensorarea;//発射元の点が選ばれる確率密度(面積に関する)  センサーの面積が微小だから hit.pdf * sensorarea = 1(確率) と見なして良い
            hit.hitLe = Vec3(0,0,0);//センサー自体は発光していない
            hit.ifhit = 1;//有限の位置にサブパスが存在することを表す
            hit.ifhit_light = 0;//センサーは光源ではない
            hit.roulette = 1;//必ずサブパスが追加されるため
        /*********************************************************************************************************/

        /*********************************weightの計算****************************************************************************/
            hit.weight = Vec3(1,1,1) / hit.pdf;//最初だけ特別
        /************************************************************************************************************************/

        /**********************    pdflastを求め, 代入する    *******************************************************/
            double const pdf_disk = 1/(M_PI * lensradius * lensradius);//レンズ上の面に関するpdf     pdf * レンズの面積 = 1(確率)
            double const& cos_lens = hit.cos_out;                      //レンズとレンズに入射するrayのなす角のcos  //レンズとセンサーは平行でなす角が等しいのでcosも等しい
            double const distance = (lenspoint - sensorPos).length();    //センサーとrayが衝突する点(レンズ上)の距離
            pdflast = pdf_disk * (distance * distance / cos_lens) ;//発射する方向に関するpdf 
        /***********************************************************************************************************/

        /*****************************    subpathのセット    *******************************************************/
            subpath.addeye(hit, 0);//センサー上の情報を0番目にセットする
        /**********************************************************************************************************/

        /******************************ray, hitが書き換えられてしまうのでraylast, hitlastそれぞれにコピーをとっておく*******************************************/
            raylast = ray;
            hitlast = hit;
        /*********************************************************************************************************************************************/

        ////////////////////////レンズ上///////////////////////////////////////////////////////////////////////////////////////////////

        /********************************レンズから発射されるrayを得る(ワールド座標)*****************************************************************************/
        {
            Vec3 direction;//カメラ座標上の値をこれから代入する

            if(_z0(lenspoint) != Vec3(0,0,0))//lenspointがレンズの中心と異なれば     normalize(_z0(lenspoint)のゼロ除算回避
            {
                direction = _z1(lenspoint - sensorPos) - _z0(lenspoint).length() / focallength * normalize(_z0(lenspoint));//カメラ座標上  レンズの通過後に曲げられる方向
                //薄レンズでは屈折後の方向の傾きは入射光の傾きから  (入射する点のレンズの中心からの距離)/(焦点距離)  だけレンズの内側方向に加えられるということを利用した
            }
            else//lenspointがレンズの中心と等しければ
            {
                direction = lenspoint - sensorPos;//直進する カメラ座標上
            }

            ray = Ray(rechangeposition(lenspoint), rechangedirection(direction));//ワールド座標に変換
        }
        /********************************************************************************************************************************************************/

        /**************************     今回の衝突点(レンズ)上の情報を得る ************************************************/

            hit.hitnormal = camForward;//レンズはcamForwardと垂直になるように決められている
            hit.hitdistance = distance;//前回の点と今回の点の距離
            hit.raydirection_in = raylast.direction;//入射rayは前回のrayに等しい
            hit.cos_in = std::abs(dot(hit.raydirection_in, hit.hitnormal));//レンズの法線ベクトルと屈折前のrayのなす角のcos
            hit.cos_out = std::abs(dot(ray.direction, hit.hitnormal));     //レンズの法線ベクトルと屈折後のrayのなす角のcos   
            hit.BSDF = Vec3(1,1,1) * std::pow(hit.cos_in, 4) / std::pow((hit.cos_out), 5);//(レンズを屈折するときの)BSDFの値を求める  (デルタ関数はpdfと打ち消し合う)   
            hit.hitpos = ray.origin;//今回の衝突点(今回のrayの発射点に等しい)
            hit.pdf = pdflast * hit.cos_in/(hit.hitdistance * hit.hitdistance);//前回のpdfであるpdflastを用いて求める
            hit.hitLe = Vec3(0,0,0);//レンズ自体は発光していない
            hit.ifhit = 1;//有限の位置にサブパスが存在することを表す
            hit.ifhit_light = 0;//レンズは光源ではない
            hit.roulette = 1;//必ず次のサブパスが追加されるため
        /****************************************************************************************************************/

        /*********************************weightの計算*******************************************************************************/
            hit.weight = subpath.hit_eye[0].weight * hitlast.cos_out * hitlast.BSDF /pdflast;//pdflastを書き換える前に使う
        /************************************************************************************************************************/

        /************************************pdflastを代入する*************************************************************/
            pdflast = 1;//レンズから発射されるrayの方向に関するpdf(デルタ関数がBSDFと打ち消し合うため1で良い)
        /****************************************************************************************************************/

        /*****************************    subpathのセット    *******************************************************/
            subpath.addeye(hit, 1);//センサー上の情報を1番目にセットする
        /**********************************************************************************************************/

        /*******************************rayを渡す********************************************/
            return ray;   //rayを渡す(ワールド座標)
        /************************************************************************************************************************/
    }

    Vec3 ThinLensCamera::rechangeposition(Vec3 const& v)const //カメラ座標をワールド座標に変換する
    {
        return camPos + rechangemat * v;
    }

    Vec3 ThinLensCamera::rechangedirection(Vec3 const& v)const //カメラ座標での方向ベクトルをワールド座標での方向ベクトルに変換する
    {
        return rechangemat * v;
    }

    Vec3 ThinLensCamera::sensor_2_camera(Vec2 const& ij) const
    {
        Vec3 xy;
        xy.x = (2.0*ij.x-width) / width;//スケールの変換        //ここでのx,yは長さのスケールをワールド座標系と等しくしてcamRight方向にx, camDown方向にy
        xy.y = (2.0*ij.y - height) / width;//スケールの変換     //
        xy.z = 0; //センサー面はカメラ座標上の平面(z=0)の一部だからカメラ座標でのzの値は0
        //  x方向の長さが2になるように形を変えずに縮める
        
        return xy;
    }
   
    int ThinLensCamera::getnum_subpath() const //constメンバ変数(num_subpath)の値を得る
    {
        return num_subpath;
    }
/**************************************************************************************************************************************************************************/