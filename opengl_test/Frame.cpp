#include <cmath>
#include <cstdlib>
#include <iostream>
#include <windows.h>
#include <GL/glut.h>
#include <GL/gl.h>  
#include <ctime>
#include <random>

using namespace std;

#define MAX_CHAR    128

int window_width = 600;
int window_height = 400;

#define LEFT_EYE    1
#define RIGHT_EYE   2
#define LEFT_ARM    3
#define RIGHT_ARM   4
#define JU_PAI      5
#define MOUTH       6
#define HAIR        7
#define TIE         8

#define BUFSIZE     512

GLuint selectBuf[BUFSIZE];

const double PI = 3.1415926;

static GLenum mode = GL_RENDER;
/*
GL_ENDER:渲染模式。几何体被光栅化，产生像素片段，
这些片段被写入帧缓冲区。这是正常模式，也是默认模式
*/



//判断被选中的是哪个物体
BOOL leftEye_Selected;
BOOL rightEye_selected;
BOOL leftArm_Selected;
BOOL rightArm_Selected;
BOOL juPai_Selected;
BOOL mouth_Selected;
BOOL hair_Selected;
BOOL tie_Selected;

//数组保存位置
GLuint leftEye[2] = { -65, 50 };
GLuint rightEye[2] = { -135, 50 };
GLuint leftArm[2] = { -50, 0 };
GLuint rightArm[2] = { -150, 0 };
GLuint juPai[2] = { 100, 50 };
GLuint mouth[2] = { -100, 60 };
GLuint hair[2] = { 220, 150 };
GLuint tie[2] = { 100,150 };//gluint:正整型

//对每一个部位设置缩放系数
static int leftEye_N = 0;
static int rightEye_N = 0;
static int leftarm_N = 0;
static int rightarm_N = 0;
static int mouth_N = 0;
static int jupai_N = 0;

//对每一个部分设置旋转角度
static int leftEye_R = 0;
static int rightEye_R = 0;
static int leftarm_R = 0;
static int rightarm_R = 0;
static int mouth_R = 0;
static int jupai_R = 0;


int body[2] = { -100, 50 };
static int theta = 0;

//获取被选中改变颜色的物体
int selectedToChangeColor = -1;
//颜色数组
double lefteye_color[3] = { 1., 1., 1. };
double righteye_color[3] = { 1., 1., 1. };
double leftarm_color[3] = { 1., 1., 1. };
double rightarm_color[3] = { 1., 1., 1. };
double mouth_color[3] = { 1, 0.8, 0 };
double jupai_color[3] = { 1., 1., 1. };
double tie_color[3] = { 1.0, 0.5, 0 };
double hair_color[3] = { 0.8, 0.2, 0.8 };

//菜单颜色
double black[3] = { 0., 0., 0. };
double pink[3] = { 0.95, 0.62, 0.76 };
double yellowishbrown[3] = { 1, 0.8, 0 };
double oranger[3] = { 1.0, 0.5, 0 };
double brown[3] = { 0.5,0.25,0. };
double purple[3] = { 0.8, 0.2, 0.8 };
double white[3] = { 1.0, 1.0, 1.0 };


GLfloat fAspect;

/****************************/
float xx[50], yy[50] = { 0 };
const int N = 40;

void drawString(const char* str) {
    static int isFirstCall = 1;
    static GLuint lists;

    if (isFirstCall) { // 如果是第一次调用，执行初始化
        // 为每一个ASCII字符产生一个显示列表
        isFirstCall = 0;

        // 申请MAX_CHAR个连续的显示列表编号
        lists = glGenLists(MAX_CHAR);

        // 把每个字符的绘制命令都装到对应的显示列表中
        wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
    }
    // 调用每个字符对应的显示列表，绘制每个字符
    for (; *str != '\0'; ++str)
        glCallList(lists + *str);
}
//画圆(tianchong)
void drawCircle(int N, double radius, int angleStart = 0, int angleEnd = 360, double x = 0, double y = 0, double s1 = 1., double s2 = 1.) {
    glBegin(GL_POLYGON);

    double start = (double)angleStart / 360.0 * N;
    double end = (double)angleEnd / 360 * N;
    for (int i = start; i < end + 1; i++) {
        glVertex3f(x + s1 * cos(i * 2 * PI / N) * radius, y + s2 * sin(i * 2 * PI / N) * radius, 0.5);
    }
    glEnd();
}
//画圆（不填充）
void drawCircle2(int N, double radius, int angleStart = 0, int angleEnd = 360, int x = 0, int y = 0, double s1 = 1., double s2 = 1.) {

    glLineWidth(0.6);
    glBegin(GL_LINE_STRIP);

    double start = (double)angleStart / 360.0 * N;
    double end = (double)angleEnd / 360 * N;
    for (int i = start; i < end + 1; i++) {
        glVertex3f(x + s1 * cos(i * 2 * PI / N) * radius, y + s2 * sin(i * 2 * PI / N) * radius, 0.5);
    }
    glEnd();
}

//画左眼
void drawLeftEye(int x, int y) {
    x = x - 5;
    glLoadIdentity();

    glTranslatef(body[0], body[1], 0);
    glRotatef(leftEye_R, 0, 0, 1);
    glTranslatef(-body[0], -body[1], 0);

    
    //眼线
    drawCircle2(100, 10+leftEye_N, 0, 360, x, y);
    //眼珠
    drawCircle(20, 2+leftEye_N, 0, 360, x, y);
    //眼白
    glColor3f(lefteye_color[0], lefteye_color[1], lefteye_color[2]);
    drawCircle(20, 10+leftEye_N, 0, 360, x, y);
}
//画右眼
void drawRightEye(int x, int y) {
    glLoadIdentity();
    
    x = x +5;
    glTranslatef(body[0], body[1], 0);
    glRotatef(rightEye_R, 0, 0, 1);
    glTranslatef(-body[0], -body[1], 0);

    glColor3f(0.0f, 0.0f, 0.0f);
    
    //画眼线
    drawCircle2(100, 10+rightEye_N, 0, 360, x, y);
    //眼珠
    drawCircle(20, 2+rightEye_N, 0, 360, x, y);
    //眼白
    glColor3f(righteye_color[0], righteye_color[1], righteye_color[2]);
    //glTranslatef(-40,50, 0);
    drawCircle(20, 10+rightEye_N, 0, 360, x, y);

}
//画嘴巴
void drawMouth(int x, int y) {
    glLoadIdentity();

    glTranslatef(body[0], body[1], 0);
    glRotatef(mouth_R, 0, 0, 1);
    glTranslatef(-body[0], -body[1], 0);

    //描边
    glColor3f(0, 0, 0);
    
    //上色
    glColor3f(mouth_color[0], mouth_color[1], mouth_color[2]);
    cout << "mouth_N" << mouth_N << endl;
    drawCircle(50, 80+mouth_N, 55, 125, x, y -18 );

    glColor3f(0, 0, 0);
    
}
//画脑袋和身体
void drawHeadandBody(int x, int y) {

    glLoadIdentity();

    glTranslatef(body[0], body[1], 0);
    glRotatef(theta, 0, 0, 1);
    glTranslatef(-body[0], -body[1], 0);
    
    glColor3f(1.0f, 1.0f, 1.0f);
    //画脑袋
    drawCircle(50, 60, 0, 360, x, y);
    drawCircle(60, 70, 0, 360, x, y - 120);
    
}
void drawLeftArm(int x, int y) {

    glLoadIdentity();

    glTranslatef(body[0], body[1], 0);
    glRotatef(leftarm_R, 0, 0, 1);
    glTranslatef(-body[0], -body[1], 0);


    glTranslatef(x, y-15, 0);
    glRotatef(110, 0, 0, 1);
    glTranslatef(-x-30, -y+13, 0);

    glColor3f(1, 1, 1);
    drawCircle(50, 50+leftarm_N, 180, 360, x, y-15, 0.25);
    glColor3f(0, 0, 0);
   

}
void drawRightArm(int x, int y) {

    glLoadIdentity();

    glTranslatef(body[0], body[1], 0);
    glRotatef(rightarm_R, 0, 0, 1);
    glTranslatef(-body[0], -body[1], 0);

    glTranslatef(x-50, y-190, 0);
    glRotatef(-110, 0, 0, 1);
    
    glColor3f(1, 1, 1);
    drawCircle(50, 50+rightarm_N, 180, 360, x, y-15, 0.25);
    glColor3f(0, 0, 0);
    
}

void drawJuPai(int x, int y) {

    glLoadIdentity();

    glTranslatef(x, y, 0);
    glRotatef(jupai_R, 0, 0, 1);
    glTranslatef(-x, -y, 0);

    glTranslatef(x, y, 0);

    glColor3f(0, 0, 0);
    
    //牌子
    glColor3f(jupai_color[0], jupai_color[1], jupai_color[2]);
    glBegin(GL_POLYGON);
    glVertex3f(-30-jupai_N, 20+ 2*jupai_N/3, 0.5);
    glVertex3f(-30- jupai_N, -20- 2 * jupai_N / 3, 0.5);
    glVertex3f(30+ jupai_N, -20- 2 * jupai_N / 3, 0.5);
    glVertex3f(30+ jupai_N, 20+ 2 * jupai_N / 3, 0.5);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3f(-5- jupai_N/3, -20, 0.5);
    glVertex3f(-5- jupai_N/3, -50- 2*jupai_N, 0.5);
    glVertex3f(5+ jupai_N/3, -50- 2*jupai_N, 0.5);
    glVertex3f(5+jupai_N/3, -20 , 0.5);
    glEnd();
    


}

void drawMoxigan(int x, int y) {

    glLoadIdentity();

    glTranslatef(x, y, 0);
    glRotatef(theta, 0, 0, 1);
    glTranslatef(-x, -y, 0);

    glTranslatef(x, y, 0);
    glRotatef(-50, 0, 0, 1);
    glColor3f(hair_color[0], hair_color[1], hair_color[2]);
    drawCircle(50, 50, 0, 90);
    glTranslatef(50, 0, 0);
    glRotatef(30, 0, 0, 1);
    glTranslatef(-50, 0, 0);
    drawCircle(50, 50, 0, 90);
    glTranslatef(50, 0, 0);
    glRotatef(30, 0, 0, 1);
    glTranslatef(-50, 0, 0);
    drawCircle(50, 50, 0, 90);
}

void myDisplay()
{
    /**************伊丽莎白de衣服（开始）**********************/
    /*
    GL_SELECT：选择模式。不会产生像素片段，也不会更改帧缓冲区内容。
    相反，如果渲染模式为GL_render，则会在选择缓冲区中返回绘制的几何体名称的记录，
    该缓冲区必须在输入选择模式之前创建（请参见GL select buffer）
    */
    


    if (mode == GL_SELECT)
        glPushName(JU_PAI);
    drawJuPai(juPai[0], juPai[1]);


   
    /**************伊丽莎白de衣服（结束）**********************/

    /**************伊丽莎白（开始）**********************/
    glColor3f(1.0f, 1.0f, 1.0f);

    if (mode == GL_SELECT)
        glPushName(RIGHT_ARM);
    drawRightArm(rightArm[0], rightArm[1]);


    if (mode == GL_SELECT)
        glPushName(LEFT_ARM);
    drawLeftArm(leftArm[0], leftArm[1]);


    if (mode == GL_SELECT)
        glPushName(MOUTH);
    drawMouth(mouth[0], mouth[1]);
    //画眼睛

    if (mode == GL_SELECT)
        glPushName(LEFT_EYE);
    drawLeftEye(leftEye[0], leftEye[1]);


    if (mode == GL_SELECT)
        glPushName(RIGHT_EYE);
    drawRightEye(rightEye[0], rightEye[1]);

    //华脑袋和身体

    if (mode == GL_SELECT)
        glPushName(-1);
    drawHeadandBody(body[0], body[1]);
    /**************伊丽莎白（结束）**********************/


    glPopMatrix();


}

void testDisplay() {


    glColor3f(1.0, 0.0, 0.0);
    if (mode == GL_SELECT)
        glLoadName(100);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(-0.1, 0, 0);
    glVertex3f(0.1, 0, 0);
    glVertex3f(0.1, 0.1, 0);
    glVertex3f(-0.1, 0.1, 0);
    glEnd();
    glPopMatrix();

    /*glColor3f(0.0, 0.0, 1.0);
    if (mode == GL_SELECT)
        glLoadName(101);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(-0.3, -0.1, 0);
    glVertex3f(-0.2, -0.1, 0);
    glVertex3f(-0.2, -0.2, 0);
    glVertex3f(-0.3, -0.2, 0);
    glEnd();
    glPopMatrix();*/


    /*glColor3f(0.0, 0.0, 1.0);
    if (mode == GL_SELECT)
    glLoadName(102);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(-0.1, -0.1, 0);
    glVertex3f(0.1, -0.1, 0);
    glVertex3f(0.1, -0.2, 0);
    glVertex3f(-0.1, -0.2, 0);
    glEnd();
    glPopMatrix();*/





    //glColor3f(1.0f, 1.0f, 0.0f);
    //if (mode == GL_SELECT)
    //glLoadName(102);
    //glPushMatrix();
    ////drawCircle(10,0.05,0,360,0.5,0.5);
    ///*
    //glBegin(GL_POLYGON);
    //for (int i = 0; i <21; i++){
    //  glVertex3f(0.5 + cos(i * 2 * PI / 20)*0.05, 0.5 + sin(i * 2 * PI / 20) *0.05, 0);
    //}
    //glEnd();*/
    //glBegin(GL_POLYGON);
    //glVertex3f(-0.3, -0.1, 0);
    //glVertex3f(-0.2, -0.1, 0);
    //glVertex3f(-0.2, -0.2, 0);
    //glVertex3f(-0.3, -0.2, 0);
    //glEnd();

    //glPopMatrix();


    //glutSwapBuffers();
}

void judegSectedObject(int x, int y) {

    glClear(GL_COLOR_BUFFER_BIT);//清理缓冲
    GLint hits, viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport); //获得viewport  
    glSelectBuffer(BUFSIZE, selectBuf);   //指定将“图元列表”（点击记录）
    //返回到selectBuf数组中
    cout << "selectbuffer" << *selectBuf << endl;

    mode = GL_SELECT;
    glRenderMode(mode);   //进入选择模式
    glInitNames();

    glMatrixMode(GL_PROJECTION);//进入投影阶段准备拾取
    glPushMatrix();//保存原来的投影矩阵
    glLoadIdentity();


    gluPickMatrix(x, viewport[3] - y, 1, 1, viewport);
    //OpenGL就会创建一个拾取矩阵，分解这个矩阵的话，可以看到，
    //这个矩阵就是上面的移动拾取框到原点，然后再放大为视体大小这两个步骤
    glOrtho(-300, 300, -200, 200, -10, 10);//建立投影矩阵
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    myDisplay();

    //testDisplay();

    glMatrixMode(GL_PROJECTION);

    glPopMatrix();  // 返回正常的投影变换  

    mode = GL_RENDER;
    hits = glRenderMode(mode);
    /*
       GLint glRenderMode (GLenum mode);
   //控制应用程序是处于渲染模式、选择模式还是反馈模式*/
    
    cout << hits;
    
    if (hits > 0) {
        
        GLuint name, * ptr;
        ptr = selectBuf;
        cout << "ptr : " << *ptr << endl;
        int selected_num = 0;
        cout << "hits::" << hits << endl;
        for (int i = 0; i < hits; i++) {
            name = *ptr;           
            ptr += 3;          
            ptr += name - 1;
            BOOL timeToBreak = false;

            if (selected_num == 0) {
                switch (*ptr) {
                case LEFT_EYE:
                    cout << "left eye" << endl;
                    leftEye_Selected = true;
                    selectedToChangeColor = LEFT_EYE;
                    selected_num++;
                    break;
                case RIGHT_EYE:
                    rightEye_selected = true;
                    selectedToChangeColor = RIGHT_EYE;
                    selected_num++;
                    cout << "right eye" << selected_num << endl;
                    break;
                case TIE:
                    cout << "tie" << endl;
                    tie_Selected = true;
                    selectedToChangeColor = TIE;
                    selected_num++;
                    break;
                case JU_PAI:
                    cout << "jupai" << endl;
                    juPai_Selected = true;
                    selectedToChangeColor = JU_PAI;
                    selected_num++;
                    break;
                case HAIR:
                    cout << "hair" << endl;
                    hair_Selected = true;
                    selectedToChangeColor = HAIR;
                    selected_num++;
                    break;
                case LEFT_ARM:
                    cout << "left arm" << endl;
                    leftArm_Selected = true;
                    selected_num++;
                    break;
                case RIGHT_ARM:
                    cout << "right arm" << endl;
                    rightArm_Selected = true;
                    selected_num++;
                    break;
                case MOUTH:
                    cout << "mouth" << endl;
                    mouth_Selected = true;
                    selectedToChangeColor = MOUTH;
                    selected_num++;
                    break;
                default:
                    timeToBreak = false;
                    break;

                }
            }


        }
    }

    glMatrixMode(GL_MODELVIEW);


}
void mouseClick(int btn, int state, int x, int y) {
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        std::cout << "鼠标落下：" << x << "," << y << std::endl;
        judegSectedObject(x, y);//命名不规范，
    }
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        std::cout << "鼠标抬起：" << x << "," << y << std::endl;

        leftEye_Selected = false;
        rightEye_selected = false;
        leftArm_Selected = false;
        rightArm_Selected = false;
        juPai_Selected = false;
        mouth_Selected = false;
        hair_Selected = false;
        tie_Selected = false;

    }
}

void init() {

    glEnable(GL_POINT_SMOOTH);//执行后，过虑线点的锯齿

    glEnable(GL_LINE_SMOOTH);//执行后，过滤线段的锯齿

    //表示是锯消除点采样的质量。 如果应用了一个较大的筛选器函数， 
    //则将提示 GL_NICEST 可能会导致生成 过程中栅格化，更多像素碎片
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); 
    //指示是锯消除行的采样质量。如果应用了一个较大的筛选器函数，
    //则将提示 GL_NICEST 可能会导致生成过程中栅格化，更多像素碎片。
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);  

    glEnable(GL_BLEND);////启用色彩混合

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//定义像素算法。
    //指定红绿蓝和 alpha 源混合因子如何计算。初始值为GL_ONE

    leftEye_Selected = false;
    rightEye_selected = false;
    leftArm_Selected = false;
    rightArm_Selected = false;
    juPai_Selected = false;
    mouth_Selected = false;
    hair_Selected = false;
    tie_Selected = false;

    glEnable(GL_DEPTH_TEST);//启用深度测试。
    //根据坐标的远近自动隐藏被遮住的图形（材料）
    glClearColor(0.5, 0.5, 0.5, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //表示要清除颜色缓冲以及深度缓冲
}

void display() {
    glMatrixMode(GL_PROJECTION);//投影
    glPushMatrix();//入栈
    glLoadIdentity();//重置当前指定的矩阵为单位矩阵.


    glOrtho(-300, 300, -200, 200, -10, 10);//一个平行投影修建空间
    glMatrixMode(GL_MODELVIEW);//投影设置完成后开始画图,需要切换到模型视图矩阵才能正确画图.
    //glLoadIdentity();
    myDisplay();
    glutSwapBuffers();//交换两个缓冲区指针，解决了频繁刷新导致的画面闪烁问题
}
void upToRotate(int key, int x, int y) {

    if (key == GLUT_KEY_RIGHT) {
        if (mouth_Selected) {
            leftEye_R += 8;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            display();
        }
        if (leftArm_Selected) {
            rightEye_R += 8;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            display();
        }
        if (rightArm_Selected) {
            rightarm_R += 8;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            display();
        }
        if (juPai_Selected) {
            jupai_R += 8;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            display();
        }
        if (mouth_Selected) {
            mouth_R += 8;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            display();
        }
        if (leftEye_Selected) {
            leftEye_R += 8;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            display();
        }
        if (rightEye_selected) {
            rightEye_R += 8;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            display();
        }
    }
    if (key == GLUT_KEY_LEFT) {
        if (mouth_Selected) {
            leftEye_R -= 8;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            display();
        }
        if (leftArm_Selected) {
            rightarm_R -= 8;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            display();
        }
        if (rightArm_Selected) {
            rightarm_R -= 8;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            display();
        }
        if (juPai_Selected) {
            jupai_R -= 8;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            display();
        }
        if (mouth_Selected) {
            mouth_R -= 8;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            display();
        }
        if (leftEye_Selected) {
            leftEye_R -= 8;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            display();
        }
        if (rightEye_selected) {
            rightEye_R -= 8;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            display();
        }
    }
    if (key == GLUT_KEY_UP) {
        if (mouth_Selected) {
            mouth_N += 5;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            display();
        }
        if (leftArm_Selected) {
            leftarm_N += 5;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            display();
        }
        if (rightArm_Selected) {
            rightarm_N += 5;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            display();
        }
        if (juPai_Selected) {
            jupai_N += 2;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            display();
        }
        if (mouth_Selected) {
            mouth_N += 5;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            display();
        }
        if (leftEye_Selected) {
            leftEye_N += 2;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            display();
        }
        if (rightEye_selected) {
            rightEye_N += 2;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            display();
        }
        
    }
    if (key == GLUT_KEY_DOWN) {
        if (mouth_Selected) {
            mouth_N -= 5;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            display();
        }
        if (leftArm_Selected) {
            leftarm_N -= 5;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            display();
        }
        if (rightArm_Selected) {
            rightarm_N -= 5;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            display();
        }
        if (juPai_Selected) {
            jupai_N -= 2;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            display();
        }
        if (mouth_Selected) {
            mouth_N -= 5;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            display();
        }
        if (leftEye_Selected) {
            leftEye_N -= 2;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            display();
        }
        if (rightEye_selected) {
            rightEye_N -= 2;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            display();
        }

    }

}
void mouseDrag(int x, int y) {
    std::cout << "鼠标拖动：" << x << "," << y << std::endl;

    if (leftEye_Selected) {
        leftEye[0] = x - window_width / 2;
        leftEye[1] = window_height / 2 - y;
    }
    if (rightEye_selected) {
        rightEye[0] = x - window_width / 2;
        rightEye[1] = window_height / 2 - y;

    }
    if (leftArm_Selected) {
        leftArm[0] = x - window_width / 2;
        leftArm[1] = window_height / 2 - y;
    }
    if (rightArm_Selected) {
        rightArm[0] = x - window_width / 2;
        rightArm[1] = window_height / 2 - y;
    }
    if (juPai_Selected) {
        juPai[0] = x - window_width / 2;
        juPai[1] = window_height / 2 - y;
    }
    if (mouth_Selected) {
        mouth[0] = x - window_width / 2;
        mouth[1] = window_height / 2 - y + 25;
    }
    if (hair_Selected) {
        hair[0] = x - window_width / 2;
        hair[1] = window_height / 2 - y;
    }
    if (tie_Selected) {
        tie[0] = x - window_width / 2;
        tie[1] = window_height / 2 - y;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    display();
    /*glutPostRedisplay();*/
}
void menu(int index) {
    double color[3] = { 0., 0., 0. };

    switch (index)
    {
    case 0:
        color[0] = black[0];
        color[1] = black[1];
        color[2] = black[2];
        break;
    case 1:
        color[0] = pink[0];
        color[1] = pink[1];
        color[2] = pink[2];
        break;
    case 2:
        color[0] = yellowishbrown[0];
        color[1] = yellowishbrown[1];
        color[2] = yellowishbrown[2];
        break;
    case 3:
        color[0] = oranger[0];
        color[1] = oranger[1];
        color[2] = oranger[2];
        break;
    case 4:
        color[0] = brown[0];
        color[1] = brown[1];
        color[2] = brown[2];
        break;
    case 5:
        color[0] = purple[0];
        color[1] = purple[1];
        color[2] = purple[2];
        break;
    case 6:
        color[0] = white[0];
        color[1] = white[1];
        color[2] = white[2];
        break;
    default:
        break;
    }
    switch (selectedToChangeColor)
    {
    case LEFT_EYE:
        cout << "zuoyan yanse " << index << endl;
        lefteye_color[0] = color[0];
        lefteye_color[1] = color[1];
        lefteye_color[2] = color[2];
        break;
    case RIGHT_EYE:
        cout << "youyan yanse " << index << endl;
        righteye_color[0] = color[0];
        righteye_color[1] = color[1];
        righteye_color[2] = color[2];
        break;
    case LEFT_ARM:
        leftarm_color[0] = color[0];
        leftarm_color[1] = color[1];
        leftarm_color[2] = color[2];
        break;
    case RIGHT_ARM:
        rightarm_color[0] = color[0];
        rightarm_color[1] = color[1];
        rightarm_color[2] = color[2];
        break;
    case MOUTH:
        mouth_color[0] = color[0];
        mouth_color[1] = color[1];
        mouth_color[2] = color[2];
        break;
    case JU_PAI:
        jupai_color[0] = color[0];
        jupai_color[1] = color[1];
        jupai_color[2] = color[2];
        break;
    case HAIR:
        hair_color[0] = color[0];
        hair_color[1] = color[1];
        hair_color[2] = color[2];
        break;
    case TIE:
        tie_color[0] = color[0];
        tie_color[1] = color[1];
        tie_color[2] = color[2];
        break;
    default:
        break;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    display();
}

void ChangeSize(int w, int h)
{

    window_width = w;
    window_height = h;

    if (h == 0) h = 1;
    glViewport(0, 0, w, h);

    fAspect = (GLfloat)w / (GLfloat)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();


    if (w <= h)
    {
        glOrtho(-300, 300, -200 * fAspect, 200 * fAspect, -10, 10);
    }
    else
    {
        glOrtho(-300 * fAspect, 300 * fAspect, -200, 200, -10, 10);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}



int main(int argc, char* argv[]) {
    //glut包通用的初始化方法
    glutInit(&argc, argv);
    //函数功能为设置初始显示模式
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(window_width, window_height);
    glutCreateWindow("小菲酱");
    //此处init（）方法为自己写的
    init();

    glutReshapeFunc(ChangeSize);///改变窗口大小时候保持形状比例
    glutDisplayFunc(display);//用于注册一个绘图函数
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseDrag);
    glutSpecialFunc(upToRotate);

    glutCreateMenu(menu);

    glutAddMenuEntry("blank", 0);
    glutAddMenuEntry("pink", 1);
    glutAddMenuEntry("yellow", 2);
    glutAddMenuEntry("orange", 3);    glutAddMenuEntry("brown", 4);
    glutAddMenuEntry("purple", 5);
    glutAddMenuEntry("white", 6);

    glutAttachMenu(GLUT_RIGHT_BUTTON);


    glutMainLoop();
	return 0;
}