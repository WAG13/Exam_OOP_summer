#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Exam/Maps/StandardMap.h"
#include "Exam/Maps/ListMap.h"
#include "Exam/Maps/TreeMap.h"
#include <memory>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resetMaps(6);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete intDateMap;
}

void MainWindow::printMessage(QString string)
{
    switch (ui->tabWidget->currentIndex())
    {
    case 0:
        ui->plainTextEdit_2->appendPlainText(string);
        break;
    case 1:
        ui->plainTextEdit_3->appendPlainText(string);
        break;
    case 2:
        ui->plainTextEdit_4->appendPlainText(string);
        break;
    }
}

void MainWindow::on_KVReal_currentIndexChanged(int index)
{
    resetMaps(index);
}

void MainWindow::resetMaps(int typeID)
{
    if (intDateMap)
    {
        delete intDateMap;
        printMessage("Видаляємо контейнер");
    }
    MapTreeType<int, DateTime>* mapTreeType;
    MapListType<int, DateTime>* mapListType;
    switch (typeID)
    {
    case 2:
        //AVL
        mapTreeType = new MapTreeTypeAVL<int, DateTime>();
        intDateMap = new TreeMap<int, DateTime>(mapTreeType);
        break;
    case 3:
        //B+
        mapTreeType = new MapTreeTypeBPlus<int, DateTime>(5);
        intDateMap = new TreeMap<int, DateTime>(mapTreeType);
        break;
    case 4:
        //Doubly linked
        mapListType = new MapListTypeDouble<int, DateTime>();
        intDateMap = new ListMap<int, DateTime>(mapListType);
        break;
    case 5:
        //Doubly linked circular
        mapListType = new MapListTypeDoubleCircular<int, DateTime>();
        intDateMap = new ListMap<int, DateTime>(mapListType);
        break;
    case 0:
        //Coalesced
    case 1:
        //Hopscotch
    case 6:
        //std::map
        intDateMap = new StandardMap<int, DateTime>();
        break;
    }
    printMessage("Створили новий контейнер");
}



void MainWindow::on_KVReal_3_currentIndexChanged(int index)
{
    printMessage(QString::number(index));
}

