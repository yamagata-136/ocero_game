/***************************************
オセロをつくる
****************************************/
#define _CRT_SECURE_NO_WARNINGS //scanfを使用するため
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*プロトタイプ宣言*/
void Init_Field();//フィールド初期化
void Print_Field();//フィールド出力
int* Select_Grid();//マス選択
int Check_Grid(const int *);//選択したマスに石を置けるかチェック
int Vertical_Check();//縦方向の石を変えられるかチェック
int Side_check();//横方向の石を変えられるかチェック
int Diagonal_Check();//斜め方向の石を変えられるかチェック
void Judge_Winner();//勝者の判定
void Change_Stone(int ,int *);//石を変える
int Break_Judge(int ,int *);//breakするかの判定
/*定数*/
#define WIDTH 8
#define HEIGHT 8
#define NONE 0
#define WHITE 1
#define BLACK 2
#define TRUE 1
#define FALSE 0
/*マクロ*/
#define CHG_TURN_FLG(Turn_Flg) (Turn_Flg == BLACK ? WHITE : BLACK)
/*グローバル変数宣言*/
int Field[HEIGHT][WIDTH];
int Gyo,Retu;
int Turn_Flg = BLACK;//先行は黒
int Stone_Cnt = 4;//初期値は4,MAX64
int main(){
    int *Fp;
    int result;

    Init_Field();
    while(Stone_Cnt<64)
    {
        Print_Field();
        if(Turn_Flg == BLACK)
        {
            puts("黒のターンです");
        }
        else
        {
            puts("白のターンです");
        }
        
        Fp = Select_Grid();
        result = Check_Grid(Fp);
        if(FALSE == result)
        {
            puts("石を置けません！もう一度入力してください");
            puts("ENTERを押してください");
            while(getchar() != '\n');//bufferクリア
            system("cls");
            continue;
        }
        else
        {
            Stone_Cnt++;//石の数を加算
            Turn_Flg = CHG_TURN_FLG(Turn_Flg);
        }
        system("cls");
    }
    return 0;
}

/******************************************
フィールド初期化
*******************************************/
void Init_Field(){
    int i,j;
    for(i=0;i<HEIGHT;i++)
    {
        for(j=0;j<WIDTH;j++)
        {
            Field[i][j] = NONE;
        }
    }
    Field[3][3] = BLACK;
    Field[4][4] = BLACK;
    Field[3][4] = WHITE;
    Field[4][3] = WHITE;
}
/*****************************************
フィールドを出力する
******************************************/
void Print_Field(){
    int i,j;
    printf("   0  1  2  3  4  5  6  7 \n");//座標番号
    for(i=0;i<HEIGHT;i++)
    {   
        printf("--------------------------\n");
        printf("%d|",i);//座標番号
        for(j=0;j<WIDTH;j++)
        {
            if(Field[i][j] == NONE)
            {
                printf("  |");
            }
            else if(Field[i][j] == WHITE)
            {
                printf("●|");
            }
            else
            {
                printf("○|");
            }
        }
        printf("\n");
    }
    printf("--------------------------\n");
}
/************************************************
マスを選択する
*************************************************/
int* Select_Grid(){
    int *Fp;
    char x,y;
    int loop_flg = TRUE;
    /*有効な入力があるまでループ*/
    while(loop_flg)//無限ループ
    {
        puts("X座標を入力してください:0～7");
        scanf("%c",&x);
        while(getchar() != '\n');//bufferクリア

        puts("Y座標を入力してください:0～7");
        scanf("%c",&y);
        while(getchar() != '\n');//bufferクリア
        
        if(('0'<=x && x<='7')&&('0'<=y && y<='7'))
        {
            loop_flg = FALSE;//有効な入力
        }
        else
        {
            puts("無効な入力です。もう一度入力してください");
        }
        
    }
    Gyo = x - '0';//X座標の保存
    Retu = y - '0';//Ｙ座標の保存
    Fp = &(Field[(x - '0')][(y - '0')]);//選択したマスのポインタ
    return Fp;
}
/**************************************************
選択したマスに石が置けるか確認
**************************************************/
int Check_Grid(const int *Fp){
    int vertical_result = FALSE;
    int side_result     = FALSE;
    int diagonal_result = FALSE;
    
    /*すでに石が置いてある場合*/
    if(*Fp != NONE)
    {
        puts("すでに石が置かれています");
        return FALSE;
    }
    else//NONEの場合
    {
        
        vertical_result = Vertical_Check();//縦の確認
        side_result     = Side_check();//横の確認
        diagonal_result = Diagonal_Check();//斜めの確認
    }
    if((vertical_result==TRUE)||(side_result==TRUE)||(diagonal_result==TRUE))
    {
        return TRUE;//石が置ける
    }
    else
    {
        return FALSE;//石が置けない
    }   
}
/*******************************************************
縦方向の石が変えられるかチェックし、変えられる場合は石を変える
*******************************************************/
int Vertical_Check(){
    int chg_OKflg=FALSE,i,cnt=0;
    int *sand_fp;
    int sand_OKflg = FALSE;
    int b_result;
        
    //上方向の確認
    sand_fp = &(Field[Gyo][Retu]);//選択したマスのポインタ
    for(i=Gyo-1;i>=0;i--)
    {
        sand_OKflg  = FALSE;
        sand_fp -= 8;
        cnt++;
        if(*sand_fp==NONE)//石がなかったら終了
        {
            break;
        }

        b_result = Break_Judge(Turn_Flg,sand_fp);
        if(b_result==TRUE)
        {
            sand_OKflg = TRUE;
            break;
        }
    }
    /*上方向の石の色を変える*/
    if((cnt == 1)||(sand_OKflg == FALSE))//cntが1の場合,sand_OKflgがTRUEの場合はなにもしない
    {
        //chg_OKflgはFALSEのままのためなにもしない
    }
    else
    {
        sand_fp = &(Field[Gyo][Retu]);
        for(i=1;i<cnt;i++)
        {
            sand_fp -= 8;
            Change_Stone(Turn_Flg,  sand_fp);
        }
        chg_OKflg = TRUE;//石を置くことが可能
    }
    /**********************************************************/
    //下方向の確認
    sand_fp = &(Field[Gyo][Retu]);//選択したマスのポインタ
    cnt = 0;//cnt初期化
    for(i=Gyo+1;i<8;i++)
    {
        sand_OKflg  = FALSE;
        sand_fp += 8;
        cnt++;
        if(*sand_fp==NONE)//石がなかったら終了
        {
            break;
        }

        b_result = Break_Judge(Turn_Flg,sand_fp);
        if(b_result==TRUE)
        {
            sand_OKflg = TRUE;
            break;
        }
    }
    /*下方向の石の色を変える*/
    if((cnt == 1)||(sand_OKflg == FALSE))//cntが1の場合,sand_OKflgがTRUEの場合はなにもしない
    {
        //chg_OKflgはFALSEのままのためなにもしない
    }
    else
    {
        sand_fp = &(Field[Gyo][Retu]);
        
        for(i=1;i<cnt;i++)
        {
            sand_fp += 8;
            Change_Stone(Turn_Flg,  sand_fp);
        }
        chg_OKflg = TRUE;//石を置くことが可能
    }
    return chg_OKflg;
}
/********************************************************
横方向の石が変えられるかチェックし、変えられる場合は石を変える
********************************************************/
int Side_check(){
    int chg_OKflg=FALSE,i,cnt=0;
    int *sand_fp;
    int sand_OKflg = FALSE;
    int b_result;
    //左方向の確認
    sand_fp = &(Field[Gyo][Retu]);//選択したマスのポインタ
    for(i=Retu-1;i>=0;i--)
    {
        sand_OKflg  = FALSE;
        sand_fp -= 1;
        cnt++;
        if(*sand_fp==NONE)//石がなかったら終了
        {
            break;
        }

        b_result = Break_Judge(Turn_Flg,sand_fp);
        if(b_result==TRUE)
        {
            sand_OKflg = TRUE;
            break;
        }
    }
    /*左方向の石の色を変える*/
    if((cnt == 1)||(sand_OKflg == FALSE))//cntが1の場合,sand_OKflgがTRUEの場合はなにもしない
    {
        //chg_OKflgはFALSEのままのためなにもしない
    }
    else
    {
        sand_fp = &(Field[Gyo][Retu]);
        for(i=1;i<cnt;i++)
        {
            sand_fp -= 1;
            Change_Stone(Turn_Flg,  sand_fp);
        }
        chg_OKflg = TRUE;//石を置くことが可能
    }

    /**********************************************************/
    //右方向の確認
    sand_fp = &(Field[Gyo][Retu]);//選択したマスのポインタ
    cnt = 0;//cnt初期化
    for(i=Retu+1;i<8;i++)
    {
        sand_OKflg  = FALSE;
        sand_fp += 1;
        cnt++;
        if(*sand_fp==NONE)//石がなかったら終了
        {
            break;
        }

        b_result = Break_Judge(Turn_Flg,sand_fp);
        if(b_result==TRUE)
        {
            sand_OKflg = TRUE;
            break;
        }
    }
    /*右方向の石の色を変える*/
    if((cnt == 1)||(sand_OKflg == FALSE))//cntが1の場合,sand_OKflgがTRUEの場合はなにもしない
    {
        //chg_OKflgはFALSEのままのためなにもしない
    }
    else
    {
        sand_fp = &(Field[Gyo][Retu]);
        
        for(i=1;i<cnt;i++)
        {
            sand_fp += 1;
            Change_Stone(Turn_Flg,  sand_fp);
        }
        chg_OKflg = TRUE;//石を置くことが可能
    }
    return chg_OKflg;
}
/***************************************************
斜めのチェック
****************************************************/
int Diagonal_Check(){
    int chg_OKflg=FALSE,i,cnt=0;
    int *sand_fp;
    int sand_OKflg = FALSE;
    int b_result;
    /**右斜め上のチェック**/
    sand_fp = &(Field[Gyo][Retu]);//選択したマスのポインタ
    for(i=Gyo-1;i>=0;i--)
    {
        sand_OKflg  = FALSE;
        sand_fp -= 7;
        cnt++;
        if(*sand_fp==NONE)//石がなかったら終了
        {
            break;
        }

        b_result = Break_Judge(Turn_Flg,sand_fp);
        if(b_result==TRUE)
        {
            sand_OKflg = TRUE;
            break;
        }
    }
    
    /*右斜め上方向の石の色を変える*/
    if((cnt == 1)||(sand_OKflg == FALSE))//cntが1の場合,sand_OKflgがTRUEの場合はなにもしない
    {
        //chg_OKflgはFALSEのままのためなにもしない
    }
    else
    {
        sand_fp = &(Field[Gyo][Retu]);
        for(i=1;i<cnt;i++)
        {
            sand_fp -= 7;
            Change_Stone(Turn_Flg,  sand_fp);
        }
        chg_OKflg = TRUE;//石を置くことが可能
    }

    /**左斜め上のチェック*******************************************/
    sand_fp = &(Field[Gyo][Retu]);//選択したマスのポインタ
    cnt = 0;
    for(i=Gyo-1;i>=0;i--)
    {
        sand_OKflg  = FALSE;
        sand_fp -= 9;
        cnt++;
        if(*sand_fp==NONE)//石がなかったら終了
        {
            break;
        }

        b_result = Break_Judge(Turn_Flg,sand_fp);
        if(b_result==TRUE)
        {
            sand_OKflg = TRUE;
            break;
        }
    }
    /*左斜め上方向の石の色を変える*/
    if((cnt == 1)||(sand_OKflg == FALSE))//cntが1の場合,sand_OKflgがTRUEの場合はなにもしない
    {
        //chg_OKflgはFALSEのままのためなにもしない
    }
    else
    {
        sand_fp = &(Field[Gyo][Retu]);
        for(i=1;i<cnt;i++)
        {
            sand_fp -= 9;
            Change_Stone(Turn_Flg,  sand_fp);
        }
        chg_OKflg = TRUE;//石を置くことが可能
    }

    /**右斜め下のチェック*******************************************/
    sand_fp = &(Field[Gyo][Retu]);//選択したマスのポインタ
    cnt = 0;
    for(i=Gyo+1;i<8;i++)
    {
        sand_OKflg  = FALSE;
        sand_fp += 9;
        cnt++;
        if(*sand_fp==NONE)//石がなかったら終了
        {
            break;
        }

        b_result = Break_Judge(Turn_Flg,sand_fp);
        if(b_result==TRUE)
        {
            sand_OKflg = TRUE;
            break;
        }
    }
    /*右斜め下方向の石の色を変える*/
    if((cnt == 1)||(sand_OKflg == FALSE))//cntが1の場合,sand_OKflgがTRUEの場合はなにもしない
    {
        //chg_OKflgはFALSEのままのためなにもしない
    }
    else
    {
        sand_fp = &(Field[Gyo][Retu]);
        for(i=1;i<cnt;i++)
        {
            sand_fp += 9;
            Change_Stone(Turn_Flg,  sand_fp);
        }
        chg_OKflg = TRUE;//石を置くことが可能
    }

    /**左斜め下のチェック*******************************************/
    sand_fp = &(Field[Gyo][Retu]);//選択したマスのポインタ
    cnt = 0;
    for(i=Gyo+1;i<8;i++)
    {
        sand_OKflg  = FALSE;
        sand_fp += 7;
        cnt++;
        if(*sand_fp==NONE)//石がなかったら終了
        {
            break;
        }

        b_result = Break_Judge(Turn_Flg,sand_fp);
        if(b_result==TRUE)
        {
            sand_OKflg = TRUE;
            break;
        }
    }
    /*左斜め下方向の石の色を変える*/
    if((cnt == 1)||(sand_OKflg == FALSE))//cntが1の場合,sand_OKflgがTRUEの場合はなにもしない
    {
        //chg_OKflgはFALSEのままのためなにもしない
    }
    else
    {
        sand_fp = &(Field[Gyo][Retu]);
        for(i=1;i<cnt;i++)
        {
            sand_fp += 7;
            Change_Stone(Turn_Flg,  sand_fp);
        } 
        chg_OKflg = TRUE;//石を置くことが可能
    }
    return chg_OKflg;
}
/**********************************************************
石を変える関数
**********************************************************/
void Change_Stone(int Turn_Flg,int *Fp){
    if(Turn_Flg==BLACK)
    {
        *Fp = BLACK;
        Field[Gyo][Retu] = BLACK;//選択したマスに石を置く
    }
    else
    {
        *Fp = WHITE;
        Field[Gyo][Retu] = WHITE;//選択したマスに石を置く
    }
}
/*********************************************************
breakするか判定する関数
*********************************************************/
int Break_Judge(int Turn_Flg,int *Fp){
    int judge = FALSE;
    if(Turn_Flg==BLACK)
    {
        if(*Fp==BLACK)
        {
            judge = TRUE;//黒のターンで黒を検出したらbreak
        }
    }
    else//if(Turn_Flg==WHITE)
    {
        if(*Fp==WHITE)
        {
            judge = TRUE;//白のターンで白を検出したらbreak
        }
    }
    return judge;
}