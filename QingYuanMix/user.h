#ifndef USER_H
#define USER_H

#include <QString>
#include <QObject>

class User :  public QObject
{
    Q_OBJECT

public:
    User();
    User(QString _name, int _privilege, int _role);

    QString getUsername() const;
    void setUsername(const QString &value);

    int getPrivilege() const;
    void setPrivilege(int value);

    int getRole() const;
    void setRole(int value);

private:
    QString username;
    int privilege;
    int role;
};

#endif // USER_H
