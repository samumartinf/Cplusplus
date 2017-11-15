#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <vector>

using std::vector;
using namespace std;

int main(){
  std::string fileNames;
  cout << "DNA Sequence Database Software\nSpecify the name of DNA sequence file names you would like to load. For multiple files, add a \",\" between each file name. \n>";
  getline(cin, fileNames);
  // Variable for the loop
  std::vector<char[]> fileNamesVector;
  int startOfFileName = 0;
  int numberOfFiles = 0;
  for(int i = 0; i < fileNames.length(); i++) {
    if (fileNames[i] == "," || fileNames[i] = "\n"){
      fileNamesVector[numberOfFiles] = fileNames.substr(startOfFileName, i-1);
      numberOfFiles++;
      if (fileNames[i+1] != " ") /* Here we asume that the input will be separated by comas and no space
      or by coma and a space (i.e. "file01, file02") but there will not be more than one space */
        startOfFileName = i+1;
      else
        startOfFileName = i+2;
    }
  }
  for (i = 0; i < numberOfFiles; i++){
    print(fileNamesVector[i]);
  }
}
