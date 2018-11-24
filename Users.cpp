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
		if (DatabaseManager::instance().find_user(username) == nullptr) {
			cout << "Password: ";
			cin >> password;
			cout << "Email: ";
			cin >> email;
			cout << "Usertype (admin/player): ";
			cin >> userType;
		}
		else {
			cout << "This username is already taken. Please try again!";
			add_user();
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



const UserTypeId PlayerUser::get_user_type() const {
	return UserTypeId::kPlayerUser;
}

const PlayerUser::GameList& PlayerUser::get_game_list() const {
	return m_ownedGames;
}

double PlayerUser::get_available_funds() const {
	return m_accountFunds;
}

void PlayerUser::search_game() {
	string title;
	cout << "Which game do you want to search?" << endl;
	cout << "Title: " << endl;
	cin >> title;
}

void PlayerUser::add_game_to_list() {

}