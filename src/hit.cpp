#include "../include/hit.h"

double const hitdistance0 = 1000000000000000000;//様々なcppファイルでconstの数値を用いてそれらを分割コンパイルしたいが,ヘッターに数値の定義を書きたくない...   そのような場合にはextern宣言をヘッダーに書いてcppにインクルードさせ, 1つのcppで数値を定義すれば良い

Hit::Hit()
{}

void Hit::set(bool const& ifhit_, bool const& ifhit_light_, double const& hitdistance_)
{
    ifhit = ifhit_;
    ifhit_light = ifhit_light_;
    hitdistance = hitdistance_;
}
void Hit::setbool(bool const& ifhit_, bool const& ifhit_light_)
{
    ifhit = ifhit_;
    ifhit_light = ifhit_light_;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
    int neg0(int const& i)  //0<=iならi,  i<0なら0を返す    neg(負数)が0になるという意味を表すneg0
    {
        return (std::abs(i) + i)/2;
    }
*/

Subpath::Subpath(int const& num_):num(num_)
{
    hit_eye = new Hit[num];
    hit_light = new Hit[num];

    num_eye = 0;
    num_light = 0;

}

Subpath::~Subpath()
{
    delete[] hit_eye;
    hit_eye = NULL;


    delete[] hit_light;
    hit_light = NULL;
}

void Subpath::addeye(Hit const & hit, int const & num_)
{
    if(num_ < num)//配列の領域を超えなければ
    {
        if(num_eye == num_)//numは保存されている点の個数, num_は今回代入する配列の番号(配列が前回の1つ後ろに代入されるなら)
        {
            hit_eye[num_] = hit;
            num_eye++;
        }
        else if(num_eye < num_)//サブパスが1つ以上飛ばしたら
        {
            std::cout << "視線サブパスの" << num_eye << "番を飛ばし, " << num_ << "番に入力しようとしていた" << std::endl;
            exit(1);
        }
        else//サブパスの前より小さい番号に入力しようとしたら
        {
            std::cout << "前回は" << num_eye - 1 << "番に入力したが, 視線サブパスが途中で戻り, " << num_ << "番に入力しようとしていた" << std::endl;
            exit(1);
        }
        
        
    }
    else//配列の領域を超えれば
    {
        std::cout << "視線サブパスの最大の番号" << num - 1 <<"を超え, " << num_ << "番に入力しようとしていた" << std::endl;
        exit(1);
    }
    
}

void Subpath::addlight(Hit const & hit, int const& num_)
{
    if(num_ < num)//配列の領域を超えなければ
    {
        if(num_light == num_)//numは保存されている点の個数, num_は今回代入する配列の番号(配列が前回の1つ後ろに代入されるなら)
        {
            hit_light[num_] = hit;
            num_light++;
        }
        else if(num_eye < num_)//サブパスが1つ以上飛ばしたら
        {
            std::cout << "光線サブパスの" << num_light <<"番を飛ばし, " << num_ << "番に入力しようとしていた" << std::endl;
            exit(1);
        }
        else//サブパスの前より小さい番号に入力しようとしたら
        {
            std::cout << "前回は" << num_light - 1 << "番に入力したが, 光線サブパスが途中で戻り, " << num_ << "番に入力しようとしていた" << std::endl;
            exit(1);
        }
        
        
    }
    else//配列の領域を超えれば
    {
        std::cout << "光線サブパスの最大の番号" << num - 1 <<"を超え, " << num_ << "番に入力しようとしていた" << std::endl;
        exit(1);
    }
    
}

void Subpath::reset()
{
    num_eye = 0;
    num_light = 0;
}

//                  (視線サブパスと光線サブパスを接続する点)までの配列の個数  サブパスを短縮できる配列の最小個数   サブパスを短縮できる配列の最小個数
double Subpath::misweight(int const& length_eye, int const& length_light, int const& min_eye, int const& min_light, double const& pdf_eye_reverse1, double const& pdf_eye_reverse2, double const& pdf_light_reverse1, double const& pdf_light_reverse2, double const& roulette_reverse_eye, double const& roulette_reverse_light)
{
    double densityratio;
    double recmisweight = 0;//初期化
    
    /********************サブパスが存在しなかった場合と負の値であった場合を排除する*****************/
        if((length_eye < 0) || (length_light < 0))
        {
            std::cout << "少なくとも一方のサブパスの個数が負の値" << std::endl;
            exit(1);
        }
        else if((length_eye == 0) && (length_light == 0))
        {
            std::cout << "どちらのサブパスも存在しない" << std::endl;
            exit(1);   
        }
    /**************************************************************************************/


    //サブパスが少なくとも一方存在すれば
    /*******************misweightを求める************************************/
    {
        if(length_light > 0)//光線サブパスが存在する場合のみ行う
        {
            densityratio = 1;//初期化
            for(int i = length_light-1; (min_light <= i); i--)//光線サブパスを短縮しながら視線サブパスを延長する場合の確率密度を求める
            {
                Hit const* const h_l = &(hit_light[i]);//変数名が長すぎるので短く表すためにポインタに格納する


                /************************************確率密度関数の比を求める*********************************************************************/
                    //光線サブパスの確率密度関数(光線側に保存されている)で割り, 視線サブパスの延長確率密度関数(光線側に保存されている)をかける
                    if(i == length_light-1)//最初は
                    {
                        densityratio *= (pdf_light_reverse1 * roulette_reverse_light) / (h_l->pdf * h_l->roulette);//最初はサブパスの接続時に求めた逆方向の確率密度関数を用いる ロシアンルーレットの確率も接続してから確定する
                    }
                    else if(i == length_light-2)//2回目は
                    {
                        densityratio *= (pdf_light_reverse2 * h_l->roulette_reverse) / (h_l->pdf * h_l->roulette);//2回目もサブパスの接続で求めた逆方向の確率密度関数を用いる ロシアンルーレットの確率はサブパス接続前から確定している
                    }
                    else//それ以降は
                    {
                        densityratio *= (h_l->pdf_reverse * h_l->roulette_reverse) / (h_l->pdf * h_l->roulette);//サブパスの接続前から確定している確率密度関数, ロシアンルーレットの確率は配列に格納されている
                    }
                /****************************************************************************************************************************/

                if(2 <= i)//iが2以上のときは接続する端点がスペキュラー(少なくとも一方)の場合はサンプルの1つとしない(サンプルの1つとしてもサンプルは必ず0になってしまうので計算の効率が悪くなると考えられるため)
                {

                    if((h_l->hitmaterial->isSpecular() == 0) && ((h_l-1)->hitmaterial->isSpecular() == 0))
                    {
                        recmisweight += abc(densityratio);
                    }
                    else
                    {
                        recmisweight += 0;//加算しない
                    }
                }
                else if(i == 1)// この場合は//サブパスの接続前から確定している確率密度関数, ロシアンルーレットの確率は配列に格納されている光線サブパスは光源上にしかないため, 平行光でもない限り常に0となることはないのでサンプルの1つとする 平行光の場合を後で実装せよ
                {
                    if(h_l->hitmaterial->isSpecular() == 0)
                    {
                        recmisweight += abc(densityratio);
                    }
                    else
                    {
                        recmisweight += 0;//加算しない
                    }
                }
                else//iが0のときは光線サブパスを構築しないのでSpecular面でもサンプルの1つとして加算する(サブパスを無理やり接続しないため, サンプルの1つとしても値が常に0になるとは限らない)  平行光の場合を後で実装せよ
                {
                    recmisweight += abc(densityratio);
                }

            }
        }
        recmisweight += 1;//サブパスをつなぎかえない場合の確率密度の比  


        if(length_eye > 0)//視線サブパスが存在する場合のみ行う
        {
            densityratio = 1;//初期化
            for(int i = length_eye-1; (min_eye <= i); i--)//視線サブパスを短縮しながら光線サブパスを延長する場合の確率密度を求める
            {
                Hit const* const h_e = &(hit_eye[i]);//変数名が長すぎるので短く表すためにポインタに格納する

                /************************************確率密度関数の比を求める*********************************************************************/
                    //視線サブパスの確率密度関数(視線側に保存されている)で割り, 光線サブパスの延長確率密度関数(視線側に保存されている)をかける
                    if(i == length_eye-1)//最初は
                    {
                        densityratio *= (pdf_eye_reverse1 * roulette_reverse_eye) / (h_e->pdf * h_e->roulette);//最初はサブパスの接続時に求めた逆方向の確率密度関数を用いる ロシアンルーレットの確率も接続してから確定する
                    }
                    else if(i == length_eye-2)//2回目は
                    {
                        densityratio *= (pdf_eye_reverse2 * h_e->roulette_reverse) / (h_e->pdf * h_e->roulette);//2回目もサブパスの接続で求めた逆方向の確率密度関数を用いる ロシアンルーレットの確率はサブパス接続前から確定している
                    }
                    else//それ以降は
                    {
                        densityratio *= (h_e->pdf_reverse * h_e->roulette_reverse) / (h_e->pdf * h_e->roulette);//サブパスの接続前から確定している確率密度関数, ロシアンルーレットの確率は配列に格納されている
                    }
                /******************************************************************************************************************************/


                if(2 <= i)//iが2以上のときは接続する端点がスペキュラー(少なくとも一方)の場合はサンプルの1つとしない(サンプルの1つとしてもサンプルは必ず0になってしまうので計算の効率が悪くなると考えられるため)
                {
                    if((h_e->hitmaterial->isSpecular() == 0) && ((h_e-1)->hitmaterial->isSpecular() == 0)) 
                    {
                        recmisweight += abc(densityratio);
                    }
                    else
                    {
                        recmisweight += 0;//加算しない
                    }
                }
                else if(i == 1)// この場合は視線サブパスはセンサー上にしかないため, 受け取る光の寄与をデルタ関数で受け取るような特殊な場合でもない限り常に0となることはないのでサンプルの1つとする このような場合はmin_eyeを1にすれば良い
                {
                    if(h_e->hitmaterial->isSpecular() == 0)
                    {
                        recmisweight += abc(densityratio);
                    }
                    else
                    {
                        recmisweight += 0;//加算しない
                    }
                }
                else//iが0のときは光線サブパスを構築しないのでSpecular面でもサンプルの1つとして加算する(サブパスの接続を行わないため, サンプルの1つとしても値が常に0になるとは限らない)
                {
                    recmisweight += abc(densityratio);
                }

            }
        }
    }
    /*******************************************************************/
    
    
    /**********************misweightが適切な値になっているのか*****************/
    if(recmisweight > 0)
    {
        return 1/recmisweight;
    }
    else
    {
        std::cout << "misweightの逆数が0以上の値でない" << std::endl;
        exit(1);
    }
    /*******************************************************************/
}