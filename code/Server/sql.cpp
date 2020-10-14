
#include "sql.h"

#include <QDateTime>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <iostream>
using namespace std;
SQL::SQL(QObject* parent) : QObject(parent) { db = nullptr; }

bool SQL::InitSql() {
    if (QSqlDatabase::contains("ChatUsers")) {
        this->PrintStr("���ݿ��Ѿ����ӣ������ٴγ�ʼ��");
        return true;
    }
    else {
        this->db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
        db->setDatabaseName("ChatUsers.db");
        if (db->open()) {
            this->Close();
            this->PrintStr("���ݿ��ʼ���ɹ�");
            return true;
        }
        else {
            this->PrintStr("���ݿ��ʼ��ʧ��");
            return false;
        }
    }
}

bool SQL::InitSql(QString ip, QString DbName, QString UserName, QString UserPWD) {
    //TODO: �Զ������ݿ�
    return false;
}

bool SQL::Open() {
    if (db->isValid() && !db->isOpen()) {
        if (db->open()) {
            this->PrintStr("���ݿ⿪���ɹ�");
            return true;
        }
        else {
            this->PrintStr("���ݿ⿪��ʧ��");
            return false;
        }
    }
    else {
        this->PrintStr("���ݿ⿪��ʧ��");
        return false;
    }
}

bool SQL::Close() {
    if (db->isValid() && db->open()) {
        QSqlQuery query;
        QString str = QString::fromLocal8Bit("delete from online_user");
        query.exec(str);
        db->close();
        this->PrintStr("���ݿ�رճɹ�");
        return true;
    }
    else {
        this->PrintStr("���ݿ�ر�ʧ��");
        return false;
    }
}

void SQL::PrintStr(string str) {
    QDateTime time = QDateTime::currentDateTime();
    string nowtime = time.toString().toLocal8Bit().toStdString();
    cout << nowtime + ">>" + str << endl;
}

bool SQL::Login(QString useName, QString userPwd) {
    if (!db->open()) {
        PrintStr("���������ݿ�δ��,�޷������¼ҵ��");

        return false;
    }
    else {
        QSqlQuery query;
        QString str = QString::fromLocal8Bit("select * from user_pwd where �ǳ�=? and ����=?;");
        query.prepare(str);
        query.addBindValue(useName);
        query.addBindValue(userPwd);
        query.exec();
        QSqlRecord rec = query.record();
        if (!query.first()) {
            PrintStr("�˺Ż����벻��ȷ");
            return false;
        }
        else {
            QString str2 = QString::fromLocal8Bit("SELECT * FROM online_user WHERE �ǳ�=") + useName + ";";
            query.exec(str2);
            rec = query.record();
            if (!query.first()) {
                // TODO
                str2.clear();
                str2 = QString::fromLocal8Bit("insert into online_user (�ǳ�) values(") + useName + ");";
                query.exec(str2);
                qDebug() << useName + QString::fromLocal8Bit("��¼�ɹ�");
                return true;
            }
            else {
                PrintStr(useName.toStdString() + "��¼ʧ��,���˺��Ѿ���¼");

                return false;
            }
        }
    }
}

bool SQL::Logout(QString useName) {
    if (!db->open()) {
        PrintStr("���������ݿ�δ��,�޷���������ҵ��");
        return false;
    }
    else {
        QSqlQuery query;
        QString str = QString::fromLocal8Bit("select * from user_pwd where �ǳ�=\"") + useName + "\"";
        query.exec(str);
        QSqlRecord rec = query.record();
        if (!query.first()) {
            PrintStr("�����ڸ��˺�");
            return false;
        }
        else {
            QString str2 = QString::fromLocal8Bit("SELECT * FROM online_user WHERE �ǳ�=\"") + useName + "\"";
            query.exec(str2);
            if (query.first()) {
                // TODO
                str2.clear();
                str2 = QString::fromLocal8Bit("delete from online_user where �ǳ�=\"") + useName + "\"";
                query.exec(str2);
                PrintStr(useName.toStdString() + "���߳ɹ�");
                return true;
            }
            else {
                PrintStr(useName.toStdString() + "����ʧ��,���˺��Ѿ�����");

                return false;
            }
        }
    }
}

bool SQL::IsOnline(QString useName) {
    if (!db->open()) {
        PrintStr("���������ݿ�δ��,�޷������ѯҵ��");
        return false;
    }
    else {
        QSqlQuery query;
        QString str = QString::fromLocal8Bit("select * from online_user where �ǳ�=") + useName + ";";
        query.exec(str);
        if (!query.first()) {
            PrintStr(useName.toLocal8Bit().toStdString() + "������");
            return false;
        }
        else {
            PrintStr(useName.toStdString() + "����");
            return true;
        }
    }
}

bool SQL::Logon(QString useName, QString userPwd) {
    if (!db->open()) {
        PrintStr("���������ݿ�δ��,�޷�����ע��ҵ��");
        return false;
    }
    else {
        if (useName == "" || userPwd == "") {
            PrintStr(useName.toStdString() + "ע��ʧ��,�˺ź����벻��Ϊ��");
            return false;
        }
        QSqlQuery query;
        QString str = QString::fromLocal8Bit("select * from user_pwd where �ǳ�=?;");
        query.prepare(str);
        query.addBindValue(useName);
        query.exec();
        if (query.first()) {
            PrintStr("�Ѿ����ڸ��˺�");
            return false;
        }
        else {
            QString str2 = "insert into user_pwd values(?,?,?);";

            query.clear();
            qDebug() << query.prepare(str2);
            query.addBindValue(useName);
            query.addBindValue(userPwd);
            query.addBindValue("");
            query.exec();
            QString str = QString::fromLocal8Bit("select * from user_pwd where �ǳ�=? and ����=?;");
            query.clear();
            query.prepare(str);
            query.addBindValue(useName);
            query.addBindValue(userPwd);
            query.exec();
            if (query.first()) {
                // TODO
                PrintStr(useName.toStdString() + "ע��ɹ�");
                return true;
            }
            else {
                PrintStr(useName.toStdString() + "ע��ʧ��");
                return false;
            }
        }
    }
}

bool SQL::Upheadimg(QString useName, QString userPwd, QString imgdata) {
    if (!db->open()) {
        PrintStr("���������ݿ�δ��,�޷��ϴ�ͷ��");
        return false;
    }
    else {
        QSqlQuery query;
        QString str = QString::fromLocal8Bit("update user_pwd set ͷ��=\"") + imgdata + QString::fromLocal8Bit("\" where �ǳ�=? and ����=?;");

        query.prepare(str);
        query.addBindValue(useName);
        query.addBindValue(userPwd);
        //        qDebug() << str;
        if (query.exec())
            return true;
        else
            return false;
    }
}

QString SQL::HeadOf(QString useName, QString) {
    if (!db->open()) {
        PrintStr("���������ݿ�δ��,�޷�����ע��ҵ��");
        return "";
    }
    else {
        QString str = QString::fromLocal8Bit("select ͷ�� from user_pwd where �ǳ�=?;");
        QSqlQuery query;
        query.prepare(str);
        query.addBindValue(useName);
        if (query.exec()) {
            if (query.first()) {
                return query.value(QString::fromLocal8Bit("ͷ��")).toString();
            }
            else {
                return "";
            }

        }
        else {
            return "";
        }
    }
}

QList<QString> SQL::OnlineList() {
    QList<QString> onlines;
    if (!db->open()) {
        PrintStr("���������ݿ�δ��,�޷�����ע��ҵ��");
        return onlines;
    }
    else {
        QSqlQuery query;
        QString str = "SELECT *"
            "FROM online_user;";
        query.exec(str);
        QSqlRecord rec;
        while (query.next()) {
            rec = query.record();
            int snamecol = rec.indexOf(QString::fromLocal8Bit("�ǳ�"));
            QString value = rec.value(snamecol).toString();
            // qDebug() << value;
            onlines.push_back(value);
        }
        return onlines;
    }
}
