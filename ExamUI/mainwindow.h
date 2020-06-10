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
#include "Exam/Sets/Set.h"
#include "Exam/SortingAlgorithms/SortingAlgorithms.h"
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

    /// @brief Occurs on pressing the "Generate randomly" button
    void on_generateRandom1_clicked();

    /// @brief Occurs on pressing the "Add manually" button
    void on_addDateTime1_clicked();

    /// @brief Occurs when the user wants to create a map using base values
    void on_createMap_clicked();

    /// @brief Occurs on selecting desired operation
    void on_pushButton_clicked();


    ////////////////////////
    /// TAB 2 (Sets)
    ////////////////////////

    /// @brief Occurs on changing the implementation of Set
    void on_KVReal_3_currentIndexChanged(int index);

    /// @brief Occurs on pressing the "Generate randomly" button
    void on_generateRandom2_clicked();

    /// @brief Occurs on pressing the "Add manually" button
    void on_addDateTime2_clicked();

    /// @brief Occurs on selecting desired operation (for one Set)
    void on_pushButton_2_clicked();

    /// @brief Occurs on selecting desired operation (for both Sets)
    void on_pushButton_3_clicked();

/*
    ////////////////////////
    /// TAB 3 (Sort)
    ////////////////////////

    /// @brief Occurs on changing the implementation of Sort
    //void on_KVReal_currentIndexChanged(int index);

    /// @brief Occurs on pressing the "Generate randomly" button
    void on_generateRandom3_clicked();

    /// @brief Occurs on pressing the "Add manually" button
    void on_addDateTime3_clicked();

    /// @brief Occurs on selecting desired operation
    void on_pushButton3_clicked();
*/

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

    ////////////////////////
    /// TAB 2 (Sets)
    ////////////////////////

    QMutex setMutex;
    Set<DateTime>* dateSetA = nullptr;
    Set<DateTime>* dateSetB = nullptr;

    QStandardItemModel* setDateTimeModelA;
    QStandardItemModel* setDateTimeModelB;

    void addSetDateTime(const DateTime& dateTime, bool setB);
    void addSetDateTime(const std::vector<DateTime>& dateTimes, bool setB);
    Set<DateTime>* getSet(int typeID);
    void resetSets(int typeID);
    bool updateSetB = false;
    bool update = false;
    void handleRemoval(qint64 milliseconds);
    void updateModels(bool setB);
/*
    ///////////////////////
    /// TAB 3 (Sort)
    ////////////////////////

    QMutex sortMutex;
    Set<DateTime>* getSort(int typeID);
    std::vector<DateTime> sortDateTime;
    Set<DateTime>* intDateSort = nullptr;
    QStandardItemModel* sortDateTimeModel;

    void addSortDateTime(const DateTime& dateTime);
    void addSortDateTime(const std::vector<DateTime>& dateTimes);
    void resetSorts(int typeID);*/

};
#endif // MAINWINDOW_H
