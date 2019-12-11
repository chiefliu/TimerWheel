#include "clock.h"
#include <netinet/tcp.h>
#include <iostream>
#include<arpa/inet.h>

using namespace std;

#define EPOLLCNT_PLACEHOLDER 5

CF_Clock::CF_Clock(int time_step):is_running_(false)
{
	if(time_step > 0)
	{
		time_step_ = time_step;
	}
	else
	{
		time_step_  = 10;
	}
	trick_ = boost::make_shared<CF_Trick>(time_step_);
}

time_t CF_Clock::GetRoughTime() const
{
	return trick_->GetRoughTime();
}

void CF_Clock::DoActiveEvents(EventsType evs)
{
	int fd = trick_->GetTrickFD();
	int epoll_fd = trick_->GetTrickepfd();
	struct epoll_event eves;
	switch (evs)
	{
	case read_event:
		eves.data.fd = fd;
		eves.events = EPOLLIN|EPOLLET;
		epoll_ctl(epoll_fd,EPOLL_CTL_MOD,fd,&eves);
		break;
	case write_event:
		eves.data.fd = fd;
		eves.events = EPOLLOUT|EPOLLET;
		epoll_ctl(epoll_fd,EPOLL_CTL_MOD,fd,&eves);
		break;
	default:
		break;
	}
}

void CF_Clock::DoRun(expire_func func)
{
	is_running_ = true;
	while(is_running_)
	{
		trick_->DoTrick(func);
	}
}

void CF_Clock::TerminateClock()
{
	is_running_ = false;
}

CF_Trick::CF_Trick(int time_step ) : time_step_(time_step),rough_time_(getTimeStamp())
{
	trick_epoll_fd_=epoll_create(EPOLLCNT_PLACEHOLDER);
	trick_fd_ = socket(AF_INET, SOCK_STREAM, 0);
	ev_.data.fd=trick_fd_;
	ev_.events=EPOLLIN|EPOLLET|EPOLLOUT;
	epoll_ctl(trick_epoll_fd_,EPOLL_CTL_ADD,trick_fd_,&ev_);
}


int CF_Trick::DoTrick(expire_func func)
{
	int num = epoll_wait(trick_epoll_fd_,events_,EPOLLEVENT_NUM,time_step_);
	
	if(num > 0)
	{
		rough_time_ = getTimeStamp();
		for(int i = 0; i < num; i++)
		{
			unsigned int   recv_evet = events_[i].events;
			int            recv_fd   = events_[i].data.fd;

			//一个不合法的套接字接入到epoll中后如果有事件触发会一直报出EPOLLHUP
			//可以利用这个机制在clock等待过程中向clock发送一个事件进行一些交互
			if(recv_evet == EPOLLHUP)
			{

			}
		}		
	}
	else
	{
		rough_time_ += time_step_;
	}
	func(rough_time_);
	return num;
}

