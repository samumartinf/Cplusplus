#include <iostream>
#include <sstream>
#include <vector>

using std::vector;
using namespace std;

//Classes
class DNA_DB{
  public:
    string fileName;
    DNA_DB();
    string getFileName(){
      return fileName;
    }
    void setFileName(string name){
      fileName = name;
    }
};
DNA_DB::DNA_DB(){ //constructor for the class object
  return;
};


//Functions used
std::vector<DNA_DB> loadFiles(std::vector<DNA_DB> dna_db);
char firstMenu(vector<DNA_DB> dna_db);

//Main Function
int main(){
  vector<DNA_DB> dna_db;
  dna_db.clear(); //Clear the vector before use
  dna_db = loadFiles(dna_db);
  char firstMenuAnswer = firstMenu(dna_db);
  char num;
  std::cout << firstMenuAnswer << '\n';
  switch (firstMenuAnswer) {
    case 'q':
    case 'Q':
      break;
    case 's':
    case 'S':
      //Function to summarize the data
      std::cout << "summarize selected" << '\n'; //Debugging purposes
    default:
      //Loop through the number of databases loaded, if not found, return the user to the  first menu
      for (int i = 1; i <= dna_db.size(); i++) {
        num = i;
        if (firstMenuAnswer == num){
          //Jump to the second menu
          std::cout << "jump to second menu using database: " << dna_db[i].getFileName() <<  '\n'; //this is not being loaded
          break;
        }
      }

  }
  return 0;
}

//Load files function: fills the vector<DNA_DB> with the names of the files
vector<DNA_DB> loadFiles(vector<DNA_DB> dna_db){
  string fileNames; //string to save the string with coma separated input
  cout << "DNA Sequence Database Software\nSpecify the name of DNA sequence file names you would like to load. For multiple files, add a \",\" between each file name. \n>";
  getline(cin,fileNames,'\n') ; //Get the whole input from the user and save it as fileNames
  //cutting the string into separated names
  istringstream ssFileNames(fileNames); //Creating a istringstream for the cutting of the sring
  string tempString;
  int i = 0; //initialize counter
  while(getline(ssFileNames, tempString, ',')) {
    if (tempString[0] == ' '){ //Remove the first space (if it exists) between the coma and the file name
      tempString = tempString.substr(1, tempString.size()-1);
    }
    dna_db.push_back(DNA_DB()); //initialising the class object
    dna_db[i].setFileName(tempString); //set name for the object
    std::cout << dna_db[i].fileName << '\n';
    i++;
  }
  return dna_db;
}

//Display the menu and return the option chosen
char firstMenu(vector<DNA_DB> dna_db){
  int i = 0;
  cout << "Select one of the following options:" << '\n';
  cout << "  (S)  Summary of the statistics of the DNA database" << '\n';
  while(i < dna_db.size()){
    std::cout << "  (" << i+1 << ")  Analyse " << dna_db[i].getFileName() << '\n'; // Currently not printing the name
    i++;
  }
  std::cout << "  (Q)  Quit" << '\n';
  char answer;
  std::cin >> answer; //An assumption of an input of only one character is made
  return answer;
}
