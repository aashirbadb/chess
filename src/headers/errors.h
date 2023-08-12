#pragma once
#include <exception>
#include <string>
enum class ErrorCode
{
    WhiteKingNotFound = 1,
    BlackKingNotFound,
    MultipleWhiteKing,
    MultipleBlackKing,
    NoStartPiece,
    InvalidPiece,
    WaitingPromotion,
    Message
};

class Error : public std::exception
{
    ErrorCode code;
    std::string error;

public:
    Error(ErrorCode _e) : code(_e) {}
    Error(std::string _err) : error(_err)
    {
        code = ErrorCode::Message;
    }

    Error(const char *_err) : error(_err)
    {
        code = ErrorCode::Message;
    }

    ~Error() {}

    const char *
    what() const noexcept override
    {
        switch (code)
        {
        case ErrorCode::WhiteKingNotFound:
            return "White king not found";
            break;
        case ErrorCode::BlackKingNotFound:
            return "Black king not found";
            break;
        case ErrorCode::MultipleWhiteKing:
            return "More than one white king is present in the board";
            break;
        case ErrorCode::MultipleBlackKing:
            return "More than one black king is present in the board";
            break;
        case ErrorCode::NoStartPiece:
            return "Start Piece not found";
            break;
        case ErrorCode::WaitingPromotion:
            return "Waiting to promote piece. Cannot perform move";
            break;
        case ErrorCode::InvalidPiece:
            return "Invalid piece";
            break;
        case ErrorCode::Message:
            return error.c_str();
        default:
            return "Unknown error";
            break;
        }
    }
};