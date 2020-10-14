#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#    pragma execution_character_set("utf-8")
#endif
#include "serversocket.h"

#include <QDateTime>
#include <QElapsedTimer>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
ServerSocket::ServerSocket() {
    sql = new SQL();
    sql->InitSql();
    sql->Open();
    conList = new QList<ClientSocket*>();
    onlineUsers = new QMap<int, QString>();
    listen(QHostAddress::Any, 12345);
}

void ServerSocket::PrintStr(string str) {
    QDateTime time = QDateTime::currentDateTime();
    string nowtime = time.toString().toLocal8Bit().toStdString();
    cout << nowtime + ">>" + QString::fromStdString(str).toLocal8Bit().toStdString() << endl;
}

void ServerSocket::Discon(int t) {
    auto temp = onlineUsers->value(t);
    if (temp == nullptr) {
        for (auto& x : *conList) {
            if (x->des == t) {
                PrintStr(QString::number(x->peerPort()).toStdString() + "|�Ͽ��������������");
                conList->removeOne(x);
            }
        }
    } else {
        sql->Logout(temp);
        onlineUsers->remove(t);
        QJsonDocument doc;
        QJsonArray jsonarray;
        QJsonObject obj;
        QList<QString> users = sql->OnlineList();
        QString temp;

        for (auto& x : users) {
            QJsonArray* temp = new QJsonArray;
            temp->append(x);
            temp->append(sql->HeadOf(x, ""));
            jsonarray.append(*temp);
        }
        obj.insert("header", 5);
        obj.insert("return", 1);
        obj.insert("from", "null");
        obj.insert("msg", jsonarray);
        doc.setObject(obj);
        for (auto& x : *conList) {
            for (auto han : onlineUsers->keys()) {
                if (han == x->socketDescriptor()) {
                    x->write(doc.toJson(QJsonDocument::Compact));
                }
            }
        }
    }
}

void ServerSocket::Login(int handle, QString user, QString pwd) {
    if (sql->Login(user, pwd)) {
        onlineUsers->insert(handle, user);
        // ���͵�¼�ɹ��źź�ˢ�������б��Զ�̶˵�,��ˢ���б�
        QString headimg = sql->HeadOf(user, pwd);

        QJsonDocument doc;
        QJsonArray jsonarray;
        QJsonObject obj;
        QList<QString> users = sql->OnlineList();
        for (auto& x : users) {
            QJsonArray* temp = new QJsonArray;
            temp->append(x);
            temp->append(sql->HeadOf(x, ""));
            jsonarray.append(*temp);
        }

        obj.insert("header", 1);
        obj.insert("return", 1);
        obj.insert("from", "null");
        obj.insert("headImg", headimg);

        doc.setObject(obj);
        // qDebug() << str_ << Qt::endl;
        for (auto& x : *conList) {
            if (x->socketDescriptor() == handle) {
                x->write(doc.toJson(QJsonDocument::Compact));
            }
        }
        //���͸������û�

        obj.insert("header", 5);
        //        obj.value("header") = 5;
        obj.insert("msg", jsonarray);
        doc.setObject(obj);
        for (auto& x : *conList) {
            // TODO
            for (auto han : onlineUsers->keys()) {
                if (han == x->socketDescriptor() && han != handle) {
                    x->write(doc.toJson(QJsonDocument::Compact));
                }
            }
        }

    } else {
        // ���͵�¼ʧ���źŸ�Զ�̶˵�
        QJsonDocument doc;
        QJsonArray jsonarray;
        QJsonObject obj;
        obj.insert("header", 1);
        obj.insert("return", 0);
        obj.insert("from", "null");
        obj.insert("msg", "");
        doc.setObject(obj);
        for (auto& x : *conList) {
            if (x->socketDescriptor() == handle) {
                x->write(doc.toJson(QJsonDocument::Compact));
            }
        }
    }
}

void ServerSocket::Logon(int handle, QString user, QString pwd) {
    if (sql->Logon(user, pwd)) {
        // ����ע��ɹ��źŸ�Զ�̶˵�
        QJsonDocument doc;
        QJsonArray jsonarray;
        QJsonObject obj;
        obj.insert("header", 2);
        obj.insert("return", 1);
        obj.insert("from", "null");
        obj.insert("msg", "");
        doc.setObject(obj);

        for (auto& x : *conList) {
            if (x->socketDescriptor() == handle) {
                x->write(doc.toJson(QJsonDocument::Compact));
            }
        }
    } else {
        // ����ע��ʧ���źŸ�Զ�̶˵�
        QJsonDocument doc;
        QJsonArray jsonarray;
        QJsonObject obj;
        obj.insert("header", 2);
        obj.insert("return", 0);
        obj.insert("from", "null");
        obj.insert("msg", "");
        doc.setObject(obj);
        for (auto& x : *conList) {
            if (x->socketDescriptor() == handle) {
                x->write(doc.toJson(QJsonDocument::Compact));
            }
        }
    }
}

void ServerSocket::Logout(int handle, QString user) {
    if (sql->Logout(user)) {
        onlineUsers->remove(handle);
        // �������߳ɹ��źź�ˢ�������б��źŸ�Զ�̶˵㣬��ˢ���б�
        QJsonDocument doc;
        QJsonArray jsonarray;
        QJsonObject obj;
        QList<QString> users = sql->OnlineList();

        for (auto& x : users) {
            QJsonArray* temp = new QJsonArray;
            temp->append(x);
            temp->append(sql->HeadOf(x, ""));
            jsonarray.append(*temp);
        }
        obj.insert("header", 5);
        obj.insert("return", 1);
        obj.insert("from", "null");
        obj.insert("msg", jsonarray);
        doc.setObject(obj);
        for (auto& x : *conList) {
            for (auto han : onlineUsers->keys()) {
                if (han == x->socketDescriptor()) {
                    x->write(doc.toJson(QJsonDocument::Compact));
                }
            }
        }
    }
}

void ServerSocket::Sendnews(int handle, QString user, QString news) {
    // TODO:������Ϣ��ָ���ͻ���,����״̬��������
    if (sql->IsOnline(user) && onlineUsers->value(handle) != nullptr && onlineUsers->key(user) != handle) {
        QJsonDocument doc;
        QJsonObject obj;
        obj.insert("header", 6);
        obj.insert("return", 1);
        obj.insert("from", onlineUsers->value(handle));
        obj.insert("msg", news);

        for (auto& x : *conList) {
            if (x->socketDescriptor() == onlineUsers->key(user)) {
                doc.setObject(obj);
                //                QString str = "61&" + onlineUsers->value(handle) + "&" + news;
                x->write(doc.toJson(QJsonDocument::Compact));
                //        qDebug() << "����" << str << Qt::endl;
            }
        }

    } else {
        QJsonDocument doc;
        QJsonObject obj;
        obj.insert("header", 3);
        obj.insert("return", 0);
        obj.insert("sendto", user);
        obj.insert("msg", news);
        doc.setObject(obj);
        for (auto& x : *conList) {
            if (x->socketDescriptor() ==handle) {
                x->write(doc.toJson(QJsonDocument::Compact));
            }
        }
    }
}

void ServerSocket::CLoseServer() {
    sql->Close();
    this->close();
}

void ServerSocket::SendUsers(int handle) {
    for (auto& x : onlineUsers->keys()) {
        if (x == handle) {
            QList<QString> users = sql->OnlineList();
            QJsonDocument doc;
            QJsonObject obj;
            QJsonArray jsonarray;
            obj.insert("header", 5);
            obj.insert("return", 1);

            for (auto& x : users) {
                QJsonArray* temp = new QJsonArray;
                temp->append(x);
                temp->append(sql->HeadOf(x, ""));
                jsonarray.append(*temp);
            }
            obj.insert("from", "null");
            obj.insert("msg", jsonarray);
            doc.setObject(obj);
            for (auto& x : *conList) {
                if (x->socketDescriptor() == handle) {
                    x->write(doc.toJson(QJsonDocument::Compact));
                }
            }
        }
    }
}

void ServerSocket::Upheadimg(QString id, QString pwd, QString imgdata) {
    if (sql->Upheadimg(id, pwd, imgdata)) {
        PrintStr(id.toStdString() + " �ϴ�ͷ��ɹ�");
        QJsonDocument doc;
        QJsonObject obj;
        QJsonArray jsonarray;
        obj.insert("header", 7);
        obj.insert("return", 1);
        for (auto var:onlineUsers->values())
        {
            if (var == id) {
                auto v = onlineUsers->key(id);
                for (auto& x : *conList) {
                    if (x->socketDescriptor() == v) {
                        x->write(doc.toJson(QJsonDocument::Compact));
                    }
                }
            }
        }
    } else {
        PrintStr(id.toStdString() + " �ϴ�ͷ��ʧ��");
    }
}

void ServerSocket::incomingConnection(qintptr handle) {
    ClientSocket* tcpclient = new ClientSocket();
    tcpclient->setSocketDescriptor(handle);
    tcpclient->des = handle;
    this->conList->append(tcpclient);
    connect(tcpclient, &ClientSocket::Discon, this, &ServerSocket::Discon);
    connect(tcpclient, &ClientSocket::Login, this, &ServerSocket::Login);
    connect(tcpclient, &ClientSocket::Logon, this, &ServerSocket::Logon);
    connect(tcpclient, &ClientSocket::Logout, this, &ServerSocket::Logout);
    connect(tcpclient, &ClientSocket::Sendnews, this, &ServerSocket::Sendnews);
    connect(tcpclient, &ClientSocket::SendUser, this, &ServerSocket::SendUsers);
    connect(tcpclient, &ClientSocket::Upheadimg, this, &ServerSocket::Upheadimg);
    PrintStr(tcpclient->peerAddress().toString().toStdString() + ":" +
             QString::number(tcpclient->peerPort()).toStdString() + "|���ӷ�����");
    // TODO���������ӷ������ɹ��źŸ�Զ�̶˵�
    QJsonDocument doc;
    QJsonObject obj;
    QJsonArray jsonarray;
    obj.insert("header", 4);
    obj.insert("return", 1);
    obj.insert("from", "null");
    obj.insert("msg", "null");
    doc.setObject(obj);
    tcpclient->write(doc.toJson(QJsonDocument::Compact));
}
