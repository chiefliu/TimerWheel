#ifndef CLOCK_H__
#define CLOCK_H__
#include "boost/noncopyable.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/make_shared.hpp"

#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <chrono>

#define EPOLLEVENT_NUM 64

enum EventsType
{
	read_event = 1,
	write_event = 2
};


class CF_Trick;
class CF_Clock;
class CF_Clock: public boost::enable_shared_from_this<CF_Clock>
{
public:
	typedef std::function<void(time_t)> expire_func;
	//millisecond
	CF_Clock(int time_step);

	void DoRun(expire_func func);

	void TerminateClock();

	void DoActiveEvents(EventsType events);

	boost::shared_ptr<CF_Clock> GetThis()
	{
		return shared_from_this();
	}

	 time_t GetRoughTime() const;


private:
	CF_Clock();

	int time_step_;

	boost::shared_ptr<CF_Trick> trick_;

	bool is_running_;
};

//使用事件循环监听超时来模拟时钟的trick，这样的好处是不会使线程休眠
class CF_Trick
{
public:
	typedef std::function<void(time_t)> expire_func;
	CF_Trick(int time_step);
	int DoTrick(expire_func func);
	int GetTrickFD()
	{
		return trick_fd_;
	}

	int GetTrickepfd()
	{
		return trick_epoll_fd_;
	}

	time_t GetRoughTime()
	{
		return rough_time_;
	}

private:

	time_t getTimeStamp()
	{
		std::chrono::time_point<std::chrono::system_clock,std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());//获取当前时间点
		std::time_t timestamp =  tp.time_since_epoch().count(); //计算距离1970-1-1,00:00的时间长度
		return timestamp;
	}

	time_t rough_time_;
	int time_step_;
	int trick_fd_;
	int trick_epoll_fd_;
	struct epoll_event ev_,events_[EPOLLEVENT_NUM];
};

#endif
