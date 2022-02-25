#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

//ファイルパスの最大文字数
#define PATH 256 
//画像の最大幅
#define WIDTH 350
//画像の最大高さ
#define HEIGHT 350
//BMPのファイルヘッダーのサイズ
#define FileHeaderSize 14 
//BMPの情報ヘッダーのサイズ
#define InfoHeaderSize 40 
//256段階最小値
#define LOW 0 
//256段階最大値
#define HIGH 255 
//グローバル変数
//BMPのファイルヘッダーを保存する
unsigned char BitMapFileHeader[FileHeaderSize];
//BMPのサイズを保存する
unsigned int biSize;
// BMPの幅を保存する
int biWidth;
//BMPの高さを保存する
int biHeight;
//上記以外のBMPの情報ヘッダーを保存する
unsigned char BitMapInfoHeader[InfoHeaderSize];


//prewittフィルターの関数宣言
void prewitt(unsigned char img[WIDTH][HEIGHT], unsigned char img_prewitt[WIDTH][HEIGHT]);
//sobelフィルターの関数宣言
void sobel(unsigned char img[WIDTH][HEIGHT], unsigned char img_sobel[WIDTH][HEIGHT]);
//BMPを読み込む関数宣言
void read_img(const char path[PATH], unsigned char img[WIDTH][HEIGHT][3]);
//BMPを書き込む関数宣言
void write_img(const char path[PATH], unsigned char img[WIDTH][HEIGHT][3]);
//BMPをグレースケールに変換する関数宣言
void BMPtoGRAY(unsigned char img[WIDTH][HEIGHT][3], unsigned char img_gray[WIDTH][HEIGHT]);
//グレースケールを計算する関数宣言
int calcGRAY(unsigned char R, unsigned char G, unsigned char B);
//グレースケールをBMPに変換する関数宣言
void GRAYtoBMP(unsigned char img_gray[WIDTH][HEIGHT], unsigned char img[WIDTH][HEIGHT][3]);
//画像回転指せる関数宣言
void Roll(unsigned char img_roll[WIDTH][HEIGHT][3], unsigned char img[WIDTH][HEIGHT][3]);

int main(void) {

    //画像のパスを保存する
    char path[PATH], hozonn1[PATH], hozonn2[PATH], hozonn3[PATH];
    //BMP画像を保存する
    unsigned char img[WIDTH][HEIGHT][3];
    //グレースケールを保存する
    unsigned char img_gray[WIDTH][HEIGHT];
    //加工後の画像を保存する
    unsigned char img_processed[WIDTH][HEIGHT];
    unsigned char img_roll[WIDTH][HEIGHT][3];
    //選択を保存する
    int num = 0, num2 = 0, num3 = 0;

    //パスを入力
    printf("加工したい画像へのパスを入力してください\n");
    //入力された値をpathに保存
    scanf("%s", path);
    //パスの画像を読み込んでimgに保存
    read_img(path, img);
    //imgのグレースケールをimg_grayに保存
    BMPtoGRAY(img, img_gray);

    printf("画像を加工する方法を選んでください\n");
    printf("グレー加工->0\n");
    printf("回転加工->1\n");
    scanf("%d", &num3);
    if (num3 == 0)
    {
        printf("フィルターを選択してください\n");
        printf("0:prewittフィルター\n");
        printf("1:sobelフィルター\n");
        scanf("%d", &num);
        //選択分岐
        if (num == 0)
        {
            //img_grayにprewittフィルタを適用してimg_processedに保存
            prewitt(img_gray, img_processed);
            //img_processedをBMPに変換しimgに保存
            GRAYtoBMP(img_processed, img);
            //prewittフィルタ適用後の画像を保存
            printf("prewittフィルター適用後のパス指定を指定してください\n");
            scanf("%s", hozonn1);
            write_img(hozonn1, img);
        }
        else if (num == 1)
        {
            //img_grayにsobelフィルタを適用してimg_processedに保存
            sobel(img_gray, img_processed);
            //img_processedをBMPに変換しimgに保存
            GRAYtoBMP(img_processed, img);
            //sobelフィルタ適用後の画像を保存
            printf("sobelフィルター適用後のパスを指定してください\n");
            scanf("%s", hozonn2);
            write_img(hozonn2, img);
        }
    }
    //-------------------------------------------------------------------

    else if (num3 == 1)
    {
        printf("画像を回転させますか？\n");
        printf("Yes->1\n");
        printf("No->0\n");
        scanf("%d", &num2);
        if (num2 == 0)
        {
            printf("回転処理をしませんでした。\n");
        }
        else
        {
            Roll(img_roll, img);
            printf("回転処理を実行しました。\n");
            printf("回転加工適用後のパスを指定してください\n");
            scanf("%s", hozonn3);
            write_img(hozonn3, img);
        }
    }
    return 0;
}

//引数としてファイルのパスと画像を保存する配列を受け取る
void read_img(const char path[PATH], unsigned char img[WIDTH][HEIGHT][3])
{
    //ファイルポインタ
    FILE* fp;
    //for文用、幅、高さ、RGB
    int i, j, RGB;
    //パスのファイルを読み込む
    fp = fopen(path, "rb");

    //ファイルの読み込みに成功したら
    if (fp != NULL)
    {
        //ファイルヘッダーを読み込む
        fread(&BitMapFileHeader, sizeof(char), FileHeaderSize, fp);
        //情報ヘッダーにあるサイズを読み込む
        fread(&biSize, sizeof(int), 1, fp);
        //情報ヘッダーにある幅を保存
        fread(&biWidth, sizeof(int), 1, fp);
        //情報ヘッダーにある高さを保存
        fread(&biHeight, sizeof(int), 1, fp);
        //残りの情報ヘッダーを保存 12 = int * 3
        fread(&BitMapInfoHeader, sizeof(char), InfoHeaderSize - 12, fp);

        //0から幅まで
        for (i = 0; i < biWidth; i++)
        {

            //0から高さまで
            for (j = 0; j < biHeight; j++)
            {

                //RGBのそれぞれ
                for (RGB = 0; RGB < 3; RGB++)
                {

                    //画素の情報を読み込んで保存する
                    fread(&img[i][j][RGB], sizeof(char), 1, fp);
                }
            }
        }
        //ファイルを閉じる
        fclose(fp);
    }
    //ファイルの読み込みに失敗したら
    else
    {
        //メッセージを表示
        printf("File Not Found.\n");
        exit(0);
    }
}

//引数としてBMPを書き込む先のパスと、書き込むBMPの画素情報が入った配列を受け取る
void write_img(const char path[PATH], unsigned char img[WIDTH][HEIGHT][3]) {
    //ファイルポインタ
    FILE* fp;
    //for文用、幅、高さ、RGB
    int i, j, RGB;
    //パスのファイルへ書き込み
    fp = fopen(path, "wb");
    //ファイルヘッダーを書き込む
    fwrite(&BitMapFileHeader, sizeof(char), FileHeaderSize, fp);
    //情報ヘッダーへファイルサイズを書き込む
    fwrite(&biSize, sizeof(int), 1, fp);
    //情報ヘッダーへ幅を書き込む
    fwrite(&biWidth, sizeof(int), 1, fp);
    //情報ヘッダーへ高さを書き込む
    fwrite(&biHeight, sizeof(int), 1, fp);
    //残りの情報ヘッダーを書き込む
    fwrite(&BitMapInfoHeader, sizeof(char), InfoHeaderSize - 12, fp);

    //0から幅まで
    for (i = 0; i < biWidth; i++)
    {
        //0から高さまで
        for (j = 0; j < biHeight; j++)
        {
            //RGBのそれぞれ
            for (RGB = 0; RGB < 3; RGB++)
            {
                //画素の情報を保存する
                fwrite(&img[i][j][RGB], sizeof(char), 1, fp);
            }
        }
    }
    //ファイルを閉じる
    fclose(fp);
}

//引数としてグレースケール画像と加工後の画像の保存先を受け取る
void prewitt(unsigned char img[WIDTH][HEIGHT], unsigned char img_prewitt[WIDTH][HEIGHT])
{
    //水平方向
    char H[9] = { -1, 0, 1,
                 -1, 0, 1,
                 -1, 0, 1 };
    //垂直方向
    char V[9] = { -1,-1,-1,
                  0, 0, 0,
                  1, 1, 1 };
    //for文用
    int i, j, k, l;

    //計算後の値、水平方向、垂直方向を一時的に保存する
    double sum, sumH, sumV;

    //左右の一番端以外の画素すべて
    for (k = 1; k < biWidth - 1; k++)
    {
        //上下の一番端の画素以外すべて
        for (l = 1; l < biHeight - 1; l++)
        {
            //値を初期化
            sumH = 0;

            //値を初期化
            sumV = 0;
            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 3; j++)
                {
                    //水平方向
                    sumH += H[i * 3 + j] * img[k - 1 + i][l - 1 + j];
                    //垂直方向
                    sumV += V[i * 3 + j] * img[k - 1 + i][l - 1 + j];
                }
            }
            sum = sqrt(pow(sumH, 2) + pow(sumV, 2));
            //sumが最低値より小さい時
            if (sum < LOW)
            {
                //sumの絶対値をとる
                sum = abs((int)sum);
            }
            //sumが最高値より大きい時
            else if (sum > HIGH)
            {
                //sumは最高値とする
                sum = HIGH;
            }
            //加工後の画素を保存
            img_prewitt[k][l] = sum;
        }
    }
}

//引数としてグレースケール画像と加工後の画像の保存先を受け取る
void sobel(unsigned char img[WIDTH][HEIGHT], unsigned char img_sobel[WIDTH][HEIGHT]) {
    //水平方向
    char H[9] = { -1, 0, 1,
                 -2, 0, 2,
                 -1, 0, 1 };
    //垂直方向
    char V[9] = { -1,-2,-1,
                  0, 0, 0,
                  1, 2, 1 };
    //for文用
    int i, j, k, l;

    //計算後の値、水平方向、垂直方向を一時的に保存する
    double sum, sumH, sumV;

    //左右の一番端以外の画素すべて
    for (k = 1; k < biWidth - 1; k++)
    {
        //上下の一番端の画素以外すべて
        for (l = 1; l < biHeight - 1; l++)
        {
            //値を初期化
            sumH = 0;

            //値を初期化
            sumV = 0;
            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 3; j++)
                {
                    //水平方向
                    sumH += H[i * 3 + j] * img[k - 1 + i][l - 1 + j];

                    //垂直方向
                    sumV += V[i * 3 + j] * img[k - 1 + i][l - 1 + j];
                }
            }
            sum = sqrt(pow(sumH, 2) + pow(sumV, 2));
            //sumが最低値より小さい時
            if (sum < LOW)
            {
                //sumの絶対値をとる
                sum = abs((int)sum);
            }
            //sumが最高値より大きい時
            else if (sum > HIGH)
            {
                //sumは最高値とする
                sum = HIGH;
            }
            //加工後の画素を保存
            img_sobel[k][l] = sum;
        }
    }
}

//引数としてBMPと加工後の画像の保存先を受け取る
void BMPtoGRAY(unsigned char img[WIDTH][HEIGHT][3], unsigned char img_gray[WIDTH][HEIGHT])
{
    //for文用
    int i, j;
    //計算後のグレースケール値を保存する
    int GRAY;
    //0から幅まで
    for (i = 0; i < biWidth; i++)
    {
        //0から高さまで
        for (j = 0; j < biHeight; j++)
        {
            //グレースケール値を計算
            GRAY = calcGRAY(img[i][j][0], img[i][j][1], img[i][j][0]);
            //計算したグレースケール値を保存
            img_gray[i][j] = GRAY;
        }
    }
}

//引数としてRGBを受け取る
int calcGRAY(unsigned char R, unsigned char G, unsigned char B)
{
    //グレースケール値を保存
    int GRAY;
    //グレースケール補正
    GRAY = (int)(0.299 * R + 0.587 * G + 0.114 * B);
    //グレースケール値が最低値より小さい時
    if (GRAY < LOW)
    {
        //グレースケール値は最低値とする
        GRAY = LOW;
    }
    //グレースケール値が最高値より大きい時
    else if (GRAY > HIGH)
    {
        //グレースケール値は最高値とする
        GRAY = HIGH;
    }
    //グレースケール値を返す
    return GRAY;
}

//引数としてグレースケールと加工後の画像の保存先を受け取る
void GRAYtoBMP(unsigned char img_gray[WIDTH][HEIGHT], unsigned char img[WIDTH][HEIGHT][3])
{
    //for文用
    int i, j, RGB;
    //0から幅まで
    for (i = 0; i < biWidth; i++)
    {
        //0から高さまで
        for (j = 0; j < biHeight; j++)
        {
            //RGBすべて
            for (RGB = 0; RGB < 3; RGB++)
            {
                //グレースケール値をRGBに入れる
                img[i][j][RGB] = img_gray[i][j];
            }
        }
    }
}

//画像回転処理
void Roll(unsigned char img_roll[WIDTH][HEIGHT][3], unsigned char img[WIDTH][HEIGHT][3])
{
    //時計回りの回転角度を保存
    int deg;
    //反時計回りの回転角度保存
    double rad;
    // cos(rad)
    double csr;
    // sin(rad)
    double snr = 0;
    // 横中心座標
    double xc;
    // 縦中心座標
    double yc;
    // 回転前の画素座標
    int x, y;
    // 回転後の画素座標
    int u, v;
    // カラー
    int k;

    // 回転角度入力，回転ベクトル計算
    printf("何度回転させますか？\n");
    scanf("%d", &deg);
    rad = -deg * acos(-1.0) / 180;
    csr = cos(rad);

    // 中心回り回転後画素座標計算
    xc = biWidth / 2;
    yc = biHeight / 2;
    for (y = 0; y < biHeight; y++)
    {
        for (x = 0; x < biWidth; x++)
        {
            u = (int)(((x - xc) * csr - (y - yc) * snr) + xc);
            v = (int)(((x - xc) * snr + (y - yc) * csr) + yc);
        }
    }

    // 回転後画素設定
    if (0 <= u && u < biWidth && 0 <= v && v < biHeight)
    {
        for (k = 0; k < 3; k++)
        {
            img_roll[k][v][u] = img[k][y][x];
        }

    }
}