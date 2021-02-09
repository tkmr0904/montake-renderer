#include <errno.h>
#include "../include/image.h"

/*テンプレート関数の定義はヘッダーに書くべし(コンパイル時に完全な関数が作られていなければならないため, 後でリンクすることができない)*/
template<typename Out> void input(Out const& out, double& parameter)//outを表示してから parameterにdouble型を入力する
{
    
    bool a;//変換できたかどうか
    std::string str1;//入力される文字列
    char* str2;//変換前の文字列
    char* c;//エラー判定に用いる

    do//文字列を数値に変換する処理
    {   
        std::cout << out << std::endl;//outを表示

        std::cin >> str1;//文字を入力

        str2 = (char*)str1.c_str();//char*型に変換

        errno = 0;//エラー判定に用いる

        parameter = strtod(str2, &c);//文字列strをdouble型に変換してparemeterに代入する


        if(errno == ERANGE)//数値が大きすぎたり小さすぎたりして変換できない場合
        {
            a = 1; //変換できていない   (parameterに数値が代入されていない)
            //ループ
        }
        else if(str2 == c)//入力されたものの先頭が数値でなくて変換できない場合
        {
            a = 1;//変換できていない    (parameterに数値が代入されていない)
            //ループ
        }
        else//変換できた場合
        {
            a = 0;//変換できた          (parameterに数値が代入されている)
            //ループを抜ける
        }

    }while(a == 1);//適切な値が入力されなければループ

    //  この時点で(parameterに数値が代入されている)
}

/****************物体を指定した個数だけランダムに追加する*******************************************************************************************************************/
    void randputsphere(int number_of_sphere, double radius_min, double radius_max, Objects& objects, double radius, L::sharePattern ppattern_l, M::sharePattern ppattern);
    void randputpipe(Objects& objects, int number_of_pipe, L::sharePattern ppattern_l, M::sharePattern ppattern);
/*********************************************************************************************************************************************************************/

void draw(int const& i, Image& image, Camera const& camera, Sky const& sky, Objects const& objects, bool const& auto_);