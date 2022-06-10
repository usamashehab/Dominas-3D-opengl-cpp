

#include <GL\glut.h>
#include <math.h>
#include <random>


float rotatex = 0, rotatey = 0, rotatez = 0;

// will be used to prevent random function from being excuted excpect 'r' is pressed
bool cond = true;  
int index1[7];
int index2[7];
void random()
{

        for (int i = 0; i <= 6; i++)
        {
            // generating random int between 0 : 5 to use as an index for choosing points
            index1[i] = rand() % ((5 - 0) + 1) + 0;
            // generating random index  in range(2,8), 8 not included
            index2[i] = rand() % ((8 - 2) + 1) + 2;
        }
   
}


// class square that take array of color as an input 
// generate a square 1 * 1 

void square(GLubyte colors[]) {
    glBegin(GL_QUADS);
    glColor3ub(colors[0], colors[1], colors[2]);
    glVertex3f(-1.0, 1.0, 1.0);
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glEnd();
}

// generate a cube by generating the same square 6 times rotated by 90 degree//  each time
void cube()
{
    // declaring colors which will be used in cube faces
    GLubyte green[] = { 2, 115, 62 };
    GLubyte dark_green[] = { 6, 92, 8 };


    /*OpenGL keeps a stack of matrices to quickly applyand remove transformations.glPushMatrix
    copies the top matrixand pushes it onto the stack, while glPopMatrix pops the top matrix
    off the stack.All transformation functions(glScaled, etc.) function on the top matrix,
    and the top matrix is what all rendering commands use to transform their data.
    By pushingand popping matrices, you can control what transformations apply
    to which objects, as well as apply transformations to groups of objects,
    and easily reverse the transformations so that they don't affect other objects.

     */

     // first square 
    glPushMatrix();  // push this matrix to top of stack to render it or scalling it,....
    glScalef(35, 70, 10);  // scalling the cupe ( x*30, 60*y, 10*z)
    square(green);


    // we don't use glPopMatrix() because we want glScale() to be applied to all squares 

    // 2n square
    glPushMatrix();  // push this matrix to top of stack 
    glRotatef(90, 0, 1, 0);
    square(dark_green);
    glPopMatrix();  // delete this matrix from stack

    // 3d square
    glPushMatrix();  //  push matirx to top of stack
    glRotatef(-90, 0, 1, 0);
    square(dark_green);
    glPopMatrix();

    // 4th square
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    square(dark_green);
    glPopMatrix();

    // 5th square
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    square(dark_green);
    glPopMatrix();

    // 6th square
    glPushMatrix();
    glRotatef(180, 0, 1, 0);
    square(green);
    glPopMatrix();  // deleting matrix of the  6th square from stack

    glPopMatrix();  // deleting matrix of the first square from stack
}

void line(GLfloat offset) {
    glPushMatrix();
    glScalef(25, 1, 1);  // increasing the line width
    glTranslatef(0, 0, offset);  // glTranslatef(X+x, Y +y, Z +z)
    glBegin(GL_LINES);
    glColor3ub(255, 255, 255); // 3f 0:1
    glVertex3f(-1.0, 1.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glEnd();
    glPopMatrix();

}


void point(GLfloat x, GLfloat y, GLfloat z) {

    glPushMatrix();
    // variable x, y, z will let me create points in several places
    glTranslatef(x, y, z);
    glColor3ub(250, 250, 250);
    GLfloat pi = 22.0 / 7.0;
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 360; i++) {
        glVertex3f(8 * sin(i * pi / 180), 8 * cos(i * pi / 180), 1);
    }
    glEnd();
    glPopMatrix();
}

void looping(int positions[][6][2], int index, int half)
{    
    for (int k = 0; k < 6; k++)
    {
        // check if we achive the end of the array
        if (positions[index][k][0] == -1) {
            break;
        }
        else
        {   // 6 and 7 are indexes for lower half of 2 and 3 stones,and they 
            // already prepared to be drawen, as they are special cases
            // so no need to (Z * -1)
            if (index == 6 || index == 7)
            {
                half = 1;
            }
            // if half == 1 it's upper half else --> lower half( Z * -1) to be reversed
            point(positions[index][k][0], half * positions[index][k][1], 10);  
        }

    }
}

void display()
{

    glClearColor(0, 0, 0, 0);  // setting the background of the window
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0, 0.5, 1);
    glMatrixMode(GL_MODELVIEW);
    glLineWidth(2);
    int x = 55, y = -25, z = 25;  // default values for translate
    if (cond) {  // check if 'r' is pressed to generate new random stones
        random();
        cond = false;
    }
    for (int i = 0; i < 7; i++)
    {
        glPushMatrix();
        glTranslatef(x * i, y * i, z * i);
        glRotatef(30, 1, 1, 0);
        glRotatef(5, 0, 0, 1);
        glRotatef(rotatey, 0, 1, 0);
        glRotatef(rotatex, 1, 0, 0);
        glRotatef(rotatez, 0, 0, 1);
        cube();  // draw stone

        line(10);  // draw line

        // array to store positions of points for each stone, it's an array 
        // of unknown number of arrays , each array from these sub arrays,
        // include 6 arrays(6 is number of maximum points in stone), each 
        // array is a position for point each one of these 6 arrays
        // include 2 int numbers as x,y for point 
        // each array that it's length < 6 will contain {-1,-1} at it's end
        // as a flag to it's end, beacause each array length is 6 , this 
        // will cause logic error (draw additional point in the center),
        // as the default value for int arrays is {0,0} 
        // stones 2 ,3 are special cases , if we multiply it's Z value by -1
        // as the other stones it will draw as it's mirrored
        /// so we create seperated half(upper , lower) for each one 
        int positions[][6][2] = {
            { {20,55},{-20,15}, {-1,-1}} ,                                  // upper half stone 2
            { {-20,15},{20,55},{0,35}, {-1,-1}} ,                           // upper half stone 3
            { {0,35},{-1,-1}},                                              // stone 1
            { {-20,15},{-20,55},{20,15}, {20,55},{-1,-1} },                 // stone 4
            { {-20,15},{-20,55}, {0,35},{20,15}, {20,55},{-1,-1} },         // stone 5
            { {-20,15}, {-20,35}, {-20,55}, {20,15}, {20,35}, {20,55}},     // stone 6
            { {-20,-55},{20,-15},{-1,-1}},                                   // lower half stone 2
            { {0,-35},{-20,-55},{20,-15},{-1,-1}}                                  // lower half stone 3                              
        };

        looping(positions, index1[i], 1);  // draw upper half of stones


        looping(positions, index2[i], -1);  // draw lower half of stones

        glPopMatrix();  // deleting first matrix from stack
    }







    glFlush();  // draw
    glutSwapBuffers();
    glutPostRedisplay();
}
// generate new random dominas
void keyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0);
    else if (key == 'r') cond = true;
    glutPostRedisplay();
}
/// control rotation with arrows 
void specFunc(int key, int x1, int y1) {
    switch (key)
    {
    case GLUT_KEY_UP: rotatex += 2; break;
    case GLUT_KEY_DOWN: rotatex -= 2; break;
    case GLUT_KEY_RIGHT: rotatey += 2; break;
    case GLUT_KEY_LEFT: rotatey -= 2; break;
    case GLUT_KEY_RIGHT + GLUT_KEY_DOWN: rotatey += 2; rotatez -= 2; break;
    case GLUT_KEY_LEFT + GLUT_KEY_UP: rotatey -= 2; rotatez += 2; break;
    case GLUT_KEY_LEFT + GLUT_KEY_DOWN: rotatey -= 2; rotatez -= 2; break;

    }
    glutPostRedisplay();
}
int main(int argc, char* argv[])
{


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(100, 10);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("dominos");
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutSpecialFunc(specFunc);
    glOrtho(-500, 500, -500.0, 500, -500.0, 500); //not gluOrtho
    glutMainLoop(); // repeat code
}
