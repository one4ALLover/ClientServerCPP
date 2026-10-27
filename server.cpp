#include <iostream>

#include <winsock2.h>

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

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}