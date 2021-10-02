#ifndef AMKGLDOMUTILS_H
#define AMKGLDOMUTILS_H

#include <Eigen/Dense>

#include <QDomDocument>
#include <QColor>
#include <QString>

/**
 * @brief Utility for dom element
 */
class AMKglDomUtils
{
public:
    static QDomElement QColorDomElement(const QColor& color, const QString& name, QDomDocument& doc);

    static void setBoolAttribute(QDomElement& element, const QString& attribute, bool value);

    static QDomElement VecDomElement(const Eigen::Vector3d& vec, const QString& name, QDomDocument& doc);

    static QDomElement QuaternionDomElement(const Eigen::Quaterniond& quat, const QString& name, QDomDocument& doc);

    static double doubleFromDom(const QDomElement& e, const QString& attribute, double defValue);

    static int intFromDom(const QDomElement& e, const QString& attribute, int defValue);

    static unsigned int uintFromDom(const QDomElement& e, const QString& attribute, unsigned int defValue);

    static bool boolFromDom(const QDomElement& e, const QString& attribute, bool defValue);

    static QColor QColorFromDom(const QDomElement& e);

    static Eigen::Vector3d VecFromDom(const QDomElement& e);

    static Eigen::Quaterniond QuaternionFromDom(const QDomElement& e);
};

#endif // AMKGLDOMUTILS_H
