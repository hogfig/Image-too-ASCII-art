#include <opencv2/opencv.hpp>
#include<iostream>
#include<math.h>
#include <chrono>
#include <thread>
#include <cstdlib>
#include<vector>
#include<fstream>

using namespace std;
using namespace cv;

//quantize image to K+1 intensities
void quantize(Mat& img, int K){
   img = img*(K/255.0);
   img = img*(255.0/K);
   img.convertTo(img,CV_8U);
}

bool CaptureImage(Mat& img){
   VideoCapture cap(0);//Declaring an object to capture stream of frames from default camera//
   if (!cap.isOpened()){ //This section prompt an error message if no video stream is found//
      cout << "No video stream detected" << endl;
      system("pause");
      return false;
   }
   //wait for camera to open 
   std::this_thread::sleep_for(200ms);

   cap >> img;
   cap.release();//Releasing the buffer memory//
   return true;
}

void PreprocessImage(Mat& img, const int& K, const int& width,const int& height, const bool& flip){
   if(width>0 && height>0){
      resize(img, img, Size(width, height), INTER_LINEAR);
   }
   if(flip){
      cv::flip(img,img,1);
   }
   //Convert image to gray.
   cv::cvtColor(img, img,6); 
   quantize(img,K);
}

void IntCharPairs(vector<pair<int, char>>& int_char_pairs){
   //$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/|()1{}[]?-_+~<>i!lI;:,\"^`'.
   //.'`^\",:;Il!i><~+_-?][}{1)(|/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$
   string gradient = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/|()1{}[]?-_+~<>i!lI;:,\"^`'.";
   int size_intesity = int_char_pairs.size();
   int size_ASCII_intensity = gradient.size();
   int step = size_ASCII_intensity/size_intesity;

   for(int i = 0; i<size_intesity; i++){
      int index = (size_ASCII_intensity-1.0)/(size_intesity-1) * i;
      int_char_pairs[i].second = gradient[index];
   }
}

void CalculateHistogram(Mat& hist,Mat& img){
   int histSize[] = {256};
   float range[] = { 0, 256 };
   const float* histRange[] = { range };
   bool uniform = true, accumulate = false;
   calcHist( &img, 1, 0, Mat(), hist, 1, histSize, histRange, uniform, accumulate);
}

void MapChar(Mat& img, ofstream& MyFile, vector<pair<int, char>>& int_char_pairs){
   //calculate histogram
   Mat hist;
   CalculateHistogram(hist, img);

   for(int i=0; i<256; i++){
      if(hist.at<int>(0,i)>0){
         int_char_pairs.push_back({i,' '});
      }
   }
   //Compleate the data structure maping the respective intensity with a character.
   IntCharPairs(int_char_pairs);
}

void WriteImgToFile(vector<pair<int, char>>& int_char_pairs, Mat& img, ofstream& MyFile){
   Size img_size = img.size();
   for(int i=0; i<img_size.height;i++){
      for(int j=0; j<img_size.width;j++){
         //map value in image to ASCII and write to file
         int img_intensity = (int)img.at<uchar>(i,j);
         auto it = find_if(int_char_pairs.begin(),int_char_pairs.end(),[&img_intensity](const pair<int, char>& element){return element.first == img_intensity;});
         MyFile << it->second;
      }
      MyFile<<endl;
   }
}

/*
   User input:
      --txt -> directory and name of txt file
      --quant -> quantization levels.
      --dir -> directory of image (*optional)
      --flip -> 1 if you want to flip your image over the y-axis (*optional).
      --resolution -> width height of output ASCII img (*optional).
*/
int main(int argc, char* argv[]) {
   string text_file, dir = {};
   bool flip=false; int width, height={};
   int obligatory_count {0};
   // Number of quantization levels.
   int K={};
   //Check user input.
   for(int i=0;i<argc;i++){
      string s = argv[i];
      if(s=="--txt"){
         text_file = argv[i+1];
         obligatory_count++;
         i++;
      }else if(s=="--dir"){
         dir = argv[i+1];
         i++;
      }else if(s=="--quant"){
         K =  atoi(argv[i+1]);
         obligatory_count++;
         i++;
      }else if(s=="--flip"){
         flip = argv[i+1];
         i++;
      }else if(s=="--resolution"){
         width = atoi(argv[i+1]);
         height = atoi(argv[i+2]);
         i+=2;
      }
   }
   if(obligatory_count!=2){
      cout<<"ERROR: Obligatory input not provided."<<endl;
      return -1;
   }
   //Open txt file
   ofstream MyFile(text_file);  
   Mat myImage;
   //Read image from directory or capture direcly from camera.
   if(dir.length()>0){
      myImage = imread(dir);
   }else{
      if(!CaptureImage(myImage)) return -1;
   }
   PreprocessImage(myImage, K, width, height, flip);
   vector<pair<int, char>> int_char_pairs = {};
   MapChar(myImage, MyFile, int_char_pairs);
   WriteImgToFile(int_char_pairs,myImage,MyFile);

   // Show captured image
   namedWindow("Quantized image");
   imshow("Quantized image", myImage);
   // Wait for any key press to close the image window.
   char c = (char)waitKey();
   MyFile.close();

   return 0;
}