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
			else if (userType == "gamestudio") {
				add_user(new GameStudio(username, pw, email));
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
	string readFile, gameTitle, gameDesc, gameStudio;
	double gamePrice;
	int gameId = 0;
	int ageRating;
	int version;

	gameFile.open("listGames.txt");

	if (gameFile.is_open()) {
		if (!(gameFile.peek() == ifstream::traits_type::eof())) {
			while (getline(gameFile, readFile)) {

				gameId = stoi(get_token(readFile));
				gameTitle = get_token(readFile);
				gameDesc = get_token(readFile);
				gamePrice = stod(get_token(readFile));
				ageRating = stoi(get_token(readFile));
				gameStudio = get_token(readFile);
				version = stoi(get_token(readFile));
				Game game(gameId, gameTitle, gameDesc, gamePrice, ageRating, gameStudio, version);
				add_game(game);

				auto pStudio = dynamic_cast<GameStudio*>(find_user(gameStudio));
				if (pStudio != nullptr) {
					pStudio->add_game_to_list(game);
					pStudio = nullptr;
				}
				else {
					cout << "Could not add game to the list" << endl;
				}

			}

			//this->idCount = gameId;
			gameFile.close();
		}
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
		cout << "Could not open the file!";
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
			cout << "Could not open the file!";
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


void DatabaseManager::store_game_data(const string& title,  const string& description, double price, const int gameAgeRating, const string& studio){
	ofstream outFile;
	outFile.open("listGames.txt", ios::app);

	string gameTitle = title;
	string gameDesc = description;
	double gamePrice = price;
	int ageRating = gameAgeRating;
	string gameStudio = studio;

	int id = ++(idCount);
	string gameId = to_string(id);
	string version = "1";
	
	if (!outFile) {
		cout << "Textfile doesn't exist!";
	}
	else {
		outFile << gameId << ',' << gameTitle << ',' << gameDesc << ',' << to_string(gamePrice) << "," << to_string(ageRating) << "," << gameStudio << "," << version << ","<<endl;
		outFile.close();
		Game game(idCount, gameTitle, gameDesc, gamePrice, ageRating, studio,stoi(version));
		add_game(game);
		game.increment_gameCounter();
		cout << "You added the game: " << title << endl;

		auto pStudio = dynamic_cast<GameStudio*>(find_user(studio));
		if (pStudio != nullptr) {
			pStudio->add_game_to_list(game);
			pStudio = nullptr;
		}
		else {
		 cout << "Couldn't open the file: store_newGame()";
		}
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
				}
				tmp += '\n';
				if (newFile.is_open()) {
					newFile << tmp;
				}
				else {
					cout << "Could not open the file!";
				}
			}
			if (!gameIsStored) {
				tmp = rPlayer->get_username() + "," + to_string(rGame->get_game_id()) + "," + get_date_of_purchase() + ",";
				rPlayer->set_date_purchased_game(dateOfPurchase);
				if (newFile.is_open()) {
					newFile << tmp;
				}
				else {
					cout << "Could not open the file!";
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
				cout << "Could not open the file!";
			}
		}
		file.close();
		newFile.close();
		remove("listUserGames.txt");
		rename("listOfUserGames.txt", "listUserGames.txt");
	}
	else {
		cout << "Could not open the file!";
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
	string readFile, id;
	ifstream file("listGames.txt", ios::app);
	ofstream newFile("listOfGames.txt");

	if (file.is_open() && newFile.is_open()) {
		while (getline(file, readFile)) {
			int position = readFile.find(',');
			id = readFile.substr(0, position);
			if (gameId != id) {
				newFile << readFile << endl;
			}else {
				m_games.erase(stoi(gameId));
				remove_users_game(gameId);
			}
		}
		file.close();
		newFile.close();
		remove("listGames.txt");
		rename("listOfGames.txt", "listGames.txt");
	}
	else {
		cout << "Could not open the file" <<endl;
	}
}

void DatabaseManager::remove_users_game(const string& gameId) {
	string readFile, username, id, tmp, purchaseDate;
	ifstream file("listUserGames.txt", ios::app);
	ofstream newFile("listOfUserGames.txt");
	bool isErased = false;
	if (file.is_open()) {
		while (getline(file, readFile)) {
			username = get_token(readFile);
			tmp = username + ",";
			while ((id = get_token(readFile)) != "") {
				purchaseDate = get_token(readFile);
				if (newFile.is_open()) {
					if (gameId != id) {
						tmp += id + "," + purchaseDate + ",";
						newFile << tmp;
					}else {
						auto findPlayer = find_user(username);
						if (findPlayer != nullptr) {
							auto pPlayer = dynamic_cast<PlayerUser*>(findPlayer);
							auto myGames = pPlayer->get_user_map();
							myGames.erase(stoi(gameId));
							if (myGames.size() == 0) {
								isErased = true;
							}
						}
					}
				}else {
					cout << "Could not open the file!";
				}
			}if (!isErased) {
				newFile << endl;
			}else {
				isErased = false;
			}
		}
		file.close();
		newFile.close();
		remove("listUserGames.txt");
		rename("listOfUserGames.txt", "listUserGames.txt");
	}else {
		cout << "Could not open the file!";
	}
}


void DatabaseManager::modify_game(Game*& pGame, const string& newGameDesc, const string& newGamePrice, const string& newVersion) {
	string readFile, id, price;
	ifstream file("listGames.txt", ios::app);
	ofstream newFile("listOfGames.txt");

	if (file.is_open() && newFile.is_open()) {
		if (!(file.peek() == ifstream::traits_type::eof())) {
			while (getline(file, readFile)) {
				int position = readFile.find(',');
				id = readFile.substr(0, position);
				if (pGame != nullptr) {
					if (pGame->get_game_id() != stoi(id)) {
						newFile << readFile << endl;
					}
					else {
						if (!newGamePrice.empty()) {
							readFile = id + "," + pGame->get_title() + "," + pGame->get_description() + "," + newGamePrice + ","
								+ to_string(pGame->get_ageRating()) + "," + pGame->get_studio() + ","
								+ to_string(pGame->get_version()) + ",\n";
							newFile << readFile;
						}
						else if (!newGameDesc.empty()) {
							readFile = id + "," + pGame->get_title() + "," + newGameDesc + ","
								+ to_string(pGame->get_price()) + "," + to_string(pGame->get_ageRating()) + ","
								+ pGame->get_studio() + "," + to_string(pGame->get_version()) + ",\n";
							newFile << readFile;
						}
						else if (!newVersion.empty()) {
							readFile = id + "," + pGame->get_title() + "," + pGame->get_description() + ","
								+ to_string(pGame->get_price()) + "," + to_string(pGame->get_ageRating()) + ","
								+ pGame->get_studio() + "," + newVersion + ",\n";
							newFile << readFile;
						}
					}
				}
				else {
					cout << "Game is null. Could not modify the game.";
				}
			}
		}
		else {
			cout << "There is no game to modify";
		}
		file.close();
		newFile.close();
		remove("listGames.txt");
		rename("listOfGames.txt", "listGames.txt");
	}
	else {
		cout << "Could not open the file";
	}
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
				cout << "Could not open the file!";
			}
		}
		file.close();
		newFile.close();
		remove("listUsers.txt");
		rename("listOfUser.txt", "listUsers.txt");
	}
	else {
		cout << "Could not open the file!";
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


