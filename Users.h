//C++ Boot Camp - Task 2 - 2018-19 
//Name: 
//Student number: 
#pragma once

#include <string>
#include <list>
#include <iostream>
#include <map>
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
	using Username = string;

	UserBase(const Username& username, const string& password, const string& email);
	virtual ~UserBase();
	// mechanism for identifying the user type at runtime.
	virtual const UserTypeId get_user_type() const const = 0;
	const string get_username() const;
	const string get_password() const;
	void set_password(const string& val);
	const string get_email() const;
	void set_email(const string& val);

private:
	const Username m_username; // The users username (unique key)
	 string m_password; // Users password.
	 string m_email; // Users email address.
};

//--
// PlayerUser represents a system user who owns games
//--
class PlayerUser : public UserBase
{
public:
	using GameList = std::list<Game::GameId>;

	using UserGames = map<Game::GameId, Game*>;

	// inherit the constructor.
	using UserBase::UserBase;

	PlayerUser(const Username& username, const string& password, const string& email, const double fund);
	// define the specific user type.
	virtual const UserTypeId  get_user_type() const override;
	//const GameList& get_game_list() const;
	double get_available_funds() const;
	void search_game_by_title();
	void list_my_games();
	void add_funds();
	void withdraw_funds(const double val);
	void buy_game();
	void add_game_to_map(const Game::GameId& id, Game* pGame);
	void play_game();

	void set_date_of_bought_game(const string& dateOfGame);
	const string get_date_of_bought_game() const;

	void set_date_of_playing_game(const string& dateOfGame);
	const string get_date_of_playing_game() const;

	void set_time_of_playing(const string& timeOfPlaying);
	const string get_time_of_playing() const;

	const map<Game::GameId, Game*> get_user_map() const;
	void output_map();


private:
	string dateOfPlay;
	string date;
	string time;
	UserGames m_usersGames;
	//GameList m_ownedGames; // List of owned games.
	double m_accountFunds = 0.0; // The players available funds.
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

	void modify_game(Game*& game, const int option, const int gameId);

	void view_statistics();


};

