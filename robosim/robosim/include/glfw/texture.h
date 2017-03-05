#pragma once

#include "glfw/gl.h"

namespace glfw
{
//***************************************************************************
// PPM image object

typedef unsigned char byte;

class Image {
  int image_width,image_height;
  byte *image_data;
public:
  Image (const char *filename);
  // load from PPM file
  ~Image();
  int width() { return image_width; }
  int height() { return image_height; }
  byte *data() { return image_data; }
};


//***************************************************************************
// Texture object.

class Texture {
  GLuint name;
public:
  Texture (const char *filename);
  ~Texture();
  void bind (int modulate);
};

}
