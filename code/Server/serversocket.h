#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <QObject>
#include <QTcpServer>
#include <iostream>

#include "clientsocket.h"
#include "sql.h"
using namespace std;
class ServerSocket : public QTcpServer {
    Q_OBJECT
  public:
    ServerSocket();
    QList<ClientSocket*>* conList;
    //  QList<ClientSocket> *onlineList;
    QMap<int, QString>* onlineUsers;
    void PrintStr(string str);  //��ӡ��Ϣ
  public slots:
    void Discon(int);                           //�Ͽ��ź�,���ж��Ƿ���Ҫִ�����߲���
    void Login(int, QString, QString);          //��¼
    void Logon(int, QString, QString);          //ע��
    void Logout(int, QString);                  //����
    void Sendnews(int, QString, QString);       //������Ϣ������
    void CLoseServer();                         //�رշ�����
    void SendUsers(int);                        //���������б�
    void Upheadimg(QString, QString, QString);  //�ϴ�ͷ��
  protected:
    void incomingConnection(qintptr handle) override;
    SQL* sql;
};

#endif  // SERVERSOCKET_H
