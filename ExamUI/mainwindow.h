#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <Exam/Data/DateTime.h>
#include <Exam/Maps/Map.h>

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

    // @brief Occurs on changing the implementation of Map
    void on_KVReal_currentIndexChanged(int index);

    void on_KVReal_3_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;

    ////////////////////////
    /// TAB 1 (Maps)
    ////////////////////////

    Map<int, DateTime>* intDateMap = nullptr;

    void resetMaps(int typeID);
};
#endif // MAINWINDOW_H
