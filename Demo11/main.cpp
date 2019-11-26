#include<iostream>
using namespace std;

class Test
{
private:
  /* data */
public:
  Test
(/* args */);
  ~Test
();
};

Test::Test(/* args */)
{
  std::cout << "Test::Test" << endl;

}

Test::~Test()
{
}


int main(int argc, char** argv) {

  Test* t = new Test;
  return 0;
}
