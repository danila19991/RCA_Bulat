﻿#include <QCoreApplication>
#include <QFile>
#include <QSettings>
#include <QProcess>
#include <QDir>

#include "Objects/controlunit.h"
#include "Objects/planner.h"
#include "Objects/scene.h"

#include "Tests/connectiontests.h"
#include "Tests/reconnectiontests.h"
#include "Tests/transfertests.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    const QString defaultRcaIp     = "localhost";
    const QString defaultSceneIp   = "localhost";
    const quint16 defaultRcaPort   = 8000;
    const quint16 defaultScenePort = 8080;

    QSettings settings("../config.ini", QSettings::IniFormat);
    QString rcaIp = settings.value("HOSTS/Rca", defaultRcaIp).toString();
    QString sceneIp = settings.value("HOSTS/Scene", defaultSceneIp).toString();
    quint16 rcaPort = static_cast<quint16>(settings.value("PORTS/Rca", defaultRcaPort).toInt());
    quint16 scenePort  = static_cast<quint16>(settings.value("PORTS/Scene", defaultScenePort).toInt());

    QDir dir = QDir::current();
    dir.cdUp();
    dir.cdUp();
    dir.cd("System/release");
    QString pathToRcaExec = dir.path();

    int waitTime = 300;

    auto res1 = QTest::qExec(new ConnectionTests(rcaIp, sceneIp, rcaPort, scenePort, pathToRcaExec, waitTime), argc, argv);
    auto res2 = QTest::qExec(new TransferTests(rcaIp, sceneIp, rcaPort, scenePort, pathToRcaExec, waitTime), argc, argv);
    auto res3 = QTest::qExec(new ReconnectionTests(rcaIp, sceneIp, rcaPort, scenePort, pathToRcaExec, waitTime), argc, argv);
    return res1 + res2 + res3;
}
