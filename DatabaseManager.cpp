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
	load_user_game();
	load_recorded_game_data();

}

void DatabaseManager::load_user_list() {
	string username, pw, email, userType, funds, age;
	double fundsInDouble;
	ifstream file("listUsers.txt");
	if (file.is_open()) {

		while (file >> username >> pw >> email >> userType >> funds >>age) {
			if (userType == "admin" || userType == "Admin") {
				add_user(new AdminUser(username, pw, email));
			}
			else if (userType == "guest") {
				add_guest(new GuestUser(username, pw, email));
			}
			else {
				fundsInDouble = stod(funds);
				add_user(new PlayerUser(username, pw, email, stod(age), fundsInDouble));
			}

		}
		file.close();
	}
	else {
		cout << "Could not open the file: " << endl;
	}
}


void DatabaseManager::load_game_list() {

	ifstream gameFile;
	string readFile;
	string gameTitle;
	string gameDesc;
	double gamePrice;
	int gameId = 0;
	int ageRating;

	gameFile.open("listGames.txt");

	if (gameFile.is_open()) {
		while (getline(gameFile, readFile)) {

			gameId = stoi(get_token(readFile));
			gameTitle = get_token(readFile);
			gameDesc = get_token(readFile);
			gamePrice = stod(get_token(readFile));
			ageRating = stoi(get_token(readFile));
			add_game(Game(gameId, gameTitle, gameDesc, gamePrice, ageRating));
		}

		this->idCount = gameId;
		gameFile.close();
	}
	else {
		cout << "Could not open this file!";
	}
}
void DatabaseManager::load_user_game() {
	string readFile;
	string username;
	string gameId;
	string date;
	ifstream file("listUserGames.txt", ios::app);

	if (file.is_open()) {
		if(!(file.peek()== ifstream::traits_type::eof())){
			while (getline(file, readFile)) {
				username = get_token(readFile);
				auto pUser = dynamic_cast<PlayerUser*>(find_user(username));
				while ((gameId = get_token(readFile)) != "") {
					date = get_token(readFile);
					pUser->set_date_purchased_game(date);
					Game::GameId id = stoi(gameId);
					Game* pGame = find_game(stoi(gameId));
					pUser->add_game_to_map(id,pGame);
					pGame->increment_gameCounter();

				}
			}
		}
		file.close();
	}
	else {
		cerr << "Couldn't open the file!";
	}
}

void DatabaseManager::load_recorded_game_data() {
	string readFile;
	string username;
	string gameTitel;
	string date;
	string time;
	string length;

	ifstream file("listRecordedGameData.txt", ios::app);

	if (file.is_open()) {
		if (!(file.peek() == ifstream::traits_type::eof())) {
			while (getline(file, readFile)) {
				username = get_token(readFile);
				gameTitel = get_token(readFile);
				date = get_token(readFile);
				time = get_token(readFile);
				length = get_token(readFile);

				auto pUser = dynamic_cast<PlayerUser*>(find_user(username));
				if (pUser != nullptr) {
					pUser->add_recorded_data(gameTitel, date, time, length);
				}
			}

			file.close();
		}
		else {
			cerr << "Could not open the file!";
		}
	}
}

void DatabaseManager::store_user_data(const string& user, const string& pw, const string& mail, const string& userType, const int userAge){
	ofstream outfile("listUsers.txt", ios_base::app);

	string username =user;
	string password = pw;
	string email = mail;
	string typeOfUser = userType;
	string funds = "0.0";
	int age = userAge;
	double fundsInDouble;
		if (outfile.is_open()){
			outfile << username << " " << password << " " << email << " " << typeOfUser << " " << funds << " " << to_string(age) << endl;
			outfile.close();
			if (typeOfUser == "admin" || typeOfUser == "Admin") {
				add_user(new AdminUser(username, password, email));
			}
			else if (typeOfUser == "guest") {
				add_guest(new GuestUser(username, password, email));

			}
			else {
				fundsInDouble = stod(funds);
				add_user(new PlayerUser(username, password, email, age, fundsInDouble));
			}
		}
		else {
			cout << "This Textfile does not exist!";
		}
	}


void DatabaseManager::store_game_data(const string& title,  const string& description, double price, const int gameAgeRating){
	ofstream outFile;
	outFile.open("listGames.txt", ios::app);

	string gameTitle = title;
	string gameDesc = description;
	int id = ++(this->idCount);
	string gameId = to_string(id);
	double gamePrice = price;
	int ageRating = gameAgeRating;
	if (!outFile) {
		cout << "Textfile doesn't exist!";
	}
	else {
		outFile << id << ',' << gameTitle << ',' << gameDesc << ',' << to_string(gamePrice) << "," << to_string(ageRating) << ";" << endl;
		outFile.close();
		add_game(Game(id, gameTitle, gameDesc, gamePrice, ageRating));
		
		cout << "You added the game: " << title << endl;
	}

}
void DatabaseManager::store_purchased_game(PlayerUser* rPlayer, Game* rGame) {
	ifstream file("listUserGames.txt", ios::app);
	ofstream newFile("listOfUserGames.txt", ios::app);
	string readFile, tmp, username, gameId, dateOfPurchase;
	bool gameIsStored = false;

	if (file.is_open()) {
		if (!(file.peek() == ifstream::traits_type::eof())) {
			while (getline(file, readFile)) {
				username = get_token(readFile);
				tmp = username + ",";
				while ((gameId = get_token(readFile)) != "") {
					tmp += gameId;
					tmp += ",";
					dateOfPurchase = get_token(readFile);
					tmp += dateOfPurchase;
					tmp += ",";
				}
				if (rPlayer->get_username() == username) {
					dateOfPurchase = get_date_of_purchase();
					string id = to_string(rGame->get_game_id()) + ",";
					tmp += id;
					tmp += dateOfPurchase;
					tmp += ",";
					gameIsStored = true;
					rPlayer->set_date_purchased_game(dateOfPurchase);
					rGame->increment_gameCounter();
				}
				tmp += '\n';
				if (newFile.is_open()) {
					newFile << tmp;
				}
				else {
					cerr << "Couldn't open the file!";
				}
			}
			if (!gameIsStored) {
				tmp = rPlayer->get_username() + "," + to_string(rGame->get_game_id()) + "," + get_date_of_purchase() + ",";
				rPlayer->set_date_purchased_game(dateOfPurchase);
				if (newFile.is_open()) {
					newFile << tmp;
				}
				else {
					cerr << "Couldn't open the file!";
				}
			}
		}
		else {
			username = rPlayer->get_username();
			gameId = to_string(rGame->get_game_id());
			tmp = username + "," + gameId + "," + get_date_of_purchase() + ",";
			rPlayer->set_date_purchased_game(dateOfPurchase);
			if (newFile.is_open()) {
				newFile << tmp;
			}
			else {
				cerr << "Couldn't open the file!";
			}
		}
		file.close();
		newFile.close();
		remove("listUserGames.txt");
		rename("listOfUserGames.txt", "listUserGames.txt");
	}
	else {
		cerr << "Couldn't open the file!";
	}
}

const string DatabaseManager::get_date_of_purchase()const {
	string dateOfPurchase;

	time_t date = time(0);   // get time now
	struct tm dateTime;
	errno_t result;
	result = localtime_s(&dateTime, &date);
	dateOfPurchase = (to_string(dateTime.tm_mday) + "/" + to_string(dateTime.tm_mon + 1) + "/" + to_string(dateTime.tm_year + 1900));
	return dateOfPurchase;
}

const string DatabaseManager::get_date_of_play_game()const {
	string dateOfPurchase;

	time_t date = time(0);   // get time now
	struct tm dateTime;
	errno_t result;
	result = localtime_s(&dateTime, &date);
	dateOfPurchase = (to_string(dateTime.tm_mday) + "/" + to_string(dateTime.tm_mon + 1) + "/" + to_string(dateTime.tm_year + 1900));
	return dateOfPurchase;
}

void DatabaseManager::store_recorded_game_data(PlayerUser* rPlayer,const Game* rGame, const string date, string time, const double length) {

	ofstream outfile("listRecordedGameData.txt", ios::app);
	string readFile;
	PlayerUser* pUser = rPlayer;
	string username = rPlayer->get_username();
	string game = rGame->get_title();
	string dateOfPlaying = date;
	string timeOfPlaying = time;
	string lengthOfPlaying = to_string(length);
	bool gameIsStored = false;

	string tmp;


	if (outfile.is_open()) {
		outfile << username << "," << game << "," << dateOfPlaying << "," << timeOfPlaying << "," << lengthOfPlaying << endl;
		outfile.close();
		
		pUser->add_recorded_data(game, dateOfPlaying, timeOfPlaying, lengthOfPlaying);
		
	}
	else {
		cout << "This Textfile does not exist!";
	}

}


void DatabaseManager::add_user(UserBase* pUser)
{
	// Store the user instance in the user map, indexed by username.
	// We are taking ownership of the memory pointer to.
		// open a file in write mode.

		if (pUser)
		{
			m_users.insert(make_pair(pUser->get_username(), pUser));

		}
}

void DatabaseManager::add_guest(UserBase* pUser) {
	if (pUser) {
		guestList.push_back(pUser);
	}
}


void DatabaseManager::add_game(Game& rGame)
{
	// Store the game indexed by game id.
	m_games.insert(make_pair(rGame.get_game_id(), rGame));
	
}

void DatabaseManager::remove_game(const string& gameId) {
	string id;
	ifstream gameFile;
	ofstream outfile("listGames.txt");
	string readfile;
	gameFile.open("listOfGames.txt", ios::app);

	if (gameFile.is_open()) {
		while (getline(gameFile, readfile)) {
			int position = readfile.find(',');
			id = readfile.substr(0, position);
			if (gameId != id) {
				outfile << readfile<< endl;
			}
			else {
				m_games.erase(stoi(gameId));
				cout << "Game successfully deleted! " << endl;
			}

		}
	}
	else {
		cout << "Could not open this file!";
	}
	gameFile.close();
	outfile.close();
	remove("listGames.txt");
	rename("listOfGames.txt", "listGames.txt");
}

void DatabaseManager::modify_game(Game*& mGame, const string& newGameDesc, const string& newGamePrice ) {
	cout << "test_modify_game" << endl;
	ifstream gameFile;
	ofstream outFile("listGames.txt");
	string id;
	string readFile;
	gameFile.open("listOfGames.txt", ios::app);

	if (gameFile.is_open()) {
		while (getline(gameFile, readFile)) {
			int position = readFile.find(',');
			id = readFile.substr(0, position);
			if (mGame->get_game_id() != stoi(id)) {
				outFile << readFile << endl;
			}
			else {
				if (!newGamePrice.empty()) {
					readFile = id + "," + mGame->get_title() + "," + mGame->get_description() + "," + newGamePrice + ",\n";

					outFile << readFile;
				}
				else if (!newGameDesc.empty()) {
					readFile = id + "," + mGame->get_title() + "," + newGameDesc + "," + to_string(mGame->get_price()) + ",\n";
					outFile << readFile;
				}

			}
		}
	}
	gameFile.close();
	outFile.close();

	remove("listRecordedGameData.txt");
	rename("listOfRecordedGameData.txt", "listRecordedGameData.txt");

}

void DatabaseManager::modify_user(const string& username, const double newFunds) {
	string readFile, id, password, email, usertype, fund , age;
	ifstream file("listUsers.txt", ios::app);
	ofstream newFile("listOfUser.txt", ios::app);

	if (file.is_open()) {
		while (getline(file, readFile)) {
			istringstream stream(readFile);
			stream >> id >> password >> email >> usertype >> fund >> age;
			if (newFile.is_open()) {
				if (id != username) {
					newFile << readFile << endl;
				}
				else {
					readFile = id + ' ' + password + ' ' + email + ' ' + usertype + ' ' + to_string(newFunds) + ' ' + age;
					newFile << readFile << endl;
				}
			}
			else {
				cerr << "Couldn't open the file!";
			}
		}
		file.close();
		newFile.close();
		remove("listUsers.txt");
		rename("listOfUser.txt", "listUsers.txt");
	}
	else {
		cerr << "Couldn't open the file!";
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


Game* DatabaseManager::find_game_with_title(string& gameTitle) {
	string title = to_lower_string(gameTitle);
	string iteratorTitle;

	auto it = m_games.begin();
	while (it != m_games.end()) {
		iteratorTitle = it->second.get_title();
		iteratorTitle = to_lower_string(iteratorTitle);
		if (iteratorTitle == title) {
			return &it->second;
		}
		++it;
	}
	return nullptr;
}

void DatabaseManager::list_games_by_age_rating(int age) {
	int ageRating = age;

	auto it = m_games.begin();
	while (it != m_games.end()) {
		if (it->second.get_ageRating() <= ageRating) {
			cout << "ID: " << it->second.get_game_id() << " Title: " << it->second.get_title() << " Price: " << it->second.get_price() << " Age rating: " << it->second.get_ageRating() << endl;
		}
		++it;
	}
}

const map<Game::GameId, Game> DatabaseManager::get_games_map() const
{
	return m_games;
}

const map<UserBase::Username, UserBase*> DatabaseManager::get_map() const {
	return m_users;
}



string DatabaseManager::get_token(string& readFile) {

	int position = readFile.find(',');
	string tmp = readFile.substr(0, position);
	readFile = readFile.substr((position + 1), readFile.length() - (position + 1));
	return tmp;
}
string DatabaseManager::to_lower_string(string& lowerString) {
	string searchLowerTitle;
	for (int i = 0; i <= lowerString.length(); ++i) {
		searchLowerTitle += tolower(lowerString[i]);
	}
	return searchLowerTitle;
}
bool DatabaseManager::find_email(const string& mail) {
	for (auto it = m_users.begin(); it != m_users.end(); ++it) {
		if ((*it).second->get_email() == mail) {
			return true;
		}
	}
	return false;
}


UserBase* DatabaseManager::find_guest(const string& email)
{
	list<UserBase*>::iterator it = guestList.begin();
	while (it != guestList.end()) {
		if ((*it)->get_email() == email) {
			return *it;
		}
		++it;
	}
	return nullptr;
}

const string DatabaseManager::getTime()const {
	SYSTEMTIME time;
	GetLocalTime(&time);
	return (to_string(time.wHour) + ":" + to_string(time.wMinute) + ":" + to_string(time.wSecond));
}


