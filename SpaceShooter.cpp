#include <iostream>
#include<SDL/SDL.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

#define M_PI 3.14159265

bool gameOver = false;
float top = 0.0;
float fast = 0.0;
float depth = 0.0;
float spaceMove = 0.0;
float ajust = 0.0;
int enemyShips = 0;
int randomX[28][15];
int randomY[28][15];
int shooting = 1;
float enemySpeed;
float negEnemySpeed;
std::vector<float> bullets;
std::vector<float> bulletLoc;
std::vector<float> bulletZ;
std::vector<float> shipX;
std::vector<float> shipY;
std::vector<float> shipDepth;
std::vector<float> curSpeed;

void ball(float scale,float x,float y)
{
    glPushMatrix();
    glTranslatef(x,y,0.0);
    glScalef(scale,scale,0.0);
    for (float i=0; i<360; i++)
    {
        glPushMatrix();
        glScalef(0.2,0.2,0.2);
        glBegin(GL_POINTS);
        glVertex3f(cos(i*M_PI/180),sin(i*M_PI/180),0.0);
        glEnd();
        glPopMatrix();
    }
    glPopMatrix();
}

void triangle(float scale, float x, float y)
{
    glPushMatrix();
    glTranslatef(x,y,0.0);
    glScalef(scale,scale,0.0);
    glBegin(GL_TRIANGLES);
        glVertex3f(0.0,-0.5,0.0);
        glVertex3f(0.0, 0.5,0.0);
        glVertex3f(0.7, 0.0,0.0);
    glEnd();
    glPopMatrix();
}

void square(float scale, float x, float y)
{
    glPushMatrix();
    glTranslatef(x,y,0.0);
    glScalef(scale,scale,0.0);
    glBegin(GL_QUADS);
        glVertex3f(-0.75,-0.5,0.0);
        glVertex3f(-0.75,0.5,0.0);
        glVertex3f(0.75,0.5,0.0);
        glVertex3f(0.75,-0.5,0.0);
    glEnd();
    glPopMatrix();
}

void space(float z){
    for(int j = 0; j < 28; j++){
    glPushMatrix();
    glColor3f(0.4,0.4,0.9);
    if (spaceMove + j + ajust <= -15.0){
        ajust=-spaceMove;
    }
    glTranslatef(-5.0 + j + spaceMove + ajust,0.0,0.0 - z);

    for (int i = 0; i < 15; i++){
        glPushMatrix();
        glBegin(GL_LINES);
        glVertex3f(-0.2 + (float)randomX[j%15][i]/10, 3.5 - (i*0.5) + (float)randomY[j%15][i]/10,0.0);
        glVertex3f(0.2 + (float)randomX[j%15][i]/10, 3.5 - (i*0.5) + (float)randomY[j%15][i]/10,0.0);
        glEnd();
        glPopMatrix();
    }
    glPopMatrix();
    }
}

void shoot(float top){
    if (shooting == 40){
        bullets.push_back(top);
        bulletLoc.push_back(0.0);
        bulletZ.push_back(depth);
    }
    if (gameOver == true){
        for (int i =0; i < bullets.size(); i++){
            bullets.erase(bullets.begin());
            bulletLoc.erase(bulletLoc.begin());
            bulletZ.erase(bulletZ.begin());
        }
    }
    if (bullets.size() >= 14){
        bullets.erase(bullets.begin());
        bulletLoc.erase(bulletLoc.begin());
        bulletZ.erase(bulletZ.begin());
    }
    for (int i = 0; i < bullets.size(); i++){
        glPushMatrix();
        glColor3f(1.0+float(bulletZ[i])/7,1.0+float(bulletZ[i])/7,0.0);
        glTranslatef(-2.5,0.0,0.0);
        glBegin(GL_QUADS);
        glVertex3f(bulletLoc[i],bullets[i]-0.05,0.0+bulletZ[i]);
        glVertex3f(bulletLoc[i]+0.3,bullets[i]-0.05,0.0+bulletZ[i]);
        glVertex3f(bulletLoc[i]+0.3,bullets[i]+0.05,0.0+bulletZ[i]);
        glVertex3f(bulletLoc[i],bullets[i]+0.05,0.0+bulletZ[i]);
        glEnd();
        if (gameOver == false){
        bulletLoc[i]+=0.008;
        }
        glPopMatrix();
    }
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45,640.0/480.0,1.0,500.0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
}



void EnemyShip(float enemyTop, float enemyDepth, float enemyRight){
    glPushMatrix();
    glTranslatef(0.0,enemyTop,enemyDepth);
    glColor3f(0.0+enemyDepth/7,0.0+enemyDepth/7,1.0);
    square(0.2,0.0+enemyRight,-0.1);
    square(0.2,0.0+enemyRight,0.1);
    glColor3f(0.5+enemyDepth/15,0.0,0.7+enemyDepth/15);
    square(0.7,0.1+enemyRight,0.0);
    glColor3f(0.4+enemyDepth/3,1.0,1.0);
    square(0.2,0.6+enemyRight,-0.2);
    square(0.2,0.6+enemyRight,0.2);
    triangle(-1.0,0.0+enemyRight,0.0);
    glPopMatrix();
}

void enemyAI(){
    enemySpeed = 0.004;//float(rand()%2-1)/100;
    negEnemySpeed = -enemySpeed;
    if (enemyShips == 400 && gameOver == false){
        shipX.push_back(5.4);
        shipY.push_back(float(rand()%4)-2.0);
        shipDepth.push_back(0.0);
        curSpeed.push_back(enemySpeed);
    }
    if (gameOver == true){
        for (int i = 0; i < shipX.size(); i++){
            shipX.erase(shipX.begin());
            shipY.erase(shipY.begin());
            shipDepth.erase(shipDepth.begin());
        }
    }
    for(int i =0; i<shipY.size(); i++){
        EnemyShip(shipY[i],shipDepth[i],shipX[i]);
        if (gameOver == false){
        shipX[i]=shipX[i]-0.001-fast;
        shipY[i]+=curSpeed[i];
        }
        if (shipY[i] > 2.5 && curSpeed[i] > 0){curSpeed[i] = negEnemySpeed;}
        if (shipY[i] < -2.5 && curSpeed[i] < 0){curSpeed[i] = enemySpeed;}
        if (shipX[i] <= -3.4){gameOver = true;}
    }
    for (int i = 0; i < bullets.size(); i++){
        for (int j = 0; j < shipX.size(); j++)
        {
            if (bulletLoc[i]-2.5>=shipX[j]-0.25 && bulletLoc[i]-2.5<=shipX[j]+0.25){
                if (bullets[i]>=shipY[j]-0.25 && bullets[i]<=shipY[j]+0.25){
                shipX.erase(shipX.begin()+j);
                shipY.erase(shipY.begin()+j);
                shipDepth.erase(shipDepth.begin()+j);
                bullets.erase(bullets.begin()+i);
                bulletLoc.erase(bulletLoc.begin()+i);
                bulletZ.erase(bulletZ.begin()+i);
                }
            }
        }
    }
}

void spaceShip(){
    glPushMatrix();
    glTranslatef(0.0,top,depth);
    glColor3f(1.0+float(depth)/7,1.0+float(depth)/7,0.0);
    square(0.2,-3.4,0.1);
    square(0.2,-3.4,-0.1);
    glColor3f(0.0,0.5+float(depth)/15,0.7+float(depth)/15);
    square(0.7,-3.5,0.0);
    glColor3f(0.6+float(depth)/10,0.0,0.0);
    square(0.2,-4.0,0.2);
    square(0.2,-4.0,-0.2);
    triangle(1.0,-3.4,0.0);
    glPopMatrix();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0,0.0,-9.0);
    spaceShip();
    enemyAI();
    shoot(top);
    space(0.0);
    space(1.0);
    space(2.0);

}

int main(int argc, char* args[])
{
    srand(time(NULL));
    for(int j = 0; j < 28; j++){
        for(int i = 0; i < 15; i++){
            randomX[j][i] = rand()%10;
            randomY[j][i] = rand()%2;
         }
    }

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_SetVideoMode(640,480,32,SDL_SWSURFACE|SDL_OPENGL);

    int loop=1;
    SDL_Event myevent;
    SDL_EnableKeyRepeat(10,10);

    init();
    while (loop==1)
    {
        if (gameOver == false){
        spaceMove = spaceMove - 0.0050;
        shooting = (shooting+1)%100;
        enemyShips = (enemyShips+1)%600;
        if (fast <=0.01){
            fast+=0.000001;
        }
        }else{fast = 0.0; top = 0.0;}
        while (SDL_PollEvent(&myevent))
        {
            switch(myevent.type)
            {
                case SDL_QUIT:
                loop=0;
                break;

                case SDL_KEYDOWN:
                if (gameOver == false){
                if (myevent.key.keysym.sym==SDLK_w&&top<2.7) top+=0.08;
                if (myevent.key.keysym.sym==SDLK_s&&top>-2.7) top-=0.08;

                //if (myevent.key.keysym.sym==SDLK_a&&depth<=-1.0) depth+=1.5;
                //if (myevent.key.keysym.sym==SDLK_d&&depth>=-2.0) depth-=1.5;
                }
                if (myevent.key.keysym.sym==SDLK_RETURN) {gameOver = false;}
                break;
            }
        }
        display();
        SDL_GL_SwapBuffers();
    }
    SDL_Quit();
    return 0;
}



