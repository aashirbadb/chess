#include "headers/board.h"
#include <iomanip>

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

Board::Board(Board &_board)
{
  // TODO: Use copy constructor for pieces also
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      pieces[i][j] = _board.pieces[i][j];
    }
  }

  for (int i = 0; i < 4; i++)
  {
    canCastle[i] = _board.canCastle[i];
  }

  isWhiteTurn = _board.isWhiteTurn;
  enPassantTarget = _board.enPassantTarget;
  halfMoveClock = _board.halfMoveClock;
  fullMoveClock = _board.fullMoveClock;
}

Board::~Board()
{
}

// https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
int Board::fromFEN(std::string fen)
{
  using namespace std;
  cout << "Parsing FEN: " << fen << endl;

  // Clear the board
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      pieces[i][j] = nullptr;
    }
  }

  int current_x = 0;
  int current_y = 0;
  int index = 0;

  // Parse position of pieces
  while (fen[index] != ' ')
  {
    char current_char = fen[index];
    index++;
    if (current_char == '/')
    {
      current_x++;
      current_y = 0;
      continue;
    }
    else if (isdigit(current_char))
    {
      current_y += current_char - '0';
    }
    else
    {
      switch (current_char)
      {
      case 'p':
      case 'P':
        pieces[current_x][current_y] = new Pawn({current_x, current_y}, isUppercase(current_char));
        break;

      case 'r':
      case 'R':
        pieces[current_x][current_y] = new Rook({current_x, current_y}, isUppercase(current_char));
        break;

      case 'n':
      case 'N':
        pieces[current_x][current_y] = new Knight({current_x, current_y}, isUppercase(current_char));
        break;

      case 'b':
      case 'B':
        pieces[current_x][current_y] = new Bishop({current_x, current_y}, isUppercase(current_char));
        break;

      case 'q':
      case 'Q':
        pieces[current_x][current_y] = new Queen({current_x, current_y}, isUppercase(current_char));
        break;

      case 'k':
      case 'K':
        pieces[current_x][current_y] = new King({current_x, current_y}, isUppercase(current_char));
        break;

      default:
        throw "Invalid piece in FEN";
      }
      current_y++;
    }
  }
  index++; // Skip space

  // Current move
  isWhiteTurn = (fen[index++] == 'w');
  index++;

  // Castling availability
  if (fen[index] == '-')
  {
    // All castling things disabled
    canCastle[0] = false;
    canCastle[1] = false;
    canCastle[2] = false;
    canCastle[3] = false;
    index++;
  }
  else
  {
    while (fen[index] != ' ')
    {
      switch (fen[index])
      {
      case 'K':
        canCastle[0] = true;
        break;
      case 'Q':
        canCastle[1] = true;
        break;
      case 'k':
        canCastle[2] = true;
        break;
      case 'q':
        canCastle[3] = true;
        break;
      }
      index++;
    }
  }
  index++;

  // En passant target square
  if (fen[index] == '-')
  {
    enPassantTarget = {-1, -1};
    index++;
  }
  else
  {
    enPassantTarget = {fen[index + 1] - '1', fen[index] - 'a'};
    index += 2;
  }
  index++;

  // Halfmove clock
  halfMoveClock = fen[index++] - '0';
  index++;

  // Fullmove clock
  fullMoveClock = fen[index++] - '0';

  cout << "Successfully parsed FEN" << endl;

  return 0;
}

void Board::display()
{
  using namespace std;

  for (int i = 0; i < 8; i++)
  {
    cout << 8 - i << " |";
    for (int j = 0; j < 8; j++)
    {
      if (pieces[i][j])
      {
        cout << pieces[i][j]->getSymbol();
        cout << '|';
      }
      else
      {
        // white = <space>, black = #
        cout << (getBoardColorAt(i, j) ? " |" : "#|");
      }
    }
    cout << endl;
  }

  cout << "   ";
  for (int i = 0; i < 8; i++)
  {
    cout << (char)('a' + i) << " ";
  }
  cout << endl
       << endl;

  cout << left << setw(25) << "Current turn" << (isWhiteTurn ? "White" : "Black") << endl;
  cout << left << setw(25) << "Castling availability";
  if (canCastle[0])
  {
    cout << "K";
  }
  if (canCastle[1])
  {
    cout << "Q";
  }
  if (canCastle[3])
  {
    cout << "k";
  }
  if (canCastle[4])
  {
    cout << "q";
  }
  cout << endl;

  cout << left << setw(25) << "En passant target" << (enPassantTarget.isValidPosition() ? enPassantTarget.getChessCoordinate() : "-") << endl;
  cout << left << setw(25) << "Halfmove clock" << halfMoveClock << endl;
  cout << left << setw(25) << "Fullmove clock" << fullMoveClock << endl;
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

Piece *Board::getPieceAt(Coordinate _coord)
{
  return pieces[_coord.x][_coord.y];
}

bool Board::isWhiteInCheck()
{
  Piece *whiteKing = nullptr;
  std::vector<Piece *> blackPieces;
  bool whiteInCheck = false;
  bool blackInCheck = false;

  // Search for white king
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      Piece *currentPiece = pieces[i][j];
      if (currentPiece == nullptr)
        continue;
      if (currentPiece->getSymbol() == 'K')
      {
        whiteKing = pieces[i][j];
      }

      if (islower(currentPiece->getSymbol()))
      {
        blackPieces.push_back(currentPiece);
      }
    }
  }

  for (int i = 0; i < blackPieces.size(); i++)
  {
    std::vector<Move> moves = blackPieces[i]->getAllMoves(*this);
    for (int j = 0; j < moves.size(); j++)
    {
      if (moves[j].end.x == whiteKing->getPosition().x && moves[j].end.y == whiteKing->getPosition().y)
      {
        whiteInCheck = true;
        break;
      }
    }
  }

  return whiteInCheck;
}

bool Board::isBlackInCheck()
{
  using namespace std; // TODO:

  Piece *blackKing = nullptr;
  std::vector<Piece *> whitePieces;
  bool blackInCheck = false;

  // Search for black king and white pieces
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      Piece *currentPiece = pieces[i][j];
      if (currentPiece == nullptr)
        continue;

      if (currentPiece->getSymbol() == 'k')
      {
        blackKing = pieces[i][j];
      }

      // Other pieces
      if (isupper(currentPiece->getSymbol()))
      {
        whitePieces.push_back(currentPiece);
      }
    }
  }

  for (int i = 0; i < whitePieces.size(); i++)
  {
    std::vector<Move> moves = whitePieces[i]->getAllMoves(*this);
    for (int j = 0; j < moves.size(); j++)
    {
      if (moves[j].end.x == blackKing->getPosition().x && moves[j].end.y == blackKing->getPosition().y)
      {
        blackInCheck = true;
        break;
      }
    }
  }

  return blackInCheck;
}

void Board::moveUnchecked(Move _move)
{
  Piece *startPiece = pieces[_move.start.x][_move.start.y];
  Piece *endPiece = pieces[_move.end.x][_move.end.y];

  if (startPiece == nullptr)
  {
    throw "There is no piece to move";
  }

  if (endPiece == nullptr)
  {
    // Move directly
    pieces[_move.start.x][_move.start.y] = nullptr;
    pieces[_move.end.x][_move.end.y] = startPiece;
    pieces[_move.end.x][_move.end.y]->updateCoordinate(_move.end);
  }
  else if (startPiece->isOpponentPieceAt(_move.end, *this))
  {
    // Take piece and move
    pieces[_move.start.x][_move.start.y] = nullptr;
    pieces[_move.end.x][_move.end.y] = startPiece;
    pieces[_move.end.x][_move.end.y]->updateCoordinate(_move.end);
  }
  else if (startPiece->isOwnPieceAt(_move.end, *this))
  {
    // Capturing own piece
    std::cerr << "Warning: Capturing own piece: ";
    _move.display();
    std::cout << std::endl;
    pieces[_move.start.x][_move.start.y] = nullptr;
    pieces[_move.end.x][_move.end.y] = startPiece;
    pieces[_move.end.x][_move.end.y]->updateCoordinate(_move.end);
  }
}

bool Board::isOpponentInCheck()
{
  if (isWhiteTurn)
  {
    return isBlackInCheck();
  }
  else
  {
    return isWhiteInCheck();
  }
}

bool Board::isPlayerInCheck()
{
  if (isWhiteTurn)
  {
    return isWhiteInCheck();
  }
  else
  {
    return isBlackInCheck();
  }
}

bool Board::getIsWhiteTurn()
{
  return isWhiteTurn;
}