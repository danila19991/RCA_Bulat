#include <QApplication>

#include "Objects/controlunit.h"
#include "Objects/planner.h"
#include "Objects/scene.h"

void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QFile file("D:/Qt Programs New/RCA/log.txt");
    if (!file.open(QIODevice::Append | QIODevice::Text))
    {
        return;
    }

    QTextStream stream(&file);
    QString currentDate = "[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz") + "]";

    switch (type)
    {
        case QtDebugMsg:
            stream << QString("%1\nFunction \"%2\", Line %3\nDebug: %4\n\n").
                      arg(currentDate).arg(context.function).arg(context.line).arg(msg);
            break;
        case QtWarningMsg:
            stream << QString("%1\nFunction \"%2\", Line %3\nWarning: %4\n\n").
                      arg(currentDate).arg(context.function).arg(context.line).arg(msg);
            break;
        case QtCriticalMsg:
            stream << QString("%1\nFunction \"%2\", Line %3\nCritical: %4\n\n").
                      arg(currentDate).arg(context.function).arg(context.line).arg(msg);
            break;
        case QtFatalMsg:
            stream << QString("%1\nFunction \"%2\", Line %3\nFatal: %4\n\n").
                      arg(currentDate).arg(context.function).arg(context.line).arg(msg);
            break;
        case QtInfoMsg:
            stream << QString("%1\nFunction \"%2\", Line %3\nInfo: %4\n\n").
                      arg(currentDate).arg(context.function).arg(context.line).arg(msg);
            break;
    }

    stream.flush();
    file.flush();
    file.close();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qInstallMessageHandler(messageHandler);

    Scene* scene = new Scene;
    Q_UNUSED(scene);

    Planner* planner = new Planner("p");
    planner->show();

    ControlUnit* unit1 = new ControlUnit("t");
    unit1->show();

    ControlUnit* unit2 = new ControlUnit("f");
    unit2->show();

    return a.exec();
}
