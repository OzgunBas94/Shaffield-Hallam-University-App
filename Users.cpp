//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ozgun Bas
//Student number: b8037359

#include "Users.h"
#include "DatabaseManager.h"

//_____________UserBase_____________

using Username = string;

UserBase::UserBase(const Username& username, const string& password, const string& email)
	: m_username(username)
	, m_password(password)
	, m_email(email)
{}

UserBase::~UserBase() {}

const string UserBase::get_username() const {
	return m_username;
}

const string UserBase::get_password() const {
	return m_password;
}
void UserBase::set_password(const string& val) {
	m_password = val;
}

const string UserBase::get_email() const {
	return m_email;
}

void UserBase::set_email(const string& val) {
	m_email = val;
}
//_____________AdminUser_____________
void AdminUser::add_user() {
	string username;
	string password;
	string email;
	string userType;
	string age;

		cout << "Add new user." << endl;
		cout << "Username: ";
		cin >> username;
		while (!(DatabaseManager::instance().find_user(username) == nullptr)) {
			cout << "This username is already taken. Please try again." << endl;
			cout << "Username: ";
			cin >> username;
		}
		cout << "Password: ";
		cin >> password;
		cout << "Email: ";
		cin >> email;
		while (DatabaseManager::instance().find_email(email)) {
			cout << "This email is already taken. Please try again!: ";
			cout << "Email: ";
			cin >> email;
		}
		cout << "Age: ";
		cin >> age;
		cout << "Usertype (admin/player): ";
		cin >> userType;
		while (! (userType != "admin" || userType != "Admin"
			||  userType != "player" || userType != "Player")) {
			cout << " This usertype is not valid. Please try again!";
			cin >> userType;
		}

		DatabaseManager::instance().store_user_data(username, password, email, userType, stoi(age));
	}

void AdminUser::add_game() {
		string title;
		string description;
		double price;
		string ageRating;

		cout << "Add new game." << endl;
		cout << "Title: ";
		cin >> title;
		cout << "Description: ";
		cin.ignore();
		getline(cin, description);
		cout << "Price: ";
		cin >> price;
		cout << "Define an age rating for this game: ";
		cin >> ageRating;

		DatabaseManager::instance().store_game_data(title, description, price, stoi(ageRating));
}

void AdminUser::list_all_users() const
{
	auto userVisitorLambda = [](const UserBase& rUser) {
		cout << rUser.get_username() << " " << rUser.get_email() << " " << "\n";
	};

	DatabaseManager::instance().visit_users(userVisitorLambda);
}

void AdminUser::delete_game(){
	cout << "ID of the game you want to delete: " << endl;
	string gameId;
	cin >> gameId;

	DatabaseManager::instance().remove_game(gameId);
}


void AdminUser::modify_game(Game*& game, const int option, const int gameId) {

	string newGameDesc;
	string newGamePrice;

	if (option == 1) {
		cout << "New description: " << endl;
		cin.ignore();
		getline(cin, newGameDesc);
		game->set_description(newGameDesc);
		DatabaseManager::instance().modify_game(game, newGameDesc, "");
		cout << "You have changed the description successfully!" << endl;
	}
	else {
		cout << "New price: " << endl;
		cin >> newGamePrice;
		double newPrice = stod(newGamePrice);
		game->set_price(newPrice);
		DatabaseManager::instance().modify_game(game,"", newGamePrice);
		cout << "You have changed the description successfully!" << endl;
	}

}

const map<Game::GameId, Game*> PlayerUser::get_user_map() const {
	return m_usersGames;
}

 list<string>PlayerUser::get_recorded_list()  {
	return m_recordedData;
}

void AdminUser::view_statistics() {
	cout << "Which statistic you want to view" << endl;
	cout << "(1) Records of all purchases" << endl;
	cout << "(2) Time each player spends in each game" << endl;
	string username, game, date, time, length;
	
	char option;
	cin >> option;

	map<UserBase::Username, UserBase*> myMap = DatabaseManager::instance().get_map();
	switch (option) {
	case '1':
		if (!myMap.empty()) {
			for (map<UserBase::Username, UserBase*>::const_iterator it = myMap.begin(); it != myMap.end(); ++it) {
				if (it->second->get_user_type() == UserTypeId::kPlayerUser) {
					PlayerUser* pUser = dynamic_cast<PlayerUser*>(it->second);
					if (!pUser->get_user_map().empty()) {
						for (auto it: pUser->get_user_map()) {
							{
								cout << pUser->get_username() << " has bought " << it.second->get_title() << " " << it.second->get_game_id() << " " << pUser->get_date_of_bought_game() << endl;
							}

						}
					}

				}

			}
		}break;

	case '2': 
		
		if (!myMap.empty()) {
			for (map<UserBase::Username, UserBase*>::iterator it = myMap.begin(); it != myMap.end(); ++it) {
				if (it->second->get_user_type() == UserTypeId::kPlayerUser) {
					PlayerUser* pUser = dynamic_cast<PlayerUser*>(it->second);
					list <string> recordedList = pUser->get_recorded_list();
					
						list<string>::const_iterator it2 = recordedList.begin();
						if (recordedList.size()>0) {
							cout << pUser->get_username();
						while (it2 != recordedList.end()) {
							{
								game = *it2;
								date = *(++it2);
								time = *(++it2);
								length = *(++it2);
								cout << "played " << game << " on " << date << " at " << time << " for" << length << endl;
								++it2;
							}
						}
					}

				}

			}
		} break;

	default:  cout << "INAVLID OPTION\n"; break;
	}
}

//_____________PlayerUser_____________

PlayerUser::PlayerUser(const Username& username, const string& password, const string& email, const int age, const double fund)
	: UserBase(username, password, email),  m_age(age), m_accountFunds(fund) {}

const UserTypeId PlayerUser::get_user_type() const {
	return UserTypeId::kPlayerUser;
}

double PlayerUser::get_available_funds() const {
	return m_accountFunds;
}

void PlayerUser::search_game_by_title() {
	string title;
	cout << "Which game do you want to search?" << endl;
	cout << "Title: ";
	cin.ignore();
	getline(cin, title);

	auto pGame = DatabaseManager::instance().find_game_with_title(title);
	if (pGame == nullptr) {
		cout << "This game is not existing! Please try again" << endl << endl;
	}
	else {
		cout << "title: " << pGame->get_title() << " description: " << pGame->get_description() << " price: " << pGame->get_price() << endl << endl;
	}
}
void PlayerUser::list_games_by_ageRating() {
	int age;
	cout << "Until which age rating do you want to list the games? " << endl;
	cout << "Age rating: ";
	cin >> age;

	DatabaseManager::instance().list_games_by_age_rating(age);

}

void PlayerUser::list_my_games() {
	cout << "My games:" << endl;
	UserGames m_usersGamesMap = get_user_map();
	if (!m_usersGamesMap.empty()) {
		for (auto it : m_usersGamesMap) {
			auto pGame = DatabaseManager::instance().find_game(it.first);
			cout << "ID: " << pGame->get_game_id() << " TITLE: " << pGame->get_title() << " DESCRIPTION: " << pGame->get_description() << endl;
		}
	}
	else {
		cout << "map is empty" << endl;
	}
}

void PlayerUser::add_funds() {
	cout << "Add funds: ";
	string fund;
	cin >> fund;
	this->m_accountFunds += stod(fund);
	double get = get_available_funds();
	cout << "FUNDS: " << get << endl;
	DatabaseManager::instance().modify_user(get_username(), m_accountFunds);
	cout << "You successfully added " << fund << " in your wallet! \n Current wallet: " << m_accountFunds << endl;
}

void PlayerUser::withdraw_funds(const double val) {
	m_accountFunds -= val;
}
void PlayerUser::buy_game() {
	Game* pGame = nullptr;
	double userFunds = 0;
	double gamePrice = 0;
	string id;

	cout << "Enter the ID of the game: ";
	cin >> id;

	if (id != "") {
		pGame = DatabaseManager::instance().find_game(stoi(id));
	}
	if (this->get_age_of_player() >= pGame->get_ageRating()) {
		if (pGame != nullptr) {
			gamePrice = pGame->get_price();
		}
		userFunds = this->get_available_funds();
		if (userFunds >= gamePrice) {
			this->withdraw_funds(gamePrice);
			DatabaseManager::instance().modify_user(this->get_username(), this->get_available_funds());
			add_game_to_map(pGame->get_game_id(), pGame);
			DatabaseManager::instance().store_purchased_game(this, pGame);
			cout << "You successfully bought the game - " << pGame->get_title() << " - " << endl;
		}
		else {
			cout << "You don't have enough money to buy this game!" << endl << endl;
		}
	}else {
			cout << "You are to young to buy this game." << endl;
		}
	}

void PlayerUser::add_game_to_map(const Game::GameId& id, Game* pGame) {
	m_usersGames.insert(make_pair(id, pGame));
}

void PlayerUser::add_recorded_data( string game, string date, string time, string length) {
	m_recordedData.push_back(game);
	m_recordedData.push_back(date);
	m_recordedData.push_back(time);
	m_recordedData.push_back(length);
}	

void PlayerUser::play_game() {
	string gameId;
	int result = 0;
	string date;
	string time;
	double length;

	cout << "Which game do you want to play? \n";
	cout << "ID: ";
	cin >> gameId;

	auto pGame = DatabaseManager::instance().find_game(stoi(gameId));
	CStopWatch stopwatch;
	stopwatch.startTimer();

	cout << "Welcome to -- " << pGame->get_title() << " --" <<endl;

	while (result == 0) {
		char option;
		cout << "(q) Quit the Game" << endl;
		cin >> option;
		switch (option) {
		case 'q': 
		
			
			date = DatabaseManager::instance().get_date_of_play_game();
			time = DatabaseManager::instance().getTime();
			stopwatch.stopTimer();
			length = stopwatch.getElapsedTime();
			
			DatabaseManager::instance().store_recorded_game_data(this,pGame,date,time,length);
			
			result = -1;
			break;
		default:  cout << "INAVLID OPTION\n"; break;
		}

	}
	cout << length;
}

void PlayerUser::set_date_purchased_game(const string& dateOfGame) {
	  date = dateOfGame;
}

const string PlayerUser::get_date_of_bought_game() const{
	return date;
}

void PlayerUser::set_date_of_playing_game(const string& dateOfPlayingGame) {
	dateOfPlay = dateOfPlayingGame;
}

void PlayerUser::set_time_of_playing(const string& timeOfPlaying) {
	time = timeOfPlaying;
}

 const string PlayerUser::get_time_of_playing() const {
	return time;
}

void PlayerUser::set_length_of_playing(string lengthOfPlaying) {

	length = lengthOfPlaying;
}

const string PlayerUser::get_length_of_playing() const {
	return length;
}

const int PlayerUser::get_age_of_player() const {
	return m_age;
}

//void PlayerUser::set_purchased_time(const string& timestemp) {
//	time = timestemp;
//}

//_____________________________________________________GUEST________________________________________________________

const UserTypeId GuestUser::get_user_type() const {
	return UserTypeId();
}




