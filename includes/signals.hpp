#ifndef SIGNALS_HPP
#define SIGNALS_HPP

// Qt include.
#include <QObject>

//! Signals.
namespace Stratogi {
class Signals :	public QObject
{
  Q_OBJECT

  static const int CHESS_TRANSFORMATION = 0; //! Offset for Transformation Figure, see @TransformationFigure
  static const int SHOGI_TRANSFORMATION = 4; //! Offset for Transformation Figure, see @TransformationFigure

  public:
    enum FigureColor{
      White = 0,
      Black = 0
    };

    Q_ENUM(FigureColor)

    enum TransformationFigure {
      // Chess Transformations
      Queen                  = CHESS_TRANSFORMATION,
      Rook                   = CHESS_TRANSFORMATION + 1,
      Knight                 = CHESS_TRANSFORMATION + 2,
      Bishop                 = CHESS_TRANSFORMATION + 3,
      // Offset of @CHESS_TRANSFORMATION + 4, since we need 4 Transformations

      // Shogi Transformations
      Promoted_Pawn          = SHOGI_TRANSFORMATION,
      Promoted_Lance         = SHOGI_TRANSFORMATION + 1,
      Promoted_Knight        = SHOGI_TRANSFORMATION + 2,
      Promoted_Rook          = SHOGI_TRANSFORMATION + 3,
      Promoted_Bishop        = SHOGI_TRANSFORMATION + 4,
      Promoted_SilverGeneral = SHOGI_TRANSFORMATION + 5,
      Promoted_GoldGeneral   = SHOGI_TRANSFORMATION + 6
      // Offset of @SHOGI_TRANSFORMATION + 7, since we need 7 Transformations

    }; // enum TransformationFigure

    Q_ENUM(TransformationFigure)

  Q_SIGNALS:
    //! Checkmate.
    void checkmate();
    //! Draw game.
    void drawgame();
    //! Transformation. Pawn Transformation in Chess, Figure Promotion in Shogi
    void transformation(int fcolor, int fx, int fy);
    //! Turn made.
    void turnMade();
    //! Once the Engine/Board finished loading
    void finishedLoading();
    //! No more undo.
    void noMoreUndo();
    //! Invalid Move.
    void invalidMove();

  public:
    Signals()
    {
    }

    virtual ~Signals()
    {
    }

    Q_INVOKABLE bool getGameState(){return _gameState;}
    Q_INVOKABLE void setGameState(bool gameState){_gameState = gameState;}

  private:
    bool _gameState;
}; // class Signals
} // END NAMEPSACE "Stratogi"

#endif // SIGNALS_HPP

