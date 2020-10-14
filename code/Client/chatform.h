#ifndef CHATFORM_H
#define CHATFORM_H

#include <QListWidgetItem>
#include <QMovie>
#include <QWidget>

#include "qnchatmessage.h"
namespace Ui {
    class ChatForm;
}

class ChatForm : public QWidget {
    Q_OBJECT

  public:
    void dealMessage(QNChatMessage* messageW, QListWidgetItem* item, QString text, QString time,
                     QNChatMessage::User_Type type);
    void dealMessageTime(QString curMsgTime);
    void MessageOfMe(QString);//�Լ�����Ϣ
    void MessgaeOfShe(QString);//�Է���Ϣ
    void Sendstatue(bool);//����״̬����ʱ���ò���
    ChatForm(QString);
    ~ChatForm();
    QString Widget_ID;
  signals:
    void Send(QString, QString);
    void Close(QString);
  public slots:
    void Rece(QString);

  private slots:
    void on_sendBtn_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

  private:
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    bool IsPress = false;
    QMovie* mouve;
    QPoint mouse;
    Ui::ChatForm* ui;
};

#endif  // CHATFORM_H
