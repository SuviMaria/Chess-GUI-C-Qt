#include "king.hh"

King::King(ChessColor col) : ChessPiece(col, KING, "king") {

}

// Palauttaa mahdolliset koordinaatit, joihin nappula voi liikkua
set<Coord> King::get_allowed_moves(const ChessBoard &board) const {

    set<Coord> moves;

    auto [row, col] = get_position();

    vector<pair<int, int>> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},
        {1, 1}, {-1, -1}, {1, -1}, {-1, 1}
    };

    for (auto [dr, dc] : directions) {
        Coord target = {row + dr, col + dc};
        if (board.coord_in_bounds(target)) {
            auto move = board.get_piece_at(target);
            if (move == nullptr || move->get_color() != get_color()) {
                moves.insert(target);
            }
        }
    }

    return moves;
}
