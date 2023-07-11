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

  // Castling
  canCastle[0] = false;
  canCastle[1] = false;
  canCastle[2] = false;
  canCastle[3] = false;

  // If there is '-' then no castling is available
  // If there is ' ' then some castling was found and set
  while (fen[index] != ' ' && fen[index] != '-')
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
  cout << endl;
}

void Board::display_meta()
{
  using namespace std;
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
  bool whiteInCheck = false;
  bool blackInCheck = false;

  if (whiteKing == nullptr)
  {
    std::cerr << "Cannot find white king" << std::endl;
    throw "Cannot find white king";
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
        std::cout << "Here" << std::endl;
        whiteInCheck = true;
        break;
      }
    }
  }

  return whiteInCheck;
}

bool Board::isBlackInCheck()
{
  bool blackInCheck = false;

  if (blackKing == nullptr)
  {
    std::cerr << "Cannot find black king" << std::endl;
    throw "Cannot find black king";
  }

  for (int i = 0; i < whitePieces.size(); i++)
  {
    std::vector<Move> moves = whitePieces[i]->getAllMoves(*this);
    if (blackInCheck)
      break;
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

  // Move directly
  pieces[_move.start.x][_move.start.y] = nullptr;
  pieces[_move.end.x][_move.end.y] = startPiece;
  pieces[_move.end.x][_move.end.y]->updateCoordinate(_move.end);
}

void Board::perfomMove(Move _move)
{
  std::cout << _move << std::endl;
  Piece *startPiece = getPieceAt(_move.start);
  Piece *endPiece = getPieceAt(_move.end);

  if (startPiece == nullptr)
  {
    std::cerr << "No piece to move" << std::endl;
    return;
  }

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
Piece *Board::createPiece(Coordinate _pos, char piece)
{
  Piece *piece_ptr;
  switch (piece)
  {
  case 'p':
  case 'P':
    piece_ptr = new Pawn({_pos.x, _pos.y}, isupper(piece));
    break;

  case 'r':
  case 'R':
    piece_ptr = new Rook({_pos.x, _pos.y}, isupper(piece));
    break;

  case 'n':
  case 'N':
    piece_ptr = new Knight({_pos.x, _pos.y}, isupper(piece));
    break;

  case 'b':
  case 'B':
    piece_ptr = new Bishop({_pos.x, _pos.y}, isupper(piece));
    break;

  case 'q':
  case 'Q':
    piece_ptr = new Queen({_pos.x, _pos.y}, isupper(piece));
    break;

  case 'k':
  case 'K':
    piece_ptr = new King({_pos.x, _pos.y}, isupper(piece));
    if (isupper(piece))
    {
      whiteKing = piece_ptr;
    }
    else
    {
      blackKing = piece_ptr;
    }
    break;

  default:
    std::cerr << "Invalid piece" << std::endl;
    throw "Invalid piece";
  }

  if (isupper(piece))
  {
    whitePieces.push_back(piece_ptr);
  }
  else if (islower(piece))
  {
    blackPieces.push_back(piece_ptr);
  }

  return piece_ptr;
}

bool Board::castlingAvailable(moveType::MoveType _type, bool _isWhitePiece)
{
  int offset = _isWhitePiece ? 0 : 2;
  int pieceOffset = 0;
  if (_type == moveType::KingsideCastle)
  {
    pieceOffset = 0;
  }
  else if (_type == moveType::QueensideCastle)
  {
    pieceOffset = 1;
  }
  else
  {
    throw "Invalid movetype for checking castling availablity";
  }

  return canCastle[offset + pieceOffset];
}
