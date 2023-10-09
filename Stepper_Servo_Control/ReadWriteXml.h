#ifndef READ_WRITE_XML_H
#define READ_WRITE_XML_H

#include <Arduino.h>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>


class ReadWriteXml {
public:
  bool containsSpecificString(const String& recvString, const String& checkString);
  String XmlCreateTag(const String& xml_tag, bool is_start_tag);
  String XmlGetStr(const String& xml_str, const String& xml_tag);
  int XmlGetInt(const String& xml_str, const String& xml_tag);
  float XmlGetFloat(const String& xml_str, const String& xml_tag);
  std::vector<float> XmlGetVector(const String& xml_str, const String& xml_tag);
};

#endif // READ_WRITE_XML_H