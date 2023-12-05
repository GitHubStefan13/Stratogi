#ifndef CHESSGAME_H
#define CHESSGAME_H

#include "signal.h"
#include "abstractGame.hpp"

#include "chessBoard.h"
#include "chessengine.h"
#include "chessfigures.h"

namespace Stratogi {
namespace Chess {

class ChessGame Q_DECL_FINAL : public AbstractGame
{
  Q_OBJECT

  public:
    ChessGame(ChessBoard &board, Signals &s);

    Q_INVOKABLE int turnsCount() const override;

    //! Handle castling.
    void handleCastling(uint32 x, uint32 y, AbstractFigure *figure, AbstractBoard &board);
    //! Handle transformation.
    bool handleTransformation();
    //! GUI informs us that the Game started, now its time to load the Engine and stuff.
    void gameStarted() override;
    //! Mark cell for hit.
    void markCellForHit(uint32 x, uint32 y, int32_t dx, int32_t dy, Move::Distance d, AbstractFigure * figure, AbstractBoard &tmpBoard) override;
    //! Check check.
    bool checkCheck(bool checkmate = false) override;
    //! Mark check.
    bool markCheck(King *king, AbstractFigure *figure, bool checkmate = false) override;
    //! Will be a check after move?
    bool isCheckAfterMove(uint32 x, uint32 y, AbstractFigure * figure, AbstractBoard &tmpBoard) const override;
    //! Check checkmate.
    bool isCheckMate() override;
    //! Check stalemate.
    bool isStaleMate() override;
    //! First click.
    void firstClick(uint32 x, uint32 y, void *user = nullptr) override;
    //! Second click.
    bool secondClick(uint32 x, uint32 y, void *user = nullptr) override;

    void turnEnded() override;
    void undo() override;

  public Q_SLOT:
    void newGame() override;
    void onEngineResult(QString result) override;
    void onEngineError(QString error, AbstractEngine::EngineErrorGrade errorGrade) override;

  protected:
    COORD_NOTATION convertEngineMove(QString engineMove) override;
    COORD_NOTATION convertDisplayMove(QString displayMove) override;
    QString convertToEngineMove(COORD_NOTATION pos) override;
    QString convertToDisplayMove(COORD_NOTATION pos) override;

  private:
    //! ChessBoard
    ChessBoard &chess_board;
    QStack<ChessBoard> m_turns;

    void searchDefeatedPieces() override;
};

}
} // END NAMEPSACE "Stratogi"

#endif // CHESSGAME_H
