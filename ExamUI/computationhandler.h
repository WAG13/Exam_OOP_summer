#ifndef COMPUTATIONHANDLER_H
#define COMPUTATIONHANDLER_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QThreadPool>
#include <QFutureWatcher>
#include <functional>


class Computation : public QObject, public QRunnable
{
    Q_OBJECT
public:
    Computation(std::function<QVector<QString>()> func);

    void run() override;

signals:
    void printLine(QString line);
    void done(qint64);

private:
    std::function<QVector<QString>()> func;
};

class ComputationHandler : public QObject
{
    Q_OBJECT

public:
    ComputationHandler(QThreadPool* threadPool);

    void start(Computation* computation);

private:
    QThreadPool* threadPool;
};

#endif // COMPUTATIONHANDLER_H
