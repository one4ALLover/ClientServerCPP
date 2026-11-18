#include <iostream>
#include <string>
#include <winsock2.h>

#include <ws2tcpip.h>

int main()
{
WSADATA wsaData;
    
    //инициализация Winsock, где параметр 1: версия Winsock (2.2), параметр 2: указатель на структуру WSADATA
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData); 
    
    //проверка на успешность инициализации Winsock
    if (result != 0)
    {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }
    // Если инициализация прошла успешно, выводим сообщение об успешной инициализации, а делее идет основной код программы
    std::cout << "WSAStartup succeeded." << std::endl;

    // Создание сокета
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    // Проверка на успешность создания сокета
    if (serverSocket == INVALID_SOCKET) 
    {
        std::cerr << "socket failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }
    std::cout << "Socket created successfully." << std::endl;

    // Настройка адреса сервера
    sockaddr_in serverAddress{}; // Структура для хранения адреса сервера

    serverAddress.sin_family = AF_INET; // Семейство адресов (IPv4)
    serverAddress.sin_port = htons(8080); // Порт сервера
    
    // Преобразование IP-адреса из текстового формата в бинарный
    int AdrResult = InetPtonA(serverAddress.sin_family, "127.0.0.1", &serverAddress.sin_addr); // Преобразование IP-адреса из текстового формата в бинарный

    // Проверка на успешность преобразования IP-адреса
    if (AdrResult == 0)
    {
        std::cerr << "Invalid IP address format." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    else if (AdrResult < 0)
    {
        std::cerr << "InetPton failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Привязка сокета к адресу
    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR)
    {
        std::cerr << "bind failed with: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "Socket bound to address successfully." << std::endl;

    // Прослушивание входящих соединений
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cerr << "listen failed with: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server is listening for incoming connections..." << std::endl;

    // Ожидание входящего соединения
    SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);

    if (clientSocket == INVALID_SOCKET)
    {
        std::cerr << "accept failed with: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Client connected successfully." << std::endl;
 
    char mesBuf[1024] = {0}; // Буфер для приема данных
    std::string output; // Строка для хранения полученного сообщения
    
    while(true)
    {
    
    int bytesReceived = recv(clientSocket, mesBuf, sizeof(mesBuf), 0); //число полученных байт
    
    //ошибка сокета
    if (bytesReceived == SOCKET_ERROR)
    {
        std::cerr << "recv failed with: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    //отключение клиента
    else if (bytesReceived == 0)
    {
        std::cout << "Client disconnected." << std::endl;
        break;
    }

    //основной цикл сообщения
    else
    {
        for (int i = 0; i < bytesReceived; i++) 
        {
            if (mesBuf[i] != '\n')
            {
                output += mesBuf[i];
            } 
            
            else 
            {
                std::cout << "Received message from client: " << output << '\n';
                output.clear();
            }
        }
    }
}

    if (!output.empty())
        {
            std::cerr << "Warning: incomplete message received." << std::endl;
        }
    
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
