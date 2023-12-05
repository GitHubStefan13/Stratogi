// Qt include.
#include <QIcon>
#include <QSettings>
#include <QQuickStyle>
#include <QQuickView>
#include <QQmlContext>
#include <QQmlComponent>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "signals.hpp"

#include "abstractEngine.hpp"

#include "chessBoard.h"
#include "chessgame.h"

#include "shogiTable.h"
#include "shogiBoard.h"
#include "shogigame.h"

#include <QDir>
#include <QStandardPaths>

int main( int argc, char ** argv )
{
  QGuiApplication::setApplicationName("Stratogi");
  QGuiApplication::setOrganizationName("dsGaming");

  // Find out the App Location and set it as Current Path for all new Files
  auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  if (path.isEmpty()) {
    qFatal("Cannot determine settings storage location");
  } else {
    QDir d{path};
    d.mkpath(d.absolutePath());
    QDir::setCurrent(d.absolutePath());
  }
  QGuiApplication app(argc, argv);

  QIcon::setThemeName("actionbar");

  QSettings settings;
  QString style = QQuickStyle::name();
  if (!style.isEmpty())
    settings.setValue("style", style);
  else
    QQuickStyle::setStyle(settings.value("style").toString());
  QQmlApplicationEngine engine;
  engine.rootContext()->setContextProperty("availableStyles", "Material, Universal, Windows");

  const QUrl url(QStringLiteral("qrc:/main.qml"));
  QObject::connect(&engine,
                   &QQmlApplicationEngine::objectCreated,
                   &app,
                   [url](QObject *obj, const QUrl &objUrl)
                   {
                     if (!obj && url == objUrl)
                       QCoreApplication::exit(-1);
                   },
                   Qt::QueuedConnection
                   );


  qmlRegisterType<Stratogi::Signals>("StratogiSignals", 1, 0, "Stratogi");

  // Load Chess QML
  Stratogi::Chess::ChessBoard chessBoard;
  Stratogi::Signals chessSigs;
  chessSigs.setGameState(false);
  Stratogi::Chess::ChessGame chessGame(chessBoard, chessSigs);
  Stratogi::OptionSettingModel *chessOptionModel = chessGame.engineThread()->optionSettingModel();

  engine.rootContext()->setContextProperty("chessBoard", &chessBoard);
  engine.rootContext()->setContextProperty("chessGame", &chessSigs);
  engine.rootContext()->setContextProperty("chessGameImpl", &chessGame);
  engine.rootContext()->setContextProperty("chessOptionSettingModel", chessOptionModel);

  // Load Shogi QML
  Stratogi::Shogi::ShogiBoard shogiBoard;
  Stratogi::Signals shogiSigs;
  shogiSigs.setGameState(false);
  Stratogi::Shogi::ShogiGame shogiGame(shogiBoard, shogiSigs);
  Stratogi::OptionSettingModel *shogiOptionModel = shogiGame.engineThread()->optionSettingModel();

  engine.rootContext()->setContextProperty("shogiTableTop", &shogiBoard.table_top);
  engine.rootContext()->setContextProperty("shogiTableBot", &shogiBoard.table_bot);
  engine.rootContext()->setContextProperty("shogiBoard", &shogiBoard);
  engine.rootContext()->setContextProperty("shogiGame", &shogiSigs);
  engine.rootContext()->setContextProperty("shogiGameImpl", &shogiGame);
  engine.rootContext()->setContextProperty("shogiOptionSettingModel", shogiOptionModel);

  engine.load(url);

  return app.exec();
}
