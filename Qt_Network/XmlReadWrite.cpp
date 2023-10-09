#include "XmlReadWrite.h"


// Function that Creates a String with the Start or End Tag of an Xml Text
QString XmlReadWrite::XmlCreateTag(const QString &xml_tag, bool is_start_tag)
{

    QString ret;

    if (is_start_tag)
        ret = "<" + xml_tag + ">";      // Start tag
    else
        ret = "</" + xml_tag + ">";     // End tag

    return ret;
}

// Function that Extracts a String Type Variable from a Text in Xml Format
QString XmlReadWrite::XmlGetStr(const QString &xml_str, const QString &xml_tag)
{
    QString ret;

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
    ret = xml_str.mid(iStart, length_to_copy);

    return ret;

}

// Function that Extracts a Variable of Type Int from a Text in Xml Format
int XmlReadWrite::XmlGetInt(const QString &xml_str, const QString &xml_tag)
{

    int ret;

    // Extract Content from Xml Text
    QString contents = XmlGetStr(xml_str, xml_tag);

    contents = contents.trimmed();  // Remove Spaces at the Beginning and End

    ret = contents.toInt();         // Convert to Integer

    return ret;
}

// Function that Extracts a Float Type Variable from a Text in Xml Format
float XmlReadWrite::XmlGetFloat(const QString &xml_str, const QString &xml_tag)
{
    float ret;

    // Extract Content from Xml Text
    QString contents = XmlGetStr(xml_str, xml_tag);

    contents = contents.trimmed();  // Remove Spaces at the Beginning and End

    ret = contents.toFloat();       // Conversion to Real

    return ret;
}

// Function that Extracts a Table of Real Values from a Text in Xml Format
QVector<float> XmlReadWrite::XmlGetVector(const QString &xml_str, const QString &xml_tag)
{
    QVector<float> ret;

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
            ret.append(value);

        }
    }

    return ret;
}

