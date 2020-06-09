#include "computationhandler.h"

#include "QElapsedTimer"

Computation::Computation(std::function<QVector<QString>()> func)
    : func(func)
{}

void Computation::run()
{
    QElapsedTimer timer;
    timer.start();

    QVector<QString> result = func();

    qint64 elapsed = timer.elapsed();

    for (const QString& line : result)
        emit printLine(line);
    emit done(elapsed);
}



ComputationHandler::ComputationHandler(QThreadPool* threadPool)
    : threadPool(threadPool)
{}

void ComputationHandler::start(Computation* computation)
{
    threadPool->start(computation);
}
