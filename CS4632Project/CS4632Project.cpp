// this is here because it is here by default
// for std::begin() and std::end()
#include <iterator>
// for json file input
#include <fstream>
// phevaluator library
#include <phevaluator/phevaluator.h>
#include <phevaluator/card.h>
#include <phevaluator/card_sampler.h>
#include <phevaluator/rank.h>
// json parser
#include <nlohmann/json.hpp>
// sqlite-amalgamation version 3.51.3
#include <sqlite3.h>
#include "Game.h"


//main method
int main()
{
    // determine the number of games to simulate
    int num_games;
    int player_id[2];
    // determine whether to create a new database or append to an existing one.
	bool append_to_db;

    // open configuration file
    std::ifstream f("../configuration.json");
    if (f.is_open()) {
        try {
            nlohmann::json j = nlohmann::json::parse(f);
            num_games = j["num_games"];
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

    //initialize tables
    const char* sql;
    char* errMsg = nullptr;
    sql = "CREATE TABLE IF NOT EXISTS game (" \
        "GameID INTEGER PRIMARY KEY AUTOINCREMENT," \
        "P1 TEXT NOT NULL," \
        "P2 TEXT NOT NULL," \
        "Winner TEXT" \
        ");";
    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cout << "Error creating table: " << errMsg << "\n";
        sqlite3_free(errMsg);
        sqlite3_close(db);
        std::exit(1);
	}
    sql = "CREATE TABLE IF NOT EXISTS round (" \
        "RoundID INTEGER PRIMARY KEY AUTOINCREMENT," \
        "GameID INTEGER REFERENCES game(GameID)," \
        "P1R INTEGER NOT NULL," \
        "P1C INTEGER NOT NULL," \
        "P2R INTEGER NOT NULL," \
        "P2C INTEGER NOT NULL," \
        "Bet INTEGER NOT NULL," \
        "Result TEXT" \
        ");";
	if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cout << "Error creating table: " << errMsg << "\n";
        sqlite3_free(errMsg);
        sqlite3_close(db);
		std::exit(1);
	}

    // initialize the game
    Game d1 = Game(player_id[0], player_id[1]);
    // run simulation
    for (int i = 1; i <= num_games; i++) {
        std::cout << "\n\n\nGAME " << i << "\n";
		d1.playGame();
    }

	// close database connection
	sqlite3_close(db);
    std::cout << "END OF PROGRAM";
    return 0;
}

// default VS template comments here because I might make use of the information later.

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
