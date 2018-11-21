//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ozgun Bas
//Student number: b8037359

#include "Users.h"
#include "DatabaseManager.h"

//_____________UserBase_____________

using Username = std::string;

UserBase::UserBase(const Username& username, const string& password, const string& email)
	: m_username(username)
	, m_password(password)
	, m_email(email)
{}

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
		//cout << "added game" << endl;
}


void AdminUser::list_all_users() const
{
	auto userVisitorLambda = [](const UserBase& rUser) {
		std::cout << rUser.get_username() << " " << rUser.get_email() << " " << "\n";
	};

	DatabaseManager::instance().visit_users(userVisitorLambda);
}

void AdminUser::delete_game(){
	cout << "ID of the game you want to delete: " << endl;
	string gameId;
	cin >> gameId;

	DatabaseManager::instance().remove_game(gameId);
}


void AdminUser::modify_game() {
//TODO
}


//_____________PlayerUser_____________