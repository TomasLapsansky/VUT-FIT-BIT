/*
 * Tomas Lapsansky (xlapsa00)
 * IPK Projekt 1 - Komunikacia client<=>server
 * 2017/2018
 */

#include <iostream>
#include <string>

#include <sys/socket.h>
#include <sys/types.h>
#include <pwd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>

#include <string.h>
#include <strings.h>

#define ERROR (-1)
#define BUFSIZE 1024

using namespace std;

string processing(string instruction, string login);

int main(int argc, char* argv[]) {

    int rc;
    int server_socket;
    struct sockaddr_in sa;
    struct sockaddr_in sa_client;
    char str[INET_ADDRSTRLEN];

    int port_number;

    char receive_buff[BUFSIZE];
    string message = "";
    string instruction;
    string login;

    // ./server -p [port]
    if(argc != 3) {
        cerr << "Usage ./server -p [port]" << endl;
        exit(ERROR);
    } else {

        if(strcmp(argv[1], "-p") != 0) {
            cerr << "Usage ./server -p [port]" << endl;
            exit(ERROR);
        }
    }

    port_number = atoi(argv[2]);

    socklen_t sa_client_len = sizeof(sa_client);
    if((server_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        cerr << "Socket creation error" << endl;
        exit(ERROR);
    }

    memset(&sa,0,sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    sa.sin_port = htons(port_number);

    if ((rc = bind(server_socket, (struct sockaddr*)&sa, sizeof(sa))) < 0)
    {
        perror("ERROR: bind");
        exit(ERROR);
    }

    if ((listen(server_socket, 1)) < 0)
    {
        perror("ERROR: listen");
        exit(ERROR);
    }

    while(1)
    {
        int comm_socket = accept(server_socket, (struct sockaddr*)&sa_client, &sa_client_len);

        //?TODO fork -- asi nie je potrebne
        if (comm_socket > 0) {

            //pomocny vypis
            if(inet_ntop(AF_INET, &sa_client.sin_addr, str, sizeof(str))) {
                printf("INFO: New connection:\n");
                printf("INFO: Client address is %s\n", str);
                printf("INFO: Client port is %d\n", ntohs(sa_client.sin_port));
            }

            int res = 0;
            while (1)
            {
                res = recv(comm_socket, receive_buff, sizeof(receive_buff),0);

                if (res <= 0)
                    break;

                //verifikacia prijatej spravy
                istringstream receive_message(receive_buff);

                getline(receive_message, instruction);
                getline(receive_message, login);

                message = processing(instruction, login);

                if(message.length() > BUFSIZE) {

                    string sub_message;
                    while (message.length() > BUFSIZE - 1) {

                        sub_message = message.substr(0, BUFSIZE);
                        message = message.substr(BUFSIZE);

                        send(comm_socket, sub_message.c_str(), sub_message.length(), 0);
                        recv(comm_socket, receive_buff, sizeof(receive_buff), 0);
                    }

                    send(comm_socket, message.c_str(), message.length(), 0);
                    recv(comm_socket, receive_buff, sizeof(receive_buff), 0);

                } else {
                    send(comm_socket, message.c_str(), message.length(), 0);
                    recv(comm_socket, receive_buff, sizeof(receive_buff), 0);
                }

                bzero(receive_buff, BUFSIZE);
            }
        }
        else
        {
            printf(".");
        }
        printf("Connection to %s closed\n",str);
        close(comm_socket);
    }
}

/*
 * funkcia rozpoznava instrukcny argument a spracovava spravu
 */
string processing(string instruction, string login) {

    struct passwd *pwd_login;
    string message;
    string founded_login;
    
    bool found = false;

    setpwent();
    while((pwd_login = getpwent()) != NULL) {

        if (instruction.compare("l") == 0) {

            if(login == "") {
                found = true;
                message.append(pwd_login->pw_name);
                message.append("\n");
            } else {

                founded_login = pwd_login->pw_name;
                if(login.compare(founded_login.substr(0,login.length())) == 0) {
                    found = true;
                    message.append(pwd_login->pw_name);
                    message.append("\n");
                }

            }

        } else if(login.compare(pwd_login->pw_name) == 0) {
            
            found = true;

            //rozlisenie parametrov
            if (instruction.compare("n") == 0) {

                message = pwd_login->pw_gecos;

            } else if (instruction.compare("f") == 0) {

                message = pwd_login->pw_dir;

            } else {
                return "<<ERROR>>";
            }
        }
    }
    
    if(!found)      //error message v pripade nenajdenia loginu
        message = "<<ERROR>>";

    return message;
}
