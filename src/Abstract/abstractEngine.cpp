#include "abstractEngine.hpp"

#include <QDir>
#include <QTimer>
#include <QGuiApplication>

namespace Stratogi {

  /* OptionSetting */

  OptionSetting::OptionSetting()
  {
  }

  OptionSetting::OptionSetting(const QString &name, const QString &type, const QString &value,
                             const int min, const int max, const QVariant qvar)
    : m_name(name), m_type(type), m_value(value),
      m_min(min), m_max(max), m_qvar(qvar)
  {
  }

  bool OptionSetting::setName(QString name)
  {
    if(m_name == name)
      return false;

    m_name = name;
    Q_EMIT nameChanged(name);

    return true;
  }

  bool OptionSetting::setType(QString type)
  {
    if(m_type == type)
      return false;

    m_type = type;
    Q_EMIT typeChanged(type);

    return true;
  }

  bool OptionSetting::setValue(QString value)
  {
    if(m_value == value)
      return false;

    m_value = value;
    Q_EMIT valueChanged(value);

    return true;
  }

  bool OptionSetting::setMin(int min)
  {
    if(m_min == min)
      return false;

    m_min = min;
    Q_EMIT minChanged(min);

    return true;
  }

  bool OptionSetting::setMax(int max)
  {
    if(m_max == max)
      return false;

    m_max = max;
    Q_EMIT maxChanged(max);

    return true;
  }

  bool OptionSetting::setqVar(QVariant qvar)
  {
    if(m_qvar == qvar)
      return false;

    m_qvar = qvar;
    Q_EMIT qvarChanged(qvar);

    return true;
  }

  QString OptionSetting::name() const
  {
    return m_name;
  }

  QString OptionSetting::type() const
  {
    return m_type;
  }

  QString OptionSetting::value() const
  {
    return m_value;
  }

  int OptionSetting::min() const
  {
    return m_min;
  }

  int OptionSetting::max() const
  {
    return m_max;
  }

  QVariant OptionSetting::qvar() const
  {
    if(m_type == "combo")
      return m_qvar.toString().split(" ");

    return m_qvar;
  }

  /* OptionSettingModel */

  OptionSettingModel::OptionSettingModel(QObject *parent) : QAbstractListModel(parent)
  {
  }

  OptionSettingModel::~OptionSettingModel()
  {
    Q_FOREACH(auto option, m_options) {
      option->deleteLater();
    }
  }

  void OptionSettingModel::addOption(OptionSetting *option)
  {
    // The Name needs to be unique!
    // So if it already exists, we will not add it!
    for(int i = 0; i < m_options.count(); i++)
    {
      if(m_options[i]->name() == option->name())
      {
        return;
      }
    }

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_options.append(option);
    endInsertRows();
  }

  OptionSetting *OptionSettingModel::getOption(QString name)
  {
    OptionSetting *res = nullptr;

    for(int i = 0; i < m_options.count(); i++)
    {
      if(m_options[i]->name() == name)
      {
        res = m_options[i];
        break;
      }
    }

    return res;
  }

  int OptionSettingModel::rowCount(const QModelIndex & parent) const
  {
    Q_UNUSED(parent);
    return m_options.count();
  }

  QVariant OptionSettingModel::data(const QModelIndex & index, int role) const
  {
    if (index.row() < 0 || index.row() >= m_options.count())
      return QVariant();

    const OptionSetting *option = m_options[index.row()];

    switch (role)
    {
      case NameRole:  return option->name();
      case TypeRole:  return option->type();
      case ValueRole: return option->value();
      case MinRole:   return option->min();
      case MaxRole:   return option->max();
      case QVarRole:  return option->qvar();
    }

    return QVariant();
  }

  Qt::ItemFlags OptionSettingModel::flags(const QModelIndex &index) const
  {
    if (!index.isValid())
      return Qt::NoItemFlags;

    return Qt::ItemIsEditable; // FIXME: Implement me!
  }

  bool OptionSettingModel::setData(const QModelIndex &index, const QVariant &value, int role)
  {
    OptionSetting *option = m_options[index.row()];

    if (data(index, role) != value) {

      bool changed = false;
      switch (role)
      {
        case NameRole:  changed = option->setName(value.toString());  break;
        case TypeRole:  changed = option->setType(value.toString());  break;
        case ValueRole: changed = option->setValue(value.toString()); break;
        case MinRole:   changed = option->setMin(value.toInt()); break;
        case MaxRole:   changed = option->setMax(value.toInt()); break;
        case QVarRole:  changed = option->setqVar(value); break;
      }

      if(changed) {
          Q_EMIT optionChanged(option);
          Q_EMIT dataChanged(index, index, QVector<int>(role));
      }

      return true;
    }
    return false;
  }

  QHash<int, QByteArray> OptionSettingModel::roleNames() const
  {
    QHash<int, QByteArray> roles;
    roles[NameRole]  = "name";
    roles[TypeRole]  = "type";
    roles[ValueRole] = "value";
    roles[MinRole]   = "min";
    roles[MaxRole]   = "max";
    roles[QVarRole]  = "qvar";

    return roles;
  }

  EngineInformation::EngineInformation()
  {

  }

  void EngineInformation::setDepth(int depth)
  {
    if(m_depth == depth)
      return;

    m_depth = depth;
  }

  void EngineInformation::setSeldepth(int seldepth)
  {
    if(m_depth == seldepth)
      return;

    m_depth = seldepth;
  }

  void EngineInformation::setMultipv(int multipv)
  {
    if(m_depth == multipv)
      return;

    m_depth = multipv;
  }

  void EngineInformation::setScore(int score)
  {
    if(m_depth == score)
      return;

    m_depth = score;
  }

  void EngineInformation::setWdl(int wdl)
  {
    if(m_depth == wdl)
      return;

    m_depth = wdl;
  }

  void EngineInformation::setNodes(int nodes)
  {
    if(m_depth == nodes)
      return;

    m_depth = nodes;
  }

  void EngineInformation::setNPS(int nps)
  {
    if(m_depth == nps)
      return;

    m_depth = nps;
  }

  void EngineInformation::setTime(int time)
  {
    if(m_depth == time)
      return;

    m_depth = time;
  }

  int EngineInformation::depth() const
  {
    return m_depth;
  }

  int EngineInformation::seldepth() const
  {
    return m_seldepth;
  }

  int EngineInformation::multipv() const
  {
    return m_multipv;
  }

  int EngineInformation::score() const
  {
    return m_score;
  }

  int EngineInformation::wdl() const
  {
    return m_wdl;
  }

  int EngineInformation::nodes() const
  {
    return m_nodes;
  }

  int EngineInformation::nps() const
  {
    return m_nps;
  }

  int EngineInformation::time() const
  {
    return m_time;
  }

  /* EngineInfoModel */

  EngineInfoModel::EngineInfoModel(QObject *parent) : QAbstractListModel(parent)
  {
  }

  EngineInfoModel::~EngineInfoModel()
  {
    Q_FOREACH(auto info, m_infos) {
      info->deleteLater();
    }
  }

  void EngineInfoModel::addInformation(EngineInformation *info)
  {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_infos.append(info);
    endInsertRows();
  }

  int EngineInfoModel::rowCount(const QModelIndex &parent) const
  {
    Q_UNUSED(parent);
    return m_infos.count();
  }

  QVariant EngineInfoModel::data(const QModelIndex &index, int role) const
  {
    if (index.row() < 0 || index.row() >= m_infos.count())
      return QVariant();

    const EngineInformation *info = m_infos[index.row()];

    switch (role)
    {
      case DepthRole:    return info->depth();
      case SeldepthRole: return info->seldepth();
      case MultipvRole:  return info->multipv();
      case ScoreRole:    return info->score();
      case WDLRole:      return info->wdl();
      case NodesRole:    return info->nodes();
      case NPSRole:      return info->nps();
      case TimeRole:     return info->time();
    }

    return QVariant();
  }

  Qt::ItemFlags EngineInfoModel::flags(const QModelIndex &index) const
  {
    if (!index.isValid())
      return Qt::NoItemFlags;

    return Qt::ItemIsEditable; // FIXME: Implement me!
  }

  QHash<int, QByteArray> EngineInfoModel::roleNames() const
  {
    QHash<int, QByteArray> roles;
    roles[DepthRole]    = "depth";
    roles[SeldepthRole] = "seldepth";
    roles[MultipvRole]  = "multipv";
    roles[ScoreRole]    = "score";
    roles[WDLRole]      = "wdl";
    roles[NodesRole]    = "nodes";
    roles[NPSRole]      = "nps";
    roles[TimeRole]     = "time";

    return roles;
  }

  /* AbstractEngine */

  AbstractEngine::AbstractEngine()
  {
    m_stopEngine = false;

    QString fileName = QDir::currentPath() + "\\engine.log";
    m_engineLog.setFileName(fileName);

    if(!m_engineLog.open(QIODevice::WriteOnly))
    {
      qDebug() << m_engineLog.errorString();
      m_engineLog.setFileName(QDir::tempPath() + "\\engine.log");
      m_engineLog.open(QIODevice::WriteOnly);
    }

    m_engineLog.write("Engine started.\n");

    // Create Option Setting Model, so we can change the Settings for the given Engine
    m_optionSettingModel = new OptionSettingModel;
  }

  AbstractEngine::~AbstractEngine()
  {
    if(AbstractEngine::isEngineProcessRunning())
      AbstractEngine::terminate(true);

    // Write the Rest of the Log into the Logfile if needed
    m_engineLog.flush();

    m_optionSettingModel->deleteLater();
  }

  void AbstractEngine::sendEngineCommand(QString command)
  {
    m_commandList.append(command);
  }

  void AbstractEngine::appendEngineMove(QString move)
  {
    if(engineIsMoveValid(move))
    {
      m_engineMoves.append(move);
      setMovePosition();

      Q_EMIT engineMoveChanged(m_engineMoves);
    }
  }

  void AbstractEngine::removeLastEngineMove()
  {
      if(m_engineMoves.isEmpty())
          return;

      m_engineMoves.removeLast();
      setMovePosition();

      Q_EMIT engineMoveChanged(m_engineMoves);
  }

  void AbstractEngine::startNewGame()
  {
      m_engineMoves.clear();
      setMovePosition();
  }

  void AbstractEngine::setEngineOptions(QString name, QString value)
  {
    if(name.isEmpty())
        return;

    if(value.isEmpty())
      m_commandList.append("setoption name " + name);
    else
      m_commandList.append("setoption name " + name + " value " + value);
  }

  QString Stratogi::AbstractEngine::getEngineOption(QString name)
  {
    return "";
  }

  bool AbstractEngine::isEngineProcessRunning()
  {
    if(m_engineProcess != nullptr &&
       m_engineProcess->isOpen())
    {
      return true;
    }
    return false;
  }

  QStringList AbstractEngine::engineMoves()
  {
      return m_engineMoves;
  }

  OptionSettingModel *AbstractEngine::optionSettingModel()
  {
    return m_optionSettingModel;
  }

  void AbstractEngine::setEngineLogPath(QString logPath)
  {
    m_engineLog.close();
    m_engineLog.setFileName(logPath);
    m_engineLog.open(QIODevice::WriteOnly);
  }

  void AbstractEngine::setMovePosition()
  {
      sendEngineCommand("position startpos moves " + m_engineMoves.join(" "));
      msleep(10);
  }

  void AbstractEngine::run()
  {
    m_engineStopped = false;

    if(m_engineProcess == nullptr)
    {
      m_engineLog.write("Please call/overwrite loadEngine first and init. the Engine-Process");

      return;
    }
    m_engineProcess->setCurrentWriteChannel(0);

    // Redirect Errors and Output to Logfile
    //m_engineProcess->setStandardErrorFile(m_engineLog.fileName());
    //m_engineProcess->setStandardOutputFile(m_engineLog.fileName());

    // Set up the Timer for every second
    QTimer timer;

    // Flush the buffered Data every second
    connect(&timer, &QTimer::timeout, this, &AbstractEngine::onEngineLogFlush);

    timer.start(5000);
    // Run the Thread constantly until we get the stop request
    while(Q_LIKELY(!m_stopEngine))
    {
      if(m_commandList.count() > 0 )
      {
        QByteArray aktCommand = m_commandList.first().toLocal8Bit() + "\n"; // The "\n" is IMPORTANT
        m_commandList.removeFirst();

        int bytesWritten = m_engineProcess->write(aktCommand);
        m_engineProcess->waitForBytesWritten(10);
        while(Q_UNLIKELY(bytesWritten != aktCommand.length())) // Fallback, trying to write the Rest, usually never happens
        {
          QByteArray leftBytes = aktCommand.left(bytesWritten);
          bytesWritten += m_engineProcess->write(leftBytes, aktCommand.length() - bytesWritten);
          m_engineProcess->waitForBytesWritten(10);

          QGuiApplication::processEvents();
        }
        //m_engineProcess->closeWriteChannel(); // Not needed if "\n is in the Command"

        connect(m_engineProcess.data(), &QProcess::bytesWritten, [this, aktCommand] {
          m_engineLog.write(QString("sendCommand: " + aktCommand + "\n").toLocal8Bit());
        });
      }

      QGuiApplication::processEvents();
      msleep(50);
    }

    m_engineStopped = true;
  }

  void AbstractEngine::terminate(bool doExit)
  {
    m_stopEngine = true;

    int timeout = 2000; // 2000ms as timeout
    while(!m_engineStopped)
    {
      msleep(50);
      timeout -=50;

      if(timeout <= 0 )
        break;
    }

    if(doExit)
      exit();
  }

  void AbstractEngine::onProcessReadyRead()
  {
    char buf[1024];

    while(m_engineProcess->readLine(buf, 1024) > 0)
    {
      QString trimmed_buf = QString(buf).trimmed();
      m_engineLog.write(QString("Received Data: " + trimmed_buf + "\n").toLocal8Bit());
      m_engineOutput.append(trimmed_buf);

      Q_EMIT engineResult(trimmed_buf);
    }
    m_engineProcess->readAll();
  }

  void AbstractEngine::onEngineLogFlush()
  {
      m_engineLog.flush();
  }

} // END NAMEPSACE "Stratogi"

