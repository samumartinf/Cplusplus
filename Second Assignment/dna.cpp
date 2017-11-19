#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>
#include <iterator>

using std::vector;
using namespace std;

//Classes
class DNA_DB{
  public:
    string fileName;
    string geneCode;
    int numBasePairs;
    string ref;
    string gid;
    string description;
    int nRegion;
    int cRegion;
    DNA_DB(string fileName);
    string getFileName(){
      return fileName;
    }
    string getHeader(){
      return header;
    }
    void setFileName(string name){
      fileName = name;
    }
};
DNA_DB::DNA_DB(string fileName){ //constructor for the class object
  string rubbish;
  getline(fastaFile, rubbish, '|');
  getline(fastaFile, gid, '|');
  getline(fastaFile, rubbish, '|');
  getline(fastaFile, ref, '|');
  getline(fastaFile, description, '\n');
};


//Functions used
vector<DNA_DB> loadFiles(std::vector<DNA_DB> dna_db);
void firstMenu(vector<DNA_DB> dna_db);
bool secondMenu(vector<DNA_DB> dna_db, int fileNum);
void summarizeSequence(ifstream &fastaFile);
void summarizeSequenceExtended(ifstream &fastaFile);

//Main Function
int main(){
  vector<DNA_DB> dna_db;
  dna_db.clear(); //Clear the vector before use
  dna_db = loadFiles(dna_db);
  cout << dna_db[0].gid  << '\n';
  firstMenu(dna_db);
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
    dna_db.push_back(DNA_DB(tempString)); //initialising the class object
    dna_db[i].setFileName(tempString); //set name for the object
    std::cout << dna_db[i].fileName << '\n';
    i++;
  }
  return dna_db;
}

//Display the menu and return the option chosen
void firstMenu(vector<DNA_DB> dna_db){
  bool stayInMenu = true;
  char answer;
  do{
    cout << "Select one of the following options:" << '\n';
    cout << "  (S) Summary of the statistics of the DNA database" << '\n';
    for (size_t i = 0; i < dna_db.size(); i++) {
      cout << "  (" << i+1 << ") Analyse " << dna_db[i].getFileName() << '\n';
    }
    cout << "  (Q) Quit" << '\n';
    cin >> answer;
    switch (answer) {
      case 'q':
      case 'Q':
        stayInMenu = false;
        break;
      case 's':
      case 'S':{
        //Call function to summarize the data
        ifstream fastaFile;
        for (size_t i = 0; i < dna_db.size(); i++) {
          fastaFile.open(dna_db[i].getFileName());
          summarizeSequence(fastaFile);
          fastaFile.close();
        }
        break;
      }
      default:
        //Loop through the number of databases loaded, if not found, return the user to the  first menu
        int selectedFile = (int)answer - 49; //Convert the character to the integer we expect
        std::cout << "selectedFile = " << selectedFile << '\n';
        if ((selectedFile <= dna_db.size()) && (selectedFile >= 0)){ //
          cout << "jump to second menu using database: " << dna_db[selectedFile].getFileName() <<  '\n'; //this is not being loaded
          stayInMenu = secondMenu(dna_db, selectedFile);
          break;
        }
    }
  } while(stayInMenu);
   //An assumption of an input of only one character is made
}

bool secondMenu(vector<DNA_DB> dna_db, int fileNum){
  //variables
  char answer;
  bool stayInMenu1 = true;
  bool stayInMenu2 = true;
  ifstream fastaFile;

  do {
    fastaFile.open(dna_db[fileNum].getFileName());
    cout << "Select one of the following options:" << '\n';
    cout << "  (H) Help\n  (S) Summary statistics of the DNA sequence" << '\n';
    cout << "  (1) Analyse gap region\n  (2) Analyse code region\n  (3) Analyse base pair range" << '\n';
    cout << "  (4) Find DNA sequence by manual input\n  (5) Find DNA sequence by file input\n  (R) Return to previous menu" << '\n';
    cout << "  (Q) Quit" << '\n';
    cin >> answer;
    switch (answer) {
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
        break;
      case 'R':
      case 'r':
        stayInMenu2 = false;
        break;
      case 'Q':
      case 'q':
        stayInMenu1 = false;
        stayInMenu2 = false;
        break;
      case 'H':
      case 'h':
        //print reference
        cout << "Code Base Description\nG  Guanine\nA Adenine\nT Thymine (Uracil in RNA)\nC Cytosine\nR Purine (A or G)\nY Pyrimidine (C or T or U)\nM Amino (A or C)\nK Ketone (G or T)\nS Strong interaction (C or G)\nW Weak interaction (A or T)\nH Not-G (A or C or T) H follows G in the alphabet\nB Not-A (C or G or T) B follows A in the alphabet\nV Not-T (not-U) (A or C or G) V follows U in the alphabet\nD Not-C (A or G or T) D follows C in the alphabet\nN Any(A or C or G or T)\n\n" << '\n';
        //Maybe implement something to wait for user input before printing again the menu?
        break;
      case 'S':
      case 's':{
        //Send open file to summarizeSequence function
        summarizeSequence(fastaFile);
        summarizeSequenceExtended(fastaFile);
        break;
      }
    }
    fastaFile.close();
  } while(stayInMenu2);
  return stayInMenu1;
}

void summarizeSequence(ifstream &fastaFile){
  string rubbish;
  string description;
  string gid;
  string ref;
  //Retrieve from the header all the relevant information
  getline(fastaFile, rubbish, '|');
  getline(fastaFile, gid, '|');
  getline(fastaFile, rubbish, '|');
  getline(fastaFile, ref, '|');
  getline(fastaFile, description, '\n');
  cout << "Sequence Identifiers:\nDescription: " << description << "\nGID: " << gid << "\nREF: " << ref << "\n\n";
}

void summarizeSequenceExtended(ifstream &fastaFile){
  //variables
  string line;
  string geneCode;
  int numBasePairs = 0;
  int cRegion = 0;
  int nRegion = 0;
  map<char, int> geneInfo; //Map where all the information about the code will be saved
  char baseTypes[15] = {'G','A','C','T','R','Y','M','K','S','W','B','V','D','N','U'};

  //initialize the map with all the possible pairs
  for (auto base : baseTypes){
    geneInfo[base] = 0;
  }
  std::cout << "Please be patient..." << "\n\n";
  while(getline(fastaFile, line)){ //itinerate through the whole file and append to geneCode
    geneCode.append(line);
  }
  for (size_t i = 0; i < geneCode.size(); i++) {
      geneInfo[geneCode[i]]++; //Count base pairs
      if (geneCode[i] == 'N' && geneCode[i+1] != 'N') {
        nRegion++;
      }
  }
  if (geneCode[geneCode.size()-1] == 'N')
    cRegion = nRegion - 1;
  else
    cRegion = nRegion + 1;
  cout << "# of Regions: " << cRegion + nRegion << '\n'<< "# N regions: " << nRegion << '\n' << "# C regions: " << cRegion << "\n\n";


  cout << "Base pair characteristics:" << '\n';
  for(auto elem : geneInfo){ //A first loop to sum all the base pairs
    numBasePairs = numBasePairs + elem.second;

  }
  cout << "# of base pairs: " << numBasePairs << '\n';
  for(auto elem : geneInfo){ //Itinerate through the elements of geneInfo to print out the ones we are interested in
    if (find(std::begin(baseTypes), end(baseTypes), elem.first) != end(baseTypes)) //find returns an iterator to the first occurrence of elem.first, or an iterator to one-past the end of the range if it is not found.
    {
      cout << elem.first << ": " << elem.second << "\n";
    } else{
      geneInfo['U'] = geneInfo['U'] + elem.second; //Add to Unknown all the values that don't belong to the standard
    }
  }
  cout << "Unknown: " << geneInfo['U'] <<"\n\n";
}

void optionOneSelected(ifstream &fastaFile) {
  string rubbish;
  string line;
  string geneCode;
  int startBP;
  int endBP;
  int gapRegionCurrent;
  int gapRegionSearching;
  getline(fastaFile, rubbish, '\n');
  while(getline(fastaFile, line)){ //itinerate through the whole file and append to geneCode
    geneCode.append(line);
  }
  for (size_t i = 0; i < geneCode.size(); i++) {
      if (geneCode[i] == 'N') {
        if (geneCode[i-1] != 'N') {
          gapRegionCurrent++;
          if (gapRegionCurrent == gapRegionSearching) {
            startBP = i;
            //gapRegionCurrent++; //This is only in to make sure that startBP does not change when we have the number we want
            cout << geneCode[i] << '\n';
          }
        }
        if (gapRegionCurrent == gapRegionSearching) {
          /* code */
        }
      }
  }
}
