#include "headers/board.h"
#include <iomanip>

// TODO: Add gamestate variable that stores gamestate(Playing, Check, CheckMate, Stalemate, Draw)
// TODO: Check for checkmates and stalemates

Board::Board()
{
  fromFEN(STARTING_FEN);
}

Board::Board(std::string fen)
{
  fromFEN(fen);
}

Board::Board(Board &src)
{
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (src.pieces[i][j] == nullptr)
      {
        pieces[i][j] = nullptr;
      }
      else
      {
        pieces[i][j] = createPiece(src.pieces[i][j]->getPosition(), src.pieces[i][j]->getSymbol());
      }
    }
  }

  isWhiteTurn = src.isWhiteTurn;
  for (int i = 0; i < 4; i++)
  {
    canCastle[i] = src.canCastle[i];
  }
  enPassantTarget = src.enPassantTarget;
  halfMoveClock = src.halfMoveClock;
  fullMoveClock = src.fullMoveClock;
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
      pieces[current_x][current_y] = createPiece({current_x, current_y}, current_char);
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
  if (isWhiteInCheck())
  {
    cout << "White is in check" << endl;
  }
  else if (isBlackInCheck())
  {
    cout << "Black is in check" << endl;
  }
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
    if (whiteInCheck)
      break;
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
  Piece *blackKing = nullptr;
  std::vector<Piece *> whitePieces;
  bool blackInCheck = false;

  // Search for black king and white pieces
  for (int i = 0; i < 8; i++)
  {
    if (blackInCheck)
      break;
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

void Board::setEnpassantTarget(Coordinate _coord)
{
  enPassantTarget = _coord;
}

void Board::moveUnchecked(Move _move)
{
  Piece *startPiece = getPieceAt(_move.start);
  Piece *endPiece = getPieceAt(_move.end);

  if (startPiece == nullptr)
  {
    std::cout << "There is no piece to move" << std::endl;
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
    std::cerr << "Warning: Capturing own piece: " << _move << std::endl;
    pieces[_move.start.x][_move.start.y] = nullptr;
    pieces[_move.end.x][_move.end.y] = startPiece;
    pieces[_move.end.x][_move.end.y]->updateCoordinate(_move.end);
  }
}

void Board::perfomMove(Move _move)
{
  Piece *startPiece = getPieceAt(_move.start);
  Piece *endPiece = getPieceAt(_move.end);

  if (startPiece == nullptr)
    return;

  bool isCorrectTurn = isWhiteTurn == startPiece->isWhite();
  if (!isCorrectTurn)
  {
    std::cerr << "It is not your turn" << std::endl;
    return;
  }

  std::vector<Move> moves = startPiece->getLegalMoves(*this);
  bool isValidMove = false;

  // Check if the move is valid
  for (int i = 0; i < moves.size(); i++)
  {
    if (_move.end.x == moves[i].end.x && _move.end.y == moves[i].end.y)
    {
      isValidMove = true;
      _move = moves[i]; // _moves may not provide moveType so use moves[i]
      break;
    }
  }

  if (isValidMove)
  {
    if (_move.type == moveType::KingsideCastle || _move.type == moveType::QueensideCastle)
    {
      // TODO: handle castling
    }
    else
    {
      pieces[_move.start.x][_move.start.y] = nullptr;
      pieces[_move.end.x][_move.end.y] = startPiece;
      pieces[_move.end.x][_move.end.y]->updateCoordinate(_move.end);
    }

    // Increment fullMoveClock on black turn
    if (!isWhiteTurn)
    {
      fullMoveClock++;
    }

    isWhiteTurn = !isWhiteTurn;
    enPassantTarget = {-1, -1}; // reset enpassant target every turn

    // Halfmove clock increments every piece but resets when there is pawn movement or capture
    if (startPiece->getSymbol() == 'p' || startPiece->getSymbol() == 'P' || _move.type == moveType::Capture)
    {
      halfMoveClock = 0;
    }
    else
    {
      halfMoveClock++;
    }
  }
  else
  {
    std::cerr << "Trying to perform an invalid move: " << _move << std::endl;
  }
}

// Helper function to create a piece at given coordinate
Piece *createPiece(Coordinate _pos, char piece)
{
  switch (piece)
  {
  case 'p':
  case 'P':
    return new Pawn({_pos.x, _pos.y}, isupper(piece));

  case 'r':
  case 'R':
    return new Rook({_pos.x, _pos.y}, isupper(piece));

  case 'n':
  case 'N':
    return new Knight({_pos.x, _pos.y}, isupper(piece));

  case 'b':
  case 'B':
    return new Bishop({_pos.x, _pos.y}, isupper(piece));

  case 'q':
  case 'Q':
    return new Queen({_pos.x, _pos.y}, isupper(piece));

  case 'k':
  case 'K':
    return new King({_pos.x, _pos.y}, isupper(piece));

  default:
    std::cerr << "Invalid piece" << std::endl;
    throw "Invalid piece";
  }

  std::cerr << "Unreachable" << std::endl;
  throw "Unreachable";
}