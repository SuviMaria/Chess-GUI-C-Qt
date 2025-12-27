#include "queen.hh"

Queen::Queen(ChessColor col) : ChessPiece(col, QUEEN, "queen") {

}

// Palauttaa mahdolliset koordinaatit, joihin nappula voi liikkua
set<Coord> Queen::get_allowed_moves(const ChessBoard &board) const {
    set<Coord> moves;
    auto [row, col] = get_position();

    // Pitävät kirjaa siitä, onko kohdalle osunut vaaka- tai pystysuunnassa toista nappulaa
    bool row_piece_forward = false;
    bool row_piece_backward = false;
    bool col_piece_forward = false;
    bool col_piece_backward = false;

    for (int i = 1; i<8; i++) {
        // Jos joka suunnassa on jo löytynyt nappula, ei iterointia enää jatketa
        if (row_piece_forward && row_piece_backward && col_piece_forward && col_piece_backward) {
            break;
        }

        // Tarkastetaan, voiko liikkua vaakasuuntaan. Mahdollista, jos
        // - koordinaatti on saavutettavissa
        // - ruutu on tyhjä
        // - siinä on erivärinen nappula
        // - vaakasuunnassa ei ole vielä kohdattu toista nappulaa
        if (board.coord_in_bounds({row+i, col}) && !row_piece_forward) {
            auto move = board.get_piece_at({row + i, col});
            if (move == nullptr) {
                moves.insert({row + i, col});
            } else if (move->get_color() != get_color()) {
                moves.insert({row + i, col});
                row_piece_forward = true;
            } else {
                row_piece_forward = true;
            }
        }

        if (board.coord_in_bounds({row-i, col}) && !row_piece_backward) {
            auto move = board.get_piece_at({row - i, col});
            if (move == nullptr) {
                moves.insert({row - i, col});
            } else if (move->get_color() != get_color()) {
                moves.insert({row - i, col});
                row_piece_backward = true;
            } else {
                row_piece_backward = true;
            }
        }

        // Tarkastetaan, voiko liikkua pystysuuntaan samoilla ehdoilla kuin vaakasuuntaan
        // tarkastuksessa
        if (board.coord_in_bounds({row, col+i}) && !col_piece_forward) {
            auto move = board.get_piece_at({row, col+i});
                    if (move == nullptr) {
                        moves.insert({row, col+i});
                    } else if (move->get_color() != get_color()) {
                        moves.insert({row + i, col});
                        col_piece_forward = true;
                    } else {
                        col_piece_forward = true;
                    }
                }

                if (board.coord_in_bounds({row, col-i}) && !col_piece_backward) {
                    auto move = board.get_piece_at({row, col-i});
                    if (move == nullptr) {
                        moves.insert({row, col-i});
                    } else if (move->get_color() != get_color()) {
                        moves.insert({row, col-i});
                        col_piece_backward = true;
                    } else {
                        col_piece_backward = true;
                    }
                }

            }

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

