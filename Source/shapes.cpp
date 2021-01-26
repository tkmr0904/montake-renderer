

#include "../Header/shapes.h"

using namespace std::literals::string_literals;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void checkPattern(int const& shapenum, int const& patternnum)//適切なPatternかどうか調べる
{
    if((patternnum != (int)SHAPES::ANYTHING) && (patternnum != shapenum))
    {
        std::cout << "不適切なPattern("s + SHAPESNAME[shapenum] + ")"s << std::endl;
        exit(1);
    }
}


/*******************Sphereの実装******************************************************************************************************************************************************/
    Sphere::Sphere(double const& R, double const& theta, double const& phi, double const& alpha, Vec3 const& Center, std::shared_ptr<L::Pattern> const& ppattern_l, std::shared_ptr<M::Pattern> const& ppattern_m):R(R), Center(Center), ppattern_l(ppattern_l), ppattern_m(ppattern_m)
    {
        checkPattern(shapenum(), ppattern_l->patternnum());//適切なPatternかどうか調べる
        checkPattern(shapenum(), ppattern_m->patternnum());//

        changemat = getmatz(-alpha) * getmaty(-theta) * getmatz(-phi);//求めておく
        rechangemat = getmatz(phi) * getmaty(theta) * getmatz(alpha);//求めておく
    }

    void Sphere::intersect0(const Ray& ray, Hit& hit0) const//(中心が原点にあるとして)1つの球に注目してrayとの衝突を考える
    {
        double a = (ray.direction).length2();//------------------------------
        double b = 2*dot(ray.direction, ray.origin-Vec3(0,0,0));//---------------|
        double c = (ray.origin-Vec3(0,0,0)).length2() - R*R;//-------------------|
        double D = (b*b)-(4.0*a*c);//                                       2次方程式の係数
        double t1,t2;//解を取り敢えず格納するため
        double distance_;//距離を取り敢えず格納するため
        Vec3 pos_;   //位置を取り敢えず格納するため
        double theta_;//衝突点のtheta(3次元極座標)
        double phi_;//衝突点のphi(3次元極座標)

        if ((D <= 0.0)||(a == 0))//実数解がないか重解(rayがかする)もしくは2次方程式でない場合
        {
            hit0.ifhit = 0;//当たっていない
            return;
        }
        else//2次方程式で2解が存在すれば
        {
            t1 = (-b - sqrt(D)) / (2.0 * a);//------------------------------------|
            t2 = (-b + sqrt(D)) / (2.0 * a);//------------------------------------|
                                            //                                    解を求める   
            if(t2 <= 0.0)//大きい方の解が0以下なら(半直線が球に突き刺さらないなら)
            {
                hit0.ifhit = 0;//当たっていない
                return;
            }
            else//半直線が球に突き刺さるなら
            if(t1 > 0.0)//2回突き刺されば
            {
                //小さい解に当たっている可能性があり, 大きい解にも当たっている可能性がある
                //まずは小さい解に当たっているか調べる
                distance_ = t1;//小さい解                                                                //
                pos_ = ray.origin + distance_ * ray.direction;                                          //とりあえず格納
                theta_ = gettheta(pos_);                                                                //
                phi_ = getphi(pos_);                                                                    //
                hit0.hitmaterial = ppattern_m->getmaterial(theta_, phi_);//Materialを取得    //

                //Vacuumかどうか判定
                if(hit0.hitmaterial->isVacuum() == 0)//Vacuumでなければ
                {
                    //小さい解に当たった
                    hit0.ifhit = 1;
                    hit0.hitpos = pos_;
                    hit0.hitdistance = distance_;
                    hit0.hitnormal = normalize(pos_-Vec3(0,0,0));
                    hit0.hitlight = ppattern_l->getlight(theta_, phi_);
                    //hit0.hitmaterialは取得済み
                    return;
                }
                else//Vacuumなら
                {
                    //小さい解には当たっていないので大きい解に当たった可能性がある
                    distance_ = t2;//大きい解                                                                 //
                    pos_ = ray.origin + distance_ * ray.direction;                                          //とりあえず格納
                    theta_ = gettheta(pos_);                                                                //
                    phi_ = getphi(pos_);                                                                    //
                    hit0.hitmaterial = ppattern_m->getmaterial(theta_, phi_);//Materialを取得    //

                    //Vacuumかどうか判定
                    if(hit0.hitmaterial->isVacuum() == 0)//Vacuumでなければ
                    {
                        //大きい解に当たった
                        hit0.ifhit = 1;
                        hit0.hitpos = pos_;
                        hit0.hitdistance = distance_;
                        hit0.hitnormal = normalize(pos_-Vec3(0,0,0));
                        hit0.hitlight = ppattern_l->getlight(theta_, phi_);
                        //hit0.hitmaterialは取得済み
                        return;
                    }
                    else//Vacuumなら
                    {   //小さい解にも大きい解にも当たっていない
                        hit0.ifhit = 0;
                        return;
                    }
                }
            }
            else//1回突き刺されば
            {
                //大きい解に当たった可能性がある
                distance_ = t2;//大きい解                                                                 //
                pos_ = ray.origin + distance_ * ray.direction;                                          //とりあえず格納
                theta_ = gettheta(pos_);                                                                //
                phi_ = getphi(pos_);                                                                    //
                hit0.hitmaterial = ppattern_m->getmaterial(theta_, phi_);//Materialを取得    //


                //Vacuumかどうか判定
                if(hit0.hitmaterial->isVacuum() == 0)//Vacuumでなければ
                {
                    hit0.ifhit = 1;//大きい解に当たった
                    hit0.hitpos = pos_;
                    hit0.hitdistance = distance_;
                    hit0.hitnormal = normalize(pos_-Vec3(0,0,0));
                    hit0.hitlight = ppattern_l->getlight(theta_, phi_);
                    //hit0.hitmaterialは取得済み
                    return;
                }
                else//Vacuumなら
                {
                    //小さい解にも大きい解にも当たっていない
                    hit0.ifhit = 0;
                    return;
                }
                
               
            }

        } 
    }

    void Sphere::intersect(const Ray& ray, Hit& hit0) const//1つの球に注目してrayとの衝突を考える
    {
        
        Ray rotatedray = changeray(ray);//基準の位置になるようにrayも一緒に移動する
        intersect0(rotatedray, hit0);//rayを当てる

        hit0.hitpos =  rechangeposition(hit0.hitpos);
        hit0.hitnormal = rechangedirection(hit0.hitnormal);
        
    }

    void Sphere::intersect0_nee(const Ray& ray, Hit& hit0, double& pdfsum_nee_1_obj) const//(中心が原点にあるとして)1つの球に注目してrayとの衝突を考える
    {
        double a = (ray.direction).length2();//------------------------------
        double b = 2*dot(ray.direction, ray.origin-Vec3(0,0,0));//---------------|
        double c = (ray.origin-Vec3(0,0,0)).length2() - R*R;//-------------------|
        double D = (b*b)-(4.0*a*c);//                                       2次方程式の係数
        double t1,t2;//解を取り敢えず格納するため
        Vec3 normal_;//法線ベクトルを取り敢えず格納するため
        double cos1, cos2;

        double distance_;//距離を取り敢えず格納するため
        Vec3 pos_;   //位置を取り敢えず格納するため
        double theta_;//衝突点のtheta(3次元極座標)
        double phi_;//衝突点のphi(3次元極座標)


        if ((D <= 0.0)||(a == 0))//実数解がないか重解(rayがかする)もしくは2次方程式でない場合
        {
            hit0.ifhit = 0;//当たっていない
        }
        else//2次方程式で2解が存在すれば
        {
            t1 = (-b - sqrt(D)) / (2.0 * a);//------------------------------------|
            t2 = (-b + sqrt(D)) / (2.0 * a);//------------------------------------|
                                            //                                    解を求める   
            if(t2 <= 0.0)//大きい方の解が0以下なら(半直線が球に突き刺さらないなら)
            {
                //衝突点なし
                hit0.ifhit = 0;//当たっていない

                //交差点なし
                pdfsum_nee_1_obj += 0;
                return;
            }
            else//半直線が球に突き刺さるなら
            if(t1 > 0.0)//2回突き刺されば
            {


                //小さい解に当たっている可能性があり, 大きい解にも当たっている可能性がある
                //まずは小さい解に当たっているか調べる
                distance_ = t1;//小さい解                                                                //
                pos_ = ray.origin + distance_ * ray.direction;                                          //とりあえず格納
                theta_ = gettheta(pos_);                                                                //
                phi_ = getphi(pos_);                                                                    //
                hit0.hitmaterial = ppattern_m->getmaterial(theta_, phi_);//Materialを取得    //

                //Vacuumかどうか判定
                if(hit0.hitmaterial->isVacuum() == 0)//Vacuumでなければ
                {
                    //小さい解に当たった
                    hit0.ifhit = 1;
                    hit0.hitpos = pos_;
                    hit0.hitdistance = distance_;
                    hit0.hitnormal = normalize(pos_-Vec3(0,0,0));
                    hit0.hitlight = ppattern_l->getlight(theta_, phi_);
                    //hit0.hitmaterialは取得済み
                }
                else//Vacuumなら
                {
                    //小さい解には当たっていないので大きい解に当たった可能性がある
                    distance_ = t2;//大きい解                                                                 //
                    pos_ = ray.origin + distance_ * ray.direction;                                          //とりあえず格納
                    theta_ = gettheta(pos_);                                                                //
                    phi_ = getphi(pos_);                                                                    //
                    hit0.hitmaterial = ppattern_m->getmaterial(theta_, phi_);//Materialを取得    //


                    //Vacuumかどうか判定
                    if(hit0.hitmaterial->isVacuum() == 0)//Vacuumでなければ
                    {
                        //大きい解に当たった
                        hit0.ifhit = 1;
                        hit0.hitpos = pos_;
                        hit0.hitdistance = distance_;
                        hit0.hitnormal = normalize(pos_-Vec3(0,0,0));
                        hit0.hitlight = ppattern_l->getlight(theta_, phi_);
                        //hit0.hitmaterialは取得済み
                    }
                    else//Vacuumなら
                    {   //小さい解にも大きい解にも当たっていない
                        hit0.ifhit = 0;

                    }
                }
                

                //大きい解と小さい解の両方に交差したので

                pos_ = ray.origin + t1 * ray.direction;                 //cos1を求める
                normal_ = normalize(pos_-Vec3(0,0,0));                  //
                cos1 = std::abs(dot(normal_, ray.direction));           //

                pos_ = ray.origin + t2 * ray.direction;                 //cos2を求める
                normal_ = normalize(pos_-Vec3(0,0,0));                  //
                cos2 = std::abs(dot(normal_, ray.direction));           //

                if(cos1 == 0)                                                           //pdfsum_nee_1_objを求める
                {                                                                       //
                    cos1 = 0.001;  
                    std::cout << "球の衝突でcosの値が0になった" << std::endl;              //
                }                                                                       //
                                                                                        //
                if(cos2 == 0)                                                           //
                {                                                                       //
                    cos2 = 0.001; 
                    std::cout << "球の衝突でcosの値が0になった" << std::endl;                //
                }                                                                       //
                //大きい解と小さい解の両方に交差したので                                       //
                pdfsum_nee_1_obj += t1*t1/(4.0*M_PI*R*R*cos1) + t2*t2/(4.0*M_PI*R*R*cos2); //
                return;

            }
            else//1回突き刺されば
            {
                //大きい解に当たった可能性がある
                distance_ = t2;//大きい解                                                                 //
                pos_ = ray.origin + distance_ * ray.direction;                                          //とりあえず格納
                theta_ = gettheta(pos_);                                                                //
                phi_ = getphi(pos_);                                                                    //
                hit0.hitmaterial = ppattern_m->getmaterial(theta_, phi_);//Materialを取得    //

                //Vacuumかどうか判定
                if(hit0.hitmaterial->isVacuum() == 0)//Vacuumでなければ
                {
                    hit0.ifhit = 1;//大きい解に当たった
                    hit0.hitpos = pos_;
                    hit0.hitdistance = distance_;
                    hit0.hitnormal = normalize(pos_-Vec3(0,0,0));
                    hit0.hitlight = ppattern_l->getlight(theta_, phi_);
                    //hit0.hitmaterialは取得済み
                }
                else//Vacuumなら
                {
                    //小さい解にも大きい解にも当たっていない
                    hit0.ifhit = 0;
                }

                

                //大きい解のみに交差したので 
                pos_ = ray.origin + t2 * ray.direction;         //cos2を求める
                normal_ = normalize(pos_-Vec3(0,0,0));          //
                cos2 = std::abs(dot(normal_, ray.direction));   //

                if(cos2 == 0)                                   //pdfsum_nee_1_objを求める
                {                                               //
                    cos2 = 0.001;
                    std::cout << "球の衝突でcosの値が0になった" << std::endl;      //
                }                                               //
                //大きい解のみに交差したので                        //
                pdfsum_nee_1_obj += t2*t2/(4.0*M_PI*R*R*cos2);     //
                return;

               
            }

        } 
    }

    void Sphere::intersect_nee(const Ray& ray, Hit& hit0, double& pdfsum_nee_1_obj) const//1つの球に注目してrayとの衝突を考える
    {
       
        Ray rotatedray = changeray(ray);//三角形が基準の位置になるようにrayも一緒に移動する
        intersect0_nee(rotatedray, hit0, pdfsum_nee_1_obj);//rayを当てる
        hit0.hitpos =  rechangeposition(hit0.hitpos);
        hit0.hitnormal = rechangedirection(hit0.hitnormal);
        
    }

    double Sphere::getpdf_nee_1_obj() const
    {
        return 1 / (4 * M_PI * R*R );
    }

    Vec3 Sphere::changeposition(Vec3 p) const
    {
        p -= Center;
        p = changemat * p;
        return p;
    }

    Vec3 Sphere::rechangeposition(Vec3 p) const
    {
        p = rechangemat * p;
        p += Center;
        return p;
    }

    Vec3 Sphere::changedirection(Vec3 p) const
    {
        p = changemat * p;
        return p;
    }

    Vec3 Sphere::rechangedirection(Vec3 p) const
    {
        p = rechangemat * p;
        return p;
    }

    Ray Sphere::changeray (Ray ray)const
    {
        ray.origin = ray.origin - Center;
        ray = rotateray(ray, changemat);

        return ray;
    }

    Ray Sphere::rechangeray (Ray ray)const
    {
        ray = rotateray(ray, rechangemat);
        ray.origin = ray.origin + Center;

        return ray;
    }

    Vec3 Sphere::getnormal0(Vec3 position0) const
    {
        return normalize(position0-Vec3(0,0,0));
    }

    Vec3 Sphere::getnormal(Vec3 position) const
    {
        position = changeposition(position);
        return rechangedirection(getnormal0(position));
    }

    void Sphere::getposition(Hit& hit) const//hit.hitpos, hit.hitnormal, hit.pdf, hit.hitlight, hit.hitmaterialを求める
    {
        double theta;    //乱数
        double phi;      //乱数
        Vec3 position;//基準座標

        theta = acos(1-rnd() * 2);
        phi = rnd() * M_PI;


        hit.ifhit = 1; //衝突点(有限の位置にある点)とみなす
        position = R * Vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta)); //rayを向ける点(回転前)
        hit.hitpos = rechangeposition(position);//ワールド座標に変換
        hit.hitnormal = getnormal(hit.hitpos);//ワールド座標の位置からワールド座標の法線ベクトルを求める
        hit.pdf = 1/(4*M_PI*R*R);
        hit.hitlight = ppattern_l->getlight(theta, phi);
        hit.hitmaterial = ppattern_m->getmaterial(theta, phi);

        //この時点でhit.pdf, hit.hitlight, hit.hitpos, hit.hitnormalは格納済み
    }

    Vec3 Sphere::pos() const//基準点に移動したときに原点にくる点がある座標を返す
    {
        return Center;
    }

    void Sphere::moveShape(const Matrix3& rotatemat, const Matrix3& rerotatemat, const Vec3& movedirection)//回してから平行移動
    {
        Center = rotatemat * Center;                            //回して
        changemat = changemat * rerotatemat;//姿勢は回すだけ        //
        rechangemat = rotatemat * rechangemat;//姿勢は回すだけ      //

        Center += movedirection;//平行移動
    }

    int Sphere::shapenum()const
    {
        return (int)SHAPES::SPHERE;
    }
/************************************************************************************************************************************************************************************/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/********************Pipeの実装*******************************************************************************************************************************************************/

    Pipe::Pipe(const double radius, const Vec3 position, const Vec3 direction, const double alpha, const std::shared_ptr<L::Pattern> ppattern_l, std::shared_ptr<M::Pattern> ppattern_m): radius(radius), position(position), ppattern_l(ppattern_l), ppattern_m(ppattern_m)
    {
        double theta = gettheta(direction);
        double phi   = getphi(direction);
        length = direction.length();
        if(length <= 0)
        {
            std::cout << "Pipeの長さが不適切(0以下)" << std::endl;
            exit(1);
        }
        if(radius <= 0)
        {
            std::cout << "Pipeの半径が不適切(0以下)" << std::endl;
            exit(1);
        }

        checkPattern(shapenum(), ppattern_l->patternnum());//適切なPatternかどうか調べる
        checkPattern(shapenum(), ppattern_m->patternnum());//

        changemat = getmatz(-alpha) * getmaty(-theta) * getmatz(-phi);//求めておく
        rechangemat = getmatz(phi) * getmaty(theta) * getmatz(alpha);//求めておく
    }

    void Pipe::intersect0(const Ray& ray, Hit& hit0) const//円柱が原点からz軸方向にある場合の衝突
    {

        //交差する必要十分条件はこれら(*)(**)(***)(****)を共に満たすこと
        //(*)   :rayが表面をかすらない
        //(**)  :rayの方向の単位ベクトルの係数が0より大きい位置にある
        //(***) :z座標が0以上length以下である
        //(****):MaterialがVacuumでない
        //
        //これら4つの条件を満たすもののうち, 距離が最も近いものを衝突点とする

        Vec3 originxy = _z0(ray.origin);
        Vec3 directionxy = _z0(ray.direction);
        double a = directionxy.length2();         //ax^2+bx+cはxy平面上に円柱とrayを投影したときの交差点(無限に長い円筒に交差する点)を解としてもつ
        double b = 2*dot(directionxy, originxy);
        double c = originxy.length2() - radius*radius;
        double D = (b*b)-(4.0*a*c);//判別式

        double t1,t2;//2次方程式の解を格納


        double distance_;//距離を取り敢えず格納するため
        Vec3 pos_;   //位置を取り敢えず格納するため
        double phi_;//衝突点のphi(3次元極座標)
        double z_;//衝突点のz座標

        if ((D <= 0.0)||(a == 0))//実数解がないか重解(rayがかする)もしくは2次方程式でない場合   
        {
            //(*)を満たす点が存在しないので
            hit0.ifhit = 0;//衝突していない
            return;
        }
        else//2つの異なる実数解が存在すれば　　どちらの解も(*)を満たすなら
        {
            t1 = (-b - sqrt(D)) / (2.0 * a);//小さい解
            t2 = (-b + sqrt(D)) / (2.0 * a);//大きい解
            if(t2 <= 0.0)//大きい方の解が0以下なら(**)を満たさない
            {
                //小さい解にも大きい解にも当たっていない   どちらの解も(**)を満たさないため
                hit0.ifhit = 0;//当たっていない
                return;
            }
            else//少なくとも一方の解が(**)を満たすなら
            {
                if(t1 > 0.0)//どちらの解も(**)を満たすなら
                {
                    //大きい解と小さい解のどちらかに衝突する可能性がある

                    //小さい解を調べる

                    //小さい解の点のz座標が0以上length以下かどうか(***)を判定する準備 
                    distance_ = t1;//小さい解                            //    
                    pos_ = ray.origin + distance_ * ray.direction;      //取り敢えず格納

                    if((0<=pos_.z) && (pos_.z<=length))//小さい解で(***)を満たすなら   
                    {
                        //Vacuumかどうか判定する準備
                        phi_ = getphi(pos_);                                                //取り敢えず格納
                        z_ = pos_.z/length;                                                 //
                        hit0.hitmaterial = ppattern_m->getmaterial(phi_, z_);   //

                        //Vacuumかどうか判定 (****)を判定
                        if(hit0.hitmaterial->isVacuum() == 0)//Vacuumでなければ (****)を満たせば
                        {
                            //小さい解で(*)(**)(***)(****)全てを満たしている
                            hit0.ifhit = 1;                         //小さい解に本当に衝突したので
                            hit0.hitpos = pos_;                     //
                            hit0.hitdistance = distance_;           //
                            hit0.hitnormal = normalize(_z0(pos_));  //
                            hit0.hitlight = ppattern_l->getlight(phi_, z_);
                            //hitmaterial0は取得済み
                            return;
                        }
                        else//Vacuumなら 小さい解で(****)を満たさないなら
                        {
                            //小さい解には当たらないことが確定したので大きい解を調べる

                            //この時点で大きい解は(*)(**)を満たしている

                            //大きい解で(***)を満たすかどうか判定する準備
                            distance_ = t2;//大きい解                    //
                            pos_ = ray.origin + distance_ * ray.direction;   //取り敢えず格納

                            if((0<=pos_.z) && (pos_.z<=length))//大きい解で(***)を満たすなら
                            {
                                //Vacuumかどうか判定する準備
                                phi_ = getphi(pos_);                                                //
                                z_ = pos_.z/length;                                                 //
                                hit0.hitmaterial = ppattern_m->getmaterial(phi_, z_);   //取り敢えず格納

                                //Vacuumかどうか判定 (****)かどうか
                                if(hit0.hitmaterial->isVacuum() == 0)//Vacuumでなければ (****)を満たすなら
                                {
                                    //小さい解で(****)を満たさず, 大きい解で(*)(**)(***)(****)全てを満たしている
                                    hit0.ifhit = 1;                         //大きい解に本当に衝突したので
                                    hit0.hitpos = pos_;                     //
                                    hit0.hitdistance = distance_;           //
                                    hit0.hitnormal = normalize(_z0(pos_));  //
                                    hit0.hitlight = ppattern_l->getlight(phi_, z_);
                                    //hitmaterial0は取得済み
                                    return;
                                }
                                else //小さい解も大きい解も(****)を満たさないなら
                                {
                                    //小さい解にも大きい解にも当たっていない  
                                    hit0.ifhit = 0;
                                    return;
                                }
                                
                            }
                            else// 小さい解は(****)を満たさず, 大きい解は(***)を満たさないなら
                            {
                                //小さい解にも大きい解にも当たっていない
                                hit0.ifhit = 0;
                                return;
                            }
                        }
                    }
                    else//小さい解で(***)を満たさないなら
                    {
                        //小さい解には当たらないことが確定したので大きい解を調べる

                        //この時点で大きい解は(*)(**)を満たしている

                        //大きい解で(***)かどうか判定する準備
                        distance_ = t2;//大きい解                   //
                        pos_ = ray.origin + distance_ * ray.direction;   //取り敢えず格納

                        if((0<=pos_.z) && (pos_.z<=length))//大きい解で(***)を満たすなら
                        {
                            //Vacuumかどうか判定する準備
                            phi_ = getphi(pos_);                                                //取り敢えず格納
                            z_ = pos_.z/length;                                                 //
                            hit0.hitmaterial = ppattern_m->getmaterial(phi_, z_);   //

                            //Vacuumかどうか判定 (****)を満たすかどうか
                            if(hit0.hitmaterial->isVacuum() == 0)//Vacuumでなければ (****)を満たすなら
                            {
                                //小さい解で(***)を満たさず, 大きい解で(*)(**)(***)(****)全てを満たす
                                hit0.ifhit = 1;                         //大きい解に本当に衝突したので
                                hit0.hitpos = pos_;                     //
                                hit0.hitdistance = distance_;           //
                                hit0.hitnormal = normalize(_z0(pos_));  //
                                hit0.hitlight = ppattern_l->getlight(phi_, z_);
                                //hitmaterial0は取得済み
                                return;
                            }
                            else//Vacuumなら (****)を満たさないなら
                            {
                                //小さい解にも大きい解にも当たっていない   小さい解は(***)を満たさず, 大きい解は(****)を満たさないため
                                hit0.ifhit = 0;
                                return;
                            }
                        }
                        else//衝突点のz座標が0以上length以下でないなら (***)を満たさないなら
                        {
                            //小さい解にも大きい解にも当たっていない   どちらの解も(***)を満たさないため
                            hit0.ifhit = 0;//実際は衝突していない
                            return;
                        }
                    }
                    
                }
                else//小さい解で(**)を満たさず, 大きい解で(**)を満たしていれば
                {
                    //大きい解に衝突する可能性がある

                    //大きい解で(***)かどうか判定する準備
                    distance_ = t2;                                   //
                    pos_ = ray.origin + distance_ * ray.direction;    //取り敢えず格納


                    if((0<=pos_.z) && (pos_.z<=length))//大きい解で(***)をみたすなら
                    {
                        //Vacuumかどうか判定する準備
                        phi_ = getphi(pos_);                                                //取り敢えず格納
                        z_ = pos_.z/length;                                                 //
                        hit0.hitmaterial = ppattern_m->getmaterial(phi_, z_);   //

                        //Vacuumかどうか判定  (****)を満たすかどうか
                        if(hit0.hitmaterial->isVacuum() == 0)//Vacuumでなければ (****)を満たすなら
                        {
                            //小さい解で(**)を満たさず, 大きい解で(*)(**)(***)(****)全てを満たすので
                            hit0.ifhit = 1;                         //大きい解に本当に衝突したので
                            hit0.hitpos = pos_;                     //
                            hit0.hitdistance = distance_;           //
                            hit0.hitnormal = normalize(_z0(pos_));  //
                            hit0.hitlight = ppattern_l->getlight(phi_, z_);
                            //hitmaterial0は取得済み
                            return;
                        }
                        else//Vacuumなら (****)を満たさないなら
                        {
                            //小さい解にも大きい解にも当たっていない   小さい解は(**)を満たさず, 大きい解は(****)を満たさないため
                            hit0.ifhit = 0;
                            return;
                        }
                    }
                    else//大きい解で(***)を満たさないなら
                    {
                        //小さい解にも大きい解にも当たっていない   小さい解は(**)を満たさず, 大きい解は(***)を満たさないため
                        hit0.ifhit = 0;//小さい解にも大きい解にも当たっていない
                        return;
                    }
                }


            }
            
        }
    }

    void Pipe::intersect(const Ray& ray, Hit& hit0) const//1つの物体に注目してrayとの衝突を考える
    {
        Ray rotatedray = changeray(ray);//円柱が基準の位置になるようにrayも一緒に移動する
        intersect0(rotatedray, hit0);//rayを当てる
        hit0.hitpos =  rechangeposition(hit0.hitpos);
        hit0.hitnormal = rechangedirection(hit0.hitnormal);
    }

    void Pipe::intersect0_nee(const Ray& ray, Hit& hit0, double& pdfsum_nee_1_obj) const//円柱が原点からz軸方向にある場合の衝突
    {
        //交差する必要十分条件
        //(*)   :rayが表面をかすらない
        //(**)  :rayの方向の単位ベクトルの係数が0より大きい位置にある
        //(***) :z座標が0以上length以下である
        //(****):MaterialがVacuumでない
        //
        //これら4つの条件を満たすもののうち, 距離が最も近いものを衝突点とする



        //確率密度関数を加える点である必要十分条件
        //(*)   :rayが表面をかすらない
        //(**)  :rayの方向の単位ベクトルの係数が0より大きい位置にある
        //(***) :z座標が0以上length以下である
        //
        //衝突点と異なり, これら4つの条件を満たす全ての点における確率関数を足し合わせる




        Vec3 originxy = _z0(ray.origin);
        Vec3 directionxy = _z0(ray.direction);
        double a = directionxy.length2();     //ax^2+bx+cはxy平面上に円柱とrayを投影したときの交差点を解としてもつ
        double b = 2*dot(directionxy, originxy);
        double c = originxy.length2() - radius*radius;
        double D = (b*b)-(4.0*a*c);//判別式

        double t1,t2;//2次方程式の解を格納

        Vec3 normal_;
        double cos1, cos2;

        double distance_;//距離を取り敢えず格納するため
        Vec3 pos_;   //位置を取り敢えず格納するため
        double phi_;//衝突点のphi(3次元極座標)
        double z_;//衝突点のz座標

        if ((D <= 0.0)||(a == 0))//実数解がないか重解(rayがかする)もしくは2次方程式でない場合
        {
            //(*)を満たす点が存在しないので
            //衝突点なし
            hit0.ifhit = 0;//衝突していない

            //(*)を満たす点が存在しないので
            //交差点なし
            pdfsum_nee_1_obj += 0;
            return;
        }
        else//解が存在すれば どちらの解も(*)を満たすなら
        {
            t1 = (-b - sqrt(D)) / (2.0 * a);//小さい解
            t2 = (-b + sqrt(D)) / (2.0 * a);//大きい解
            if(t2 <= 0.0)//どちらの解も(**)を満たさないなら
            {
                //衝突点なし
                hit0.ifhit = 0;//衝突していない

                //交差点なし
                pdfsum_nee_1_obj += 0;
                return;
            }
            else//少なくとも一方の解が(**)を満たすなら
            if(t1 > 0.0)//どちらの解も(**)を満たすなら
            {

                //大きい解,小さい解のどちらにも交差した可能性がある
            
                //小さい解で(***)を満たすかどうか調べる
                distance_ = t1;                                 //
                pos_ = ray.origin + distance_ * ray.direction;  //取り敢えず格納
                

                if((0<=pos_.z) && (pos_.z<=length))//小さい解で(***)を満たすなら
                {
                    //Vacuumかどうか判定する準備
                    phi_ = getphi(pos_);                                                //取り敢えず格納
                    z_ = pos_.z/length;                                                 //
                    hit0.hitmaterial = ppattern_m->getmaterial(phi_, z_);   //

                    //Vacuumかどうか判定 (****)を判定
                    if(hit0.hitmaterial->isVacuum() == 0)//Vacuumでなければ (****)を満たせば
                    {
                        //小さい解で(*)(**)(***)(****)全てを満たしている
                        hit0.ifhit = 1;                         //小さい解に本当に衝突したので
                        hit0.hitpos = pos_;                     //
                        hit0.hitdistance = distance_;           //
                        hit0.hitnormal = normalize(_z0(pos_));  //
                        hit0.hitlight = ppattern_l->getlight(phi_, z_);
                        //hitmaterial0は取得済み
                    }
                    else//Vacuumなら 小さい解で(****)を満たさないなら
                    {
                        //小さい解には当たらないことが確定したので大きい解を調べる

                        //この時点で大きい解は(*)(**)を満たしている

                        //大きい解で(***)を満たすかどうか判定する準備
                        distance_ = t2;//大きい解                    //
                        pos_ = ray.origin + distance_ * ray.direction;   //取り敢えず格納

                        if((0<=pos_.z) && (pos_.z<=length))//大きい解で(***)を満たすなら
                        {
                            //Vacuumかどうか判定する準備
                            phi_ = getphi(pos_);                                                //
                            z_ = pos_.z/length;                                                 //
                            hit0.hitmaterial = ppattern_m->getmaterial(phi_, z_);   //取り敢えず格納

                            //Vacuumかどうか判定 (****)かどうか
                            if(hit0.hitmaterial->isVacuum() == 0)//Vacuumでなければ (****)を満たすなら
                            {
                                //小さい解で(****)を満たさず, 大きい解で(*)(**)(***)(****)全てを満たしている
                                hit0.ifhit = 1;                         //大きい解に本当に衝突したので
                                hit0.hitpos = pos_;                     //
                                hit0.hitdistance = distance_;           //
                                hit0.hitnormal = normalize(_z0(pos_));  //
                                hit0.hitlight = ppattern_l->getlight(phi_, z_);
                                //hitmaterial0は取得済み
                            }
                            else //小さい解も大きい解も(****)を満たさないなら
                            {
                                //小さい解にも大きい解にも当たっていない  
                                hit0.ifhit = 0;
                            }
                            
                        }
                        else// 小さい解は(****)を満たさず, 大きい解は(***)を満たさないなら
                        {
                            //小さい解にも大きい解にも当たっていない
                            hit0.ifhit = 0;
                        }
                    }


                    ////小さい解で(*)(**)(***)を満たすので
                    //小さい解の確率密度関数を加算できる
                    pos_ = ray.origin + t1 * ray.direction;        //
                    normal_ = normalize(_z0(pos_));                //cos1を求める
                    cos1 = std::abs(dot(normal_, ray.direction));  //
                    
                    //////////////////////////////////////////////////////////////////////

                    //小さい解の確率密度は加算できることはわかったが,大きい解の確率密度も加算できるのか調べる
                    pos_ = ray.origin + t2 * ray.direction; //取り敢えず大きい解の位置を格納

                    if((0<=pos_.z) && (pos_.z<=length))//大きい解で(***)を満たすなら
                    {
                        //大きい解の確率密度も加算できる
                        //pos_は計算済み
                        normal_ = normalize(_z0(pos_));                 //cos2を求める
                        cos2 = std::abs(dot(normal_, ray.direction));   //

                            //大きい解と小さい解の両方に交差したので
                        pdfsum_nee_1_obj += t1*t1/(2*M_PI*radius*length*cos1) + t2*t2/(2*M_PI*radius*length*cos2);

                        return;
                    }
                    else//大きい解の確率密度は加算できないのなら
                    {
                        //確率密度を加算できる小さい解のみなので
                        pdfsum_nee_1_obj += t1*t1/(2*M_PI*radius*length*cos1);

                        return;
                    }
                    

                }
                else//小さい解で(***)を満たさないなら
                {
                    //この時点で大きい解は(*)(**)を満たしている

                    //大きい解で(***)を満たすかどうか調べる
                    distance_ = t2;                                        //
                    pos_ = ray.origin + distance_ * ray.direction;         //取り敢えず格納

                    if((0<=pos_.z) && (pos_.z<=length))//大きい解で(***)を満たすなら
                    {
                        //Vacuumかどうか判定する準備
                        phi_ = getphi(pos_);                                                //取り敢えず格納
                        z_ = pos_.z/length;                                                 //
                        hit0.hitmaterial = ppattern_m->getmaterial(phi_, z_);   //

                        //Vacuumかどうか判定 (****)を判定
                        if(hit0.hitmaterial->isVacuum() == 0)//Vacuumでなければ (****)を満たせば
                        {
                            //小さい解で(***)を満たさず, 大きい解で(*)(**)(***)(****)全てを満たしているので
                            hit0.ifhit = 1;                         //大きい解に本当に衝突したので
                            hit0.hitpos = pos_;                     //
                            hit0.hitdistance = distance_;           //
                            hit0.hitnormal = normalize(_z0(pos_));  //
                            hit0.hitlight = ppattern_l->getlight(phi_, z_);
                            //hitmaterial0は取得済み
                        }
                        else//Vacuumなら 大きい解で(****)を満たさないなら
                        {
                            //小さい解で(***)を満たさず, 大きい解で(****)を満たさないので
                            hit0.ifhit = 0;
                        }



                        //大きい解の確率密度関数のみ加算できる
                        //posは計算済み
                        normal_ = normalize(_z0(pos_));               //cos2を求める
                        cos2 = std::abs(dot(normal_, ray.direction)); //

                        //大きい解の確率密度関数のみ加算できるので
                        pdfsum_nee_1_obj += t2*t2/(2*M_PI*radius*length*cos2);

                        return;
                    }
                   else//どちらの解も(***)を満たさないなら
                    {   
                        //衝突点なし
                        hit0.ifhit = 0;//実際は衝突していない

                        //交差点なし
                        pdfsum_nee_1_obj += 0;
                        return;
                    }
                }
                
            }
            else//小さい解で(**)を満たさず, 大きい解で(**)を満たすなら
            {
                
               //大きい解で(***)を満たすかどうか調べる
               distance_ = t2;                                //
               pos_ = ray.origin + distance_ * ray.direction; //取り敢えず格納

                if((0<=pos_.z) && (pos_.z<=length))//大きい解で(***)を満たすなら
                {
                    //Vacuumかどうか判定する準備
                    phi_ = getphi(pos_);                                                //取り敢えず格納
                    z_ = pos_.z/length;                                                 //
                    hit0.hitmaterial = ppattern_m->getmaterial(phi_, z_);   //

                    //Vacuumかどうか判定 (****)を判定
                    if(hit0.hitmaterial->isVacuum() == 0)//Vacuumでなければ (****)を満たせば
                    {
                        //小さい解で(**)を満たさず, 大きい解で(*)(**)(***)(****)全てを満たしているので
                        hit0.ifhit = 1;                         //大きい解に本当に衝突したので
                        hit0.hitpos = pos_;                     //
                        hit0.hitdistance = distance_;           //
                        hit0.hitnormal = normalize(_z0(pos_));  //
                        hit0.hitlight = ppattern_l->getlight(phi_, z_);
                        //hitmaterial0は取得済み
                    }
                    else//Vacuumなら 大きい解で(****)を満たさないなら
                    {
                        //小さい解で(**)を満たさず, 大きい解で(****)を満たさないので
                        hit0.ifhit = 0;
                    }



                    //大きい解の確率密度関数のみ加算できる
                    pos_ = ray.origin + t2 * ray.direction;
                    normal_ = normalize(_z0(pos_));               //cos2を求める
                    cos2 = std::abs(dot(normal_, ray.direction)); //

                    //大きい解の確率密度関数のみ加算できるので
                    pdfsum_nee_1_obj += t2*t2/(2*M_PI*radius*length*cos2);

                    return;
                }
                else//大きい解で(***)を満たさないなら
                {
                    //小さい解で(**)を満たさず, 大きい解で(***)を満たさないので
                    //衝突点なし
                    hit0.ifhit = 0;//実際は衝突していない

                    //確率密度関数は加算できない
                    pdfsum_nee_1_obj += 0;
                    return;
                }
            }
            
        }
    }

    void Pipe::intersect_nee(const Ray& ray, Hit& hit0, double& pdfsum_nee_1_obj) const//1つの物体に注目してrayとの衝突を考える
    {
        Ray rotatedray = changeray(ray);//円柱が基準の位置になるようにrayも一緒に移動する
        intersect0_nee(rotatedray, hit0, pdfsum_nee_1_obj);//rayを当てる
        hit0.hitpos =  rechangeposition(hit0.hitpos);
        hit0.hitnormal = rechangedirection(hit0.hitnormal);
    }

    double Pipe::getpdf_nee_1_obj() const
    {
        return 1 / (M_PI * radius * radius * length);
    }

    bool Pipe::ifinside0(Vec3 p)
    {
        if((_z0(p).length() < radius)&&(0 < p.z)&&(p.z < length))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

    bool Pipe::ifinside(Vec3 p)
    {
        p = changeposition(p);
        
        return ifinside0(p);
    }

    Vec3 Pipe::changeposition(Vec3 p) const
    {
        p -= position;
        p = changemat * p;
        return p;
    }

    Vec3 Pipe::rechangeposition(Vec3 p) const
    {
        p = rechangemat * p;
        p += position;
        return p;
    }

    Vec3 Pipe::changedirection(Vec3 p) const
    {
        p = changemat * p;
        return p;
    }

    Vec3 Pipe::rechangedirection(Vec3 p) const
    {
        p = rechangemat * p;
        return p;
    }

    Ray Pipe::changeray (Ray ray)const
    {
        ray.origin = ray.origin - position;
        ray = rotateray(ray, changemat);

        return ray;
    }

    Ray Pipe::rechangeray (Ray ray)const
    {
        ray = rotateray(ray, rechangemat);
        ray.origin = ray.origin + position;

        return ray;
    }

    Vec3 Pipe::getnormal0(Vec3 position0) const
    {
        return normalize(_z0(position0));
    }

    Vec3 Pipe::getnormal(Vec3 position) const
    {
        position = changeposition(position);
        return rechangedirection(getnormal0(position));
    }

    void Pipe::getposition(Hit& hit) const//hit.hitpos, hit.hitnormal, hit.pdf, hit.hitlight, hit.hitmaterialを求める
    {
        double phi;        //乱数
        double z;          //乱数
        Vec3 position;//基準座標

        phi = 2 * M_PI * rnd();
        z = rnd();

        hit.ifhit = 1; //衝突点(有限の位置にある点)とみなす
        position = Vec3(radius * cos(phi), radius * sin(phi), z * length);
        hit.hitpos = rechangeposition(position);//ワールド座標での位置を求める
        hit.hitnormal = getnormal(hit.hitpos);//ワールド座標の位置からワールド座標の法線ベクトルを求める
        hit.pdf = 1/(2*M_PI*radius*length);
        hit.hitlight = ppattern_l->getlight(phi, z);
        hit.hitmaterial = ppattern_m->getmaterial(phi, z);

        //この時点でhit.pdf, hit.hitlight, hit.hitpos, hit.hitnormalは格納済み
    }

    Vec3 Pipe::pos() const//基準点に移動したときに原点にくる点がある座標を返す
    {
        return position;
    }

    void Pipe::moveShape(const Matrix3& rotatemat, const Matrix3& rerotatemat, const Vec3& movedirection)//回してから平行移動
    {
        position = rotatemat * position;                            //回して
        changemat = changemat * rerotatemat;//姿勢は回すだけ        //
        rechangemat = rotatemat * rechangemat;//姿勢は回すだけ      //

        position += movedirection;//平行移動
    }

    int Pipe::shapenum()const
    {
        return (int)SHAPES::PIPE;
    }
/************************************************************************************************************************************************************************************/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/***********Triangleの実装************************************************************************************************************************************************************/

    Triangle::Triangle(Vec3 point0, Vec3 point1, Vec3 point2, double alpha, std::shared_ptr<L::Pattern> ppattern_l, std::shared_ptr<M::Pattern> ppattern_m):point0(point0), ppattern_l(ppattern_l), ppattern_m(ppattern_m)
    {
        normal = normalize(cross(point2-point0, point1-point0));//法線ベクトル
        double theta = gettheta(normal);//theta
        double phi   = getphi(normal);//phi
        p1 = z0(rotatey(rotatez(point1-point0, -phi), -theta));//xy平面上に移したときにpoint1が移される位置
        p2 = z0(rotatey(rotatez(point2-point0, -phi), -theta));//xy平面上に移したときにpoint2が移される位置
        M = inverse2(Matrix2(p1, p2));//これに  xy平面上の位置ベクトルをかけると係数を並べたベクトルが求まる
        double cos_p1_p2 = dot(normalize(p1), normalize(p2));//p1とp2のなす角
        sin_p1_p2 = sqrt(1-cos_p1_p2*cos_p1_p2);
        
        checkPattern(shapenum(), ppattern_l->patternnum());//適切なPatternかどうか調べる
        checkPattern(shapenum(), ppattern_m->patternnum());//


        changemat = getmatz(-alpha) * getmaty(-theta) * getmatz(-phi);//求めておく
        rechangemat = getmatz(phi) * getmaty(theta) * getmatz(alpha);//求めておく
    }

    void Triangle::intersect0(const Ray& ray, Hit& hit0) const//xy平面上にある三角形における衝突
    {
        double t;//係数
        Vec3 pos_;
        Vec3 normal_ =  Vec3(0,0,1);
        if(ray.direction.z == 0)//xy平面とrayの向きが平行なら
        {
            hit0.ifhit = 0;//衝突しない
            return;//脱出
        }

        t = -ray.origin.z/ray.direction.z;//係数を求める
        if(t<=0)//rayの半直線がxy平面に突き刺さらなければ
        {
            hit0.ifhit = 0;//衝突しない
            return;//脱出
        }
        //rayの半直線がxy平面に突き刺されば

        pos_ = ray.origin + t * ray.direction;//取り敢えず衝突点を保存する


        Vec2 v = M*z0(pos_);//係数を並べたベクトルを求める(逆行列)


        if((v.x > 0) && (v.y > 0))//両方の係数が正なら
        {

            if((v.x + v.y) < 1)//係数の和<1なら
            {

                hit0.hitmaterial = ppattern_m->getmaterial(v.x, v.y);//取り敢えず格納
                if(hit0.hitmaterial->isVacuum() == 0)//Vacuumでなければ
                {
                    hit0.ifhit = 1;                                                                                                     //衝突するので
                    hit0.hitpos = pos_;                                                                                                 //
                    hit0.hitdistance = t;//係数が長さに等しい                                                                              //
                    hit0.hitnormal =normal_;         
                    hit0.hitlight = ppattern_l->getlight(v.x, v.y);
                    //hit0.hitmaterialは取得済み;

                    return;
                }
                else
                {
                    hit0.ifhit = 0;//衝突していない
                    return;
                }         
            }
            else//係数の和>=1なら
            {
                hit0.ifhit = 0;//衝突しない
                return;
            }
        }
        else//少なくとも一方の係数が0以下なら
        {
            hit0.ifhit = 0;//衝突しない
            return;
        }

    
    }   

    void Triangle::intersect(const Ray& ray, Hit& hit0) const//1つの物体に注目してrayとの衝突を考える
    {
        Ray rotatedray = changeray(ray);//三角形が基準の位置になるようにrayも一緒に移動する
        intersect0(rotatedray, hit0);//rayを当てる

        hit0.hitpos =  rechangeposition(hit0.hitpos);
        hit0.hitnormal = rechangedirection(hit0.hitnormal);
        
        
    }

    void Triangle::intersect0_nee(const Ray& ray, Hit& hit0, double& pdfsum_nee_1_obj) const//xy平面上にある三角形における衝突
    {
        double t;//係数
        Vec3 pos_;
        Vec3 normal_ =  Vec3(0,0,1);
        double cos;//平面とrayのなす角

        if(ray.direction.z == 0)//平面とrayの向きが平行なら
        {
            hit0.ifhit = 0;//衝突しない
            pdfsum_nee_1_obj += 0;//加算しない
            return;//脱出
        }

        t = -ray.origin.z/ray.direction.z;//係数を求める
        if(t<=0)//rayの半直線が平面に突き刺さらなければ
        {
            hit0.ifhit = 0;//衝突しない
            pdfsum_nee_1_obj += 0;//加算しない
            return;//脱出
        }

        pos_ = ray.origin + t * ray.direction;//取り敢えず衝突点を保存する


        Vec2 v = M*z0(hit0.hitpos);//係数を並べたベクトルを求める(逆行列)


        if((v.x > 0) && (v.y > 0))//両方の係数が正なら
        {
            

            if((v.x + v.y) < 1)//係数の和<1なら
            {


                hit0.hitmaterial = ppattern_m->getmaterial(v.x, v.y);//取り敢えず格納
                if(hit0.hitmaterial->isVacuum() == 0)//Vacuumでなければ
                {
                    hit0.ifhit = 1;                                                                                                     //衝突するので
                    hit0.hitpos = pos_;                                                                                                 //
                    hit0.hitdistance = t;//係数が長さに等しい                                                                              //
                    hit0.hitnormal = normal_;         
                    hit0.hitlight = ppattern_l->getlight(v.x, v.y);
                    //hit0.hitmaterialは取得済み;
                }
                else
                {
                    hit0.ifhit = 0;//衝突していない
                }  


                //確率密度関数を加算できるので
                cos = std::abs(dot(normal_, ray.direction));                            //cosを求める
                pdfsum_nee_1_obj += t*t/(0.5*p1.length()*p2.length()*sin_p1_p2*cos);       //pdfsum_nee_1_objを求める

                return;
            }
            else//係数の和>=1なら
            {
                hit0.ifhit = 0;//衝突しない
                pdfsum_nee_1_obj += 0;//加算しない
                return;
            }
        }
        else//両方の係数が0以下なら
        {
            hit0.ifhit = 0;//衝突しない
            pdfsum_nee_1_obj += 0;//加算しない
            return;
        }

    
    }   

    void Triangle::intersect_nee(const Ray& ray, Hit& hit0, double& pdfsum_nee_1_obj) const//1つの物体に注目してrayとの衝突を考える
    {
        Ray rotatedray = changeray(ray);//三角形が基準の位置になるようにrayも一緒に移動する
        intersect0_nee(rotatedray, hit0, pdfsum_nee_1_obj);//rayを当てる
        hit0.hitpos =  rechangeposition(hit0.hitpos);
        hit0.hitnormal = rechangedirection(hit0.hitnormal);
    }

    double Triangle::getpdf_nee_1_obj() const
    {
        return 1 / (0.5 * sin_p1_p2 * p1.length() * p2.length());
    }

    Vec3 Triangle::changeposition(Vec3 p) const
    {
        p -= point0;
        p = changemat * p;
        return p;
    }

    Vec3 Triangle::rechangeposition(Vec3 p) const
    {
        p = rechangemat * p;
        p += point0;
        return p;

    }

    Vec3 Triangle::changedirection(Vec3 p) const
    {
        p = changemat * p;
        return p;
    }

    Vec3 Triangle::rechangedirection(Vec3 p) const
    {
        p = rechangemat * p;
        return p;
    }

    Ray Triangle::changeray (Ray ray)const
    {
        ray.origin = ray.origin - point0;
        ray = rotateray(ray, changemat);

        return ray;
    }

    Ray Triangle::rechangeray (Ray ray)const
    {
        ray = rotateray(ray, rechangemat);
        ray.origin = ray.origin + point0;

        return ray;
    }

    Vec3 Triangle::getnormal0(Vec3 position0) const
    {
        return Vec3(0,0,1);
    }

    Vec3 Triangle::getnormal(Vec3 position) const
    {
        position = changeposition(position);
        return rechangedirection(getnormal0(position));
    }

    void Triangle::getposition(Hit& hit) const//hit.hitpos, hit.hitnormal, hit.pdf, hitlight, hit.hitmaterialを求める
    {
        double x_p1; //乱数
        double x_p2; //乱数
        Vec3 position;//基準座標

        x_p1 = rnd();
        x_p2 = rnd();

        hit.ifhit = 1; //衝突点(有限の位置にある点)とみなす
        position = x_p1 * Vec2_to_Vec3(p1) + x_p2 * Vec2_to_Vec3(p2);
        hit.hitpos = rechangeposition(position);//ワールド座標の位置を求める
        hit.hitnormal = getnormal(hit.hitpos);//ワールド座標の位置からワールド座標の法線ベクトルを求める
        hit.pdf = 1/(0.5*p1.length()*p2.length()*sin_p1_p2);
        hit.hitlight = ppattern_l->getlight(x_p1, x_p2);
        hit.hitmaterial = ppattern_m->getmaterial(x_p1, x_p2);
    }

    Vec3 Triangle::pos() const//基準点に移動したときに原点にくる点がある座標を返す
    {
        return point0;
    }

    void Triangle::moveShape(const Matrix3& rotatemat, const Matrix3& rerotatemat, const Vec3& movedirection)//回してから平行移動
    {
        point0 = rotatemat * point0;                            //回して
        changemat = changemat * rerotatemat;//姿勢は回すだけ        //
        rechangemat = rotatemat * rechangemat;//姿勢は回すだけ      //

        point0 += movedirection;//平行移動
    }

    int Triangle::shapenum()const
    {
        return (int)SHAPES::TRIANGLE;
    }
/************************************************************************************************************************************************************************************/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/************Diskの実装***************************************************************************************************************************************************************/

    Disk::Disk(double R, Vec3 Center, double phi, double theta, double alpha, std::shared_ptr<L::Pattern> ppattern_l, std::shared_ptr<M::Pattern> ppattern_m):R(R), Center(Center), ppattern_l(ppattern_l), ppattern_m(ppattern_m)
    {
        checkPattern(shapenum(), ppattern_l->patternnum());//適切なPatternかどうか調べる
        checkPattern(shapenum(), ppattern_m->patternnum());//


        changemat = getmatz(-alpha) * getmaty(-theta) * getmatz(-phi);//求めておく
        rechangemat = getmatz(phi) * getmaty(theta) * getmatz(alpha);//求めておく

    }

    void Disk::intersect0(const Ray& ray, Hit& hit0) const//xy平面上にあり,中心が原点の場合
    {
        double t;
        Vec3 pos_;
        Vec3 normal_ = Vec3(0,0,1);

        double hitr;//衝突点のr
        double hitphi;//衝突点のphi

        if(ray.direction.z != 0)//rayの方向がxy平面と平行でないなら
        {
            t = -(ray.origin.z/ray.direction.z);
        }
        else//rayの方向がxy平面と平行なら
        {
            hit0.ifhit = 0;//当たっていない
            return;
        }

        if(t<=0)//rayの方向がxy平面と平行でなく,半直線がxy平面に突き刺さらないなら
        {
            hit0.ifhit = 0;//当たっていない
            return;
        }

        //rayの方向がxy平面と平行でなく,半直線がxy平面に突き刺さるなら

        pos_ = ray.origin + t * ray.direction;//xy平面との交点を取り敢えず求める
        if(_z0(pos_).length()<=R)//半直線が円盤に突き刺さっているなら
        {

            hitr = pos_.length()/R;
            hitphi = getphi(pos_);
            hit0.hitmaterial = ppattern_m->getmaterial(hitr, hitphi);


            if(hit0.hitmaterial->isVacuum() == 0)//Vacuumでなければ
            {

                hit0.ifhit = 1;                                                                                             //当たったので
                hit0.hitpos = pos_;                                                                                         //
                hit0.hitdistance = t;                                                                                       //
                hit0.hitnormal = normal_;
                hit0.hitlight = ppattern_l->getlight(hitr, hitphi);
                //hit.hitmaterialは取得済み
                return;
            }
            else//Vacuumなら
            {
                hit0.ifhit = 0;//当たっていない
                return;
            }
            
        }
        else//半直線が円盤に突き刺さっていないなら
        {
            hit0.ifhit = 0;//当たっていない
            return;
        }


    }

    void Disk::intersect(const Ray& ray, Hit& hit0) const//1つの物体に注目してrayとの衝突を考える
    {
        Ray rotatedray = changeray(ray);//三角形が基準の位置になるようにrayも一緒に移動する
        intersect0(rotatedray, hit0);//rayを当てる

        hit0.hitpos =  rechangeposition(hit0.hitpos);
        hit0.hitnormal = rechangedirection(hit0.hitnormal);
    }

    void Disk::intersect0_nee(const Ray& ray, Hit& hit0, double& pdfsum_nee_1_obj) const
    {
        double t;
        Vec3 pos_;
        Vec3 normal_ = Vec3(0,0,1);
        double cos;

        double hitr;//衝突点のr
        double hitphi;//衝突点のphi

        if(ray.direction.z == 0)//rayの方向がxy平面と平行なら
        {
            //衝突点なし
            hit0.ifhit = 0;//当たっていない

            //交差していない
            pdfsum_nee_1_obj += 0;
            return;
        }
        else//rayの方向がxy平面と平行でないなら
        {
            t = -(ray.origin.z/ray.direction.z);
        }

        if(t<=0)//rayの方向がxy平面と平行でなく,半直線がxy平面に突き刺さらないなら
        {
            //衝突点なし
            hit0.ifhit = 0;//当たっていない

            //交差していない
            pdfsum_nee_1_obj += 0;
            return;
        }

        //rayの方向がxy平面と平行でなく,半直線がxy平面に突き刺さるなら

        pos_ = ray.origin + t * ray.direction;//xy平面との交点を取り敢えず求める
        if(_z0(pos_).length()<=R)//半直線が円盤に突き刺さっているなら
        {

            
            hitr = pos_.length()/R;
            hitphi = getphi(pos_);
            hit0.hitmaterial = ppattern_m->getmaterial(hitr, hitphi);


            if(hit0.hitmaterial->isVacuum() == 0)//Vacuumでなければ
            {

                hit0.ifhit = 1;                                                                                             //当たったので
                hit0.hitpos = pos_;                                                                                         //
                hit0.hitdistance = t;                                                                                       //
                hit0.hitnormal = normal_;
                hit0.hitlight = ppattern_l->getlight(hitr, hitphi);
                //hit.hitmaterialは取得済み
            }
            else//Vacuumなら
            {
                hit0.ifhit = 0;//当たっていない
            }


            //交差したので
            cos = std::abs(dot(normal_, ray.direction));//cosを求める
            pdfsum_nee_1_obj = t*t/(M_PI*R*R*cos);//pdfsum_nee_1_objを求める

            return;
        }
        else//半直線が円盤に突き刺さっていないなら
        {
            //衝突点なし
            hit0.ifhit = 0;//当たっていない

            //交差していない
            pdfsum_nee_1_obj += 0;
            return;
        }


    }

    void Disk::intersect_nee(const Ray& ray, Hit& hit0, double& pdfsum_nee_1_obj) const//1つの物体に注目してrayとの衝突を考える
    {
        Ray rotatedray = changeray(ray);//円柱が基準の位置になるようにrayも一緒に移動する
        intersect0_nee(rotatedray, hit0, pdfsum_nee_1_obj);//rayを当てる
        hit0.hitpos =  rechangeposition(hit0.hitpos);
        hit0.hitnormal = rechangedirection(hit0.hitnormal);
    }

    double Disk::getpdf_nee_1_obj() const
    {
        return 1 / (M_PI * R*R);
    }
    
    Vec3 Disk::changeposition(Vec3 p) const
    {
        p -= Center;
        p = changemat * p;
        return p;
    }

    Vec3 Disk::rechangeposition(Vec3 p) const
    {
        p = rechangemat * p;
        p += Center;
        return p;
    }

    Vec3 Disk::changedirection(Vec3 p) const
    {
        p = changemat * p;
        return p;
    }

    Vec3 Disk::rechangedirection(Vec3 p) const
    {
        p = rechangemat * p;
        return p;
    }

    Ray Disk::changeray (Ray ray)const
    {
        ray.origin = ray.origin - Center;
        ray = rotateray(ray, changemat);

        return ray;
    }

    Ray Disk::rechangeray (Ray ray)const
    {
        ray = rotateray(ray, rechangemat);
        ray.origin = ray.origin + Center;

        return ray;
    }

    Vec3 Disk::getnormal0(Vec3 position0) const
    {
        return Vec3(0,0,1);
    }

    Vec3 Disk::getnormal(Vec3 position) const
    {
        position = changeposition(position);
        return rechangedirection(getnormal0(position));
    }

    void Disk::getposition(Hit& hit) const//hit.hitpos, hit.hitnormal, hit.pdf, hit.hitlight, hit.hitmaterialを求める
    {
        double r;   //乱数
        double phi; //乱数
        Vec3 position;//基準座標での位置

        r = R * sqrt(rnd());
        phi = 2 * M_PI * rnd();

        hit.ifhit = 1; //衝突点(有限の位置にある点)とみなす
        position = r * Vec3(cos(phi), sin(phi), 0);
        hit.hitpos = rechangeposition(position);//ワールド座標の位置を求める
        hit.hitnormal = getnormal(hit.hitpos);//ワールド座標の位置からワールド座標の法線ベクトルを求める
        hit.pdf = 1/(M_PI*R*R);
        hit.hitlight = ppattern_l->getlight(r/R, phi);
        hit.hitmaterial = ppattern_m->getmaterial(r/R, phi);

        //この時点でhit.pdf, hit.hitlight, hit.hitpos, hit.hitnormalは格納済み
    }

    Vec3 Disk::pos() const//基準点に移動したときに原点にくる点がある座標を返す
    {
        return Center;
    }

    void Disk::moveShape(const Matrix3& rotatemat, const Matrix3& rerotatemat, const Vec3& movedirection)//回してから平行移動
    {
        Center = rotatemat * Center;                            //回して
        changemat = changemat * rerotatemat;//姿勢は回すだけ        //
        rechangemat = rotatemat * rechangemat;//姿勢は回すだけ      //

        Center += movedirection;//平行移動
    }

    int Disk::shapenum()const
    {
        return (int)SHAPES::DISK;
    }
/************************************************************************************************************************************************************************************/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/***************Cuboidの実装**********************************************************************************************************************************************************/

    Cuboid::Cuboid(double x_length, double y_length, double z_length, Vec3 Center, double theta, double phi, double alpha, std::shared_ptr<L::Pattern> ppattern_l_x_u, std::shared_ptr<L::Pattern> ppattern_l_x_d, std::shared_ptr<L::Pattern> ppattern_l_y_u, std::shared_ptr<L::Pattern> ppattern_l_y_d, std::shared_ptr<L::Pattern> ppattern_l_z_u, std::shared_ptr<L::Pattern> ppattern_l_z_d, std::shared_ptr<M::Pattern> ppattern_m_x_u, std::shared_ptr<M::Pattern> ppattern_m_x_d, std::shared_ptr<M::Pattern> ppattern_m_y_u, std::shared_ptr<M::Pattern> ppattern_m_y_d, std::shared_ptr<M::Pattern> ppattern_m_z_u, std::shared_ptr<M::Pattern> ppattern_m_z_d):x_length(x_length), y_length(y_length), z_length(z_length), Center(Center)
    {

        ppattern_m[0][0] = ppattern_m_x_u;
        ppattern_m[0][1] = ppattern_m_x_d;
        ppattern_m[1][0] = ppattern_m_y_u;
        ppattern_m[1][1] = ppattern_m_y_d;
        ppattern_m[2][0] = ppattern_m_z_u;
        ppattern_m[2][1] = ppattern_m_z_d;

        ppattern_l[0][0] = ppattern_l_x_u;
        ppattern_l[0][1] = ppattern_l_x_d;
        ppattern_l[1][0] = ppattern_l_y_u;
        ppattern_l[1][1] = ppattern_l_y_d;
        ppattern_l[2][0] = ppattern_l_z_u;
        ppattern_l[2][1] = ppattern_l_z_d;


        for(int j = 0; j<2; j++)                                            //
        {                                                                   //
            for(int i = 0; i<3; i++)                                        //
            {                                                               //適切なPatternかどうか調べる
                checkPattern(shapenum(), ppattern_l[i][j]->patternnum());   //
                checkPattern(shapenum(), ppattern_l[i][j]->patternnum());   //
            }                                                               //
        }                                                                   //


        changemat = getmatz(-alpha) * getmaty(-theta) * getmatz(-phi);//求めておく
        rechangemat = getmatz(phi) * getmaty(theta) * getmatz(alpha);//求めておく


    }
    
    void Cuboid::intersect0(const Ray& ray, Hit& hit0) const
    {
        //距離が最も近いものを求める
        //6面とも衝突しない場合はifhit=1 が行われず, 最終的にifhitが0となり, 少なくとも1つの面と衝突する場合はifhit=1 が少なくとも1回行われるのでifhitは1となる

        double t;//係数
    
        Vec3 pos_;
        //Vec3 normal_;

        double hitx;
        double hity;
        double hitz;

        hit0.hitdistance = hitdistance0; //この処理は必要 なぜならt < hit0.hitdistanceが成立しないと衝突しないとみなすから
        hit0.ifhit = 0;//この処理は必要

        //右左前後上下


        for(int i = 0; i < 3; i++)//0, 1, 2はそれぞれx, y, zを表す
        {
            if(ray.direction.a[i] == 0)//iが0, 1, 2のとき, それぞれyz, zx, xy平面と平行である場合を排除する
            {
                continue;//下のfor(衝突判定)を飛ばす
            }

            //平行でなければ衝突判定を行う
            for(int j = 0; j < 2; j++)//jが0, 1のとき, それぞれ正の側の平面か負の側の平面か
            {
                t = (-ray.origin.a[i] + length[i][j])/ray.direction.a[i];//ベクトルの係数を求める

                if((t>0) && (t < hit0.hitdistance))
                {
                    pos_ = ray.origin + t * ray.direction;//交点を求める


                        int num1 = (i+1)%3;// iが0, 1, 2のときに num1は1, 2, 0であり, y, z, xを表す
                        int num2 = (i+2)%3;// iが0, 1, 2のときに num2は2, 0, 1であり, z, x, yを表す
                    if
                    (
                        /***************************************/ //iが0,1,2のときlength[num1][0]はy,z,x方向の直方体の頂点の座標成分の大きさになる
                        (-length[num1][0] <= pos_.a[num1]) 

                        && 

                        ( pos_.a[num1]    <= length[num1][0])
                        /***************************************/ //iが0,1,2のときpos_.a[num1]はy,z,x方向の直方体の衝突点の座標成分の大きさになる

                        &&

                        /***************************************/ //iが0,1,2のときlength[num2][0]はz,x,y方向の直方体の頂点の座標成分の大きさになる
                        (-length[num2][0] <= pos_.a[num2])

                        &&

                        ( pos_.a[num2]    <= length[num2][0])
                        /***************************************/ //iが0,1,2のときpos_.a[(num2]はz,x,y方向の直方体の衝突点の座標成分の大きさになる
                    )
                    {
                        /****************辺の長さが2になるようにx,y,z軸方向に伸ばしたりちぢめたり(定数倍)した座標を求める*******************/
                        hitx = pos_.a[0]/length[0][0];//
                        hity = pos_.a[1]/length[1][0];//
                        hitz = pos_.a[2]/length[2][0];//
                        /*********************************************************************************************************/

                        double hitxyz[3]= {hitx, hity, hitz};//取り敢えず配列に格納する


                        hit0.hitmaterial = (ppattern_m[i][j])->getmaterial(hitxyz[num1],hitxyz[num2]);//求めた配列からMaterialを求める

                        if(hit0.hitmaterial->isVacuum() == 0)//Vacuumでなければ
                        {
                            hit0.ifhit = 1;                                                                                             //当たったので
                            hit0.hitpos = pos_;                                                                                         //
                            hit0.hitdistance = t;                                                                                       //
                            hit0.hitnormal = normal_[i][j];
                            hit0.hitlight = (ppattern_l[i][j])->getlight(hitxyz[num1],hitxyz[num2]);
                            //hit0.hitmaterialは取得済み
                            //return;
                        }
                        else//Vacuumなら
                        {
                            //何もしない もしifhit = 0としてしまうと他の面で衝突した場合にその面で衝突しなかったと扱われてしまうため
                            //return;してはいけない なぜなら他の平面で衝突判定を飛ばしてしまう可能性があるため
                        }
                    }
                }
            }
        }
    }

    void Cuboid::intersect(const Ray& ray, Hit& hit0) const
    {
        Ray rotatedray = changeray(ray);//基準の位置になるようにrayも一緒に移動する
        intersect0(rotatedray, hit0);//rayを当てる

        hit0.hitpos =  rechangeposition(hit0.hitpos);
        hit0.hitnormal = rechangedirection(hit0.hitnormal);
    }

    void Cuboid::intersect0_nee(const Ray& ray, Hit& hit0, double& pdfsum_nee_1_obj) const
    {
        double t;//係数
        double cos;
    
        Vec3 pos_;
        //Vec3 normal_;

        double hitx;
        double hity;
        double hitz;

        hit0.hitdistance = hitdistance0;    //初期化
        hit0.ifhit = 0;                     //

        //右左前後上下

         for(int i = 0; i < 3; i++)//0, 1, 2はそれぞれx, y, zを表す
        {
            if(ray.direction.a[i] == 0)//iが0, 1, 2のとき, それぞれyz, zx, xy平面と平行である場合を排除する
            {
                continue;//下のfor(衝突判定)を飛ばす
            }

            //平行でなければ
            for(int j = 0; j < 2; j++)//jが0, 1のとき, それぞれ正の側の平面か負の側の平面か
            {
                t = (-ray.origin.a[i] + length[i][j])/ray.direction.a[i];//ベクトルの係数を求める
                if((t>0) && (t < hit0.hitdistance))
                {
                    pos_ = ray.origin + t * ray.direction;//交点を求める

                    int num1 = (i+1)%3;// iが0, 1, 2のときに num1は1, 2, 0であり, y, z, xを表す
                    int num2 = (i+2)%3;// iが0, 1, 2のときに num2は2, 0, 1であり, z, x, yを表す
                        //0, 1, 2はそれぞれx, y, zを表す
                    if
                    (
                        /***************************************/ //iが0,1,2のときlength[num1][0]はy,z,x方向の直方体の頂点の座標成分の大きさになる
                        (-length[num1][0] <= pos_.a[num1]) 

                        && 

                        ( pos_.a[num1]    <= length[num1][0])
                        /***************************************/ //iが0,1,2のときpos_.a[num1]はy,z,x方向の直方体の衝突点の座標成分の大きさになる

                        &&

                        /***************************************/ //iが0,1,2のときlength[num2][0]はz,x,y方向の直方体の頂点の座標成分の大きさになる
                        (-length[num2][0] <= pos_.a[num2])

                        &&

                        ( pos_.a[num2]    <= length[num2][0])
                        /***************************************/ //iが0,1,2のときpos_.a[(num2]はz,x,y方向の直方体の衝突点の座標成分の大きさになる
                    )
                    {
                        /****************辺の長さが2になるようにx,y,z軸方向に伸ばしたりちぢめたり(定数倍)した座標を求める*******************/
                        hitx = pos_.a[0]/length[0][0];
                        hity = pos_.a[1]/length[1][0];
                        hitz = pos_.a[2]/length[2][0];
                        /*******************************************************************************************************/

                        double hitxyz[3]= {hitx, hity, hitz};//取り敢えず配列に格納する

                        hit0.hitmaterial = (ppattern_m[i][j])->getmaterial(hitxyz[num1],hitxyz[num2]);//求めた配列からMaterialを求める

                        if(hit0.hitmaterial->isVacuum() == 0)////Vacuumでなければ
                        {
                            hit0.ifhit = 1;                                                                                             //当たったので
                            hit0.hitpos = pos_;                                                                                         //
                            hit0.hitdistance = t;                                                                                       //
                            hit0.hitnormal = normal_[i][j];
                            hit0.hitlight = (ppattern_l[i][j])->getlight(hitxyz[num1],hitxyz[num2]);
                            //hit0.hitmaterialは取得済み
                            //return;
                        }
                        else//Vacuumなら
                        {
                            //何もしない もしifhit = 0としてしまうと他の面で衝突した場合にその面で衝突しなかったと扱われてしまうため
                            //return;してはいけない なぜなら他の平面で衝突判定を飛ばしてしまう可能性があるため
                        }


                        cos = std::abs(dot(ray.direction, normal_[i][j]));
                        pdfsum_nee_1_obj += t*t / (2 * length[num1][0] * 2 * length[num2][0] *cos)*(1.0/6.0);// 衝突判定があろうとなかろうとサンプリングされるため

                    }
                }
            }   
        }
    }

    void Cuboid::intersect_nee(const Ray& ray, Hit& hit0, double& pdfsum_nee_1_obj) const
    {
        Ray rotatedray = changeray(ray);//基準の位置になるようにrayも一緒に移動する
        intersect0_nee(rotatedray, hit0, pdfsum_nee_1_obj);//rayを当てる
        hit0.hitpos =  rechangeposition(hit0.hitpos);
        hit0.hitnormal = rechangedirection(hit0.hitnormal);
    }

    double Cuboid::getpdf_nee_1_obj() const
    {
        return 1 / ((x_length*y_length + y_length*z_length + z_length*x_length)* 2);
    }

    bool Cuboid::ifinside0(Vec3 p)
    {
        double x = 0.5*x_length;
        double y = 0.5*y_length;
        double z = 0.5*z_length;

        double x_ = p.x;
        double y_ = p.y;
        double z_ = p.z;

        if((-x<x_)&&(x_<x)&&(-y<y_)&&(y_<y)&&(-z<z_)&&(z_<z))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

    bool Cuboid::ifinside(Vec3 p)
    {
        p = changeposition(p);
        
        return ifinside0(p);
    }

    Vec3 Cuboid::changeposition(Vec3 p) const
    {
        p -= Center;
        p = changemat * p;
        return p;
    }

    Vec3 Cuboid::rechangeposition(Vec3 p) const
    {
        p = rechangemat * p;
        p += Center;
        return p;
    }

    Vec3 Cuboid::changedirection(Vec3 p) const
    {
        p = changemat * p;
        return p;
    }

    Vec3 Cuboid::rechangedirection(Vec3 p) const
    {
        p = rechangemat * p;
        return p;
    }

    Ray Cuboid::changeray (Ray ray)const
    {
        ray.origin = ray.origin - Center;
        ray = rotateray(ray, changemat);

        return ray;
    }

    Ray Cuboid::rechangeray (Ray ray)const
    {
        ray = rotateray(ray, rechangemat);
        ray.origin = ray.origin + Center;

        return ray;
    }

    void Cuboid::getposition(Hit& hit) const//hit.hitpos, hit.hitnormal, hit.pdf, hit.hitlight, hit.hitmaterialを求める
    {

        double x = 0.5 * x_length;//
        double y = 0.5 * y_length;//見やすくするため
        double z = 0.5 * z_length;//

        double X = x_length;//
        double Y = y_length;//見やすくするため
        double Z = z_length;//

        Vec3 position;//基準座標での位置
        Vec3 normal;//基準座標での法線ベクトル

        int k;
        
        double a = (rnd() * (x_length*y_length + y_length*z_length + z_length*x_length)*2);//乱数

        if(a < x_length*y_length)// | + |   |   |   |   |   |
        {
            k = 0;
            goto p1;
        }

        a -= x_length*y_length;

        if(a < x_length*y_length)// |   | + |   |   |   |   |
        {
            k = 1;
            goto p1;
        }

        a -= x_length*y_length;

        if(a < y_length*z_length)// |   |   | + |   |   |   |
        {
            k = 2;
            goto p1;
        }

        a -= y_length*z_length;

        if(a < y_length*z_length)// |   |   |   | + |   |   |
        {
            k = 3;
            goto p1;
        }

        a -= y_length*z_length;

        if(a < z_length*x_length)// |   |   |   |   | + |   |
        {
            k = 4;
            goto p1;
        }

        a -= z_length*x_length;

        if(a <= z_length*x_length)// |   |   |   |   |   | + |
        {
            k = 5;
            goto p1;
        }

        a -= z_length*x_length;

        if(0 < a)
        {
            std::cout << "cuboidの6面のうちの1面の選択の失敗" << std::endl;
            exit(1);
        }



        p1://ここにジャンプする


        double s = 2.0 * rnd() - 1.0;//乱数
        double t = 2.0 * rnd() - 1.0;//乱数


        switch (k)
        {
        case 0:
            position = Vec3(x, s * y, t * z);
            normal = Vec3(1,0,0);
            hit.pdf = (1/6.0)*(1/(Y*Z));
            hit.hitlight = ppattern_l[0][0]->getlight(s, t);
            hit.hitmaterial = ppattern_m[0][0]->getmaterial(s, t);
            break;

        case 1:
            position = Vec3(-x, s * y, t * z);
            normal = Vec3(-1,0,0);
            hit.pdf = (1/6.0)*(1/(Y*Z));
            hit.hitlight = ppattern_l[0][1]->getlight(s, t);
            hit.hitmaterial = ppattern_m[0][1]->getmaterial(s, t);
            break;

        case 2:
            position = Vec3(t * x, y, s * z);
            normal = Vec3(0,1,0);
            hit.pdf = (1/6.0)*(1/(Z*X));
            hit.hitlight = ppattern_l[1][0]->getlight(s, t);
            hit.hitmaterial = ppattern_m[1][0]->getmaterial(s, t);
            break;

        case 3:
            position = Vec3(t * x, -y, s * z);
            normal = Vec3(0,-1,0);
            hit.pdf = (1/6.0)*(1/(Z*X));
            hit.hitlight = ppattern_l[1][1]->getlight(s, t);
            hit.hitmaterial = ppattern_m[1][1]->getmaterial(s, t);
            break;

        case 4:
            position = Vec3(s * x, t * y, z);
            normal = Vec3(0,0,1);
            hit.pdf = (1/6.0)*(1/(X*Y));
            hit.hitlight = ppattern_l[2][0]->getlight(s, t);
            hit.hitmaterial = ppattern_m[2][0]->getmaterial(s, t);
            break;

        case 5:
            position = Vec3(s * x, t * y, -z);
            normal = Vec3(0,0,-1);
            hit.pdf = (1/6.0)*(1/(X*Y));
            hit.hitlight = ppattern_l[2][1]->getlight(s, t);
            hit.hitmaterial = ppattern_m[2][1]->getmaterial(s, t);
            break;

        default:
            std::cout << "cuboidのサンプリングでエラー発生" << std::endl;
            exit(1);
            break;
        }

        //この時点でhit.pdf, hit.hitlightは格納済み
        hit.ifhit = 1; //衝突点(有限の位置にある点)とみなす
        hit.hitpos = rechangeposition(position);
        hit.hitnormal = rechangedirection(normal);
        //この時点でhit.pdf, hit.hitlight, hit.hitpos, hit.hitnormalは格納済み

    }

    Vec3 Cuboid::pos() const//基準点に移動したときに原点にくる点がある座標を返す
    {
        return Center;
    }

    void Cuboid::moveShape(const Matrix3& rotatemat, const Matrix3& rerotatemat, const Vec3& movedirection)//回してから平行移動
    {
        Center = rotatemat * Center;                            //回して
        changemat = changemat * rerotatemat;//姿勢は回すだけ        //
        rechangemat = rotatemat * rechangemat;//姿勢は回すだけ      //

        Center += movedirection;//平行移動
    }

    int Cuboid::shapenum()const
    {
        return (int)SHAPES::CUBOID;
    }
/************************************************************************************************************************************************************************************/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/************Objectsの実装************************************************************************************************************************************************************/

    Objects::Objects()
    {
        for(int i = 0; i < (int)SHAPES::NUM_OF_SHAPES; i++)//初期化
        {
            objectnumber[i] = 0;
            objectnumber_nee[i] = 0;
        }
        objectnumbersum = 0;//初期化
        objectnumbersum_nee = 0;//初期化
    }

    void Objects::addshape(std::shared_ptr<Shapes> shape1)//番号を取得してShapeの種類で振り分ける
    {
        int shapenum = shape1->shapenum();//
        (shapes[shapenum]).push_back(shape1);
        (objectnumber[shapenum])++;
        objectnumbersum++;
    }

    void Objects::addshape_nee(std::shared_ptr<Shapes> shape1)//番号を取得してShapeの種類で振り分ける
    {
        int shapenum = shape1->shapenum();
        (shapes_nee[shapenum]).push_back(shape1);
        (objectnumber_nee[shapenum])++;
        objectnumbersum_nee++;
    }
/************************************************************************************************************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void moveStructure(Objects& obj0, Objects& obj, Matrix3 const& rotatemat, Matrix3 const& rerotatemat, Vec3 const& movedirection)//obj0の物体をobjに移す
{
    for(int j = 0; j < (int)SHAPES::NUM_OF_SHAPES; j++)
    {
        for(int i = 0; i < (obj0.objectnumber[j]); i++)
        {
            (obj0.shapes[j])[i]->moveShape(rotatemat, rerotatemat, movedirection);

            obj.addshape((obj0.shapes[j])[i]);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/**********Cylinderの実装*************************************************************************************************************************************************************/


    Cylinder::Cylinder(const double radius, const Vec3 position, const Vec3 direction, const double alpha): radius(radius)
    {
        double theta = gettheta(direction);
        double phi = getphi(direction);
        length = direction.length();

        rotatemat = getmatz(phi) * getmaty(theta) * getmatz(alpha);//求めておく
        rerotatemat = getmatz(-alpha) * getmaty(-theta) * getmatz(-phi);//求めておく
    
        movedirection = position;
    }

    Cylinder::Cylinder(const double radius, const double length, const Vec3 position, const double theta, const double phi, const double alpha): radius(radius), length(length)
    {
        rotatemat = getmatz(phi) * getmaty(theta) * getmatz(alpha);//求めておく
        rerotatemat = getmatz(-alpha) * getmaty(-theta) * getmatz(-phi);//求めておく
    
        movedirection = position;
    }

    Cylinder::Cylinder()
    {}

    void Cylinder::getstructure0(Objects& obj0) const//基準位置の物体を作成し、obj0に入れる
    {
        Pipe pipe = Pipe(radius, Vec3(0,0,0), Vec3(0,0,length), 0, q1, std::make_shared <M::Colorful_pipe> (p1) );
        Disk disk1 = Disk(radius, Vec3(0,0,0), 0, M_PI, 0, q1, std::make_shared <M::Colorful_disk> (p2) );
        Disk disk2 = Disk(radius, Vec3(0,0,length), 0, 0, 0, q1, std::make_shared <M::Colorful_disk> (p2) );

        obj0.addshape(std::make_shared<Pipe> (pipe));
        obj0.addshape(std::make_shared<Disk> (disk1));
        obj0.addshape(std::make_shared<Disk> (disk2));
    }


    void Cylinder::putStructure(Objects& objects)const///基準位置の物体を回転したものをobjectsに入れる(設置する)
    {
        Objects obj0;

        getstructure0(obj0);//回転前の物体をobj0に入れる

        moveStructure(obj0, objects, rotatemat, rerotatemat, movedirection);//物体配置(回してから)

    }

/************************************************************************************************************************************************************************************/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/******************Cylinder_centerの実装************************************************************************************************************************************************/


    Cylinder_center::Cylinder_center(const double radius, const double length, const Vec3 position, const double theta, const double phi, const double alpha): radius(radius), length(length)
    {
        rotatemat = getmatz(phi) * getmaty(theta) * getmatz(alpha);//求めておく
        rerotatemat = getmatz(-alpha) * getmaty(-theta) * getmatz(-phi);//求めておく
    
        movedirection = position;
    }

    Cylinder_center::Cylinder_center()
    {}

    void Cylinder_center::getstructure0(Objects& obj0) const//基準位置の物体を作成し、obj0に入れる
    {
        Pipe pipe = Pipe(radius, Vec3(0,0,-length/2.0), Vec3(0,0,length), 0, q1, std::make_shared <M::Colorful_pipe> (p1) );
        Disk disk1 = Disk(radius, Vec3(0,0,-length/2.0), 0, M_PI, M_PI, q1, std::make_shared <M::Colorful_disk> (p2) );
        Disk disk2 = Disk(radius, Vec3(0,0,length/2.0), 0, 0, 0, q1, std::make_shared <M::Colorful_disk> (p2) );

        obj0.addshape(std::make_shared<Pipe> (pipe));
        obj0.addshape(std::make_shared<Disk> (disk1));
        obj0.addshape(std::make_shared<Disk> (disk2));
    }

    void Cylinder_center::putStructure(Objects& objects)const///基準位置の物体を回転したものをobjectsに入れる(設置する)
    {
        Objects obj0;

        getstructure0(obj0);//回転前の物体をobj0に入れる

        moveStructure(obj0, objects, rotatemat, rerotatemat, movedirection);//物体配置(回してからobjにいれる)

    }

/************************************************************************************************************************************************************************************/



////////////////////////////////////////////////////////////////////////////////////////////////

Vec3 AtMost255(Vec3 v)//最大の値を255にする
{
    Vec3 v1 = vabs(v);
    double a  = std::max({v1.x, v1.y, v1.z});
    if(a != 0)//ゼロ除算を避ける
    {
        return v1*255/a;
    }
    else
    {
        return Vec3(255, 255, 255);
    }
    
    
}

/*************Accelの実装*************************************************************************************************************************************************************/

    Accel::Accel()
    {}

    void Accel::intersects(const Ray& ray, const Objects& objects, Hit& hit)
    {
        Hit hit0;//仮のhitnomal, hitposを格納

        for(int j = 0; j < (int)SHAPES::NUM_OF_SHAPES; j++)
        {
            for(int i = 0; i < objects.objectnumber[j]; i++)//物体を全て調べる
            {

                
                (objects.shapes[j])[i]->intersect(ray, hit0);//ifhitの値と hitpos, hitnormalの仮の値を取得
                
                if(hit0.ifhit == 1)//もし当たったら
                {

                    if(hit0.hitdistance < hit.hitdistance)//これで最も近い物体の距離を得る
                    {
                       
                        hit.hitdistance = hit0.hitdistance;//物体距離の更新
                        hit.ifhit       = (1);
                        hit.ifhit_light = (0);
                        hit.hitnormal   = hit0.hitnormal;//とりあえず法線ベクトルを保存
                        hit.hitpos      = hit0.hitpos;//とりあえず位置を保存
                        hit.hitlight    = hit0.hitlight;
                        hit.hitmaterial = hit0.hitmaterial;
                    }
                    else if(hit0.hitdistance >= hitdistance0)
                    {
                        std::cout  <<"distance0の値が小さすぎる" << std::endl;//表示
                        exit(1);//離脱
                    }
                    else
                    {
                        //何もしない
                    }
                }
                else//もし当たらなかったら
                {
                    //何もしない
                }
            } 
        }
    }

    void Accel::intersects_nee1(const Ray& ray, const Objects& objects, Hit& hit)//neeの物体に当たらないかどうか確認
    {
        Hit hit0;
        
        for(int j = 0; j < (int)SHAPES::NUM_OF_SHAPES; j++)
        {
            for(int i = 0; i < objects.objectnumber_nee[j]; i++)//物体を順番に調べる
            {
                (objects.shapes_nee[j])[i]->intersect(ray, hit0);//ifhitの値と hitpos, hitnormalの仮の値を取得
                
                if(hit0.ifhit == 1)//もし当たったら
                {
                    if(hit0.hitdistance < hit.hitdistance)//これで最も近い物体の距離を得る
                    {  
                        hit.hitdistance = hit0.hitdistance;//物体距離の更新
                        hit.ifhit       = (1);
                        hit.ifhit_light = (1);
                    }
                    else if(hit0.hitdistance >= hitdistance0)
                    {
                        std::cout  <<"distance0の値が小さすぎる" << std::endl;//表示
                        exit(1);//離脱
                    }
                    else
                    {
                        //何もしない
                    }
                }
                else//もし当たらなかったら
                {
                    //何もしない
                }
            }  
        }
    }

    void Accel::intersects_nee2(const Ray& ray, const Objects& objects, Hit& hit, double& pdfsum_nee)
    {
        Hit hit0;//仮のhitnomal, hitposを格納
        double pdfsum_nee_1_obj;
        double pdf_nee_1_obj;
 

        for(int j = 1; j < (int)SHAPES::NUM_OF_SHAPES; j++)
        {
            for(int i = 0; i < objects.objectnumber_nee[j]; i++)//物体を全て調べる
            {
                (objects.shapes_nee[j])[i]->intersect_nee(ray, hit0, pdfsum_nee_1_obj);//ifhitの値と hitpos, hitnormalの仮の値を取得
                
                if(hit0.ifhit == 1)//もし当たったら
                {
                    if(hit0.hitdistance<hit.hitdistance)//これで最も近い物体の距離を得る
                    {
                        hit.hitdistance = hit0.hitdistance;//物体距離の更新
                        hit.ifhit       = (hit.ifhit||1);
                        hit.ifhit_light = (hit.ifhit_light||1);
                        hit.hitpos      = hit0.hitpos;//とりあえず位置を保存
                        hit.hitnormal   = hit0.hitnormal;//とりあえず法線ベクトルを保存
                        hit.hitlight    = hit0.hitlight;
                        hit.hitmaterial = hit0.hitmaterial;
                        pdf_nee_1_obj   = (objects.shapes_nee[j])[i]->getpdf_nee_1_obj();
                    }
                    else if(hit0.hitdistance >= hitdistance0)
                    {
                        std::cout  <<"distance0の値が小さすぎる" << std::endl;//表示
                        exit(1);//離脱
                    }
                    else
                    {
                        //何もしない
                    }
                }
                else//もし当たらなかったら
                {
                    //何もしない
                }
            }    
        }
        
        pdfsum_nee = pdfsum_nee_1_obj * (1.0 / objects.objectnumbersum_nee); //ここでpdfsum_neeを求める (物体を選ぶ確率が全て等しいから)
        hit.pdf_nee = pdf_nee_1_obj * (1.0 / objects.objectnumbersum_nee); //ここでpdf_neeを求める (物体を選ぶ確率が全て等しいから)
    }
/************************************************************************************************************************************************************************************/