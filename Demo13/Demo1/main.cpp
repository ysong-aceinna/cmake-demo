// ref: 
#include <fstream>
#include <iostream>
#include <json/json.h>
#include <cassert>
#include <errno.h>
#include <string.h>

using namespace std;
/*
    注意test1.json 和 test2.json的格式不同，
    test1.json是以 [] 作为起止字符，而test2.json是以 {} 为起止字符。
*/
int read_json1()
{
    ifstream ifs;
    ifs.open("../test1.json");
    assert(ifs.is_open());

    Json::Reader reader;
    Json::Value root;

    if (!reader.parse(ifs, root, false))
    {
        cout << "reader parse error: " << strerror(errno) << endl;
        return -1;
    }

    string name;
    int age;
    int size;

    size = root.size();
    cout << "total " << size << " elements" << endl;
    for (int i = 0; i < size; ++i)
    {
        name = root[i]["name"].asString();
        age = root[i]["age"].asInt();

        cout << "name: " << name << ", age: " << age << endl;
    }
}

int read_json2() {
  Json::Value root;
  std::ifstream ifs;
    ifs.open("../test2.json");

  Json::CharReaderBuilder builder;
  builder["collectComments"] = true;
  JSONCPP_STRING errs;
  if (!parseFromStream(builder, ifs, &root, &errs)) {
    std::cout << errs << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << root << std::endl;
  return EXIT_SUCCESS;
}

void write_json(string f)
{
    Json::Value root;
    Json::FastWriter writer;
    Json::Value person;
    
    person["age"] = 28;
    person["name"] = "HH";
    root.append(person);

    string json_file = writer.write(root);

    ofstream ofs;
    ofs.open(f);
    assert(ofs.is_open());
    ofs << json_file;

    return;
}

int main(void)
{
    cout << "#####################" << endl;
    read_json1();
    cout << "#####################" << endl;
    read_json2();
    write_json("test3.json");
    return 0;
}
