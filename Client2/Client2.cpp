#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h> // Для функции InetPton

#pragma comment(lib, "ws2_32.lib")

#define MAX_BUFFER_SIZE 1024

#include <sstream>

template <typename T>
std::string NumberToString(T Number)
{
    std::ostringstream ss;
    ss << Number;
    return ss.str();
}
int main()
{
    setlocale(LC_ALL, "Russian");
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "Ошибка при инициализации Winsock!" << std::endl;
        return -1;
    }

    // Создание сокета
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cerr << "Ошибка при создании сокета!" << std::endl;
        WSACleanup();
        return -1;
    }

    // Установка параметров адреса сервера
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8888);

    // Преобразование IP-адреса с помощью InetPton
    if (InetPtonA(AF_INET, "127.0.0.1", &(serverAddress.sin_addr)) <= 0)
    {
        std::cerr << "Некорректный IP-адрес!" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    // Подключение к серверу
    if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR)
    {
        std::cerr << "Ошибка при подключении к серверу!" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Подключение к серверу установлено." << std::endl;

    // Буфер для приема данных от сервера
    char buffer[MAX_BUFFER_SIZE] = { 0 };

    // Получение сообщения от сервера
    recv(clientSocket, buffer, MAX_BUFFER_SIZE, 0);
    std::cout << "Сообщение от сервера: " << buffer << std::endl;

    // Ввод числа
    std::cout << "Введите число от 0 до 100: ";
    int guess;
    std::cin >> guess;

    // Отправка числа на сервер
    std::string message = NumberToString<int>(guess);
    if (send(clientSocket, message.c_str(), message.length(), 0) == SOCKET_ERROR)

        // Получение результата от сервера
        memset(buffer, 0, sizeof(buffer));
   int a = recv(clientSocket, buffer, MAX_BUFFER_SIZE, 0);
   buffer[a] = '\0';

    // Вывод результата
    std::cout << "Результат: " << buffer << std::endl;

    // Закрытие сокета и завершение программы
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
