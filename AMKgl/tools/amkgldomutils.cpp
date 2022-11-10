#include "amkgldomutils.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <QDebug>

QDomElement AMKglDomUtils::QColorDomElement(const QColor& color, const QString& name, QDomDocument& doc)
{
    QDomElement de = doc.createElement(name);
    de.setAttribute("red", QString::number(color.red()));
    de.setAttribute("green", QString::number(color.green()));
    de.setAttribute("blue", QString::number(color.blue()));

    return de;
}

void AMKglDomUtils::setBoolAttribute(QDomElement& element, const QString& attribute, bool value)
{
    element.setAttribute(attribute, (value ? "true" : "false"));
}

QDomElement AMKglDomUtils::VecDomElement(const Eigen::Vector3d& vec, const QString& name, QDomDocument& doc)
{
    QDomElement de = doc.createElement(name);
    de.setAttribute("x", QString::number(vec.x()));
    de.setAttribute("y", QString::number(vec.y()));
    de.setAttribute("z", QString::number(vec.z()));

    return de;
}

QDomElement AMKglDomUtils::QuaternionDomElement(const Eigen::Quaterniond& quat, const QString& name, QDomDocument& doc)
{
    QDomElement de = doc.createElement(name);
    de.setAttribute("q0", QString::number(quat.x()));
    de.setAttribute("q1", QString::number(quat.y()));
    de.setAttribute("q2", QString::number(quat.z()));
    de.setAttribute("q3", QString::number(quat.w()));

    return de;
}

double AMKglDomUtils::doubleFromDom(const QDomElement& e, const QString& attribute, double defValue)
{
    double value = defValue;
    if (e.hasAttribute(attribute)) {
        const QString s = e.attribute(attribute);
        bool ok;
        value = s.toDouble(&ok);
        if (!ok) {
            qWarning() << QString("'%1' is not a valid double syntax for attribute \"%2\" in initialization of \"%3\". Setting value to %4.")
                    .arg(s).arg(attribute).arg(e.tagName()).arg(QString::number(defValue)).toUtf8();
            value = defValue;
        }
    } else {
        qWarning() << QString("\"%1\" attribute missing in initialization of \"%2\". Setting value to %3.")
                .arg(attribute).arg(e.tagName()).arg(QString::number(value)).toUtf8();
    }

#if defined(isnan)
    // The "isnan" method may not be available on all platforms.
    // Find its equivalent or simply remove these two lines
    if (isnan(value))
        qWarning() << QString("Warning, attribute \"%1\" initialized to Not a Number in \"%2\"")
                .arg(attribute).arg(e.tagName()).toUtf8();
#endif

    return value;
}

int AMKglDomUtils::intFromDom(const QDomElement& e, const QString& attribute, int defValue)
{
    int value = defValue;
    if (e.hasAttribute(attribute))
    {
        const QString s = e.attribute(attribute);
        bool ok;
        value = s.toInt(&ok);
        if (!ok) {
            qWarning() << QString("'%1' is not a valid integer syntax for attribute \"%2\" in initialization of \"%3\". Setting value to %4.")
                    .arg(s).arg(attribute).arg(e.tagName()).arg(QString::number(defValue)).toUtf8();
            value = defValue;
        }
    } else {
        qWarning() << QString("\"%1\" attribute missing in initialization of \"%2\". Setting value to %3.")
                .arg(attribute).arg(e.tagName()).arg(QString::number(value)).toUtf8();
    }

    return value;
}

unsigned int AMKglDomUtils::uintFromDom(const QDomElement& e, const QString& attribute, unsigned int defValue)
{
    unsigned int value = defValue;
    if (e.hasAttribute(attribute))
    {
        const QString s = e.attribute(attribute);
        bool ok;
        value = s.toUInt(&ok);
        if (!ok) {
            qWarning() << QString("'%1' is not a valid unsigned integer syntax for attribute \"%2\" in initialization of \"%3\". Setting value to %4.")
                    .arg(s).arg(attribute).arg(e.tagName()).arg(QString::number(defValue)).toUtf8();
            value = defValue;
        }
    } else {
        qWarning() << QString("\"%1\" attribute missing in initialization of \"%2\". Setting value to %3.")
                .arg(attribute).arg(e.tagName()).arg(QString::number(value)).toUtf8();
    }

    return value;
}

bool AMKglDomUtils::boolFromDom(const QDomElement& e, const QString& attribute, bool defValue)
{
    bool value = defValue;
    if (e.hasAttribute(attribute))
    {
        const QString s = e.attribute(attribute);
        if (s.toLower() == QString("true"))
            value = true;
        else if (s.toLower() == QString("false"))
            value = false;
        else
        {
            qWarning() << QString("'%1' is not a valid boolean syntax for attribute \"%2\" in initialization of \"%3\". Setting value to %4.")
                    .arg(s).arg(attribute).arg(e.tagName()).arg(defValue?"true":"false").toUtf8();
        }
    } else {
        qWarning() << QString("\"%1\" attribute missing in initialization of \"%2\". Setting value to %3.")
                .arg(attribute).arg(e.tagName()).arg(value?"true":"false").toUtf8();
    }

    return value;
}

QColor AMKglDomUtils::QColorFromDom(const QDomElement& e)
{
    int color[3];
    QStringList attribute;
    attribute << "red" << "green" << "blue";
    for (int i=0; i<attribute.count(); ++i)
        color[i] = intFromDom(e, attribute[i], 0);
    return QColor(color[0], color[1], color[2]);
}

Eigen::Vector3d AMKglDomUtils::VecFromDom(const QDomElement& e)
{
    double values[3];
    QStringList attribute;
    attribute << "x" << "y" << "z";
    for (int i=0; i<attribute.count(); ++i)
        values[i] = doubleFromDom(e, attribute[i], 0);
    return Eigen::Vector3d(values[0], values[1], values[2]);
}

Eigen::Quaterniond AMKglDomUtils::QuaternionFromDom(const QDomElement& e)
{
    double values[4];
    QStringList attribute;
    attribute << "q0" << "q1" << "q2" << "q3";
    for (int i=0; i<attribute.count(); ++i)
        values[i] = doubleFromDom(e, attribute[i], 0);
    return Eigen::Quaterniond(values[3], values[0], values[1], values[2]);
}
