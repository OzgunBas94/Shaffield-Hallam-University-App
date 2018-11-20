//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ozgun Bas
//Student number: b8037359

#include "DatabaseManager.h"

DatabaseManager::DatabaseManager()
{

}

DatabaseManager::~DatabaseManager()
{
	// We must delete users since we have take ownership.
	for (auto it : m_users)
	{
		if (it.second) delete it.second;
	}
}

DatabaseManager& DatabaseManager::instance()
{
	static DatabaseManager s_instance;
	return s_instance;
}


void DatabaseManager::load_data()
{
	
	load_user_list();
	load_game_list();

}


void DatabaseManager::store_user_data(string user, string pw, string mail, string userType)
{
	ofstream outfile("listOfUsers.txt", std::ios_base::app);

	string username =user;
	string password = pw;
	string email = mail;
	string typeOfUser = userType;

		if (!outfile) {
			cerr << "Textfile doesn't exist!";
		}
		else {
			outfile << username << " " << password << " " << email << " " << userType << endl;
			outfile.close();
		}
	}


void DatabaseManager::load_user_list() {
	string user;
	string pw;
	string email;
	ifstream file("listOfUsers.txt");
	if (file.is_open()) {

		while (file >> user >> pw >> email) {
			add_user(new AdminUser(user, pw, email));
		}
		file.close();
	}
	else {
		cout << "File is not open" << endl;
	}
}

void DatabaseManager::store_game_data(string title, string description, double price)
{
	ofstream outFile;
	outFile.open("listOfGames.txt", ios::app);

	string gameTitle = title;
	string gameDesc = description;
	int id = ++(this->idCount);
	string gameId = to_string(id);
	double gamePrice = price;

	if (!outFile) {
		cout << "Textfile doesn't exist!";
	}
	else {
		outFile << id << ',' << gameTitle << ',' << gameDesc << ',' << to_string(gamePrice) << endl;
		outFile.close();
		add_game(Game(id, gameTitle, gameDesc, gamePrice));
		cout << "You added the game: " << title << endl;
	}

}

void DatabaseManager::load_game_list() {

	ifstream gameFile;
	string readFile;
	int gameId = 0;
	string gameTitle;
	string gameDesc;
	double gamePrice;
	
	gameFile.open("listOfGames.txt");

	if (gameFile.is_open()) {
		while (getline(gameFile, readFile)) {
			gameId = stoi(get_token(readFile));
			gameTitle = get_token(readFile);
			gameDesc = get_token(readFile);
			gamePrice = stod(get_token(readFile));

			add_game(Game(gameId, gameTitle, gameDesc, gamePrice));
		}
		this->idCount = gameId;
		gameFile.close();
	}
	else {
		cout << "Could not open this file!";
	}
}


string DatabaseManager::get_token(string& readFile) {
	int position = readFile.find(',');
	string tmp = readFile.substr(0, position);
	readFile = readFile.substr((position + 1), readFile.length() - (position + 1));
	return tmp;
}

void DatabaseManager::add_user(UserBase* pUser)
{
	// Store the user instance in the user map, indexed by username.
	// We are taking ownership of the memory pointer to.
		// open a file in write mode.


		if (pUser)
		{
			m_users.insert(std::make_pair(pUser->get_username(), pUser));

		}
}


UserBase* DatabaseManager::find_user(const UserBase::Username& username)
{
	auto it = m_users.find(username);
	if (it != m_users.end())
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

void DatabaseManager::add_game(Game& rGame)
{
	// Store the game indexed by game id.
	m_games.insert(std::make_pair(rGame.get_game_id(), rGame));
}

Game* DatabaseManager::find_game(const Game::GameId gameid)
{
	auto it = m_games.find(gameid);
	if (it != m_games.end())
	{
		return &it->second;
	}
	else
	{
		return nullptr;
	}
}
bool DatabaseManager::find_email() {
	//TODO
	return false;
}