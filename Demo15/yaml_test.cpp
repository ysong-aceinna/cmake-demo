
// Ref: https://blog.csdn.net/briblue/article/details/89515470
// https://latelee.blog.csdn.net/article/details/111994720

#include <iostream>
#include "include/yaml-cpp/yaml.h"
#include <fstream>

using namespace std;

int main(int argc,char** argv)
{
    YAML::Node config = YAML::LoadFile("/Users/songyang/project/code/github/cmake-demo/Demo15/config.yaml");

    //YAML::Node的类型有4中，参考NodeType定义。
    cout << "Node type " << config.Type() << endl;
    cout << "skills type " << config["skills"].Type() << endl;

    cout << "name:" << config["name"].as<string>() << endl;
    cout << "sex:" << config["sex"].as<string>() << endl;
    cout << "age:" << config["age"].as<int>() << endl;

    cout << "skills c++:" << config["skills"]["c++"].as<int>() << endl;
    cout << "skills java:" << config["skills"]["java"].as<int>() << endl;
    cout << "skills android:" << config["skills"]["android"].as<int>() << endl;
    cout << "skills python:" << config["skills"]["python"].as<int>() << endl;

    for(YAML::const_iterator it= config["skills"].begin(); it != config["skills"].end();++it)
    {
        cout << it->first.as<string>() << ":" << it->second.as<int>() << endl;
    }

    YAML::Node test1 = YAML::Load("[1,2,3,4]");
    cout << " Type: " << test1.Type() << endl;

    YAML::Node test2 = YAML::Load("1");
    cout << " Type: " << test2.Type() << endl;

    YAML::Node test3 = YAML::Load("{'id':1,'degree':'senior'}");
    cout << " Type: " << test3.Type() << endl;

    ofstream fout("testconfig.xml");

    config["score"] = 99;

    fout << config; // 保存yaml文件。

    fout.close();


    return 0;
}
