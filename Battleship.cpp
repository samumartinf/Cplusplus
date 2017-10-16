#include <iostream>
#include <cstdio>

using namespace std;

void printBoard(char board[11][11], int rows, int columns, int turn){
  std::cout << "  0 1 2 3 4\t Turn: " << turn <<'\n';
  for (int i = 0; i < rows; i++){
    if (i%2)
      std::cout << i/2;
    else
      std::cout << " ";
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
  int turn = 1;


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
  for (int i = 0; i < 5; i++)
    secretBoard[i][0] = true;
  //Fill in medium ship
  for (int i = 2; i < 5; i++)
    secretBoard[1][i] = true;
  //Fill in the small boat
  for (int i = 2; i < 4; i++)
    secretBoard[4][i] = true;
  //Print the board:
  printBoard(board, rows, columns, turn);

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

    turn++;
    if (secretBoard[attackRow][attackColumn]){
      std::cout << "\nA ship was hit!" << '\n';
      if (board[attackRow*2 + 1][attackColumn*2 + 1] != 'X')
        hitCounter--;
      board[attackRow*2 + 1][attackColumn*2 + 1] = 'X';
    } else {
      std::cout << "You missed..." << '\n';
      board[attackRow*2 + 1][attackColumn*2 + 1] = 'O';
    }
    if (hitCounter == 0){
      playerKeepsPlaying = false;
      std::cout << "YOU WON!" << '\n';
      std::cout << "It took you " << turn << " turns" << '\n';
    }
    if (turn >= 15){
      playerKeepsPlaying = false;
      std::cout << "YOU LOST!" << '\n';
    }

    printBoard(board, rows, columns, turn);
  }
}
