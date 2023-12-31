#include "headers/board.h"
#include <iomanip>
#include <stdlib.h>
#include "headers/errors.h"
#include "headers/utils.h"

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
  whiteKing = nullptr;
  blackKing = nullptr;

  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (src.getPieceAt({j, i}) == nullptr)
      {
        getPieceAt({j, i}) = nullptr;
      }
      else
      {
        getPieceAt({j, i}) = createPiece(src.getPieceAt({j, i})->getPosition(), src.getPieceAt({j, i})->getSymbol());
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
  // cerr << "Parsing FEN: " << fen << endl;

  whiteKing = nullptr;
  blackKing = nullptr;

  // Clear the board
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      getPieceAt({i, j}) = nullptr;
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
    else if (isdigit(current_char) > 0)
    {
      current_x += current_char - '0';
    }
    else
    {
      getPieceAt({current_x, current_y}) = createPiece({current_x, current_y}, current_char);
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
  if (fen[index] == '-')
  {
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
    enPassantTarget = {fen[index], fen[index + 1]};
    index += 2;
  }

  index++;

  // Halfmove clock
  char buf1[8] = {'\0'};
  int idx = 0;
  while (isdigit(fen[index]) > 0)
  {
    buf1[idx] = fen[index];
    index++;
    idx++;
  }
  index++;
  buf1[idx] = '\0';
  halfMoveClock = atoi(buf1);

  // Fullmove clock
  char buf2[8] = {'\0'};
  idx = 0;
  while (isdigit(fen[index]) > 0)
  {
    buf2[idx] = fen[index];
    index++;
    idx++;
  }
  fullMoveClock = atoi(buf2);
  index++;

  // Other things
  state = GameState::Playing;
  promotionPiece = nullptr;

  if (whiteKing == nullptr)
    throw Error(ErrorCode::WhiteKingNotFound);

  if (blackKing == nullptr)
    throw Error(ErrorCode::BlackKingNotFound);

  return 0;
}

std::string Board::toFEN()
{
  std::string res;
  for (int i = 0; i < 8; i++)
  {
    int count = 0;
    for (int j = 0; j < 8; j++)
    {
      Piece *pc = getPieceAt({j, i});
      if (pc == nullptr)
      {
        count++;
      }
      else
      {
        if (count > 0)
          res.append(std::to_string(count));

        count = 0;
        res.push_back(pc->getSymbol());
      }
    }
    if (count > 0)
      res.append(std::to_string(count));
    if (i != 7)
      res.push_back('/');
  }

  res.push_back(' ');
  res.push_back(isWhiteTurn ? 'w' : 'b');
  res.push_back(' ');
  bool cast = false;
  if (!canCastle[0] && !canCastle[1] && !canCastle[2] && !canCastle[3])
    res.push_back('-');

  if (canCastle[0])
    res.push_back('K');

  if (canCastle[1])
    res.push_back('Q');

  if (canCastle[2])
    res.push_back('k');

  if (canCastle[3])
    res.push_back('q');

  res.push_back(' ');

  if (enPassantTarget.isValidPosition())
    res.append(enPassantTarget.getChessCoordinate());
  else
    res.push_back('-');

  res.push_back(' ');
  res.append(std::to_string(halfMoveClock));

  res.push_back(' ');
  res.append(std::to_string(fullMoveClock));

  return res;
}

void Board::display()
{
  using namespace std;

  for (int i = 0; i < 8; i++)
  {
    cout << 8 - i << " |";
    for (int j = 0; j < 8; j++)
    {
      if (getPieceAt({j, i}))
      {
        cout << getPieceAt({j, i})->getSymbol();
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
    cerr << "White is in check" << endl;
  }
  else if (isBlackInCheck())
  {
    cerr << "Black is in check" << endl;
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

Piece *&Board::getPieceAt(Coordinate _coord)
{
  return pieces[_coord.y][_coord.x];
}

bool Board::isWhiteInCheck()
{
  bool whiteInCheck = false;

  if (whiteKing == nullptr)
    throw ErrorCode::WhiteKingNotFound;

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
    throw Error(ErrorCode::BlackKingNotFound);

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

void Board::moveUnchecked(Move _move)
{
  Piece *startPiece = getPieceAt(_move.start);
  Piece *endPiece = getPieceAt(_move.end);

  if (startPiece == nullptr)
  {
    throw Error(ErrorCode::NoStartPiece);
  }

  if (isEnpassant(this, _move))
  {
    getPieceAt({_move.end.x, _move.start.y}) = nullptr; // capture opponent piece
  }

  bool kingsidec = isKingSideCastle(this, _move);
  bool queensidec = isQueenSideCastle(this, _move);

  if (kingsidec != 0 || queensidec != 0) // Handles casting
  {
    int tempy = isWhiteTurn ? 7 : 0;
    Piece *king = getPieceAt({4, tempy});
    if (kingsidec == 1)
    {
      Piece *rook = getPieceAt({7, tempy});

      getPieceAt({4, tempy}) = nullptr; // remove king
      getPieceAt({7, tempy}) = nullptr; // remove rook
      getPieceAt({5, tempy}) = rook;    // place rook
      getPieceAt({6, tempy}) = king;    // place king
      rook->updateCoordinate({5, tempy});
      king->updateCoordinate({6, tempy});
    }

    if (queensidec == 1)
    {
      Piece *rook = getPieceAt({0, tempy});
      getPieceAt({0, tempy}) = nullptr; // remove rook
      getPieceAt({2, tempy}) = king;    // place king
      getPieceAt({3, tempy}) = rook;    // place rook
      getPieceAt({4, tempy}) = nullptr; // remove king
      rook->updateCoordinate({3, tempy});
      king->updateCoordinate({2, tempy});
    }
  }
  else
  {

    // Move directly
    getPieceAt(_move.start) = nullptr;
    getPieceAt(_move.end) = startPiece;
    getPieceAt(_move.end)->updateCoordinate(_move.end);
  }
}

MoveType Board::performMove(Move _move)
{
  // Disallow moving if a promotion is pending
  if (isWaitingForPromotion())
  {
    throw Error(ErrorCode::WaitingPromotion);
  }

  Piece *startPiece = getPieceAt(_move.start);
  Piece *endPiece = getPieceAt(_move.end);
  Piece *capturePiece = endPiece;
  // check if there is a piece to move
  if (startPiece == nullptr)
    throw Error(ErrorCode::NoStartPiece);

  bool isCorrectTurn = isWhiteTurn == startPiece->isWhite();
  if (!isCorrectTurn)
    throw Error(ErrorCode::IncorrectTurn);

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

  if (!isValidMove)
    throw Error(ErrorCode::InvalidMove);

  // Store if pawn has moved two pieces
  // Used after move has been performed to set enpassant target square
  bool pawntwomoves = isPawnTwoMovesAhead(this, _move);
  bool kingsidec = isKingSideCastle(this, _move);
  bool queensidec = isQueenSideCastle(this, _move);
  MoveType mvType = MoveType::Normal;

  // Move the pieces
  if (kingsidec != 0 || queensidec != 0) // Handles casting
  {
    int tempy = isWhiteTurn ? 7 : 0;
    Piece *king = getPieceAt({4, tempy});
    if (king == nullptr)
      throw Error("Attempting to perform castle when king is not in starting position");

    if (kingsidec == 1)
    {
      Piece *rook = getPieceAt({7, tempy});
      if (rook == nullptr)
        throw Error("Attempting to perform castle when rook is not in starting position");

      getPieceAt({4, tempy}) = nullptr; // remove king
      getPieceAt({7, tempy}) = nullptr; // remove rook
      getPieceAt({5, tempy}) = rook;    // place rook
      getPieceAt({6, tempy}) = king;    // place king
      rook->updateCoordinate({5, tempy});
      king->updateCoordinate({6, tempy});
    }

    if (queensidec == 1)
    {
      Piece *rook = getPieceAt({0, tempy});
      if (rook == nullptr)
        throw Error("Attempting to perform castle when rook is not in starting position");

      getPieceAt({0, tempy}) = nullptr; // remove rook
      getPieceAt({2, tempy}) = king;    // place king
      getPieceAt({3, tempy}) = rook;    // place rook
      getPieceAt({4, tempy}) = nullptr; // remove king
      rook->updateCoordinate({3, tempy});
      king->updateCoordinate({2, tempy});
    }

    if (kingsidec == 1 || queensidec == 1)
    {
      int offset = isWhiteTurn ? 0 : 2;
      canCastle[offset] = false;
      canCastle[offset + 1] = false;
      mvType = MoveType::Castle;
    }
  }
  else if (isEnpassant(this, _move))
  {
    int direction = isWhiteTurn ? 1 : -1;
    Coordinate captureCoord = {_move.end.x, _move.start.y};

    capturePiece = getPieceAt(captureCoord); // Special case

    getPieceAt(_move.start) = nullptr;  // remove startpiece
    getPieceAt(captureCoord) = nullptr; // capture opponent piece

    getPieceAt(_move.end) = startPiece;
    getPieceAt(_move.end)->updateCoordinate(_move.end);

    mvType = MoveType::Enpassant;
  }
  else if (isPromotion(this, _move)) // Handles promotion
  {
    getPieceAt(_move.start) = nullptr;
    getPieceAt(_move.end) = startPiece;
    getPieceAt(_move.end)->updateCoordinate(_move.end);

    promotionPiece = startPiece;

    if (_move.promotion)
    {
      promoteTo(_move.promotion, false);
    }
    mvType = MoveType::Promotion;
  }
  else // Handles other moves
  {
    getPieceAt(_move.start) = nullptr;
    getPieceAt(_move.end) = startPiece;
    getPieceAt(_move.end)->updateCoordinate(_move.end);

    if (isCapture(this, _move))
    {
      mvType = MoveType::Capture;
    }
  }

  if (capturePiece != nullptr)
  {
    capturedPieces.push_back(capturePiece);
  }

  // Set enpassant target if pawn has moved two moves ahead
  // Otherwise reset enpassant target square
  if (pawntwomoves)
  {
    int direction = isWhiteTurn ? 1 : -1;
    enPassantTarget = {_move.end.x, _move.end.y + direction};
  }
  else
    enPassantTarget = {-1, -1}; // reset enpassant target every turn

  // Increment fullMoveClock on black turn
  if (!isWhiteTurn)
  {
    fullMoveClock++;
  }

  // Halfmove clock increments every piece but resets when there is pawn movement or capture
  // Otherwise increment it by one
  if (tolower(startPiece->getSymbol()) == 'p' || isCapture(this, _move))
    halfMoveClock = 0;
  else
    halfMoveClock++;

  char startSymbol = startPiece->getSymbol();

  // Remove castling availability if rook or king moves
  if ((startSymbol == 'R') && (_move.start == Coordinate{7, 7})) // disable white kingside castle
  {
    canCastle[0] == false;
  }
  else if ((startSymbol == 'R') && (_move.start == Coordinate{7, 0})) // disable white queenside castle
  {
    canCastle[1] = false;
  }
  else if ((startSymbol == 'r') && (_move.start == Coordinate{0, 7})) // disable black kingside castle
  {
    canCastle[2] == false;
  }
  else if ((startSymbol == 'r') && (_move.start == Coordinate{0, 0})) // disable black queenside castle
  {
    canCastle[3] = false;
  }
  else if (startSymbol == 'K') // disable white castling
  {
    canCastle[0] = false;
    canCastle[1] = false;
  }
  else if (startSymbol == 'k') // disable black castling
  {
    canCastle[2] = false;
    canCastle[3] = false;
  }

  // check for draws
  if (halfMoveClock >= 50)
  {
    std::cerr << "Draw!" << std::endl;
    state = GameState::Draw;
  }

  if (!isWaitingForPromotion())
    isWhiteTurn = !isWhiteTurn; // Change to opposite player's turn

  // Check for stalemates and checkmates
  std::vector<Move> allMoves = getAllPlayerLegalMoves(isWhiteTurn);

  if (allMoves.size() == 0 && isPlayerInCheck())
  {
    state = isWhiteTurn ? GameState::BlackWins : GameState::WhiteWins;
    std::cerr << "Checkmate!\n"
              << (isWhiteTurn ? "Black Wins!" : "White wins!") << std::endl;
  }
  else if (allMoves.size() == 0)
  {
    state = GameState::Stalemate;
    std::cerr
        << "Stalemate!" << std::endl;
  }

  previousMoves.push_back(_move);

  return mvType;
}

// Helper function to create a piece at given coordinate
Piece *Board::createPiece(Coordinate _pos, char piece)
{
  Piece *piece_ptr;
  switch (piece)
  {
  case 'p':
  case 'P':
    piece_ptr = new Pawn({_pos.x, _pos.y}, isUppercase(piece));
    break;

  case 'r':
  case 'R':
    piece_ptr = new Rook({_pos.x, _pos.y}, isUppercase(piece));
    break;

  case 'n':
  case 'N':
    piece_ptr = new Knight({_pos.x, _pos.y}, isUppercase(piece));
    break;

  case 'b':
  case 'B':
    piece_ptr = new Bishop({_pos.x, _pos.y}, isUppercase(piece));
    break;

  case 'q':
  case 'Q':
    piece_ptr = new Queen({_pos.x, _pos.y}, isUppercase(piece));
    break;

  case 'k':
  case 'K':
    piece_ptr = new King({_pos.x, _pos.y}, isUppercase(piece));
    if (isUppercase(piece))
    {
      if (whiteKing == nullptr)
        whiteKing = piece_ptr;
      else
        throw Error(ErrorCode::MultipleWhiteKing);
    }
    else
    {
      if (blackKing == nullptr)
        blackKing = piece_ptr;
      else
        throw Error(ErrorCode::MultipleBlackKing);
    }
    break;

  default:
    throw Error(ErrorCode::InvalidPiece);
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
    throw Error("Wrong castling type");
    break;
  }
}

std::vector<Move> Board::getAllPlayerMoves(bool _white)
{
  std::vector<Piece *> allpieces = _white ? getWhitePieces() : getBlackPieces();
  std::vector<Move> allMoves;

  for (int i = 0; i < allpieces.size(); i++)
  {
    std::vector<Move> movesForPiece = allpieces[i]->getAllMoves(*this);
    for (int j = 0; j < movesForPiece.size(); j++)
    {
      allMoves.push_back(movesForPiece[j]);
    }
  }

  return allMoves;
}

std::vector<Move> Board::getAllPlayerLegalMoves(bool _white)
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
      if (getPieceAt({i, j}) != nullptr && getPieceAt({i, j})->isWhite())
      {
        pcs.push_back(getPieceAt({i, j}));
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
      if (getPieceAt({i, j}) != nullptr && !getPieceAt({i, j})->isWhite())
      {
        pcs.push_back(getPieceAt({i, j}));
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

void Board::promoteTo(char _type, bool addtoprevmove)
{
  if ((promotionPiece != nullptr))
  {
    char promopc = isWhiteTurn ? toupper(_type) : tolower(_type);
    Coordinate dest = promotionPiece->getPosition();
    Piece *temp = createPiece(dest, promopc);
    getPieceAt(dest) = temp;
    promotionPiece = nullptr;
    if (addtoprevmove)
    {
      previousMoves.back().promotion = promopc;
      isWhiteTurn = !isWhiteTurn;
    }
  }
}

Piece *Board::getPromotionPiece()
{
  return promotionPiece;
}

std::vector<Move> Board::getPreviousMoves()
{
  return previousMoves;
}

void Board::undoLastMove()
{
  if (previousMoves.size() > 0)
  {
    Move prev = previousMoves.back();
    // Undoing simple moves is easy. But we also need to handle undoing
    // moves like castling, promotion, and castling availability undo
    throw Error("TODO:");
  }
}

void Board::resign()
{
  state = isWhiteTurn ? GameState::WhiteResigns : GameState::BlackResigns;
}

std::vector<Piece *> Board::getCapturedPieces(bool _whitePiece)
{
  std::vector<Piece *> pcs;
  for (int i = 0; i < capturedPieces.size(); i++)
  {
    if (_whitePiece == capturedPieces[i]->isWhite())
      pcs.push_back(capturedPieces[i]);
  }

  return pcs;
}