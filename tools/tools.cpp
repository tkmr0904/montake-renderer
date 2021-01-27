#include <string>
#include <fcntl.h>
#include <termios.h>
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
