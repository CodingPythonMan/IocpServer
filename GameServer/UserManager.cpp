#include "pch.h"
#include "AccountManager.h"
#include "UserManager.h"

void UserManager::ProcessSave()
{
	// accountLock
	Account* account = AccountManager::Instance()->GetAccount(100);

	// userLock
	std::lock_guard<std::mutex> guard(_mutex);
	
}