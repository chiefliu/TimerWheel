
#include "Timer.h"
#include <string>
#include <map>
#include "boost/ref.hpp"
#include <iostream>

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
int main()
{

	boost::shared_ptr<CF_TimerWheel> wheel = boost::make_shared<CF_TimerWheel>(1000);
	wheel->Run();

	for(int i = 1; i < 100; i++)
	{

	}

	std::thread t1([&]()
	{
		wheel->AddTimer(15000,Once,&getinfo);
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
	return 0;
}
