#include "chessgame.h"

#include <QCoreApplication>

namespace Stratogi {
namespace Chess {

  ChessGame::ChessGame(ChessBoard &board, Signals &s) : AbstractGame (board, s), chess_board(board)
  {
    m_engineThread = new ChessEngine();
    m_engineColor = AbstractFigure::FigureColor::Black; //! @todo depends on the GAME

    m_turns.append(chess_board);

    m_defeatedPieces.insert("Pawn", 0);
    m_defeatedPieces.insert("Bishop", 0);
    m_defeatedPieces.insert("Kngiht", 0);
    m_defeatedPieces.insert("Rook", 0);
    m_defeatedPieces.insert("Queen", 0);
  }

  int ChessGame::turnsCount() const
  {
      return m_turns.size();
  }

  void ChessGame::markCellForHit(uint32 x, uint32 y, int32_t dx, int32_t dy, Move::Distance d,
    AbstractFigure *figure, AbstractBoard & tmpAbstractBoard)
  {
    AbstractGame::markCellForHit(x, y, dx, dy, d, figure, tmpAbstractBoard);

    x += dx;
    y += dy;

    if(d != Move::Any &&
       x < tmpAbstractBoard._rowSize &&
       y < tmpAbstractBoard._columnSize)
    {
      // Take on the pass.
      if(figure->type() == FigureType::PawnFigure)
      {
        uint32 tmpY = y;

        switch(figure->color())
        {
          case AbstractFigure::White :
          {
            y += 1;
          }
            break;

          case AbstractFigure::Black :
          {
            y -= 1;
          }
            break;
        }

        AbstractFigure *f = m_board.figures()[y][x];
        if(f && f->type() == FigureType::PawnFigure) {
            Pawn *p = reinterpret_cast<Pawn*>(f);

            if(p->isPass() && !isCheckAfterMove(x, tmpY, figure, tmpAbstractBoard)){
                m_board.markRed(x, tmpY);

                m_possibleMoves.append(x * 10 + tmpY);
            }
        }
      }
    }
  }

  void ChessGame::firstClick(uint32 x, uint32 y, void *user)
  {
    Q_UNUSED(user);

    AbstractGame::firstClick(x, y);

    AbstractFigure *figure = m_board.figures()[y][x];
    if(figure)
    {
      if(figure->color() == m_turnColor) // Is the turn correct?
      {
        m_selected = figure;
        m_selectedX = x;
        m_selectedY = y;

        const AbstractFigure::Moves moves = figure->moves();

        AbstractBoard &tmpAbstractBoard = m_board;

        // For each possibe move.
        for( int i = 0; i < 5; ++i )
        {
          for( int j = 0; j < 5; ++j )
          {
            // Castling highlight.
            if( figure->type() == Abstract::FigureType::KingFigure &&
              !figure->isFirstMoveDone() && !m_isKingCheck)
            {
              // Queen Casting
              if(!m_board.figures()[y][x - 1] &&
                 !m_board.figures()[y][x - 2] &&
                 m_board.figures()[y][x - 4] &&
                 !m_board.figures()[y][x - 4]->isFirstMoveDone())
              {
                if(!isCheckAfterMove(x - 2, y, figure, tmpAbstractBoard) &&
                   !isCheckAfterMove(x - 1, y, figure, tmpAbstractBoard))
                {
                  m_possibleMoves.append((x - 2 ) * 10 + y);

                  m_board.markBlue(x - 2, y);
                }
              }

              if(!m_board.figures()[y][x + 1] &&
                 !m_board.figures()[y][x + 2] &&
                 m_board.figures()[y][x + 3] &&
                 !m_board.figures()[y][x + 3]->isFirstMoveDone() )
              {
                 if(!isCheckAfterMove(x + 2, y, figure, tmpAbstractBoard) &&
                    !isCheckAfterMove(x + 1, y, figure, tmpAbstractBoard))
                 {
                  m_possibleMoves.append(( x + 2 ) * 10 + y);

                  m_board.markBlue(x + 2, y);
                }
              }
            }
          }
        }
      }
    }

    if(m_selected)
    {
       // Set Defaults, can't know that until we confirm the Second Move
      m_selected->setMoveType(MovementType::Normal);
      m_selected->setTransformationType(TransformationType::No_Tranformation);
    }
  }

  bool ChessGame::secondClick(uint32 x, uint32 y, void *user)
  {
    Q_UNUSED(user);

    if(m_board.getColor(y, x) == Abstract::Color::Red)
    {
      // Position was marked for a hit, so now that we moved there
      // the Piece will be taken out of the game, so we put it in the
      // according table of the Player
      m_selected->setMoveType(MovementType::Defeat);
    }

    bool res = AbstractGame::secondClick(x, y);
    if(m_possibleMoves.contains(x * 10 + y))
    {
      m_possibleMoves.clear();

      handleCastling( x, y, m_selected, m_board);

      // Pass by pawn.
      if(m_selected->type() == FigureType::PawnFigure){

        // Calculate the Move Difference, CAREFUL we use uint32 so make sure
        // we cast to int, otherwise we risk a overflow and the result is wrong
        int moveDiff = qAbs((int)y - (int)m_selectedY);
        if(moveDiff == 2)
          static_cast<Pawn*>(m_selected)->setPass(true);
      }

      handleTransformation();

      // Clear pass by pawn flag.
      AbstractFigure::FigureColor c = (m_selected->color() == AbstractFigure::White ? AbstractFigure::Black
                                                                                    : AbstractFigure::White);

      switch(c)
      {
        case AbstractFigure::Black :
        {
          for( uint x = 0; x < m_board._rowSize; ++x )
          {
            if(m_board.figures()[3][x] &&
               m_board.figures()[3][x]->type() == FigureType::PawnFigure &&
               m_board.figures()[3][x]->color() == c)
            {
              static_cast<Pawn*>(m_board.figures()[3][x])->setPass(false);
            }
          }
        }
          break;

        case AbstractFigure::White :
        {
          for( uint x = 0; x < m_board._rowSize; ++x )
          {
            if(m_board.figures()[4][x] &&
               m_board.figures()[4][x]->type() == FigureType::PawnFigure &&
               m_board.figures()[4][x]->color() == c )
            {
              static_cast<Pawn*>(m_board.figures()[4][x])->setPass(false);
            }
          }
        }
          break;
      }

      m_selected->firstMoveDone();
      m_selected = nullptr;

      return true;
    }
    else {
      return res;
    }
  }

  bool ChessGame::checkCheck(bool checkmate)
  {
    return AbstractGame::checkCheck(checkmate);
  }

  bool ChessGame::markCheck(King *king, AbstractFigure *figure, bool checkmate)
  {
    return AbstractGame::markCheck(king, figure, checkmate);
  }

  bool ChessGame::isCheckAfterMove(uint32 x, uint32 y, AbstractFigure *figure, AbstractBoard &tmpAbstractBoard) const
  {
    //AbstractFigure *f = tmpAbstractBoard.figure(figure->index());

    //handleCastling( x, y, f, tmpAbstractBoard);

    return AbstractGame::isCheckAfterMove(x, y, figure, tmpAbstractBoard);
  }

  bool ChessGame::isCheckMate()
  {
    bool res = AbstractGame::isCheckMate();
    AbstractBoard &tmpAbstractBoard = m_board;

    // For each figure.
    for(uint x = 0; x < m_board._rowSize; ++x)
    {
      for(uint y = 0; y < m_board._columnSize; ++y)
      {
        if(tmpAbstractBoard.figures()[y][x] && tmpAbstractBoard.figures()[y][x]->color() == m_turnColor)
        {
          AbstractFigure * tmpFigure = tmpAbstractBoard.figures()[y][x];

          // For each possible move.
          for(int i = 0; i < 5; ++i)
          {
            for(int j = 0; j < 5; ++j)
            {
              int dx = 0;
              int dy = 0;

              if(tmpFigure->color() == AbstractFigure::White)
              {
                dx = j - 2;
                dy = i - 2;
              }
              else
              {
                dx = 2 - j;
                dy = 2 - i;
              }

//              int x = tmpFigure->x() + dx;
//              int y = tmpFigure->y() + dy;

//              Move::Distance d = tmpFigure->moves()[i][j].dist();

//              if(d == Move::Any || d == Move::TwoFirstTime)
//              {

//              }
//              else if(tmpFigure->moves()[i][j] .dist() != Move::No &&
//                      x >= 0 &&
//                      x < m_board._rowSize &&
//                      y >= 0 &&
//                      y < m_board._columnSize)
//              {
//                if(tmpFigure->type() == FigureType::PawnFigure)
//                {
//                  int tmpY = y;

//                  switch(tmpFigure->color())
//                  {
//                    case AbstractFigure::White :
//                    {
//                      y += 1;
//                    }
//                      break;

//                    case AbstractFigure::Black :
//                    {
//                      y -= 1;
//                    }
//                      break;
//                  }

////                  Pawn * p = reinterpret_cast<Pawn*>(tmpAbstractBoard.figures()[y][x]);

////                  if(p && p->isPass() && !isCheckAfterMove(x, tmpY, tmpFigure, tmpAbstractBoard))
////                    return false;
//                }
//              }
            }
          }
        }
      }
    }

    return res;
  }

  bool ChessGame::isStaleMate()
  {
    return AbstractGame::isStaleMate();
  }

  bool ChessGame::handleTransformation()
  {
    if(m_selected->type() == FigureType::PawnFigure)
    {
      switch(m_selected->color())
      {
        case AbstractFigure::White:
        {
          if(m_selected->y() == 0)
          {
            Q_EMIT m_signals.transformation(AbstractFigure::White, m_selected->x(), m_selected->y());

            return true;
          }
        }
          break;

        case AbstractFigure::Black:
        {
          if( m_selected->y() == 7 )
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

  void ChessGame::gameStarted()
  {
    AbstractGame::gameStarted(); // Needs to be called because slots do not call the derived Function

    Q_EMIT m_signals.finishedLoading();
  }

  void ChessGame::handleCastling(uint32 x, uint32 y, AbstractFigure * figure, AbstractBoard & board)
  {
    Q_UNUSED(y)
    bool castled = false;

    // Castling.
    if(figure->type() == FigureType::KingFigure && !figure->isFirstMoveDone())
    {
      switch(figure->color())
      {
        case AbstractFigure::White :
        {
          if(x == 2)
          {
            if(board.figures()[7][0] && !board.figures()[7][0]->isFirstMoveDone())
            {
               board.move(0, 7, 3, 7);
               castled = true;
            }
          }
          else if(x == 6)
          {
            if(board.figures()[7][7] && !board.figures()[7][7]->isFirstMoveDone())
            {
               board.move(7, 7, 5, 7);
               castled = true;
            }
          }
        }
          break;

        case AbstractFigure::Black :
        {
          if(x == 2)
          {
            if(board.figures()[0][0] &&
              !board.figures()[0][0]->isFirstMoveDone() )
            {
               board.move(0, 0, 3, 0);
               castled = true;
            }
          }
          else if(x == 6)
          {
            if(board.figures()[0][7] &&
              !board.figures()[0][7]->isFirstMoveDone() )
            {
               board.move(7, 0, 5, 0);
               castled = true;
            }
          }
        }
          break;
      }
    }

    if(castled)
    {
      // We already appended the Rook Move to the board, but its not done yet
      // we also need to put in the King move, otherwise the UNDO will not reset
      // the Board correctly, so we will remove the Last move and append the new board
      // that way it is considered as ONE MOVE
      m_turns.removeLast();
      m_turns.append(chess_board);
    }
  }

  void ChessGame::turnEnded()
  {
    m_turns.append(chess_board);
  }

  void ChessGame::undo()
  {
    if(m_turns.size() > 1)
    {
      m_turns.pop();
      chess_board = m_turns.top();
      clearCellsColor();
      markTurnLabel();

      m_board.removeLastMove();
      m_engineThread->removeLastEngineMove();

      m_engineThread->sendEngineCommand("position startpos moves " + m_engineThread->engineMoves().join(" "));

      if(m_useEngine && m_engineColor == m_turnColor) {
          m_engineThread->calcNextEngineMove();
      }

      if(m_turns.size() == 1)
        Q_EMIT m_signals.noMoreUndo();
      m_board.update();
    }
    else
        Q_EMIT m_signals.noMoreUndo();
  }

  void ChessGame::newGame()
  {
    AbstractGame::newGame();

    m_turns.clear();
    m_turns.append(chess_board);
  }

  void ChessGame::onEngineResult(QString result)
  {
#ifdef QT_DEBUG
    qDebug() << "Received Engine-Result: " + result;
#endif

    if(m_engineThread == nullptr)
      return;

    if(result.startsWith("id"))
    {
      // Identify the Engine ID
      result.remove(0, 3);

      if(result.startsWith("name"))
      {
        result.remove(0, 5);
        QString name = result;
      }

      if(result.startsWith("author"))
      {
        result.remove(0, 7);
        QString author = result;
      }

      return;
    }

    if(result.startsWith("uciok"))
    {
      m_engineThread->sendEngineCommand("isready");
      return;
    }

    if(result.startsWith("readyok"))
    {
      // TODO Engine is ready

      return;
    }

    if(result.startsWith("bestmove")) // For example "bestmove d2d4 ponder c2c4
    {     
      QStringList tmp = result.split(" ");
      QString move = (tmp.size() > 1) ? result.split(" ").at(1) : "";

      if(!move.isEmpty())
      {
        // TODO Check if the Move is valid? Engine shouldnt really make any mistakes though...

        if(move == "resign")
        {
          qDebug() << "Engine is surrendering, good job!";
          Q_EMIT surrender();

          return;
        }

        COORD_NOTATION engineMove = convertEngineMove(move);
        firstClick(engineMove.origin.x, engineMove.origin.y);
        if(m_selected == nullptr) // There is no Piece at this Position, INVALID ENGINE MOVE!
        {
          Q_EMIT m_signals.invalidMove();
          m_possibleMoves.clear(); // So the Abstract secondClick will fail at set everything up for us!

          return;
        }

        secondClick(engineMove.destination.x, engineMove.destination.y);

        //if(engineMove.transformation == TransformationType::Accepted)
        //{
        //  transformation(1, 0, engineMove.destination.x, engineMove.destination.y);
        //}
      }

      return;
    }

    if(result.startsWith("info"))
    {
      result.remove(0, 5);

      return;
    }

    //
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

    if(result.startsWith("Unkown Command"))
    {
      // TODO Unkown Command send to Engine
      qDebug() << "Engine received unkown Command:" + result;
    }

  }

  COORD_NOTATION ChessGame::convertEngineMove(QString engineMove)
  {
    COORD_NOTATION move = {0, {0, 0}, 0, {0, 0}, 0, 0};

    // Move Depends on the Length
    int l_str = engineMove.length();
    switch (l_str) {
      case 4: // Normal/Defeat Move
      {
        move.origin.x = AlphabeticalToAlphanumeric(engineMove[0]);
        move.origin.y = QString(engineMove[1]).toUInt();
        inverseAlphanumeric(move.origin.y, m_board._columnSize);

        move.destination.x = AlphabeticalToAlphanumeric(engineMove[2]);
        move.destination.y = QString(engineMove[3]).toUInt();
        inverseAlphanumeric(move.destination.y, m_board._columnSize);

        if(isCapture(move.origin.x, move.origin.y, move.destination.x, move.destination.y))
          move.movement = MovementType::Defeat;
        else
          move.movement = MovementType::Normal;

        move.transformation = TransformationType::No_Tranformation;

        break;
      }

      default:
      {
        qDebug() << "Something went wrong while trying to Convert the Engine-Move to Standard-Notation:" << engineMove;
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

  COORD_NOTATION ChessGame::convertDisplayMove(QString displayMove)
  {
    // The Display Move and the Engine Move are the same here!
    return convertEngineMove(displayMove);
  }

  QString ChessGame::convertToEngineMove(COORD_NOTATION pos)
  {
    QString engineMove = "";

    //FigureType f_type = static_cast<FigureType>(pos.piece);
    MovementType m_type = static_cast<MovementType>(pos.movement);

    switch (m_type) {

      case MovementType::Normal:
      {
        inverseAlphanumeric(pos.origin.y, m_board._columnSize);
        engineMove += AlphanumericToAlphabetical(pos.origin.x);
        engineMove += QString::number(pos.origin.y);

        break;
      }

      case MovementType::Defeat:
      {
        inverseAlphanumeric(pos.origin.y, m_board._columnSize);
        engineMove += AlphanumericToAlphabetical(pos.origin.x);
        engineMove += QString::number(pos.origin.y);

        break;
      }

    }

    inverseAlphanumeric(pos.destination.y, m_board._columnSize);
    engineMove += AlphanumericToAlphabetical(pos.destination.x);
    engineMove += QString::number(pos.destination.y);

    switch (pos.transformation)
    {
      case TransformationType::Transform_to_Queen:  engineMove += ""; break;
      case TransformationType::Transform_to_Rook:   engineMove += ""; break;
      case TransformationType::Transform_to_Knight: engineMove += ""; break;
      case TransformationType::Transform_to_Bishop: engineMove += ""; break;
      default:break;
    }

    return engineMove;
  }

  QString ChessGame::convertToDisplayMove(COORD_NOTATION pos)
  {
      // This is FINE! The Engine Move is understandable to Users
      return convertToEngineMove(pos);
  }

  void ChessGame::searchDefeatedPieces()
  {
      // Number of Pieces we found during the search over the Board
      int fPawns  = 0;
      int fBishop = 0;
      int fKnight = 0;
      int fRook   = 0;
      int fQueen  = 0;

      for(uint x = 0; x < m_board._rowSize; ++x)
      {
        for(uint y = 0; y < m_board._columnSize; ++y)
        {
            AbstractFigure *figure = m_board.figures()[y][x];

            // Is it an actual Piece?
            if(figure == nullptr)
              continue;

            // Is the Color the Color of the opposite Turn( a Piece we can defeat)
            if(figure->color() != m_turnColor)
              continue;

            // Find out the Type of the Figure
            switch (figure->type())
            {

              case FigureType::PawnFigure:
              {
                fPawns++;
                break;
              }

              case FigureType::BishopFigure:
              {
                fBishop++;
                break;
              }

              case FigureType::KnightFigure:
              {
                fKnight++;
                break;
              }

              case FigureType::RookFigure:
              {
                fRook++;
                break;
              }

              case FigureType::QueenFigure:
              {
                fQueen++;
                break;
              }

              default:
                continue;
            }

        }
      }

      m_defeatedPieces["Pawn"]   = 8 - fPawns;
      m_defeatedPieces["Bishop"] = 2 - fBishop;
      m_defeatedPieces["Knight"] = 2 - fKnight;
      m_defeatedPieces["Rook"]   = 2 - fRook;
      m_defeatedPieces["Queen"]  = 1 - fQueen;
  }

}
} // END NAMEPSACE "Stratogi"
