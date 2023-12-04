#include "shogigame.h"

#include <QThread>

namespace Stratogi {
namespace Shogi {

  ShogiGame::ShogiGame(ShogiBoard &board, Signals &s) : AbstractGame (board, s), shogi_board(board)
  {
    m_engineThread = new ShogiEngine();
    m_turns.append(shogi_board);
    m_engineColor = AbstractFigure::FigureColor::Black;

    // Insert all the Pieces that can be defeated in this GameMode
    m_defeatedPieces.insert("Pawn", 0);
    m_defeatedPieces.insert("Rook", 0);
    m_defeatedPieces.insert("Bishop", 0);
    m_defeatedPieces.insert("Knight", 0);
    m_defeatedPieces.insert("Lance", 0);
    m_defeatedPieces.insert("SilverGeneral", 0);
    m_defeatedPieces.insert("GoldGeneral", 0);
  }

  int ShogiGame::turnsCount() const
  {
      return m_turns.size();
  }

  void ShogiGame::firstClick(const uint32 x, const uint32 y, void *user)
  {
    Q_UNUSED(user);

    if(m_guiLocked)
      return;

    if(x >= ShogiTable::OFFSET_TOPTABLE && y >= ShogiTable::OFFSET_TOPTABLE) // Click for the Tables
    {
      if(isEngineMove)
      {
        FigureType *type = static_cast<FigureType*>(user);
        if(type == nullptr)
          return;

        if(m_engineColor ==  AbstractFigure::FigureColor::White)
        {
          bottomTableClick(*type);
        }
        else
        {
          topTableClick(*type);
        }
      }
      else
      {
        if(x >= ShogiTable::OFFSET_BOTTOMTABLE && y >= ShogiTable::OFFSET_BOTTOMTABLE) // Click in Bottom Table
          bottomTableClick(y, x);
        else // Click in Top Table
          topTableClick(y, x);
      }

      if(m_selected)
      {
        m_selected->setMoveType(MovementType::Drop);
        m_selected->setTransformationType(TransformationType::No_Tranformation);
      }
    }
    else
    {
      AbstractGame::firstClick(x, y); // Click in the Board, can be handled by the Abstract Click

      if(m_selected)
      {
        // Set Defaults, can't know that until we confirm the Second Move
        m_selected->setMoveType(MovementType::Normal);
        m_selected->setTransformationType(TransformationType::No_Tranformation);
      }
    }
  }

  bool ShogiGame::secondClick(const uint32 x, const uint32 y, void *user)
  {
    if(m_guiLocked)
      return false;

    if(x >= ShogiTable::OFFSET_TOPTABLE && y >= ShogiTable::OFFSET_TOPTABLE) // Table Click, no Moves possible in the Tables
    {
      m_possibleMoves.clear();
      m_selected = nullptr;
      clearCellsColor();
      firstClick(x, y);
      return false;
    }

    if(m_board.getColor(y, x) == Abstract::Color::Red)
    {
      // Position was marked for a hit, so now that we moved there
      // the Piece will be taken out of the game, so we put it in the
      // according table of the Player
      static_cast<ShogiBoard&>(m_board).defeated(m_board.figure(y, x)); // Should never be NULL, if MarkCellRed is working correctly
      m_selected->setMoveType(MovementType::Capture);
    }

    bool res = AbstractGame::secondClick(x, y, user);
    if(m_possibleMoves.contains(x * 10 + y))
    {
      m_possibleMoves.clear();

      if(handlePromotion())
      {
        ShogiBoard *tmpBoard = reinterpret_cast<ShogiBoard*>(&m_board);

        if(tmpBoard) // Safety Measurement, otherwise we could end up in a deadlock
        {
          // Wait for user to Accept or Decline the Promotion
          // The UI is blocked at this Point until the user confirms the PromotionDialog, so we are not blocking the MainThread
          bool wait = true;

          if(!isEngineMove) // When the Engine did the Move, we do not need to wait
          {
            connect(tmpBoard, &ShogiBoard::promotionConfirmed, this, [=, &wait](bool accepted)
            {
              if(accepted)
                m_selected->setTransformationType(TransformationType::Accepted);
              else
                m_selected->setTransformationType(TransformationType::Declined);

              wait = false;
              disconnect(tmpBoard, &ShogiBoard::promotionConfirmed, this, NULL);
            });

            waitForEngine(wait);
          }
        }
      }

      m_selected->firstMoveDone();
      m_selected->allowTransformation();
      m_selected = nullptr;

      return true;
    }
    else {
      return res;
    }
  }

  void ShogiGame::newGame()
  {
    AbstractGame::newGame();

    m_turns.clear();
    m_turns.append(shogi_board);
  }

  void ShogiGame::onEngineResult(QString result)
  {
#ifdef QT_DEBUG
    qDebug() << "Received Engine-Result: " + result;
#endif

    if(m_engineThread == nullptr)
      return;

    if(result == "usiok")
    {
      m_engineThread->sendEngineCommand("isready");
      return;
    }

    if(result == "readyok")
    {
      m_engineThread->sendEngineCommand("usinewgame");
      return;
    }

    if(result == "posok")
    {
      Q_EMIT enginePositionSet();
      return;
    }

    if(result.startsWith("legalmoves"))
    {
      Q_EMIT engineValidMoves(result);

      return;
    }

    if(result.startsWith("bestmove"))
    {
      QStringList tmp = result.split(" ");
      QString move = (tmp.size() > 1) ? result.split(" ").at(1) : "";

      if(!move.isEmpty())
      {
        // TODO Check if the Move is valid? Engine shouldnt really make any mistakes though...
        isEngineMove = true;

        if(move == "resign")
        {
          qDebug() << "Engine is surrendering, good job!";
          Q_EMIT surrender();

          return;
        }

        COORD_NOTATION engineMove = convertEngineMove(move);
        if(engineMove.origin.x < m_board._rowSize)
          inverseAlphanumeric(engineMove.origin.x, m_board._rowSize);
        inverseAlphanumeric(engineMove.destination.x, m_board._rowSize);

        if(engineMove.movement == MovementType::Drop)
        {
          FigureType type = getFigureType(move, false);
          firstClick(engineMove.origin.x, engineMove.origin.y, &type);

          // First Click will set this correctly depending on the Move-Type
          engineMove.origin.x = m_selectedX;
          engineMove.origin.y = m_selectedY;
        }
        else
          firstClick(engineMove.origin.x, engineMove.origin.y);
        if(m_selected == nullptr) // There is no Piece at this Position, INVALID ENGINE MOVE!
        {
          Q_EMIT m_signals.invalidMove();
          m_possibleMoves.clear(); // So the Abstract secondClick will fail at set everything up for us!

          return;
        }

        secondClick(engineMove.destination.x, engineMove.destination.y);

        if(engineMove.transformation == TransformationType::Accepted)
        {
          transformation(1, 0, engineMove.destination.x, engineMove.destination.y);
        }
      }

      return;
    }

    if(result.startsWith("option"))
    {
      result.remove(0, 7);

      QStringList optionSettings = result.split(" ");

      QString oName;
      QString oType;
      QString oDefault;
      QString oMin;
      QString oMax;
      QString oVar;

      int varIndex = -1; // Index-Counter for optionSettings
      for(int i = 0; i < optionSettings.count(); i++){
        QString aktOption = optionSettings.at(i);

        if(aktOption == "name"){
          varIndex = 0;
          continue;
        }

        if(aktOption == "type"){
          varIndex = 1;
          continue;
        }

        if(aktOption == "default"){
          varIndex = 2;
          continue;
        }

        if(aktOption == "min"){
          varIndex = 3;
          continue;
        }

        if(aktOption == "max"){
          varIndex = 4;
          continue;
        }

        if(aktOption == "var"){
          varIndex = 5;
          continue;
        }

        switch (varIndex)
        {
          case -1: break; // Invalid, was never SET
          case 0: oName    += aktOption + " "; break;
          case 1: oType    += aktOption; break;
          case 2: oDefault += aktOption; break;
          case 3: oMin     += aktOption; break;
          case 4: oMax     += aktOption; break;
          case 5: oVar     += aktOption + " "; break;
        }
      }

      OptionSetting *option = new OptionSetting(oName, oType, oDefault);

      if(!oMin.isEmpty())
        option->setMin(oMin.toInt());

      if(!oMax.isEmpty())
        option->setMax(oMax.toInt());

      if(!oVar.isEmpty())
        option->setqVar(oVar);

      m_engineThread->optionSettingModel()->addOption(option);

      return;
    }


  }

  bool ShogiGame::checkCheck(bool checkmate)
  {
    return AbstractGame::checkCheck(checkmate);
  }

  bool ShogiGame::markCheck(King *king, AbstractFigure *figure, bool checkmate)
  {
    return AbstractGame::markCheck(king, figure, checkmate);
  }

  bool ShogiGame::isCheckAfterMove(uint32 x, uint32 y, AbstractFigure *figure, AbstractBoard &tmpAbstractBoard) const
  {
    return AbstractGame::isCheckAfterMove(x, y, figure, tmpAbstractBoard);
  }

  bool ShogiGame::isCheckMate()
  {
    bool res = AbstractGame::isCheckMate();
    AbstractBoard &tmpAbstractBoard = m_board;

    // For each figure.
    for(int x = 0; x < m_board._rowSize; ++x)
    {
      for(int y = 0; y < m_board._columnSize; ++y)
      {
        if(tmpAbstractBoard.figures()[y][x] && tmpAbstractBoard.figures()[y][x]->color() == m_turnColor)
        {
        }
      }
    }

    return res;
  }

  bool ShogiGame::isStaleMate()
  {
    return AbstractGame::isStaleMate();
  }

  bool ShogiGame::handlePromotion()
  {
    if(m_selected->type() == FigureType::PawnFigure          ||
       m_selected->type() == FigureType::LanceFigure         ||
       m_selected->type() == FigureType::KnightFigure        ||
       m_selected->type() == FigureType::SilverGeneralFigure ||
       m_selected->type() == FigureType::RookFigure          ||
       m_selected->type() == FigureType::BishopFigure)
    {
      if(m_selected->isTransformed())
        return false;

      if(isEngineMove)
      {
        if(m_selected->transformation_type() == TransformationType::Accepted)
          return true;
        else
          return false;
      }

      switch(m_selected->color())
      {
        case AbstractFigure::White:
        {
          if(m_selected->y() < 3 && m_selected->isAllowedTransformation())
          {
            Q_EMIT m_signals.transformation(AbstractFigure::White, m_selected->x(), m_selected->y());

            return true;
          }
        }
          break;

        case AbstractFigure::Black:
        {
          if( m_selected->y() > 5 && m_selected->isAllowedTransformation())
          {
            Q_EMIT m_signals.transformation(AbstractFigure::Black, m_selected->x(), m_selected->y());

            return true;
          }
        }
          break;
      }
    }

    return false;
  }

  void ShogiGame::gameStarted()
  {
    AbstractGame::gameStarted(); // Needs to be called because slots do not call the derived Function

    Q_EMIT m_signals.finishedLoading();
  }

  void ShogiGame::turnEnded()
  {
    m_turns.append(shogi_board);
  }

  void ShogiGame::undo()
  {
    if( m_turns.size() > 1 )
    {
      m_turns.pop();
      shogi_board = m_turns.top();
      m_board.update();
      markTurnLabel();

      if(m_turns.size() == 1)
        Q_EMIT m_signals.noMoreUndo();
    }
    else
        Q_EMIT m_signals.noMoreUndo();
  }

  void ShogiGame::searchDefeatedPieces()
  {

  }

  void ShogiGame::topTableClick(FigureType type)
  {
    for(uint32 iRow = 0; iRow < shogi_board.table_top.ROWS; iRow++)
    {
      for(uint32 iColumn = 0; iColumn < shogi_board.table_top.COLUMNS; iColumn++)
      {
        AbstractFigure *figure = shogi_board.table_top.figure(iRow, iColumn);
        if(figure && figure->type() == type)
        {
          m_selected = figure;
          m_selectedX = ShogiTable::OFFSET_TOPTABLE + iColumn;
          m_selectedY = ShogiTable::OFFSET_TOPTABLE + iRow;

          calcTableMoves(figure, m_board);
          return;
        }
      }
    }
  }

  void ShogiGame::topTableClick(const uint32_t x, const uint32_t y)
  {
    AbstractFigure *figure = shogi_board.table_top.figure(x - ShogiTable::OFFSET_TOPTABLE, y - ShogiTable::OFFSET_TOPTABLE);

    if(figure)
    {
      if(figure->color() == m_turnColor) // Is it really his turn now?
      {
        m_selected = figure;
        m_selectedX = x;
        m_selectedY = y;

        calcTableMoves(figure, m_board);
      }
    }
  }

  void ShogiGame::bottomTableClick(FigureType type)
  {
    for(uint32 iRow = 0; iRow < shogi_board.table_bot.ROWS; iRow++)
    {
      for(uint32 iColumn = 0; iColumn < shogi_board.table_bot.COLUMNS; iColumn++)
      {
        AbstractFigure *figure = shogi_board.table_bot.figure(iRow, iColumn);
        if(figure && figure->type() == type)
        {
          m_selected = figure;
          m_selectedX = ShogiTable::OFFSET_BOTTOMTABLE + figure->x();
          m_selectedY = ShogiTable::OFFSET_BOTTOMTABLE + figure->y();

          calcTableMoves(figure, m_board);
        }
      }
    }
  }

  void ShogiGame::bottomTableClick(const uint32_t x, const uint32_t y)
  {
    AbstractFigure *figure = shogi_board.table_bot.figure(x - ShogiTable::OFFSET_BOTTOMTABLE, y - ShogiTable::OFFSET_BOTTOMTABLE);

    if(figure)
    {
      if(figure->color() == m_turnColor) // Is it really his turn now?
      {
        m_selected = figure;
        m_selectedX = x;
        m_selectedY = y;

        calcTableMoves(figure, m_board);
      }
    }
  }

  void ShogiGame::calcTableMoves(AbstractFigure *figure, AbstractBoard &tmpAbstractBoard)
  {
    // HowTo Calc Moves
    // Check 1. Is the spot free?
    // Check 2. Can the Figure even make a move, ignoring Figures that are blocking the move?
    // Check 3. ONLY FOR PAWNS: Does this Row contain another Pawn of the same color?

    switch (figure->type()) {

      case LanceFigure:
      case PawnFigure:
      case KnightFigure:
      {
        uint rows, columns, y, x, impossibleRows;

        //! Check 2 handling
        if(figure->type() == KnightFigure)
          impossibleRows = 2; // Knight can never be positioned in the last two Rows, otherwise it wont be able to make a move
        else
          impossibleRows = 1; // Lance and Pawn can not be placed in the last Row, otherwise they wont be able to make a move
        //! Check 2 fulfilled

        columns = ShogiBoard::COLUMNS;
        bool pawnInColumn;
        for(x = 0; x < columns; x++ ){

          //! Check3 handling
          pawnInColumn = false; // Reset
          if(figure->color() == AbstractFigure::FigureColor::Black){
            y = 0; // We "start" at 0
            rows  = ShogiBoard::ROWS - impossibleRows; // And "stop" at Rows - impossibleRows
          } else {
            y = impossibleRows; // We "start" after impossibleRows
            rows  = ShogiBoard::ROWS; // And "stop" at Rows
          }

          if(figure->type() == PawnFigure)
          {
            // TODO if you are bored: Could probably be solved more efficiently but it doesnt really matter its constant anyways
            for(uint iPawn = 0; iPawn < ShogiBoard::ROWS; iPawn++){
              AbstractFigure *bFigure = m_board.figures()[iPawn][x];
              if(bFigure && bFigure->figure()->type() == PawnFigure)
              {
                if((int)bFigure->figure()->color() == (int)figure->color())
                {
                  pawnInColumn = true;
                  break;
                }
              }
            }
          }
          //! Check3 is fulfilled

          //! Check 1 handling
          for(; y < rows; y++ ){
            if(m_board.figures()[y][x])
              continue; // Not free != nullptr, NEXT!

            if(!pawnInColumn)
              markCellsForMove(x, y, 0, 0, Move::One, figure, tmpAbstractBoard);
          }
          //! Check1 is fulfilled
        }

        break;
      }

      default: // All the other pieces can move wherever they want, aslong as the spot isn't taken
      {
        for(uint x = 0; x < ShogiBoard::COLUMNS; x++ ){
          for(uint y = 0; y < ShogiBoard::ROWS; y++ ){

            if(m_board.figures()[y][x])  // Check1 is fulfilled
              continue; // Not free != nullptr, NEXT!

            // Check 2 is always fulfilled because the rest of the pieces can move forth and back,
            // so they can move away from every spot.

            // Check 3 is fulfilled because we are not a PAWN
            markCellsForMove(x, y, 0, 0, Move::One, figure, tmpAbstractBoard);
          }
        }
      }
    }
  }

  COORD_NOTATION ShogiGame::convertEngineMove(QString gikouMove)
  {     
    COORD_NOTATION move = {0, {0, 0}, 0, {0, 0}, 0, 0};

    // Move Depends on the Length
    int l_str = gikouMove.length();
    switch (l_str) {
      case 4: // Normal/Capture or Drop Move
      {
        move.origin.x = (gikouMove[0].isDigit()) ? QString(gikouMove[0]).toUInt() : 0;
        move.origin.y = AlphabeticalToAlphanumeric(gikouMove[1]);

        move.destination.x = (gikouMove[2].isDigit()) ? QString(gikouMove[2]).toUInt() : 0;
        move.destination.y = AlphabeticalToAlphanumeric(gikouMove[3]);

        if(isCapture(move.origin.x, move.origin.y, move.destination.x, move.destination.y))
          move.movement = MovementType::Capture;
        else
          move.movement = MovementType::Normal;

        move.transformation = TransformationType::No_Tranformation;

        if(gikouMove.contains("*"))
        {
          move.movement = MovementType::Drop;

          AbstractFigure *f = getFirstTableFigure(FigureType::PawnFigure);
          if(f != nullptr)
          {
            uint32 tableOffset;

            if(m_turnColor == AbstractFigure::White)
              tableOffset = shogi_board.table_bot.OFFSET_BOTTOMTABLE;
            else
              tableOffset = shogi_board.table_top.OFFSET_TOPTABLE;

            move.origin.x = f->x() + tableOffset;
            move.origin.y = f->y() + tableOffset;

            move.piece = f->type();
          }
          else
          {
            // Move not POSSIBLE! return Overflowed Value, that way the move will
            // be marked as invalid
            move.origin.x = shogi_board.INVALID_MOVE;
            move.origin.y = shogi_board.INVALID_MOVE;
          }
        }

        break;
      }

      case 5: // Promotion Move + Capture
      {
        move.origin.x = (gikouMove[0].isDigit()) ? QString(gikouMove[0]).toUInt() : 0;
        move.origin.y = AlphabeticalToAlphanumeric(gikouMove[1]);

        move.destination.x = (gikouMove[2].isDigit()) ? QString(gikouMove[2]).toUInt() : 0;
        move.destination.y = AlphabeticalToAlphanumeric(gikouMove[3]);

        AbstractFigure *dFigure = m_board.figures()[move.destination.y][move.destination.x];
        if(dFigure != nullptr)
        {
          // There is a Figure at the destination, so its a capture, otherwise the Move would be Invalid
          move.movement = MovementType::Capture;
        }
        else
        {
          move.movement = MovementType::Normal;
        }

        QString t_type = gikouMove.at(4);
        if(t_type == getPromotionNotation(TransformationType::Accepted))
          move.transformation = TransformationType::Accepted;
        if(t_type == getPromotionNotation(TransformationType::Declined))
          move.transformation = TransformationType::Declined;

        break;
      }

      default:
      {
        qDebug() << "Something went wrong while trying to Convert the Gikou Move to Standard-Notation:" << gikouMove;
        break;
      }

    }

    // Find the selected Piece on the Board
    AbstractFigure *figure = m_board.figure(move.origin.y, move.origin.x);
    if(figure)
    {
      move.piece = figure->type();
    }

    return move;
  }

  QString ShogiGame::convertToEngineMove(COORD_NOTATION pos)
  {
    QString gikouMove = "";

    FigureType f_type = static_cast<FigureType>(pos.piece);
    MovementType m_type = static_cast<MovementType>(pos.movement);

    switch (pos.movement) {

      case MovementType::Normal:
      case MovementType::Capture:
      {
        inverseAlphanumeric(pos.origin.x, m_board._rowSize);
        gikouMove += QString::number(pos.origin.x);
        gikouMove += AlphanumericToAlphabetical(pos.origin.y);

        break;
      }

      case MovementType::Drop:
      {
        gikouMove += getFigureNotation(f_type, false); // Pieces on the Table are never promoted!
        gikouMove += getMovementNotation(m_type);

        break;
      }

    }
    inverseAlphanumeric(pos.destination.x, m_board._rowSize);

    gikouMove += QString::number(pos.destination.x);
    gikouMove +=AlphanumericToAlphabetical(pos.destination.y);

    switch (pos.transformation)
    {
      case TransformationType::Accepted: gikouMove += "+"; break;
      case TransformationType::Declined: gikouMove += "="; break;
      default:break;
    }

    return gikouMove;
  }

  COORD_NOTATION ShogiGame::convertDisplayMove(QString displayMove)
  {
    COORD_NOTATION pos = {0, {0, 0}, 0, {0, 0}, 0, 0};

    if(displayMove.size() >= 4)
    {
      displayMove = displayMove.toLower();

      QChar y_1 = displayMove.at(1);
      QChar y_2 = displayMove.at(3);

      pos.origin.y      = y_1.unicode() - 0x0061;
      pos.destination.y = y_2.unicode() - 0x0061;
    }

    return pos;
  }

  QString ShogiGame::convertToDisplayMove(COORD_NOTATION pos)
  {
    QString kifuMove = "";

    AbstractFigure *figure = m_board.figures()[pos.destination.y][pos.destination.x]; // When we call this we usually already moved the Piece, thats why we can find it at the Destination
    if(figure)
    {
      QString x_1 = QString::number(pos.origin.x);
      QString x_2 = QString::number(pos.origin.y);

      QString move_type = getMovementNotation(pos.movement);

      switch (pos.movement) {

        case MovementType::Normal:
        case MovementType::Capture:
        {
          inverseAlphanumeric(pos.origin.x, m_board._rowSize);
          break;
        }

        case MovementType::Drop:
        {
          x_1 = "";
          x_2 = "";

          break;
        }

      }
      inverseAlphanumeric(pos.destination.x, m_board._rowSize);

      QString y_1 = QString::number(pos.destination.x);
      QString y_2 = QString::number(pos.destination.y);

      QString type = getFigureNotation(pos.piece, figure->isTransformed());
      QString promotion_type = getPromotionNotation(pos.transformation);

      kifuMove = type + x_1 + x_2 + move_type + y_1 + y_2 + promotion_type;
    }

    return kifuMove;
  }

  FigureType ShogiGame::getFigureType(QString type, bool promoted)
  {
    bool use_western = true;

    if(use_western)
    {
       if(promoted)
       {
         return PawnFigure;
       }
       else
       {
         // Most common at Top for a small Optimization
         if(type.startsWith(KIFU_PAWN_ENG)) return PawnFigure;
         if(type.startsWith(KIFU_GOLD_ENG)) return GoldGeneralFigure;
         if(type.startsWith(KIFU_ROOK_ENG)) return RookFigure;
         if(type.startsWith(KIFU_BISHOP_ENG)) return BishopFigure;
         if(type.startsWith(KIFU_SILVER_ENG)) return SilverGeneralFigure;
         if(type.startsWith(KIFU_KNIGHT_ENG)) return KnightFigure;
         if(type.startsWith(KIFU_LANCE_ENG)) return LanceFigure;

         return KingFigure;
       }
    }
    else
    {
      return PawnFigure;
    }
  }

  QString ShogiGame::getFigureNotation(uint32_t type, bool promoted)
  {
    bool use_western = true;

    FigureType f_type = static_cast<FigureType>(type);
    if(use_western)
    {
      switch (f_type) {

        case KingFigure:
        {
          if(promoted)
            return KIFU_P_KING_ENG;
          return KIFU_KING_ENG;
        }

        case GoldGeneralFigure:
        {
          if(promoted)
            return KIFU_P_GOLD_ENG;
          return KIFU_GOLD_ENG;
        }

        case RookFigure:
        {
          if(promoted)
            return KIFU_P_ROOK_ENG;
          return KIFU_ROOK_ENG;
        }

        case BishopFigure:
        {
          if(promoted)
            return KIFU_P_BISHOP_ENG;
          return KIFU_BISHOP_ENG;
        }

        case SilverGeneralFigure:
        {
          if(promoted)
            return KIFU_P_SILVER_ENG;
          return KIFU_SILVER_ENG;
        }

        case KnightFigure:
        {
          if(promoted)
            return KIFU_P_KNIGHT_ENG;
          return KIFU_KNIGHT_ENG;
        }

        case LanceFigure:
        {
          if(promoted)
            return KIFU_P_LANCE_ENG;
          return KIFU_LANCE_ENG;
        }

        case PawnFigure:
        {
          if(promoted)
            return KIFU_P_PAWN_ENG;
          return KIFU_PAWN_ENG;
        }

      }
    }
    else
    {
      return "";
    }
  }

  QString ShogiGame::getMovementNotation(uint32_t move_type)
  {
    MovementType m_type = static_cast<MovementType>(move_type);
    switch (m_type) {

      case Normal:
      {
        return KIFU_MOVE_NORMAL;
      }

      case Capture:
      {
        return KIFU_MOVE_CAPTURE;
      }

      case Drop:
      {
        return KIFU_MOVE_DROP;
      }

    }
  }

  QString ShogiGame::getPromotionNotation(uint32_t promotion_type)
  {
    TransformationType t_type = static_cast<TransformationType>(promotion_type);
    switch (t_type) {

      case No_Tranformation:
      {
        return "";
      }

      case Accepted:
      {
        return KIFU_PROMOTE_ACCEPT;
      }

      case Declined:
      {
        return KIFU_PROMOTE_DECLINE;
      }
    }

    return ""; // Just in case something goes wrong, we dont want unitialized Data to get out...
  }

  AbstractFigure *ShogiGame::getFirstTableFigure(FigureType type)
  {
    ShogiTable *aktBoard;

    if(m_turnColor == AbstractFigure::White)
      aktBoard = &shogi_board.table_bot;
    else
      aktBoard = &shogi_board.table_top;

    for(uint32_t x = 0; x < aktBoard->ROWS; x++)
    {
      for(uint32_t y = 0; y < aktBoard->COLUMNS; y++)
      {
        AbstractFigure *f = aktBoard->figure(x, y);
        if(f != nullptr)
        {
          if(f->type() == type)
          {
            f->setX(x);
            f->setY(y);

            return f;
          }
        }
      }
    }

    return nullptr;
  }

}
} // END NAMEPSACE "Stratogi"
