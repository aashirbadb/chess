#include "headers/board.h"
#include <iomanip>
#include "headers/utils.h"
#include <stdlib.h>

bool isCapture(Board *_board, Move _move)
{
  Piece *piece = _board->getPieceAt(_move.start);
  return piece != nullptr && piece->isOpponentPieceAt(_move.end, _board);
}

bool isPromotion(Board *_board, Move _move)
{
  Piece *piece = _board->getPieceAt(_move.start);

  if (toupper(piece->getSymbol()) == 'P')
  {
    return _move.end.isPromotionSquare(isupper(piece->getSymbol()));
  }
  return false;
}

bool isCastle(Board *_board, Move _move)
{
  return false;
}

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
      if (src.pieces[j][i] == nullptr)
      {
        pieces[j][i] = nullptr;
      }
      else
      {
        pieces[j][i] = createPiece(src.pieces[j][i]->getPosition(), src.pieces[j][i]->getSymbol());
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
  for (int i = 0; i < allCreatedPiece.size(); i++)
  {
    delete allCreatedPiece[i];
  }
}

// https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
int Board::fromFEN(std::string fen)
{
  using namespace std;
  cerr << "Parsing FEN: " << fen << endl;

  // Clear the board
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      pieces[j][i] = nullptr;
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
      current_y++;
      current_x = 0;
      continue;
    }
    else if (isdigit(current_char))
    {
      current_x += current_char - '0';
    }
    else
    {
      pieces[current_y][current_x] = createPiece({current_x, current_y}, current_char);
      current_x++;
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
    // enPassantTarget = {fen[index + 1] - '1', fen[index] - 'a'};
    enPassantTarget = {fen[index], fen[index + 1]};
    index += 2;
  }
  index++;

  // Halfmove clock
  char buf1[8] = {'\0'};
  int idx = 0;
  while (isdigit(fen[index]))
  {
    buf1[idx++] = fen[index++];
  }
  halfMoveClock = atoi(buf1);
  index++;

  // Fullmove clock
  char buf2[8] = {'\0'};
  idx = 0;
  while (isdigit(fen[index]))
  {
    buf2[idx++] = fen[index++];
  }
  fullMoveClock = atoi(buf2);

  cerr << "Successfully parsed FEN" << endl;

  // Other things
  state = GameState::Playing;
  promotionPiece = nullptr;

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
  return pieces[_coord.y][_coord.x];
}

bool Board::isWhiteInCheck()
{
  bool whiteInCheck = false;

  if (whiteKing == nullptr)
  {
    std::cerr << "Cannot find white king" << std::endl;
    throw "Cannot find white king";
  }

  std::vector<Piece *> blackPieces = getBlackPieces();

  for (int i = 0; i < blackPieces.size(); i++)
  {
    if (whiteInCheck)
      break;
    std::vector<Move> moves = blackPieces[i]->getAllMoves(*this);
    for (int j = 0; j < moves.size(); j++)
    {
      if (moves[j].end == whiteKing->getPosition())
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
  bool blackInCheck = false;

  if (blackKing == nullptr)
  {
    std::cerr << "Cannot find black king" << std::endl;
    throw "Cannot find black king";
  }

  std::vector<Piece *> whitePieces = getWhitePieces();

  for (int i = 0; i < whitePieces.size(); i++)
  {
    std::vector<Move> moves = whitePieces[i]->getAllMoves(*this);
    if (blackInCheck)
      break;
    for (int j = 0; j < moves.size(); j++)
    {
      if (moves[j].end == blackKing->getPosition())
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
    std::cerr << "There is no piece to move" << std::endl;
    throw "There is no piece to move";
  }

  // Move directly
  pieces[_move.start.y][_move.start.x] = nullptr;
  pieces[_move.end.y][_move.end.x] = startPiece;
  pieces[_move.end.y][_move.end.x]->updateCoordinate(_move.end);
}

void Board::performMove(Move _move)
{
  if (isWaitingForPromotion())
    throw "Waiting to promote piece";
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
      break;
    }
  }

  if (isValidMove)
  {
    if (isCastle(this, _move))
    {
      // TODO: handle castling
    }
    else if (isPromotion(this, _move))
    {
      // Promotion
      // TODO: Allow user to choose promotion piece
      pieces[_move.start.y][_move.start.x] = nullptr;
      pieces[_move.end.y][_move.end.x] = startPiece;
      pieces[_move.end.y][_move.end.x]->updateCoordinate(_move.end);

      promotionPiece = startPiece;
    }
    else
    {
      pieces[_move.start.y][_move.start.x] = nullptr;
      pieces[_move.end.y][_move.end.x] = startPiece;
      pieces[_move.end.y][_move.end.x]->updateCoordinate(_move.end);
    }

    // Increment fullMoveClock on black turn
    if (!isWhiteTurn)
    {
      fullMoveClock++;
    }

    isWhiteTurn = !isWhiteTurn;
    enPassantTarget = {-1, -1}; // reset enpassant target every turn

    // Halfmove clock increments every piece but resets when there is pawn movement or capture
    if (startPiece->getSymbol() == 'p' || startPiece->getSymbol() == 'P' || isCapture(this, _move))
    {
      halfMoveClock = 0;
    }
    else
    {
      halfMoveClock++;
    }

    char startSymbol = startPiece->getSymbol();

    // Remove availability if rook or king moves

    // White kingside castle
    if (startSymbol == 'R' && (_move.start == Coordinate{7, 7}))
    {
      canCastle[0] == false;
    }

    // White queenside castle
    if (startSymbol == 'R' && (_move.start == Coordinate{7, 0}))
    {
      canCastle[1] = false;
    }

    // White kingside castle
    if (startSymbol == 'r' && (_move.start == Coordinate{0, 7}))
    {
      canCastle[2] == false;
    }

    // White queenside castle
    if (startSymbol == 'r' && (_move.start == Coordinate{0, 0}))
    {
      canCastle[3] = false;
    }

    // If king moves, no castle available for the player
    if (startSymbol == 'K')
    {
      canCastle[0] = false;
      canCastle[1] = false;
    }

    if (startSymbol == 'k')
    {
      canCastle[2] = false;
      canCastle[3] = false;
    }
  }
  else
  {
    std::cerr << "Trying to perform an invalid move: " << _move << std::endl;
  }

  if (halfMoveClock >= 50)
  {
    std::cerr << "Draw!" << std::endl;
    state = GameState::Draw;
  }

  // Check for stalemates and checkmates
  std::vector<Move> allMoves = getAllPlayerMoves(isWhiteTurn);
  if (allMoves.size() == 0 && isPlayerInCheck())
  {
    if (isWhiteTurn)
    {
      state = GameState::BlackWins;
    }
    else
    {
      state = GameState::WhiteWins;
    }
    std::cerr << "Checkmate!\n"
              << (isWhiteTurn ? "Black Wins!" : "White wins!") << std::endl;
  }
  else if (allMoves.size() == 0)
  {
    state = GameState::Stalemate;
    std::cerr
        << "Stalemate!" << std::endl;
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

  allCreatedPiece.push_back(piece_ptr);

  return piece_ptr;
}

bool Board::castlingAvailable(char _type)
{
  switch (_type)
  {
  case 'K':
    return canCastle[0];
    break;
  case 'Q':
    return canCastle[1];
    break;
  case 'k':
    return canCastle[2];
    break;
  case 'q':
    return canCastle[3];
    break;

  default:
    throw "Wrong castling type";
    break;
  }
}

std::vector<Move> Board::getAllPlayerMoves(bool _white)
{
  std::vector<Piece *> allpieces = _white ? getWhitePieces() : getBlackPieces();
  std::vector<Move> allMoves;

  for (int i = 0; i < allpieces.size(); i++)
  {
    std::vector<Move> movesForPiece = allpieces[i]->getLegalMoves(*this);
    for (int j = 0; j < movesForPiece.size(); j++)
    {
      allMoves.push_back(movesForPiece[j]);
    }
  }

  return allMoves;
}

Coordinate Board::getEnpassantTarget()
{
  return enPassantTarget;
}

void Board::setPlayerTurn(bool _turn)
{
  isWhiteTurn = _turn;
}

std::vector<Piece *> Board::getWhitePieces()
{
  std::vector<Piece *> pcs;
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (pieces[j][i] != nullptr && pieces[j][i]->isWhite())
      {
        pcs.push_back(pieces[j][i]);
      }
    }
  }

  return pcs;
}

std::vector<Piece *> Board::getBlackPieces()
{
  std::vector<Piece *> pcs;
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (pieces[j][i] != nullptr && !pieces[j][i]->isWhite())
      {
        pcs.push_back(pieces[j][i]);
      }
    }
  }

  return pcs;
}

Piece *Board::getWhiteKing()
{
  return whiteKing;
}

Piece *Board::getBlackKing()
{
  return blackKing;
}

GameState Board::getGameState()
{
  return state;
}

bool Board::isWaitingForPromotion()
{
  return promotionPiece != nullptr;
}

void Board::promoteTo(char _type)
{
  if (promotionPiece != nullptr && promotionPiece->isWhite() == isupper(_type))
  {
    Coordinate dest = promotionPiece->getPosition();
    Piece *temp = createPiece(dest, _type);
    pieces[dest.y][dest.x] = temp;
    promotionPiece = nullptr;
  }
}

Piece *Board::getPromotionPiece()
{
  return promotionPiece;
}