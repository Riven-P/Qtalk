#ifndef SQL_H
#define SQL_H
#include <QObject>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <iostream>

using namespace std;
class SQL : public QObject {
    Q_OBJECT
  public:
    explicit SQL(QObject* parent = nullptr);
    bool InitSql();  //Ĭ�ϳ�ʼ��
    bool InitSql(QString ip, QString DbName, QString UserName, QString UserPWD);
    bool Open();
    bool Close();
    void PrintStr(string str);                                          //��ӡ��Ϣ
    bool Login(QString useName, QString userPwd);                       //��¼
    bool Logout(QString useName);                                       //����
    bool IsOnline(QString useName);                                     //�Ƿ�����
    bool Logon(QString useName, QString userPwd);                       //ע��
    bool Upheadimg(QString useName, QString userPwd, QString imgdata);  //�ϴ�ͷ��
    QString HeadOf(QString useName, QString uesrPwd);                   //��ȡͷ��
    QList<QString> OnlineList();                                        //��ȡ�����б�
  signals:
  private:
    QSqlDatabase* db;
};

#endif  // SQL_H
