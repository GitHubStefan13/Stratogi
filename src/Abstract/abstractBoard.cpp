// Chess include.
#include "abstractBoard.hpp"

// C++ include.
#include <algorithm>

// Qt include.
#include <QColor>
#include <QVariant>

namespace Stratogi {

  AbstractBoard::AbstractBoard(uint32 rowSize, uint32 columnSize)
  {
    this->_rowSize = rowSize;
    this->_columnSize = columnSize;
    // Resize Vectors to the specified size
    m_board.resize(_rowSize);
    for(auto& item : m_board){
      item.resize(_columnSize);
    }
    m_colors.resize(_rowSize, std::vector<int>(_columnSize));
  }

  AbstractBoard::AbstractBoard(const AbstractBoard &abstractBoard) : QAbstractListModel( Q_NULLPTR )
  {
  }

  AbstractBoard &AbstractBoard::operator =(const AbstractBoard &other)
  {
    if( this != &other )
    {
      copyState(other);
    }

    return *this;
  }

  AbstractBoard::~AbstractBoard()
  {
  }

  void AbstractBoard::move(uint32 fromX, uint32 fromY, uint32 toX, uint32 toY, bool testMove)
  {
    if(fromX > _rowSize ||
       fromY > _columnSize ||
       toX > _rowSize ||
       toY > _columnSize)
    {
      qDebug() << "Invalid move, position is outside the Board";
      return;
    }

    AbstractFigure *from = m_board[fromY][fromX];

    if(from)
    {
      m_board[fromY][fromX] = nullptr;
      m_board[toY][toX] = from;

      from->setX(toX);
      from->setY(toY);

      const QModelIndex fromIndex = index(fromY * _rowSize + fromX, 0);
      const QModelIndex toIndex = index(toY * _rowSize + toX, 0);

      if(!testMove) {
        Q_EMIT dataChanged(fromIndex, fromIndex);
        Q_EMIT dataChanged(toIndex, toIndex);
      }
    }
  }

  AbstractBoard::FiguresOnBoard &AbstractBoard::figures()
  {
    return m_board;
  }

  AbstractFigure *AbstractBoard::figure(uint32 index) const
  {
    if(index >= 0 && index < m_figures.size())
      return m_figures.at(index).data();

    qDebug() << "AbstractFigure::figure: Index is not valid:" << index << m_figures.size();
    return nullptr;
  }

  AbstractFigure *AbstractBoard::figure(uint32 x, uint32 y) const
  {
    if(x < _rowSize && y < _columnSize)
      return m_board[x][y];
    return nullptr;
  }

  QHash<int, QByteArray> AbstractBoard::roleNames() const
  {
    QHash< int, QByteArray > names;

    names[ CellImageSourceRole ] = "CellImageSource";
    names[ CurrentPieceColorRole ] = "CurrentPieceColor";
    names[ BluePieceColorRole ] = "BluePieceColor";
    names[ RedPieceColorRole ] = "RedPieceColor";
    names[ PieceColorRole ] = "PieceColor";
    names[ BorderColorRole ] = "BorderColor";

    return names;
  }

  void AbstractBoard::newGame()
  {
    m_transformed.clear();
    m_moves.clear();

    // Clear possible Move Colors
    // NOTE: also Updates the Board so we do not need to cast update again
    clearColors();

    std::for_each(m_figures.begin(), m_figures.end(), [] (QSharedPointer<AbstractFigure> &abstractfigure){
        // Reset all first Moves, that may have been mapped in the last Game
        abstractfigure->firstMoveDone(false);
        // Reset Transformation
        abstractfigure->transform(false);
    });
  }

  void AbstractBoard::update()
  {      
    const QModelIndex start = index(0, 0);
    const QModelIndex end   = index(_rowSize * _columnSize - 1, 0);

    Q_EMIT dataChanged(start, end);
  }

  void AbstractBoard::update(uint32_t x, uint32_t y)
  {
    QModelIndex cell = index(y *_rowSize + x);
    Q_EMIT dataChanged(cell, cell);
  }

  void AbstractBoard::copyState(const AbstractBoard &other)
  {
    for( const auto & o : qAsConst(other.m_transformed))
      m_transformed.append(o->copy());

    for(uint32_t i = 0; i < _rowSize; ++i)
    {
      for(uint32_t j = 0; j < _columnSize; ++j)
      {
        if(other.m_board[i][j])
        {
          AbstractFigure *o = other.m_board[i][j];
          AbstractFigure *f = (o->index() < m_figures.size() ? m_figures.at(o->index() ).data()
                                                             : m_transformed.at(o->index() - m_figures.size()).data());

          m_board[i][j] = f;

          f->setX(o->x());
          f->setY(o->y());
          f->firstMoveDone(o->isFirstMoveDone());
        }
        else
          m_board[i][j] = nullptr;

        m_colors[i][j] = other.m_colors[i][j];
      }
    }
  }

  int AbstractBoard::rowCount( const QModelIndex & parent ) const
  {
    Q_UNUSED( parent )

    return static_cast<int>(_rowSize * _columnSize);
  }

  QVariant AbstractBoard::data(const QModelIndex &index, int role) const
  {
    Q_UNUSED(index)
    Q_UNUSED(role)

    return "AbstractBoard::data -> This needs to be overriden by the Derived CLASS!";
  }

  void AbstractBoard::markBlue(uint32 x, uint32 y)
  {
    m_colors[y][x] = Abstract::Color::Blue;
    update(x, y);
  }

  void AbstractBoard::markRed(uint32 x, uint32 y )
  {
    m_colors[y][x] = Abstract::Color::Red;
    update(x, y);
  }

  void AbstractBoard::markCheck(uint32 x, uint32 y)
  {
    m_colors[y][x] = Abstract::Color::Check;
    update(x, y);
  }

  int AbstractBoard::getColor(uint32 x, uint32 y)
  {
    return m_colors[x][y];
  }

  QStringList AbstractBoard::getMoves()
  {
    return m_moves;
  }

  QString AbstractBoard::getMovesAsString()
  {
    return m_moves.join(", ");
  }

  void AbstractBoard::appendMove(QString move)
  {
    if(move == "")
      return;

    QString lastMove = (m_moves.isEmpty()) ? "" : m_moves.last();

    if(lastMove != move)
    {
      m_moves.append(move);
      Q_EMIT movesChanged();
    }
  }

  void AbstractBoard::removeLastMove()
  {
    if(m_moves.isEmpty())
      return;

    m_moves.removeLast();
    Q_EMIT movesChanged();
  }

  AbstractFigure *AbstractBoard::whiteKing() const
  {
    return m_whiteKing;
  }

  AbstractFigure *AbstractBoard::blackKing() const
  {
    return m_blackKing;
  }

} // END NAMEPSACE "Stratogi"
