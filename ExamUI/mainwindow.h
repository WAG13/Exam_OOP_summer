#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>
#include <QMutex>
#include <QThreadPool>
#include <QMainWindow>
#include <QString>
#include <QDateTime>
#include <QStandardItemModel>
#include "Exam/Data/DateTime.h"
#include "Exam/Maps/Map.h"
#include "computationhandler.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void printMessage(QString string);

    ////////////////////////
    /// TAB 1 (Maps)
    ////////////////////////

    /// @brief Occurs on changing the implementation of Map
    void on_KVReal_currentIndexChanged(int index);

    /// @brief Occurs on selecting Random values vs Manual input
    void on_KVReal_3_currentIndexChanged(int index);

    /// @brief Occurs on pressing the "Generate randomly" button
    void on_generateRandom1_clicked();

    /// @brief Occurs on pressing the "Add manually" button
    void on_addDateTime1_clicked();

    /// @brief Occurs when the user wants to create a map using base values
    void on_createMap_clicked();

    /// @brief Occurs on selecting desired operation
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    static DateTime toDateTime(const QDateTime& dateTime);

    ComputationHandler compHandler;
    void runComputationOnCurrentTab(std::function<QVector<QString>()> func);
    void printElapsedTime(qint64 milliseconds);

    ////////////////////////
    /// TAB 1 (Maps)
    ////////////////////////

    QMutex mapMutex;
    std::vector<DateTime> mapDateTimeBase;
    Map<DateTime, int>* intDateMap = nullptr;

    QStandardItemModel* mapDateTimeModel;

    void addBaseMapDateTime(const DateTime& dateTime);
    void addBaseMapDateTime(const std::vector<DateTime>& dateTimes);
    void resetMaps(int typeID);
};
#endif // MAINWINDOW_H
