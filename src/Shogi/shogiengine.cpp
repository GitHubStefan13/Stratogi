#include "shogiengine.h"

namespace Stratogi {
namespace Shogi {

  ShogiEngine::ShogiEngine()
  {

  }

  ShogiEngine::~ShogiEngine()
  {

  }

  bool ShogiEngine::event(QEvent *event)
  {
    bool consumed;

    consumed = false;

    if(!consumed)
      return QThread::event(event);
    return true;
  }

  bool ShogiEngine::eventFilter(QObject *watched, QEvent *event)
  {
    bool consumed;

    consumed = false;

    if(!consumed)
      return QThread::eventFilter(watched, event);
    return true;
  }

  void ShogiEngine::loadEngine()
  {
    QString dir;
    QString programm;
#ifdef Q_OS_WINDOWS
    dir = "C:\\Users\\Admin-PC\\Downloads\\";
    programm = dir + "fairy_stockfish.exe";
#else
    programm = "/media/sf_Shared_Folder/Projects/Stratogi/thirdparty/binary";
#endif
    m_engineProcess = new QProcess();
    m_engineProcess->setWorkingDirectory(dir);
    connect(m_engineProcess, &QProcess::readyRead, this, &ShogiEngine::onProcessReadyRead);

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

      sendEngineCommand("usi");
    });
    start();
  }

  void ShogiEngine::calcNextEngineMove()
  {
    sendEngineCommand("go depth 10"); //! 10 is enough for now... @todo: Figure out a better way to do this
  }

  bool ShogiEngine::engineIsMoveValid(QString move)
  {
    /*sendEngineCommand("position startpos moves " + gikouMoves);
    wait = true;
    connect(this, &ShogiGame::enginePositionSet, this, [this, &wait]{
      wait = false;
      disconnect(this, &ShogiGame::enginePositionSet, this, NULL);
    });
    waitForEngine(wait);

    sendEngineCommand("legalmoves");

    wait = true;
    QString validMoves;
    connect(this, &ShogiGame::engineValidMoves, this, [this, &wait, &validMoves](QString result){
      validMoves = result;
      wait = false;
      disconnect(this, &ShogiGame::engineValidMoves, this, NULL);
    });
    waitForEngine(wait);
    */

    //return validMoves.contains(move);

    // TODO: Check if move is valid, currently always true because secondClick checks it
//    m_engineMoves.append(move);

//    sendEngineCommand("position startpos moves " + m_engineMoves.join(" "));
//    msleep(10);

    return true;
  }

  void ShogiEngine::startNewGame()
  {
    sendEngineCommand("usinewgame");

    AbstractEngine::startNewGame();
  }



} // END NAMESPACE "Shogi"
} // END NAMEPSACE "Stratogi"
