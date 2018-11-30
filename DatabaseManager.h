//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ozgun Bas
//Student number: b8037359
#pragma once

#include "Game.h"
#include "Users.h"
#include "MenuSystem.h"

#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>

using namespace std;

//--
// DatabaseManager is the one central database for the whole system
// In this example it has been designed using the singleton pattern.
//--
class DatabaseManager
{
public:
	// Singleton instance definition.
	static DatabaseManager& instance();

	// Initialize the database from storage.
	void load_data();
	void load_user_list();
	void load_game_list();
	void load_user_game();
	void load_recorded_game_data();


	// Write all the data to storage.
	void store_user_data(const string& user, const string& pw, const string& mail, const string& usertype, const int userAge);
	void store_game_data(const string& title, const string& description, double price, const int gameAgeRating, const string& studio);
	void store_purchased_game(PlayerUser* rPlayer, Game* rGame);
	void store_recorded_game_data(PlayerUser* rPlayer, const Game* rGame, const string date, string time, const double length);

	// Adds a user to the db.
	void add_user(UserBase* pUser);
	// Adds a game to the db.
	void add_game(const Game& rGame);
	void add_guest(UserBase* pUser);

	// Finds a user by username, return nullptr if the user is not found.
	UserBase* find_user(const UserBase::Username& username);

	// iterating users using visitor pattern
	template<typename Visitor> void visit_users(Visitor& func){for (auto it : m_users) { func(*it.second); }}


	// Finds a game by id, returns nullptr if the game is not found.
	Game* find_game(const Game::GameId gameid);

	bool find_email(const string& mail);

	UserBase* find_guest(const string& email);

	Game* find_game_with_title(string& gameTitle);

	// iterating games using visitor pattern
	template<typename Visitor> void visit_games(Visitor& func){for (auto it : m_games) { func(it.second); }}

	// Types
	using UserContainer = map<UserBase::Username, UserBase*>;

	using GameContainer = map<Game::GameId, Game>;

	using UserGuest = list<UserBase*>;

	void list_games_by_age_rating(int age);
	const map<Game::GameId, Game> get_games_map()const;

	//get the variables of the file
	string get_token(string& readFile);
	//removing the game
	void remove_game(const string& gameId);

	void remove_users_game(const string& gameId);

	void modify_user(const string& username, const double newFunds);

	void modify_game(Game*& pGame, const string& newGameDesc, const string& newGamePrice, const string& newVersion);
	
	string to_lower_string(string& lowerString);
	

	const map<UserBase::Username, UserBase*> get_map() const;

	const string get_date_of_purchase()const;

	const string get_date_of_play_game()const;

	const string getTime()const;

private:
	// Constructors are private for singleton pattern.
	DatabaseManager();
	~DatabaseManager();


private:
	UserGuest guestList;
	UserContainer m_users;
	GameContainer m_games;
	int idCount=0;
};

