#include "transfertests.h"

TransferTests::TransferTests(QString rcaIp, QString sceneIp, quint16 rcaPort, quint16 scenePort,
                             QString pathToRcaExec, int waitTime, QObject *parent)
    : QObject(parent),
      rcaIp(rcaIp), sceneIp(sceneIp), rcaPort(rcaPort), scenePort(scenePort),
      pathToRcaExec(pathToRcaExec), waitTime(waitTime), scene(nullptr)
{

}

void TransferTests::init()
{
    scene = new Scene(scenePort);
    scene->startServer();

    rcaProcess.start(pathToRcaExec + "/System", QStringList());
    QTest::qWait(waitTime);
}

void TransferTests::cleanup()
{
    scene->deleteLater();
    rcaProcess.kill();
    rcaProcess.waitForFinished();
    QTest::qWait(waitTime);
}

void TransferTests::sendFromPlannerToUnitWithoutResponse()
{
    Planner planner("p", rcaIp, rcaPort);
    planner.connectToServer();
    ControlUnit unit("t", rcaIp, rcaPort);
    unit.connectToServer();
    QTest::qWait(waitTime);
    planner.sendMsg("t:message");
    QTest::qWait(waitTime);

    QVERIFY(unit.getLastMessage() == "message");
    QCOMPARE(scene->getLastMessage().isEmpty(), true);
}

void TransferTests::sendFromUnitToScene()
{
    ControlUnit unit("t", rcaIp, rcaPort);
    unit.connectToServer();
    QTest::qWait(waitTime);
    unit.sendMsgToScene("message");
    QTest::qWait(waitTime);

    QCOMPARE(scene->getLastMessage(), "message");
}

void TransferTests::sendFromPlannerToUnitWithResponse()
{
    Planner planner("p", rcaIp, rcaPort);
    planner.connectToServer();
    ControlUnit unit("t", rcaIp, rcaPort);
    unit.connectToServer();
    QTest::qWait(waitTime);
    planner.sendMsg("t:message");
    QTest::qWait(waitTime);
    bool unitReceivedMessageFromPlanner = (unit.getLastMessage() == "message");
    unit.sendMsgToScene("message");
    QTest::qWait(waitTime);
    bool sceneReceivedMessageFromUnit = (scene->getLastMessage() == "message");

    QVERIFY(unitReceivedMessageFromPlanner);
    QVERIFY(sceneReceivedMessageFromUnit);
}

void TransferTests::sendFromPlannerToUnconnectedUnit()
{
    Planner planner("p", rcaIp, rcaPort);
    planner.connectToServer();
    ControlUnit unit("t", rcaIp, rcaPort);
    QTest::qWait(waitTime);
    planner.sendMsg("t:message1");
    QTest::qWait(waitTime);
    bool unconnectedUnitNotReceivedMessage = unit.getLastMessage().isEmpty();
    unit.connectToServer();
    QTest::qWait(waitTime);
    planner.sendMsg("t:message2");
    QTest::qWait(waitTime);
    bool connectedUnitGotMessage = unit.getLastMessage() == "message2";

    QCOMPARE(unconnectedUnitNotReceivedMessage, true);
    QCOMPARE(connectedUnitGotMessage, true);
}

void TransferTests::sendFromPlannerToMultipleUnits()
{
    Planner planner("p", rcaIp, rcaPort);
    planner.connectToServer();
    ControlUnit unitT("t", rcaIp, rcaPort);
    unitT.connectToServer();
    QTest::qWait(waitTime);
    ControlUnit unitF("f", rcaIp, rcaPort);
    unitF.connectToServer();
    QTest::qWait(waitTime);
    planner.sendMsg("t:messageT|f:messageF");
    QTest::qWait(waitTime);

    QCOMPARE(unitT.getLastMessage(), "messageT");
    QCOMPARE(unitF.getLastMessage(), "messageF");

}

void TransferTests::sendFromPlannerToUnitWithoutDelay()
{
    Planner planner("p", rcaIp, rcaPort);
    planner.connectToServer();
    ControlUnit unit("t", rcaIp, rcaPort);
    unit.connectToServer();
    QTest::qWait(waitTime);
    planner.sendMsg("t:message1|t:message2");
    QTest::qWait(waitTime);

    QVERIFY(unit.getLastMessage() == "message1message2");
}

void TransferTests::sendFromMultipleUnitsToScene()
{
    ControlUnit unitT("t", rcaIp, rcaPort);
    unitT.connectToServer();
    QTest::qWait(waitTime);
    ControlUnit unitF("f", rcaIp, rcaPort);
    unitF.connectToServer();
    QTest::qWait(waitTime);
    unitT.sendMsgToScene("messageT");
    unitF.sendMsgToScene("messageF");
    QTest::qWait(waitTime);

    QVERIFY(scene->getLastMessage() == "messageTmessageF");
}

void TransferTests::sendExitCmd()
{
    Planner planner("p", rcaIp, rcaPort);
    planner.connectToServer();
    ControlUnit unit("t", rcaIp, rcaPort);
    unit.connectToServer();
    QTest::qWait(waitTime);
    planner.sendMsg("e");
    QTest::qWait(waitTime);
    bool unitDisconnected = unit.isDisconnected();
    bool plannerDisconnected = planner.isDisconnected();
    bool rcaDicsonnected = (rcaProcess.state() == QProcess::NotRunning);

    QCOMPARE(unitDisconnected, true);
    QCOMPARE(plannerDisconnected, true);
    QCOMPARE(rcaDicsonnected, true);
}
