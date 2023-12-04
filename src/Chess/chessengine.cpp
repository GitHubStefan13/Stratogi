#include "chessengine.h"

#include <QDebug>

namespace Stratogi {
namespace Chess {

  ChessEngine::ChessEngine()
  {

  }

  ChessEngine::~ChessEngine()
  {

  }

  bool ChessEngine::event(QEvent *event)
  {
    bool consumed;
    consumed = false;

    if(!consumed)
      return QThread::event(event);
    return true;
  }

  bool ChessEngine::eventFilter(QObject *watched, QEvent *event)
  {
    bool consumed;
    consumed = false;

    if(!consumed)
      return QThread::eventFilter(watched, event);
    return true;
  }

  void ChessEngine::loadEngine()
  {
    QString dir;
    QString programm;
#ifdef Q_OS_WINDOWS
    dir = "C:\\Users\\Admin-PC\\Downloads\\";
    programm = dir + "fairy_stockfish.exe";
#else
    dir = "C:\\Users\\Stefan\\Downloads\\";
    programm = dir + "stockfish.exe";
#endif
    m_engineProcess = new QProcess();
    m_engineProcess->setWorkingDirectory(dir);
    connect(m_engineProcess, &QProcess::readyRead, this, &ChessEngine::onProcessReadyRead);

    m_engineProcess->setProgram(programm);
    m_engineProcess->moveToThread(this);

    connect(this, &QThread::started, [this]
    {
      m_engineProcess->start(); // Defaults to opening the Device in ReadWrite
      if(!m_engineProcess->waitForStarted(5000))
      {
        QString error = m_engineProcess->errorString();
        qDebug(error.toLocal8Bit());
      }

      sendEngineCommand("uci");
    });
    start();
  }

  void ChessEngine::calcNextEngineMove()
  {
    sendEngineCommand("go depth 10"); //! 10 is enough for now... @todo: Figure out a better way to do this
  }

  bool ChessEngine::engineIsMoveValid(QString move)
  {
    // TODO: Check if move is valid, currently always true because secondClick checks it
      return true;
  }

  void ChessEngine::startNewGame()
  {
    sendEngineCommand("ucinewgame");

    AbstractEngine::startNewGame();
  }

} // END NAMESPACE "Chess"
} // END NAMEPSACE "Stratogi"
