
#include "Timer.h"
#include <string>
#include <map>
#include "boost/ref.hpp"
#include <iostream>
#include "boost/dynamic_bitset.hpp"
#include "boost/utility/binary.hpp"
#include "boost/bimap.hpp"
#include "boost/typeof/typeof.hpp"

using namespace std;


struct ent
{
	int status;
	string name ;
};

int getinfo(char* ch,long size)
{
	cout<<"Timer ID = "<<size<<"||hello clock !"<<endl;
}
void add(boost::shared_ptr<CF_TimerWheel> wheel,long time)
{
	wheel->AddTimer(time,Once,&getinfo);
}

void RunTimeWheel()
{
	boost::shared_ptr<CF_TimerWheel> wheel = boost::make_shared<CF_TimerWheel>(1000);
	wheel->Run();
	std::thread t1([&]()
	{
		wheel->AddTimer(5000,Circle,&getinfo);
	});
	std::thread t2([&]()
	{
		wheel->AddTimer(30000,Once,&getinfo);
	});
	std::thread t3([&]()
	{
		wheel->AddTimer(45000,Once,&getinfo);
	});

	while(1)
	{

	}
}

int main()
{
	boost::bimap<int,string> bmp;

	bmp.left.insert(make_pair(1,"abc"));

	BOOST_AUTO(pos,bmp.right.find("abc"));
	if(pos != bmp.right.end())
	{
		cout<<pos->second<<endl;
	}
	//int index = bmp.right["abc"];

	




	return 0;
}
