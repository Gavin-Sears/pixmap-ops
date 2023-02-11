// Copyright 2021, Aline Normoyle, alinen

/**
* Assignment 02: By the power of greyscale!
*
* This program is a photo editing tool that 
* currently supports loading and saving images,
* as well as returning the width and height, 
* getting and setting pixels either by row or
* by index. One can also implement a variety
* of visual effects which alter the colors and
* /or incorporate other images.
*
* leaks --atExit -- ../bin/pixmap_test
*
* lldb ../bin/pixmap_test
*
* r (for run)
*
* thread backtrace
*
* quit
*
* @author: S. Gavin Sears
* @version: February 3, 2023
*/

#include <typeinfo>
#include <iostream>

#include "image.h"

#include <cassert>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace agl {

// Default Constructor
Image::Image() {
   _width = 0;
   _height = 0;

   _data = NULL;
}

// General Constructor
Image::Image(int width, int height)  {
   _width = width;
   _height = height;

   _data = new unsigned char[3 * _width * _height];
}

// Copy
Image::Image(const Image& orig) {
   _width = orig._width;
   _height = orig._height;

   _data = new unsigned char[3 * _width * _height];
   memcpy(_data, orig._data, sizeof(unsigned char) * 3 * _height * _width);
}

// Assignment
Image& Image::operator=(const Image& orig) {
  if (&orig == this) {
    return *this;
  }

   delete[] _data;

   _data = new unsigned char[3 * orig._width * orig._height];

   _width = orig._width;
   _height = orig._height;
   memcpy(_data, orig._data, sizeof(unsigned char) * 3 * _height * _width);

  return *this;
}

// Destructor
Image::~Image() {
   delete[] _data;
}

// Getters
int Image::width() const { 
   return _width;
}

int Image::height() const {
   return _height;
}

unsigned char* Image::data() const {
   return _data;
}

// Setter
void Image::set(int width, int height, unsigned char* data) {
   _width = width;
   _height = height;
   
   delete[] _data;

   _data = new unsigned char[3 * _width * _height];
   memcpy(_data, data, sizeof(unsigned char) * 3 * _height * _width);
}

// Loader
bool Image::load(const std::string& filename, bool flip) {

   delete[] _data;
   _data = NULL;

   // Getting file info
   int x, y, n;
   unsigned char* img = stbi_load(filename.c_str(), &x, &y, &n, 3);

   // Checking that file was read
   if (img != NULL) {
      set(x, y, img);
      stbi_image_free(img);
      return true;
   }
   else {
      return false;
   }

}


bool Image::save(const std::string& filename, bool flip) const {

   int myNewImage = stbi_write_png(filename.c_str(), 
                                    _width, 
                                    _height, 
                                    // Number of Channels (RGB for now)
                                    3,
                                    // Pixels
                                    _data, 
                                    // Byte stride
                                    (_width * 3)
                                    );

   // Checking that file was saved
   if (myNewImage != 0) { 
      return true;
   }
   else {
      return false;
   }
}

// Pixel getter by row and column
Pixel Image::get(int row, int col) const {

  // Convert row and column to position
  int pos = (3 * ((row * _width) + col));

  // Indexing for RGB values
  return Pixel{ _data[pos], _data[pos + 1], _data[pos + 2] };
}

// Pixel setter by row and column
void Image::set(int row, int col, const Pixel& color) {

   // Finding position to set
   int pos = (3 * ((row * _width) + col));

   // Changing values of RGB
   _data[pos] = color.r;
   _data[pos + 1] = color.g;
   _data[pos + 2] = color.b;

}

// Pixel getter by index
Pixel Image::get(int i) const {

   return Pixel { 
         _data[i * 3], 
         _data[i * 3 + 1], 
         _data[i * 3 + 2] 
   };
}

// Pixel setter by index
void Image::set(int i, const Pixel& c) {

   _data[i * 3] = c.r;
   _data[i * 3 + 1] = c.g;
   _data[i * 3 + 2] = c.b;
}

// Image resize
Image Image::resize(int w, int h) const {
   // New image to translate to
   Image result(w, h);

   // Placeholders for loop
   int origRow;
   int origCol;
   int origPos;
   Pixel colorful = {};

   // For each row (y)
   for (int r = 0; r < h; ++r) {
      
      // For each column (x)
      for (int c = 0; c < w; ++c) {

         // Calculating corresponding row/column on original picture
         origRow = (int)((r / (h - (float)1)) 
                           * (_height - 1)) / 1;

         origCol = (int)((c / (w - (float)1)) 
                           * (_width - 1)) / 1;

         // Getting original pixel color
         colorful = get(origRow, origCol);

         // Setting pixel on new image
         result.set(r, c, colorful);
      }
      
   }

   return result;
}

Image Image::flipHorizontal() const {
   Image result(_width, _height);

   Pixel colorful = {};

   // For each row (y)
   for (int r = 0; r < _height; ++r) {
      
      // For each column (x)
      for (int c = 0; c < _width; ++c) {

         // Getting current pixel
         colorful = get(r, c);

         // Setting pixels in reverse order
         result.set((_height - 1 - r), (_width - 1 - c), colorful);
      }

   }

   return result;
}

Image Image::flipVertical() const {
   Image result(_width, _height);

   Pixel colorful = {};

   // For each row (y)
   for (int r = 0; r < _height; ++r) {

      // For each column (x)
      for (int c = 0; c < _width; ++c) {

         colorful = get(r, c);

         result.set(r, (_width - 1 - c), colorful);
      }
   }

   return result;
}

Image Image::rotate90() const {
   Image result(_height, _width);

   Pixel colorful = {};

   // For each row (y)
   for (int r = 0; r < _height; ++r) {

      // For each column (x)
      for (int c = 0; c < _width; ++c) {

         colorful = get(r, c);

         result.set(c, r, colorful);
      }
   }

   return result;
}

Image Image::subimage(int startx, int starty, int w, int h) const {
   Image sub(w, h);

   // Tracking Pixel
   Pixel colorful = {};

   // For each row in area (y)
   for (int r = 0; r < h; ++r) {
      
      // For each column in area (x)
      for (int c = 0; c < w; ++c) {

         // Getting current pixel offset by start
         colorful = get(r + starty, c + startx);

         // Setting pixels in reverse order
         sub.set(r, c, colorful);
      }

   }

    return sub;
}

void Image::replace(const Image& image, int startx, int starty) {
   Pixel colorful = {};

   // For each row in area (y)
   for (int r = 0; r < image.height(); ++r) {
      
      // For each column in area (x)
      for (int c = 0; c < image.width(); ++c) {

         // Getting current pixel offset by start
         colorful = image.get(r, c);

         // Setting pixel
         set((r + startx), (c + starty), colorful);
      }

   }
  
}

Image Image::swirl() const {
   Image result(_width, _height);

   // Tracking Pixel
   Pixel colorful = {};

   // For each row in area (y)
   for (int r = 0; r < _height; ++r) {
      
      // For each column in area (x)
      for (int c = 0; c < _width; ++c) {

         // Getting current pixel offset by start
         colorful = get(r, c);

         // Subtracting colors from 255
         colorful = {
            colorful.g,
            colorful.b,
            colorful.r
         };

         // Setting pixels in reverse order
         result.set(r, c, colorful);
      }

   }
   
   return result;
}

Image Image::add(const Image& other) const {
   Image result(_width, _height);

   // Tracking Pixel
   Pixel colorful = {};
   // Tracking Pixel other
   Pixel otherful = {};

   // Final colors
   int myR;
   int myG;
   int myB;

   // For each row in area (y)
   for (int r = 0; r < _height; ++r) {
      
      // For each column in area (x)
      for (int c = 0; c < _width; ++c) {

         // Getting current pixel
         colorful = get(r, c);
         // Getting curent pixel from other
         otherful = other.get(r, c);

         myR = (int)colorful.r + (int)otherful.r;
         myG = (int)colorful.g + (int)otherful.g;
         myB = (int)colorful.b + (int)otherful.b;

         if (myR > 255) {
            myR = 255;
         }
         if (myG > 255) {
            myG = 255;
         }
         if (myB > 255) {
            myB = 255;
         }

         // Subtracting colors from 255
         colorful = {
            (unsigned char)myR,
            (unsigned char)myG,
            (unsigned char)myB
         };

         // Setting pixels in reverse order
         result.set(r, c, colorful);
      }

   }

   return result;
}

Image Image::subtract(const Image& other) const {
   Image result(_width, _height);

   // Tracking Pixel
   Pixel colorful = {};
   // Tracking Pixel other
   Pixel otherful = {};

   // Final colors
   int myR;
   int myG;
   int myB;

   // For each row in area (y)
   for (int r = 0; r < _height; ++r) {
      
      // For each column in area (x)
      for (int c = 0; c < _width; ++c) {

         // Getting current pixel
         colorful = get(r, c);
         // Getting curent pixel from other
         otherful = other.get(r, c);

         myR = (int)colorful.r - (int)otherful.r;
         myG = (int)colorful.g - (int)otherful.g;
         myB = (int)colorful.b - (int)otherful.b;

         if (myR < 0) {
            myR = 0;
         }
         if (myG < 0) {
            myG = 0;
         }
         if (myB < 0) {
            myB = 0;
         }

         // Subtracting colors from 255
         colorful = {
            (unsigned char)myR,
            (unsigned char)myG,
            (unsigned char)myB
         };

         // Setting pixels in reverse order
         result.set(r, c, colorful);
      }

   }
   
   return result;
}

Image Image::multiply(const Image& other) const {
   Image result(_width, _height);

   // Tracking Pixel
   Pixel colorful = {};
   // Tracking Pixel other
   Pixel otherful = {};

   // Final colors
   int myR;
   int myG;
   int myB;

   // For each row in area (y)
   for (int r = 0; r < _height; ++r) {
      
      // For each column in area (x)
      for (int c = 0; c < _width; ++c) {

         // Getting current pixel
         colorful = get(r, c);
         // Getting curent pixel from other
         otherful = other.get(r, c);

         myR = (int)colorful.r * (int)otherful.r;
         myG = (int)colorful.g * (int)otherful.g;
         myB = (int)colorful.b * (int)otherful.b;

         
         if (myR > 255) {
            myR = 255;
         } else if (myR < 0) {
            myR = 0;
         }
         if (myG > 255) {
            myG = 255;
         } else if (myG < 0) {
            myG = 0;
         }
         if (myB > 255) {
            myB = 255;
         } else if (myB < 0) {
            myB = 0;
         }

         // Subtracting colors from 255
         colorful = {
            (unsigned char)myR,
            (unsigned char)myG,
            (unsigned char)myB
         };

         // Setting pixels in reverse order
         result.set(r, c, colorful);
      }

   }

   return result;
}

Image Image::difference(const Image& other) const {
   Image result(0, 0);

   return result;
}

Image Image::lightest(const Image& other) const {
   Image result(_width, _height);

   // Tracking Pixel
   Pixel colorful = {};
   // Tracking Pixel other
   Pixel otherful = {};

   // Final colors
   int myR;
   int myG;
   int myB;

   // For each row in area (y)
   for (int r = 0; r < _height; ++r) {
      
      // For each column in area (x)
      for (int c = 0; c < _width; ++c) {

         // Getting current pixel
         colorful = get(r, c);
         // Getting curent pixel from other
         otherful = other.get(r, c);
         
         if (colorful.r >= otherful.r) {
            myR = colorful.b;
         } else {
            myR = otherful.r;
         }
         if (colorful.g >= otherful.g) {
            myG = colorful.g;
         } else {
            myG = otherful.g;
         }
         if (colorful.b >= otherful.b) {
            myB = colorful.b;
         } else {
            myB = otherful.b;
         }

         // Subtracting colors from 255
         colorful = {
            (unsigned char)myR,
            (unsigned char)myG,
            (unsigned char)myB
         };

         // Setting pixels in reverse order
         result.set(r, c, colorful);
      }

   }
  
   return result;
}

Image Image::darkest(const Image& other) const {
   Image result(_width, _height);

   // Tracking Pixel
   Pixel colorful = {};
   // Tracking Pixel other
   Pixel otherful = {};

   // Final colors
   int myR;
   int myG;
   int myB;

   // For each row in area (y)
   for (int r = 0; r < _height; ++r) {
      
      // For each column in area (x)
      for (int c = 0; c < _width; ++c) {

         // Getting current pixel
         colorful = get(r, c);
         // Getting curent pixel from other
         otherful = other.get(r, c);
         
         if (colorful.r <= otherful.r) {
            myR = colorful.b;
         } else {
            myR = otherful.r;
         }
         if (colorful.g <= otherful.g) {
            myG = colorful.g;
         } else {
            myG = otherful.g;
         }
         if (colorful.b <= otherful.b) {
            myB = colorful.b;
         } else {
            myB = otherful.b;
         }

         // Subtracting colors from 255
         colorful = {
            (unsigned char)myR,
            (unsigned char)myG,
            (unsigned char)myB
         };

         // Setting pixels in reverse order
         result.set(r, c, colorful);
      }

   }
  
   return result;
}

Image Image::gammaCorrect(float gamma) const {
   Image result(_width, _height);

   // Intensities of each color
   float intensR;
   float intensG;
   float intensB;
   int myR;
   int myG;
   int myB;
   Pixel colorful = {};

   // For each row (y)
   for (int r = 0; r < _height; ++r) {
      
      // For each column (x)
      for (int c = 0; c < _width; ++c) {

         // Getting current pixel
         colorful = get(r, c);

         // Getting intensities for inverse gamma
         intensR = colorful.r / 255.0f;
         intensG = colorful.g / 255.0f;
         intensB = colorful.b / 255.0f;

         // Getting inverse gamma, then finding final color
         myR = (int)(255 * pow(intensR, 1/gamma));
         myG = (int)(255 * pow(intensG, 1/gamma));
         myB = (int)(255 * pow(intensB, 1/gamma));

         // Defining new pixel
         colorful = {
            (unsigned char)myR, 
            (unsigned char)myG, 
            (unsigned char)myB 
         };

         // Setting new pixel
         result.set(r, c, colorful);
      }

   }
 
   return result;
}

Image Image::alphaBlend(const Image& other, float alpha) const {
   // this.pixels = this.pixels * (1-alpha) + other.pixel * alpha
   Image result(_width, _height);

   Pixel colorful = {};
   Pixel colorOther = {};

   int colorfulR;
   int colorfulG;
   int colorfulB;

   // For each row (y)
   for (int r = 0; r < _height; ++r) {
      
      // For each column (x)
      for (int c = 0; c < _width; ++c) {

         // Getting current pixel
         colorful = get(r, c);
         colorOther = other.get(r, c);

         // Defining colors
         colorfulR = ((int)colorful.r * 
                     (1.0f - alpha)) + 
                     ((int)colorOther.r * alpha);
         colorfulG = ((int)colorful.g * 
                     (1.0f - alpha)) + 
                     ((int)colorOther.g * alpha);
         colorfulB = ((int)colorful.b * 
                     (1.0f - alpha)) + 
                     ((int)colorOther.b * alpha);

         // Defining new pixel
         colorful = {
            (unsigned char)colorfulR, 
            (unsigned char)colorfulG, 
            (unsigned char)colorfulB
         };

         // Setting new pixel
         result.set(r, c, colorful);
      }

   }

   return result;
}

Image Image::invert() const {
   Image image(_width, _height);

   // Tracking Pixel
   Pixel colorful = {};

   // For each row in area (y)
   for (int r = 0; r < _height; ++r) {
      
      // For each column in area (x)
      for (int c = 0; c < _width; ++c) {

         // Getting current pixel offset by start
         colorful = get(r, c);

         // Subtracting colors from 255
         colorful = {
            (unsigned char)(255 - (int)colorful.r),
            (unsigned char)(255 - (int)colorful.g),
            (unsigned char)(255 - (int)colorful.b)
         };

         // Setting pixels in reverse order
         image.set(r, c, colorful);
      }

   }
   
   return image;
}

// Grayscale operator
Image Image::grayscale() const {
   Image result(_width, _height);

   // Placeholders for loop
   int intens;
   Pixel colorful = {};

   // For each row (y)
   for (int r = 0; r < _height; ++r) {
      
      // For each column (x)
      for (int c = 0; c < _width; ++c) {

         // Getting current RGB value
         colorful = get(r, c);

         // Setting intensity
         intens = (int)((1 / (float)3) * 
                  (colorful.r + colorful.g + colorful.b));

         colorful = {
            (unsigned char)intens, 
            (unsigned char)intens, 
            (unsigned char)intens
         };

         // Setting pixel on new image
         result.set(r, c, colorful);
      }
      
   }

   return result;
}

Image Image::colorJitter(int size) const {
   Image image(0, 0);
  
   return image;
}

Image Image::bitmap(int size) const {
   Image image(0, 0);

   return image;
}

Image Image::addBorder(int wid) const {
   Image image(_width + (wid * 2), _height + (wid * 2));

   int myR;
   int myG;
   int myB;
   
   Pixel colorful = {};

   // For each row (y)
   for (int r = 0; r < (_height + (wid * 2)); ++r) {
      
      // For each column (x)
      for (int c = 0; c < (_width + (wid * 2)); ++c) {

         // Creating fun border effect
         myR = r;
         myG = c;
         myB = (c / 3);

         // Clamping RGB values
         if (myR > 255) {
            myR = 255;
         } else if (myR < 0) {
            myR = 0;
         }
         if (myG > 255) {
            myG = 255;
         } else if (myG < 0) {
            myG = 0;
         }
         if (myB > 255) {
            myB = 255;
         } else if (myB < 0) {
            myB = 0;
         }

         colorful = {
            (unsigned char)myR, 
            (unsigned char)myG, 
            (unsigned char)myB
         };

         // Setting pixel on new image
         image.set(r, c, colorful);
      }
      
   }

   image.replace(*this, wid, wid);

   return image;
}

Image Image::average(const Pixel& c) const {
   Image result(_width, _height);

   Pixel colorful = {};

   int origR = c.r;
   int origG = c.g;
   int origB = c.b;

   int myR;
   int myG;
   int myB;

   // For each row (y)
   for (int r = 0; r < (_height); ++r) {
      
      // For each column (x)
      for (int c = 0; c < (_width); ++c) {

         colorful = get(r, c);

         // Creating fun border effect
         myR = ((int)colorful.r + origR) / 2;
         myG = ((int)colorful.g + origG) / 2;
         myB = ((int)colorful.b + origB) / 2;

         // Clamping RGB values
         if (myR > 255) {
            myR = 255;
         } else if (myR < 0) {
            myR = 0;
         }
         if (myG > 255) {
            myG = 255;
         } else if (myG < 0) {
            myG = 0;
         }
         if (myB > 255) {
            myB = 255;
         } else if (myB < 0) {
            myB = 0;
         }

         colorful = {
            (unsigned char)myR, 
            (unsigned char)myG, 
            (unsigned char)myB
         };

         // Setting pixel on new image
         result.set(r, c, colorful);
      }
      
   }

   return result;

}

Image Image::tesselate(const Image& other, float scale) const {
   // Each mini image will have a size dependant on the scale
   int miniWidth = (int)(other.width() * scale);
   int miniHeight = (int)(other.height() * scale);

   Image result((miniWidth * _width), (miniHeight * _height));

   // Subimage
   Image sub;

   Pixel colorful = {};

   // For each starting y position for a given subimage
   for (int iy = 0; iy < (miniHeight * _height); iy += miniHeight) {

      // For each starting x position for a given subimage
      for (int ix = 0; ix < (miniWidth * _width); ix += miniWidth) {

         sub = other.resize(miniWidth, miniHeight);

         colorful = get((iy / miniHeight), (ix / miniWidth));

         sub = sub.average(colorful);

         result.replace(sub, iy, ix);

      }

   }

   return result;

}

void Image::fill(const Pixel& c) {

}
// namespace agl

}

