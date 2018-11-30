//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ozgun Bas
//Student number: b8037359
#pragma once

#include <string>
#include <list>
#include <iostream>
#include <map>
#include <iomanip>
#include <algorithm>

#include "Game.h"
#include "Stopwatch.h"

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
	, kGuestUser
	, kGameStudioUser
};

//_____________________________USER BASE____________________________
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

//_____________________________PLAYER USER_____________________________
//--
// PlayerUser represents a system user who owns games
//--
class PlayerUser : public UserBase
{

public:
	//types
	using GameList = list<Game::GameId>;
	using UserGames = map<Game::GameId, Game*>;
	using RecordedData = list<string>;

	// inherit the constructor.
	using UserBase::UserBase;

	PlayerUser(const Username& username, const string& password, const string& email, const int age, const double fund);
	


	void withdraw_funds(const double val);

	void buy_game();

	void play_game();

	void add_game_to_map(const Game::GameId& id, Game* pGame);

	void add_recorded_data(string, string, string, string);

	void add_funds();

	void gift_a_game();

	const UserGames& get_game_map() const;

	const map<Game::GameId, Game*> get_user_map() const;

	list<string>get_recorded_list();
	
	// define the specific user type.
	virtual const UserTypeId  get_user_type() const override;

	const string get_time_of_playing() const;

	const string get_date_of_bought_game() const;

	const string get_length_of_playing() const;

	double get_available_funds() const;

	const int get_age_of_player() const;

	void search_game_by_title();

	void set_date_purchased_game(const string& dateOfGame);
	
	void set_date_of_playing_game(const string& dateOfGame);

	void set_time_of_playing(const string& timeOfPlaying);

	void set_length_of_playing(string length);

	void list_my_games();

	void list_games_by_ageRating();


private:
	int age;
	string dateOfPlay;
	string date;
	string time;
	string length;
	stopWatch timer;
	double accountFunds = 0.0; // The players available funds.

	UserGames m_usersGames;
	RecordedData m_recordedData;

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
//_____________________________GUEST_____________________________
class GuestUser : public UserBase {

	using UserBase::UserBase;

	virtual const UserTypeId get_user_type() const override;
};

//_____________________________GAME STUDIO_____________________________
class GameStudio : public UserBase {
public:
	using GameList = list<Game>;

	using UserBase::UserBase;

	virtual const UserTypeId get_user_type() const override;

	void set_version();

	float const get_version(const string& gameId) const;

	void add_game_to_list(const Game& rGame);

	const list<Game> get_gameList() const;

	const void output_gameList() const;

private:
	GameList l_gameList;
};