#include <iostream>
#include <string>
#include <fstream>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 3);
    
    cout << "서버 대기 중..." << endl;

    while (true) {
        int ns = accept(server_fd, NULL, NULL);
        char buf[1024];
        read(ns, buf, 1024);

        // 파일 이름 안 따지고 무조건 index.html 읽어서 전송
        ifstream f("index.html");
        string body((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());

        string res = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\n" + body;
        send(ns, res.c_str(), res.length(), 0);
        close(ns);
        cout << "전송 완료!" << endl;
    }
    return 0;
}
