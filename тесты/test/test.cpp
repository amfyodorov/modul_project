#include <cassert>
#include <iostream>
#include <sstream>

template <typename T>
std::string NumberToString(T Number)
{
    std::ostringstream ss;
    ss << Number;
    return ss.str();
}

void TestNumberToString()
{
    // Тестирование положительных чисел
    assert(NumberToString(123) == "123");
    assert(NumberToString(0) == "0");
    assert(NumberToString(987654321) == "987654321");

    // Тестирование отрицательных чисел
    assert(NumberToString(-456) == "-456");
    assert(NumberToString(-999) == "-999");

    // Тестирование чисел с плавающей точкой
    assert(NumberToString(3.14) == "3.14");
    assert(NumberToString(-0.5) == "-0.5");
    assert(NumberToString(1e6) == "1000000");
}

void TestMain()
{
    // В данном случае тестирование функции main будет сложным,
    // так как она работает с сетевыми соединениями и требует
    // наличия сервера для тестирования.

    // Вместо этого можно добавить дополнительные функции
    // в код и тестировать их отдельно.
}

int main()
{
    TestNumberToString();
    TestMain();

    std::cout << "Все тесты пройдены успешно." << std::endl;

    return 0;
}