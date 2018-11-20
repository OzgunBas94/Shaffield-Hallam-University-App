//C++ Boot Camp - Task 2 - 2018-19 
//Name: 
//Student number: 
#pragma once

#include <string>
#include <list>
#include <iostream>
#include "Game.h"

class DatabaseManager;

using namespace std;
//--
// UserTypeId represents an identifier for the specific user type.
//--
enum class UserTypeId
{
	kInvalid = 0
	, kPlayerUser
	, kAdminUser
};

//--
// UserBase represents a user base class for the system.
//--
class UserBase
{
public:
	using Username = std::string;

	UserBase(const Username& username, const std::string& password, const std::string& email);
	
	virtual ~UserBase() {}

	// mechanism for identifying the user type at runtime.
	virtual const UserTypeId get_user_type() const const = 0;

	const std::string get_username() const { return m_username; }

	const std::string get_password() const { return m_password; }
	void set_password(const std::string& val) { m_password = val; }

	const std::string get_email() const { return m_email; }
	void set_email(const std::string& val) { m_email = val; }

	const void list_all_games()const;
private:
	const Username m_username; // The users username (unique key)
	 string m_password; // Users password.
	 string m_email; // Users email address.
	//const UserTypeId m_userType; //Users user type
};

//--
// PlayerUser represents a system user who owns games
//--
class PlayerUser : public UserBase
{
public:
	using GameList = std::list<Game::GameId>;

	// inherit the constructor.
	using UserBase::UserBase;
	
	// define the specific user type.
	virtual const UserTypeId  get_user_type() const override { return UserTypeId::kPlayerUser; }

	const PlayerUser::GameList& get_game_list() const { return m_ownedGames; }

	double get_available_funds() const { return m_accountFunds; }

private:
	GameList m_ownedGames; // List of owned games.
	double m_accountFunds; // The players available funds.
};

//--
// AdminUser represents a system user who has privileges to modify the system.
//--
class AdminUser : public UserBase
{
public:
	// inherit the constructor.
	using UserBase::UserBase;

	// define the specific user type.
	virtual const UserTypeId get_user_type() const override { return UserTypeId::kAdminUser; }

	void add_user();

	void add_game();

	void list_all_users() const;

	void delete_game();

	void modify_game();

};

