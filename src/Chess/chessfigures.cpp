#include "chessfigures.h"

namespace Stratogi {
namespace Chess {

  Pawn::Pawn(uint32 xv, uint32 yv, FigureColor c, const QString & n, uint32 index)
    :	AbstractFigure( xv, yv, c, n, index )
    ,	m_isPass( false )
  {
  }

  Pawn::~Pawn()
  {
  }

  bool Pawn::isPass() const
  {
    return m_isPass;
  }

  void Pawn::setPass( bool on )
  {
    m_isPass = on;
  }

  const Pawn::Moves &Pawn::moves() const
  {
    return m_moves;
  }

  uint32_t Pawn::type() const
  {
    return FigureType::PawnFigure;
  }

  QSharedPointer<Stratogi::AbstractFigure> Pawn::copy() const
  {
    QSharedPointer<AbstractFigure> res(new Pawn(x(), y(), color(),
      name(), index()));
    res->firstMoveDone(isFirstMoveDone());
    static_cast<Pawn*>(res.data())->m_isPass = m_isPass;

    return res;
  }

  const Pawn::Moves Pawn::m_moves = {
    std::array< Move, 5 > {
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No }
    },

    std::array< Move, 5 > {
      Move{ Move::Unknown, Move::No },
      Move{ Move::Hit, Move::One },
      Move{ Move::Movement, Move::TwoFirstTime },
      Move{ Move::Hit, Move::One },
      Move{ Move::Unknown, Move::No }
    },

    std::array< Move, 5 > {
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No }
    },

    std::array< Move, 5 > {
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No }
    },

    std::array< Move, 5 > {
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No }
    }
  };


  Rook::Rook(uint32 xv, uint32 yv, FigureColor c, const QString & n, uint32 index)
    :	AbstractFigure( xv, yv, c, n, index )
  {
  }

  Rook::~Rook()
  {
  }

  const Rook::Moves &Rook::moves() const
  {
    return m_moves;
  }

  uint32_t Rook::type() const
  {
    return FigureType::RookFigure;
  }

  QSharedPointer< AbstractFigure > Rook::copy() const
  {
    QSharedPointer< AbstractFigure > res( new Rook( x(), y(), color(),
      name(), index() ) );
    res->firstMoveDone( isFirstMoveDone() );

    return res;
  }

  const Rook::Moves Rook::m_moves = {
    std::array< Move, 5 > {
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No }
    },

    std::array< Move, 5 > {
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{Move::MoveHit, Move::Any },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No }
    },

    std::array< Move, 5 > {
      Move{ Move::Unknown, Move::No },
      Move{ Move::MoveHit, Move::Any },
      Move{ Move::Unknown, Move::No },
      Move{ Move::MoveHit, Move::Any },
      Move{ Move::Unknown, Move::No }
    },

    std::array< Move, 5 > {
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::MoveHit, Move::Any },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No }
    },

    std::array< Move, 5 > {
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No }
    }
  };


//
// Knight
//

  Knight::Knight(uint32 xv, uint32 yv, FigureColor c, const QString & n, uint32 index)
    :	AbstractFigure( xv, yv, c, n, index )
  {
  }

  Knight::~Knight()
  {
  }

  const Knight::Moves &Knight::moves() const
  {
    return m_moves;
  }

  uint32_t Knight::type() const
  {
    return FigureType::KnightFigure;
  }

  QSharedPointer<AbstractFigure> Knight::copy() const
  {
    QSharedPointer< AbstractFigure > res( new Knight( x(), y(), color(),
      name(), index() ) );
    res->firstMoveDone( isFirstMoveDone() );

    return res;
  }

  const Knight::Moves Knight::m_moves = {
  std::array< Move, 5 >{ Move{ Move::Unknown, Move::No },
    Move{ Move::Movement | Move::Hit, Move::One },
    Move{ Move::Unknown, Move::No },
    Move{ Move::Movement | Move::Hit, Move::One },
    Move{ Move::Unknown, Move::No } },

  std::array< Move, 5 > { Move{ Move::Movement | Move::Hit, Move::One },
    Move{ Move::Unknown, Move::No },
    Move{ Move::Unknown, Move::No }, Move{ Move::Unknown, Move::No },
    Move{ Move::Movement | Move::Hit, Move::One } },

  std::array< Move, 5 > { Move{ Move::Unknown, Move::No },
    Move{ Move::Unknown, Move::No },
    Move{ Move::Unknown, Move::No }, Move{ Move::Unknown, Move::No },
    Move{ Move::Unknown, Move::No } },

  std::array< Move, 5 > { Move{ Move::Movement | Move::Hit, Move::One },
    Move{ Move::Unknown, Move::No },
    Move{ Move::Unknown, Move::No }, Move{ Move::Unknown, Move::No },
    Move{ Move::Movement | Move::Hit, Move::One } },

  std::array< Move, 5 > { Move{ Move::Unknown, Move::No },
    Move{ Move::Movement | Move::Hit, Move::One },
    Move{ Move::Unknown, Move::No },
    Move{ Move::Movement | Move::Hit, Move::One },
    Move{ Move::Unknown, Move::No } }
};


//
// Bishop
//


  Bishop::Bishop(uint32 xv, uint32 yv, FigureColor c, const QString & n, uint32 index)
    :	AbstractFigure( xv, yv, c, n, index )
  {
  }

  Bishop::~Bishop()
  {
  }

  const Bishop::Moves &Bishop::moves() const
  {
    return m_moves;
  }

  uint32_t Bishop::type() const
  {
    return FigureType::BishopFigure;
  }

  QSharedPointer<AbstractFigure> Bishop::copy() const
  {
    QSharedPointer< AbstractFigure > res( new Bishop( x(), y(), color(),
      name(), index() ) );
    res->firstMoveDone( isFirstMoveDone() );

    return res;
  }

  const Bishop::Moves Bishop::m_moves = {
    std::array<Move, 5>{
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No }
    },

    std::array<Move, 5>{
      Move{ Move::Unknown, Move::No },
      Move{ Move::MoveHit, Move::Any },
      Move{ Move::Unknown, Move::No },
      Move{ Move::MoveHit, Move::Any },
      Move{ Move::Unknown, Move::No }
    },

    std::array<Move, 5>{
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No }
    },

    std::array<Move, 5>{
      Move{ Move::Unknown, Move::No },
      Move{ Move::MoveHit, Move::Any },
      Move{ Move::Unknown, Move::No },
      Move{ Move::MoveHit, Move::Any },
      Move{ Move::Unknown, Move::No }
    },

    std::array<Move, 5>{
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No }
    }
  };


//
// Queen
//

  Queen::Queen(uint32 xv, uint32 yv, FigureColor c, const QString & n, uint32 index)
    :	AbstractFigure( xv, yv, c, n, index )
  {
  }

  Queen::~Queen()
  {
  }

  const Queen::Moves &Queen::moves() const
  {
    return m_moves;
  }

  uint32_t Queen::type() const
  {
    return FigureType::QueenFigure;
  }

  QSharedPointer<AbstractFigure> Queen::copy() const
  {
    QSharedPointer< AbstractFigure > res( new Queen( x(), y(), color(),
      name(), index() ) );
    res->firstMoveDone( isFirstMoveDone() );

    return res;
  }

  const Queen::Moves Queen::m_moves = {
    std::array<Move, 5>{
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No }
    },

    std::array<Move, 5>{
      Move{ Move::Unknown, Move::No },
      Move{ Move::Movement | Move::Hit, Move::Any },
      Move{ Move::Movement | Move::Hit, Move::Any },
      Move{ Move::Movement | Move::Hit, Move::Any },
      Move{ Move::Unknown, Move::No }
    },

    std::array<Move, 5>{
      Move{ Move::Unknown, Move::No },
      Move{ Move::Movement | Move::Hit, Move::Any },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Movement | Move::Hit, Move::Any },
      Move{ Move::Unknown, Move::No }
    },

    std::array<Move, 5>{
      Move{ Move::Unknown, Move::No },
      Move{ Move::Movement | Move::Hit, Move::Any },
      Move{ Move::Movement | Move::Hit, Move::Any },
      Move{ Move::Movement | Move::Hit, Move::Any },
      Move{ Move::Unknown, Move::No }
    },

    std::array<Move, 5>{
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No }
    }
  };

} // END NAMESPACE "Chess"
} // END NAMEPSACE "Stratogi"
