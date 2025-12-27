/*
 *
 * Program author
 * Name: Suvi Ketola
 * Student number: 151864674
 * UserID: hdsuke
 * E-Mail: suvi.ketola@tuni.fi
 * Feedback language (fi/en): fi
 *
 * Tämä ohjelma toteuttaa graafisen käyttöliittymän shakkipelille, jossa pelilauta ja nappulat
 * on esitetty käyttäen Qt-kirjastoa. Pelilauta on toteutettu 8x8 ruudukkona, jossa kukin
 * ruutu on painike (QPushButton), jolle on annettu ruudun väriä ja siinä olevaa nappulaa
 * vastaava kuva. Nappulat on esitetty kuvakkeilla ja käyttäjä voi pelata
 * shakkia siirtämällä nappuloita hiirellä, jolloin painikkeiden kuvat (icons) määritetään
 * uudelleen. Samassa yhteydessä tarkastetaan, onko ruutu, johon siirrytään musta vai valkoinen
 * ja uusi kuva asetetaan tämän mukaan.
 *
 * Peli tarjoaa seuraavat toiminnot:
 * - Valkoisen ja mustan vuorot vuorotellevat. Vain vuorossa olevia nappuloita voi liikuttaa.
 * - Käyttäjä voi siirtää nappuloita: ensimmäisellä klikkauksella valitaan nappula ja toisella
 *   valitaan kohderuutu. Jos siirto on sallittu, nappula siirtyy ja vuoro vaihtuu. Vain
 *   sallitut siirrot ovat mahdollisia ja pelilaudan muut painikkeet ovat tällöin disabled
 *   -tilassa. Mikäli siirron haluaa perua, voi nappulaa klikata uudelleen jolloin vuoro säilyy samalla pelaajalla
 * - Pelissä voidaan aloittaa uusi peli tai sulkea sovellus.
 * - Kun pelaaja voittaa (syö kuningas), peli päättyy ja voittaja ilmoitetaan.
 * - Nappuloiden siirrot toteutetaan vaihtelemalla painikkeiden kuvakkeita
 *
 */


#include "mainwindow.hh"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Pelin aloittaminen
    newGame();

    // Vuorojen näyttäminen käyttöliittymässä
    turnLabel = new QLabel("White's turn", this);
    turnLabel->setGeometry(50, 400, 100, 50);
    turnLabel->show();

    // Uusi peli
    QPushButton* newGameButton = new QPushButton(this);
    newGameButton->setGeometry(50, 450, 100, 50);
    newGameButton->setText("New game");
    newGameButton->show();
    connect(newGameButton, &QPushButton::clicked, this, &MainWindow::resetGame);

    // Poistu painike
    QPushButton* quitButton = new QPushButton(this);
    quitButton->setGeometry(50, 500, 100, 50);
    quitButton->setText("Quit");
    quitButton->show();
    connect(quitButton, &QPushButton::clicked, this, &MainWindow::quitGame);

}

// Etsii painikkeen koordinaatin boardSquares vektorista ja tarkastaa, onko sijainnissa nappulaa
// Mikäli painike sisältää nappulan:
// - ensimmäisellä painalluksella vain ruudut, joihin on mahdollista siirtyä enabled
// - toisella painalluksella tarkistetaan, onko ruutuun mahdollista siirtyä. Mikäli
//   näin on, vaihdetaan ruutujen kuvakkeet keskenään.
void MainWindow::buttonClick() {

    QPushButton* clickedButton = qobject_cast<QPushButton*>(sender()); // Painettu ruutu

    // Etsitään koordinaatti
    int clickedRow = -1, clickedCol = -1;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (boardSquares[row][col] == clickedButton) {
                clickedRow = row;
                clickedCol = col;
                break;
                }
            }
        }

    // Kun nappulaa klikataan
    if (!isSelected && !gameOver) {

        auto piece = board_.get_piece_at(clickedRow, clickedCol);

        // Ruudun ollessa valittuna ja mikäli siinä on nappula väritetään mahdolliset siirrot reunoilta punaisiksi
        if (piece!=nullptr && piece->get_color() == currentTurn_) {

            // Haetaan siirrot ja state muuttujinen päivitys
            allowedMoves = piece->get_allowed_moves(board_);
            std::set<std::pair<int, int>> allowedSquares;
            allowedSquares.insert({clickedRow , clickedCol});
            selectedCoord = {clickedRow, clickedCol};
            selectedButton = clickedButton;
            selectedPiece = piece;

            // Asetetaan vain mahdolliset siirrot mahdollisiksi klikata laudalla
            for (const auto& i : allowedMoves) {
                boardSquares[i.row][i.col]->setEnabled(true);
                allowedSquares.insert({i.row, i.col});
            }

            // Muut napit disabled
            for (int r = 0; r < 8; ++r) {
                for (int c = 0; c < 8; ++c) {
                    if (allowedSquares.find({r, c}) == allowedSquares.end()) {
                        boardSquares[r][c]->setEnabled(false);
                    }
                }
            }
            isSelected = true;
        }
    }
    else if (!gameOver) {

        // Yritetään liikkua
        Coord target = {clickedRow, clickedCol};
        if (allowedMoves.find(target) != allowedMoves.end()) {

            // Siirretään nappulaa GUI:ssa
            swapIcons(selectedButton, selectedCoord.row, selectedCoord.col,
                      clickedButton, target.row, target.col, selectedPiece->get_type(),
                      selectedPiece->get_color());


            // Tarkistetaan, onko kuningas syöty ja tulostetaan mahdollinen voittaja
            auto targetPiece = board_.get_piece_at(target);
            if (targetPiece!=nullptr && targetPiece->get_type()==KING) {
                if (currentTurn_ == WHITE) {
                    turnLabel->setText("White won!");
                } else {
                    turnLabel->setText("Black won!");
                }
                // Painikkeet disablet
                for (int r = 0; r < 8; ++r) {
                    for (int c = 0; c < 8; ++c) {
                        boardSquares[r][c]->setEnabled(false);
                    }
                }
                gameOver = true;
            }
            // Mikäli peli ei ole päättynyt:
            else {
                // Vaihdetaan vuoroa
                switchTurn();
                if (currentTurn_ == WHITE) {
                    turnLabel->setText("White's turn");
                } else {
                    turnLabel->setText("Black's turn");
                }
            }

            // Päivitetään pelilauta
            board_.set_piece(clickedRow, clickedCol, selectedPiece);
            board_.set_piece(selectedCoord.row, selectedCoord.col, nullptr);
            selectedPiece->set_position(target);

        }

        // Reset GUI
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                boardSquares[r][c]->setEnabled(true);
            }
        }

        // Reset state muuttujat
        isSelected = false;
        selectedButton = nullptr;
        selectedPiece = nullptr;
        allowedMoves.clear();

    }
}

// Asettaa pelilaudalla nappulan annettuun kohtaan. Parametrina painike, koordinaatit, nappulantyyppi ja väri.
// Ei käytetä sotilaille
void MainWindow::placeIcon(QPushButton* button, int row, int col,
                           PieceType pieceType, ChessColor color) {

    QString type = pieceTypeToString(pieceType);

    QString colorPrefix = (color == WHITE) ? "wt" : "bl";
    QString squareColor = ((row + col) % 2 != 0) ? "wt" : "bl";

    QString imagePath = QString(":pieces/%1-%2-on-%3.png")
                        .arg(colorPrefix)
                        .arg(type)
                        .arg(squareColor);

    QPixmap picture;
    picture.load(imagePath);
    button->setIcon(picture);
    button->setIconSize(QSize(44, 44));
}

// Muuntaa nappulan tyypin merkkijonoksi. Apufunktio. Parametrina nappulan tyyppi
QString MainWindow::pieceTypeToString(PieceType type) {
    switch (type) {
        case KING: return "king";
        case QUEEN: return "queen";
        case BISHOP: return "bishop";
        case KNIGHT: return "knight";
        case ROOK: return "rook";
        case PAWN: return "pawn";
        default: return "unknown";
    }
}

// Asettaa painikkeelle uuden kuvan sen mukaan, onko kyseessä oleva ruutu musta vai valkoinen. Parametrina
// lähtö ja kohde painikkeet, näiden koordinaatit laudalla ja siirrettävän nappulan tyyppi ja väri
void MainWindow::swapIcons(QPushButton* from, int fromRow, int fromCol,
                           QPushButton* to, int toRow, int toCol,
                           PieceType pieceType, ChessColor color) {

    QString type = pieceTypeToString(pieceType);

    QString colorPrefix = (color == WHITE) ? "wt" : "bl";
    QString tosquareColor = ((toRow + toCol) % 2 != 0) ? "wt" : "bl";
    QString fromsquareColor = ((fromRow + fromCol) % 2 != 0) ? "wt" : "bl";

    // Rakennetaan kuvien tiedostopolut
    QString imagePath = QString(":pieces/%1-%2-on-%3.png")
                        .arg(colorPrefix)
                        .arg(type)
                        .arg(tosquareColor);

    QString fromImagePath = QString(":pieces/empty-%1.png")
                        .arg(fromsquareColor);

    QPixmap picture;
    picture.load(imagePath);
    to->setIcon(picture);
    picture.load(fromImagePath);
    from->setIcon(picture);
    to->setIconSize(QSize(44, 44));
    from->setIconSize(QSize(44, 44));
}

// Vaihtaa vuoroa: mikäli vuorossa on valkoinen, vaihdetaan mustaksi ja toisin päin
void MainWindow::switchTurn() {
    currentTurn_ = (currentTurn_ == WHITE) ? BLACK : WHITE;
}

// Lopettaa pelin
void MainWindow::quitGame() {
    QApplication::quit();
}

// Aloittaa uuden pelin luomalla uuden chessBoard() instanssin sekä asettaa GUI:n alkutilaan
void MainWindow::newGame() {

    QPixmap picture;

    // 8X8 ruudukon luominen shakkilaudan pohjaksi
    boardSquares.resize(8);
    for (int i=0; i<8; i++) { //row

        boardSquares[i].resize(8);
        for (int j=0; j<8; j++) { //col

            QPushButton* square = new QPushButton(this); // Osoitin uuteen ruutuun

            // Musta ruutu, jos molemmat koordinaatit samaan aikaan parillisia tai parittomia
            if ((i%2!=0 && j%2!=0) || (i%2==0 && j%2==0)) {
                picture.load(":pieces/empty-bl.png");}

            // Valkoinen ruutu muussa tapauksessa
            else {
                picture.load(":pieces/empty-wt.png");}

            square->setIcon(picture);
            square->setIconSize(QSize(44, 44));
            square->setGeometry(50*i, 50*j, 50, 50); // x, y, leveys, korkeus
            square->show();

            boardSquares[i][j] = square;

            // Yhdistetään buttuonClick-metodiin
            connect(square, &QPushButton::clicked, this, &MainWindow::buttonClick);
        }
    }


    // Luodaan nappulat ja asetetaan ne alkuasemiin ja yhdistetään GUI:hin
    // Sotilaat
    for (int i = 0; i<8; i++) {

        if (i%2!=0) {
            picture.load(":pieces/wt-pawn-on-bl.png");
        }
        else {
            picture.load(":pieces/wt-pawn-on-wt.png");
        }

        std::string pawnWhite = "pawnWhite" + std::to_string(i);
        pieceMap[pawnWhite] = std::make_shared<Pawn>(WHITE);

        //Sijainti laudalla
        board_.set_piece(1, i, pieceMap[pawnWhite]);
        pieceMap[pawnWhite]->set_position({1, i});
        // Sijainti GUI:ssa
        boardSquares[1][i]->setIcon(picture);
        boardSquares[1][i]->setIconSize(QSize(44, 44));

        if (i%2!=0) {
            picture.load(":pieces/bl-pawn-on-wt.png");
        }
        else {
            picture.load(":pieces/bl-pawn-on-bl.png");
        }

        std::string pawnBlack = "pawnBlack" + std::to_string(i);
        pieceMap[pawnBlack] = std::make_shared<Pawn>(BLACK);
        //Sijainti laudalla
        board_.set_piece(6, i, pieceMap[pawnBlack]);
        pieceMap[pawnBlack]->set_position({6, i});
        //Sijainti GUI:ssa
        boardSquares[6][i]->setIcon(picture);
        boardSquares[6][i]->setIconSize(QSize(44, 44));
    }

    // Kuninkaat
    placeIcon(boardSquares[0][4], 0, 4, KING, WHITE);
    pieceMap["kingW"] = std::make_shared<King>(WHITE);
    pieceMap["kingW"]->set_position({0,4});
    board_.set_piece(0, 4, pieceMap["kingW"]);

    placeIcon(boardSquares[7][4], 7, 4, KING, BLACK);
    pieceMap["kingB"] = std::make_shared<King>(BLACK);
    pieceMap["kingB"]->set_position({7,4});
    board_.set_piece(7, 4, pieceMap["kingB"]);

    // Tornit
    placeIcon(boardSquares[0][0], 0, 0, ROOK, WHITE);
    pieceMap["rookW1"] = std::make_shared<Rook>(WHITE);
    pieceMap["rookW1"]->set_position({0,0});
    board_.set_piece(0, 0, pieceMap["rookW1"]);

    placeIcon(boardSquares[0][7], 0, 7, ROOK, WHITE);
    pieceMap["rookW2"] = std::make_shared<Rook>(WHITE);
    pieceMap["rookW2"]->set_position({0,7});
    board_.set_piece(0, 7, pieceMap["rookW2"]);

    placeIcon(boardSquares[7][0], 7, 0, ROOK, BLACK);
    pieceMap["rookB1"] = std::make_shared<Rook>(BLACK);
    pieceMap["rookB1"]->set_position({7,0});
    board_.set_piece(7, 0, pieceMap["rookB1"]);

    placeIcon(boardSquares[7][7], 7, 7, ROOK, BLACK);
    pieceMap["rookB2"] = std::make_shared<Rook>(BLACK);
    pieceMap["rookB2"]->set_position({7,7});
    board_.set_piece(7, 7, pieceMap["rookB2"]);

    // Ratsut
    placeIcon(boardSquares[0][1], 0, 1, KNIGHT, WHITE);
    pieceMap["knightW1"] = std::make_shared<Knight>(WHITE);
    pieceMap["knightW1"]->set_position({0,1});
    board_.set_piece(0, 1, pieceMap["knightW1"]);

    placeIcon(boardSquares[0][6], 0, 6, KNIGHT, WHITE);
    pieceMap["knightW2"] = std::make_shared<Knight>(WHITE);
    pieceMap["knightW2"]->set_position({0,6});
    board_.set_piece(0, 6, pieceMap["knightW2"]);

    placeIcon(boardSquares[7][6], 7, 6, KNIGHT, BLACK);
    pieceMap["knightB1"] = std::make_shared<Knight>(BLACK);
    pieceMap["knightB1"]->set_position({7,6});
    board_.set_piece(7, 6, pieceMap["knightB1"]);

    placeIcon(boardSquares[7][1], 7, 1, KNIGHT, BLACK);
    pieceMap["knightB2"] = std::make_shared<Knight>(BLACK);
    pieceMap["knightB2"]->set_position({7,1});
    board_.set_piece(7, 1, pieceMap["knightB2"]);

    // Lähetit
    placeIcon(boardSquares[0][2], 0, 2, BISHOP, WHITE);
    pieceMap["bishopW1"] = std::make_shared<Bishop>(WHITE);
    pieceMap["bishopW1"]->set_position({0,2});
    board_.set_piece(0, 2, pieceMap["bishopW1"]);

    placeIcon(boardSquares[0][5], 0, 5, BISHOP, WHITE);
    pieceMap["bishopW2"] = std::make_shared<Bishop>(WHITE);
    pieceMap["bishopW2"]->set_position({0,5});
    board_.set_piece(0, 5, pieceMap["bishopW2"]);

    placeIcon(boardSquares[7][2], 7, 2, BISHOP, BLACK);
    pieceMap["bishopB1"] = std::make_shared<Bishop>(BLACK);
    pieceMap["bishopB1"]->set_position({7,2});
    board_.set_piece(7, 2, pieceMap["bishopB1"]);

    placeIcon(boardSquares[7][5], 7, 5, BISHOP, BLACK);
    pieceMap["bishopB2"] = std::make_shared<Bishop>(BLACK);
    pieceMap["bishopB2"]->set_position({7,5});
    board_.set_piece(7, 5, pieceMap["bishopB2"]);

    // Kuningattaret
    placeIcon(boardSquares[0][3], 0, 3, QUEEN, WHITE);
    pieceMap["queenW"] = std::make_shared<Queen>(WHITE);
    pieceMap["queenW"]->set_position({0,3});
    board_.set_piece(0, 3, pieceMap["queenW"]);

    placeIcon(boardSquares[7][3], 7, 3, QUEEN, BLACK);
    pieceMap["queenB"] = std::make_shared<Queen>(BLACK);
    pieceMap["queenB"]->set_position({7,3});
    board_.set_piece(7, 3, pieceMap["queenB"]);

}



// Asettaa pelilaudan alkutilaan poistamalla painikkeet, asettamalla chessboard luokan
// board_ instanssin alkutilaan ja asettamalla pelin tilaa seuraavat muuttujat alkutilaan
void MainWindow::clearBoard() {
    // Poistetaan painikkeet ja asetetaan pelilauta alkutilaan
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            delete boardSquares[row][col];
            board_.set_piece(row, col, nullptr);
        }
    }
    // Muuttujien reset
    pieceMap.clear();
    board_ = ChessBoard();
    turnLabel->setText("White's turn");
    currentTurn_ = WHITE;
    isSelected = false;
    selectedButton = nullptr;
    selectedPiece = nullptr;
    allowedMoves.clear();
    gameOver = false;
}

// Aloittaa uuden pelin tyhjentämällä ensin pelilaudan ja lopettamalla nykyisen pelin
// kutsumalla clearBoard() funktiota, jonka
// jälkeen aloitetaan uusi peli kutsumalla newGame() funktiota.
void MainWindow::resetGame() {
    // Reset muuttujille
    clearBoard();
    newGame();
}

// Destruktori
MainWindow::~MainWindow()
{
    delete ui;
}
