#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <string>
#include <iostream>
#include <algorithm>

#include "chesspiece.hh"
#include "pawn.hh"
#include "king.hh"
#include "rook.hh"
#include "knight.hh"
#include "bishop.hh"
#include "queen.hh"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

private:
    Ui::MainWindow *ui;

    // Luodaan pelilauta ChessBoard luokan avulla
    ChessBoard board_;
    // Ruutujen tallennus vektoriin
    QVector<QVector<QPushButton*>> boardSquares;

    // Nappulat tänne
    std::map<std::string, std::shared_ptr<ChessPiece>> pieceMap;

    // Pitää kirjaa, onko nappi valittuna
    bool isSelected = false;
    QPushButton* selectedButton = nullptr;
    Coord selectedCoord;
    std::shared_ptr<ChessPiece> selectedPiece = nullptr;
    std::set<Coord> allowedMoves;

    // Pitää kirjaa vuoroista. Aloitusvuoro valkoisella
    ChessColor currentTurn_ = WHITE;
    // Näyttää GUI:ssa kumman vuoro on
    QLabel* turnLabel;
    // Onko peli päättynyt
    bool gameOver = false;

private slots:
    void buttonClick();
    void swapIcons(QPushButton* from, int fromRow, int fromCol,
                   QPushButton* to, int toRow, int toCol,
                   PieceType pieceType, ChessColor color);
    void switchTurn();
    void newGame();
    void quitGame();
    void clearBoard();
    void resetGame();
    QString pieceTypeToString(PieceType type);
    void placeIcon(QPushButton* button, int row, int col,
                               PieceType pieceType, ChessColor color);
};

#endif // MAINWINDOW_HH
