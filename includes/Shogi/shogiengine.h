#ifndef SHOGIENGINE_H
#define SHOGIENGINE_H

#include "abstractEngine.hpp"

namespace Stratogi {
namespace Shogi {

  class ShogiEngine : public AbstractEngine
  {
    public:
      ShogiEngine();
      virtual ~ShogiEngine() override;

      bool event(QEvent *event) override;
      bool eventFilter(QObject *watched, QEvent *event) override;

      void loadEngine() override;
      void calcNextEngineMove() override;
      bool engineIsMoveValid(QString move) override;
      void startNewGame() override;

  };

} // END NAMESPACE "Shogi"
} // END NAMEPSACE "Stratogi"

#endif // SHOGIENGINE_H
