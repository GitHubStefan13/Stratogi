#ifndef SHOGIFIGURES_H
#define SHOGIFIGURES_H

#include <QtDebug>

#include "abstractFigure.hpp"
#include "figuretypes.h"

namespace Stratogi {
namespace Shogi {

  // Since most of the Figures actually use the Gold General Moves when promoted
  // so we can just save us some time and implement the promoted MovePattern for each
  // Promotion Type(General, Rook, Bishop)
  class AbstractPromoted : public AbstractFigure
  {
    public:
      AbstractPromoted(uint32 xv, uint32 yv, FigureColor c, const QString & n, uint32 index);
      void promote();
    protected:
       //! \brief Moves of the Promoted Figure, is set to Gold General moves
      static const Moves gGeneral_Moves;
      //! \brief Moves of the Promoted Rook, is set to Dragon moves
      static const Moves Dragon_Moves;
      //! \brief Moves of the Promoted Bishop, is set to Horse moves
      static const Moves Horse_Moves;
  };

  //! Pawn.
  class Pawn : public AbstractPromoted
  {
    public:
      Pawn(uint32 xv, uint32 yv, FigureColor c, const QString & n, uint32 index);
      virtual ~Pawn() override;

      //! \return Possible moves.
      const Moves & moves() const override;

      //! \return Type of figure.
      uint32_t type() const override;

      //! Copy.
      QSharedPointer<AbstractFigure> copy() const override;

    private:
      static const Moves m_moves;

  }; // class Pawn

  //! Rook.
  class Rook : public AbstractPromoted
  {
    public:
      Rook(uint32 xv, uint32 yv, FigureColor c, const QString & n, uint32 index);
      virtual ~Rook() override;

      //! \return Possible moves.
      const Moves & moves() const override;

      //! \return Type of figure.
      virtual uint32_t type() const override;

      //! Copy.
      QSharedPointer<AbstractFigure> copy() const override;

    private:
      static const Moves m_moves;

  }; // class Castle

  //! Knight.
  class Knight Q_DECL_FINAL : public AbstractPromoted
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
  class Bishop Q_DECL_FINAL : public AbstractPromoted
  {
    public:
      Bishop(uint32 xv, uint32 yv, FigureColor c, const QString & n, uint32 index);
      virtual ~Bishop() override;

      //! \return Possible moves.
      const Moves & moves() const override;

      //! \return Type of figure.
      virtual uint32_t type() const override;

      //! Copy.
      QSharedPointer<AbstractFigure> copy() const override;

    private:
      static const Moves m_moves;
  }; // class

  //! Lance.
  class Lance Q_DECL_FINAL :	public AbstractPromoted
  {
    public:
      Lance(uint32 xv, uint32 yv, FigureColor c, const QString & n, uint32 index);
      virtual ~Lance() override;

      //! \return Possible moves.
      const Moves & moves() const override;

      //! \return Type of figure.
      virtual uint32_t type() const override;

      //! Copy.
      QSharedPointer<AbstractFigure> copy() const override;

    private:
      static const Moves m_moves;
  }; // class Lance

  //! SilverGeneral.
  class SilverGeneral Q_DECL_FINAL :	public AbstractPromoted
  {
    public:
      SilverGeneral(uint32 xv, uint32 yv, FigureColor c, const QString & n, uint32 index);
      virtual ~SilverGeneral() override;

      //! \return Possible moves.
      const Moves & moves() const override;

      //! \return Type of figure.
      virtual uint32_t type() const override;

      //! Copy.
      QSharedPointer<AbstractFigure> copy() const override;

    private:
      static const Moves m_moves;
  }; // class SilverGeneral

  //! GoldGeneral.
  class GoldGeneral Q_DECL_FINAL :	public AbstractFigure
  {
    public:
      GoldGeneral(uint32 xv, uint32 yv, FigureColor c, const QString & n, uint32 index);
      virtual ~GoldGeneral() override;

      //! \return Possible moves.
      const Moves & moves() const override;

      //! \return Type of figure.
      uint32_t type() const override;

      //! Copy.
      QSharedPointer<AbstractFigure> copy() const override;

    private:
      static const Moves m_moves;
  }; // class GoldGeneral

} // END NAMESPACE "Shogi"
} // END NAMEPSACE "Stratogi"

#endif // SHOGIFIGURES_H

