#ifndef SHOGITABLE_H
#define SHOGITABLE_H

#include <QColor>
#include <QAbstractListModel>
#include <QDebug>

#include "abstractFigure.hpp"

namespace Stratogi {
namespace Shogi {

  class ShogiTable Q_DECL_FINAL: public QAbstractListModel
  {
    Q_OBJECT

    enum RoleNames {
      CellImageSourceRole   = Qt::UserRole,
      CurrentPieceColorRole = CellImageSourceRole   + 1,
      PieceColorRole        = CurrentPieceColorRole + 1,
      BorderColorRole       = PieceColorRole        + 1
    }; // enum RoleNames

    public:
      ShogiTable();
      ShogiTable(const ShogiTable &shogiBoard);
      ShogiTable &operator = (const ShogiTable &other);

      static const int OFFSET_TOPTABLE = 100;
      static const int OFFSET_BOTTOMTABLE = OFFSET_TOPTABLE + 100;

      static const uint32 ROWS    = 2;
      static const uint32 COLUMNS = 10;

      using FiguresOnTable = std::vector<std::vector<AbstractFigure*>>;

      //! \return Figures on board.
      //! \note Returned type is two-dimensinal array and indexes are
      //! as in usual C array.
      FiguresOnTable &figures();

      //! \return Figure by the x, y value in the TableGrid FiguresOnTable
      //! \attention If there is no Figure at that Position a nullptr will be returned
      AbstractFigure *figure(const uint32_t x, const uint32_t y) const;

      //! Sets the Figure at the Position x, y in the TableGrid FiguresOnTable
      void setFigure(const uint32 x, const uint32 y, AbstractFigure *figure);

      void update();
      void update(uint32_t x, uint32_t y);
      int rowCount(const QModelIndex & parent = QModelIndex()) const override;
      QVariant data(const QModelIndex & index, int role) const override;
      QHash<int, QByteArray> roleNames() const override;

      void reset();
      void copyState(const ShogiTable &other);
    private:
      FiguresOnTable m_table;
  };

} // END NAMESPACE Shogi
} // END NAMEPSACE "Stratogi"

#endif // SHOGITABLE_H
