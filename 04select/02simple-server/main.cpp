#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

constexpr uint32_t Backlog = 10;

// simple server
namespace ss {
    class SimpleServer {
    public:
        SimpleServer(int fd, int port) : m_sock_fd(fd), m_port(port) {
            m_addr.sin_family = AF_INET;
            m_addr.sin_port = htons(port);
            m_addr.sin_addr.s_addr = INADDR_ANY;
        }

        bool _bind() const {
            if (bind(m_sock_fd, (sockaddr*)&m_addr,sizeof(m_addr)) < 0) {
                perror("bind");
                return false;
            }
            return true;
        }

        bool _listen(int backlog) const {
            if (listen(m_sock_fd,backlog) < 0) {
                perror("listen");
                return false;
            }
            return true;
        }

        int run() {
            cout << "listening at: " << inet_ntoa(m_addr.sin_addr) << ":" << m_port << endl;
            socklen_t addr_len;
            while(1) {
                int client_fd = accept(m_sock_fd, (sockaddr*)&m_addr, &addr_len);
                if (client_fd < 0) {
                    perror("accept");
                    return -1;
                }
                // ...

            }

            return 0;
        }

    private:
        int m_sock_fd;
        int m_port;
        sockaddr_in m_addr;
    };
}

int main() {
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("socket");
        return -1;
    }

    ss::SimpleServer s(sock_fd, 8000);
    if (!s._bind()) {
        cout << "bind error" << endl;
        return -1;
    }
    if (! s._listen(Backlog)) {
        cout << "listen error" << endl;
        return -1;
    }

    s.run();

    return 0;
}
