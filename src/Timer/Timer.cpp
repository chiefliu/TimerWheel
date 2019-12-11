#include "Timer.h"
#include <iostream>




CF_TimerWheel::CF_TimerWheel(int time_step):timer_id_(1),work_running_(true)
{
	timer_heap_ = boost::make_shared<TimerHeap>();
	clock_ = boost::make_shared<CF_Clock>(time_step);	
}

CF_TimerWheel::~CF_TimerWheel()
{
	Stop();
}

void CF_TimerWheel::CheckExpire(time_t time)
{
	
	std::unique_lock<std::mutex> lock(mutx_);
	cout<<"check Expire time = "<<time<< endl;
	TimerEntity entity;
	while(true == timer_heap_->GetRoot(entity))
	{
		if(entity.interval_ <= time)
		{
			timer_heap_->pop(entity);

			TimerID id = entity.id_;
			TimerMap::iterator iter = timer_map_.find(id);
			if(iter != timer_map_.end())
			{
				if(iter->second.status_ != DISABLE)
				{
					std::unique_lock<std::mutex> work_lock(work_mutx_);
					woker_queue_.push(entity);
					cv_.notify_one();
				}
				timer_map_.erase(iter);
			} 
		}
		else
		{
			break;
		}
	}
}

bool CF_TimerWheel::GetTask(TimerEntity& work)
{
	std::unique_lock<std::mutex> work_lock(work_mutx_);
	while(woker_queue_.empty())
	{
		cv_.wait(work_lock);
	}
	if(woker_queue_.empty())
	{
		return false;
	}
	work = woker_queue_.front();
	woker_queue_.pop();
	return true;
}

void CF_TimerWheel::OnTimerWoker()
{
	while(work_running_)
	{
		TimerEntity entity;
		if(true == GetTask(entity))
		{
			//do work
			entity.func_(nullptr,entity.id_);
		}
	}
}

void CF_TimerWheel::StopWorker()
{
	work_running_ = false;
	work_thread_.join();
}

void CF_TimerWheel::StartWoker()
{
	work_thread_ = std::thread(std::bind(&CF_TimerWheel::OnTimerWoker,GetThis()));
}


TimerID CF_TimerWheel::GetNext()
{
	if(timer_id_ == MAX_TIMERID)
	{
		return 0;
	}
	else
	{
		return timer_id_++;
	}
}

TimerID CF_TimerWheel::AddTimer(long long interval,TimerType type,TimerFunc func)
{
	std::unique_lock<std::mutex> lock(mutx_);
	TimerID id = GetNext();
	if(id != 0)
	{
		interval = clock_->GetRoughTime()+ interval;

		cout<<"Timer ID = "<<id<<"|| execute time = "<<interval<<endl;
		TimerEntity entity(interval,type,func,id);	
		timer_heap_->InsertElement(entity);
		timer_map_.insert(make_pair(id,entity));
	}	
	return id;
}

bool CF_TimerWheel::DisableTimer(TimerID id)
{
	std::unique_lock<std::mutex> lock(mutx_);
	TimerMap::iterator iter = timer_map_.find(id);
	if(iter != timer_map_.end())
	{
		iter->second.status_ = DISABLE;
		return true;
	}
	return false;
}

void CF_TimerWheel::StartClock()
{
	ExpireFunc func = std::bind(&CF_TimerWheel::CheckExpire,GetThis(),std::placeholders::_1);
	clock_thead_ = std::thread(std::bind(&CF_Clock::DoRun,clock_->GetThis(),
		func));
}

void CF_TimerWheel::StopClock()
{
	clock_->TerminateClock();
	clock_thead_.join();
}