#ifndef TIMER_H__
#define TIMER_H__

#include <thread>
#include <queue>
#include <map>
#include <string>
#include <functional>
#include <mutex>
#include <condition_variable>
#include "CFHeap.hpp"
#include "clock.h"



using namespace std;

#define MAX_TIMERID 9223372036854775807

enum TimerType
{
	Once =  1,
	Circle = 2
};

enum TimerStatus
{
	WAIT = 1,
	DISABLE = 2
};
typedef std::function<int(char*,long)>  TimerFunc;
typedef long long TimerID;
class TimerEntity
{
public:
	TimerType type_;
	TimerFunc func_;
	TimerID   id_;
	TimerStatus status_;
	long  long  interval_;
	long  long  shift_time_;

	TimerEntity(long long interval,TimerType type,TimerFunc func,TimerID id)
	{
		type_ = type;
		interval_ = interval;
		func_ = func;
		id_ = id;
		status_ = WAIT;
	}
	TimerEntity()
	{

	}
	static bool TimerSmaller(TimerEntity t1,TimerEntity t2)
	{
		return t1.interval_ < t2.interval_ ? true : false;
	}
};



class CF_TimerWheel;
class CF_TimerWheel :public boost::enable_shared_from_this<CF_TimerWheel>
{
public:
	typedef queue<TimerEntity>   WorkQueue;
	typedef CFMinHeap<TimerEntity,TimerEntity::TimerSmaller> TimerHeap;
	typedef map<TimerID,TimerEntity>       TimerMap;
	typedef std::function<void(time_t)> ExpireFunc;

	 CF_TimerWheel(int time_step);
	virtual ~CF_TimerWheel();

	TimerID AddTimer(long long interval,TimerType type,TimerFunc func);
	bool DisableTimer(TimerID id);

	void Run()
	{
		StartWoker();
		StartClock();
	}

	void Stop()
	{
		StopClock();
		StopWorker();
	}

	boost::shared_ptr<CF_TimerWheel> GetThis()
	{
		return shared_from_this();
	}

	bool GetTask(TimerEntity& work);

	void OnTimerWoker();



private:

	void AddTimerNoLock(long long interval,TimerType type,TimerFunc func,TimerID entity_id);

	void StartClock();
	void StopClock();
	
	void StartWoker();
	void StopWorker();

	void CheckExpire(time_t time);

	TimerID GetNext();

	mutex mutx_;
	mutex work_mutx_;
	condition_variable cv_;
	boost::shared_ptr<TimerHeap>  timer_heap_;
	boost::shared_ptr<CF_Clock> clock_;
	TimerMap timer_map_;
	TimerID timer_id_;
	WorkQueue woker_queue_;


	bool work_running_;

	std::thread work_thread_,clock_thead_;



};

#endif
