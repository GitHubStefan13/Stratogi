#ifndef SHOGIGAME_H
#define SHOGIGAME_H

#include "signal.h"
#include "abstractGame.hpp"

#include "shogiBoard.h"
#include "shogiengine.h"
#include "shogifigures.h"

namespace Stratogi {
namespace Shogi {

class ShogiGame Q_DECL_FINAL : public AbstractGame
{
  Q_OBJECT

  public:
    ShogiGame(ShogiBoard &board, Signals &s);

    //! Kifu Notation for the Figure-Types, while not Promoted in Western Form
    const QString KIFU_PAWN_ENG   = "P";
    const QString KIFU_LANCE_ENG  = "L";
    const QString KIFU_KNIGHT_ENG = "N";
    const QString KIFU_SILVER_ENG = "S";
    const QString KIFU_GOLD_ENG   = "G";
    const QString KIFU_BISHOP_ENG = "B";
    const QString KIFU_ROOK_ENG   = "R";
    const QString KIFU_KING_ENG   = "K";
    //! Kifu Notation for the Figure-Types, while Promoted in Western Form
    const QString KIFU_P_PAWN_ENG   = "+P";
    const QString KIFU_P_LANCE_ENG  = "+L";
    const QString KIFU_P_KNIGHT_ENG = "+N";
    const QString KIFU_P_SILVER_ENG = "+S";
    const QString KIFU_P_GOLD_ENG   = "+G";
    const QString KIFU_P_BISHOP_ENG = "H";
    const QString KIFU_P_ROOK_ENG   = "D";
    const QString KIFU_P_KING_ENG   = "K";
    //! Kifu Notation for the Figure-Types, while not Promoted in Original Form
    const QString KIFU_PAWN_JAP   = "?";
    const QString KIFU_LANCE_JAP  = "?";
    const QString KIFU_KNIGHT_JAP = "?";
    const QString KIFU_SILVER_JAP = "?";
    const QString KIFU_GOLD_JAP   = "?";
    const QString KIFU_BISHOP_JAP = "?";
    const QString KIFU_ROOK_JAP   = "?";
    const QString KIFU_KING_JAP   = "?";
    //! Kifu Notation for the Figure-Types, while Promoted in Original Form
    const QString KIFU_P_PAWN_JAP   = "?";
    const QString KIFU_P_LANCE_JAP  = "?";
    const QString KIFU_P_KNIGHT_JAP = "?";
    const QString KIFU_P_SILVER_JAP = "?";
    const QString KIFU_P_GOLD_JAP   = "?";
    const QString KIFU_P_BISHOP_JAP = "?";
    const QString KIFU_P_ROOK_JAP   = "?";
    const QString KIFU_P_KING_JAP   = "?";
    //! Kifu Move Notation for the Move Type
    const QString KIFU_MOVE_NORMAL  = "-";
    const QString KIFU_MOVE_CAPTURE = "x";
    const QString KIFU_MOVE_DROP    = "*";
    //! Kifu Promotion Notation for the Promotion Type
    const QString KIFU_PROMOTE_ACCEPT  = "+";
    const QString KIFU_PROMOTE_DECLINE = "=";

    Q_INVOKABLE int turnsCount() const override;

    //! Handle transformation.
    bool handlePromotion();
    //! GUI informs us that the Game started, now its time to load the Engine and stuff.
    void gameStarted() override;
    //! Check for Check
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
    void firstClick(const uint32 x, const uint32 y, void *user = nullptr) override;
    //! Second click.
    bool secondClick(const uint32 x, const uint32 y, void *user = nullptr) override;

    void newGame() override;
    void onEngineResult(QString result) override;
    void onEngineError(QString error, AbstractEngine::EngineErrorGrade errorGrade) override;
    void turnEnded() override;
    void undo() override;

  Q_SIGNALS:
    void enginePositionSet();
    void engineValidMoves(QString result);

  protected:
    COORD_NOTATION convertEngineMove(QString engineMove) override;
    COORD_NOTATION convertDisplayMove(QString displayMove) override;
    QString convertToEngineMove(COORD_NOTATION pos) override;
    QString convertToDisplayMove(COORD_NOTATION pos) override;

  private:    
    //! ShogiBoard
    ShogiBoard &shogi_board;
    //! Previous boards.
    QStack<ShogiBoard> m_turns;

    void searchDefeatedPieces() override;

    void topTableClick(FigureType type);
    void topTableClick(const uint32_t x, const uint32_t y);
    void bottomTableClick(FigureType type);
    void bottomTableClick(const uint32_t x, const uint32_t y);

    void calcTableMoves(AbstractFigure *figure, AbstractBoard &tmpAbstractBoard);

    FigureType getFigureType(QString type, bool promoted);
    QString getFigureNotation(uint32_t type, bool promoted);
    QString getMovementNotation(uint32_t move_type);
    QString getPromotionNotation(uint32_t promotion_type);
    AbstractFigure *getFirstTableFigure(FigureType type);
};

}
} // END NAMEPSACE "Stratogi"

#endif // SHOGIGAME_H
