#include "ferconfig.h"

FerConfig::FerConfig(QObject *parent) : QObject(parent)
{

}

bool FerConfig::loadConfig(QList<FerStep *> *steps)
{
    QFile file("fer_para_conf_1");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Read configuration file failed!";
        return false;
    }

    int stepNum = 0;
    while(!file.atEnd())
    {
        QString rawStr = QString(file.readLine());
        QStringList contentStrList = rawStr.split("\n", QString::SplitBehavior::SkipEmptyParts);

        QString endStr = "";
        if(contentStrList.length() > 0)
        {
            endStr = contentStrList.at(0).toLocal8Bit().constData();
            if(endStr == "<end>")
            {
                break;
            }
        }
        else
        {
            continue;
        }

        QStringList fieldsStrList = contentStrList.at(0).split(" ", QString::SplitBehavior::SkipEmptyParts);
        if(fieldsStrList.length() > 0)
        {
            QString value = fieldsStrList.at(0);
            if(value == "<step_" + QString::number(stepNum + 1) + ">")
            {
                FerStep* ferStep = new FerStep();
                QList<FerCase *> ferCases;

                int caseNum = 0;
                while(!file.atEnd())
                {
                    rawStr = "";
                    contentStrList.clear();
                    fieldsStrList.clear();
                    rawStr = QString(file.readLine());
                    contentStrList = rawStr.split("\n", QString::SplitBehavior::SkipEmptyParts);

                    endStr = "";
                    if(contentStrList.length() > 0)
                    {
                        endStr = contentStrList.at(0).toLocal8Bit().constData();
                        if(endStr == "<end>")
                        {
                            ferStep->setFerCases(ferCases);
                            break;
                        }
                    }
                    else
                    {
                        ferStep->setFerCases(ferCases);
                        break;
                    }

                    fieldsStrList = contentStrList.at(0).split(" ", QString::SplitBehavior::SkipEmptyParts);
                    if(fieldsStrList.length() > 0)
                    {
                        value = fieldsStrList.at(0).toLocal8Bit().constData();
                        if(value == "plan_numb")
                        {
                            ferStep->setPlanNum(QByteArray(fieldsStrList.at(1).toLocal8Bit().constData()).toInt());
                        }
                        else if(value == "air_mode")
                        {
                            ferStep->setAirMode(QByteArray(fieldsStrList.at(1).toLocal8Bit().constData()).toInt());
                        }
                        else if(value == "next_step_time_max")
                        {
                            ferStep->setNextStepTimeMax(QByteArray(fieldsStrList.at(1).toLocal8Bit().constData()).toInt());
                        }
                        else if(value == "next_step_time_min")
                        {
                            ferStep->setNextStepTimeMin(QByteArray(fieldsStrList.at(1).toLocal8Bit().constData()).toInt());
                        }
                        else if(value == "next_step_temp")
                        {
                            ferStep->setNextStepTemp(QByteArray(fieldsStrList.at(1).toLocal8Bit().constData()).toFloat());
                        }
                        else if(value == "hope_temp")
                        {
                            ferStep->setHopeTemp(QByteArray(fieldsStrList.at(1).toLocal8Bit().constData()).toFloat());
                        }
                        else if(value == "default_para")
                        {
                            ferStep->setDefaultParaAE(QByteArray(fieldsStrList.at(1).toLocal8Bit().constData()).toInt());
                            ferStep->setDefaultParaSTA(QByteArray(fieldsStrList.at(2).toLocal8Bit().constData()).toInt());
                        }
                        else if(value == "case_" + QString::number(caseNum+1))
                        {
                            FerCase* ferCase = new FerCase;
                            ferCase->setLowTempture(QByteArray(fieldsStrList.at(1).toLocal8Bit().constData()).toFloat());
                            ferCase->setHighTempture(QByteArray(fieldsStrList.at(5).toLocal8Bit().constData()).toFloat());
                            ferCase->setAe(QByteArray(fieldsStrList.at(6).toLocal8Bit().constData()).toFloat());
                            ferCase->setSta(QByteArray(fieldsStrList.at(7).toLocal8Bit().constData()).toFloat());
                            ferCases.append(ferCase);
                            caseNum++;
                        }
                    }
                }

                steps->append(ferStep);
                stepNum++;
            }
        }
    }

    return true;
}

QList<FerStep *> FerConfig::getFerSteps() const
{
    return ferSteps;
}

void FerConfig::setFerSteps(const QList<FerStep *> &value)
{
    ferSteps = value;
}
