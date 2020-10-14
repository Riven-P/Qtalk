#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H
#include <QObject>
#include <QTcpSocket>

class ClientSocket : public QTcpSocket {
    Q_OBJECT
  public:
    int des;
    ClientSocket();  
  signals:
    void Discon(int);                           //�Ͽ��ź�
    void Login(int, QString, QString);          //��¼
    void Logon(int, QString, QString);          //ע��
    void Logout(int, QString);                  //����
    void Sendnews(int, QString, QString);       //������Ϣ������
    void SendUser(int);                         //���������б�
    void Upheadimg(QString, QString, QString);  //�ϴ�ͷ��
  private slots:
    void DataRece();  //����
    void DiscCon();   //�Ͽ�
};

#endif  // CLIENTSOCKET_H
