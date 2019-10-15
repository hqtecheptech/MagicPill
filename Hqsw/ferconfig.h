#ifndef FERCONFIG_H
#define FERCONFIG_H

#include <QObject>
#include <QList>
#include <QFile>
#include <QIODevice>
#include <QDebug>
#include <ferstep.h>

class FerConfig : public QObject
{
    Q_OBJECT
public:
    explicit FerConfig(QObject *parent = 0);
    bool loadConfig(QList<FerStep *> *steps);

    QList<FerStep *> getFerSteps() const;
    void setFerSteps(const QList<FerStep *> &value);

signals:

public slots:

private:
    QList<FerStep *> ferSteps;
};

#endif // FERCONFIG_H
