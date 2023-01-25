#pragma once
#include "Job.h"
#include "LockQueue.h"

/*------------------
	JobQueue
------------------*/

class JobQueue : public enable_shared_from_this<JobQueue>
{
public:
	void DoAsync(CallbackType&& callback)
	{
		Push(ObjectPool<Job>::MakeShared(std::move(callback)));
	}

	template<typename T, typename Ret, typename... Args>
	void DoAsync(Ret(T::*memFunc)(Args...), Args... args)
	{
		shared_ptr<T> owner = static_pointer_cast<T>(shared_from_this());
		Push(ObjectPool<Job>::MakeShared(owner, memFunc, std::forward<Args>(args)...));
	}

	void				ClearJob() { _jobs.Clear(); }

private:
	void				Push(JobRef&& job);
	void				Execute();

protected:
	LockQueue<JobRef>	_jobs;
	Atomic<int32>		_jobCount = 0;
};

