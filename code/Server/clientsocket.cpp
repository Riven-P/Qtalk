#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#    pragma execution_character_set("utf-8")
#endif
#include "clientsocket.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <iostream>
ClientSocket::ClientSocket() {
    des = NULL;
    connect(this, &ClientSocket::readyRead, this, &ClientSocket::DataRece);
    connect(this, &ClientSocket::disconnected, this, &ClientSocket::DiscCon);
}
/**
 * @brief ���յ�Json��ʽ����Ϣ������
 * 
 */
void ClientSocket::DataRece() {
    while (bytesAvailable() > 0) {
        int len = bytesAvailable();
        QByteArray data;
        data.resize(len);
        read(data.data(), data.size());
        //        QString msg = QString::fromLocal8Bit(data.data());
        //    std::cout << msg.toStdString() << std::endl;
        QJsonDocument doc;
        //        qDebug() << data;
        doc = QJsonDocument::fromJson(data);
        //        qDebug() << doc.toJson(QJsonDocument::JsonFormat::Compact);
        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            QJsonValue value = obj.value("header");
            if (value.toInt() == 0) {  //����
                emit Logout(des, obj.value("id").toString());
            }
            if (value.toInt() == 1) {  //����
                                       //                qDebug() << "1";
                emit this->Login(des, obj.value("id").toString(), obj.value("pwd").toString());
            }
            if (value.toInt() == 2) {  //ע��
                emit this->Logon(des, obj.value("id").toString(), obj.value("pwd").toString());
            }
            if (value.toInt() == 3) {  //������Ϣ
                emit Sendnews(des, obj.value("sendto").toString(), obj.value("msg").toString());
                qDebug() << des << "���͸�" << obj.value("sendto").toString() <<"��Ϣ��" << obj.value("msg").toString()
                    << Qt::endl;
            }
            if (value.toInt() == 4) {  //���������б�
                emit SendUser(des);
            }
            if (value.toInt() == 5) {  //�ϴ�ͷ��
                QJsonObject obj = doc.object();
                QJsonValue value = obj.value("msg");
                emit Upheadimg(obj.value("id").toString(), obj.value("pwd").toString(), value.toString());
            }
        }
    }
    return;
}
/**
 * @brief ĳ��socket�Ͽ�,�ر�ǰ������������������
 * 
 */
void ClientSocket::DiscCon() {
    emit this->Discon(des);
    this->des = 0;
}
