#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <cstring>
#include <vector>
#include <unistd.h>
using namespace std;

constexpr int Backlog = 20;
constexpr int NumFd = 1024;

namespace Select {
    class Server {
    public:
        explicit Server(const int port) : _sock(-1), _port(port) {}

        void init() {
            _sock = _socket();
            _bind();
            _listen();
        }

        void run() {
            // 要读的fd数组，全部初始化为-1
            std::vector<int> readfds(NumFd, -1);
            readfds[0] = _sock;
            int max_fd = 0;
            fd_set set;
            while(1) {
                FD_ZERO(&set);
                for (int i = 0; i < NumFd; ++i) {
                    if (readfds[i] != -1) {
                        FD_SET(readfds[i], &set);
                        if (max_fd < readfds[i]) {
                            max_fd = readfds[i];
                        }
                    }
                }

                switch (select(max_fd+1, &set, nullptr, nullptr,nullptr)) {
                    case 0:
                        cout << "timeout" << endl;
                        break;
                    case -1:
                        perror("select");
                        break;
                    default:
                        handle(readfds, set);
                        break;
                }
            }
        }

        void add_fd(vector<int>& fds, int fd) {
            for (int i = 0; i < fds.size(); ++i) {
                if (fds[i] == -1) {
                    fds[i] = fd;
                    return;
                }
            }
        }

        void handle(vector<int>& readfds, fd_set& set) {
            for (int i = 0; i < readfds.size(); ++i) {
                if (readfds[i] == -1) {
                    return;
                }

                if (FD_ISSET(readfds[i], &set)) { // 有可读的fd
                    cout << "cur ready: " << readfds[i] << endl;
                    if (readfds[i] == _sock) { // 有新连接
                        struct sockaddr addr;
                        socklen_t len;
                        int new_fd = accept(_sock, &addr, &len);
                        cout << "new fd: " << new_fd << endl;
                        add_fd(readfds, new_fd);
                    }
                    else {
                        char buf[1024]{};
                        int sz = recv(readfds[i], buf, sizeof(buf), MSG_DONTWAIT);
                        switch(sz) {
                            case -1: {
                                perror("recv");
                                break;
                            }
                            case 0: {
                                readfds[i] = -1;
                                cout << "peer closed" << endl;
                                break;
                            }
                            default: {
                                buf[sz] = '\0';
                                cout << "<<<<<<<<<<["<< readfds[i] << "]: " << buf << endl;
                                break;
                            }
                        }
                    }
                }
            }
        }

    private:
        int _socket() {
            int fd = socket(AF_INET, SOCK_STREAM, 0);
            if (fd < 0) {
                perror("socket");
                exit(-1);
            }
            return fd;
        }

        void _bind() {
            struct sockaddr_in addr{};
            addr.sin_family = AF_INET;
            addr.sin_port = htons(_port);
            addr.sin_addr.s_addr = INADDR_ANY;
            if (bind(_sock,(struct sockaddr*)&addr, sizeof(addr)) < 0) {
                perror("bind");
                exit(-1);
            }
        }

        void _listen() {
            if (listen(_sock, Backlog) < 0) {
                perror("listen");
                exit(-1);
            }
        }

        int _sock;
        int _port;
    };
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "usage: server <Port>" << endl;
        return -1;
    }
    int port = atoi(argv[1]);
    Select::Server server(port);
    server.init();
    server.run();


    return 0;
}
