#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Exam/Data/DataGenerator.h"
#include "Exam/Data/DateTime.h"
#include "Exam/Maps/StandardMap.h"
#include "Exam/Maps/ListMap.h"
#include "Exam/Maps/TreeMap.h"
#include <QDate>
#include <QTime>
#include <memory>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , compHandler(QThreadPool::globalInstance())
{
    ui->setupUi(this);

    mapDateTimeModel = new QStandardItemModel();
    ui->tableView->setModel(mapDateTimeModel);


    resetMaps(6);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete intDateMap;
    delete mapDateTimeModel;
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

void MainWindow::printElapsedTime(qint64 milliseconds)
{
    printMessage("Операція зайняла " + QString::number(milliseconds) + " мілісекунд.");
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
    MapTreeType<DateTime, int>* mapTreeType;
    MapListType<DateTime, int>* mapListType;
    switch (typeID)
    {
    case 2:
        //AVL
        mapTreeType = new MapTreeTypeAVL<DateTime, int>();
        intDateMap = new TreeMap<DateTime, int>(mapTreeType);
        break;
    case 3:
        //B+
        mapTreeType = new MapTreeTypeBPlus<DateTime, int>(5);
        intDateMap = new TreeMap<DateTime, int>(mapTreeType);
        break;
    case 4:
        //Doubly linked
        mapListType = new MapListTypeDouble<DateTime, int>();
        intDateMap = new ListMap<DateTime, int>(mapListType);
        break;
    case 5:
        //Doubly linked circular
        mapListType = new MapListTypeDoubleCircular<DateTime, int>();
        intDateMap = new ListMap<DateTime, int>(mapListType);
        break;
    case 0:
        //Coalesced
    case 1:
        //Hopscotch
    case 6:
        //std::map
        intDateMap = new StandardMap<DateTime, int>();
        break;
    }
    printMessage("Створили новий контейнер");

    runComputationOnCurrentTab([&]() {
        QVector<QString> result;

        mapMutex.lock();
        for (size_t i = 0; i < mapDateTimeBase.size(); i++)
            intDateMap->set(i, mapDateTimeBase[i]);
        result.push_back("Додали " + QString::number(mapDateTimeBase.size()) + " елементів");
        mapMutex.unlock();

        return result;
    });
}



void MainWindow::on_KVReal_3_currentIndexChanged(int index)
{
    printMessage(QString::number(index));
}

void MainWindow::on_generateRandom1_clicked()
{
    static RandomDataGenerator<DateTime> dataGen;
    static DateTime min(2000, 1, 1, 0, 0, 0);
    static DateTime max(2100, 1, 1, 0, 0, 0);

    int count = ui->randomGenCount1->value();
    std::vector<DateTime> random = dataGen.generateVector(min, max, count);
    //printMessage("Додали " + QString::number(count) + " нових дат");

    addBaseMapDateTime(random);
}

void MainWindow::on_addDateTime1_clicked()
{
    QDateTime qDateTime = ui->dateTimeEdit1->dateTime();
    DateTime dateTime = toDateTime(qDateTime);
    //printMessage("Додали " + QString::fromStdString(dateTime.toString()));

    addBaseMapDateTime(dateTime);

}


void MainWindow::addBaseMapDateTime(const DateTime &dateTime)
{
    mapDateTimeBase.push_back(dateTime);
    QList<QStandardItem*> row;
    row.append(new QStandardItem(QString::number(mapDateTimeBase.size() - 1)));
    row.append(new QStandardItem(QString::fromStdString(dateTime.toString())));
    mapDateTimeModel->appendRow(row);

    ui->tableView->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::addBaseMapDateTime(const std::vector<DateTime> &dateTimes)
{
    for (DateTime dateTime : dateTimes)
        addBaseMapDateTime(dateTime);
}

void MainWindow::on_createMap_clicked()
{
    resetMaps(ui->KVReal->currentIndex());
}


DateTime MainWindow::toDateTime(const QDateTime& dateTime)
{
    QDate qDate = dateTime.date();
    QTime qTime = dateTime.time();
    return DateTime(qDate.year(), qDate.month(), qDate.day(), qTime.hour(), qTime.minute(), qTime.second());
}



void MainWindow::runComputationOnCurrentTab(std::function<QVector<QString>()> func)
{
    Computation* computation = new Computation(func);
    connect(computation, &Computation::done, this, &MainWindow::printElapsedTime);
    connect(computation, &Computation::printLine, this, &MainWindow::printMessage);
    compHandler.start(computation);
}


void MainWindow::on_pushButton_clicked()
{
    int type = ui->KVOperations->currentIndex();
    int key;
    QDateTime qValue;
    DateTime value;

    switch (type)
    {
    case 0: //get element
        key = ui->KVKeyInt->value();
        runComputationOnCurrentTab([&]()
        {
            QVector<QString> result;
            mapMutex.lock();

            if (!intDateMap->contains(key)) {
                result.push_back("Не містить ключа " + QString::number(key));
            }
            else {
                result.push_back(QString::fromStdString(intDateMap->get(key).toString()));
            }

            mapMutex.unlock();
            return result;
        });
        break;
    case 1: //set element
        key = ui->KVKeyInt->value();
        qValue = ui->dateTimeEdit->dateTime();
        value = toDateTime(qValue);

        runComputationOnCurrentTab([&]()
        {
            mapMutex.lock();

            intDateMap->set(key, value);

            mapMutex.unlock();
            return QVector<QString>();
        });
        break;
    case 2: //remove element
        key = ui->KVKeyInt->value();
        runComputationOnCurrentTab([&]()
        {
            mapMutex.lock();

            intDateMap->remove(key);

            mapMutex.unlock();
            return QVector<QString>();
        });
        break;
    case 3: //all keys
        key = ui->KVKeyInt->value();
        runComputationOnCurrentTab([&]()
        {
            QVector<QString> result;
            mapMutex.lock();

            std::vector<int> keys = intDateMap->getKeys();
            for (int key : keys)
                result.push_back(QString::number(key));

            mapMutex.unlock();
            return result;
        });
        break;
    case 4: //all values
        runComputationOnCurrentTab([&]()
        {
            QVector<QString> result;
            mapMutex.lock();

            std::vector<DateTime> values = intDateMap->getValues();
            for (const DateTime& dateTime : values)
                result.push_back(QString::fromStdString(dateTime.toString()));

            mapMutex.unlock();
            return result;
        });
        break;
    case 5: //all pairs
        key = ui->KVKeyInt->value();
        runComputationOnCurrentTab([&]()
        {
            QVector<QString> result;
            mapMutex.lock();

            std::vector<std::pair<int, DateTime>> kvps = intDateMap->getKVPs();
            for (const auto& pair : kvps)
            {
                result.push_back("[ " + QString::number(pair.first) + " ] = " +
                                 QString::fromStdString(pair.second.toString()));
            }

            mapMutex.unlock();
            return result;
        });
        break;
    }
}
