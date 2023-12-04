#include "abstractGame.hpp"

// Qt include.
#include <QVariant>
#include <QString>
#include <QMetaObject>
#include <QThread>
#include <QGuiApplication>
#include <QDebug>

namespace Stratogi {

  AbstractGame::AbstractGame(AbstractBoard &board, Signals &s)
    :	m_board(board)
    ,	m_turnColor(AbstractFigure::FigureColor::White)
    ,	m_selected(nullptr)
    ,	m_selectedX(0)
    ,	m_selectedY(0)
    ,	m_isKingCheck(false)
    ,	m_signals(s)
    ,	m_checkmate(false)
    ,   m_defeatedPieces()
  {
    connect(&m_signals, &Signals::turnMade, this, &AbstractGame::turnEnded );
  }

  AbstractGame::~AbstractGame()
  {
    if(m_engineThread != nullptr)
    {
      if(m_engineThread->isRunning())
      {
        delete m_engineThread;
        m_engineThread = nullptr;
      }
    }
  }

  AbstractEngine *AbstractGame::engineThread() const
  {
      return m_engineThread;
  }

  void AbstractGame::useEngine(bool useEngine)
  {
    m_useEngine = useEngine;

    //! Reloads the Engine if needed
    gameStarted();
  }

  int AbstractGame::defeatedPieceCount(QString pieceName)
  {
    if(m_defeatedPieces.contains(pieceName))
      return m_defeatedPieces.value(pieceName);
    else
      return 0;
  }
  
  /**
   * @brief Marks a Cell for Move and adds the Move to a List of possible Moves
   * @param x: The Cell's x Position we will start moving from
   * @param y: The Cells's y Position we will start moving from
   * @param dx: Offset to x, so if we have the Position {1, 2}, the Cell will be at {1 + dx, 2}
   * @param dy: Offset to y, so if we have the Position {1, 2}, the Cell will be at {1, 2 + dy}
   * @param d: The distance the figure can move
   * @param figure: The figure that will be moving
   * @param tmpAbstractBoard: The Board the figure will move on
   */
  void AbstractGame::markCellsForMove(uint32 x, uint32 y, int32_t dx, int32_t dy, Move::Distance d,
    AbstractFigure *figure, AbstractBoard &tmpAbstractBoard)
  {
    x += dx;
    y += dy;

    int move = 0;

    if(d == Move::Any || d == Move::TwoFirstTime)
    {
      while(x < m_board._rowSize && y < m_board._columnSize)
      {
        if(!m_board.figures()[y][x]) // Is there even a Figure at that Position?
        {
          if(!isCheckAfterMove(x, y, figure, tmpAbstractBoard)) // We are not allowed to make a move that will end us in a Check
          {
            m_board.markBlue(x, y);

            m_possibleMoves.append( x * 10 + y );
          }
        }
        else
          break;

        x += dx;
        y += dy;
        ++move;

        if(d == Move::TwoFirstTime && figure->isFirstMoveDone())
          break;

        if(d == Move::TwoFirstTime && move == 2)
          break;
      }
    }
    // Only one cell move.
    else if(x < m_board._rowSize && y < m_board._columnSize)
    {
      if(!m_board.figures()[y][x]) // Is there even a Figure at that Position?
      {
        if(!isCheckAfterMove(x, y, figure, tmpAbstractBoard)) // We are not allowed to make a move that will end us in a Check
        {
          m_board.markBlue(x, y);

          m_possibleMoves.append(x * 10 + y);
        }
      }
    }
  }

  void AbstractGame::markCellForHit(uint32 x, uint32 y, int32_t dx, int32_t dy, Move::Distance d,
    AbstractFigure *figure, AbstractBoard &tmpAbstractBoard)
  {
    x += dx;
    y += dy;

    if(d == Move::Any)
    {
      while(x < m_board._rowSize && y < m_board._columnSize)
      {
        if(m_board.figures()[y][x]) // Is there even a Figure at that Position?
        {
          if(figure->color() != m_board.figures()[y][x]->figure()->color())
          {
            if(!isCheckAfterMove(x, y, figure, tmpAbstractBoard)) // We are not allowed to make a move that will end us in a Check
            {
              m_board.markRed(x, y);

              m_possibleMoves.append(x * 10 + y);
            }
          }

          break;
        }

        x += dx;
        y += dy;
      }
    }
    else if(x < tmpAbstractBoard._rowSize && y < tmpAbstractBoard._columnSize)
    {
      if(m_board.figures()[y][x] && figure->color() != m_board.figures()[y][x]->figure()->color())
      {
        if(!isCheckAfterMove(x, y, figure, tmpAbstractBoard)) // We are not allowed to make a move that will end us in a Check
        {
          m_board.markRed(x, y);

          m_possibleMoves.append(x * 10 + y);
        }
      }
    }
  }

  void AbstractGame::clearCellsColor()
  {
    m_board.clearColors();
  }

  void AbstractGame::firstClick(const uint32 x, const uint32 y, void *user)
  {
    Q_UNUSED(user);

    if(y >= m_board._rowSize || x >= m_board._columnSize)
       return;

    m_possibleMoves.clear(); // Always clear the Possible moves on the First Click, better be safe than sorry

    AbstractFigure *figure = m_board.figures()[y][x];

    if(figure)
    {
      if(figure->color() == m_turnColor) // Is it really his turn now?
      {
        m_selected = figure;
        m_selectedX = x;
        m_selectedY = y;

        m_board.markBlue(x, y);

        const AbstractFigure::Moves moves = figure->moves();

        AbstractBoard &tmpAbstractBoard = m_board;

        // For each possibe move.
        for(int i = 0; i < AbstractFigure::MOVES; ++i )
        {
          for(int j = 0; j < AbstractFigure::MOVES; ++j )
          {
            int dx = 0;
            int dy = 0;

            if(figure->color() == AbstractFigure::White)
            {
              dx = j - 2;
              dy = i - 2;
            }
            else
            {
              dx = 2 - j;
              dy = 2 - i;
            }

            // Moves highlight.
            if(moves[i][j].types() & Move::Movement)
            {
              markCellsForMove(x, y, dx, dy, moves[i][j].dist(), figure, tmpAbstractBoard);
            }

            // Hit highlight.
            if(moves[i][j].types() & Move::Hit)
            {
              markCellForHit(x, y, dx, dy, moves[i][j].dist(), figure, tmpAbstractBoard);
            }
          }
        }
      }
    }
  }

  void AbstractGame::waitForEngine(const bool &ready)
  {
      while(ready) // Wait for Engine to send a Signal
      {
        QThread::msleep(10);
        QGuiApplication::processEvents();

        m_guiLocked = true; // Block our Board-Gui until the User either Accepts or Declines the Promotion
      }
      m_guiLocked = false;
  }

  /**
   * @brief Inverse coord using size
   * @param value The Move in Coordinal Notation
   * @param size The maximum Size from which we will do the inverse
   *
   * @example Size = 9, Move is 1 then the Inverse is 9
   * @example Size = 9, Move is 4 then the Inverse is 5
   *
   * @attention  Coord-Value has to be in the Range 1 - size
   */
  void AbstractGame::inverseAlphanumeric(uint32_t &value,  uint32_t size)
  {
      value = size - value;
  }

  QString AbstractGame::AlphanumericToAlphabetical(uint32_t value)
  {
    return QString(QChar(value + 0x0061));
  }

  uint32_t AbstractGame::AlphabeticalToAlphanumeric(QChar value)
  {
    ushort unicode = value.unicode();
    if(unicode >= 'a' && unicode <= 'z')
      return unicode - 0x0061; // Not an alphabetical letter a...z

    return 0;
  }

  void AbstractGame::onEngineOptionChanged(OptionSetting *option)
  {
    if(Q_UNLIKELY(option->type() == "button"))
    {
      engineThread()->setEngineOptions(option->name(), "");
    } else {
      engineThread()->setEngineOptions(option->name(), option->value());
    }

  }

  bool AbstractGame::secondClick(uint32 x, uint32 y, void *user)
  {
    Q_UNUSED(user);

    if(y >= m_board._rowSize || x >= m_board._columnSize)
      return false;

    // Is this move made by the Engine? Then we want to skip or automatically set certain things
    isEngineMove = (m_turnColor == m_engineColor) & m_useEngine;

    if(m_possibleMoves.contains(x * 10 + y))
    {
      m_board.move(m_selectedX, m_selectedY, x, y);

      markTurnLabel();

      bool turn = false;

      if(checkCheck() && isCheckMate()){
        checkCheck( true );

        m_checkmate = true;

        Q_EMIT m_signals.checkmate();

        return true;
      }
      turn = true;

      // Searches and writes the defeated Pieces into :m_defeatedPieces
      searchDefeatedPieces();

      // Get the current Position and create our Coord-Notation from it
      COORD_POSITION from = {m_selectedX, m_selectedY};
      COORD_POSITION to   = {x, y};
      COORD_NOTATION pos = {m_selected->type(), from, m_selected->move_type(), to, m_selected->transformation_type(), 0};

      // Convert it to a move the Engine can use and check if the move is valid, ITS CHECKED TWICE!
      QString newMove = convertToEngineMove(pos);
      if(!m_engineThread->engineIsMoveValid(newMove) || m_selected == nullptr)
      {
        Q_EMIT m_signals.invalidMove();
        m_possibleMoves.clear(); // So the Abstract secondClick will fail at setting everything up for us!
      } else {
        // So we are good to add everything to our MoveList since the move is valid
        m_engineThread->appendEngineMove(newMove); // This is for the Engine to know where we are
        m_board.appendMove(convertToDisplayMove(pos)); // This is for the display of the Enginemove, might differ from the engineMove!
      }

      if(!isEngineMove)
        m_engineThread->calcNextEngineMove();
      else
        clearCellsColor(); // Clear all leftovers, EngineMove might have displayed

      Q_EMIT m_signals.turnMade();
      return turn;
    }
    else
    {
      m_selected = nullptr;

      clearCellsColor();

      checkCheck();

      firstClick(x, y);

      return false;
    }
  }

  void AbstractGame::markTurnLabel()
  {
    if( m_turnColor == AbstractFigure::White )
    {
      m_turnColor = AbstractFigure::Black;
    }
    else
    {
      m_turnColor = AbstractFigure::White;
    }
  }

  bool AbstractGame::checkCheck( bool checkmate )
  {
    King * king = static_cast<King*>((m_turnColor == AbstractFigure::Black ? m_board.blackKing()
                                                                           : m_board.whiteKing()
                  ));

    bool res = false;

    m_isKingCheck = false;

    // For each figure.
    for(uint32 x = 0; x < m_board._rowSize; ++x )
    {
      for(uint32 y = 0; y <  m_board._columnSize; ++y )
      {
        if(m_board.figures()[y][x] && m_board.figures()[y][x]->color() != m_turnColor)
           if(markCheck(king, m_board.figures()[y][x], checkmate))
                res = true;
      }
    }

    return res;
  }

  bool AbstractGame::markCheck(King *king, AbstractFigure *figure, bool checkmate)
  {
    // For each possible move.
    for(int i = 0; i < 5; ++i)
    {
      for(int j = 0; j < 5; ++j)
      {
        if(figure->moves()[i][j].types() & Move::Hit)
        {
          int dx = 0;
          int dy = 0;

          if(figure->color() == AbstractFigure::White)
          {
            dx = j - 2;
            dy = i - 2;
          }
          else
          {
            dx = 2 - j;
            dy = 2 - i;
          }

          QList<QPair<int, int>> turns;
          turns.append(qMakePair(figure->x(), figure->y()));

          uint32 x = figure->x() + dx;
          uint32 y = figure->y() + dy;

          turns.append(qMakePair(x, y));

          if(figure->moves()[i][j].dist() == Move::Any)
          {
            while(x >= 0 &&
                  x < m_board._rowSize &&
                  y >= 0 &&
                  y < m_board._columnSize)
            {
              if(m_board.figures()[y][x] == king)
              {
                m_isKingCheck = true;

                for(auto & p : qAsConst(turns))
                {
                  if(!checkmate)
                    m_board.markCheck(p.first, p.second);
                  else
                    m_board.markRed(p.first, p.second);
                }

                return true;
              }
              else if(m_board.figures()[y][x])
                break;

              x += dx;
              y += dy;

              turns.append(qMakePair(x, y));
            }
          }
          else if(figure->moves()[i][j].dist() != Move::No &&
                  x >= 0 &&
                  x < m_board._rowSize &&
                  y >= 0 &&
                  y < m_board._columnSize)
          {
            if(m_board.figures()[y][x] == king)
            {
              m_isKingCheck = true;

              for(auto & p : qAsConst(turns))
              {
                if(!checkmate)
                  m_board.markCheck(p.first, p.second);
                else
                  m_board.markRed(p.first, p.second);
              }

              return true;
            }
          }
        }
      }
    }

    return false;
  }

  bool AbstractGame::isCheckAfterMove(uint32 x, uint32 y, AbstractFigure *figure, AbstractBoard &tmpAbstractBoard ) const
  {
    AbstractFigure *f = tmpAbstractBoard.figure(figure->index());

    const uint32 oldX = f->x();
    const uint32 oldY = f->y();

    bool res = false;

    AbstractFigure *oldFigure = tmpAbstractBoard.figures()[y][x];

    tmpAbstractBoard.move(oldX, oldY, x, y, true);

    King *king = static_cast<King*>((f->color() == AbstractFigure::White ? tmpAbstractBoard.whiteKing()
                                                                         : tmpAbstractBoard.blackKing()
                 ));

    // For each figure.
    for(uint32 x = 0; x < m_board._rowSize; ++x)
    {
      for(uint32 y = 0; y <  m_board._columnSize; ++y)
      {
        if(tmpAbstractBoard.figures()[y][x] && tmpAbstractBoard.figures()[y][x]->color() != f->color())
        {
          AbstractFigure * tmpFigure = tmpAbstractBoard.figures()[y][x];

          // For each possible move.
          for(uint32 i = 0; i < 5; ++i)
          {
            for(uint32 j = 0; j < 5; ++j)
            {
              if(tmpFigure->moves()[i][j].types() & Move::Hit)
              {
                uint32 dx = 0;
                uint32 dy = 0;

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

                uint32 x = tmpFigure->x() + dx;
                uint32 y = tmpFigure->y() + dy;

                if(tmpFigure->moves()[i][j].dist() == Move::Any)
                {
                  while(x < m_board._rowSize && y < m_board._columnSize)
                  {
                    if(tmpAbstractBoard.figures()[y][x] == king)
                    {
                      res = true;

                      break;
                    }
                    else if(tmpAbstractBoard.figures()[y][x])
                      break;

                    x += dx;
                    y += dy;
                  }
                }
                else if(x < m_board._rowSize && y < m_board._columnSize)
                {
                  if(tmpAbstractBoard.figures()[y][x] == king)
                    res = true;
                }
              }

              if(res)
                break;
            }

            if(res)
              break;
          }

          if(res)
            break;
        }
      }

      if(res)
        break;
    }

    tmpAbstractBoard.move(x, y, oldX, oldY, true);

    if(oldFigure)
      tmpAbstractBoard.figures()[oldFigure->y()][oldFigure->x()] = oldFigure;

    return res;
  }

  bool AbstractGame::isCheckMate()
  {
    AbstractBoard &tmpAbstractBoard = m_board;

    // For each figure.
    for(uint32 x = 0; x < m_board._rowSize; ++x)
    {
      for(uint32 y = 0; y < m_board._columnSize; ++y)
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

              uint32 x = tmpFigure->x() + dx;
              uint32 y = tmpFigure->y() + dy;

              int move = 0;

              Move::Distance d = tmpFigure->moves()[i][j].dist();

              if(d == Move::Any || d == Move::TwoFirstTime)
              {
                bool stop = false;

                while( x >= 0 && x < 8 && y >= 0 && y < 8 )
                {
                  ++move;

                  if(tmpAbstractBoard.figures()[y][x])
                  {
                    if(tmpAbstractBoard.figures()[y][x]->color() == m_turnColor)
                      break;
                    else if(!(tmpFigure->moves()[i][j].types() & Move::Hit))
                      break;
                    else
                      stop = true;
                  }

                  const bool firstMoveDone =
                    tmpFigure->isFirstMoveDone();

                  if(!isCheckAfterMove(x, y, tmpFigure, tmpAbstractBoard))
                    return false;

                  if(d == Move::TwoFirstTime)
                  {
                    if(firstMoveDone)
                      break;
                    else if(move == 2)
                      break;
                  }

                  if( stop )
                    break;

                  x += dx;
                  y += dy;
                }
              }
              else if(tmpFigure->moves()[i][j] .dist() != Move::No &&
                      x >= 0 &&
                      x < m_board._rowSize &&
                      y >= 0 &&
                      y < m_board._columnSize)
              {
                if(tmpAbstractBoard.figures()[y][x])
                {
                  if(tmpAbstractBoard.figures()[y][x]->color() == m_turnColor)
                    continue;
                  else if(!(tmpFigure->moves()[i][j].types() & Move::Hit))
                    continue;
                }
                else if(tmpFigure->moves()[i][j].types() & Move::Hit &&
                       !(tmpFigure->moves()[i][j].types() & Move::Movement))
                    continue;

                if(!isCheckAfterMove(x, y, tmpFigure, tmpAbstractBoard))
                  return false;
              }
            }
          }
        }
      }
    }

    return true;
  }

  bool AbstractGame::isStaleMate()
  {
    return isCheckMate();
  }

  bool AbstractGame::isCapture(uint32 x1, uint32 y1, uint32 x2, uint32 y2)
  {
    AbstractFigure *oFigure = m_board.figures()[y1][x1];
    AbstractFigure *dFigure = m_board.figures()[y2][x2];

    if(oFigure != nullptr && dFigure != nullptr)
    {
      if(oFigure->color() != dFigure->color())
        return true;
    }
    return false;
  }

  void AbstractGame::clicked(int x, int y)
  {
    if(!m_selected)
      firstClick(x, y);
    else
    {
      if(secondClick(x, y))
      {
        clearCellsColor();

        if(!m_checkmate && isStaleMate())
          Q_EMIT m_signals.drawgame();
      }
    }
  }

  void AbstractGame::turnEnded()
  {
  }

  void AbstractGame::hovered(int x, int y )
  {
    Q_UNUSED( x )
    Q_UNUSED( y )
  }

  void AbstractGame::newGame()
  {
    // Clean up the Engine
    m_board.newGame();
    m_board.update();

    // Tell Engine we want to start a new Game
    m_engineThread->startNewGame();

    Q_EMIT m_signals.noMoreUndo();

    m_turnColor = AbstractFigure::White;
    m_isKingCheck = false;
    m_possibleMoves.clear();
    m_selectedX = -1;
    m_selectedY = -1;
    m_checkmate = false;
  }

  void AbstractGame::transformation(int figure, int c, int x, int y)
  {
    m_board.transformation(figure, c, x, y);

    if(checkCheck() && isCheckMate())
    {
      checkCheck(true);

      m_checkmate = true;

      Q_EMIT m_signals.checkmate();
    }
    else
      Q_EMIT m_signals.turnMade();

    m_board.update();
  }

  void AbstractGame::gameStarted()
  {
    if(m_useEngine)
    {
      if(!m_engineThread->isEngineProcessRunning())
        m_engineThread->loadEngine();
      connect(m_engineThread, &AbstractEngine::engineResult, this, &AbstractGame::onEngineResult);

      connect(m_engineThread->optionSettingModel(), &OptionSettingModel::optionChanged,
              this, &AbstractGame::onEngineOptionChanged);
    }
  }

} // END NAMEPSACE "Stratogi"
