#ifndef ABSTRACT_ENGINE_HPP
#define ABSTRACT_ENGINE_HPP

#include <QFile>
#include <QEvent>
#include <QDebug>
#include <QThread>
#include <QProcess>
#include <QAbstractListModel>

namespace Stratogi {

  class OptionSetting : public QObject
  {
    Q_OBJECT

    //! This options EXIST ALWAYS
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString value READ value WRITE setValue NOTIFY valueChanged)

    //! This Value or optional/denpending on the Type
    Q_PROPERTY(int min READ min WRITE setMin NOTIFY minChanged)
    Q_PROPERTY(int max READ max WRITE setMax NOTIFY maxChanged)
    Q_PROPERTY(QVariant qvar READ qvar WRITE setqVar NOTIFY qvarChanged)

    public:
      OptionSetting();
      OptionSetting(const QString &name, const QString &type, const QString &value,
                    const int min = 0, const int max = 0, const QVariant qvar = "");

      //! Set the Name of the Option, returns true if the Value was changed otherwise false
      bool setName(QString name);
      //! Set the Type of the Option, returns true if the Value was changed otherwise false
      bool setType(QString type);
      //! Set the Value/Default Value of the Option, returns true if the Value was changed otherwise false
      bool setValue(QString value);
      //! Set the min Value of the Option, returns true if the Value was changed otherwise false
      bool setMin(int min);
      //! Set the max Value of the Option, returns true if the Value was changed otherwise false
      bool setMax(int max);
      //! Set the "var" Value of the Option, returns true if the Value was changed otherwise false
      bool setqVar(QVariant qvar);

      QString name() const;
      QString type() const;
      QString value() const;
      int min() const;
      int max() const;
      QVariant qvar() const;

    Q_SIGNALS:
      void nameChanged(QString name);
      void typeChanged(QString type);
      void valueChanged(QString value);
      void minChanged(int min);
      void maxChanged(int max);
      void qvarChanged(QVariant qvar);

    private:
      // This Values ALWAYS EXIST
      QString m_name;
      QString m_type;
      QString m_value;

      // This Values are optional/depending on the Type
      int m_min;
      int m_max;
      QVariant m_qvar;
  };

  class OptionSettingModel : public QAbstractListModel
  {
    Q_OBJECT

    public:
      enum OptionSettingRoles {
        NameRole = Qt::UserRole + 1,
        TypeRole,
        ValueRole,
        MinRole,
        MaxRole,
        QVarRole
      };

      OptionSettingModel(QObject *parent = 0);
      ~OptionSettingModel();

      void addOption(OptionSetting *option);
      OptionSetting *getOption(QString name);
      int rowCount(const QModelIndex & parent = QModelIndex()) const override;
      QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

      Qt::ItemFlags flags(const QModelIndex &index) const override;
      bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Q_SIGNALS:
      void optionChanged(OptionSetting *option);

    protected:
      QHash<int, QByteArray> roleNames() const override;

    private:
      QList<OptionSetting*> m_options;
  };

  class EngineInformation : public QObject
  {
    Q_OBJECT

    //! This options EXIST ALWAYS
    Q_PROPERTY(int depth READ depth WRITE setDepth)
    Q_PROPERTY(int seldepth READ seldepth WRITE setSeldepth)
    Q_PROPERTY(int multipv READ multipv WRITE setMultipv)
    Q_PROPERTY(int score READ score WRITE setScore)
    Q_PROPERTY(int wdl READ wdl WRITE setWdl)
    Q_PROPERTY(int nodes READ nodes WRITE setNodes)
    Q_PROPERTY(int nps READ nps WRITE setNPS)
    Q_PROPERTY(int time READ time WRITE setTime)

    public:
      EngineInformation();

      //! Set the Depth of the Information
      void setDepth(int depth);
      //! Set the Seldepth of the Information
      void setSeldepth(int seldepth);
      //! Set the Multipv of the Information
      void setMultipv(int multipv);
      //! Set the Score of the Information
      void setScore(int score);
      //! Set the Wdl of the Information
      void setWdl(int wdl);
      //! Set the Nodes of the Information
      void setNodes(int nodes);
      //! Set the NPS of the Information
      void setNPS(int nps);
      //! Set the Time of the Option
      void setTime(int time);

      int depth() const;
      int seldepth() const;
      int multipv() const;
      int score() const;
      int wdl() const;
      int nodes() const;
      int nps() const;
      int time() const;

    private:
      int m_depth;
      int m_seldepth;
      int m_multipv;
      int m_score;
      int m_wdl;
      int m_nodes;
      int m_nps;
      int m_time;
  };

  class EngineInfoModel : public QAbstractListModel
  {
      Q_OBJECT

      public:
        enum EngineInfoRoles {
          DepthRole = Qt::UserRole + 1,
          SeldepthRole,
          MultipvRole,
          ScoreRole,
          WDLRole,
          NodesRole,
          NPSRole,
          TimeRole
        };

        EngineInfoModel(QObject *parent = 0);
        ~EngineInfoModel();

        void addInformation(EngineInformation *info);
        int rowCount(const QModelIndex & parent = QModelIndex()) const override;
        QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

        Qt::ItemFlags flags(const QModelIndex &index) const override;
      protected:
        QHash<int, QByteArray> roleNames() const override;

      private:
        QList<EngineInformation*> m_infos;
  };

  //! Board.
  class AbstractEngine : public QThread
  {
    Q_OBJECT

  public:
    AbstractEngine();
    virtual ~AbstractEngine();

    enum EngineErrorGrade {
        WARNING,
        CRITICAL
    };

    //! Virtual Functions for the Engine, needs to be implemented for the Subclasses
    //!
    //! @example The Engine will be loaded during Construction and calculate the next Move
    //!          after each Turn( IF TURNED ON ) the Engine will also be able to accept
    //!          Game Options and Commands during the Game such as STOP Calculating OR
    //!          the Thread Count that we use to calculate the next Move
    //!
    //! @note Chess uses UCI = Universal Chess Interface
    //!       Shogi uses USI = Universal Shogi Interface
    virtual void loadEngine() = 0;
    virtual void unloadEngine() = 0;
    virtual void calcNextEngineMove() = 0;
    virtual bool engineIsMoveValid(QString move) = 0;
    virtual void setEngineOptions(QString name, QString value); //! @todo MIGHT BE DIFFERENT FOR ENGINES
    virtual QString getEngineOption(QString name);              //! @todo MIGHT BE DIFFEReNT FOR ENGINES
    virtual void startNewGame();
    virtual bool isEngineProcessRunning();

    QStringList engineMoves();
    OptionSettingModel *optionSettingModel();

    //! The Path the Engine will write the Logfile to
    void setEngineLogPath(QString logPath);

    void sendEngineCommand(QString command);

    void appendEngineMove(QString move);
    void removeLastEngineMove();

  Q_SIGNALS:
    //! The Engine Result
    void engineResult(QString result);
    //! When the EngineMove List changed
    void engineMoveChanged(QStringList moveList);
    //! Error occurred while using the Engine
    void engineError(QString error, EngineErrorGrade errorGrade);

  protected Q_SLOTS:
    void onProcessReadyRead();
    void onEngineLogFlush();

  protected:
    //! The Process of the Engine running inside the engineThread
    QSharedPointer<QProcess> m_engineProcess;
    //! Used to save the Moves for the Engine, since they might have a different Notation
    QStringList m_engineMoves;

    void run() override;
    virtual void terminate(bool doExit);
    //! Set the current Position from :m_engineMoves
    //! @note Mainly used to reset to a older Position by changing m_engineMoves
    virtual void setMovePosition();

  private:
    //! Will be set true once we want to stop the Thread
    bool m_stopEngine;
    //! Usually false once the Thread started, true once the Thread stopped running
    bool m_engineStopped;
    //! The Logfile where the Engine writes Logs to
    QFile m_engineLog;
    //! Current list of all the Command that will be worked on in FIFO Queue
    QStringList m_commandList;
    //! One Line per Index of the Output we receive from the Engine
    QStringList m_engineOutput;
    //! Option List will be accessible through QML for Display
    OptionSettingModel *m_optionSettingModel;
  }; // class Engine

} // END NAMEPSACE "Stratogi"

#endif // ABSTRACT_ENGINE_HPP
