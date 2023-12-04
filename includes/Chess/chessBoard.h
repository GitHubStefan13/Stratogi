#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QColor>
#include <QAbstractListModel>

#include "abstractBoard.hpp"

#include "chessfigures.h"
#include "figuretypes.h"

namespace Stratogi {
namespace Chess {

  class ChessBoard Q_DECL_FINAL: public AbstractBoard
  {
    Q_OBJECT

    static const uint32 ROWS    = 8;
    static const uint32 COLUMNS = 8;

    public:
      ChessBoard();
      ChessBoard(const ChessBoard &chessBoard);
      ChessBoard &operator = (const ChessBoard & other);

      typedef AbstractFigure* FiguresOnBoard[8][8];

      //! Make move.
      void move(uint32 fromX, uint32 fromY, uint32 toX, uint32 toY, bool testMove = false) override;
      void clearColors() override;

      QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    public Q_SLOTS:
      void newGame() override;
      void transformation(int figure, int c, int x, int y) override;

    protected:
      QList<QSharedPointer<AbstractFigure>> initFigures() const override;
  };

} // END NAMESPACE chess
} // END NAMEPSACE "Stratogi"

#endif // CHESSBOARD_H
