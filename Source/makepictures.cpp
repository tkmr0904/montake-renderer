#include <iostream>
#include <math.h>
#include "../Header/draw.h"


#define   share(a,b)  std::make_shared<a>(b) //a: 型名,  b: オブジェクト ShapeをObjectsに追加するときに便利
void setobjects(Objects& objects)//物体の情報をobjectsに書き込む
{
    std::cout << "▼物体作成開始▼" << std::endl;//表示

    /**************物体表面の情報(光の反射に関する)******************************************************************/
        M::sharePattern q1  = M::pPlain(  M::Diffuse(Vec3(1,1,1))            );       
        M::sharePattern q2  = M::pPlain(  M::Diffuse(Vec3(0.0,0.0,1.0))      );
        M::sharePattern q3  = M::pPlain(  M::Diffuse(Vec3(1.0,0.0,0.0))      );
        M::sharePattern q4  = M::pPlain(  M::Diffuse(Vec3(0.0,1.0,0.0))      );
        M::sharePattern q5  = M::pPlain(  M::Diffuse(Vec3(0,0,0))            );
        M::sharePattern q6  = M::pPlain(  M::Diffuse(Vec3(0.5,0.5,1.0))      );
        M::sharePattern q7  = M::pPlain(  M::Diffuse(Vec3(1.0,0.5,0.5))      );
        M::sharePattern q8  = M::pPlain(  M::Diffuse(Vec3(0.5,1.0,0.5))      );
        M::sharePattern q9  = M::pPlain(  M::Mirror(Vec3(1,1,1))             );
        M::sharePattern q10 = M::pPlain(  M::Glass(1.5,1,0.5)               );
        M::sharePattern q11 = M::pPlain(  M::Glass(1,2.42,0.5)               );
        M::sharePattern q12 = M::pPlain(  M::Vacuum()                        );


        M::sharePattern q13 =  M::pColorful_cuboid();
        M::sharePattern q14 =  M::pDiskHole_cuboid(0.2);
        M::sharePattern q15 =  M::pColorful_pipe();
        M::sharePattern q16 =  M::pColorful_disk();
        M::sharePattern q17 =  M::pColorful_triangle();
        M::sharePattern q18 =  M::pColorful_sphere();
        M::sharePattern q19 =  M::pPicture_sphere("../Photo/moon2.png");
        //M::sharePattern q20 = share(M::Picture_disk, M::Picture_disk("../Photo/clock.ppm"));
        M::sharePattern q21 =  M::pPicture_rectangle("../Photo/a.ppm");
    /***************************************************************************************/


    /****************物体表面の情報(発光に関する)********************************************/
        L::sharePattern r1 = L::pPlain(L::Uniform(20*Vec3(255,255,255)));
        L::sharePattern r2 = L::pPlain(L::Uniform(Vec3(0,0,0)));
        L::sharePattern r3 = L::pPlain(L::Uniform(5*Vec3(255,255,255)));
        L::sharePattern r4 = L::ppattern_disk();
    /***************************************************************************************/


    /*****************物体を設定してobjectsに格納*******************************************************************************/
        objects.addshape(share(Cuboid, Cuboid(200,200,200, Vec3(0,0,0), 0,0,0, r2, r2, r2, r2, r2, r2, q4, q4, q3, q3, q14, q21)));
        objects.addshape_nee(share(Disk, Disk(20, Vec3(0,0,100+3), M_PI*0, M_PI*1, M_PI*0,  r1, q1)));
        objects.addshape(share(Pipe, Pipe(20, Vec3(0,0,100), Vec3(0,0,3), 0, r2, q2)));
        objects.addshape(share(Pipe, Pipe(20, Vec3(100,0,50), Vec3(-3,0,0), 0, r2, q2)));
        objects.addshape(share(Disk, Disk(20, Vec3(100-3,0,50), M_PI*1, M_PI*0.5, M_PI*0,  r2, q2)));
    objects.addshape(share(Sphere, Sphere(30, 0, M_PI*1, 0, Vec3(0, 30*2/sqrt(3),-70), r2, q19)));
    objects.addshape(share(Sphere, Sphere(30, 0, M_PI*1, 0, Vec3(-30,-30  /sqrt(3),-70), r2, q19)));
    objects.addshape(share(Sphere, Sphere(30, 0, M_PI*1, 0, Vec3(30,-30  /sqrt(3),-70), r2, q19)));
    objects.addshape(share(Sphere, Sphere(30, 0, M_PI*1, 0, Vec3(0,0,-70+30*sqrt(8/3)), r2, q19)));
        //objects.addshape(share(Pipe, Pipe(40, Vec3(0,0,100), Vec3(0,0,5), 0, Vec3(0,0,0), q2)));
    //objects.addshape(share(Cuboid, Cuboid(70,70,70, Vec3(0,0,0), 0,0,0, r2, r2, r2, r2, r2, r2, q10, q10, q10, q10, q10, q10)));
            //objects.addshape(share(Sphere,Sphere(60, 0, 0, 0, Vec3(0,0,-10), r2, q10)));
        //    objects.addshape(share(Sphere,Sphere(21, 0, 0, 0, Vec3(0,0,100), r2, q10)));

        //objects.addpipe(share(Pipe, Pipe(20, Vec3(0,0,-20), Vec3(0,0,40), 0, Vec3(0,0,0), q15)));

        //objects.addsphere(std::make_shared<Sphere>(Sphere(35, 0, 0, 1.2*M_PI,Vec3(0,0,0), Light(Vec3(0,0,0)), q19)));
        //objects.addshape(share(Sphere, Sphere(30, 0, 0, 1.2*M_PI,Vec3(0,0,0), Light(Vec3(0,0,0)), q18)));
    /************************************************************************************************************************/


    std::cout << "▲物体作成終了▲" << std::endl << std::endl;//表示
}


int main(int argc,char *argv[])
{
    /*******************4行の白い太線を表示************************************/
        std::cout << (
                    "\033[1;37m--------------------------------------------------------------------------------------------------------------------------------------\n"
                              "--------------------------------------------------------------------------------------------------------------------------------------\n"
                              "--------------------------------------------------------------------------------------------------------------------------------------\n"
                              "--------------------------------------------------------------------------------------------------------------------------------------\n"
                    "\033[m"
                    );
    /***********************************************************************/

    for(int i = 0; i <= 0; i++)//複数画像を描画する
    {
        

        std::cout << ("\033[1;33m---------------------------------------------------------------------------------------------------------\n---------------------------------------------------------------------------------------------------------\n \033[m");

        /***自動でループを回すか******/
            bool auto_ = 0;
        /**************************/

        /*****************************imageの設定***************************************************************************/
            int width;
            int height;
            int s;

            s = 1;

            switch(s)//画像の縦, 横を決める
            {
                case 1:
                width = 1440;
                height = 2560;
                break;
                
                case 2:
                width = 360;
                height = 640;
                break;                

                case 3:
                width = 144;
                height = 256;
                break;

                case 4:
                width = 72;
                height = 128;
                break;

                case 5:
                width = 60;
                height = 106;
                break;

                case 6:
                width = 1366;
                height = 768;
                break;

                case 7:
                width = 683;
                height = 384;
                break;

                case 8:
                width = 1000;
                height = 1000;
                break;

                case 9:
                width = 100;
                height = 100;
                break;

                case 10:
                width = 200;
                height = 100;
                break;

                default:
                width = 1000;
                height = 1000;
                break;
            }

            double p_eye   = 0.5;
            double p_light = 0.5;
            bool ifnee;
            bool ifeye;
            bool iflight;
            int p_eye_1_num = 4;  //サブパスで無限遠の点が生成されないにならない限り絶対にこの個数まで点が延長される
            int p_light_1_num =4;//サブパスで無限遠の点が生成されないにならない限り絶対にこの個数まで点が延長される

            s = 1;

            switch (s)//サンプルのとり方を決める
            {
                case 0:
                    ifeye = 1;
                    ifnee = 1;
                    iflight = 0;
                    break;

                case 1:
                    ifeye = 1;
                    ifnee = 0;
                    iflight = 1;
                    break;

                case 2:
                    ifeye = 1;
                    ifnee = 0;
                    iflight = 0;
                    break;

                default:
                    std::cout << "switchのsの値がおかしい" << std::endl;
                    exit(1);
            }


            


            Image image =Image(width, height, p_eye, p_light, p_eye_1_num, p_light_1_num, ifnee, ifeye, iflight);//画像の横と縦,平均回数
        /*******************************************************************************************************************/

        /*******************************************カメラを設定する***********************************************************************************************/
            double holedistance = 1.44;//0.54;
            double distance = 105-3;//135;

            //最初はカメラから見たとき   前がz軸(ワールド座標)の正の方向, 右がx軸(ワールド座標)の正の方向, 下がy軸(ワールド座標)の正の方向 であるとする   y軸方向の正の向きにを向きながらxy平面上に立って真上を向くイメージ
            //これからalpha, theta, phiで回転する
            double alpha = 0.5*M_PI;                      //x軸方向の負の向きに立って真上を向くイメージ
            double theta = (0.62-0.04)*M_PI; // (0.52)*M_PI;   //x軸方向に背中を向け, x軸の正の方向に体を反らすイメージ
            double phi   = (2.0*i/360)*M_PI;    //(1.2)*M_PI;      //極座標のphiの方向に背中を向け, 体を真後ろに反らすイメージ

            double ratio = 1/80.0;//レンズの倍率
            double D = distance;//ピントを合わせる距離
            double focallength = ratio*D/(ratio+1)/(ratio+1);   //ratioとDからレンズの距離と焦点距離を求める
            double lensdistance = ratio*D/(ratio+1);            //
            double lensradius = 1.5;//レンズの絞りの大きさ   大きいほどボケやすい

            double sensorsensitivity = 13000000;

            PinholeCamera camera = PinholeCamera(width, height, angle_to_direction(theta, phi)*-distance,  alpha  , theta, phi, holedistance, 0.1, sensorsensitivity);//カメラ
            //ThinLensCamera camera = ThinLensCamera(width, height, angle_to_direction(theta, phi)*-distance, alpha, theta, phi, lensradius, lensdistance, focallength, sensorsensitivity);

            //width, height, angle_to_direction(theta, phi)*-distance, theta, phi, lensradius, lensdistance, focallength, sensorsensitivityを使う
        /*******************************************************************************************************************************************************/

        /*******************無限遠からやってくる光の色を設定する*********************/
            //IBL sky("../Photo/Mono_Lake_C_Ref.hdr");
            //IBL sky("../Photo/Ridgecrest_Road_Ref.hdr");
            //UniformSky sky = UniformSky(Vec3(128,128,255));
            UniformSky sky = UniformSky(20*Vec3(255,255,255));
        /**********************************************************************/

        /*********物体の作成****************/
            Objects objects;
            setobjects(objects);//物体の作成
        /*********************************/

        draw( i , image, camera, sky, objects, auto_);//画像を1つ描画する
        // ファイル番号 
    }

}