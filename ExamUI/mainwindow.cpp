#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Exam/Data/DataGenerator.h"
#include "Exam/Data/DateTime.h"
#include "Exam/Maps/StandardMap.h"
#include "Exam/Maps/TreeMap.h"
#include "Exam/Maps/ListMap.h"
#include "Exam/Sets/TreeSet.h"
#include "Exam/Sets/ListSet.h"
#include "Exam/Sets/HashSet.h"
#include "Exam/Sets/VectorSet.h"
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

    setDateTimeModelA = new QStandardItemModel();
    ui->listView_2->setModel(setDateTimeModelA);
    setDateTimeModelB = new QStandardItemModel();
    ui->listView_3->setModel(setDateTimeModelB);

    resetMaps(6);
    resetSets(4);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete intDateMap;
    delete mapDateTimeModel;
    delete dateSetA;
    delete dateSetB;
    delete setDateTimeModelA;
    delete setDateTimeModelB;
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


////////////////////////
/// TAB 1 (Maps)
////////////////////////


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
    ui->plainTextEdit_2->clear();
    resetMaps(ui->KVReal->currentIndex());
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



////////////////////////
/// TAB 2 (Sets)
////////////////////////


Set<DateTime>* MainWindow::getSet(int typeID)
{
    SetTreeType<DateTime>* setTreeType;
    SetListType<DateTime>* setListType;
    //SetHashType<DateTime>* setHashType;

    switch (typeID)
    {
    case 0:
        //Coalesced
        //setHashType = new SetHashType<DateTime>();
        //return new SetHashType<DateTime>(setHashType);
    case 1:
        //Hopscotch
        //setHashType = new SetHashType<DateTime>();
        //return new SetHashType<DateTime>(setHashType);
    case 2:
        //AVL
        setTreeType = new SetTreeTypeAVL<DateTime>();
        return new TreeSet<DateTime>(setTreeType);
    case 3:
        //B+
        setTreeType = new SetTreeTypeBPlus<DateTime>(500);
        return new TreeSet<DateTime>(setTreeType);
    case 4:
        //Doubly linked
        setListType = new SetListTypeDouble<DateTime>();
        return new ListSet<DateTime>(setListType);
    case 5:
        //Doubly linked circular
        setListType = new SetListTypeDoubleCircular<DateTime>();
        return new ListSet<DateTime>(setListType);
    case 6:
        //std::vector
        return new VectorSet<DateTime>();
    }
    return nullptr;
}

void MainWindow::resetSets(int typeID)
{
    if (intDateMap)
    {
        delete dateSetA;
        delete dateSetB;
        printMessage("Видаляємо контейнер");
    }

    dateSetA = getSet(typeID);
    dateSetB = getSet(typeID);

    printMessage("Створили новий контейнер");

    setDateTimeModelA->clear();
    setDateTimeModelB->clear();
}

void MainWindow::updateModels(bool setB)
{
    QStandardItemModel* model = setB ? setDateTimeModelB : setDateTimeModelA;
    Set<DateTime>* set = setB ? dateSetB : dateSetA;
    model->clear();
    for (auto it = set->begin(); it != set->end(); it++)
    {
        model->appendRow(new QStandardItem(QString::fromStdString(it->toString())));
    }
}

void MainWindow::on_generateRandom2_clicked()
{
    static RandomDataGenerator<DateTime> dataGen;
    static DateTime min(2000, 1, 1, 0, 0, 0);
    static DateTime max(2100, 1, 1, 0, 0, 0);

    int count = ui->spinBox_2->value();
    std::vector<DateTime> random = dataGen.generateVector(min, max, count);
    //printMessage("Додали " + QString::number(count) + " нових дат");

    addSetDateTime(random, ui->KVReal_5->currentIndex());
}

void MainWindow::on_addDateTime2_clicked()
{
    QDateTime qDateTime = ui->dateTimeEdit1->dateTime();
    DateTime dateTime = toDateTime(qDateTime);
    //printMessage("Додали " + QString::fromStdString(dateTime.toString()));

    addSetDateTime(dateTime, ui->KVReal_5->currentIndex());
}

void MainWindow::addSetDateTime(const DateTime &dateTime, bool setB)
{
    Set<DateTime>* set;
    QStandardItemModel* model;
    QListView* view;

    if (setB) {
        set = dateSetB;
        model = setDateTimeModelB;
        view = ui->listView_3;
    } else {
        set = dateSetA;
        model = setDateTimeModelA;
        view = ui->listView_2;
    }

    if (!set->contains(dateTime))
        set->insert(dateTime);
    model->appendRow(new QStandardItem(QString::fromStdString(dateTime.toString())));
}

void MainWindow::addSetDateTime(const std::vector<DateTime> &dateTimes, bool setB)
{
    for (DateTime dateTime : dateTimes)
        addSetDateTime(dateTime, setB);
}

void MainWindow::on_pushButton_2_clicked()
{
    DateTime value = toDateTime(ui->dateTimeEdit_3->dateTime());
    int type = ui->KVOperations_2->currentIndex();
    bool setB = ui->comboBox->currentIndex();
    Set<DateTime>* set = setB ? dateSetB : dateSetA;


    switch (type)
    {
    case 0: //contains?
        runComputationOnCurrentTab([&]()
        {
            QVector<QString> result;
            setMutex.lock();

            if (set->contains(value)) {
                result.push_back("Містить елемент " + QString::fromStdString(value.toString()));
            }
            else {
                result.push_back("Не містить елемент " + QString::fromStdString(value.toString()));
            }

            setMutex.unlock();
            return result;
        });
        break;
    case 1: //remove element
        Computation* computation = new Computation([&]()
        {
            setMutex.lock();

            if (!set->contains(value)) {
                update = false;
            } else {
                update = true;
                set->remove(value);
            }
            setMutex.unlock();
            return QVector<QString>();
        });
        updateSetB = setB;
        connect(computation, &Computation::done, this, &MainWindow::handleRemoval);
        connect(computation, &Computation::printLine, this, &MainWindow::printMessage);
        compHandler.start(computation);
        break;
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    int setImpl = ui->SortReal->currentIndex();
    int type = ui->KVOperations_3->currentIndex();
    switch (type)
    {
    case 0: //union
        runComputationOnCurrentTab([&]()
        {
            QVector<QString> result;
            setMutex.lock();

            std::unique_ptr<Set<DateTime>> set(getSet(setImpl));

            set->insertUnion(dateSetA, dateSetB);

            for (auto it = set->begin(); it != set->end(); it++)
                result.push_back(QString::fromStdString(it->toString()));

            setMutex.unlock();
            return result;
        });
        break;
    case 1: //intersection
        runComputationOnCurrentTab([&]()
        {
            QVector<QString> result;
            setMutex.lock();

            std::unique_ptr<Set<DateTime>> set(getSet(setImpl));

            set->insertIntersection(dateSetA, dateSetB);

            for (auto it = set->begin(); it != set->end(); it++)
                result.push_back(QString::fromStdString(it->toString()));

            setMutex.unlock();
            return result;
        });
        break;
    case 2: //complement
        runComputationOnCurrentTab([&]()
        {
            QVector<QString> result;
            setMutex.lock();

            std::unique_ptr<Set<DateTime>> set(getSet(setImpl));

            set->insertComplement(dateSetA, dateSetB);

            for (auto it = set->begin(); it != set->end(); it++)
                result.push_back(QString::fromStdString(it->toString()));

            setMutex.unlock();
            return result;
        });
        break;
    case 3: //symmetrical diff
        runComputationOnCurrentTab([&]()
        {
            QVector<QString> result;
            setMutex.lock();

            std::unique_ptr<Set<DateTime>> set(getSet(setImpl));

            set->insertSymmetricDiff(dateSetA, dateSetB);

            for (auto it = set->begin(); it != set->end(); it++)
                result.push_back(QString::fromStdString(it->toString()));

            setMutex.unlock();
            return result;
        });
        break;
    }
}

void MainWindow::handleRemoval(qint64 milliseconds)
{
    printElapsedTime(milliseconds);
    if (update)
        updateModels(updateSetB);
}

void MainWindow::on_KVReal_3_currentIndexChanged(int index)
{
    resetSets(index);
}

////////////////////////
/// TAB 3 (Sort)
////////////////////////
/*
Set<DateTime>* MainWindow::getSort(int typeID)
{
    SetTreeType<DateTime>* setTreeType;
    SetListType<DateTime>* setListType;
    switch (typeID)
    {
    case 0:
        //AVL
        setTreeType = new SetTreeTypeAVL<DateTime>();
        return new TreeSet<DateTime>(setTreeType);
    case 1:
        //B+
        setTreeType = new SetTreeTypeBPlus<DateTime>(500);
        return new TreeSet<DateTime>(setTreeType);
    case 2:
        //Doubly linked
        setListType = new SetListTypeDouble<DateTime>();
        delete new ListSet<DateTime>(setListType);
    case 3:
        //Doubly linked circular
        setListType = new SetListTypeDoubleCircular<DateTime>();
        delete new ListSet<DateTime>(setListType);
    case 4:
        //std::vector
        return new VectorSet<DateTime>();
    }
    return nullptr;

}

void MainWindow::resetSorts(int typeID)
{
    if (intDateSort)
    {
        delete intDateSort;
        printMessage("Видаляємо контейнер");
    }

    intDateSort = getSort(typeID);

    printMessage("Створили новий контейнер");

    sortDateTimeModel->clear();
}

void MainWindow::on_generateRandom3_clicked()
{
    static RandomDataGenerator<DateTime> dataGen;
    static DateTime min(2000, 1, 1, 0, 0, 0);
    static DateTime max(2100, 1, 1, 0, 0, 0);

    int count = ui->randomGenCount3->value();
    std::vector<DateTime> random = dataGen.generateVector(min, max, count);
    //printMessage("Додали " + QString::number(count) + " нових дат");

    addSortDateTime(random);
}

void MainWindow::on_addDateTime3_clicked()
{
    QDateTime qDateTime = ui->dateTimeEdit3->dateTime();
    DateTime dateTime = toDateTime(qDateTime);
    //printMessage("Додали " + QString::fromStdString(dateTime.toString()));

    addSortDateTime(dateTime);
}

void MainWindow::addSortDateTime(const DateTime &dateTime)
{
    Set<DateTime>* set;
    QStandardItemModel* model;
    QListView* view;

    set = intDateSort;
    model = sortDateTimeModel;
    view = ui->listView_4;

    if (!set->contains(dateTime))
        set->insert(dateTime);
    model->appendRow(new QStandardItem(QString::fromStdString(dateTime.toString())));
}

void MainWindow::addSortDateTime(const std::vector<DateTime> &dateTimes)
{
    for (DateTime dateTime : dateTimes)
        addSortDateTime(dateTime);
}

void MainWindow::on_pushButton3_clicked()
{

    int type = ui->SortReal->currentIndex();
    int setImpl = ui->sortType->currentIndex();
    int operation = ui->SortOperations->currentIndex();
    int kryt = ui->Kryt->currentIndex();
    std::unique_ptr<Sorting<DateTime>> sorting;


    std::vector<DateTime> result;
    Set<DateTime>* set = intDateSort;
    for (auto it = intDateSort->begin(); it != set->end(); it++)
        result.push_back(*it);

    switch (type)
    {
    case 0:
        sorting = std::make_unique<SelectionSort<DateTime>>();
        break;
    case 1:
        sorting = std::make_unique<HeapSort<DateTime>>();
        break;
    case 2:
        sorting = std::make_unique<MergeSort<DateTime>>();
        break;
    case 3:
        sorting = std::make_unique<InsertionSort<DateTime>>();
        break;
    case 4:
        sorting = std::make_unique<QuickSort<DateTime>>();
        break;
    }
    switch (kryt)
    {
    case 0:
        sorting->setComparator([](DateTime const& left, DateTime const& right) { return left < right; });
        break;
    case 1:
        sorting->setComparator([](DateTime const& left, DateTime const& right) { return left > right; });
        break;
    }

    switch (operation)
    {
    case 0:
        runComputationOnCurrentTab([&]()
        {
            QVector<QString> result;
            std::vector<DateTime> array;
            sortMutex.lock();
            std::unique_ptr<Set<DateTime>> set(getSet(setImpl));
            for (auto it = intDateSort->begin(); it != set->end(); it++)
                array.push_back(*it);
            sorting.get()->sort(array, 0, array.size());
            sortMutex.unlock();
            return result;
        });
        break;

    }


    addSortDateTime(result);

}
*/
