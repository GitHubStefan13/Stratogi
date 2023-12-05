#ifndef CHESSENGINE_H
#define CHESSENGINE_H

#include "abstractEngine.hpp"

namespace Stratogi {
namespace Chess {

  class ChessEngine : public AbstractEngine
  {
    public:
      ChessEngine();
      virtual ~ChessEngine() override;

      bool event(QEvent *event) override;
      bool eventFilter(QObject *watched, QEvent *event) override;

      void loadEngine() override;
      void unloadEngine() override;
      void calcNextEngineMove() override;
      bool engineIsMoveValid(QString move) override;
      void startNewGame() override;
  };

} // END NAMESPACE "Chess"
} // END NAMEPSACE "Stratogi"

#endif //  CHESSENGINE_H
