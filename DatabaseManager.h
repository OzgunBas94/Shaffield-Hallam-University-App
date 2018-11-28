//C++ Boot Camp - Task 2 - 2018-19 
//Name: 
//Student number: 
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
	void store_user_data(string user, string pw, string mail, string usertype);
	void store_game_data(string& title, string& description, double price);
	void store_purchased_game(PlayerUser* rPlayer, Game* rGame);
	void store_recorded_game_data(PlayerUser* rPlayer, const Game* rGame, const string date, string time, const double length);

	// Adds a user to the db.
	void add_user(UserBase* pUser);

	// Finds a user by username, return nullptr if the user is not found.
	UserBase* find_user(const UserBase::Username& username);

	// iterating users using visitor pattern
	template<typename Visitor> void visit_users(Visitor& func)
	{
		for (auto it : m_users) { func(*it.second); }
	}

	// Adds a game to the db.
	void add_game(Game& rGame);

	// Finds a game by id, returns nullptr if the game is not found.
	Game* find_game(const Game::GameId gameid);

	// iterating games using visitor pattern
	template<typename Visitor> void visit_games(Visitor& func)
	{
		for (auto it : m_games) { func(it.second); }
	}

	string get_token(string& readFile);
	void remove_game(const string& gameId);
	void modify_game(Game*& game, const string& newGameDesc, const string& newGamePrice);
	Game* find_game_with_title(string& gameTitle);
	string to_lower_string(string& lowerString);
	void modify_user(const string& username, const double newFunds);
	bool find_email(const string& mail);
	const map<UserBase::Username, UserBase*> get_map() const;
	const string get_date_of_purchase()const;
	const string get_date_of_play_game()const;
	const string getTime()const;
	UserBase* find_guest(const string& email);
	void add_guest(UserBase* pUser);
private:
	// Constructors are private for singleton pattern.
	DatabaseManager();
	~DatabaseManager();


private:
	// Types
	using UserContainer = map<UserBase::Username, UserBase*>;
	using GameContainer = map<Game::GameId, Game>;
	using UserGuest = list<UserBase*>;


	UserGuest guestList;
	UserContainer m_users;
	GameContainer m_games;
	int idCount;
};

