#include "pch.h"
#include "AccountManager.h"
#include "UserManager.h"

void AccountManager::ProcessLogin()
{
	// accountLock
	std::lock_guard<std::mutex> guard(_mutex);

	// userLock
	User* user = UserManager::Instance()->GetUser(100);
}