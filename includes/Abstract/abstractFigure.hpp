#ifndef ABSTRACT_FIGURE_HPP
#define ABSTRACT_FIGURE_HPP

#include "ctypes.h"
#include "figuretypes.h"

// Qt include.
#include <QFlags>
#include <QSharedPointer>

// C++ include.
#include <array>

namespace Stratogi {

//! Move.
//! \brief What kind of move we can do
//!        the Distance is how many fields we an move in one MOVE
//!        the Type is if we can only move or also do a hit to BEAT a Piece at that Position
class Move {

  public:
    //! Type of movement.
    enum Type {
      Unknown  = 0x00,
      Movement = 0x01,
      Hit      = 0x02,
      MoveHit  = 0x03   // Just a combination of Movement and Hit, since its just alot
    }; // enum Type
    using Types = int; // For Flags with the Type Enum

    //! Distance.
    enum Distance {
      No,
      One,
      Any,
      TwoFirstTime
    }; // enum Distance

    Move( Types t, Distance d );
    ~Move();

    //! \return Types.
    Types types() const;

    //! \return Distance.
    Distance dist() const;

  private:
    //! Types.
    Types m_types;
    //! Distance.
    Distance m_dist;
};

//! Abstractfigure on the board.
class AbstractFigure {

  public:

    //! The Color of the Figures, all used Boards only use White and Black,
    //! can be extended if needed
    enum FigureColor {
      White,
      Black
    };

    AbstractFigure(uint32 xv, uint32 yv, FigureColor c, const QString & name, uint32 index);
    virtual ~AbstractFigure();

    //! Moves.
    //! \brief With this we can define all Possible Moves in Grid with the Size 25(for now)
    //!
    //! \example We currently have 5 rows, with 5 columns, the figure is in the Middle
    //!          "A" stands for ANY MOVE,
    //!          "M" stands for single MOVE, depending on the Type-Flag it can also be a HIT!
    //!          "X" stands for no move
    //!          "O" stands for the Position of the Figure
    //!          "H" Only shown when the Figure can only do a HIT but not actually Move(Pawn Pass)
    //!
    //! Figure:          Rook:                 Knight:                    Bishop               Pawn
    //!   1.Row        X-X-A-X-X              X-M-X-M-X                  A-X-X-X-A           X-X-X-X-X
    //!   2.Row        X-X-A-X-X              M-X-X-X-M                  X-A-X-A-X           X-H-M-H-X
    //!   3.Row        A-A-O-A-A              X-X-O-X-X                  X-X-O-X-X           X-X-O-X-X
    //!   4.Row        X-X-A-X-X              M-X-X-X-M                  X-A-X-A-X           X-X-X-X-X
    //!   5.Row        X-X-A-X-X              X-M-X-M-X                  A-X-X-X-A           X-X-X-X-X
    //!   IMPORTANT!
    //!      "ANY" only needs to be defined in the line it starts, usually thats next to the Position of the Figure
    //!
    //!
    //! \attention The Start Position e.g where the First Row Starts is defined in the Method:move(...)
    //!            So with the example of 5Row we start from the "AktPos - 2" e.g "2 - AktPos" depending
    //!            on the Direction(Start from Top, Start from Bottom)
    //!
    static const int MOVES = 5;
    using Moves = std::array<std::array<Move, MOVES>, MOVES>;

    //! \return Possible moves.
    //! \brief Every Figure has certain possible moves which they have to define by themselves
    //!        This needs some explanations, first read the Information about the typedef MOVES
    //!
    //!        Every Figure will define their own MOVES so we can display and mark it accordingly
    //!        if this needs extension for other Modes feel free to extend Moves. \see @MOVES@
    virtual const Moves & moves() const = 0;

    //! \return Type of figure.
    virtual uint32_t type() const = 0;

    //! \return The Object Figure;
    virtual AbstractFigure* figure();

    //! \return True if this figure is THE King Figure
    //! Should be overwritten by the derived King Class
    virtual bool isKing();

    //! \return Index in the array.
    uint32 index() const;

    //! \return Move-Type of the Figure.
    uint32 move_type() const;
    //! Set Move-Type.
    void setMoveType(uint32 value);

    //! \return Transformation-Type of the Figure.
    uint32 transformation_type() const;
    //! Set Move-Type.
    void setTransformationType(uint32 value);

    //! \return X.
    uint32 x() const;
    //! Set X.
    void setX(uint32 value );

    //! \return Y.
    uint32 y() const;
    //! Set Y.
    void setY(uint32 value );

    //! \return Color.
    FigureColor color() const;

    //! \return Name.
    const QString & name() const;

    //! \return Is first move done?
    bool isFirstMoveDone() const;
    //! Mark figure as did first move.
    void firstMoveDone(bool done = true);
    //! \return Is Figure transformed?
    bool isTransformed() const;
    //! Mark figure as Transformed or not.
    void transform(bool transform = true);
    //! \return Can we Promote?
    bool isAllowedTransformation() const;
    //! Mark figure to be allowed to Promote
    void allowTransformation(bool transform = true);
    //! Set a new Color for a Figure, they can change Sides in some Games(Shogi...)
    void setColor(FigureColor newColor);

    //! Copy.
    virtual QSharedPointer<AbstractFigure> copy() const = 0;

  private:
    //! X.
    uint32 m_x;
    //! Y.
    uint32 m_y;
    //! Color.
    FigureColor m_c;
    //! Name.
    QString m_name;
    //! Is the Figure transformed?
    bool m_transformed;
    //! First move has been done.
    bool m_firstMoveDone;
    //! Are we allowed to Promote? Default is YES
    bool m_canTransform;
    //! Index in the array.
    uint32 m_index;
    //! Move-Type of the Current Figure/Move.
    uint32 m_move_type;
    //! Transformation-Type of the Current Figure/Move.
    uint32 m_transformation_type;
};

//! King. Every Strategy Board has some Form of King, thats why I put into this file
class King Q_DECL_FINAL :	public AbstractFigure
{
  public:
    King(uint32 xv, uint32 yv, FigureColor c, const QString & n, uint32 index);
    virtual ~King() override;

    //! \return Possible moves.
    const Moves & moves() const override;

    //! \return Type of figure.
    uint32_t type() const override;

    //! \return This will always return true, since we are THE KING
    bool isKing() override;

    //! Copy.
    QSharedPointer<AbstractFigure> copy() const override;

  private:
    static const Moves m_moves;

};

} // End Namespace Stratogi
#endif // ABSTRACT_FIGURE_HPP
