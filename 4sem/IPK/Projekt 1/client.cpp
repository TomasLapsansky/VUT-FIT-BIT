/*
 * Tomas Lapsansky (xlapsa00)
 * IPK Projekt 1 - Komunikacia client<=>server
 * 2017/2018
 */

#include <iostream>

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <string.h>
#include <strings.h>

using namespace std;

#define ERROR (-1)
#define BUFSIZE 1024

int main(int argc, char* argv[]) {

    bool arg_n = false;
    bool arg_f = false;
    bool arg_l = false;

    int socket_id, port_number;
    const char *server_hostname;
    struct hostent *server;
    struct sockaddr_in server_address;
    char buf[BUFSIZE];

    string send_message;    //sprava ktora sa posle na server
    const char *login;

    // ./client -h [server_name] -p [port] [-n|-f|-l] login
    if((argc != 7) && (argc != 6)) {
        cerr << "Wrong number of parameters" << endl;
        cerr << "./client -h [server_name] -p [port] [-n|-f|-l] login" << endl;
        exit(ERROR);
    } else {

        if(strcmp(argv[1], "-h") != 0) {
            cerr << "Missing parameter -h" << endl;
            cerr << "./client -h [server_name] -p [port] [-n|-f|-l] login" << endl;
            exit(ERROR);
        }
        if(strcmp(argv[3], "-p") != 0) {
            cerr << "Missing parameter -p" << endl;
            cerr << "./client -h [server_name] -p [port] [-n|-f|-l] login" << endl;
            exit(ERROR);
        }

        if(strcmp(argv[5], "-n") == 0) {
            arg_n = true;
            if(argc != 7) {
                cerr << "Wrong number of parameters" << endl;
                exit(ERROR);
            }
        } else if(strcmp(argv[5], "-f") == 0) {
            arg_f = true;
            if(argc != 7) {
                cerr << "Wrong number of parameters" << endl;
                exit(ERROR);
            }
        } else if(strcmp(argv[5], "-l") == 0) {
            arg_l = true;
        } else {
            cerr << "Missing parameter [-n|-f|-l]" << endl;
            cerr << "./client -h [server_name] -p [port] [-n|-f|-l] login" << endl;
            exit(ERROR);
        }
    }
    server_hostname = argv[2];
    port_number = atoi(argv[4]);
    if(argc == 7)
        login = argv[6];
    else
        login = "";

    // DNS
    if((server = gethostbyname(server_hostname)) == NULL) {
        cerr << "No such host as " << server_hostname << endl;
        exit(ERROR);
    }

    // Creation of socket
    if((socket_id = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {

        cerr << "Error in creation of socket" << endl;
        exit(ERROR);
    }

    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    bcopy(server->h_addr, (char *)&server_address.sin_addr.s_addr, (size_t)server->h_length);
    server_address.sin_port = htons(port_number);

    if(connect(socket_id, (struct sockaddr *) &server_address, sizeof(server_address)) != 0) {
        cerr << "Connection error" << endl;
        exit(ERROR);
    }

    /*
     * Sprava bude v tvare:
     * [n|f|l]
     * xlogin00
     */
    if(arg_n)
        send_message.append("n\n");
    else if(arg_f)
        send_message.append("f\n");
    else if(arg_l)
        send_message.append("l\n");
    send_message.append(login);

    if(send(socket_id, send_message.c_str(), send_message.length(), 0) < 0) {
        cerr << "Sending error" << endl;
        exit(ERROR);
    }

    int receive = 0;
    string message;

    while ((receive = recv(socket_id, buf, sizeof(buf), 0)) > 0) {

        if (receive < 0) {
            cerr << "Error: receive" << endl;
            exit(EXIT_FAILURE);
        }

        if(strcmp("<<ERROR>>", buf) == 0) {
            cerr << "ERROR : Login not found" << endl;
            close(socket_id);
            return 0;
        } else
            message.append(buf);

        bzero(buf, sizeof(buf));

        send(socket_id, "received", sizeof("received"), 0);

        if (receive < BUFSIZE - 1) {
            break;
        }

    }

    cout << message << endl;

    close(socket_id);
    return 0;

}
