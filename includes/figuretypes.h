#ifndef FIGURETYPES_H
#define FIGURETYPES_H

namespace Stratogi {

  namespace Abstract {

    enum FigureType {
      KingFigure
    };

    enum MovementType {
      Normal,
      Defeat
    };

    enum TransformationType {
      No_Tranformation
    };

    enum Color {
      Blue,
      Red,
      Check,
      None
    }; // enum Color
  }


  namespace Chess {

    enum FigureType {
      KingFigure,
      QueenFigure,
      BishopFigure,
      KnightFigure,
      RookFigure,
      PawnFigure
    };

    enum MovementType {
      Normal,
      Defeat
    };

    enum TransformationType {
      No_Tranformation,
      Transform_to_Queen,
      Transform_to_Rook,
      Transform_to_Knight,
      Transform_to_Bishop
    };

  }

  namespace Shogi {

    enum FigureType {
      KingFigure,            // No Promotion
      GoldGeneralFigure,     // No Promotion

      RookFigure,            // Promotes to Dragon
      BishopFigure,          // Promotes to Bishop
      SilverGeneralFigure,   // Promotes to "Promoted Silver", same Moves as Gold General
      KnightFigure,          // Promotes to "Promoted Knight", same Moves as Gold General
      LanceFigure,           // Promotes to "Promoted Lance",  same Moves as Gold General
      PawnFigure             // Promotes to "Tokin",           same Moves as Gold General
    };

    enum MovementType {
      Normal,
      Capture,
      Drop
    };

    enum TransformationType {
      No_Tranformation,
      Accepted,
      Declined
    };

  }

} // END NAMEPSACE "Stratogi"

#endif // FIGURETYPES_H
