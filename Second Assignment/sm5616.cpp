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
    int nRegion = 0;
    int cRegion = 0;
    int numBasePairs = 0;
    vector<pair<int, int>> nRegions; //vector containing the pair of start and end index of the non coding regions
    vector<pair<int, int>> cRegions; //vector containing the pair of start and end index of the coding regions
    map<char, int> geneInfo; //Map where the base pair information will be saved
    char baseTypes[15] = {'G','A','C','T','R','Y','M','K','S','W','B','V','D','N','U'};
    DNA_DB(string fileName); //Initialiser for the class
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
  //As there is no previous base pair to compare it with, the first base pair get counted separatedly
  if (geneCode[0] == 'N'){
    nRegion++;
    nRegions.push_back(make_pair(0, 0));
  }
  else{
    cRegion++;
    cRegions.push_back(make_pair(0, 0));
  }
  //Itinerate over the whole geneCode to count the number of basepairs, coding and noncoding regions, also saving the start of those regions
  for (size_t i = 0; i < geneCode.size(); i++){
    geneInfo[geneCode[i]]++; //count baseTypes
    if (i > 0 && geneCode[i] == 'N' && geneCode[i-1] != 'N') { //Start of a noncoding regions
      nRegion++;
      nRegions.push_back(make_pair(i, 0)); //Save the value for the begining of the region
    }
    if (i > 0 && geneCode[i] != 'N' && geneCode[i-1] == 'N') { //Start of a coding region
      cRegion++;
      cRegions.push_back(make_pair(i, 0)); //Save the value for the beggning of the region
    }
  }
  //Go again over the whole gene code to add the end of the coding and non coding regions
  int countN = 0;
  int countC = 0;
  for (size_t i = 0; i < geneCode.size(); i++){
    if (i > 0 && geneCode[i] == 'N' && geneCode[i-1] != 'N') {
      cRegions[countC].second = i-1;
      countC++;
    }
    if (i > 0 && geneCode[i] != 'N' && geneCode[i-1] == 'N') {
      nRegions[countN].second = i-1;
      countN++;
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
void analyseGivenRegion(DNA_DB dataBase);
void findByInput(DNA_DB dataBase, string regionToFind);

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

  //Cutting the string into separated names
  string tempString;
  bool validFiles; //Check that the file names exist
  do {
    validFiles = true;
    getline(cin,fileNames,'\n'); //Get the whole input from the user and save it as fileNames
    istringstream ss(fileNames);
    //Set the value of tempString to the characters until the next coma, check and remove if there is an empty espace between the coma and the name.
    while(getline(ss, tempString, ',')) {
      //get rid of spaces
      if (tempString[0] == ' '){
        tempString = tempString.substr(1, tempString.size()-1);
      }
      ifstream file(tempString);
      if (!file) {
        cout << "Please try again with valid files: " << '\n';
        validFiles = false;
        break;
      } else {
      dna_db.push_back(DNA_DB(tempString)); //initialising a new class object per file
      }
    }
  } while(validFiles == false);

  return dna_db;
}

void firstMenu(vector<DNA_DB> dna_db){
  //variables
  bool stayInMenu = true;
  string input;
  char answer = {0};

  //Stay in first menu while true
  do{
    cout << "Select one of the following options:" << '\n';
    cout << "  (S) Summary of the statistics of the DNA database" << '\n';
    for (size_t i = 0; i < dna_db.size(); i++) {
      cout << "  (" << i+1 << ") Analyse " << dna_db[i].fileName << '\n';
    }
    cout << "  (Q) Quit" << '\n';
    //Check for a valid one character input (Avoid using ">>" as it is considered bad practice to get input, creates multiple errors when combined with getline)
    while (true) {
      getline(cin, input);

      if (input.length() == 1) {
        answer = input[0];
        break;
      }

      cout << "Invalid input, please enter only one character" << endl;
    }
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
        if ((selectedFile <= dna_db.size()) && (selectedFile >= 0)){ //Check that the number introduces is withing the range
          cout << "You are using database: " << dna_db[selectedFile].getFileName() <<  '\n'; //tell the user which database is being used
          stayInMenu = secondMenu(dna_db[selectedFile]);
          break;
        }
    }
  } while(stayInMenu);
}

bool secondMenu(DNA_DB dataBase){
  //variables
  char answer;
  string input;
  string fileN;
  bool validFile;
  bool stayInMenu1 = true;
  bool stayInMenu2 = true;
  //Stay in menu while true (same as in the first menu)
  do {
    cout << "Select one of the following options:" << '\n';
    cout << "  (H) Help\n  (S) Summary statistics of the DNA sequence" << '\n';
    cout << "  (1) Analyse gap region\n  (2) Analyse code region\n  (3) Analyse base pair range" << '\n';
    cout << "  (4) Find DNA sequence by manual input\n  (5) Find DNA sequence by file input\n  (R) Return to previous menu" << '\n';
    cout << "  (Q) Quit" << '\n';

    while (true) { //Here again check for a proper one character input
      getline(cin, input);
      if (input.length() == 1) {
        answer = input[0];
        break;
      }
      cout << "Invalid input, please enter only one character" << endl;
    }

    switch (answer) {
      case '1':
        analyseGapRegion(dataBase);
        break;
      case '2':
        analyseCodingRegion(dataBase);
        break;
      case '3':
        analyseGivenRegion(dataBase);
        break;
      case '4':{
        cout << "Specify the nucleotides you would like to find: " << '\n';
        string nucleotides;
        getline(cin, nucleotides);
        findByInput(dataBase, nucleotides);
        break;
      }
      case '5':{
        cout << "Specify the DNA sequence file you would like to find: " << '\n';
        getline(cin, fileN);
        vector<DNA_DB> fileVect;
        ifstream file(fileN);
        do {
          validFile = true;
          ifstream file(fileN);
          if (!file) {
            cout << "Please try again with a valid file: " << '\n';
            getline(cin, fileN);
            validFile = false;
          }
        } while(validFile);
        fileVect.push_back(DNA_DB(fileN));
        findByInput(dataBase, fileVect[fileVect.size() -1].geneCode);
        break;
      }
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
  cout << "# of Regions: " << dataBase.cRegion + dataBase.nRegion << '\n'<< "# N regions: " << dataBase.nRegion << '\n' << "# C regions: " << dataBase.cRegion << "\n\n";

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
  string regionStr;

  if (dataBase.nRegion == 0){
    cout << "There are no gap regions" << '\n';
  } else {
    do {
      while (true) {
        cout << "Enter gap region number: " << '\n';
        getline(cin, regionStr);
        // This code converts from string to number safely.
       stringstream myStream(regionStr);
       if (myStream >> selectedRegion)
         break;
       cout << "Invalid number, please try again" << endl;
     }
    } while(selectedRegion > dataBase.nRegion || selectedRegion < 1); //Chech that the input is valid
    cout << "Selected sequence: \nBase pair range: (" << dataBase.nRegions[selectedRegion-1].first << ", " << dataBase.nRegions[selectedRegion-1].second << ")\nSequence:\n";
    for (size_t i = dataBase.nRegions[selectedRegion-1].first; i < dataBase.nRegions[selectedRegion-1].second; i++) {
      cout << dataBase.geneCode[i];
    }
  }

  cout << '\n';
}

void analyseCodingRegion(DNA_DB dataBase){
  //variable
  int selectedRegion;
  string regionStr;
  if (dataBase.cRegion == 0) {
    cout << "Sorry, there are no coding regions" << '\n';
  } else {
    do {
      while (true) {
        cout << "Enter coding region number: " << '\n';
        getline(cin, regionStr);
        // This code converts from string to number safely.
       stringstream myStream(regionStr);
       if (myStream >> selectedRegion){
         break;
       }
       cout << "Invalid number, please try again" << endl;
     }
   } while(selectedRegion > dataBase.cRegion || selectedRegion < 1); //Make sure that the region entered is valid and that so is the input
    cout << "Selected sequence: \nBase pair range: (" << dataBase.cRegions[selectedRegion-1].first << ", " << dataBase.cRegions[selectedRegion-1].second << ")\nSequence:\n";

    //Just print the whole region
    for (size_t i = dataBase.cRegions[selectedRegion-1].first; i < dataBase.cRegions[selectedRegion-1].second; i++) {
      cout << dataBase.geneCode[i];
    }
  }
  cout << '\n';
}

void analyseGivenRegion(DNA_DB dataBase){
  //variables
  string startStr;
  string endStr;
  string range;
  int start;
  int end;

  cout << "Enter region to analyse: " << '\n';

  //Get the range and convert the string values to int
  getline(cin,range,'\n'); //Get the whole input from the user and save it as range

  //Cutting the string into separated values
  istringstream ssRange(range);
  getline(ssRange, startStr, ',');
  getline(ssRange, endStr);
  stringstream ssStart(startStr);
  stringstream ssEnd(endStr);
  //Set the value of tempString to the characters until the next coma, check and remove if there is an empty espace between the coma and the name.
  ssStart >> start;
  ssEnd >> end;
  for (size_t i = start; i < end ; i++) {
    cout << dataBase.geneCode[i];
  }
  cout << '\n';
}

void findByInput(DNA_DB dataBase, string regionToFind) {
  string code = dataBase.geneCode;
  int count = 1;
  size_t found;
  do {
    found = code.find(regionToFind);
    if (found != string::npos) {
      cout <<  count << ". Base pair range: (" << found << ", " << found + regionToFind.size() << ")\n";
      //Print the previous 20 base pairs if possible
      if (found > 20) {
        for (size_t i = found-20; i < found; i++) {
          cout << code[i];
        }
      }
      cout << '\n';

      for (size_t i = found; i < found + regionToFind.size(); i++) {
        cout << code[i];
      }
      cout << '\n';
      //Print the next 20 base pairs if possible
      if (code.size() - (found + regionToFind.size() > 20)) {
        for (size_t i = found + regionToFind.size(); i < found + regionToFind.size() + 20; i++) {
          cout << code[i];
        }
      }
      cout << '\n';
      count++;
      code = code.substr(found + regionToFind.size()); //Remove from the main string in which we look for all the region that has been searched for already
    }
  } while (found != string::npos);
}
