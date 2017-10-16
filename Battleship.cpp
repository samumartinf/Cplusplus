#include <iostream>
#include <cstdio>

using namespace std;

void printBoard(char board[11][11], int rows, int columns){
  for (int i = 0; i < rows; i++){
    for (int j = 0; j < columns; j++)
      cout << board[i][j];
    cout << '\n';
  }
}
int main(){
  int rows = 11;
  int columns = 11;
  int secretRows = rows/2;
  int secretColumns = rows/2;


  //Create the board
  char board[11][11];
  //Itinerate over the columns
  for (int i = 0; i < columns; i++){
    //Populate the even rows with dashes
    for (int j = 0; j < rows; j = j + 2){
      board[j][i] = '-';
    }
    //Populate the odd rows with "|" and spaces
    for (int j = 1; j < rows; j = j + 2){
      if (i%2 == 1){
        board[j][i] = ' ';
      } else {
        board[j][i] = '|';
      }
    }
  }

  //Create Secret array
  bool secretBoard[secretRows][secretColumns];
  for (int r = 0; r < 5; r++){
    for (int c = 0; c < 5; c++){
      secretBoard[r][c] = false;
    }
  }
  //Fill in the Big ship
  for (int i = 1; i < 5; i++)
    secretBoard[1][i] = true;
  //Fill in medium ship
  for (int i = 0; i < 3; i++)
    secretBoard[4][i] = true;
  //Fill in the small boat
  for (int i = 3; i < 5; i++)
    secretBoard[i][4] = true;
  //Print the board:
  printBoard(board, rows, columns);

  bool playerKeepsPlaying = true;
  int attackRow = 0;
  int attackColumn = 0;
  int hitCounter = 9;

  while (playerKeepsPlaying) {
    do {
      std::cout << "Enter a row: ";
      std::cin >> attackRow;
      std::cout << "\nEnter a column: ";
      std::cin >> attackColumn;
    } while(attackRow <  0 || attackRow >  4 || attackColumn <  0 || attackColumn > 4);

    if (secretBoard[attackRow][attackColumn]){
      std::cout << "\nA ship was hit!" << '\n';
      if (board[attackRow*2 + 1][attackColumn*2 + 1] != 'S')
        hitCounter--;
      board[attackRow*2 + 1][attackColumn*2 + 1] = 'S';
    } else {
      std::cout << "You missed..." << '\n';
      board[attackRow*2 + 1][attackColumn*2 + 1] = 'O';
    }
    if (hitCounter == 0){
      playerKeepsPlaying = false;
      std::cout << "YOU WON!" << '\n';
    }

    printBoard(board, rows, columns);
  }
}
