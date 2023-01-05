#pragma once
#include <mutex>

class User
{

};

class UserManager
{
public:
	static UserManager* Instance()
	{
		static UserManager instance;
		return &instance;
	}

	User* GetUser(int32 id)
	{
		std::lock_guard<std::mutex> guard(_mutex);
		// ¹º°¡ °®°í ¿È
		return nullptr;
	}

	void ProcessSave();
private:
	std::mutex _mutex;
};