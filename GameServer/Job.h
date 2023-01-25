#pragma once

// C++ 11 apply

template<int... Remains>
struct seq
{};

template<int N, int... Remains>
struct gen_seq : gen_seq<N-1, N-1, Remains...>
{};

template<int... Remains>
struct gen_seq<0, Remains...> : seq<Remains...>
{};

template<typename Ret, typename... Args>
void xapply(Ref(*func)(Args...), std::tuple<Args...>& tup)
{
	return xapply_helper(func, gen_seq<sizeof...(Args)>(), tup);
}

template<typename F, typename... Args, int... ls>
void xapply_helper(F func, seq<ls...>, std::tuple<Args...>& tup)
{
	(func)(std::get<ls>(tup)...);
}

// ÇÔ¼öÀÚ (Functor)

template<typename Ret, typename... Args>
class FuncJob
{
	using FuncType = Ret(*)(Args...);
	
public:
	FuncJob(FuncType func, Args... args) : _func(func), _tuple(args...)
	{

	}

	Ret Execute()
	{
		//std::apply(_func, _tuple); // C++ 17
		xapply(_func, _tuple);
	}

private:
	FuncType _func;
	//Args... _args;
	std::tuple<Args...> _tuple;
};

//////////////////////////////////////////////////

class IJob
{
public:
	virtual void Execute() {}
};

using JobRef = shared_ptr<IJob>;

class JobQueue
{
public:
	void Push(JobRef job)
	{
		WRITE_LOCK;
		_jobs.push(job);
	}

	JobRef Pop()
	{
		WRITE_LOCK;
		if (_jobs.empty())
			return nullptr;

		JobRef ret = _jobs.front();
		_jobs.pop();
		return ret;
	}

private:
	USE_LOCK;
	queue<JobRef> _jobs;
};