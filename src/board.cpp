#include "headers/board.h"

Board::Board()
{
  fromFEN(STARTING_FEN);
}

Board::Board(std::string fen)
{
  fromFEN(fen);
}

int Board::fromFEN(std::string fen)
{
  using namespace std;
  cout << "Parsing FEN: " << fen << endl;

  int current_x = 0;
  int current_y = 0;
  bool piece_placement = true;

  for (int i = 0; i < fen.length(); i++)
  {
    char current_char = fen.at(i);
    if (piece_placement)
    {
      if (current_char == '/')
      {
        current_x++;
        current_y = 0;
        continue;
      }
      else if (current_char >= '0' && current_char <= '9')
      {
        for (int j = 0; j < current_char - '0'; j++)
        {
          pieces[current_x][current_y] = NULL;
          current_y++;
        }
      }
      else if (current_char == ' ')
      {
        piece_placement = false;
      }
      else
      {
        pieces[current_x][current_y] = new Piece(current_x, current_y, current_char);
        current_y++;
      }
    }
  }

  return 0;
}

void Board::display()
{
  using namespace std;
  cout << "\n\nBoard:\n\n";

  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (pieces[i][j])
      {
        cout << pieces[i][j]->getSymbol();
      }
      else
      {
        cout << (getBoardColorAt(i, j) ? " " : "#");
      }
    }
    cout << "\n";
  }

  cout << flush;
  // std::flush(std::cout);
}

bool Board::getBoardColorAt(int x, int y)
{
  if (x % 2 == 0)
  {
    return !(y % 2);
  }
  else
  {
    return y % 2;
  }
}