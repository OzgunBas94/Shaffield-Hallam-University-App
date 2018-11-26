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
		cout << "Usertype (admin/player): ";
		cin >> userType;
		while (! (userType != "admin" || userType != "Admin"
			||  userType != "player" || userType != "Player")) {
			cout << " This usertype is not valid. Please try again!";
			cin >> userType;
		}

		DatabaseManager::instance().store_user_data(username, password, email, userType);
	}

void AdminUser::add_game() {
		int gameId;
		string title;
		string description;
		double price;

		cout << "Add new game." << endl;
		cout << "Title: ";
		cin >> title;
		cout << "Description: ";
		cin.ignore();
		getline(cin, description);
		cout << "Price: ";
		cin >> price;

		DatabaseManager::instance().store_game_data(title, description, price);
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
//_____________PlayerUser_____________

PlayerUser::PlayerUser(const Username& username, const string& password, const string& email, const double fund)
	: UserBase(username, password, email), m_accountFunds(fund) {}

const UserTypeId PlayerUser::get_user_type() const {
	return UserTypeId::kPlayerUser;
}

const PlayerUser::GameList& PlayerUser::get_game_list() const {
	return m_ownedGames;
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

void PlayerUser::list_my_games() {
	cout << "My games:" << endl;
	GameList ownedGames = get_game_list();
	for (list<Game::GameId>::const_iterator it = ownedGames.begin(); it != ownedGames.end(); ++it) {
		auto pGame = DatabaseManager::instance().find_game(*it);
		cout << "ID: " << pGame->get_game_id() << " TITLE: " << pGame->get_title() << " DESCRIPTION: " << pGame->get_description() << endl;
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
	cin.ignore();
	getline(cin, id);
	if (id != "") {
		pGame = DatabaseManager::instance().find_game(stoi(id));
	}
	if (pGame != nullptr) {
		gamePrice = pGame->get_price();
	}
	userFunds = this->get_available_funds();
	if (userFunds >= gamePrice) {
		this->withdraw_funds(gamePrice);
		DatabaseManager::instance().modify_user(this->get_username(), this->get_available_funds());
		m_ownedGames.push_back(pGame->get_game_id());
		DatabaseManager::instance().store_bought_game(this, pGame);
		cout << "You successfully bought the game - " << pGame->get_title() << " - " << endl ;
	}
	else {
		cout << "You don't have enough money to buy this game!" << endl << endl;
	}
}

void PlayerUser::add_game_to_list(const Game::GameId& id) {
	m_ownedGames.push_back(id);
}
