#include <stdlib.h>

#include <stdbool.h>

#include <stdio.h>

#include <math.h>

#include <GLUT/glut.h>

#include <OpenGL/gl.h>

#include <OpenGL/glu.h>

#include <time.h>


#define RAD (M_PI / 180.0)


/* グローバル変数 */

int   xBegin = 0;               /* マウスドラッグの始点X座標 */

int   yBegin = 0;               /* マウスドラッグの始点Y座標 */

int   PressButton = 0;          /* 現在押されているマウスボタン（1:左,2:中,3:右） */

float CameraAzimuth   = 90.0;   /* カメラの位置（方位角） */

float CameraElevation = 0.0;    /* カメラの位置（仰角） */

float CameraDistance  = 5.0;    /* カメラの位置（原点からの距離） */

float CameraX = 0.0;            /* カメラの位置（X座標） */

float CameraY = -1.0;            /* カメラの位置（Y座標） */

float CameraZ = 5.0;            /* カメラの位置（Z座標） */

float CubeX = 0.0;              /* 立方体1の位置（X座標） */

float CubeY = -2.0;              /* 立方体1の位置（Y座標） */

float CubeZ = 0.0;              /* 立方体1の位置（Z座標） */

float CubeX2 = 0.0;              /* 立方体2の位置（X座標） */

float CubeY2 = -2.0;              /* 立方体2の位置（Y座標） */

float CubeZ2 = -4.0;              /* 立方体2の位置（Z座標） */

float BulletX = 0.0;            /* 弾1の位置（X座標） */

float BulletY = 0.0;            /* 弾1の位置（Y座標） */

float BulletZ = 0.0;            /* 弾1の位置（Z座標） */

float BulletX2 = 0.0;            /* 弾2の位置（X座標） */

float BulletY2 = 0.0;            /* 弾2の位置（Y座標） */

float BulletZ2 = 0.0;            /* 弾2の位置（Z座標） */

float CubeVx = 0.05;            /* 立方体X方向の速度 */

float CubeVx2 = 0.05;

double kyokai = 1.5;

float CubeVy = 0;               /* 立方体1Y方向の速度 */

float CubeVy2 = 0;              /* 立方体2Y方向の速度 */

int   Shooting = 0;             /* 弾1発射中かどうか */

int   Jumping = 0;              /* ジャンプ1中かどうか */

int   Shooting2 = 0;             /* 弾2発射中かどうか */

int   Jumping2 = 0;              /* ジャンプ2中かどうか */

double hit = 0;   /*弾2の当たり判定*/

double hit2 = 0;  /*弾1の当たり判定*/

char   die_flag = 99;  /*ゲームの終了判定*/

double ntime = 0,ptime = 0; /*時間経過*/




/* 関数のプロトタイプ宣言 */

void display(void);

void timer(int timerID);

void keyboard(unsigned char key, int x, int y );

void mouseButton(int button, int state, int x, int y );

void mouseDrag(int x, int y);

void myInit (char *windowTitle);



/***********************************************************
 
 |  関数：main()
 
 |  説明：メイン関数
 
 |  引数：int argc       実行時引数の数
 
 |  引数：char** argv    実行時引数の内容（文字列配列）
 
 |  戻値：int            0:正常終了
 
 ***********************************************************/

int main(int argc, char** argv)

{
    
    /* 初期化 */
    
    glutInit(&argc, argv);  /* OpenGL の初期化 */
    
    myInit(argv[0]);        /* ウインドウ表示と描画設定の初期化 */
    
    
    
    /* イベント処理ループ */
    
    glutMainLoop();
    
    
    /* プログラム終了 */
    
    return( 0 );
    
}


// 描画関数

void drawString3D(const char *str, float charSize, float lineWidth)

{
    
    glPushMatrix();
    
    glPushAttrib(GL_LINE_BIT);
    
    glScaled(0.001 * charSize, 0.001 * charSize, 0.01);
    
    glLineWidth(lineWidth);
    
    while(*str){
        
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *str);
        
        ++str;
        
    }
    
    glPopAttrib();
    
    glPopMatrix();
    
}


/**********************************************************
 
 |  関数：display()
 
 |  説明：「１枚の」グラフィック描画イベント処理
 
 |  引数：なし
 
 |  戻値：なし
 
 ***********************************************************/

void display(void)

{
    
    /* 初期化 */
    
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   /* 画面を消去 */
    
    glMatrixMode(GL_MODELVIEW);             /* 幾何（描画位置など設定する）モード */
    
    glLoadIdentity();                       /* 幾何を初期化する */
    
    
    
    /* 視点の設定 */
    
    gluLookAt(  CameraX, CameraY, CameraZ,  /* カメラの位置 */
              
              0.0, -1.0, 0.0,  /* 注視点の位置 */
              
              0.0, 1.0, 0.0); /* カメラ上方向のベクトル */
    
    
    
    // 初期状態(文字の描画)
    
    if(die_flag==99){
        
        glPushMatrix ();
        
        glColor3f( 1.0, 1.0, 1.0 ); /* 描画色を白にする */
        
        glTranslatef(-2.0, -1.0, -2.0);
        
        drawString3D("Press ' SPACE KEY ' to start", 2.0, 1.0);
        
        glPopMatrix ();
        
    }
    
    
    
    /* 立方体1の描画 */
    
    glPushMatrix ();                /* 描画位置を保存 */
    
    glColor3f(1.0, 0.0, 0.0);       /* 描画色を白にする */
    
    glTranslatef(CubeX, CubeY, CubeZ); /* 描画位置 */
    
    glutWireCube (0.5);             /* ワイヤーの立方体を描画 */
    
    glPopMatrix ();                 /* 描画位置を戻す */
    
    
    
    /* 立方体2の描画 */
    
    glPushMatrix ();                /* 描画位置を保存 */
    
    glColor3f(0.0, 1.0, 0.0);       /* 描画色を白にする */
    
    glTranslatef(CubeX2, CubeY2, CubeZ2); /* 描画位置 */
    
    glutWireCube (0.5);             /* ワイヤーの立方体を描画 */
    
    glPopMatrix ();                 /* 描画位置を戻す */
    
    
    
    if (Shooting == 1)
        
    {
        
        /* 弾1の描画 */
        
        glPushMatrix ();                /* 描画位置を保存 */
        
        glColor3f(1.0, 0.0, 0.0);       /* 描画色を赤にする */
        
        glTranslatef(BulletX, BulletY, BulletZ); /* 描画位置 */
        
        glutWireSphere (0.3, 10, 10);   /* 球の弾を描画 */
        
        glPopMatrix ();                 /* 描画位置を戻す */
        
    }
    
    
    
    if (Shooting2 == 1)
        
    {
        
        /* 弾2の描画 */
        
        glPushMatrix ();                /* 描画位置を保存 */
        
        glColor3f(0.0, 1.0, 0.0);       /* 描画色を赤にする */
        
        glTranslatef(BulletX2, BulletY2, BulletZ2); /* 描画位置 */
        
        glutWireSphere (0.3, 10, 10);   /* 球の弾を描画 */
        
        glPopMatrix ();                 /* 描画位置を戻す */
        
    }
    
    
    
    if(die_flag==1){
        
        glPushMatrix ();
        
        glColor3f( 0.0, 1.0, 0.0 );/* 描画色を緑にする */
        
        glTranslatef(-2.3, -1.0, -2.0);
        
        drawString3D("GREEN PLAYER WIN !!!", 3.0, 1.0);
        
        glPopMatrix ();
        
    }
    
    
    
    if(die_flag==2){
        
        glPushMatrix ();
        
        glColor3f( 1.0, 0.0, 0.0 );/* 描画色を赤にする */
        
        glTranslatef(-2.0, -1.0, -2.0);
        
        drawString3D("RED PLAYER WIN !!!", 3.0, 1.0);
        
        glPopMatrix ();
        
    }
    
    
    
    if(die_flag==3){
        
        glPushMatrix ();
        
        glColor3f( 1.0, 1.0, 1.0 );/* 描画色を白にする */
        
        glTranslatef(-1.0, -1.0, -2.0);
        
        drawString3D("TIME UP !!!", 3.0, 1.0);
        
        glPopMatrix ();
        
    }
    
    /* 上記で描画されたCGをモニターに出力 */
    
    glutSwapBuffers();
    
}



/***********************************************************
 
 |  関数：timer(int timerID)
 
 |  説明：タイマー（設定時間経過）イベント処理
 
 |  引数：int timerID    イベントが発生したタイマーの識別ID
 
 |  戻値：なし
 
 ***********************************************************/

void timer(int timerID)

{
    
    /* 次のタイマーを15ミリ秒後に設定 */
    
    glutTimerFunc(15, timer, 0);
    
    
    
    switch (die_flag) {
            
            
            
        case 0:
            
            
            
            /* 立方体1を左右に移動させる */
            
            CubeX += CubeVx;
            
            if( CubeX >= kyokai || CubeX <= -kyokai ) CubeVx = -CubeVx;
            
            
            /* 立方体2を左右に移動させる */
            
            CubeX2 -= CubeVx2;
            
            
            if( fabsf(CubeX2) >= kyokai) {
                CubeVx2 = -CubeVx2;
            }
            
            if(CubeX2 < 0.00005 && CubeX2 > -0.00005){
                kyokai -= 0.025;
            }
        
//            if(kyokai >= 0 && CubeX2 == kyokai ){
//                kyokai -= 0.05;
//            }

            
            /* 弾1発射中の計算 */
            
            if (Shooting == 1)
                
            {
                
                BulletZ -= 0.2;
                
                /* 弾1が画面奥まで行ったら消す */
                
                if (BulletZ < -5.0) Shooting = 0;
                
            }
            
            
            
            /* 弾2発射中の計算 */
            
            if (Shooting2 == 1)
                
            {
                
                BulletZ2 += 0.2;
                
                /* 弾2が画面手前まで行ったら消す */
                
                if (BulletZ2 > 1.0) Shooting2 = 0;
                
            }
            
            
            
            /* 立方体それぞれのジャンプ中の計算 */
            
            if (Jumping == 1)
                
            {
                
                CubeY += CubeVy;
                
                CubeVy -= 0.015;
                
                
                
                if (CubeY < -2.0)
                    
                {
                    
                    CubeY = -2.0;
                    
                    Jumping = 0;
                    
                }
                
            }
            
            
            
            if (Jumping2 == 1)
                
            {
                
                CubeY2 += CubeVy2;
                
                CubeVy2 -= 0.015;
                
                
                
                if (CubeY2 < -2.0)
                    
                {
                    
                    CubeY2 = -2.0;
                    
                    Jumping2 = 0;
                    
                }
                
            }
            
            
            
            /*弾2の当たり判定*/
            
            hit =  sqrt(pow(CubeX-BulletX2,2.0)
                        
                        +pow(CubeY-BulletY2,2.0)+pow(CubeZ-BulletZ2,2.0));
            
            if(hit<0.25){
                
                die_flag=1;
                
            }
            
            
            
            /*弾1の当たり判定*/
            
            hit2 =  sqrt(pow(CubeX2-BulletX,2.0)
                         
                         +pow(CubeY2-BulletY,2.0)+pow(CubeZ2-BulletZ,2.0));
            
            if(hit2<0.25){
                
                die_flag=2;
                
            }
            
            
            
            /*一定時間経過はgls11.cを参考に記述した*/
            
            /*一定時間経過でゲーム自動終了*/
            
            ntime =  ntime-(ntime-0.005);
            
            ptime += ntime;
            
            if(ptime>9.5){
                
                die_flag=3;
                
            }
            
            
            
            break;
            
            
            
        case 1:  /* GREEN PLAYER WIN !!! */
            
            break;
            
            
            
        case 2:   /* RED PLAYER WIN !!! */
            
            break;
            
            
            
        case 3:   /* TIME UP !!! */
            
            break;
            
    }
    
    
    
    /* 描画要求（直後に display() 関数が呼ばれる） */
    
    glutPostRedisplay();
    
}



/***********************************************************
 
 |  関数：keyboard()
 
 |  説明：キーボードが押された時のイベント処理
 
 |  引数：unsigned char key  押されたキーの文字コード
 
 |  引数：int x              キーが押されたときのマウスポインタのX座標
 
 |  引数：int y              キーが押されたときのマウスポインタのY座標
 
 |  戻値：なし
 
 ***********************************************************/

void keyboard(unsigned char key, int x, int y )

{
    
    /* キーボード処理 */
    
    switch( key )
    
    {
            
        case ' ':    /* ゲームスタート */
            
            die_flag = 0;
            
            break;
            
            
            
        case 'q':    /* プログラム終了 */
            
            exit( 0 );
            
            break;
            
            
            
            /*弾の軌道、立方体のジャンプはgls11.cを参考に記述した*/
            
            
            
        case 'a':    /* 赤い弾の軌道 */
            
            if (Shooting == 0)
                
            {
                
                BulletX = CubeX;
                
                BulletY = CubeY;
                
                BulletZ = CubeZ;
                
                Shooting = 1;
                
            }
            
            break;
            
            
            
        case 'z':    /* 赤い立方体のジャンプ */
            
            if (Jumping == 0)
                
            {
                
                CubeVy = 0.2;
                
                Jumping = 1;
                
            }
            
            break;
            
            
        case 'k':    /* 緑の弾の軌道 */
            
            if (Shooting2 == 0)
                
            {
                
                BulletX2 = CubeX2;
                
                BulletY2 = CubeY2;
                
                BulletZ2 = CubeZ2;
                
                Shooting2 = 1;
                
            }
            
            break;
            
            
            
        case 'm':    /* 緑の立方体のジャンプ */
            
            if (Jumping2 == 0)
                
            {
                
                CubeVy2 = 0.2;
                
                Jumping2 = 1;
                
            }
            
            break;
            
            
    }
    
    
    
    /* 描画要求（直後に display() 関数が呼ばれる） */
    
    glutPostRedisplay();
    
    
    
    /* コンパイル時の警告対策（定義された変数を使わないと警告になるので） */
    
    x = y = 0;
    
}

/*以下のmoueButton関数、mouseDrag関数、myInt関数はgls11.cより引用した*/

/***********************************************************
 
 |  関数：mouseButton()
 
 |  説明：マウスのボタン操作時のイベント処理
 
 |  引数：int button     操作したマウスボタンの番号
 
 |  引数：int state      操作の種類 GLUT_DOWN（押す）かGLUT_UP（離す）
 
 |  引数：int x          キーが押されたときのマウスポインタのX座標
 
 |  引数：int y          キーが押されたときのマウスポインタのY座標
 
 |  戻値：なし
 
 ***********************************************************/

void mouseButton(int button, int state, int x, int y )

{
    
    /* マウスが押された時 */
    
    if (state == GLUT_DOWN)
        
    {
        
        switch(button)
        
        {
                
            case GLUT_LEFT_BUTTON:  /* マウス左ボタンを押した時の処理 */
                
                PressButton = button;
                
                break;
                
                
                
            case GLUT_MIDDLE_BUTTON:/* マウス中ボタンを押した時の処理 */
                
                break;
                
                
                
            case GLUT_RIGHT_BUTTON: /* マウス右ボタンを押した時の処理 */
                
                PressButton = button;
                
                break;
                
        }
        
        
        
        /* マウスボタンを押した瞬間の始点座標を記録する */
        
        xBegin = x;
        
        yBegin = y;
        
    }
    
}



/***********************************************************
 
 |  関数：mouseDrag()
 
 |  説明：画面上でマウスがドラッグされた時のイベント処理
 
 |  引数：int x          現在のマウスポインタのX座標
 
 |  引数：int y          現在のマウスポインタのY座標
 
 |  戻値：なし
 
 ***********************************************************/

void mouseDrag(int x, int y)

{
    
    /* マウスボタンが押されてからの移動量を計算する */
    
    int xMove = x - xBegin;
    
    int yMove = y - yBegin;
    
    
    
    switch (PressButton)
    
    {
            
            /* マウス左ボタンドラッグ時の処理 */
            
            /* 視点を方位角，仰角に対して回転させる */
            
        case GLUT_LEFT_BUTTON:
            
            CameraAzimuth   += (float)xMove / 2.0;
            
            CameraElevation += (float)yMove / 2.0;
            
            if (CameraElevation >  90.0) CameraElevation =  90.0;
            
            if (CameraElevation < -90.0) CameraElevation = -90.0;
            
            break;
            
            
            
            /* マウス右ボタンドラッグ時の処理 */
            
            /* 視点を奥行き方向に前後に移動させる */
            
        case GLUT_RIGHT_BUTTON:
            
            CameraDistance += (float)yMove / 40.0;
            
            break;
            
    }
    
    CameraX = CameraDistance * cos(CameraAzimuth * RAD) * cos(CameraElevation * RAD);
    
    CameraY = CameraDistance * sin(CameraElevation * RAD);
    
    CameraZ = CameraDistance * sin(CameraAzimuth * RAD) * cos(CameraElevation * RAD);
    
    printf("Camera AZ:%.1f, EL:%.1f, dist:%.1f, x,y,z= %.1f, %.1f, %.1f\n",CameraAzimuth, CameraElevation, CameraDistance, CameraX, CameraY,CameraZ);
    
    
    
    /* 現在のマウスポインタの座標を次の始点用に記録する */
    
    xBegin = x;
    
    yBegin = y;
    
    
    
    /* 描画要求（直後に display() 関数が呼ばれる） */
    
    glutPostRedisplay();
    
}



/***********************************************************
 
 |  関数：myInit()
 
 |  説明：ウインドウ表示と描画設定の初期化
 
 |  引数：char *windowTitle      ウインドウのタイトルバーに表示する文字列
 
 |  戻値：なし
 
 ***********************************************************/

void myInit (char *windowTitle)

{
    
    /* ウインドウのサイズ */
    
    int winWidth  = 400;
    
    int winHeight = 400;
    
    /* ウインドウの縦横の比を計算 */
    
    float aspect = (float)winWidth / (float)winHeight;
    
    
    
    /* OpenGLウインドウ作成までの初期化 */
    
    glutInitWindowPosition(0, 0);                   /* ウインドウ表示位置 */
    
    glutInitWindowSize(winWidth, winHeight);        /* ウインドウサイズ */
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);   /* 描画モード */
    
    glutCreateWindow(windowTitle);                  /* ウインドウの表示 */
    
    glClearColor (0.0, 0.0, 0.0, 1.0);              /* 画面消去色の設定 */
    
    
    
    /* イベント発生時に呼び出す関数の登録 */
    
    glutKeyboardFunc(keyboard);     /* キーボードを押した時 */
    
    glutMouseFunc   (mouseButton);  /* マウスボタンを押した時*/
    
    glutMotionFunc  (mouseDrag);    /* マウスドラッグした時 */
    
    glutDisplayFunc (display);      /* 画面表示 */
    
    glutTimerFunc(15, timer, 0);    /* タイマーを15ミリ秒後に設定 */
    
    
    
    /* CG描画設定 */
    
    glMatrixMode(GL_PROJECTION);    /* 透視投影(遠近投影法)設定モードに切り替え */
    
    glLoadIdentity();               /* 透視投影行列を初期化 */
    
    gluPerspective(45.0, aspect, 1.0, 20.0);    /* 透視投影行列の設定 */
    
    /* 視野角45度, 縦横比 aspect，描画前面までの奥行 1.0，描画背面までの奥行 20.0 */
    
}

