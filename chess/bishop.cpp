#include "bishop.hh"

Bishop::Bishop(ChessColor col) : ChessPiece(col, BISHOP, "bishop") {

}

// Palauttaa mahdolliset koordinaatit, joihin nappula voi liikkua.
set<Coord> Bishop::get_allowed_moves(const ChessBoard &board) const {
    set<Coord> moves;
    auto [row, col] = get_position();


    // Suunta flags: koillinen, kaakko, lounas, luode
    bool ne_blocked = false;
    bool nw_blocked = false;
    bool se_blocked = false;
    bool sw_blocked = false;

    for (int i = 1; i < 8; i++) {

    // Koillinen
    if (board.coord_in_bounds({row - i, col + i}) && !ne_blocked) {
        auto piece = board.get_piece_at({row - i, col + i});
        if (piece == nullptr) {
            moves.insert({row - i, col + i});
        } else {
            if (piece->get_color() != get_color()) {
                moves.insert({row - i, col + i});
            }
            ne_blocked = true;
            }
    }

    // Kaakko
    if (board.coord_in_bounds({row - i, col - i}) && !nw_blocked) {
        auto piece = board.get_piece_at({row - i, col - i});
        if (piece == nullptr) {
            moves.insert({row - i, col - i});
        } else {
            if (piece->get_color() != get_color()) {
                moves.insert({row - i, col - i});
            }
            nw_blocked = true;
            }
    }

    // Lounas
    if (board.coord_in_bounds({row + i, col + i}) && !se_blocked) {
        auto piece = board.get_piece_at({row + i, col + i});
        if (piece == nullptr) {
            moves.insert({row + i, col + i});
        } else {
            if (piece->get_color() != get_color()) {
                moves.insert({row + i, col + i});
            }
            se_blocked = true;
        }
    }

    // Luode
    if (board.coord_in_bounds({row + i, col - i}) && !sw_blocked) {
        auto piece = board.get_piece_at({row + i, col - i});
        if (piece == nullptr) {
            moves.insert({row + i, col - i});
        } else {
            if (piece->get_color() != get_color()) {
                moves.insert({row + i, col - i});
            }
            sw_blocked = true;
        }
    }
    }


    return moves;
}


