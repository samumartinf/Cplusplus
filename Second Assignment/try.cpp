#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>
#include <iterator>
#include <utility>

using std::vector;
using namespace std;

//Classes
class DNA_DB{
  public:
    string fileName;
    string geneCode; //The string with all base pairs from the file
    string ref;
    string gid;
    string description;
    int countNRegions = 0;
    int countCRegions = 0;
    int numBasePairs = 0;
    DNA_DB(string fileName); //Initialiser for the class
    pair<int, int> nRegions[5000] = {};
    pair<int, int> cRegions[5000] = {};
    map<char, int> geneInfo; //Map where the base pair information will be saved
    char baseTypes[15] = {'G','A','C','T','R','Y','M','K','S','W','B','V','D','N','U'};
    void setFileName(string name){
      fileName = name;
    }
    string getFileName(){
      return fileName;
    }
};
DNA_DB::DNA_DB(string fileName){ //constructor for the class object
  //Set the fileNames
  setFileName(fileName);
  //opening of the file
  ifstream fastaFile;
  fastaFile.open(fileName);

  //handle the header
  string rubbish;
  string line;
  getline(fastaFile, rubbish, '|');
  getline(fastaFile, gid, '|');
  getline(fastaFile, rubbish, '|');
  getline(fastaFile, ref, '|');
  getline(fastaFile, description, '\n');

  //Handle the base pair regions of the file
  for (auto base : baseTypes){
    geneInfo[base] = 0; //setting to cero the counter of base types.
  }
  cout << "Please be patient..." << "\n\n";
  while(getline(fastaFile, line)){ //itinerate through the whole file and append to geneCode
    geneCode.append(line);
  }
  for (size_t i = 0; i < geneCode.size(); i++) {
    geneInfo[geneCode[i]]++; //count baseTypes
    if (geneCode[0] == 'N')
      countNRegions++;
    else
      countCRegions++;
    if (i > 0 && geneCode[i] == 'N' && geneCode[i-1] != 'N') {
      countNRegions++;
    }
    if (i > 0 && geneCode[i] != 'N' && geneCode[i-1] == 'N') {
      countCRegions++;
    }
  }

  int countCoding = 0;
  int countNonCoding = 0;
  for (size_t i = 0; i < geneCode.size(); i++) {
    if (geneCode[0] == 'N')
      countNonCoding++;
    else
      countCoding++;
    if (i > 0 && geneCode[i] == 'N' && geneCode[i-1] != 'N') {
      countNonCoding++;
      nRegions[countNonCoding] = make_pair(i, 0);
      cRegions[countCoding-1].second = i-1;
    }
    if (i > 0 && geneCode[i] != 'N' && geneCode[i-1] == 'N') {
      countCoding++;
      cRegions[countCoding] = make_pair(i, 0);
      nRegions[countNonCoding-1].second = i-1;
    }
  }
  for(auto elem : geneInfo){ //A first loop to sum all the base pairs
    numBasePairs = numBasePairs + elem.second;
  }
};

//Functions used
vector<DNA_DB> loadFiles(std::vector<DNA_DB> dna_db);
void firstMenu(vector<DNA_DB> dna_db);
bool secondMenu(DNA_DB dataBase);
void summarizeSequence(DNA_DB dataBase);
void summarizeSequenceExtended(DNA_DB dataBase);
void analyseGapRegion(DNA_DB dataBase);
void analyseCodingRegion(DNA_DB dataBase);

//Main Function
int main(){
  vector<DNA_DB> dna_db;
  dna_db.clear(); //Clear the vector before use
  dna_db = loadFiles(dna_db);
  firstMenu(dna_db);
  return 0;
}

//Load files function: fills the vector<DNA_DB> with the files
vector<DNA_DB> loadFiles(vector<DNA_DB> dna_db){
  string fileNames; //String to save the string with coma separated input
  cout << "DNA Sequence Database Software\nSpecify the name of DNA sequence file names you would like to load. For multiple files, add a \",\" between each file name. \n>";
  getline(cin,fileNames,'\n') ; //Get the whole input from the user and save it as fileNames

  //Cutting the string into separated names
  istringstream ssFileNames(fileNames);
  string tempString;
  //Set the value of tempString to the characters until the next coma, check and remove if there is an empty espace between the coma and the name.
  while(getline(ssFileNames, tempString, ',')) {
    if (tempString[0] == ' '){
      tempString = tempString.substr(1, tempString.size()-1);
    }
    dna_db.push_back(DNA_DB(tempString)); //initialising a new class object per file
  }
  return dna_db;
}

void firstMenu(vector<DNA_DB> dna_db){
  bool stayInMenu = true;
  char answer;
  do{
    cout << "Select one of the following options:" << '\n';
    cout << "  (S) Summary of the statistics of the DNA database" << '\n';
    for (size_t i = 0; i < dna_db.size(); i++) {
      cout << "  (" << i+1 << ") Analyse " << dna_db[i].fileName << '\n';
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
          summarizeSequence(dna_db[i]);
          fastaFile.close();
        }
        break;
      }
      default:
        //Loop through the number of databases loaded, if not found, return the user to the  first menu
        int selectedFile = (int)answer - 49; //Convert the character to the integer we expect
        cout << "selectedFile = " << selectedFile << '\n';
        if ((selectedFile <= dna_db.size()) && (selectedFile >= 0)){ //
          cout << "jump to second menu using database: " << dna_db[selectedFile].getFileName() <<  '\n'; //this is not being loaded
          stayInMenu = secondMenu(dna_db[selectedFile]);
          break;
        }
    }
  } while(stayInMenu);
   //An assumption of an input of only one character is made
}

bool secondMenu(DNA_DB dataBase){
  //variables
  char answer;
  bool stayInMenu1 = true;
  bool stayInMenu2 = true;

  do {
    cout << "Select one of the following options:" << '\n';
    cout << "  (H) Help\n  (S) Summary statistics of the DNA sequence" << '\n';
    cout << "  (1) Analyse gap region\n  (2) Analyse code region\n  (3) Analyse base pair range" << '\n';
    cout << "  (4) Find DNA sequence by manual input\n  (5) Find DNA sequence by file input\n  (R) Return to previous menu" << '\n';
    cout << "  (Q) Quit" << '\n';
    cin >> answer;
    switch (answer) {
      case '1':
        analyseGapRegion(dataBase);
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
        break;
      case 'S':
      case 's':{
        //Send open file to summarizeSequence function
        summarizeSequence(dataBase);
        summarizeSequenceExtended(dataBase);
        break;
      }
    }
  } while(stayInMenu2);
  return stayInMenu1;
}

void summarizeSequence(DNA_DB dataBase){
  cout << "Sequence Identifiers:\nDescription: " << dataBase.description << "\nGID: " << dataBase.gid << "\nREF: " << dataBase.ref << "\n\n";
}

void summarizeSequenceExtended(DNA_DB dataBase){
  //Regions characteristics
  cout << "# of Regions: " << dataBase.countCRegions + dataBase.countNRegions << '\n'<< "# N regions: " << dataBase.countNRegions << '\n' << "# C regions: " << dataBase.countCRegions << "\n\n";

  //Base pair characteristics
  cout << "Base pair characteristics:" << '\n';
  cout << "# of base pairs: " << dataBase.numBasePairs << '\n';
  for(auto elem : dataBase.geneInfo){ //Itinerate through the elements of geneInfo to print out the ones we are interested in
    if (find(std::begin(dataBase.baseTypes), end(dataBase.baseTypes), elem.first) != end(dataBase.baseTypes)) //find returns an iterator to the first occurrence of elem.first, or an iterator to one-past the end of the range if it is not found.
    {
      cout << elem.first << ": " << elem.second << "\n";
    } else{
      dataBase.geneInfo['U'] = dataBase.geneInfo['U'] + elem.second; //Add to Unknown all the values that don't belong to the standard of basepairs
    }
  }
  cout << "Unknown: " << dataBase.geneInfo['U'] <<"\n\n";
}

void analyseGapRegion(DNA_DB dataBase){
  //variable
  int selectedRegion;
  cout << "Enter gap region number: " << '\n';
  cin >>  selectedRegion;
  cout << "Selected sequence: \nBase pair range: (" << dataBase.nRegions[selectedRegion-1].first << ", " << dataBase.nRegions[selectedRegion-1].second << ")\nSequence:\n";
  for (size_t i = dataBase.nRegions[selectedRegion-1].first; i < dataBase.nRegions[selectedRegion-1].first; i++) {
    cout << dataBase.geneCode[i] << '\n';
  }
}

void analyseCodingRegion(DNA_DB dataBase){
  //variable
  int selectedRegion;
  cout << "Enter coding region number: " << '\n';
  cin >>  selectedRegion;
  cout << "Selected sequence: \nBase pair range: (" << dataBase.cRegions[selectedRegion-1].first << ", " << dataBase.cRegions[selectedRegion-1].second << ")\nSequence:\n";
  for (size_t i = dataBase.cRegions[selectedRegion-1].first; i < dataBase.cRegions[selectedRegion-1].first; i++) {
    cout << dataBase.geneCode[i] << '\n';
  }
}
