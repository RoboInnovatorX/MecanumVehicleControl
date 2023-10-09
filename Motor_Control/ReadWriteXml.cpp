#include "ReadWriteXml.h"
#include <QtXml>
#include <QTextStream>

ReadWriteXml::ReadWriteXml()
{

}

// Function to generates an XML string based on the provided input values.
QString ReadWriteXml::WriteXml(const QString& Root_tag,const QString& Tg1, int Vl1,const QString& Tg2, int Vl2, const QString& Tg3,int Vl3, const QString& Tg4, int Vl4)
{
    // Convert integer values to QStrings
    QString value1, value2, value3, value4;
    value1 = QString::number(Vl1);
    value2 = QString::number(Vl2);
    value3 = QString::number(Vl3);
    value4 = QString::number(Vl4);

    // Declare a QString to store the generated XML content
    QString myXmlString;
    // Create an XML stream writer that writes to the myXmlString variable
    QXmlStreamWriter xmlWriter(&myXmlString);

    // Set auto-formatting for the XML writer
    xmlWriter.setAutoFormatting(true);
    // Write the start element tag with the provided root tag
    xmlWriter.writeStartElement(Root_tag);
    // Write all the child element with the tag and the content
    xmlWriter.writeTextElement(Tg1,value1);
    xmlWriter.writeTextElement(Tg2,value2);
    xmlWriter.writeTextElement(Tg3,value3);
    xmlWriter.writeTextElement(Tg4,value4);

    // Close the root element tag
    xmlWriter.writeEndElement();

    // Return the generated XML content
    return myXmlString;
}

// Function that Creates a String with the Start or End Tag of an Xml Text
QString ReadWriteXml::XmlCreateTag(const QString &xml_tag, bool is_start_tag)
{

    QString xml_tag_string;

    // check if it is start tag or not and generate xml string with correct tag
    xml_tag_string = (is_start_tag ? ("<" + xml_tag + ">") : ("</" + xml_tag + ">"));

    return xml_tag_string;
}

QString ReadWriteXml::XmlCreateString(const QString &xml_tag, int xml_value)
{
    QString xml_tag_string;

    xml_tag_string = XmlCreateTag(xml_tag, true)+ QString::number(xml_value)+ XmlCreateTag(xml_tag,false);

    return xml_tag_string;
}

// Function that Extracts a String Type Variable from a Text in Xml Format
QString ReadWriteXml::XmlGetStr(const QString &xml_str, const QString &xml_tag)
{
    QString xml_tag_string;

    // Create Start and End Tag
    QString tagOpen = XmlCreateTag(xml_tag, true);
    QString tagClose = XmlCreateTag(xml_tag, false);

    // Calculate the Indices of the Tags in the Text
    int iStart = xml_str.indexOf(tagOpen);
    int iEnd = xml_str.indexOf(tagClose);

    // Calculate Index of the End of the Start Tag
    iStart += tagOpen.length();

    // Calculate the Space Occupied by the Variable
    int length_to_copy = iEnd - iStart;

    // Get the Variable
    xml_tag_string = xml_str.mid(iStart, length_to_copy);

    return xml_tag_string;
}

// Function that Extracts a Variable of Type Int from a Text in Xml Format
int ReadWriteXml::XmlGetInt(const QString &xml_str, const QString &xml_tag)
{
    int xml_tag_string;

    // Extract Content from Xml Text
    QString contents = XmlGetStr(xml_str, xml_tag);

    contents = contents.trimmed();  // Remove Spaces at the Beginning and End

    xml_tag_string = contents.toInt();         // Convert to Integer

    return xml_tag_string;
}

// Function that Extracts a Float Type Variable from a Text in Xml Format
float ReadWriteXml::XmlGetFloat(const QString &xml_str, const QString &xml_tag)
{
    float xml_tag_string;

    // Extract Content from Xml Text
    QString contents = XmlGetStr(xml_str, xml_tag);

    contents = contents.trimmed();  // Remove Spaces at the Beginning and End

    xml_tag_string = contents.toFloat();       // Conversion to Real

    return xml_tag_string;
}

// Function that Extracts a Table of Real Values from a Text in Xml Format
QVector<float> ReadWriteXml::XmlGetVector(const QString &xml_str, const QString &xml_tag)
{
    QVector<float> xml_tag_string;

    // Extract Content from Xml Text
    QString contents = XmlGetStr(xml_str, xml_tag);

    contents = contents.trimmed();  // Remove Spaces at the Beginning and End

    // If a Table is found
    if (contents.startsWith("[") && contents.endsWith("]")){

        // Remove the Brackets
        contents = contents.mid(1, contents.length() - 2);

        // Separate By Commas
        QStringList fields = contents.split(",");

        // For each Element
        for (int i = 0; i < fields.count(); i++){

            // Convert to Float and Add to Result Table
            float value = fields[i].toFloat();
            xml_tag_string.append(value);

        }
    }

    return xml_tag_string;
}
