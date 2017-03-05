#pragma once

#include "glfw/gl.h"

namespace glfw {

class DrawUtility
{
	public:
		DrawUtility();
		~DrawUtility();

		void frameStart();
		void frameEnd();
		void resize(int w, int h);

        void setColor(float r, float g, float b, float alpha);

		void drawRectangle(float a,float b);
		void drawBox(float lx, float ly, float lz);
		void drawSphere(float radius,int slices,int stacks);
		void drawCylinder(float radius,float length,int slices,int stacks);
		void drawTrimesh(const float* vertices, const unsigned int* indices, size_t index_count);

	protected:

	private:
        GLUquadricObj *quad_;

};

}
