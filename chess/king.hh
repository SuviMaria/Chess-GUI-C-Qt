#ifndef KING_HH
#define KING_HH

#include "chesspiece.hh"
#include "vector"

/**
 * @brief The King class
 * The king moves one square in any direction, horizontally, vertically, or diagonally.
 */
class King : public ChessPiece
{
public:
    King(ChessColor col);
    virtual ~King() = default;

    virtual set<Coord> get_allowed_moves(ChessBoard const& board) const override;

private:

};

#endif // KING_HH
