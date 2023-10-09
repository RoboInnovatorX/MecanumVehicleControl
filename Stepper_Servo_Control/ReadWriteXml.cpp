#include "ReadWriteXml.h"

// Function that Creates a String with the Start or End Tag of an Xml Text
String ReadWriteXml::XmlCreateTag(const String& xml_tag, bool is_start_tag)
{
    String ret;

    if (is_start_tag)
        ret = "<" + xml_tag + ">";      // Start tag
    else
        ret = "</" + xml_tag + ">";     // End tag

    return ret;
}

// Functions to check if the string contain specific string
bool ReadWriteXml::containsSpecificString(const String& recvString, const String& checkString)
{
    // Check if the substring is present in recvString
    int foundPos = recvString.indexOf(checkString);
    return foundPos != -1;
}

// Function to trim leading and trailing spaces from a string
String trim(const String& str)
{
    int first = 0;
    int last = str.length() - 1;

    // Find the first non-whitespace character from the start
    while (first <= last && isspace(str.charAt(first))) {
        first++;
    }

    // Find the first non-whitespace character from the end
    while (last >= first && isspace(str.charAt(last))) {
        last--;
    }

    return str.substring(first, last + 1);
}

// Function that Extracts a String Type Variable from a Text in Xml Format
String ReadWriteXml::XmlGetStr(const String& xml_str, const String& xml_tag)
{
    String ret;

    // Create Start and End Tag
    String tagOpen = XmlCreateTag(xml_tag, true);
    String tagClose = XmlCreateTag(xml_tag, false);

    // Calculate the Indices of the Tags in the Text
    int iStart = xml_str.indexOf(tagOpen);
    int iEnd = xml_str.indexOf(tagClose);

    // Calculate Index of the End of the Start Tag
    iStart += tagOpen.length();

    // Calculate the Space Occupied by the Variable
    int length_to_copy = iEnd - iStart;

    // Get the Variable
    ret = xml_str.substring(iStart, iStart + length_to_copy);

    return trim(ret);
}

// Function that Extracts a Variable of Type Int from a Text in Xml Format
int ReadWriteXml::XmlGetInt(const String& xml_str, const String& xml_tag)
{
    int ret = 0;

    // Extract Content from Xml Text
    String contents = XmlGetStr(xml_str, xml_tag);

    // Convert to Integer
    ret = contents.toInt();

    return ret;
}

// Function that Extracts a Float Type Variable from a Text in Xml Format
float ReadWriteXml::XmlGetFloat(const String& xml_str, const String& xml_tag)
{
    float ret = 0.0f;

    // Extract Content from Xml Text
    String contents = XmlGetStr(xml_str, xml_tag);

    // Convert to Float
    ret = contents.toFloat();

    return ret;
}

// Function that Extracts a Table of Real Values from a Text in Xml Format
std::vector<float> ReadWriteXml::XmlGetVector(const String& xml_str, const String& xml_tag)
{
    std::vector<float> ret;

    // Extract Content from Xml Text
    String contents = XmlGetStr(xml_str, xml_tag);

    // If a Table is found
    if (contents.startsWith("[") && contents.endsWith("]")) {
        // Remove the Brackets
        contents = contents.substring(1, contents.length() - 1);

        // Separate By Commas
        int commaIndex;
        do {
            commaIndex = contents.indexOf(',');
            String token = contents.substring(0, commaIndex);
            contents = contents.substring(commaIndex + 1);
            // Convert to Float and Add to Result Table
            ret.push_back(token.toFloat());
        } while (commaIndex != -1);
    }

    return ret;
}