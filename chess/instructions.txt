CHESS GUI C++ QT - A graphical chess game written in C++ using the Qt framework.

Final project for course "Programming 3" 2025 in the Tampere University. Chess piece images were provided as part of the course material.

ENG

This program is a graphical chess game in which two players can play against each other on the same computer. The game supports standard chess rules (without special moves such as castling or en passant).
The program is implemented using the Qt C++ library and includes a graphical user interface where pieces can be clicked and moved using the mouse.

1. Starting the Program
	1. Open the project in the Qt Creator development environment.
	2. Make sure that the resource file (e.g. pieces.qrc) contains all required image files for the chess pieces.
	3. Build and run the program (Ctrl + R or click Run).

2. User Interface Structure
- An 8×8 grid (board) with alternating black and white squares.
- Each square is a button (QPushButton) to which a piece icon is assigned.
- The color of the player whose turn it is is shown in the user interface
("White's turn" / "Black's turn").
- Only the player whose turn it is can move their pieces.

3. Starting the Game
When the game starts:
- All pieces are placed in their initial positions on the board.
- Players can start taking turns moving their own pieces. White moves first.

4. Moves and Clicks
- First click: Selects a square containing one of the player's own pieces.
- Possible moves are shown as enabled (other squares are disabled).
- Second click: Selects the target square. If the square is an allowed destination, the piece is moved there.
- A piece can only be moved to an allowed square.
- If the player does not want to move the selected piece, clicking the same piece again keeps the turn with the same player, allowing them to make a new move.

5. Win Condition
- If a player captures the opponent’s king, the game ends immediately.
- A message announcing the winner is displayed:
"White won!" or "Black won!"
- After this, all squares are set to a disabled state (they can no longer be clicked), and the game must be restarted (New game) or exited (Quit).

FIN

Tämä ohjelma on graafinen shakkipeli, jossa kaksi pelaajaa voi pelata toisiaan vastaan samalla koneella. 
Peli tukee tavallisia shakin sääntöjä (ilman erikoissiirtoja kuten linnoitus tai en passant).

Ohjelma on toteutettu Qt C++ -kirjastoa hyödyntäen, ja se sisältää graafisen käyttöliittymän, jossa nappuloita voi klikata ja siirtää hiirellä.

1. Ohjelman käynnistys
	1. Avaa projekti Qt Creator -kehitysympäristössä.
	2. Varmista, että resurssitiedosto (esim. pieces.qrc) sisältää kaikki tarvittavat kuvatiedostot shakkinappuloille.
	3. Käännä ja suorita ohjelma (Ctrl + R tai paina "Run").

2. Käyttöliittymän rakenne
- 8×8 ruudukko (lauta), jossa vuorottelevat mustat ja valkoiset ruudut.
- Jokainen ruutu on painike (QPushButton), johon asetetaan nappulan kuvake.
- Vuorossa olevan pelaajan väri näkyy käyttöliittymässä ("White's turn" / "Black's turn"). Vain vuorossa oleva pelaaja
voi siirtää nappuloitaan.

3. Pelin aloittaminen
Kun peli käynnistyy:
- Kaikki nappulat asetetaan aloitusasemaan laudalle.
- Pelaajat voivat aloittaa vuorotellen siirtämällä omia nappuloitaan. Valkoinen aloittaa.

4. Siirrot ja painallukset
- Ensimmäinen klikkaus: Valitsee ruudun, jossa on oma nappula. Mahdolliset siirrot näkyvät aktivoituna (muut ruudut estetään).
- Toinen klikkaus: Valitaan kohderuutu. Jos ruutu on sallittu siirtokohde, nappula siirtyy siihen. Nappulan
voi siirtää vain sallituun ruutuun.
- Mikäli valittua nappulaa ei halua siirtää, klikkaa samaa nappulaa uudelleen, jolloin vuoro säilyy samalla pelaajalla ja voi
tehdä uuden siirron. 

5. Voiton määrittely
- Jos pelaaja syö vastustajan kuninkaan, peli päättyy välittömästi.
- Näytölle tulee ilmoitus voittajasta: "White won!" tai "Black won!"
- Tämän jälkeen kaikki ruudut asetetaan disabled tilaan (ei voi enää painaa) ja peli tulee aloittaa uudelleen (New game) tai lopettaa (Quit).

