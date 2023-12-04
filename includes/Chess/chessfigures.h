#ifndef CHESSFIGURES_H
#define CHESSFIGURES_H

#include "abstractFigure.hpp"
#include "figuretypes.h"

namespace Stratogi {
namespace Chess {

  //! Pawn.
  class Pawn Q_DECL_FINAL	:	public AbstractFigure
  {
    public:
      Pawn(uint32 xv, uint32 yv, FigureColor c, const QString & n, uint32 index);
      virtual ~Pawn() override;

      //! \return Possible moves.
      const Moves & moves() const override;

      //! \return Type of figure.
      uint32_t type() const override;

      //! Is pass?
      bool isPass() const;
      //! Set pass.
      void setPass( bool on = true );

      //! Copy.
      QSharedPointer<AbstractFigure> copy() const override;

    private:
      static const Moves m_moves;
      //! Is aisle?
      bool m_isPass;
  }; // class Pawn

  //! Rook.
  class Rook Q_DECL_FINAL :	public AbstractFigure
  {
    public:
      Rook(uint32 xv, uint32 yv, FigureColor c, const QString & n, uint32 index);
      virtual ~Rook() override;

      //! \return Possible moves.
      const Moves & moves() const override;

      //! \return Type of figure.
      uint32_t type() const override;

      //! Copy.
      QSharedPointer<AbstractFigure> copy() const override;

    private:
      static const Moves m_moves;
  }; // class Castle

  //! Knight.
  class Knight Q_DECL_FINAL :	public AbstractFigure
  {
    public:
      Knight(uint32 xv, uint32 yv, FigureColor c, const QString & n, uint32 index);
      virtual ~Knight() override;

      //! \return Possible moves.
      const Moves & moves() const override;

      //! \return Type of figure.
      uint32_t type() const override;

      //! Copy.
      QSharedPointer<AbstractFigure> copy() const override;

    private:
      static const Moves m_moves;
  }; // class Knight

  //! Bishop.
  class Bishop Q_DECL_FINAL :	public AbstractFigure
  {
    public:
      Bishop(uint32 xv, uint32 yv, FigureColor c, const QString & n, uint32 index);
      virtual ~Bishop() override;

      //! \return Possible moves.
      const Moves & moves() const override;

      //! \return Type of figure.
      uint32_t type() const override;

      //! Copy.
      QSharedPointer<AbstractFigure> copy() const override;

    private:
      static const Moves m_moves;
  }; // class

  //! Queen.
  class Queen Q_DECL_FINAL :	public AbstractFigure
  {
    public:
      Queen(uint32 xv, uint32 yv, FigureColor c, const QString &n, uint32 index);
      virtual ~Queen() override;

      //! \return Possible moves.
      const Moves & moves() const override;

      //! \return Type of figure.
      uint32_t type() const override;

      //! Copy.
      QSharedPointer<AbstractFigure> copy() const override;

    private:
      static const Moves m_moves;
  }; // class Queen

} // END NAMESPACE "Chess"
} // END NAMEPSACE "Stratogi"

#endif // CHESSFIGURES_H
