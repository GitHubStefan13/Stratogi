#include "shogifigures.h"

namespace Stratogi {
namespace Shogi {

  AbstractPromoted::AbstractPromoted(uint32 xv, uint32 yv, AbstractFigure::FigureColor c, const QString &n, uint32 index)
    :	AbstractFigure( xv, yv, c, n, index )
  {
  }

  void AbstractPromoted::promote()
  {
    qDebug() << this->name() << ": is getting a promotation. Keep up the good work!";
    transform();
  }

  const AbstractPromoted::Moves AbstractPromoted::gGeneral_Moves = {
    std::array<Move, 5>{
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No }
    },

    std::array<Move, 5>{
      Move{ Move::Unknown, Move::No },
      Move{ Move::MoveHit, Move::One },
      Move{ Move::MoveHit, Move::One },
      Move{ Move::MoveHit, Move::One },
      Move{ Move::Unknown, Move::No }
    },

    std::array<Move, 5>{
      Move{ Move::Unknown, Move::No },
      Move{ Move::MoveHit, Move::One },
      Move{ Move::Unknown, Move::No },
      Move{ Move::MoveHit, Move::One },
      Move{ Move::Unknown, Move::No }
    },

    std::array<Move, 5>{
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::MoveHit, Move::One },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No}
    },

    std::array<Move, 5>{
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No }
    }
  };

  const AbstractPromoted::Moves AbstractPromoted::Dragon_Moves = {
    std::array< Move, 5 > {
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No }
    },

    std::array< Move, 5 > {
      Move{ Move::Unknown, Move::No },
      Move{ Move::MoveHit, Move::One },
      Move{ Move::MoveHit, Move::Any },
      Move{ Move::MoveHit, Move::One },
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
      Move{ Move::MoveHit, Move::One },
      Move{ Move::MoveHit, Move::Any },
      Move{ Move::MoveHit, Move::One },
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


  const AbstractPromoted::Moves AbstractPromoted::Horse_Moves = {
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
      Move{ Move::MoveHit, Move::One },
      Move{ Move::MoveHit, Move::Any },
      Move{ Move::Unknown, Move::No }
    },

    std::array<Move, 5>{
      Move{ Move::Unknown, Move::No },
      Move{ Move::MoveHit, Move::One },
      Move{ Move::Unknown, Move::No },
      Move{ Move::MoveHit, Move::One },
      Move{ Move::Unknown, Move::No }
    },

    std::array<Move, 5>{
      Move{ Move::Unknown, Move::No },
      Move{ Move::MoveHit, Move::Any },
      Move{ Move::MoveHit, Move::One },
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

// *************************************************************
// ************************  Pawn  *****************************
// *************************************************************

  Pawn::Pawn(uint32 xv, uint32 yv, FigureColor c, const QString & n, uint32 index)
    :	AbstractPromoted(xv, yv, c, n, index)
  {
  }

  Pawn::~Pawn()
  {
  }

  const Pawn::Moves &Pawn::moves() const
  {
    if(isTransformed())
      return gGeneral_Moves;
    else
      return m_moves;
  }

  uint32_t Pawn::type() const
  {
    return FigureType::PawnFigure;
  }

  QSharedPointer<AbstractFigure> Pawn::copy() const
  {
    QSharedPointer<AbstractFigure> res(new Pawn(x(), y(), color(),
      name(), index()));
    res->firstMoveDone(isFirstMoveDone());

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
      Move{ Move::Unknown,  Move::No },
      Move{ Move::Unknown,  Move::No },
      Move{ Move::MoveHit, Move::One },
      Move{ Move::Unknown,  Move::No },
      Move{ Move::Unknown,  Move::No }
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

// *************************************************************
// ************************  Rook  *****************************
// *************************************************************

  Rook::Rook(uint32 xv, uint32 yv, FigureColor c, const QString & n, uint32 index)
    :	AbstractPromoted( xv, yv, c, n, index )
  {
  }

  Rook::~Rook()
  {
  }

  const Rook::Moves &Rook::moves() const
  {
    if(isTransformed())
      return Dragon_Moves;
    else
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
      Move{ Move::MoveHit, Move::Any },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No }
    },

    std::array< Move, 5 > {
      Move{ Move::Unknown, Move::No },
      Move{ Move::Movement | Move::Hit, Move::Any },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Movement | Move::Hit, Move::Any },
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

// *************************************************************
// ************************ Knight *****************************
// *************************************************************

  Knight::Knight(uint32 xv, uint32 yv, FigureColor c, const QString & n, uint32 index)
    :	AbstractPromoted( xv, yv, c, n, index )
  {
  }

  Knight::~Knight()
  {
  }

  const Knight::Moves &Knight::moves() const
  {
    if(isTransformed())
      return gGeneral_Moves;
    else
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
    std::array<Move, 5>{
      Move{ Move::Unknown, Move::No },
      Move{ Move::MoveHit, Move::One },
      Move{ Move::Unknown, Move::No },
      Move{ Move::MoveHit, Move::One },
      Move{ Move::Unknown, Move::No }
    },

    std::array<Move, 5>{
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No }
    },

    std::array<Move, 5> {
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
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
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No }
    }
  };

// *************************************************************
// ************************ Bishop *****************************
// *************************************************************

  Bishop::Bishop(uint32 xv, uint32 yv, FigureColor c, const QString & n, uint32 index)
    :	AbstractPromoted( xv, yv, c, n, index )
  {
  }

  Bishop::~Bishop()
  {
  }

  const Bishop::Moves &Bishop::moves() const
  {
    if(isTransformed())
      return Horse_Moves;
    else
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

// *************************************************************
// ************************ Lance *****************************
// *************************************************************

  Lance::Lance(uint32 xv, uint32 yv, FigureColor c, const QString & n, uint32 index)
    :	AbstractPromoted( xv, yv, c, n, index )
  {
  }

  Lance::~Lance()
  {
  }

  const Lance::Moves &Lance::moves() const
  {
    if(isTransformed())
      return gGeneral_Moves;
    else
      return m_moves;
  }

  uint32_t Lance::type() const
  {
    return FigureType::LanceFigure;
  }

  QSharedPointer<AbstractFigure> Lance::copy() const
  {
    QSharedPointer< AbstractFigure > res( new Lance( x(), y(), color(),
      name(), index() ) );
    res->firstMoveDone( isFirstMoveDone() );

    return res;
  }

  const Lance::Moves Lance::m_moves = {
    std::array<Move, 5>{
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No }
    },

    std::array< Move, 5 > {
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::MoveHit, Move::Any },
      Move{ Move::Unknown, Move::No },
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
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
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

  // *************************************************************
  // ******************** Silver General *************************
  // *************************************************************

  SilverGeneral::SilverGeneral(uint32 xv, uint32 yv, FigureColor c, const QString & n, uint32 index)
    :	AbstractPromoted( xv, yv, c, n, index )
  {
  }

  SilverGeneral::~SilverGeneral()
  {
  }

  const SilverGeneral::Moves &SilverGeneral::moves() const
  {
    if(isTransformed())
      return gGeneral_Moves;
    else
      return m_moves;
  }

  uint32_t SilverGeneral::type() const
  {
    return FigureType::SilverGeneralFigure;
  }

  QSharedPointer<AbstractFigure> SilverGeneral::copy() const
  {
    QSharedPointer< AbstractFigure > res( new SilverGeneral( x(), y(), color(),
      name(), index() ) );
    res->firstMoveDone( isFirstMoveDone() );

    return res;
  }

  const SilverGeneral::Moves SilverGeneral::m_moves = {
    std::array<Move, 5>{
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No }
    },

    std::array<Move, 5>{
      Move{ Move::Unknown, Move::No },
      Move{ Move::MoveHit, Move::One },
      Move{ Move::MoveHit, Move::One },
      Move{ Move::MoveHit, Move::One },
      Move{ Move::Unknown, Move::No}
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
      Move{ Move::MoveHit, Move::One },
      Move{ Move::Unknown, Move::One },
      Move{ Move::MoveHit, Move::One },
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

  // *************************************************************
  // ********************** Gold General *************************
  // *************************************************************

  GoldGeneral::GoldGeneral(uint32 xv, uint32 yv, FigureColor c, const QString & n, uint32 index)
    :	AbstractFigure( xv, yv, c, n, index )
  {
  }

  GoldGeneral::~GoldGeneral()
  {
  }

  const GoldGeneral::Moves &GoldGeneral::moves() const
  {
    return m_moves;
  }

  uint32_t GoldGeneral::type() const
  {
    return FigureType::GoldGeneralFigure;
  }

  QSharedPointer<AbstractFigure> GoldGeneral::copy() const
  {
    QSharedPointer< AbstractFigure > res( new GoldGeneral( x(), y(), color(),
      name(), index() ) );
    res->firstMoveDone( isFirstMoveDone() );

    return res;
  }

  const GoldGeneral::Moves GoldGeneral::m_moves = {
    std::array<Move, 5>{
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No }
    },

    std::array<Move, 5>{
      Move{ Move::Unknown, Move::No },
      Move{ Move::MoveHit, Move::One },
      Move{ Move::MoveHit, Move::One },
      Move{ Move::MoveHit, Move::One },
      Move{ Move::Unknown, Move::No }
    },

    std::array<Move, 5>{
      Move{ Move::Unknown, Move::No },
      Move{ Move::MoveHit, Move::One },
      Move{ Move::Unknown, Move::No },
      Move{ Move::MoveHit, Move::One },
      Move{ Move::Unknown, Move::No }
    },

    std::array<Move, 5>{
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::MoveHit, Move::One },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No}
    },

    std::array<Move, 5>{
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No },
      Move{ Move::Unknown, Move::No }
    }
  };

} // END NAMESPACE "Shogi"
} // END NAMEPSACE "Stratogi"
