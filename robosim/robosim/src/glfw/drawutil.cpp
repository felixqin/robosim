#include "glfw/gl.h"
#include "glfw/drawutil.h"

namespace glfw {

DrawUtility::DrawUtility()
{
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

    quad_ = gluNewQuadric();
}

DrawUtility::~DrawUtility()
{
    gluDeleteQuadric(quad_);
}

void DrawUtility::frameStart()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset The View
}

void DrawUtility::frameEnd()
{
    //TODO
}
void DrawUtility::resize(int width, int height)
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

void DrawUtility::setColor(float r, float g, float b, float alpha)
{
    //TODO
}

void DrawUtility::drawRectangle(float a,float b)
{
    float lx = a/2;
    float ly = b/2;

    glBegin(GL_QUADS);
      glNormal3f(0,0,1);
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-lx,ly,0);
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-lx,-ly,0);
      glTexCoord2f(1.0f, 0.0f); glVertex3f(lx,-ly,0);
      glTexCoord2f(1.0f, 1.0f); glVertex3f(lx,ly,0);
    glEnd();
}

void DrawUtility::drawBox(float lx, float ly, float lz)
{
    GLfloat hx = lx*0.5f;
    GLfloat hy = ly*0.5f;
    GLfloat hz = lz*0.5f;

    const GLfloat vertex_list[] = {
        -hx,-hy,-hz,
        -hx, hy,-hz,
        -hx, hy, hz,
        -hx,-hy, hz,
         hx,-hy,-hz,
         hx, hy,-hz,
         hx, hy, hz,
         hx,-hy, hz
    };

    //前后左右四个面顶点索引
    static const GLuint index_list1[] = {
        4, 5, 6, 7,
        0, 3, 2, 1,
        1, 2, 6, 5,
        0, 4, 7, 3,
    };
    //上下两个面顶点索引
    static const GLuint index_list2[] = {
        2, 3, 7, 6,
        0, 1, 5, 4
    };
    //前后左右四个面顶点纹理
    static const GLint texture_coord_list1[] = {
        0, 0,   1, 0,   1, 1,   0, 1,
        1, 0,   0, 0,   0, 1,   1, 1,
    };
    //上下两个面顶点纹理
    static const GLint texture_coord_list2[] = {
        0, 0,   1, 0,   1, 1,   0, 1,
        0, 1,   1, 1,   1, 0,   0, 0,
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertex_list);

    glTexCoordPointer(2,GL_INT,0,texture_coord_list1);
    glDrawElements(GL_QUADS, 16, GL_UNSIGNED_INT, index_list1);

    glTexCoordPointer(2,GL_INT,0,texture_coord_list2);
    glDrawElements(GL_QUADS,  8, GL_UNSIGNED_INT, index_list2);
}
void DrawUtility::drawTrimesh(const float* vertices, const unsigned int* indices, size_t index_count)
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, indices);
}

void DrawUtility::drawSphere(float radius,int slices,int stacks)
{
    gluSphere(quad_,radius,slices,stacks);
}

void DrawUtility::drawCylinder(float radius,float length,int slices,int stacks)
{
    glTranslatef(0,0,-length/2);
    gluCylinder(quad_,radius,radius,length,slices,stacks);

    glTranslatef(0,0,length);
    gluDisk(quad_,0,radius,slices,stacks);

    glRotatef(180,1,0,0);
    glTranslatef(0,0,length);
    gluDisk(quad_,0,radius,slices,stacks);
}


}
