// Copyright 2021, Aline Normoyle, alinen

#include <iostream>
#include "image.h"
using namespace std;
using namespace agl;

int main(int argc, char** argv)
{
   Image image;
   if (!image.load("../images/feep.png")) {
      std::cout << "ERROR: Cannot load image! Exiting...\n";
      exit(0);
   }
   
   for (int i = 0; i < image.height(); i++) {
      for (int j = 0; j < image.width(); j++) {
         Pixel c = image.get(i, j);
         std::cout << "(" << (int)c.r << "," << (int)c.g << "," << (int)c.b << ") ";
      }
      std::cout << std::endl;
   }
   image.save("feep-test-save.png"); // should match original
   
   // should print 4 4
   cout << "loaded feep: " << image.width() << " " << image.height() << endl;

   // test: copy constructor
   Image copy = image; 
   copy.save("feep-test-copy.png"); // should match original and load into gimp

   // test: assignment operator
   copy = image; 
   copy.save("feep-test-assignment.png"); // should match original and load into gimp

   // should print r,g,b
   Pixel pixel = image.get(1, 1);
   cout << (int) pixel.r << " " << (int) pixel.g << " " << (int) pixel.b << endl;

   // test: setting a color
   pixel.r = 255;
   image.set(1, 1, pixel);
   image.save("feep-test-newcolor.png");

   Image newImage = image.flipHorizontal();
   newImage.save("newFeep.png");

   // test a non-trivial image
   if (!image.load("../images/earth.png")) {
      std::cout << "ERROR: Cannot load image! Exiting...\n";
      exit(0);
   }

   // should print 400 400
   cout << "loaded earth: " << image.width() << " " << image.height() << endl;
   // resize

   Image resize = image.resize(200,300);
   resize.save("earth-200-300.png");

   // grayscale
   Image grayscale = image.grayscale(); 
   grayscale.save("earth-grayscale.png");

   // flip horizontal
   Image flip = image.flipHorizontal(); 
   flip.save("earth-flip.png"); 

   // sub image
   Image sub = image.subimage(200, 200, 100, 100); 
   sub.save("earth-subimage.png"); 

   // gamma correction
   Image gamma = image.gammaCorrect(0.6f); 
   gamma.save("earth-gamma-0.6.png"); 

   gamma = image.gammaCorrect(2.2f);
   gamma.save("earth-gamma-2.2.png"); 

   // alpha blend
   Image soup;
   soup.load("../images/soup.png");

   // Ghoulish
   Image ghoulish = image.subtract(image.invert());
   Image soupy = soup.resize(400, 400);
   ghoulish = soupy.subtract(image);
   ghoulish = ghoulish.addBorder(25);
   ghoulish.save("ghoulish.png");

   
   Image axolotl;
   if (!axolotl.load("../images/Axolotl.png")) {
      std::cout << "ERROR: Cannot load image! Exiting...\n";
      exit(0);
   }

   // World of Axolotl
   Image resizer = image.resize(40, 40);
   Image tiles = resizer.tesselate(axolotl, 1.0f);
   tiles = tiles.addBorder(30);
   tiles.save("worldOfAxolotl.png");

   Image beast;
   if (!beast.load("../images/MrBeastTile.png")) {
      std::cout << "ERROR: Cannot load image! Exiting...\n";
      exit(0);
   }

   // Examples
   Image borderBeast = beast.addBorder(100);
   borderBeast.save("borderBeast.png");

   beast = beast.invert();
   beast.save("theBeast.png");

   Image Hflip = beast.flipHorizontal();
   Hflip.save("beastHFlip.png");

   Image rot90 = beast.rotate90();
   rot90.save("beastrot90.png");

   Image swirly = image.swirl();
   swirly.save("earthSwirl.png");

   Image adding = image.add(soup.resize(400, 400));
   adding.save("earthPlusSoup.png");

   Image subbing = image.subtract(soup.resize(400, 400));
   subbing.save("earthMinusSoup.png");

   Image multiplying = image.multiply(image);
   multiplying.save("earthSquared.png");

   Image light = (axolotl.resize(400, 400)).lightest(image);
   light.save("earthVsAxolotl.png");

   Image dark = (axolotl.resize(400, 400)).darkest(image);
   dark.save("axolotlVsEarth.png");

   Image averageAxolotl = axolotl.average({255, 0, 0});
   averageAxolotl.save("averageAxolotlR.png");

   averageAxolotl = axolotl.average({0, 255, 0});
   averageAxolotl.save("averageAxolotlG.png");
   
   averageAxolotl = axolotl.average({0, 0, 255});
   averageAxolotl.save("averageAxolotlB.png");

   Image tesSoup = soup.resize(40, 40);
   tesSoup = tesSoup.tesselate(image, 0.7f);
   tesSoup.save("wordlySoup.png");

   // End of Examples

   //Inception
   Image inception = tiles.resize(400, 400);
   Image doubleInception = inception.tesselate(beast, 0.05f);
   doubleInception.save("inception.png");

   //Echoed axolotls
   Image EchoesR = axolotl.average({255, 0, 0});

   Image EchoesG = axolotl.average({0, 255, 0}); 
   EchoesG = EchoesG.resize((EchoesG.width() / 2), (EchoesG.height() / 2));

   Image EchoesB = axolotl.average({0, 0, 255}); 
   EchoesB = EchoesB.resize((EchoesB.width() / 4), (EchoesB.height() / 4));

   int gy = (int) (0.5f * (axolotl.width() - EchoesG.width()));
   int gx = (int) (0.5f * (axolotl.height() - EchoesG.height()));

   int by = (int) (0.5f * (axolotl.width() - EchoesB.width()));
   int bx = (int) (0.5f * (axolotl.height() - EchoesB.height()));

   EchoesR.replace(EchoesG, gx, gy);
   EchoesR.replace(EchoesB, bx, by);
   EchoesR.save("echoes.png");

   int y = (int) (0.5f * (image.width() - soup.width()));
   int x = (int) (0.5f * (image.height() - soup.height()));
   Image background = image.subimage(x, y, soup.width(), soup.height());
   background.save("background-test.png");
   Image blend = background.alphaBlend(soup, 0.5f);
   image.replace(blend, x, y);
   image.save("earth-blend-0.5.png");
}

