#include <string>
#include <fcntl.h>
#include <termios.h>
#include <sstream>
#include "../Header/vec.h"

template <typename a> std::string arrayn(a out[], int const& num_array)
{
    std::stringstream stream;

    stream << '(' ;

    for(int i = 0; i < num_array -1; i++)
    {
        stream << std::to_string(out[i]) + ',';
    }

    stream << std::to_string(out[num_array - 1]) + ')';
    
    return stream.str();
}

/*******************ファイル内の文字の塊を読み込む処理******************************************************/
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
/*****************************************************************************************************/

/************************ppmファイルを読み込む処理*********************************************************/
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
            std::cout << "ファイルを閉じることができなかった" << std::endl;
            exit(1);
        }

        std::cout << "読み込み終了" << std::endl;
    }


    void loadppm(const std::string str, int& mode, int& imagewidth, int& imageheight, int& maxbrightness,  std::shared_ptr<Vec3[]>& color)
    {
        using namespace std::literals::string_literals;

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
/*****************************************************************************************************/

/******************様々な型を任意の個数表示できるようにする****************************************/
    void p()//これがないとコンパイラが危ないと判断するらしい(restの要素数が0でp(rest...)が実行されるとfirstに値が渡されない)
    {
        std::cout << std::endl;
    }

    template <typename Tfirst, typename ... Trest> void p(Tfirst first, Trest ... rest)
    {
        std::cout << first;

        if(sizeof...(rest) > 0)
        {
            std::cout << ", ";
            p(rest...);
        }
        else
        {
            std::cout << std::endl;
            return;
        }
    }
/*******************************************************************************************/

/****************outputに入力した任意の個数の様々な型の値を表示する****************************************************/
    //...はパックを展開してカンマ区切りにしたものと同等であると考えて良い

    template <typename... T, std::size_t... n>//引数をもとにして型のパックを推測してこれを関数内で使う        例えば   int a,   double b,   char c
            //   ^             ----^----                                                                     ^   ^      ^    ^     ^   ^
            //   ^              型名がstd::size_tである変数がカンマ区切りで並んだパック(仮に名付けて)n                 |  |      |     |     |   |
            //---^---                                                                                        |  L------|-----L-----|---L-----------変数パック
            //様々なtypemane(型名)がカンマ区切りで並んだパック(仮に名付けて)T                                        |         |           |    
                                                                       //                                    L---------L-----------L---------------型パック
    void output0(std::index_sequence<n...>, T... _out)//パラメーターパック_outと整数の列nを受取り, 出力する          
            //          ^                      ----^----                                                             
            //          ^                 型パックTに対する変数パック_out                                                 
            //----------^----------                                                                           
            //   型名しか必要ない(関数の中で使われない)ので変数名は不要(ここで欲しいのはn...)                                                    
            //                                                                                                      
    {
        std::tuple<T...> out(_out...);//outは1つのインスタンス  パラメーターパック_outの要素を左から配列のようにしたもの
        using swallow = int[];      //ここでの役割はtypedefと同じようなもの
        (void)swallow//中括弧全体をint[]にキャストし, 更にvoidにキャストするとうまく行く
        {
            (std::cout << std::get<n>(out) << ", ", 0)...//outの要素をcoutで出力する     (文字を表示する処理(nという名のパック), 0)...

            //------^------
            //これら[1], [2], [3], [4]の組み合わせのようなもの
            //[1]・・・・・int a[] = {1, 2, 3, 5}; は 配列に数値を代入する処理
            //[2]・・・・・(処理や式(パックを含む))...は  (処理や式(パックを含む)), (処理や式(パックを含む)), (処理や式(パックを含む)), (処理や式(パックを含む))・・・・のようにカンマ区切りになっているものと同等に考えて良い
            //[3]・・・・・ int b = ( 処理や式, 数値 );はbに数値を代入するついでに処理を行う
            //[4]・・・・・(int)1;のように右辺値やリテラルのみでもかまわない
        };
        std::cout << "\b\b" << ' ' << std::endl;//最後の2文字", "を空白"  "に置き換え, 改行する
    }

    template <typename ...T> 
    void output(T ... _out)//任意の個数のパラメーターを受け取って出力する
    {
        output0(std::make_index_sequence<sizeof...(_out)>{}, _out...);//std::make_index_sequence<整数>はstd::index_sequence<0,1,2,3,...,整数-1>をコンパイルの時点で生成する
    }
/***************************************************************************************************************/


int kbhit(void)//何かキーが押されていれば1, 押されていなければ0を返す
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
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


#define digitnum_image 4
void Output_To_Ppm(std::string str, Vec3* data, int width, int height, const int &i) //ppm画像を1つ出力
{
    using namespace std::literals::string_literals;

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
                c[0] = (int)(data[width * j + i]).x;  c[1] = (int)(data[width * j + i]).y;  c[2] = (int)(data[width * j + i]).z;

                file.write(c, 3);  //色を書き込む
            }

            std::cout << (100 * (j+1) / height) << '%' << "\r" << std::flush; //進行状況を表示

        }
    /********************************************************************************************************/
    file.close(); //ファイルを閉じる

    std::cout << "▲書き込み終了▲" << std::endl; //表示
}
#undef digitnum_image


