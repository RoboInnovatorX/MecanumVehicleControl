#ifndef READWRITEXML_H
#define READWRITEXML_H

#include <QString>


class ReadWriteXml
{
public:
    ReadWriteXml();
    // Function to create an XML string with specified root tag and tag-value pairs
    QString WriteXml(const QString& Root_tag, const QString& Tg1, int Vl1,const QString& Tg2, int Vl2, const QString& Tg3,int Vl3, const QString& Tg4, int Vl4);
    // Function to generate an XML tag string, with specified tag name and type (opening/closing)
    QString XmlCreateTag (const QString& xml_tag, bool is_start_tag);
    QString XmlCreateString(const QString& xml_tag, int xml_value);  // Function to create an XML tag containing an integer value
    QString XmlGetStr(const QString& xml_str, const QString& xml_tag); // Function to extract content from an XML tag as a QString
    int XmlGetInt(const QString& xml_str, const QString& xml_tag); // Function to extract an integer value from an XML tag's content
    float XmlGetFloat(const QString& xml_str, const QString& xml_tag); // Function to extract a floating-point value from an XML tag's content
    QVector<float> XmlGetVector(const QString& xml_str, const QString& xml_tag); // Function to extract a vector of floating-point values from an XML tag's content
};

#endif // READWRITEXML_H
