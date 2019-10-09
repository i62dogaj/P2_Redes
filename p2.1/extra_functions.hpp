#ifndef __EXTRA_FUNCTIONS_HPP__
#define __EXTRA_FUNCTIONS_HPP__

#include <sqlite3.h>
#include <string>
#include <cctype>
#include <vector>
#include <iostream>
//#include "jugador.hpp"


bool sql_exists(const std::string &st, const std::string &ps = ""){
    sqlite3 *db;
    int rc = sqlite3_open("players.db", &db);
    // TODO: Control de errores.
    std::string order;
    if(ps.empty())
        order = "select * from players where username = ?";
    else
        order = "select * from players where username = ? and password = ?";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, order.c_str(), -1 , &stmt, NULL);
    if (rc != SQLITE_OK)
        throw std::string(sqlite3_errmsg(db));
    // sqlite3_bind_text(stmt, 1, st.c_str(), -1, 0);
    // http://stackoverflow.com/a/14438228
    sqlite3_bind_text(stmt, 1, st.c_str(), -1, 0);
    if(!ps.empty())
        sqlite3_bind_text(stmt, 2, ps.c_str(), -1, 0);
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return rc==SQLITE_ROW;
}

void sql_stmt(const std::string &st){
    sqlite3 *db;
    sqlite3_open("players.db", &db);
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, st.c_str(), -1 , &stmt, NULL);
    if (rc != SQLITE_OK)
        throw std::string(sqlite3_errmsg(db));
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

std::vector<std::string> get_random_refran(){
    sqlite3 *db, *db_troll;
    sqlite3_stmt *stmt, *stmt_troll;
    std::string order, res1, res2;
    std::vector<std::string> res(2);
    sqlite3_open("refranes.db", &db);
    sqlite3_open("cojones.db", &db_troll);
    order = "select count(id) from refranes";
    sqlite3_prepare_v2(db, order.c_str(), -1, &stmt, NULL);
    sqlite3_step(stmt);
    int size = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    int random = 1 + rand() % size;
    order = "select solution from refranes where id = ?";
    sqlite3_prepare_v2(db, order.c_str(), -1, &stmt, NULL);
    sqlite3_prepare_v2(db_troll, order.c_str(), -1, &stmt_troll, NULL);
    sqlite3_bind_int(stmt, 1, random);
    sqlite3_bind_int(stmt_troll, 1, random);
    sqlite3_step(stmt);
    sqlite3_step(stmt_troll);
    res[0] = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
    res[1] = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt_troll, 0)));
    sqlite3_finalize(stmt);
    sqlite3_finalize(stmt_troll);
    sqlite3_close(db);
    sqlite3_close(db_troll);
    return res;
}

std::string convert_solution(const std::string &solution){
    std::string res = "";
    for(auto it = solution.begin() ; it != solution.end() ; ++it){
        if((*it ) == ' ')
            res += " ";
        else
            res += "-";
    }
    return res;
}

// Check isalpha(c) before
bool is_vowel(const char &c){
    return std::string("aeiou").find(tolower(c)) != std::string::npos;
}

#endif
