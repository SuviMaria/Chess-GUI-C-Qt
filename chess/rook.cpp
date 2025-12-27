#include "rook.hh"

Rook::Rook(ChessColor col) : ChessPiece(col, ROOK, "rook") {

}

// Palauttaa sallitut koordinaatit set-muodossa.
// Tornin on sallittua liikkua vaaka- ja pystysuunnassa, mutta se ei voi hyppiä nappuloiden
// yli. Sallitut liikkeet etsitään iteroimalla pelilaudan koordinaatteja vaaka- ja pystysuunnassa.
// Mikäli kohdalle osuu toinen nappula, row_piece ja col_piece muuttujat saavat arvon true eikä
// iterointia jatketa enää tähän suuntaan.
set<Coord> Rook::get_allowed_moves(const ChessBoard &board) const {

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

    return moves;
}
