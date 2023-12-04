#include "abstractFigure.hpp"

namespace Stratogi {

Move::Move(Types t, Distance d)
  :	m_types(t)
  ,	m_dist(d)
{
}

Move::~Move()
{
}

Move::Types Move::types() const
{
	return m_types;
}

Move::Distance Move::dist() const
{
	return m_dist;
}

AbstractFigure::AbstractFigure(uint32 xv, uint32 yv, FigureColor c, const QString & n, uint32 index)
    :m_x(xv)
    ,m_y(yv)
    ,m_c(c)
    ,m_name(n)
    ,m_transformed(false)
    ,m_firstMoveDone(false)
    ,m_canTransform(true)
    ,m_index(index)
{
}

uint32 AbstractFigure::index() const
{
  return m_index;
}

uint32 AbstractFigure::move_type() const
{
  return m_move_type;
}

void AbstractFigure::setMoveType(uint32 value)
{
  m_move_type = value;
}

uint32 AbstractFigure::transformation_type() const
{
  return m_transformation_type;
}

void AbstractFigure::setTransformationType(uint32 value)
{
  m_transformation_type = value;
}

AbstractFigure::~AbstractFigure()
{
}

AbstractFigure *AbstractFigure::figure()
{
  return this;
}

bool AbstractFigure::isKing()
{
  return false;
}

uint32 AbstractFigure::x() const
{
  return m_x;
}

void AbstractFigure::setX(uint32 value)
{
  m_x = value;
}

uint32 AbstractFigure::y() const
{
  return m_y;
}

void AbstractFigure::setY(uint32 value)
{
  m_y = value;
}

AbstractFigure::FigureColor AbstractFigure::color() const
{
	return m_c;
}

const QString &AbstractFigure::name() const
{
	return m_name;
}

bool AbstractFigure::isFirstMoveDone() const
{
	return m_firstMoveDone;
}

void AbstractFigure::firstMoveDone(bool done)
{
  m_firstMoveDone = done;
}

bool AbstractFigure::isTransformed() const
{
  return m_transformed;
}

void AbstractFigure::transform(bool transform)
{
  m_transformed = transform;
}

bool AbstractFigure::isAllowedTransformation() const
{
  return m_canTransform;
}

void AbstractFigure::allowTransformation(bool transform)
{
  m_canTransform = transform;
}

void AbstractFigure::setColor(AbstractFigure::FigureColor newColor)
{
  m_c = newColor;
}

King::King(uint32 xv, uint32 yv, FigureColor c, const QString & n, uint32 index) :	AbstractFigure(xv, yv, c, n, index)
{
}

King::~King()
{
}

const King::Moves &King::moves() const
{
  return m_moves;
}

uint32_t King::type() const
{
  return Abstract::FigureType::KingFigure;
}

bool King::isKing()
{
  return true;
}

QSharedPointer<AbstractFigure> King::copy() const
{
  QSharedPointer<AbstractFigure>res(new King(x(), y(), color(), name(), index()));
  res->firstMoveDone(isFirstMoveDone());

  return res;
}

const King::Moves King::m_moves = {
  std::array<Move, 5>{
    Move{ Move::Unknown, Move::No },
    Move{ Move::Unknown, Move::No },
    Move{ Move::Unknown, Move::No },
    Move{ Move::Unknown, Move::No },
    Move{ Move::Unknown, Move::No }
  },

  std::array<Move, 5>{
    Move{Move::Unknown, Move::No},
    Move{Move::Movement | Move::Hit, Move::One},
    Move{Move::Movement | Move::Hit, Move::One},
    Move{Move::Movement | Move::Hit, Move::One},
    Move{Move::Unknown, Move::No}
  },

  std::array<Move, 5>{
    Move{Move::Unknown, Move::No},
    Move{Move::Movement | Move::Hit, Move::One},
    Move{Move::Unknown, Move::No},
    Move{Move::Movement | Move::Hit, Move::One},
    Move{Move::Unknown, Move::No}
  },

  std::array<Move, 5>{
    Move{Move::Unknown, Move::No},
    Move{Move::Movement | Move::Hit, Move::One},
    Move{Move::Movement | Move::Hit, Move::One},
    Move{Move::Movement | Move::Hit, Move::One},
    Move{Move::Unknown, Move::No}
  },

  std::array<Move, 5>{
    Move{Move::Unknown, Move::No},
    Move{Move::Unknown, Move::No},
    Move{Move::Unknown, Move::No},
    Move{Move::Unknown, Move::No},
    Move{Move::Unknown, Move::No}
  }
};

} // End Namespace Stratogi

