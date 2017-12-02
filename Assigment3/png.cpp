#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <iterator>

using std::vector;
using namespace std;

class png{
public:
  int width;
  int heigth;

};

int main(){
  //variables
  string fileName;
  cout << "Welcome to the PNG processing software!" << '\n';

  while (true){
    std::cout << "Please enter an image name: (inluding .png)" << '\n';
    getline(cin, fileName)
    ifstream image;
    image.open(fileName);
    if (!image){
      std::cout << "Don't try to fool me... This file does not exist!" << '\n';
    }else{
      break;
    }
  }

  while (image) {
    getline(image, cout);
  }

  return 0;
}
