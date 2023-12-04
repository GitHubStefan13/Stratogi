#ifndef ABSTRACT_GAME_HPP
#define ABSTRACT_GAME_HPP

// Qt include.
#include <QObject>
#include <QProcess>
#include <QThread>
#include <QVector>
#include <QStack>

// Chess include.
#include "abstractBoard.hpp"
#include "abstractFigure.hpp"
#include "abstractEngine.hpp"
#include "signals.hpp"

// C++ include.
#include <stdexcept>

namespace Stratogi {

  typedef struct  {
    uint32 x;
    uint32 y;
  } COORD_POSITION;

  typedef struct {
    uint32_t piece;             //! The Piece-Identifier that made the move, for example: P:Pawn, Q:Queen
    COORD_POSITION origin;      //! The Origin of the Piece for example 11, for the Position x:1 y:1
    uint32_t movement;          //! The Movement Type, for example "-" = simple Movement, "X" = defeat a piece
    COORD_POSITION destination; //! The destination the Piece will be moving to
    uint32_t transformation;    //! The Transformation the Piece performed
    uint32_t extra;             //! Some optional extra that can be set
  } COORD_NOTATION;

  //! Game.
  class AbstractGame :	public QObject
  {
    Q_OBJECT

  public:
    AbstractGame(AbstractBoard &board, Signals &s);
    virtual ~AbstractGame();

    AbstractEngine *engineThread() const;

    //! Returns the number of turns made so far
    virtual Q_INVOKABLE int turnsCount() const = 0;
    //! Set if we want to use the Engine or not
    Q_INVOKABLE void useEngine(bool useEngine = true);
    //! The number of defeatedPiece, depending on the PieceName
    //! @example defeatedPieceCount("Pawn") would return 2 for two defeated Pawns for White OR Black
    Q_INVOKABLE int defeatedPieceCount(QString pieceName);

  public Q_SLOTS:
    //! Clicked.
    void clicked(int x, int y );
    //! Hovered.
    void hovered(int x, int y );
    //! Transformation.
    void transformation(int figure, int c, int x, int y);
    //! New game.
    virtual void newGame();
    //! Game started
    virtual void gameStarted();
    //! Turn ended.
    virtual void turnEnded() = 0;
    //! Undo.
    virtual void undo() = 0;
    //! The Result we receive from the Engine
    virtual void onEngineResult(QString result) = 0;
  Q_SIGNALS:
    //! CPU or Player gave up and surrendered
    void surrender();

  protected:
    //! Lock the Board from UserInteraction
    bool m_guiLocked = false;

    //! Clear cells color.
    void clearCellsColor();
    //! Mark turn.
    void markTurnLabel();
    //! Tell the GUI that the Engine is calculating a Move
    void waitForEngine(const bool &ready);
    //! Mark cells for move.
    virtual void markCellsForMove(uint32 x, uint32 y, int32_t dx, int32_t dy, Move::Distance d, AbstractFigure * figure, AbstractBoard &tmpBoard);
    //! Mark cell for hit.
    virtual void markCellForHit(uint32 x, uint32 y, int32_t dx, int32_t dy, Move::Distance d, AbstractFigure * figure, AbstractBoard &tmpBoard);
    //! Check check.
    virtual bool checkCheck(bool checkmate = false);
    //! Mark check.
    virtual bool markCheck(King *king, AbstractFigure *figure, bool checkmate = false);
    //! Will be a check after move?
    virtual bool isCheckAfterMove(uint32 x, uint32 y, AbstractFigure * figure, AbstractBoard &tmpBoard) const;
    //! Check checkmate.
    virtual bool isCheckMate();
    //! Check stalemate.
    virtual bool isStaleMate();
    //! Would moving to X-Y Coord be a Capture/Defeat the Piece?
    virtual bool isCapture(uint32 x1, uint32 y1, uint32 x2, uint32 y2);
    //! First click.
    virtual void firstClick(const uint32 x, const uint32 y, void *user = nullptr);
    //! Second click.
    //! \attention YOU HAVE TO CALL THIS IN THE DERIVED FUNCTION!
    //!            m_possibleMoves.clear()           // ALWAYS
    //!            m_selected = nullptr              // ALWAYS
    //!            m_selected->firstMoveDone();      // WHEN THE FIRST MOVE IS DONE
    virtual bool secondClick(const uint32 x, const uint32 y, void *user = nullptr) = 0;

  protected:
    //! Board.
    AbstractBoard &m_board;
    //! Color of turn.
    AbstractFigure::FigureColor m_turnColor;
    //! The turn Color the Engine/Second-Player is using
    AbstractFigure::FigureColor m_engineColor;
    //! Selected figure.
    AbstractFigure *m_selected;
    //! Selected X.
    uint32 m_selectedX;
    //! Selected Y.
    uint32 m_selectedY;
    //! Possible moves. \attention This only works for boards up to 9*9 Cells
    QVector<uint32> m_possibleMoves;
    //! Is King in check?
    bool m_isKingCheck;
    //! Signals.
    Signals &m_signals;
    //! Checkmate flag.
    bool m_checkmate;
    //! A Map of the deafeated Pieces the Subclass will write stuff after EACH TURN
    QMap<QString, int> m_defeatedPieces;

    //! Only set when we want to play against an Engine
    bool m_useEngine;
    //! Is the Move made by the Engine? ONLY TRUE if m_useEngine is also set
    bool isEngineMove;
    //! The Thread of the Engine, that is running, NULL if no engine is running
    AbstractEngine *m_engineThread = nullptr;

    //! Inverse coord using size,
    //! @example Size = 9, Move is 1 then the Inverse is 9
    //! @example Size = 9, Move is 4 then the Inverse is 5
    void inverseAlphanumeric(uint32_t &value,  uint32_t size);
    QString AlphanumericToAlphabetical(uint32_t value);
    uint32_t AlphabeticalToAlphanumeric(QChar value);

    //! Convert an Engine Move (for example d2d4 to our Standard Coord-Notation
    virtual COORD_NOTATION convertEngineMove(QString engineMove) = 0;
    //! Convert a Display Move(for example e4 to our Standard Coord-Notation)
    virtual COORD_NOTATION convertDisplayMove(QString displayMove) = 0;
    //! Convert our Coord-Notation to the Engine Move, that the Engine will understand
    virtual QString convertToEngineMove(COORD_NOTATION pos) = 0;
    //! Convert our Coord-Notation to the Display Move, that the User will see
    virtual QString convertToDisplayMove(COORD_NOTATION pos) = 0;

    //! Calculate and Map the defeated Pieces to :m_defeatedPieces
    virtual void searchDefeatedPieces() = 0;

  private Q_SLOTS:
    void onEngineOptionChanged(OptionSetting *option);
  }; // class Game

} // END NAMEPSACE "Stratogi"

#endif // ABSTRACT_GAME_HPP
