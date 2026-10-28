#include <iostream>

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
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    // Проверка на успешность создания сокета
    if (clientSocket == INVALID_SOCKET) 
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
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0)
    {
        std::cerr << "inet_pton failed." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR)
    {
        std::cerr << "connect failed with error: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

     closesocket(clientSocket);
     WSACleanup();
     return 0;
}