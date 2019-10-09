#ifndef P2_JUGADOR_HPP
#define P2_JUGADOR_HPP

#include <string>
#include <sqlite3.h>
//#include "partida.hpp"
#include "extra_functions.hpp"

namespace r{
    class Player{
    private:
        std::string _username;
        std::string _password;
        uint _score;
        uint _wins;
        uint _loses;
        int _socket;
//        Game * _partida;
    public:
        Player(){}
        Player(const std::string &st, const int &socket):_socket(socket){
            this->load_user(st);
        }
        Player(const std::string &username, const std::string &password){
            this->create_user(username, password);
        }
        inline void set_socket(const int &socket) { _socket = socket; }
        inline int get_socket() const { return _socket; }
//        Game * get_partida() const { return _partida; }
//        void set_partida(Game *partida) { _partida = partida; }
        void load_user(const std::string &username){
            sqlite3 *db;
            sqlite3_open("players.db", &db);
            sqlite3_stmt *stmt;
            std::string order = "select * from players where username = ?";
            int rc = sqlite3_prepare_v2(db, order.c_str(), -1, &stmt, NULL);
            if( rc != SQLITE_OK )
                // TODO: Quitar los throws y manejar los errores.
                throw std::string(sqlite3_errmsg(db));
            sqlite3_bind_text(stmt, 1, username.c_str(), -1, 0);
            rc = sqlite3_step(stmt);
            // http://stackoverflow.com/a/804131
            this->set_username(std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0))));
            this->set_password(std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1))));
            this->set_score(sqlite3_column_int(stmt, 2));
            this->set_wins(sqlite3_column_int(stmt, 3));
            this->set_loses(sqlite3_column_int(stmt, 4));
            sqlite3_finalize(stmt);
            sqlite3_close(db);
        }
        void create_user(const std::string &username, const std::string &password){
            sqlite3 *db;
            sqlite3_open("players.db", &db);
            std::string order = "insert into players (username, password) values (?, ?)";
            sqlite3_stmt *stmt;
            int rc = sqlite3_prepare_v2(db, order.c_str(), -1, &stmt, NULL);
            if (rc != SQLITE_OK)
                throw std::string(sqlite3_errmsg(db));
            sqlite3_bind_text(stmt, 1, username.c_str(), -1, 0);
            sqlite3_bind_text(stmt, 2, password.c_str(), -1, 0);
            sqlite3_step(stmt);
            sqlite3_close(db);
            this->set_username(username);
            this->set_password(password);
            this->set_score(0);
            this->set_wins(0);
            this->set_loses(0);
        }
        // TODO: Actualizar las funciones para que usen la base de datos.
        inline void set_username(const std::string &username) { _username = username; }
        inline void set_password(const std::string &password) { _password = password; }
        inline void set_score(const uint &score) { _score = score; }
        inline void set_wins(const uint &wins) { _wins = wins; }
        inline void set_loses(const uint &loses) { _loses = loses; }
        inline void add_score(const uint &score) { _score += score; }
        inline void add_win() { _wins += 1; }
        inline void add_lose() { _loses += 1; }
        void commit() {
            sql_stmt("update players set wins = " + std::to_string(_wins) + ", loses = " + std::to_string(_loses) + ", score = " + std::to_string(_score) + " where username = \"" + _username + "\"");
        }
        inline std::string get_username() const { return _username; }
        inline std::string get_password() const { return _password; }
        inline uint get_score() const { return _score; }
        inline uint get_wins() const { return _wins; }
        inline uint get_loses() const {return _loses; }
        friend std::ostream &operator <<(std::ostream &output, const Player &p){
            output << "Stats:" << std::endl;
            output << "\tTotal score: " << p.get_score() << std::endl;
            output << "\tWins: " << p.get_wins() << std::endl;
            output << "\tLoses: " << p.get_loses() << std::endl;
            return output;
        }
        std::string get_stats() const {
            std::string res = "Stats:\n\tTotal score: " + std::to_string(this->get_score()) + "\n\tWins: " + std::to_string(this->get_wins()) + "\n\tLoses: " + std::to_string(this->get_loses()) + "\n\n";
            return res;
        }
    };
}

#endif
