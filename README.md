# pixmap-ops

Image manipulation demos based on the PPM image format.

TODO: Add a representative image for your project here

## How to build

*Windows*

Open git bash to the directory containing this repository.

```
pixmap-ops $ mkdir build
pixmap-ops $ cd build
pixmap-ops/build $ cmake -G "Visual Studio 17 2022" ..
pixmap-ops/build $ start pixmap-ops.sln
```

Your solution file should contain two projects: `pixmap_art` and `pixmap_test`.
To run from the git bash command shell, 

```
pixmap-ops/build $ ../bin/Debug/pixmap_test
pixmap-ops/build $ ../bin/Debug/pixmap_art
```

*macOS*

Open terminal to the directory containing this repository.

```
pixmap-ops $ mkdir build
pixmap-ops $ cd build
pixmap-ops/build $ cmake ..
pixmap-ops/build $ make
```

To run each program from build, you would type

```
pixmap-ops/build $ ../bin/pixmap_test
pixmap-ops/build $ ../bin/pixmap_art
```

## Image operators

This image editor supports basic class features for creating, loading, and saving images, as well as commonly used operators, such as resizing, flipping vertically, creating a subimage, replacing parts of images with other images, performing gamma corrections, performing alphablend operations, and creating a greyscale effect. Below are custom features that have also been implemented:

## add

![earthPlusSoup.png](Examples/earthPlusSoup.png)

Takes the color values and adds them. Caps at 255 for RGB values.

## subtract

![earthMinusSoup.png](Examples/earthMinusSoup.png)

Takes the color values and subtracts them. Minimum is 0 for RGB values.

## multiply

![earthSquared.png](Examples/earthSquared.png)

Takes the color values and multiplies them. Caps at 255, and has a minimum of 0.

## lightest

![earthVsAxolotl.png](Examples/earthVsAxolotl.png)

For each RGB value, the lightest value of two images is chosen.

## darkest

![axolotlVsEarth.png](Examples/axolotlVsEarth.png)

For each RGB value, the darkest of two images is chosen.

## Swirl

![earthSwirl.png](Examples/earthSwirl.png)

Each pixel is altered, with the red values becoming the green values, the green values becoming the blue values, and the blue values becoming the red values.

## addBorder

![borderBeast.png](Examples/borderBeast.png)

Creates a larger image with a pattern based on row and column position, then inserts the original image in the middle.

## invert

![theBeast.png](Examples/theBeast.png)

Takes each RGB value, and subtracts them from 255 to get the final color.

## flipHorizontal

![beastHFlip.png](Examples/beastHFlip.png)

Writes the pixels back into the image backwards to produce an upside-down version of the image.

## rotate90

![beastrot90.png](Examples/beastrot90.png)

Writes the column and row data in place of eachother. Produces a tilted version of the image.

## average

![averageAxolotlR.png](Examples/averageAxolotlR.png)

![averageAxolotlG.png](Examples/averageAxolotlG.png)

![averageAxolotlB.png](Examples/averageAxolotlB.png)

Takes each RGB value, and compares them to a color provided by the user. Takes the average RGB value between the two to give the image a colored tint.

## tesselate

![wordlySoupBetter.png](Examples/wordlySoupBetter.png)

Creates a small version of a given image, scales it to a desired magnitude, and replaces each pixel in the image with said subimage. Each subimage is also averaged with the original pixel color.

## Results

Below are some images created using default and custom features:

## "Ghoulish"

![ghoulish.png](Examples/ghoulish.png)

For this image, I subtracted an inverted image of earth from a regular image of earth. I then resized a soup image to the same dimensions of the earth image, and subtracted the original earth image from the soup image. I then added a 25 pixel border.

## "World of Axolotl"

![worldOfAxolotl.png](Examples/worldOfAxolotl.png)

I have tesselated the axolotl image onto the earth, then added a 30 pixel border.

## "Inception"

![inception.png](Examples/inception.png)

Not much more going on here than before. This one was mostly done to test the limits of the tesselate function, which definitely do exist. I have tesselated the inverted picture of mr beast onto "World of Axolotl"

## "Echoes"

![echoes.png](Examples/echoes.png)

This image consists of the averaged axolotl images, as seen earlier, but this time they have been progressively scaled by 1/2 and stacked on top of eachother.
