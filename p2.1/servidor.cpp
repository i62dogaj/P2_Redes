#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <csignal>
#include <string>
#include <regex.h>
#include <algorithm>
#include <cstring>
#include <ctime>
#include <vector>
#include <map>
#include <unistd.h> // close()
#include "servidor.hpp"
#include "jugador.hpp"
#include "partida.hpp"
#include "extra_functions.hpp"

bool salir = false;

void manejador(int sigsum){
    std::cout << std::endl << "Saliendo..." << std::endl;
    salir = true;
    signal(SIGINT, manejador);
}

int main(){
    srand(time(NULL));
    int sock, nuevo_socket;
    int salida;
    sockaddr_in servidor, cliente;
    bool encontrado = false;
    char buffer[250];
    std::regex e;
    std::vector<r::Game> partidas;
    std::vector<r::Player> p_buffer;
    std::map<int, int> mapa_aux;
    std::string buffer_string;
    fd_set read_fds, aux_fds;
    int on, ret;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        std::cerr << "No se puede abrir el socket cliente" << std::endl;
        exit(-1);
    }

    on = 1;
    ret = setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));//no bloquear puerto

    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(2050);
    servidor.sin_addr.s_addr = htonl(INADDR_ANY);

    if( bind(sock, (sockaddr *) &servidor, sizeof(servidor)) == -1 ){//reservar el puerto 2050
        std::cerr << "No se pudo hacer bind" << std::endl;
        close(sock);
        exit(-1);
    }

    socklen_t cliente_len = sizeof(cliente);

    if(listen(sock, 1) == -1){//escuchar por el puerto 2050
        std::cerr << "No se pudo hacer listen" << std::endl;
        close(sock);
        exit(-1);
    }

    FD_ZERO(&read_fds);
    FD_ZERO(&aux_fds);
    FD_SET(sock, &read_fds);
    FD_SET(0, &read_fds);

    signal(SIGINT, manejador);
    while(true){
        if(salir){
            for(auto it = p_buffer.begin() ; it != p_buffer.end() ; ++it){
                send(it->get_socket(), "Desconexión del servidor", strlen("Desconexión del servidor"), 0);
                close(it->get_socket());
                FD_CLR(it->get_socket(), &read_fds);

            }
            close(sock);
            exit(EXIT_SUCCESS);
        }
        aux_fds = read_fds;
        salida = select(FD_SETSIZE, &aux_fds, NULL, NULL, NULL);//esperar
        if(salida > 0){
            for(int i = 0 ; i < FD_SETSIZE ; ++i){
                if(FD_ISSET(i, &aux_fds)){
                    if(i == sock){
                        if((nuevo_socket = accept(sock, (sockaddr *) &cliente, &cliente_len)) == -1 ){
                            std::cerr << "Error aceptando peticiones" << std::endl;
                        }
                        else{
                            if(p_buffer.size() < MAX_CLIENTES){
                                r::Player p;
                                p.set_socket(nuevo_socket);
                                p_buffer.push_back(p);
                                FD_SET(nuevo_socket, &read_fds);
                                send(nuevo_socket, "+Ok. Usuario conectado.\n", strlen("+Ok. Usuario conectado.\n"), 0);
                            }
                            else{
                                send(nuevo_socket, "-Err. Demasiados clientes conectados.\n", strlen("-Err. Demasiados clientes conectados.\n"), 0);
                                close(nuevo_socket);
                            }
                        }
                    }
                    else if(i == 0){ // Leemos de teclado.
                        std::getline(std::cin, buffer_string);
                        if( buffer_string == "SALIR" ){
                            salir = true;
                        }
                    }
                    else{
                        bzero(buffer, sizeof(buffer));
                        if( recv(i, buffer, sizeof(buffer), 0) > 0 ){
                            std::string s(buffer);
                            int peticion = manejar_peticion(s);
                            if(s.empty()) peticion = 9;
                            switch(peticion){
                                case 1:{
                                     e = "^USUARIO ([[:alnum:]]+)$";
                                    std::string nombre = std::regex_replace (s,e,"$1",std::regex_constants::format_no_copy);
                                    if(check1(i, p_buffer)){
                                        send(i, "-Err. Usted ya está logueado.\n", strlen("-Err. Usted ya está logueado.\n"), 0);
                                    }
                                    else{
                                        if(sql_exists(nombre)){
                                            send(i, "+Ok. Usuario correcto.\n", strlen("+Ok. Usuario correcto.\n"), 0);
                                            for(auto it = p_buffer.begin() ; it != p_buffer.end() ; ++it){
                                                if(it->get_socket() == i)
                                                    it->set_username(nombre);
                                            }
                                        }
                                        else{
                                            send(i, "-Err. Usuario incorrecto.\n", strlen("-Err. Usuario incorrecto.\n"), 0);
                                        }
                                    }
                                    break;}
                                case 2:{
                                    e = "^PASSWORD ([[:alnum:]]+)$";
                                    std::string passw = std::regex_replace (s,e,"$1",std::regex_constants::format_no_copy);
                                    if(check2(i, p_buffer)){
                                        send(i, "-Err. Usted ya está logueado.\n", strlen("-Err. Usted ya está logueado.\n"), 0);
                                    }
                                    else if(not check1(i, p_buffer)){
                                        send(i, "-Err. Debe de haber validado su usuario antes de introducir la contraseña.\n", strlen("-Err. Debe de haber validado su usuario antes de introducir la contraseña.\n"), 0);
                                    }
                                    else{
                                        for(auto it = p_buffer.begin() ; it != p_buffer.end() ; ++it){
                                            if(it->get_socket() == i){
                                                if(it->get_password().empty() && sql_exists(it->get_username(), passw)){
                                                    it->load_user(it->get_username());
                                                    encontrado = true;
                                                    send(i,"+Ok. Usuario validado.\n",strlen("+Ok. Usuario validado.\n"),0);
                                                }
                                            }
                                        }
                                        if(!encontrado)
                                            send(i, "-Err. Error en la validación.\n", strlen("-Err. Error en la validación.\n"), 0);
                                    }
                                    break;}
                                case 3:{
                                    e = "^REGISTRO -u ([[:alnum:]]+) -p ([[:alnum:]]+)$";
                                    std::string nombre = std::regex_replace (s,e,"$1",std::regex_constants::format_no_copy);
                                    std::string passw = std::regex_replace (s,e,"$2",std::regex_constants::format_no_copy);
                                    if(check2(i, p_buffer)){
                                        send(i, "-Err. Usted ya está logueado.\n", strlen("-Err. Usted ya está logueado.\n"), 0);
                                    }
                                    else if(sql_exists(nombre)){
                                        send(i, "-Err. El usuario ya existe. Utilice otro nombre.\n", strlen("-Err. El usuario ya existe. Utilice otro nombre.\n"), 0);
                                    }
                                    else{
                                        for(auto it = p_buffer.begin() ; it != p_buffer.end() ; ++it){
                                            if(it->get_socket() == i){
                                                it->create_user(nombre, passw);
                                                send(i,"+Ok. Usuario registrado correctamente.\n",strlen("+Ok. Usuario registrado correctamente.\n"),0);
                                            }
                                        }
                                    }
                                    break;}
                                case 4:{//partida individual
                                    if(check2(i, p_buffer) && not mapa_aux[i]){
                                        r::Game tmp_g(SINGLEPLAYER);
                                        for(auto it = p_buffer.begin() ; it != p_buffer.end() ; ++it){
                                            if(it->get_socket() == i){
                                                tmp_g.add_player(*it);
                                                partidas.push_back(tmp_g);
                                                mapa_aux[i] = partidas.size();
                                                send(i, (tmp_g.get_actual_state()+"\n").c_str(), (tmp_g.get_actual_state()+"\nS").size(), 0);
                                            }
                                        }
                                    }
                                    else{
                                        send(i , "-Err. Asegúrate que estás logueado y no estás en partida.\n", strlen("-Err. Asegúrate que estás logueado y no estás en partida.\n"), 0);
                                    }
                                    break;}
                                case 5:{//multiplayer
                                    if(check2(i, p_buffer) && not mapa_aux[i]){//comprobar logueo
                                        for(int j = 0 ; j < partidas.size() ; ++j){//recorremos una partida multijugador no empezada
                                            if(!partidas[j].is_ready()){
                                                for(auto it = p_buffer.begin() ; it != p_buffer.end() ; ++it){
                                                    if(it->get_socket() == i){
                                                        partidas[j].add_player(*it);//se añaden en orden
                                                        mapa_aux[i] = j+1;
                                                        std::string msg = "+Ok. Unido a partida multijugador existente. Cuando haya 3 jugadores se le avisará y podrá empezar.\n";
                                                        send(i, msg.c_str(), msg.size(), 0);
                                                        break;
                                                    }
                                                }
                                                break;
                                            }
                                        }
                                        if(mapa_aux[i]){//cuando entra una tercera persona
                                            if(partidas[mapa_aux[i]-1].is_ready()){
                                                for(auto it = mapa_aux.begin() ; it != mapa_aux.end() ; ++it){
                                                    if(it->second == mapa_aux[i]){
                                                        send(it->first, "+Ok. Partida multijugador iniciada. Espere su turno por favor.\n", strlen("+Ok. Partida multijugador iniciada. Espere su turno por favor\n"), 0);
                                                        usleep(3000);
                                                        send(it->first, (partidas[mapa_aux[i]-1].get_actual_state() + "\n").c_str(), (partidas[mapa_aux[i]-1].get_actual_state() + "\n").size(), 0);
                                                    }
                                                }
                                                send(partidas[mapa_aux[i]-1].curr_player().get_socket(), "+Ok. Es su turno, puede comenzar\n", strlen("+Ok. Es su turno, puede comenzar\n"), 0);
                                                //avisamos a jugador numero 1 de que le toca
                                            }
                                        }
                                        else{//todabia no existe ninguna partida, se va crear
                                            r::Game tmp_g(MULTIPLAYER);
                                            for(auto it = p_buffer.begin() ; it != p_buffer.end() ; ++it) {
                                                if (it->get_socket() == i) {
                                                    tmp_g.add_player(*it);
                                                    partidas.push_back(tmp_g);
                                                    mapa_aux[i] = partidas.size();
                                                    send(i, "+Ok. Partida multijugador creada. Cuando esté lista para ser jugada será avisado.\n", strlen("+Ok. Partida multijugador creada. Cuando esté lista para ser jugada será avisado.\n"), 0);
                                                }
                                            }
                                        }
                                    }
                                    else{
                                        send(i , "-Err. Asegúrate que estás logueado y no estás en partida.\n", strlen("-Err. Asegúrate que estás logueado y no estás en partida.\n"), 0);
                                    }
                                    break;}
                                case 6:{//consonante
                                    if(check2(i, p_buffer) && mapa_aux[i]){
                                        std::regex e ("^CONSONANTE ([b-df-hj-np-tv-z])$");
                                        char consonante = std::regex_replace (s,e,"$1",std::regex_constants::format_no_copy)[0];
                                        if(!partidas[mapa_aux[i]-1].is_turn(i)){
                                            send(i, "-Err. Debe esperar su turno.\n", strlen("-Err. Debe esperar su turno.\n"), 0);
                                        }
                                        else if(partidas[mapa_aux[i]-1].get_actual_state() == partidas[mapa_aux[i]-1].get_solution()){
                                            send(i, "¡Ya no hay más chicha, resuelve con el comando RESOLVER solución!\n", strlen("¡Ya no hay más chicha, resuelve con el comando RESOLVER solución!\n"), 0);
                                        }
                                        else{
                                            if(partidas[mapa_aux[i]-1].check_letter(consonante)){
                                                send(i, "+Ok. Existe la consonante.\n", strlen("+Ok. Existe la consonante.\n"), 0);
                                            }
                                            else{
                                                send(i, "+Ok. No existe la consonante.\n", strlen("+Ok. No existe la consonante.\n"), 0);
                                                partidas[mapa_aux[i]-1].next_turn();
                                                if(partidas[mapa_aux[i]-1].get_gm() == MULTIPLAYER and partidas[mapa_aux[i]-1].get_total() > 1)
                                                    send(partidas[mapa_aux[i]-1].curr_player().get_socket(), "+Ok. Es su turno, puede comenzar\n", strlen("+Ok. Es su turno, puede comenzar\n"), 0);
                                            }

                                            std::string state = partidas[mapa_aux[i]-1].get_actual_state() + "\n";//otros usuarios reciben el estado actual del juego
                                            for(auto it = p_buffer.begin() ; it != p_buffer.end() ; ++it){
                                                if(mapa_aux[it->get_socket()] == mapa_aux[i])
                                                    send(it->get_socket(), state.c_str(), state.size(), 0);
                                            }
                                        }
                                    }
                                    else{
                                        send(i , "-Err. Asegúrate que estás logueado y estás en partida.\n", strlen("-Err. Asegúrate que estás logueado y estás en partida.\n"), 0);
                                    }
                                    break;}
                                case 7:{//vocal
                                    if(check2(i, p_buffer) && mapa_aux[i]){
                                        std::regex e ("^VOCAL ([aeiou])$");
                                        char vocal = std::regex_replace (s,e,"$1",std::regex_constants::format_no_copy)[0];
                                        if(!partidas[mapa_aux[i]-1].is_turn(i)){
                                            send(i, "-Err. Debe esperar su turno.\n", strlen("-Err. Debe esperar su turno.\n"), 0);
                                        }
                                        else if(partidas[mapa_aux[i]-1].get_actual_state() == partidas[mapa_aux[i]-1].get_solution()){
                                            send(i, "¡Ya no hay más chicha, resuelve con el comando RESOLVER solción!\n", strlen("¡Ya no hay más chicha, resuelve con el comando RESOLVER solción!\n"), 0);
                                        }
                                        else if(!partidas[mapa_aux[i]-1].can_afford()){//permite siempre en singlePlayer y en multiplayer comprueba que tengas 50 puntos
                                            send(i, "-Err. Necesitas al menos 50 puntos para solicitar una vocal.", strlen("-Err. Necesitas al menos 50 puntos para solicitar una vocal."), 0);
                                        }
                                        else{
                                            if(partidas[mapa_aux[i]-1].check_letter(vocal)){
                                                send(i, "+Ok. Existe la vocal.\n", strlen("+Ok. Existe la vocal.\n"), 0);
                                            }
                                            else{
                                                send(i, "+Ok. No existe la vocal.\n", strlen("+Ok. No existe la vocal.\n"), 0);
                                                partidas[mapa_aux[i]-1].next_turn();
                                                if(partidas[mapa_aux[i]-1].get_gm() == MULTIPLAYER)
                                                    send(partidas[mapa_aux[i]-1].curr_player().get_socket(), "+Ok. Es su turno, puede comenzar\n", strlen("+Ok. Es su turno, puede comenzar\n"), 0);
                                            }

                                            std::string state = partidas[mapa_aux[i]-1].get_actual_state() + "\n";
                                            for(auto it = p_buffer.begin() ; it != p_buffer.end() ; ++it){
                                                if(mapa_aux[it->get_socket()] == mapa_aux[i])
                                                    send(it->get_socket(), state.c_str(), state.size(), 0);
                                            }
                                        }
                                    }
                                    else{
                                        send(i , "-Err. Asegúrate que estás logueado y estás en partida.\n", strlen("-Err. Asegúrate que estás logueado y estás en partida.\n"), 0);
                                    }
                                    break;}
                                case 8:{//Resolver
                                    if(check2(i, p_buffer) && mapa_aux[i]){
                                        std::regex e ("^RESOLVER (.+)$");
                                        std::string sol = std::regex_replace (s,e,"$1",std::regex_constants::format_no_copy);
                                        if(!partidas[mapa_aux[i]-1].is_turn(i)){
                                            send(i, "-Err. Debe esperar su turno.\n", strlen("-Err. Debe esperar su turno.\n"), 0);
                                        }
                                        else{
                                            if(partidas[mapa_aux[i]-1].solve(sol)){
                                                send(i, "+Ok. Enhorabuena.\n", strlen("+Ok. Enhorabuena.\n"), 0);
                                                usleep(300);

                                                if(partidas[mapa_aux[i]-1].get_gm() == SINGLEPLAYER){
                                                    partidas[mapa_aux[i]-1].add_points();
                                                    for(auto it = p_buffer.begin() ; it != p_buffer.end() ; ++it){
                                                        if(it->get_socket() == i){
                                                            *it = partidas[mapa_aux[i]-1].curr_player();
                                                            send(i, (it->get_stats()).c_str(), it->get_stats().size(), 0);
                                                        }
                                                    }
                                                }
                                                else{
                                                    for(auto it = p_buffer.begin() ; it != p_buffer.end() ; ++it){
                                                        if(mapa_aux[it->get_socket()] == mapa_aux[i] ){
                                                            if(it->get_socket() != i){
                                                                it->add_lose();
                                                                it->commit();
                                                            }
                                                            else{
                                                                it->add_win();
                                                                it->add_score(partidas[mapa_aux[i]-1].get_points());
                                                                it->commit();
                                                            }
                                                            send(it->get_socket(), (it->get_stats()).c_str(), it->get_stats().size(), 0);
                                                        }
                                                    }
                                                    std::map<int,int> mapa_tmp = mapa_aux;
                                                    for(auto it = mapa_tmp.begin() ; it != mapa_tmp.end() ; ++it){
                                                        if(it->second == mapa_aux[i] && it->first != i){
                                                            send(it->first , "Partida acabada, alguien intentó resolver y acertó.\n", strlen("Partida acabada, alguien intentó resolver y acertó.\n"), 0);
                                                            usleep(300);
                                                            send(it->first , "Inicia otra partida con PARTIDA-INDIVIDUAL o PARTIDA-GRUPO\n", strlen("Inicia otra partida con PARTIDA-INDIVIDUAL o PARTIDA-GRUPO\n"), 0);
                                                            mapa_aux.erase(it->first);
                                                        }
                                                    }
                                                }
                                                send(i, "Inicia otra partida con PARTIDA-INDIVIDUAL o PARTIDA-GRUPO\n", strlen("Inicia otra partida con PARTIDA-INDIVIDUAL o PARTIDA-GRUPO\n"), 0);
                                                partidas.erase(partidas.begin() + mapa_aux[i]);
                                                mapa_aux.erase(i);
                                            }
                                            else{
                                                send(i, "+Ok. Le deseamos mejor suerte la próxima vez.\n", strlen("+Ok. Le deseamos mejor suerte la próxima vez.\n"), 0);
                                                usleep(300);
                                                send(i, "Inicia otra partida con PARTIDA-INDIVIDUAL o PARTIDA-GRUPO\n", strlen("Inicia otra partida con PARTIDA-INDIVIDUAL o PARTIDA-GRUPO\n"), 0);
                                                if(partidas[mapa_aux[i]-1].get_gm() == MULTIPLAYER){
                                                    std::map<int,int> mapa_tmp = mapa_aux;
                                                    for(auto it = mapa_tmp.begin() ; it != mapa_tmp.end() ; ++it){
                                                        if(it->second == mapa_aux[i] && it->first != i){
                                                            send(it->first , "Partida acabada, alguien intentó resolver y falló.\n", strlen("Partida acabada, alguien intentó resolver y falló.\n"), 0);
                                                            usleep(300);
                                                            send(it->first , "Inicia otra partida con PARTIDA-INDIVIDUAL o PARTIDA-GRUPO\n", strlen("Inicia otra partida con PARTIDA-INDIVIDUAL o PARTIDA-GRUPO\n"), 0);
                                                            mapa_aux.erase(it->first);
                                                        }
                                                    }
                                                }
                                                partidas.erase(partidas.begin() + mapa_aux[i]);
                                                mapa_aux.erase(i);
                                            }
                                        }
                                    }
                                    else{
                                        send(i , "-Err. Asegúrate que estás logueado y estás en partida.\n", strlen("-Err. Asegúrate que estás logueado y estás en partida.\n"), 0);
                                    }
                                    break;}
                                case 9:{//Salir
                                    close(i);
                                    FD_CLR(i,&read_fds);
                                    std::map<int,int> mapa_tmp = mapa_aux;
                                    if(mapa_aux[i] and partidas[mapa_aux[i]-1].get_gm() == MULTIPLAYER){
                                        partidas[mapa_aux[i]-1].del_player();
                                        partidas[mapa_aux[i]-1].next_turn();
                                        send(partidas[mapa_aux[i]-1].curr_player().get_socket(), "+Ok. Es su turno, puede comenzar\n", strlen("+Ok. Es su turno, puede comenzar\n"), 0);
                                    }
                                    int pos;
                                    for(int j = 0 ; j < p_buffer.size() ; ++j){
                                        if(p_buffer[j].get_socket() == i)
                                            pos = j;
                                    }
                                    p_buffer.erase(p_buffer.begin()+pos);
                                    mapa_aux.erase(i);
                                }
                                default:
                                    send(i, "-Err.\n", strlen("-Err.\n"), 0);
                                    break;
                            }
                            fflush(stdout);
                        }
                        else{
                            close(i);
                            FD_CLR(i,&read_fds);
                            int pos;
                            for(int i = 0 ; i < p_buffer.size() ; ++i){
                                if(p_buffer[i].get_socket() == i)
                                    pos = i;
                            }
                            p_buffer.erase(p_buffer.begin()+pos);
                            mapa_aux.erase(i);
                        }
                    }
                }
            }
        }
    }
    close(sock);
    return 0;
}
