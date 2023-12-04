#ifndef SHOGIBOARD_H
#define SHOGIBOARD_H

#include <QColor>
#include <QAbstractListModel>
#include <QDebug>

#include "abstractBoard.hpp"
#include "shogiTable.h"


namespace Stratogi {
namespace Shogi {

  class ShogiBoard Q_DECL_FINAL: public AbstractBoard
  {
    Q_OBJECT

    public:
      ShogiBoard();
      ShogiBoard(const ShogiBoard &shogiBoard);

      static const uint32 ROWS    = 9;
      static const uint32 COLUMNS = 9;
      static const uint32 INVALID_MOVE = 99999;

      //! ShogiTable Top
      ShogiTable table_top;
      //! ShogiTable Bottom
      ShogiTable table_bot;

      ShogiBoard &operator = (const ShogiBoard &other);

      typedef AbstractFigure* FiguresOnBoard[8][8];

      //! Makes a move on the Board, includes the Tables in Shogi
      void move(uint32 fromX, uint32 fromY, uint32 toX, uint32 toY, bool testMove = false) override;
      //! Clear Board Colors, for example after we marked the Board for hits
      void clearColors() override;
      //! Starts a new Game and clears the old game
      void newGame() override;
      //! Transforms a Piece in Shogi it would be a Promototion
      void transformation(int figure, int c, int x, int y) override;
      //! Updates the entire Board
      void update() override;
      //! Updates a Single Cell at the given Position
      void update(uint32_t x, uint32_t y) override;
      //! A Figure got defeated
      void defeated(AbstractFigure *figure);
      //! Data at the given Index, tells QML information about the Board and so on...
      QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

    Q_SIGNALS:
      //! Signal send once the User confirms or declines the Promotion
      void promotionConfirmed(bool accepted);

    protected:
      QList<QSharedPointer<AbstractFigure>> initFigures() const override;
      void copyState( const AbstractBoard &other) override;

    private:
      void tableMove(uint32 fromX, uint32 fromY, uint32 toX, uint32 toY, ShogiTable *table);
  };

} // END NAMESPACE Shogi
} // END NAMEPSACE "Stratogi"

#endif // SHOGIBOARD_H
