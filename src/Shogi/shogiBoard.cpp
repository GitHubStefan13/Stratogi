#include "shogiBoard.h"

#include "figuretypes.h"
#include "shogifigures.h"

namespace Stratogi {
namespace Shogi {

ShogiBoard::ShogiBoard() : AbstractBoard (ROWS, COLUMNS)
{
  m_figures = initFigures();
  m_whiteKing = static_cast<King*>(m_figures.at(35).data());
  m_blackKing = static_cast<King*>(m_figures.at(4).data());

  newGame();
}

ShogiBoard::ShogiBoard(const ShogiBoard &shogiBoard) : AbstractBoard (ROWS, COLUMNS)
{
  m_figures = initFigures();
  m_whiteKing = static_cast<King*>(m_figures.at(20).data());
  m_blackKing = static_cast<King*>(m_figures.at(4).data());

  copyState(shogiBoard);
}

ShogiBoard &ShogiBoard::operator = ( const ShogiBoard &other)
{
  if(this != &other)
  {
    m_figures = initFigures();
    m_blackKing = static_cast<King*>(m_figures.at(4).data());  // See initFigures King 1
    m_whiteKing = static_cast<King*>(m_figures.at(35).data()); // See initFigures King 2

    copyState(other);
  }

  return *this;
}

void ShogiBoard::move(uint32 fromX, uint32 fromY, uint32 toX, uint32 toY, bool testMove)
{
  if(fromX < ROWS && fromY < COLUMNS)
    AbstractBoard::move(fromX, fromY, toX, toY, testMove);
  else {
    // Clicked on the Tables
    if(fromX >= ShogiTable::OFFSET_BOTTOMTABLE && fromY >= ShogiTable::OFFSET_BOTTOMTABLE){ // Click in Bottom Table
      fromX -= ShogiTable::OFFSET_BOTTOMTABLE;
      fromY -= ShogiTable::OFFSET_BOTTOMTABLE;
      tableMove(fromX, fromY, toX, toY, &table_bot);
    } else { // Click in Top Table
      fromX -= ShogiTable::OFFSET_TOPTABLE;
      fromY -= ShogiTable::OFFSET_TOPTABLE;
      tableMove(fromX, fromY, toX, toY, &table_top);
    }
  }
}

void ShogiBoard::newGame()
{
  // First Row Black
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
  static_cast<AbstractFigure*>(m_figures.at(8).data())->setX(8);
  static_cast<AbstractFigure*>(m_figures.at(8).data())->setY(0);
  // Reset Color to default, color changes once they move to the Table
  static_cast<AbstractFigure*>(m_figures.at(0).data())->setColor(AbstractFigure::FigureColor::Black);
  static_cast<AbstractFigure*>(m_figures.at(1).data())->setColor(AbstractFigure::FigureColor::Black);
  static_cast<AbstractFigure*>(m_figures.at(2).data())->setColor(AbstractFigure::FigureColor::Black);
  static_cast<AbstractFigure*>(m_figures.at(3).data())->setColor(AbstractFigure::FigureColor::Black);
  static_cast<AbstractFigure*>(m_figures.at(4).data())->setColor(AbstractFigure::FigureColor::Black);
  static_cast<AbstractFigure*>(m_figures.at(5).data())->setColor(AbstractFigure::FigureColor::Black);
  static_cast<AbstractFigure*>(m_figures.at(6).data())->setColor(AbstractFigure::FigureColor::Black);
  static_cast<AbstractFigure*>(m_figures.at(7).data())->setColor(AbstractFigure::FigureColor::Black);
  static_cast<AbstractFigure*>(m_figures.at(8).data())->setColor(AbstractFigure::FigureColor::Black);


  // Second Row Black
  static_cast<AbstractFigure*>(m_figures.at(9).data())->setX(1);
  static_cast<AbstractFigure*>(m_figures.at(9).data())->setY(1);
  static_cast<AbstractFigure*>(m_figures.at(10).data())->setX(7);
  static_cast<AbstractFigure*>(m_figures.at(10).data())->setY(1);
  // Reset Color to default, color changes once they move to the Table
  static_cast<AbstractFigure*>(m_figures.at(9).data())->setColor(AbstractFigure::FigureColor::Black);
  static_cast<AbstractFigure*>(m_figures.at(10).data())->setColor(AbstractFigure::FigureColor::Black);

  // Third Row Black
  static_cast<AbstractFigure*>(m_figures.at(11).data())->setX(0);
  static_cast<AbstractFigure*>(m_figures.at(11).data())->setY(2);
  static_cast<AbstractFigure*>(m_figures.at(12).data())->setX(1);
  static_cast<AbstractFigure*>(m_figures.at(12).data())->setY(2);
  static_cast<AbstractFigure*>(m_figures.at(13).data())->setX(2);
  static_cast<AbstractFigure*>(m_figures.at(13).data())->setY(2);
  static_cast<AbstractFigure*>(m_figures.at(14).data())->setX(3);
  static_cast<AbstractFigure*>(m_figures.at(14).data())->setY(2);
  static_cast<AbstractFigure*>(m_figures.at(15).data())->setX(4);
  static_cast<AbstractFigure*>(m_figures.at(15).data())->setY(2);
  static_cast<AbstractFigure*>(m_figures.at(16).data())->setX(5);
  static_cast<AbstractFigure*>(m_figures.at(16).data())->setY(2);
  static_cast<AbstractFigure*>(m_figures.at(17).data())->setX(6);
  static_cast<AbstractFigure*>(m_figures.at(17).data())->setY(2);
  static_cast<AbstractFigure*>(m_figures.at(18).data())->setX(7);
  static_cast<AbstractFigure*>(m_figures.at(18).data())->setY(2);
  static_cast<AbstractFigure*>(m_figures.at(19).data())->setX(8);
  static_cast<AbstractFigure*>(m_figures.at(19).data())->setY(2);
  // Reset Color to default, color changes once they move to the Table
  static_cast<AbstractFigure*>(m_figures.at(11).data())->setColor(AbstractFigure::FigureColor::Black);
  static_cast<AbstractFigure*>(m_figures.at(12).data())->setColor(AbstractFigure::FigureColor::Black);
  static_cast<AbstractFigure*>(m_figures.at(13).data())->setColor(AbstractFigure::FigureColor::Black);
  static_cast<AbstractFigure*>(m_figures.at(14).data())->setColor(AbstractFigure::FigureColor::Black);
  static_cast<AbstractFigure*>(m_figures.at(15).data())->setColor(AbstractFigure::FigureColor::Black);
  static_cast<AbstractFigure*>(m_figures.at(16).data())->setColor(AbstractFigure::FigureColor::Black);
  static_cast<AbstractFigure*>(m_figures.at(17).data())->setColor(AbstractFigure::FigureColor::Black);
  static_cast<AbstractFigure*>(m_figures.at(18).data())->setColor(AbstractFigure::FigureColor::Black);
  static_cast<AbstractFigure*>(m_figures.at(19).data())->setColor(AbstractFigure::FigureColor::Black);

  // First Row White
  static_cast<AbstractFigure*>(m_figures.at(20).data())->setX(0);
  static_cast<AbstractFigure*>(m_figures.at(20).data())->setY(6);
  static_cast<AbstractFigure*>(m_figures.at(21).data())->setX(1);
  static_cast<AbstractFigure*>(m_figures.at(21).data())->setY(6);
  static_cast<AbstractFigure*>(m_figures.at(22).data())->setX(2);
  static_cast<AbstractFigure*>(m_figures.at(22).data())->setY(6);
  static_cast<AbstractFigure*>(m_figures.at(23).data())->setX(3);
  static_cast<AbstractFigure*>(m_figures.at(23).data())->setY(6);
  static_cast<AbstractFigure*>(m_figures.at(24).data())->setX(4);
  static_cast<AbstractFigure*>(m_figures.at(24).data())->setY(6);
  static_cast<AbstractFigure*>(m_figures.at(25).data())->setX(5);
  static_cast<AbstractFigure*>(m_figures.at(25).data())->setY(6);
  static_cast<AbstractFigure*>(m_figures.at(26).data())->setX(6);
  static_cast<AbstractFigure*>(m_figures.at(26).data())->setY(6);
  static_cast<AbstractFigure*>(m_figures.at(27).data())->setX(7);
  static_cast<AbstractFigure*>(m_figures.at(27).data())->setY(6);
  static_cast<AbstractFigure*>(m_figures.at(28).data())->setX(8);
  static_cast<AbstractFigure*>(m_figures.at(28).data())->setY(6);
  // Reset Color to default, color changes once they move to the Table
  static_cast<AbstractFigure*>(m_figures.at(20).data())->setColor(AbstractFigure::FigureColor::White);
  static_cast<AbstractFigure*>(m_figures.at(21).data())->setColor(AbstractFigure::FigureColor::White);
  static_cast<AbstractFigure*>(m_figures.at(22).data())->setColor(AbstractFigure::FigureColor::White);
  static_cast<AbstractFigure*>(m_figures.at(23).data())->setColor(AbstractFigure::FigureColor::White);
  static_cast<AbstractFigure*>(m_figures.at(24).data())->setColor(AbstractFigure::FigureColor::White);
  static_cast<AbstractFigure*>(m_figures.at(25).data())->setColor(AbstractFigure::FigureColor::White);
  static_cast<AbstractFigure*>(m_figures.at(26).data())->setColor(AbstractFigure::FigureColor::White);
  static_cast<AbstractFigure*>(m_figures.at(27).data())->setColor(AbstractFigure::FigureColor::White);
  static_cast<AbstractFigure*>(m_figures.at(28).data())->setColor(AbstractFigure::FigureColor::White);

  // Second Row White
  static_cast<AbstractFigure*>(m_figures.at(29).data())->setX(1);
  static_cast<AbstractFigure*>(m_figures.at(29).data())->setY(7);
  static_cast<AbstractFigure*>(m_figures.at(30).data())->setX(7);
  static_cast<AbstractFigure*>(m_figures.at(30).data())->setY(7);
  // Reset Color to default, color changes once they move to the Table
  static_cast<AbstractFigure*>(m_figures.at(29).data())->setColor(AbstractFigure::FigureColor::White);
  static_cast<AbstractFigure*>(m_figures.at(30).data())->setColor(AbstractFigure::FigureColor::White);

  // Third Row White
  static_cast<AbstractFigure*>(m_figures.at(31).data())->setX(0);
  static_cast<AbstractFigure*>(m_figures.at(31).data())->setY(8);
  static_cast<AbstractFigure*>(m_figures.at(32).data())->setX(1);
  static_cast<AbstractFigure*>(m_figures.at(32).data())->setY(8);
  static_cast<AbstractFigure*>(m_figures.at(33).data())->setX(2);
  static_cast<AbstractFigure*>(m_figures.at(33).data())->setY(8);
  static_cast<AbstractFigure*>(m_figures.at(34).data())->setX(3);
  static_cast<AbstractFigure*>(m_figures.at(34).data())->setY(8);
  static_cast<AbstractFigure*>(m_figures.at(35).data())->setX(4);
  static_cast<AbstractFigure*>(m_figures.at(35).data())->setY(8);
  static_cast<AbstractFigure*>(m_figures.at(36).data())->setX(5);
  static_cast<AbstractFigure*>(m_figures.at(36).data())->setY(8);
  static_cast<AbstractFigure*>(m_figures.at(37).data())->setX(6);
  static_cast<AbstractFigure*>(m_figures.at(37).data())->setY(8);
  static_cast<AbstractFigure*>(m_figures.at(38).data())->setX(7);
  static_cast<AbstractFigure*>(m_figures.at(38).data())->setY(8);
  static_cast<AbstractFigure*>(m_figures.at(39).data())->setX(8);
  static_cast<AbstractFigure*>(m_figures.at(39).data())->setY(8);
  // Reset Color to default, color changes once they move to the Table
  static_cast<AbstractFigure*>(m_figures.at(31).data())->setColor(AbstractFigure::FigureColor::White);
  static_cast<AbstractFigure*>(m_figures.at(32).data())->setColor(AbstractFigure::FigureColor::White);
  static_cast<AbstractFigure*>(m_figures.at(33).data())->setColor(AbstractFigure::FigureColor::White);
  static_cast<AbstractFigure*>(m_figures.at(34).data())->setColor(AbstractFigure::FigureColor::White);
  static_cast<AbstractFigure*>(m_figures.at(35).data())->setColor(AbstractFigure::FigureColor::White);
  static_cast<AbstractFigure*>(m_figures.at(36).data())->setColor(AbstractFigure::FigureColor::White);
  static_cast<AbstractFigure*>(m_figures.at(37).data())->setColor(AbstractFigure::FigureColor::White);
  static_cast<AbstractFigure*>(m_figures.at(38).data())->setColor(AbstractFigure::FigureColor::White);
  static_cast<AbstractFigure*>(m_figures.at(39).data())->setColor(AbstractFigure::FigureColor::White);

  // Reset Transformation
  for(int i = 0; i < m_figures.count(); i++){
    static_cast<AbstractFigure*>(m_figures.at(i).data())->transform(false);
  }

  m_board = {
    {
      m_figures.at(0).data(),
      m_figures.at(1).data(),
      m_figures.at(2).data(),
      m_figures.at(3).data(),
      m_figures.at(4).data(),
      m_figures.at(5).data(),
      m_figures.at(6).data(),
      m_figures.at(7).data(),
      m_figures.at(8).data()
    },

    {
      nullptr,
      m_figures.at(9).data(),
      nullptr,
      nullptr,
      nullptr,
      nullptr,
      nullptr,
      m_figures.at(10).data(),
      nullptr
    },

    {
      m_figures.at(11).data(),
      m_figures.at(12).data(),
      m_figures.at(13).data(),
      m_figures.at(14).data(),
      m_figures.at(15).data(),
      m_figures.at(16).data(),
      m_figures.at(17).data(),
      m_figures.at(18).data(),
      m_figures.at(19).data()
    },

    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {
      m_figures.at(20).data(),
      m_figures.at(21).data(),
      m_figures.at(22).data(),
      m_figures.at(23).data(),
      m_figures.at(24).data(),
      m_figures.at(25).data(),
      m_figures.at(26).data(),
      m_figures.at(27).data(),
      m_figures.at(28).data()
    },

    {
      nullptr,
      m_figures.at(29).data(),
      nullptr,
      nullptr,
      nullptr,
      nullptr,
      nullptr,
      m_figures.at(30).data(),
      nullptr
    },

    {
      m_figures.at(31).data(),
      m_figures.at(32).data(),
      m_figures.at(33).data(),
      m_figures.at(34).data(),
      m_figures.at(35).data(),
      m_figures.at(36).data(),
      m_figures.at(37).data(),
      m_figures.at(38).data(),
      m_figures.at(39).data()
    }
  };

  table_top.reset();
  table_bot.reset();

  AbstractBoard::newGame();
}

void ShogiBoard::transformation(int figure, int c, int x, int y)
{
  qDebug() << "X:" << x << " Y:" << y;
  bool promote = static_cast<bool>(figure); // We simple set figure to 0 or 1 to know if we promote or not, this happens on the QML-Side

  if(promote)
  {
    if(m_board[y][x])
    {
      static_cast<AbstractPromoted*>(m_board[y][x])->promote();
      update(x, y);
    }
  }

  Q_EMIT promotionConfirmed(promote);
}

void ShogiBoard::update()
{
  AbstractBoard::update();

  table_top.update();
  table_bot.update();
}

void ShogiBoard::update(uint32_t x, uint32_t y)
{
  AbstractBoard::update(x, y);

  table_top.update(x, y);
  table_bot.update(x, y);
}

void ShogiBoard::defeated(AbstractFigure *figure)
{
  uint32_t m_x = table_top.ROWS;    // Table Top And Bottom have the same count of rows
  uint32_t m_y = table_top.COLUMNS; // Table Top And Bottom have the same count of columns

  switch(figure->color()){

    case AbstractFigure::White:
    {
      for(uint32_t x = 0; x < m_x; x++){
        for(uint32_t y = 0; y < m_y; y++){
           if(!table_top.figure(x, y)) // Find a free Spot
           {
             figure->transform(false);
             figure->setColor(AbstractFigure::FigureColor::Black); // Change Color, it belongs to the Dark side now
             table_top.setFigure(x, y, figure);
             return;
           }
        }
      }

      break;
    }

    case AbstractFigure::Black:
    {
      for(uint32_t x = 0; x < m_x; x++){
        for(uint32_t y = 0; y < m_y; y++){
           if(!table_bot.figure(x, y)) // Find a free Spot
           {
             figure->transform(false);
             figure->setColor(AbstractFigure::FigureColor::White); // Change Color, it belongs to the Good side now
             table_bot.setFigure(x, y, figure);
             return;
           }
        }
      }

      break;
    }
  }
}

QList<QSharedPointer<AbstractFigure>> ShogiBoard::initFigures() const
{
  QList<QSharedPointer<AbstractFigure>> res = {
    QSharedPointer<AbstractFigure>(new Lance{           0, 0, AbstractFigure::Black, QLatin1String("lance-black-1" ),     0}),
    QSharedPointer<AbstractFigure>(new Knight{          1, 0, AbstractFigure::Black, QLatin1String("knight-black-1" ),    1}),
    QSharedPointer<AbstractFigure>(new SilverGeneral{   2, 0, AbstractFigure::Black, QLatin1String("sGeneral-black-1" ),  2}),
    QSharedPointer<AbstractFigure>(new GoldGeneral{     3, 0, AbstractFigure::Black, QLatin1String("gGeneral-black-1" ),  3}),
    QSharedPointer<AbstractFigure>(new King{            4, 0, AbstractFigure::Black, QLatin1String("king-black-1" ),      4}),
    QSharedPointer<AbstractFigure>(new GoldGeneral{     5, 0, AbstractFigure::Black, QLatin1String("gGeneral-black-2" ),  5}),
    QSharedPointer<AbstractFigure>(new SilverGeneral{   6, 0, AbstractFigure::Black, QLatin1String("sGeneral-black-2" ),  6}),
    QSharedPointer<AbstractFigure>(new Knight{          7, 0, AbstractFigure::Black, QLatin1String("knight-black-2" ),    7}),
    QSharedPointer<AbstractFigure>(new Lance{           8, 0, AbstractFigure::Black, QLatin1String("lance-black-2" ),     8}),
    QSharedPointer<AbstractFigure>(new Rook{            1, 1, AbstractFigure::Black, QLatin1String("lance-black-1" ),     9}),
    QSharedPointer<AbstractFigure>(new Bishop{          7, 1, AbstractFigure::Black, QLatin1String("lance-black-1" ),     10}),
    QSharedPointer<AbstractFigure>(new Pawn{            0, 2, AbstractFigure::Black, QLatin1String("pawn-black-1" ),      11}),
    QSharedPointer<AbstractFigure>(new Pawn{            1, 2, AbstractFigure::Black, QLatin1String("pawn-black-2" ),      12}),
    QSharedPointer<AbstractFigure>(new Pawn{            2, 2, AbstractFigure::Black, QLatin1String("pawn-black-3" ),      13}),
    QSharedPointer<AbstractFigure>(new Pawn{            3, 2, AbstractFigure::Black, QLatin1String("pawn-black-4" ),      14}),
    QSharedPointer<AbstractFigure>(new Pawn{            4, 2, AbstractFigure::Black, QLatin1String("pawn-black-5" ),      15}),
    QSharedPointer<AbstractFigure>(new Pawn{            5, 2, AbstractFigure::Black, QLatin1String("pawn-black-6" ),      16}),
    QSharedPointer<AbstractFigure>(new Pawn{            6, 2, AbstractFigure::Black, QLatin1String("pawn-black-7" ),      17}),
    QSharedPointer<AbstractFigure>(new Pawn{            7, 2, AbstractFigure::Black, QLatin1String("pawn-black-8" ),      18}),
    QSharedPointer<AbstractFigure>(new Pawn{            8, 2, AbstractFigure::Black, QLatin1String("pawn-black-9" ),      19}),

    QSharedPointer<AbstractFigure>(new Pawn{            0, 6, AbstractFigure::White, QLatin1String("pawn-white-1" ),      20}),
    QSharedPointer<AbstractFigure>(new Pawn{            1, 6, AbstractFigure::White, QLatin1String("pawn-white-2" ),      21}),
    QSharedPointer<AbstractFigure>(new Pawn{            2, 6, AbstractFigure::White, QLatin1String("pawn-white-3" ),      22}),
    QSharedPointer<AbstractFigure>(new Pawn{            3, 6, AbstractFigure::White, QLatin1String("pawn-white-4" ),      23}),
    QSharedPointer<AbstractFigure>(new Pawn{            4, 6, AbstractFigure::White, QLatin1String("pawn-white-5" ),      24}),
    QSharedPointer<AbstractFigure>(new Pawn{            5, 6, AbstractFigure::White, QLatin1String("pawn-white-6" ),      25}),
    QSharedPointer<AbstractFigure>(new Pawn{            6, 6, AbstractFigure::White, QLatin1String("pawn-white-7" ),      26}),
    QSharedPointer<AbstractFigure>(new Pawn{            7, 6, AbstractFigure::White, QLatin1String("pawn-white-8" ),      27}),
    QSharedPointer<AbstractFigure>(new Pawn{            8, 6, AbstractFigure::White, QLatin1String("pawn-white-9" ),      28}),
    QSharedPointer<AbstractFigure>(new Bishop{          1, 7, AbstractFigure::White, QLatin1String("bishop-white-1" ),    29}),
    QSharedPointer<AbstractFigure>(new Rook{            7, 7, AbstractFigure::White, QLatin1String("rook-white-1" ),      30}),
    QSharedPointer<AbstractFigure>(new Lance{           0, 8, AbstractFigure::White, QLatin1String("lance-white-1" ),     31}),
    QSharedPointer<AbstractFigure>(new Knight{          1, 8, AbstractFigure::White, QLatin1String("knight-white-1" ),    32}),
    QSharedPointer<AbstractFigure>(new SilverGeneral{   2, 8, AbstractFigure::White, QLatin1String("sGeneral-white-1" ),  33}),
    QSharedPointer<AbstractFigure>(new GoldGeneral{     3, 8, AbstractFigure::White, QLatin1String("gGeneral-white-1" ),  34}),
    QSharedPointer<AbstractFigure>(new King{            4, 8, AbstractFigure::White, QLatin1String("king-white-1" ),      35}),
    QSharedPointer<AbstractFigure>(new GoldGeneral{     5, 8, AbstractFigure::White, QLatin1String("gGeneral-white-2" ),  36}),
    QSharedPointer<AbstractFigure>(new SilverGeneral{   6, 8, AbstractFigure::White, QLatin1String("sGeneral-white-2" ),  37}),
    QSharedPointer<AbstractFigure>(new Knight{          7, 8, AbstractFigure::White, QLatin1String("knight-white-2" ),    38}),
    QSharedPointer<AbstractFigure>(new Lance{           8, 8, AbstractFigure::White, QLatin1String("lance-white-2" ),     39}),
  };

  return res;
}

void ShogiBoard::copyState(const AbstractBoard &other)
{
  AbstractBoard::copyState(other);


  const ShogiBoard *shogiBoard = reinterpret_cast<const ShogiBoard*>(&other);
  table_top.copyState(shogiBoard->table_top);
  table_bot.copyState(shogiBoard->table_bot);
}

QVariant ShogiBoard::data(const QModelIndex & index, int role) const
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

        if(f->isTransformed())
        {
          res.append(QLatin1String("P_"));
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

void ShogiBoard::clearColors()
{
  m_colors = {
    { Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None },
    { Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None },
    { Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None },
    { Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None },
    { Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None },
    { Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None },
    { Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None },
    { Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None },
    { Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None, Abstract::Color::None }
  };

  const QModelIndex from = index(0, 0);
  const QModelIndex to = index(ROWS * COLUMNS - 1, 0);

  Q_EMIT dataChanged(from, to);
}

void ShogiBoard::tableMove(uint32 fromX, uint32 fromY, uint32 toX, uint32 toY, ShogiTable *table)
{
  AbstractFigure *from = table->figure(fromX, fromY);

  if(from)
  {
    table->setFigure(fromY, fromX, nullptr);
    m_board[toY][toX] = from;

    from->setX(toX);
    from->setY(toY);
    from->allowTransformation(false);

    const QModelIndex fromIndex = index(fromY * _rowSize + fromX, 0);
    const QModelIndex toIndex = index(toY * _rowSize + toX, 0);

    Q_EMIT dataChanged(fromIndex, toIndex);
  }
}

} // END NAMESPACE "Shogi"
} // END NAMEPSACE "Stratogi"
