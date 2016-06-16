#include <Vector/DBC.h>
#include <iostream>
#include <algorithm>
#include <jsonxx.h>
#include <sstream>
#include <type_traits>
#include <typeindex>
#include <string>
#include <emscripten.h>

using namespace std;
using namespace Vector::DBC;
using namespace jsonxx;

/**
 * Function definitions
 */

template <typename R>
R convert_enum(ByteOrder e);

template <typename R>
R convert_enum(ValueType v);

template <typename T>
string convert_key(T k) {
    return to_string(k);
}

string convert_key(string k) {
    return k;
}

Object parse(const Signal& s);
Object parse(const Node& n);

template <typename string>
string parse(string str);

template <typename Map, typename Key, typename Value>
Object parse_map(Map map);

template <typename List, typename Value>
Array parse_list(List list);

/**
 * End of definitions
 */

template <typename R>
R convert_enum(ByteOrder e) {
    typedef std::underlying_type<ByteOrder>::type utype;
    utype val = static_cast<utype>(e);
    if(val == '0') {
        return "BigEndian";
    }
    return "LittleEndian";
}

template <typename R>
R convert_enum(ValueType v) {
    typedef std::underlying_type<ValueType>::type utype;
    utype val = static_cast<utype>(v);
    if(val == '+') {
        return "Unsigned";
    } else {
        return "Signed";
    }
}

Object parse(const Node& n) {
  Object obj;
  obj << "name" << n.name;
  return obj;
}

Object parse(const Message& m) {
  Object obj;
  obj << "id" << m.id;
  obj << "name" << m.name;
  obj << "size" << m.size;
  obj << "transmitter" << m.transmitter;
  obj << "signals" << parse_map<map<string, Signal>, string, Signal>(m.signals);
  obj << "transmitters" << parse_list<set<string>, string>(m.transmitters);
  return obj;
}

Object parse(const Signal& s) {
  Object obj;
  obj << "name" << s.name;
  obj << "startBit" << s.startBit;
  obj << "signalSize" << s.bitSize;
  obj << "byteOrder" << convert_enum<string>(s.byteOrder);
  obj << "valueType" << convert_enum<string>(s.valueType);
  obj << "factor" << s.factor;
  obj << "offset" << s.offset;
  obj << "minimum" << s.minimumPhysicalValue;
  obj << "maximum" << s.maximumPhysicalValue;
  obj << "unit" << s.unit;
  obj << "receivers" << parse_list<set<string>, string>(s.receivers);
  return obj;
}

template <typename string>
string parse(string str) {
    return str;
}

template <typename Map, typename Key, typename Value>
Object parse_map(Map map) {
  Object obj;
  for_each(map.begin(), map.end(), [&](pair<Key, Value> p) {
      obj << convert_key(p.first) << parse(p.second);
  });
  return obj;
}

template <typename List, typename Value>
Array parse_list(List list) {
    Array arr;
    for_each(list.begin(), list.end(), [&](Value v) {
        arr << parse(v);
    });
    return arr;
}

string parse(Network const& network) {
    Object res;
    res << "nodes" << parse_map<map<string, Node>, string, Node>(network.nodes);
    res << "messages" << parse_map<map<unsigned int, Message>, unsigned int, Message>(network.messages);
    return res.json();
}

extern "C" {
  const char* run() {
    ostringstream ss;
    File file;
    Network network;
    auto status = file.load(network, "/test.dbc");
    if (int(status) != 0) {
      ss << "Not OK" << endl;
      return ss.str().c_str();
    }
    return parse(network).c_str();
  }
}

