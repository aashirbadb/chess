#include "headers/board.h"

// Uppercase represents white pieces
bool isUppercase(char ch)
{
  return (ch >= 'A' && ch <= 'Z');
}

Board::Board()
{
  fromFEN(STARTING_FEN);
}

Board::Board(std::string fen)
{
  fromFEN(fen);
}

// https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
int Board::fromFEN(std::string fen)
{
  using namespace std;
  cout << "Parsing FEN: " << fen << endl;

  int current_x = 0;
  int current_y = 0;
  bool piece_placement = true;

  for (int i = 0; i < fen.length(); i++)
  {
    char current_char = fen[i];
    if (piece_placement)
    {
      // '/' represents a new row
      if (current_char == '/')
      {
        current_x++;
        current_y = 0;
        continue;
      }
      else if (current_char >= '0' && current_char <= '9')
      {
        // if there are digits, skip that number of digits
        for (int j = 0; j < current_char - '0'; j++)
        {
          pieces[current_x][current_y] = nullptr;
          current_y++;
        }
      }
      else if (current_char == ' ') // space represents end of piece placement
      {
        piece_placement = false;
      }
      else // create proper pieces and place in board
      {
        // Uppercase pieces are white and lowercase pieces are black
        switch (current_char)
        {
        case 'p':
        case 'P':
          pieces[current_x][current_y] = new Pawn({current_x, current_y}, isUppercase(current_char));
          break;

        case 'r':
        case 'R':
          // TODO:Arpit
          break;

        case 'n':
        case 'N':
          // TODO: Arpit
          break;

        case 'b':
        case 'B':
          // TODO: Arpit
          break;

        case 'q':
        case 'Q':
          // TODO: Bhuwan
          break;

        case 'k':
        case 'K':
          // TODO: Bhuwan
          break;

        default:
          // TODO: Should never happen. throw error here
          pieces[current_x][current_y] = new Piece({current_x, current_y}, isUppercase(current_char));
          break;
        }
        current_y++;
      }
    }
  }

  return 0;
}

void Board::display()
{
  using namespace std;
  cout << "\n\nBoard:\n";

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
        // white = <space>, black = #
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