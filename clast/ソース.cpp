#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

//�t�@�C���p�X�̍ő啶����
#define PATH 256 
//�摜�̍ő啝
#define WIDTH 350
//�摜�̍ő卂��
#define HEIGHT 350
//BMP�̃t�@�C���w�b�_�[�̃T�C�Y
#define FileHeaderSize 14 
//BMP�̏��w�b�_�[�̃T�C�Y
#define InfoHeaderSize 40 
//256�i�K�ŏ��l
#define LOW 0 
//256�i�K�ő�l
#define HIGH 255 
//�O���[�o���ϐ�
//BMP�̃t�@�C���w�b�_�[��ۑ�����
unsigned char BitMapFileHeader[FileHeaderSize];
//BMP�̃T�C�Y��ۑ�����
unsigned int biSize;
// BMP�̕���ۑ�����
int biWidth;
//BMP�̍�����ۑ�����
int biHeight;
//��L�ȊO��BMP�̏��w�b�_�[��ۑ�����
unsigned char BitMapInfoHeader[InfoHeaderSize];


//prewitt�t�B���^�[�̊֐��錾
void prewitt(unsigned char img[WIDTH][HEIGHT], unsigned char img_prewitt[WIDTH][HEIGHT]);
//sobel�t�B���^�[�̊֐��錾
void sobel(unsigned char img[WIDTH][HEIGHT], unsigned char img_sobel[WIDTH][HEIGHT]);
//BMP��ǂݍ��ފ֐��錾
void read_img(const char path[PATH], unsigned char img[WIDTH][HEIGHT][3]);
//BMP���������ފ֐��錾
void write_img(const char path[PATH], unsigned char img[WIDTH][HEIGHT][3]);
//BMP���O���[�X�P�[���ɕϊ�����֐��錾
void BMPtoGRAY(unsigned char img[WIDTH][HEIGHT][3], unsigned char img_gray[WIDTH][HEIGHT]);
//�O���[�X�P�[�����v�Z����֐��錾
int calcGRAY(unsigned char R, unsigned char G, unsigned char B);
//�O���[�X�P�[����BMP�ɕϊ�����֐��錾
void GRAYtoBMP(unsigned char img_gray[WIDTH][HEIGHT], unsigned char img[WIDTH][HEIGHT][3]);
//�摜��]�w����֐��錾
void Roll(unsigned char img_roll[WIDTH][HEIGHT][3], unsigned char img[WIDTH][HEIGHT][3]);

int main(void) {

    //�摜�̃p�X��ۑ�����
    char path[PATH], hozonn1[PATH], hozonn2[PATH], hozonn3[PATH];
    //BMP�摜��ۑ�����
    unsigned char img[WIDTH][HEIGHT][3];
    //�O���[�X�P�[����ۑ�����
    unsigned char img_gray[WIDTH][HEIGHT];
    //���H��̉摜��ۑ�����
    unsigned char img_processed[WIDTH][HEIGHT];
    unsigned char img_roll[WIDTH][HEIGHT][3];
    //�I����ۑ�����
    int num = 0, num2 = 0, num3 = 0;

    //�p�X�����
    printf("���H�������摜�ւ̃p�X����͂��Ă�������\n");
    //���͂��ꂽ�l��path�ɕۑ�
    scanf("%s", path);
    //�p�X�̉摜��ǂݍ����img�ɕۑ�
    read_img(path, img);
    //img�̃O���[�X�P�[����img_gray�ɕۑ�
    BMPtoGRAY(img, img_gray);

    printf("�摜�����H������@��I��ł�������\n");
    printf("�O���[���H->0\n");
    printf("��]���H->1\n");
    scanf("%d", &num3);
    if (num3 == 0)
    {
        printf("�t�B���^�[��I�����Ă�������\n");
        printf("0:prewitt�t�B���^�[\n");
        printf("1:sobel�t�B���^�[\n");
        scanf("%d", &num);
        //�I�𕪊�
        if (num == 0)
        {
            //img_gray��prewitt�t�B���^��K�p����img_processed�ɕۑ�
            prewitt(img_gray, img_processed);
            //img_processed��BMP�ɕϊ���img�ɕۑ�
            GRAYtoBMP(img_processed, img);
            //prewitt�t�B���^�K�p��̉摜��ۑ�
            printf("prewitt�t�B���^�[�K�p��̃p�X�w����w�肵�Ă�������\n");
            scanf("%s", hozonn1);
            write_img(hozonn1, img);
        }
        else if (num == 1)
        {
            //img_gray��sobel�t�B���^��K�p����img_processed�ɕۑ�
            sobel(img_gray, img_processed);
            //img_processed��BMP�ɕϊ���img�ɕۑ�
            GRAYtoBMP(img_processed, img);
            //sobel�t�B���^�K�p��̉摜��ۑ�
            printf("sobel�t�B���^�[�K�p��̃p�X���w�肵�Ă�������\n");
            scanf("%s", hozonn2);
            write_img(hozonn2, img);
        }
    }
    //-------------------------------------------------------------------

    else if (num3 == 1)
    {
        printf("�摜����]�����܂����H\n");
        printf("Yes->1\n");
        printf("No->0\n");
        scanf("%d", &num2);
        if (num2 == 0)
        {
            printf("��]���������܂���ł����B\n");
        }
        else
        {
            Roll(img_roll, img);
            printf("��]���������s���܂����B\n");
            printf("��]���H�K�p��̃p�X���w�肵�Ă�������\n");
            scanf("%s", hozonn3);
            write_img(hozonn3, img);
        }
    }
    return 0;
}

//�����Ƃ��ăt�@�C���̃p�X�Ɖ摜��ۑ�����z����󂯎��
void read_img(const char path[PATH], unsigned char img[WIDTH][HEIGHT][3])
{
    //�t�@�C���|�C���^
    FILE* fp;
    //for���p�A���A�����ARGB
    int i, j, RGB;
    //�p�X�̃t�@�C����ǂݍ���
    fp = fopen(path, "rb");

    //�t�@�C���̓ǂݍ��݂ɐ���������
    if (fp != NULL)
    {
        //�t�@�C���w�b�_�[��ǂݍ���
        fread(&BitMapFileHeader, sizeof(char), FileHeaderSize, fp);
        //���w�b�_�[�ɂ���T�C�Y��ǂݍ���
        fread(&biSize, sizeof(int), 1, fp);
        //���w�b�_�[�ɂ��镝��ۑ�
        fread(&biWidth, sizeof(int), 1, fp);
        //���w�b�_�[�ɂ��鍂����ۑ�
        fread(&biHeight, sizeof(int), 1, fp);
        //�c��̏��w�b�_�[��ۑ� 12 = int * 3
        fread(&BitMapInfoHeader, sizeof(char), InfoHeaderSize - 12, fp);

        //0���畝�܂�
        for (i = 0; i < biWidth; i++)
        {

            //0���獂���܂�
            for (j = 0; j < biHeight; j++)
            {

                //RGB�̂��ꂼ��
                for (RGB = 0; RGB < 3; RGB++)
                {

                    //��f�̏���ǂݍ���ŕۑ�����
                    fread(&img[i][j][RGB], sizeof(char), 1, fp);
                }
            }
        }
        //�t�@�C�������
        fclose(fp);
    }
    //�t�@�C���̓ǂݍ��݂Ɏ��s������
    else
    {
        //���b�Z�[�W��\��
        printf("File Not Found.\n");
        exit(0);
    }
}

//�����Ƃ���BMP���������ސ�̃p�X�ƁA��������BMP�̉�f��񂪓������z����󂯎��
void write_img(const char path[PATH], unsigned char img[WIDTH][HEIGHT][3]) {
    //�t�@�C���|�C���^
    FILE* fp;
    //for���p�A���A�����ARGB
    int i, j, RGB;
    //�p�X�̃t�@�C���֏�������
    fp = fopen(path, "wb");
    //�t�@�C���w�b�_�[����������
    fwrite(&BitMapFileHeader, sizeof(char), FileHeaderSize, fp);
    //���w�b�_�[�փt�@�C���T�C�Y����������
    fwrite(&biSize, sizeof(int), 1, fp);
    //���w�b�_�[�֕�����������
    fwrite(&biWidth, sizeof(int), 1, fp);
    //���w�b�_�[�֍�������������
    fwrite(&biHeight, sizeof(int), 1, fp);
    //�c��̏��w�b�_�[����������
    fwrite(&BitMapInfoHeader, sizeof(char), InfoHeaderSize - 12, fp);

    //0���畝�܂�
    for (i = 0; i < biWidth; i++)
    {
        //0���獂���܂�
        for (j = 0; j < biHeight; j++)
        {
            //RGB�̂��ꂼ��
            for (RGB = 0; RGB < 3; RGB++)
            {
                //��f�̏���ۑ�����
                fwrite(&img[i][j][RGB], sizeof(char), 1, fp);
            }
        }
    }
    //�t�@�C�������
    fclose(fp);
}

//�����Ƃ��ăO���[�X�P�[���摜�Ɖ��H��̉摜�̕ۑ�����󂯎��
void prewitt(unsigned char img[WIDTH][HEIGHT], unsigned char img_prewitt[WIDTH][HEIGHT])
{
    //��������
    char H[9] = { -1, 0, 1,
                 -1, 0, 1,
                 -1, 0, 1 };
    //��������
    char V[9] = { -1,-1,-1,
                  0, 0, 0,
                  1, 1, 1 };
    //for���p
    int i, j, k, l;

    //�v�Z��̒l�A���������A�����������ꎞ�I�ɕۑ�����
    double sum, sumH, sumV;

    //���E�̈�Ԓ[�ȊO�̉�f���ׂ�
    for (k = 1; k < biWidth - 1; k++)
    {
        //�㉺�̈�Ԓ[�̉�f�ȊO���ׂ�
        for (l = 1; l < biHeight - 1; l++)
        {
            //�l��������
            sumH = 0;

            //�l��������
            sumV = 0;
            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 3; j++)
                {
                    //��������
                    sumH += H[i * 3 + j] * img[k - 1 + i][l - 1 + j];
                    //��������
                    sumV += V[i * 3 + j] * img[k - 1 + i][l - 1 + j];
                }
            }
            sum = sqrt(pow(sumH, 2) + pow(sumV, 2));
            //sum���Œ�l��菬������
            if (sum < LOW)
            {
                //sum�̐�Βl���Ƃ�
                sum = abs((int)sum);
            }
            //sum���ō��l���傫����
            else if (sum > HIGH)
            {
                //sum�͍ō��l�Ƃ���
                sum = HIGH;
            }
            //���H��̉�f��ۑ�
            img_prewitt[k][l] = sum;
        }
    }
}

//�����Ƃ��ăO���[�X�P�[���摜�Ɖ��H��̉摜�̕ۑ�����󂯎��
void sobel(unsigned char img[WIDTH][HEIGHT], unsigned char img_sobel[WIDTH][HEIGHT]) {
    //��������
    char H[9] = { -1, 0, 1,
                 -2, 0, 2,
                 -1, 0, 1 };
    //��������
    char V[9] = { -1,-2,-1,
                  0, 0, 0,
                  1, 2, 1 };
    //for���p
    int i, j, k, l;

    //�v�Z��̒l�A���������A�����������ꎞ�I�ɕۑ�����
    double sum, sumH, sumV;

    //���E�̈�Ԓ[�ȊO�̉�f���ׂ�
    for (k = 1; k < biWidth - 1; k++)
    {
        //�㉺�̈�Ԓ[�̉�f�ȊO���ׂ�
        for (l = 1; l < biHeight - 1; l++)
        {
            //�l��������
            sumH = 0;

            //�l��������
            sumV = 0;
            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 3; j++)
                {
                    //��������
                    sumH += H[i * 3 + j] * img[k - 1 + i][l - 1 + j];

                    //��������
                    sumV += V[i * 3 + j] * img[k - 1 + i][l - 1 + j];
                }
            }
            sum = sqrt(pow(sumH, 2) + pow(sumV, 2));
            //sum���Œ�l��菬������
            if (sum < LOW)
            {
                //sum�̐�Βl���Ƃ�
                sum = abs((int)sum);
            }
            //sum���ō��l���傫����
            else if (sum > HIGH)
            {
                //sum�͍ō��l�Ƃ���
                sum = HIGH;
            }
            //���H��̉�f��ۑ�
            img_sobel[k][l] = sum;
        }
    }
}

//�����Ƃ���BMP�Ɖ��H��̉摜�̕ۑ�����󂯎��
void BMPtoGRAY(unsigned char img[WIDTH][HEIGHT][3], unsigned char img_gray[WIDTH][HEIGHT])
{
    //for���p
    int i, j;
    //�v�Z��̃O���[�X�P�[���l��ۑ�����
    int GRAY;
    //0���畝�܂�
    for (i = 0; i < biWidth; i++)
    {
        //0���獂���܂�
        for (j = 0; j < biHeight; j++)
        {
            //�O���[�X�P�[���l���v�Z
            GRAY = calcGRAY(img[i][j][0], img[i][j][1], img[i][j][0]);
            //�v�Z�����O���[�X�P�[���l��ۑ�
            img_gray[i][j] = GRAY;
        }
    }
}

//�����Ƃ���RGB���󂯎��
int calcGRAY(unsigned char R, unsigned char G, unsigned char B)
{
    //�O���[�X�P�[���l��ۑ�
    int GRAY;
    //�O���[�X�P�[���␳
    GRAY = (int)(0.299 * R + 0.587 * G + 0.114 * B);
    //�O���[�X�P�[���l���Œ�l��菬������
    if (GRAY < LOW)
    {
        //�O���[�X�P�[���l�͍Œ�l�Ƃ���
        GRAY = LOW;
    }
    //�O���[�X�P�[���l���ō��l���傫����
    else if (GRAY > HIGH)
    {
        //�O���[�X�P�[���l�͍ō��l�Ƃ���
        GRAY = HIGH;
    }
    //�O���[�X�P�[���l��Ԃ�
    return GRAY;
}

//�����Ƃ��ăO���[�X�P�[���Ɖ��H��̉摜�̕ۑ�����󂯎��
void GRAYtoBMP(unsigned char img_gray[WIDTH][HEIGHT], unsigned char img[WIDTH][HEIGHT][3])
{
    //for���p
    int i, j, RGB;
    //0���畝�܂�
    for (i = 0; i < biWidth; i++)
    {
        //0���獂���܂�
        for (j = 0; j < biHeight; j++)
        {
            //RGB���ׂ�
            for (RGB = 0; RGB < 3; RGB++)
            {
                //�O���[�X�P�[���l��RGB�ɓ����
                img[i][j][RGB] = img_gray[i][j];
            }
        }
    }
}

//�摜��]����
void Roll(unsigned char img_roll[WIDTH][HEIGHT][3], unsigned char img[WIDTH][HEIGHT][3])
{
    //���v���̉�]�p�x��ۑ�
    int deg;
    //�����v���̉�]�p�x�ۑ�
    double rad;
    // cos(rad)
    double csr;
    // sin(rad)
    double snr = 0;
    // �����S���W
    double xc;
    // �c���S���W
    double yc;
    // ��]�O�̉�f���W
    int x, y;
    // ��]��̉�f���W
    int u, v;
    // �J���[
    int k;

    // ��]�p�x���́C��]�x�N�g���v�Z
    printf("���x��]�����܂����H\n");
    scanf("%d", &deg);
    rad = -deg * acos(-1.0) / 180;
    csr = cos(rad);

    // ���S����]���f���W�v�Z
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

    // ��]���f�ݒ�
    if (0 <= u && u < biWidth && 0 <= v && v < biHeight)
    {
        for (k = 0; k < 3; k++)
        {
            img_roll[k][v][u] = img[k][y][x];
        }

    }
}