#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
//THIS THE TEST FILE WE WORKIN WIT DAWG
#if defined WIN32
#include <freeglut.h>
#elif defined __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif
using namespace std;

// Program Variables

enum Shape {none, circle, cross};
void pressKey ( unsigned char key, int x, int y );
void output(float r, float g, float b, float x, float y, int length, char *string);
bool checkWinX();
bool checkWinO();
float  mX = 0;
float mY = 0;
int option = 0;
int width = 700;
int height = 700;
bool home = true;
bool player1 = true;
bool gridOn = false;
bool ai = false;
bool PvP = false;
bool empty[9] = {true, true, true, true, true, true, true, true, true};
void MouseFunc(int b, int s, int x, int y);
int gameCounter = 0;
int moves[9] = {0};
int winGrid[9][9] =
{
    {0,0,0},
    {0,0,0},
    {0,0,0}
};

struct Shapes{
    
    float x = 0;
    float y = 0;
    float w = 0;
    float h = 0;
    Shape shape;
    
    Shapes(){
        x = -0.8;
        y = 0.8;
        w = 0.4;
        h = 0.4;
        shape = none;
    }
    
    Shapes(float x, float y, float w, float h){
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
        shape = none;
    }
    
    void draw(){
        
        if(home)
        {
            homeScreen();
        }
        if(gameCounter == 9 && !checkWinO() && !checkWinX())
        {
            output(0, 0, 1, -0.21, 0.3, 9, "NO WINNER");
            output( 1, 1, 0,-0.21, -0.3, 10, "GAME OVER!");
            output( 1, 1, 0, -0.35, -0.38, 24, "Press Space for new game");
        }
        
        if(shape == cross){
            glLineWidth(10);
            glColor3f(1, 0, 0);
            glBegin(GL_LINES);
            glVertex2f(x + 0.1, y - 0.1);
            glVertex2f(x + w - 0.1, y - h + 0.1);
            glVertex2f(x + 0.1, y - h + 0.1);
            glVertex2f(x + w - 0.1, y - 0.1);
            glEnd();
        }
        
        if(shape == circle){
            
            glLineWidth(10);
            glColor3f(0, 1, 0);
            glBegin(GL_LINES);
            for(int i = 0; i <= 360; i ++){
                float theta = i*M_PI/180;
                glVertex2f((w/2 - 0.1)*cos(theta) + (x  + 0.2), (w/2 - 0.1)*sin(theta) + (y - 0.2));
            }
            glEnd();
            
        }
        
        if(gridOn){
        glColor3f(1, 1, 1);
        glBegin(GL_POLYGON);
        glVertex2f(x, y);
        glVertex2f(x + w,  y);
        glVertex2f( x + w,  y - h);
        glVertex2f( x, y - h);
        glEnd();
    }
    }
    
    
    //I DONT UNDERSTAND HOW THIS FUNCTION WORKS
    bool contains(float  localmX, float localmY){
        return (mX > x && mX < x + w) && (mY < y && mY > y - h);
    }
    
    void homeScreen(){
        output(1, 1, 1, -0.3, 0, 18, "1) Player V Player");
        output(1, 1, 1, -0.34, -0.2, 20, "2) Player V Computer");
    }
    
    
};

vector<Shapes>  grid;

Shapes R1;




void appDrawScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //BackGround Color
    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    for(int i = 0; i < grid.size(); i++){
        grid[i].draw();
    }
    
    if(checkWinX()){
        gameCounter = 9;
        output(1, 0, 0, -0.3, 0.3, 16, "X IS THE WINNER!");
        output( 1, 1, 0,-0.21, -0.3, 10, "GAME OVER!");
        output( 1, 1, 0, -0.35, -0.38, 24, "Press Space for new game");
    }
    if(checkWinO()){
        gameCounter = 9;
        output(0, 1, 0, -0.3, 0.3, 16, "O IS THE WINNER!");
        output( 1, 1, 0,-0.21, -0.3, 10, "GAME OVER!");
        output( 1, 1, 0, -0.35, -0.38, 24, "Press Space for new game");
    }
    
    
    
    
    glFlush();
    glutSwapBuffers();
}





void idle(){
    
    glutPostRedisplay();
}


int main(int argc, char** argv){
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(20, 60);
    glutInitWindowSize(width, height);
    glutCreateWindow("Test Project");
    glMatrixMode ( GL_PROJECTION );
    
    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    
    
    grid.push_back(Shapes(-0.8, 0.8, 0.4, 0.4));   //First container of vector
    grid.push_back(Shapes(-0.2, 0.8, 0.4, 0.4));
    grid.push_back(Shapes(0.4, 0.8, 0.4, 0.4));
    
    grid.push_back(Shapes(-0.8, 0.2, 0.4, 0.4));
    grid.push_back(Shapes(-0.2, 0.2, 0.4, 0.4));
    grid.push_back(Shapes(0.4, 0.2, 0.4, 0.4));
    
    grid.push_back(Shapes(-0.8, -0.4, 0.4, 0.4));
    grid.push_back(Shapes(-0.2, -0.4, 0.4, 0.4));
    grid.push_back(Shapes(0.4, -0.4, 0.4, 0.4));
    
    
    
    glutDisplayFunc(appDrawScene);
    glutMouseFunc(MouseFunc);
    
    // Set callback for resizing th window
    // glutReshapeFunc(appReshapeFunc);
    
    //Functions for Keyboard Input
    glutKeyboardFunc ( pressKey );
    
    
    //glutSpecialUpFunc ( releaseKey );
    
    // Set callback to handle mouse dragging
    //    glutMotionFunc(appMotionFunc);
    //glutSpecialFunc(appSpecialKey);
    //  glutSpecialUpFunc(releaseKey);
    //   glOrtho ( 0, 640, 640, 0, -1, 1 );
    
    // Start the main loop
    glutIdleFunc(idle);
    glutMainLoop();
}


void windowToScene(float& x, float& y) {
    x = (2.0f*(x / float(width))) - 1.0f;
    y = 1.0f - (2.0f*(y / float(height)));
}




//-------------------------------------------------------
// A function to handle mouse clicks
// Called every time the mouse button goes up or down
// Arguments:
//    b    - mouse button that was clicked, left or right
//    s      - state, either mouse-up or mouse-down
//    x, y - coordinates of the mouse when click occured
//-------------------------------------------------------
void MouseFunc(int b, int s, int x, int y) {
    mX = (float)x;
    mY = (float)y;
    windowToScene( mX, mY);
    //b = 0 left clipck
    // b = 2 right click
    
    //s = 0 mouse down!
    if(b == 0){
        
        if (s == 0){
            
            for (int i = 0; i < grid.size(); i++){
                if(grid[i].contains( mX, mY)){
                    if (empty[i] && gameCounter != 9){
                        if(player1){
                            grid[i].shape = cross;
                        }
                      else if(PvP){
                            grid[i].shape = circle;
                       }
                        empty[i] = false;
                        gameCounter++;
                        player1 = !player1;
                    }
                
                        //break; I dont have this but it works fine?
                    
                    if(ai){
                        for (int i =0; i < grid.size(); i++) {
                             //if(grid[i].contains( mX, mY))
                            if(empty[i] && gameCounter != 9){
                                grid[i].shape = circle;
                                empty[i] = false;
                                gameCounter++;
                                player1 = !player1;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}





bool checkWinX(){
    if(grid[0].shape == cross && grid[3].shape == cross && grid[6].shape == cross)
        return true;
    if(grid[1].shape == cross && grid[4].shape == cross && grid[7].shape == cross)
        return true;
    if(grid[2].shape == cross && grid[5].shape == cross && grid[8].shape == cross)
        return true;
    if(grid[0].shape == cross && grid[1].shape == cross && grid[2].shape == cross)
        return true;
    if(grid[3].shape == cross && grid[4].shape == cross && grid[5].shape == cross)
        return true;
    if(grid[6].shape == cross && grid[7].shape == cross && grid[8].shape == cross)
        return true;
    if(grid[0].shape == cross && grid[4].shape == cross && grid[8].shape == cross)
        return true;
    if(grid[2].shape == cross && grid[4].shape == cross && grid[6].shape == cross)
        return true;
    
    return false;
}

bool checkWinO(){
    if(grid[0].shape == circle && grid[3].shape == circle && grid[6].shape == circle)
        return true;
    if(grid[1].shape == circle && grid[4].shape == circle && grid[7].shape == circle)
        return true;
    if(grid[2].shape == circle && grid[5].shape == circle && grid[8].shape == circle)
        return true;
    if(grid[0].shape == circle && grid[1].shape == circle && grid[2].shape == circle)
        return true;
    if(grid[3].shape == circle && grid[4].shape == circle && grid[5].shape == circle)
        return true;
    if(grid[6].shape == circle && grid[7].shape == circle && grid[8].shape == circle)
        return true;
    if(grid[0].shape == circle && grid[4].shape == circle && grid[8].shape == circle)
        return true;
    if(grid[2].shape == circle && grid[4].shape == circle && grid[6].shape == circle)
        return true;
    
    return false;
}









void pressKey ( unsigned char key, int x, int y ){
   
    switch (key) {
    if(gameCounter == 9){
            case ' ':
                for (int i = 0; i < 9; i++) {
                    empty[i] = true;
                    grid[i].shape = none;
                    player1 = true;
                }
                gameCounter = 0;
                break;
        }
            case 49:
                home = false;
                ai = false;
            PvP = true;
                gridOn = true;
                break;
        case 50:
            home = false;
            ai = true;
            PvP = false;
            gridOn = true;
            default:
                break;
        }
        
    
    glutPostRedisplay();
}

void releaseKey(int key, int x, int y)
{
    glutPostRedisplay();
}


void output(float r, float g, float b, float x, float y, int length, char *string)
{
    glColor3f( r, g, b );
    glRasterPos2f(x, y);
    
    for (int i = 0; i < length; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
    }
}



