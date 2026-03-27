// this is here because it is here by default
// for std::begin() and std::end()
#include <iterator>
// for json file input
#include <fstream>
// phevaluator library
#include <phevaluator/phevaluator.h>
// json parser
#include <nlohmann/json.hpp>
// sqlite-amalgamation version 3.51.3
#include <sqlite3.h>
#include "Game.h"

static void create_tables(sqlite3* db) {
    //initialize tables
    const char* sql;
    char* errMsg = nullptr;
    //create game table
    sql = "CREATE TABLE IF NOT EXISTS game (" \
        "GameID INTEGER PRIMARY KEY AUTOINCREMENT," \
        "Bot1ID INTEGER NOT NULL," \
        "Bot2ID INTEGER NOT NULL," \
        "GameWinner INTEGER," \
        "Starting_Chip_Count INTEGER NOT NULL" \
        ");";
    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cout << "Error creating table: " << errMsg << "\n";
        sqlite3_free(errMsg);
        sqlite3_close(db);
        std::exit(1);
    }
    //create round table
    sql = "CREATE TABLE IF NOT EXISTS round (" \
        "GameID INTEGER NOT NULL," \
        "RoundNumber INTEGER NOT NULL," \
        "RoundWinner INTEGER," \
        "B1Hand VARCHAR(16)," \
        "B2Hand VARCHAR(16)," \
        "RevealedCC VARCHAR(64)," \
        "PRIMARY KEY (GameID, RoundNumber)," \
        "FOREIGN KEY (GameID) REFERENCES game(GameID)" \
        ");";
    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cout << "Error creating table: " << errMsg << "\n";
        sqlite3_free(errMsg);
        sqlite3_close(db);
        std::exit(1);
    }
    //create action table
    sql = "CREATE TABLE IF NOT EXISTS action (" \
        "GameID INTEGER NOT NULL," \
        "RoundNumber INTEGER NOT NULL," \
        "SequenceNumber INTEGER NOT NULL," \
        "ActingPlayer INTEGER NOT NULL," \
        "Phase INTEGER NOT NULL," \
        "Action INTEGER NOT NULL," \
        "PRIMARY KEY (GameID, RoundNumber, SequenceNumber)," \
        "FOREIGN KEY (GameID, RoundNumber) " \
        "REFERENCES round(GameID, RoundNumber)" \
        ");";
    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cout << "Error creating table: " << errMsg << "\n";
        sqlite3_free(errMsg);
        sqlite3_close(db);
        std::exit(1);
    }
}

static void insert_game(sqlite3* db, int bot1_id, int bot2_id, int starting_chip_count) {
    const char* sql = "INSERT INTO game (Bot1ID, Bot2ID, Starting_Chip_Count) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cout << "Error preparing statement: " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        std::exit(1);
    }
    if (sqlite3_bind_int(stmt, 1, bot1_id) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 2, bot2_id) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 3, starting_chip_count) != SQLITE_OK) {
        std::cout << "Error binding parameters: " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        std::exit(1);
    }
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cout << "Error executing statement: " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        std::exit(1);
    }
    sqlite3_finalize(stmt);
}

static void add_winner(sqlite3* db, int game_id, int winner_id) {
	const char* sql = "UPDATE game SET GameWinner = ? WHERE GameID = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cout << "Error preparing statement: " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        std::exit(1);
    }
    if (sqlite3_bind_int(stmt, 1, winner_id) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 2, game_id) != SQLITE_OK) {
        std::cout << "Error binding parameters: " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        std::exit(1);
    }
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cout << "Error executing statement: " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        std::exit(1);
    }
    sqlite3_finalize(stmt);
}

//main method
int main()
{
    // determine the number of games to simulate
    int num_games;
    int player_id[2];
    int num_chips;
    // determine whether to create a new database or append to an existing one.
	bool append_to_db;
    int game_id;

    // open configuration file
    std::ifstream f("../configuration.json");
    if (f.is_open()) {
        try {
            nlohmann::json j = nlohmann::json::parse(f);
            num_games = j["num_games"];
			num_chips = j["num_chips"];
			append_to_db = j["append_to_db"];
			player_id[0] = j["Bot1"];
			player_id[1] = j["Bot2"];
        }
        catch (std::exception& e) {
            std::cout << "Error parsing JSON file: " << e.what() << "\n";
            num_games = 1;
        }
    }
    else {
		std::cout << "Could not find JSON file.";
        std::exit(1);
    }
    //initialize the sqlite database
	sqlite3* db;
	sqlite3_open("simulation.db", &db);
    if (db == nullptr) {
        std::cout << "Error opening database.\n";
        std::exit(1);
	}
	create_tables(db);
    // initialize the game
    Game d1 = Game(player_id[0], player_id[1]);
    
    // run simulation
    for (int i = 1; i <= num_games; i++) {
        std::cout << "\n\n\nGAME " << i << "\n";
        //insert into Game table before other tables have to insert into tables of their own
        insert_game(db, player_id[0], player_id[1], num_chips);
        int game_id = sqlite3_last_insert_rowid(db);
	    add_winner(db, game_id, d1.playGame(num_chips, db, game_id));
    }
	// close database connection
	sqlite3_close(db);
    std::cout << "END OF PROGRAM";
    return 0;
}
