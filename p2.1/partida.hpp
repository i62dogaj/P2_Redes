#ifndef P2_PARTIDA_HPP
#define P2_PARTIDA_HPP

#include <vector>
#include <cassert>
#include <string>
#include "jugador.hpp"
#include "extra_functions.hpp"
#define MULTIPLAYER 2
#define SINGLEPLAYER 1

namespace r{
    typedef int type;
    class Game{
    private:
        std::vector<Player> _players;
        type _game_mode;
        bool _ready;
        int _turn;
        int _moves;
        bool _ended;
        int _total;
        std::string _solution;
        std::string _troll;
        std::string _aux;
        std::string _actual_state;
        std::vector<int> _points;
    public:
        /**
         * La idea del parámetro troll_mode es que se puedan resolver algunos refranes con la
         * típica muletilla "patada en los cojones". Con esto se pretende hacer el juego más
         * ameno y entretenido, en ningún caso ofender a nadie.
         */
        Game(const type &game_mode, const bool &troll_mode = true): _game_mode(game_mode), _turn(0), _moves(0), _ended(false), _ready(false){
            if(game_mode == SINGLEPLAYER){
                _total = 1;
//                _players = std::vector<Player>(1);
            }
            else{
                _total = 3;
//                _players = std::vector<Player>(3);
                _points = {0, 0, 0};
            }
            std::vector<std::string> aux =get_random_refran();
            _aux = _solution = aux[0];
            _troll = troll_mode? aux[1] : aux[0];
            _actual_state = convert_solution(_solution);
        }
        inline std::vector<Player> get_players() const { return _players; }
        inline void set_turn(const int &turn) { _turn = turn; }
        inline std::string get_solution() const { return _solution; }
        inline std::string get_solution_troll() const { return _troll; }
        inline std::string get_actual_state() const { return _actual_state; }
        inline std::string get_aux() const { return _aux; }
        inline type get_gm() const { return _game_mode; }
        inline int get_turn() const { return _turn; }
        inline int get_total() const { return _total; }
        inline int get_moves() const { return _moves; }
        inline bool is_ended() const { return _ended; }
        inline bool is_turn(const int &socket) { return _players[_turn].get_socket() == socket; }
        inline bool can_afford() { return _game_mode == MULTIPLAYER ? _points[_turn] >= 50 : true; }
        inline void end_game() { _ended = true; }
        inline int get_points() const { return _game_mode == SINGLEPLAYER? _players[0].get_score() : _points[_turn]; }
        inline Player curr_player() { return _players[_turn]; }
        void del_player(){
            if(_game_mode == MULTIPLAYER){
                _players.erase(_players.begin() + _turn);
                _total--;
            }
        }
        /**
         * When to call this function?
         * Every time inside multiplayers game and at the end of singleplayer games cuz in sp it will end the game
         */
        void add_points(){
            assert(this->is_ready() && !this->is_ended());
            if(_game_mode == MULTIPLAYER){
                _points[_turn] += 50;
            }
            else{
                switch (_moves){
                    case 0: case 1: case 2: case 3: case 4:
                        _players[0].add_score(150);
                        _players[0].add_win();
                        break;
                    case 5: case 6: case 7: case 8:
                        _players[0].add_score(100);
                        _players[0].add_win();
                        break;
                    case 9: case 10: case 11:
                        _players[0].add_score(70);
                        _players[0].add_win();
                        break;
                    case 12: case 13: case 14: case 15:
                        _players[0].add_score(50);
                        _players[0].add_win();
                        break;
                    default:
                        _players[0].add_lose();
                }
                _players[0].commit();
                this->end_game();
            }
        }
        /**
         * Function that updates the turn.
         */
        void next_turn(){
            assert(this->is_ready() && !this->is_ended());
            if(_game_mode == MULTIPLAYER)
                this->set_turn((this->get_turn()+1)%_total);
        }
        /**
         * Function that checks if the letter is in the solution and
         * administrates the points.
         */
        bool check_letter(const char &l){
            assert(this->is_ready() && !this->is_ended());
            // If the actual state is the solution, we shouldn't be here
            assert(_actual_state != _solution);
            bool found = false;
            std::size_t pos;//recorrer cadenas
            type gm = this->get_gm();//obtener modo de juego
            _moves++;
            if(_game_mode == MULTIPLAYER && is_vowel(l))
                _points[this->get_turn()] -= 50;
            do{
                pos = _aux.find(l);
                if(pos!=std::string::npos){
                    found = true;
                    // When multiplayer mode and consonant, we add 50 points
                    if(gm == MULTIPLAYER && !is_vowel(_aux[pos]))
                        this->add_points();
                    // Now we update the auxiliar string so we won't find the letter again in the string
                    _aux[pos] = '_';
                    _actual_state[pos] = l;
                }
            }while(pos!=std::string::npos);
            return found;
        }
        // TODO: Añadir puntos al ganar
        inline bool solve(const std::string &cadena) { return cadena == this->get_solution() || cadena == this->get_solution_troll(); }

        inline bool is_ready() const { return _players.size() == _total; }
        // The user of the class must check if it is ready before adding someone.
        void add_player(const Player &jugador){
            // Only add players when the game isn't ready
            assert(!this->is_ready());
            _players.push_back(jugador);
        }

    };
}

#endif //P2_PARTIDA_HPP
