#ifndef ABSTRACT_BOARD_HPP
#define ABSTRACT_BOARD_HPP

#include "abstractFigure.hpp"
#include "signals.hpp"

// Qt include.
#include <QList>
#include <QDebug>
#include <QSharedPointer>
#include <QAbstractListModel>

namespace Stratogi {

  //! Board.
  class AbstractBoard :	public QAbstractListModel
  {
    Q_OBJECT

    Q_PROPERTY(QString moves READ getMovesAsString NOTIFY movesChanged);
  public:
    AbstractBoard(uint32 rowSize, uint32 columnSize);
    AbstractBoard(const AbstractBoard &abstractBoard);
    AbstractBoard & operator = (const AbstractBoard & other);
    virtual ~AbstractBoard() override;

    enum RoleNames {
      CellImageSourceRole   = Qt::UserRole,
      CurrentPieceColorRole = CellImageSourceRole   + 1,
      BluePieceColorRole    = CurrentPieceColorRole + 1,
      RedPieceColorRole     = BluePieceColorRole    + 1,
      PieceColorRole        = RedPieceColorRole     + 1,
      BorderColorRole       = PieceColorRole        + 1
    }; // enum RoleNames

    using FiguresOnBoard = std::vector<std::vector<AbstractFigure*>>;

    //! \return Figures on board.
    //! \note Returned type is two-dimensinal array and indexes are
    //! as in usual C array.
    FiguresOnBoard &figures();

    //! \return Figure by index in the array.
    AbstractFigure* figure(uint32 index) const;

    //! \return Figure at the board Position x, y - Starting at the Top Left
    //! \attention Will return a nullptr if there is no Figure at the Position
    AbstractFigure* figure(uint32 x, uint32 y) const;

    //! Make a move.
    virtual void move(uint32 fromX, uint32 fromY, uint32 toX, uint32 toY, bool testMove = false);

    //! Mark blue.
    void markBlue(uint32 x, uint32 y);
    //! Mark red.
    void markRed(uint32 x, uint32 y);
    //! Mark check.
    void markCheck(uint32 x, uint32 y);

    //! Get the Color at the Position x, y - getter for m_colors
    int getColor(uint32 x, uint32 y);

    //! Get all the Moves from the MoveList
    QStringList getMoves();
    //!
    QString getMovesAsString();
    //! Add Move to the MoveList in the Game Notation
    void appendMove(QString move);
    //! Removes the Last move in the Movelist, only works if not empty
    void removeLastMove();

    //! Clear blue|red.
    virtual void clearColors() = 0;

    //! \return White king.
    AbstractFigure* whiteKing() const;
    //! \return Black king.
    AbstractFigure* blackKing() const;

    int rowCount(const QModelIndex & parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    //! Attributes
    uint32 _rowSize;      // The Row Size of the Abstract Board, will be set from the Derived Class
    uint32 _columnSize;   // The Column Size of the Abstract Board, will be set from the Derived Class
  public Q_SLOTS:
    //! New game.
    virtual void newGame();
    //! Update.
    virtual void update();
    //! Update a specific Cell
    virtual void update(uint32_t x, uint32_t y);
    //! Transformation.
    virtual void transformation(int figure, int c, int x, int y) = 0;

  Q_SIGNALS:
    void movesChanged();

  protected:
    //! Init figures.
    virtual QList<QSharedPointer<AbstractFigure>> initFigures() const = 0;
    //! Copy state of the board.
    virtual void copyState(const AbstractBoard &other);

    //! Board.
    FiguresOnBoard m_board;
    //! Figures.
    QList<QSharedPointer<AbstractFigure>> m_figures;
    //! Transformed figures.
    QList<QSharedPointer<AbstractFigure>> m_transformed;
    //! White king.
    AbstractFigure* m_whiteKing;
    //! Black king.
    AbstractFigure* m_blackKing;

    typedef std::vector<std::vector<int>> Colors;

    //! Cells' marks.
    Colors m_colors;

  private:
    //! Move List of every taken Move
    QStringList m_moves;

  }; // class Board

} // END NAMEPSACE "Stratogi"

#endif // ABSTRACT_BOARD_HPP
