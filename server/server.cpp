#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <cstdlib>
#include <ctime>

#pragma comment(lib, "ws2_32.lib")

#define MAX_BUFFER_SIZE 1024

int main()
{
    setlocale(LC_ALL, "Russian");
    srand(time(0));
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "Ошибка при инициализации Winsock!" << std::endl;
        return -1;
    }

    // Создание сокета
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET)
    {
        std::cerr << "Ошибка при создании сокета!" << std::endl;
        WSACleanup();
        return -1;
    }

    // Установка параметров адреса сервера
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8888);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Привязка сокета к адресу сервера
    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR)
    {
        std::cerr << "Ошибка при привязке сокета!" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    // Прослушивание подключений
    if (listen(serverSocket, 2) == SOCKET_ERROR)
    {
        std::cerr << "Ошибка при прослушивании сокета!" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Сервер запущен и ожидает подключений." << std::endl;

    // Принятие подключений от клиентов
    SOCKET clientSocket1 = accept(serverSocket, NULL, NULL);
    SOCKET clientSocket2 = accept(serverSocket, NULL, NULL);

    std::cout << "Подключение к двум клиентам установлено." << std::endl;

    // Генерация случайного числа
    int secretNumber = rand() % 101;
    for (int i = 0; i < rand() % 8; i++)
        secretNumber = (secretNumber * rand()) % 101;
    std::cout << "Сгенерированное число: " << secretNumber << std::endl;

    // Отправка сгенерированного числа обоим клиентам
    std::string message = "Угадайте число от 0 до 100:";
    send(clientSocket1, message.c_str(), message.length(), 0);
    send(clientSocket2, message.c_str(), message.length(), 0);

    // Буферы для приема данных от клиентов
    char buffer1[MAX_BUFFER_SIZE];
    char buffer2[MAX_BUFFER_SIZE];

    // Получение и сравнение чисел от клиентов
    recv(clientSocket1, buffer1, MAX_BUFFER_SIZE, 0);
    recv(clientSocket2, buffer2, MAX_BUFFER_SIZE, 0);

    int guess1 = atoi(buffer1);
    int guess2 = atoi(buffer2);

    int diff1 = abs(guess1 - secretNumber);
    int diff2 = abs(guess2 - secretNumber);

    std::string result;

    if (diff1 < diff2)
    {
        result = "Игрок 1 победил!";
        send(clientSocket1, "Вы выйграли!", 13, 0);
        send(clientSocket2, "Вы проиграли.", 14, 0);
    }
    else if (diff2 < diff1)
    {
        result = "Игрок 2 победил!";
        send(clientSocket1, "Вы проиграли.", 14, 0);
        send(clientSocket2,"Вы победили!", 13, 0);
    }
    else
    {
        result = "Ничья!";
        send(clientSocket1, result.c_str(), result.length(), 0);
        send(clientSocket2, result.c_str(), result.length(), 0);
    }

    std::cout << "Результат: " << result << std::endl;

    // Закрытие сокетов и завершение программы
    closesocket(clientSocket1);
    closesocket(clientSocket2);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
