#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include <QObject>

class Credentials
{
public:
    Credentials(QString usern, QString passw)
    {
        username = usern;
        password = passw;
    }

    QString GetUsername() const
    {
        return username;
    }

    QString GetPassword() const
    {
        return password;
    }
private:
    QString username;
    QString password;
};
#endif // CREDENTIALS_H
