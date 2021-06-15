#include "user.h"

User::User()
{

}

User::User(QString _name, int _privilege, int _role)
{
    username = _name;
    privilege = _privilege;
    role = _role;
}

QString User::getUsername() const
{
    return username;
}

void User::setUsername(const QString &value)
{
    username = value;
}

int User::getPrivilege() const
{
    return privilege;
}

void User::setPrivilege(int value)
{
    privilege = value;
}

int User::getRole() const
{
    return role;
}

void User::setRole(int value)
{
    role = value;
}
