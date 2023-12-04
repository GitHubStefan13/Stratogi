#include "chessBoard.h"

namespace Stratogi {
namespace Chess {

  ChessBoard::ChessBoard() : AbstractBoard (ROWS, COLUMNS)
  {
    m_figures = initFigures();
    m_whiteKing = static_cast<King*>(m_figures.at(20).data());
    m_blackKing = static_cast<King*>(m_figures.at(4).data());

    newGame();
  }

  ChessBoard::ChessBoard(const ChessBoard &chessBoard) : AbstractBoard (ROWS, COLUMNS)
  {
    m_figures = initFigures();
    m_whiteKing = static_cast<King*>(m_figures.at(20).data());
    m_blackKing = static_cast<King*>(m_figures.at(4).data());

    copyState(chessBoard);
  }

  ChessBoard &ChessBoard::operator = ( const ChessBoard &other)
  {
    if(this != &other)
    {
      m_figures = initFigures();
      m_whiteKing = static_cast<King*>(m_figures.at(20).data());
      m_blackKing = static_cast<King*>(m_figures.at(4).data());

      copyState(other);
    }

    return *this;
  }

  void ChessBoard::move(uint32 fromX, uint32 fromY, uint32 toX, uint32 toY, bool testMove)
  {
    AbstractFigure *from = m_board[fromY][fromX]->figure();

    AbstractBoard::move(fromX, fromY, toX, toY, testMove);
    if(from)
    {
      // Check if we passed a Pawn with the Move
      if(from->type() == FigureType::PawnFigure )
      {
        Pawn *p = static_cast<Pawn*>(from);

        switch(p->color())
        {
          case AbstractFigure::FigureColor::White :
            toY += 1;
            break;

          case AbstractFigure::FigureColor::Black :
            toY -= 1;
            break;
        }

        Pawn *killed = dynamic_cast<Pawn*>(m_board[toY][toX]);
        if(killed && killed->isPass())
        {
          if(!testMove){
            m_board[toY][toX] = nullptr;

            const QModelIndex toIndex = index( toY * _rowSize + toX, 0 );
            Q_EMIT dataChanged(toIndex, toIndex);
          }
        }
      }
    }


  }

  void ChessBoard::newGame()
  {
    // Put the Figures at the correct Position on the Board
    static_cast<AbstractFigure*>(m_figures.at(0).data())->setX(0);
    static_cast<AbstractFigure*>(m_figures.at(0).data())->setY(0);
    static_cast<AbstractFigure*>(m_figures.at(1).data())->setX(1);
    static_cast<AbstractFigure*>(m_figures.at(1).data())->setY(0);
    static_cast<AbstractFigure*>(m_figures.at(2).data())->setX(2);
    static_cast<AbstractFigure*>(m_figures.at(2).data())->setY(0);
    static_cast<AbstractFigure*>(m_figures.at(3).data())->setX(3);
    static_cast<AbstractFigure*>(m_figures.at(3).data())->setY(0);
    static_cast<AbstractFigure*>(m_figures.at(4).data())->setX(4);
    static_cast<AbstractFigure*>(m_figures.at(4).data())->setY(0);
    static_cast<AbstractFigure*>(m_figures.at(5).data())->setX(5);
    static_cast<AbstractFigure*>(m_figures.at(5).data())->setY(0);
    static_cast<AbstractFigure*>(m_figures.at(6).data())->setX(6);
    static_cast<AbstractFigure*>(m_figures.at(6).data())->setY(0);
    static_cast<AbstractFigure*>(m_figures.at(7).data())->setX(7);
    static_cast<AbstractFigure*>(m_figures.at(7).data())->setY(0);

    static_cast<AbstractFigure*>(m_figures.at(8).data())->setX(0);
    static_cast<AbstractFigure*>(m_figures.at(8).data())->setY(1);
    static_cast<AbstractFigure*>(m_figures.at(9).data())->setX(1);
    static_cast<AbstractFigure*>(m_figures.at(9).data())->setY(1);
    static_cast<AbstractFigure*>(m_figures.at(10).data())->setX(2);
    static_cast<AbstractFigure*>(m_figures.at(10).data())->setY(1);
    static_cast<AbstractFigure*>(m_figures.at(11).data())->setX(3);
    static_cast<AbstractFigure*>(m_figures.at(11).data())->setY(1);
    static_cast<AbstractFigure*>(m_figures.at(12).data())->setX(4);
    static_cast<AbstractFigure*>(m_figures.at(12).data())->setY(1);
    static_cast<AbstractFigure*>(m_figures.at(13).data())->setX(5);
    static_cast<AbstractFigure*>(m_figures.at(13).data())->setY(1);
    static_cast<AbstractFigure*>(m_figures.at(14).data())->setX(6);
    static_cast<AbstractFigure*>(m_figures.at(14).data())->setY(1);
    static_cast<AbstractFigure*>(m_figures.at(15).data())->setX(7);
    static_cast<AbstractFigure*>(m_figures.at(15).data())->setY(1);

    for(int i = 8; i < 16; ++i)
      static_cast<Pawn*>(m_figures.at(i).data())->setPass(false);

    static_cast<AbstractFigure*>(m_figures.at(16).data())->setX(0);
    static_cast<AbstractFigure*>(m_figures.at(16).data())->setY(7);
    static_cast<AbstractFigure*>(m_figures.at(17).data())->setX(1);
    static_cast<AbstractFigure*>(m_figures.at(17).data())->setY(7);
    static_cast<AbstractFigure*>(m_figures.at(18).data())->setX(2);
    static_cast<AbstractFigure*>(m_figures.at(18).data())->setY(7);
    static_cast<AbstractFigure*>(m_figures.at(19).data())->setX(3);
    static_cast<AbstractFigure*>(m_figures.at(19).data())->setY(7);
    static_cast<AbstractFigure*>(m_figures.at(20).data())->setX(4);
    static_cast<AbstractFigure*>(m_figures.at(20).data())->setY(7);
    static_cast<AbstractFigure*>(m_figures.at(21).data())->setX(5);
    static_cast<AbstractFigure*>(m_figures.at(21).data())->setY(7);
    static_cast<AbstractFigure*>(m_figures.at(22).data())->setX(6);
    static_cast<AbstractFigure*>(m_figures.at(22).data())->setY(7);
    static_cast<AbstractFigure*>(m_figures.at(23).data())->setX(7);
    static_cast<AbstractFigure*>(m_figures.at(23).data())->setY(7);

    static_cast<AbstractFigure*>(m_figures.at(24).data())->setX(0);
    static_cast<AbstractFigure*>(m_figures.at(24).data())->setY(6);
    static_cast<AbstractFigure*>(m_figures.at(25).data())->setX(1);
    static_cast<AbstractFigure*>(m_figures.at(25).data())->setY(6);
    static_cast<AbstractFigure*>(m_figures.at(26).data())->setX(2);
    static_cast<AbstractFigure*>(m_figures.at(26).data())->setY(6);
    static_cast<AbstractFigure*>(m_figures.at(27).data())->setX(3);
    static_cast<AbstractFigure*>(m_figures.at(27).data())->setY(6);
    static_cast<AbstractFigure*>(m_figures.at(28).data())->setX(4);
    static_cast<AbstractFigure*>(m_figures.at(28).data())->setY(6);
    static_cast<AbstractFigure*>(m_figures.at(29).data())->setX(5);
    static_cast<AbstractFigure*>(m_figures.at(29).data())->setY(6);
    static_cast<AbstractFigure*>(m_figures.at(30).data())->setX(6);
    static_cast<AbstractFigure*>(m_figures.at(30).data())->setY(6);
    static_cast<AbstractFigure*>(m_figures.at(31).data())->setX(7);
    static_cast<AbstractFigure*>(m_figures.at(31).data())->setY(6);

    for( int i = 24; i < 32; ++i )
      static_cast<Pawn*>(m_figures.at(i).data())->setPass(false);

    // Places the Pieces at the correct Positions
    m_board = {
      {
        m_figures.at(0).data(),
        m_figures.at(1).data(),
        m_figures.at(2).data(),
        m_figures.at(3).data(),
        m_figures.at(4).data(),
        m_figures.at(5).data(),
        m_figures.at(6).data(),
        m_figures.at(7).data()
      },

      {
        m_figures.at(8).data(),
        m_figures.at(9).data(),
        m_figures.at(10).data(),
        m_figures.at(11).data(),
        m_figures.at(12).data(),
        m_figures.at(13).data(),
        m_figures.at(14).data(),
        m_figures.at(15).data()
      },

      {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
      {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
      {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
      {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

      {
        m_figures.at(24).data(),
        m_figures.at(25).data(),
        m_figures.at(26).data(),
        m_figures.at(27).data(),
        m_figures.at(28).data(),
        m_figures.at(29).data(),
        m_figures.at(30).data(),
        m_figures.at(31).data()
      },

      {
        m_figures.at(16).data(),
        m_figures.at(17).data(),
        m_figures.at(18).data(),
        m_figures.at(19).data(),
        m_figures.at(20).data(),
        m_figures.at(21).data(),
        m_figures.at(22).data(),
        m_figures.at(23).data()
      }
    };

    AbstractBoard::newGame();
  }

  void ChessBoard::transformation(int figure, int c, int x, int y)
  {
    switch(figure)
    {
      case Signals::Queen :
      {
        m_transformed.append(QSharedPointer<AbstractFigure>(new Queen(x, y, (c == AbstractFigure::White ? AbstractFigure::White : AbstractFigure::Black ),
                                                            QLatin1String("queen"), 32 + m_transformed.size())));

        m_board[y][x] = m_transformed.last().data();

        const QModelIndex idx = index(y * ROWS + x, 0);

        Q_EMIT dataChanged( idx, idx );
      }
        break;

      case Signals::Rook :
      {
        m_transformed.append(QSharedPointer<AbstractFigure>(new Rook(x, y, (c == AbstractFigure::White ? AbstractFigure::White : AbstractFigure::Black),
                                                            QLatin1String("queen"), 32 + m_transformed.size())));

        m_board[ y ][ x ] = m_transformed.last().data();

        const QModelIndex idx = index( y * 8 + x, 0 );

        Q_EMIT dataChanged( idx, idx );
      }
        break;

      case Signals::Knight :
      {
        m_transformed.append(QSharedPointer<AbstractFigure>(new Knight(x, y, (c == AbstractFigure::White ? AbstractFigure::White : AbstractFigure::Black),
                                                            QLatin1String("queen"), 32 + m_transformed.size())));

        m_board[y][x] = m_transformed.last().data();

        const QModelIndex idx = index(y * ROWS + x, 0);

        Q_EMIT dataChanged( idx, idx );
      }
        break;

      case Signals::Bishop :
      {
        m_transformed.append(QSharedPointer<AbstractFigure>(new Bishop(x, y, (c == AbstractFigure::White ? AbstractFigure::White : AbstractFigure::Black),
                                                            QLatin1String("queen"), 32 + m_transformed.size())));

        m_board[y][x] = m_transformed.last().data();

        const QModelIndex idx = index(y * ROWS + x, 0);

        Q_EMIT dataChanged(idx, idx);
      }
        break;
    }
  }

  QList<QSharedPointer<AbstractFigure>> ChessBoard::initFigures() const
  {      
    QList<QSharedPointer<AbstractFigure>> res = {
      QSharedPointer<AbstractFigure>(new Rook{   0, 0, AbstractFigure::Black, QLatin1String("rook-black-1" ),  0}),
      QSharedPointer<AbstractFigure>{new Knight{ 1, 0, AbstractFigure::Black, QLatin1String("knight-black-1"), 1}},
      QSharedPointer<AbstractFigure>{new Bishop{ 2, 0, AbstractFigure::Black, QLatin1String("bishop-black-1"), 2}},
      QSharedPointer<AbstractFigure>{new Queen{  3, 0, AbstractFigure::Black, QLatin1String("queen-black"),    3}},
      QSharedPointer<AbstractFigure>{new King{   4, 0, AbstractFigure::Black, QLatin1String("king-black"),     4}},
      QSharedPointer<AbstractFigure>{new Bishop{ 5, 0, AbstractFigure::Black, QLatin1String("bishop-black-2"), 5}},
      QSharedPointer<AbstractFigure>{new Knight{ 6, 0, AbstractFigure::Black, QLatin1String("knight-black-2"), 6}},
      QSharedPointer<AbstractFigure>{new Rook{   7, 0, AbstractFigure::Black, QLatin1String("rook-black-2"),   7}},
      QSharedPointer<AbstractFigure>{new Pawn{   0, 1, AbstractFigure::Black, QLatin1String("pawn-black-1"),   8}},
      QSharedPointer<AbstractFigure>{new Pawn{   1, 1, AbstractFigure::Black, QLatin1String("pawn-black-2"),   9}},
      QSharedPointer<AbstractFigure>{new Pawn{   2, 1, AbstractFigure::Black, QLatin1String("pawn-black-3"),   10}},
      QSharedPointer<AbstractFigure>{new Pawn{   3, 1, AbstractFigure::Black, QLatin1String("pawn-black-4"),   11}},
      QSharedPointer<AbstractFigure>{new Pawn{   4, 1, AbstractFigure::Black, QLatin1String("pawn-black-5"),   12}},
      QSharedPointer<AbstractFigure>{new Pawn{   5, 1, AbstractFigure::Black, QLatin1String("pawn-black-6"),   13}},
      QSharedPointer<AbstractFigure>{new Pawn{   6, 1, AbstractFigure::Black, QLatin1String("pawn-black-7"),   14}},
      QSharedPointer<AbstractFigure>{new Pawn{   7, 1, AbstractFigure::Black, QLatin1String("pawn-black-8"),   15}},

      QSharedPointer<AbstractFigure>{new Rook{   0, 7, AbstractFigure::White, QLatin1String("rook-white-1"),   16}},
      QSharedPointer<AbstractFigure>{new Knight{ 1, 7, AbstractFigure::White, QLatin1String("knight-white-1"), 17}},
      QSharedPointer<AbstractFigure>{new Bishop{ 2, 7, AbstractFigure::White, QLatin1String("bishop-white-1"), 18}},
      QSharedPointer<AbstractFigure>{new Queen{  3, 7, AbstractFigure::White, QLatin1String("queen-white"),    19}},
      QSharedPointer<AbstractFigure>{new King{   4, 7, AbstractFigure::White, QLatin1String("king-white"),     20}},
      QSharedPointer<AbstractFigure>{new Bishop{ 5, 7, AbstractFigure::White, QLatin1String("bishop-white-2"), 21}},
      QSharedPointer<AbstractFigure>{new Knight{ 6, 7, AbstractFigure::White, QLatin1String("knight-white-2"), 22}},
      QSharedPointer<AbstractFigure>{new Rook{   7, 7, AbstractFigure::White, QLatin1String("rook-white-2"),   23}},
      QSharedPointer<AbstractFigure>{new Pawn{   0, 6, AbstractFigure::White, QLatin1String("pawn-white-1"),   24}},
      QSharedPointer<AbstractFigure>{new Pawn{   1, 6, AbstractFigure::White, QLatin1String("pawn-white-2"),   25}},
      QSharedPointer<AbstractFigure>{new Pawn{   2, 6, AbstractFigure::White, QLatin1String("pawn-white-3"),   26}},
      QSharedPointer<AbstractFigure>{new Pawn{   3, 6, AbstractFigure::White, QLatin1String("pawn-white-4"),   27}},
      QSharedPointer<AbstractFigure>{new Pawn{   4, 6, AbstractFigure::White, QLatin1String("pawn-white-5"),   28}},
      QSharedPointer<AbstractFigure>{new Pawn{   5, 6, AbstractFigure::White, QLatin1String("pawn-white-6"),   29}},
      QSharedPointer<AbstractFigure>{new Pawn{   6, 6, AbstractFigure::White, QLatin1String("pawn-white-7"),   30}},
      QSharedPointer<AbstractFigure>{new Pawn{   7, 6, AbstractFigure::White, QLatin1String("pawn-white-8"),   31}}
    };

    return res;
  }

  QVariant ChessBoard::data(const QModelIndex & index, int role) const
  {
    uint32 row = index.row();

    if(row >= rowCount())
      return QVariant();

    if(m_colors.empty() )
      return QVariant();

    uint32 r = row / ROWS;
    uint32 c = row % ROWS;

    const AbstractFigure *f = m_board[r][c];

    switch(role)
    {
      case CellImageSourceRole :
      {
        if(f)
        {
          QString res = QLatin1String("qrc:/C_");

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
            case  Chess::FigureType::KingFigure :
              res.append(QLatin1String("King"));
              break;

            case Chess::FigureType::QueenFigure :
              res.append(QLatin1String("Queen"));
              break;

            case Chess::FigureType::RookFigure :
              res.append(QLatin1String("Rook"));
              break;

            case Chess::FigureType::KnightFigure :
              res.append(QLatin1String("Knight"));
              break;

            case Chess::FigureType::BishopFigure :
              res.append(QLatin1String("Bishop"));
              break;

            case Chess::FigureType::PawnFigure :
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

      case BluePieceColorRole :
        return (m_colors[r][c] == Abstract::Color::Blue ? true : false);

      case RedPieceColorRole :
        return (m_colors[r][c] == Abstract::Color::Red ? true : false);

      case PieceColorRole :
        return (m_colors[r][c] == Abstract::Color::Check ? true : false);

      default :
        break;
    }

    return QVariant();
  }

  void ChessBoard::clearColors()
  {
    m_colors = {
      {Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None,
       Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None
      },
      {Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None,
       Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None
      },
      {Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None,
       Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None
      },
      {Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None,
       Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None
      },
      {Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None,
       Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None
      },
      {Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None,
       Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None
      },
      {Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None,
       Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None
      },
      {Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None,
       Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None
      }
    };

    update();
  }

} // END NAMESPACE "Chess"
} // END NAMEPSACE "Stratogi"
