ルートディレクトリ内で以下のコマンドを実行する。
mkdir ./build && cd ./build

それから以下のコマンドを実行する
cmake ../ && make -s -j 8 && ./src/a.out