#include "shogiTable.h"

namespace Stratogi {
namespace Shogi {

ShogiTable::ShogiTable()
{
  // Resize Vectors to the specified size
  m_table.resize(ROWS);
  for(auto& item : m_table){
    item.resize(COLUMNS);
  }
  reset();
}

ShogiTable::ShogiTable(const ShogiTable &shogiBoard) : QAbstractListModel( Q_NULLPTR )
{
  Q_UNUSED(shogiBoard)
}

ShogiTable &ShogiTable::operator =(const ShogiTable &other)
{
  copyState(other);
  return *this;
}

AbstractFigure *ShogiTable::figure(const uint32_t x, const uint32_t y) const
{
  if(x < ROWS && y < COLUMNS)
    return m_table[x][y];
  return nullptr;
}

void ShogiTable::setFigure(const uint32_t x, const uint32_t y, AbstractFigure *figure)
{
  if(x >= ROWS || y >= COLUMNS)
    return;

  beginResetModel();
  m_table[x][y] = figure;
  endResetModel();
}

void ShogiTable::update()
{
  Q_EMIT dataChanged(index(0), index(ROWS * ROWS + COLUMNS * COLUMNS));
}

void ShogiTable::update(uint32_t x, uint32_t y)
{
  QModelIndex cell = index(x * ROWS + y);
  Q_EMIT dataChanged(cell, cell);
}

int ShogiTable::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent)

  return ROWS * COLUMNS;
}

QVariant ShogiTable::data(const QModelIndex & index, int role) const
{
  int row = index.row();

  if(row >= rowCount())
    return QVariant();

  uint32 r = static_cast<uint32_t>(row) / COLUMNS;
  uint32 c = static_cast<uint32_t>(row) % COLUMNS;

  const AbstractFigure *f = m_table[r][c];

  switch(role)
  {
    case CellImageSourceRole :
    {
      if(f)
      {
        QString res = QLatin1String("qrc:S_");

        switch(f->color())
        {
          case AbstractFigure::FigureColor::White :
            res.append(QLatin1String("W_"));
            break;

          case AbstractFigure::FigureColor::Black :
            res.append(QLatin1String("B_"));
            break;
        }

        switch(f->type())
        {
          case  Shogi::FigureType::KingFigure :
            res.append(QLatin1String("King"));
            break;

          case Shogi::FigureType::GoldGeneralFigure :
            res.append(QLatin1String("gGeneral"));
            break;

          case Shogi::FigureType::SilverGeneralFigure :
            res.append(QLatin1String("sGeneral"));
            break;

          case Shogi::FigureType::KnightFigure :
            res.append(QLatin1String("Knight"));
            break;

          case Shogi::FigureType::LanceFigure :
            res.append(QLatin1String("Lance"));
            break;

          case Shogi::FigureType::RookFigure :
            res.append(QLatin1String("Rook"));
            break;

          case Shogi::FigureType::BishopFigure :
            res.append(QLatin1String("Bishop"));
            break;

          case Shogi::FigureType::PawnFigure :
            res.append(QLatin1String("Pawn"));
            break;
        }

        res.append(QLatin1String(".png"));

        return res;
      }
      else
        return QLatin1String("qrc:img/empty.png");
    }

    case CurrentPieceColorRole :
      return (r % 2 == 0 ? (c % 2 == 0 ? QColor(Qt::white)
                                       : QColor(Qt::lightGray))
                         : (c % 2 == 0 ? QColor(Qt::lightGray)
                                       : QColor(Qt::white)));
    case BorderColorRole :
      return QColor( Qt::lightGray );

    default :
      break;
  }

  return QVariant();
}

QHash<int, QByteArray> ShogiTable::roleNames() const
{
  QHash< int, QByteArray > names;

  names[ CellImageSourceRole ] = "CellImageSource";
  names[ CurrentPieceColorRole ] = "CurrentPieceColor";
  names[ PieceColorRole ] = "PieceColor";
  names[ BorderColorRole ] = "BorderColor";

  return names;
}

void ShogiTable::copyState(const ShogiTable &other)
{
  for(uint32_t i = 0; i < ROWS; ++i)
    {
      for(uint32_t j = 0; j < COLUMNS; ++j)
      {
        if(other.m_table[i][j])
        {
          AbstractFigure *f = other.m_table[i][j];
          m_table[i][j] = f;
        }
        else
          m_table[i][j] = nullptr;
      }
    }
}

void ShogiTable::reset()
{
  beginResetModel();
  m_table = {
    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}
  };
  endResetModel();

  update();
}

} // END NAMESPACE "Shogi"
} // END NAMEPSACE "Stratogi"
