# Image-too-ASCII-art
An application that converts images to ASCII art. The executable takes in the following user inputs:
+ `--txt` -> directory/name of txt file (ex. ./test.txt)
+ `--quant` -> quantization level (number of displayed intensities 1 for just black and white; 2 for 3 intensities ...).
+ `--dir` -> directory of image (*optional) (if not provided the program accesses the built in camera and takes a foto).
+ `--flip` -> 1 if you want to flip your image over the y-axis (*optional).
+ `--resolution` -> width height of output ASCII img (*optional). (ex. 108 192)

Example: 
+ Running the following command: `./Image-too-ASCII-art --txt ./Marilyn.txt --quant 12 --dir ./Description\ media /marilyn.jpg`
+ The input image:
  <br />
 ![alt text](https://github.com/hogfig/Image-too-ASCII-art/blob/main/Description%20media/marilyn.jpg)
+ The output image when ajusting the font size to 1:
 ![alt text](https://github.com/hogfig/Image-too-ASCII-art/blob/main/Description%20media/Marilynq12.png)
+ The closeup of the ASCII art:
 ![alt text](https://github.com/hogfig/Image-too-ASCII-art/blob/main/Description%20media/Marilynq12-closeup.png)


