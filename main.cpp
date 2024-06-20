#include <iostream>
#include <limits>
#include <string>
#include <fstream>
//#include <conio.h>
#include <ctime>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
using namespace std;

struct USER { //Структура пользователя
    string Login;
    string Password;
    int Rights = 0;
};

USER U;//Пользователь
/*
* Rights - хранение прав
* 0 бит - чтение файлов
* 1 бит - запись в файл
* 2 бит - создание файлов
* 3 бит удаление файлов
*/
void Book(string text);//Функция журналирования
bool Authentication();//Функция аутентификации
void ReadFiles();//Функция чтения
void WriteFiles();//Функция записи
void CreatFiles();//Функция создания файла
void DeleteFiles();//Функция удаления файла
void UserActions();//Функция выбора действия пользователем
void CreatUser();//Функция создания пользователей
void DeleteUser();//Функция удаления пользователя
void Rights();//Функция переназначения прав
void AdminAction();//Функция выбора действия администратора

int main()
{
    setlocale(LC_ALL, "");
    char change = '0';
    while (change != '2') {
        cout << "Меню: 1 - Аутентификация, 2 - Выход" << endl;
        change = cin.get();
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка буфера
        if (change == '1') {
            if (Authentication()) {
                if (U.Login == "ADMIN") {
                    AdminAction();
                } else {
                    UserActions();
                }
            }
        } else if (change != '2')
            cout << "Ошибка: выбрано неверное действие!" << endl;
    }
    return 0;
}

void Book(string text)
{
    time_t now = time(0);
    char* dt = ctime(&now);
    ofstream Book("/home/user/Pract_leto_Kotunov/pract1/book.txt", ios_base::app);
    Book << dt << text << endl << endl;
    Book.close();
}
bool Authentication()
{
    cout << "Аутентификация" << endl;
error1:
    string Correct_Login, Correct_Password;
    cout << "Введите логин пользователя: ";
    cin >> U.Login;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка буфера
    cout << "Введите пароль: ";
    cin >> U.Password;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка буфера
    ifstream Data("/home/user/Pract_leto_Kotunov/pract1/" + U.Login + ".txt");
    if (Data.is_open()) {
        Data >> Correct_Password;
        if (Correct_Password == U.Password) {
            Data >> U.Rights;
            return true;
        } else {
            cout << "Ошибка: Введен неверный пароль!" << endl;
            return false;
        }
        Data.close();
    } else {
        cout << "Ошибка: Введен неверный логин!" << endl;
        return false;
    }
}
void ReadFiles()//Функция чтения
{
    if (U.Rights & (1 << 0)) {//Проверка права на чтение файлов (0 бит)
        string FName,Line;
        //, Line;//Название файла и строка для хранения данных
        cout << "Введите  названия файла"<<endl;
        cin >> FName;
        ifstream RFile(FName);//Открытие файла
        if (RFile.is_open()) { //Проверка на существование
            cout << "---------------------------------------------" << endl;
            while (getline(RFile, Line)) { //пока есть строки, чтение
                cout << Line << endl;//Вывод строки файла
            }
            cout << "---------------------------------------------" << endl;
            RFile.close();//Закрытие файла
            Book("Файл " + FName + " успешно прочитан!");
        } else {
            cout << "Ошибка: Файл <" << FName << "> не найден!" << endl;
        }
    } else {
        cout << "Ошибка: У пользователя <" << U.Login << "> отсутствуют права на чтение файлов!" << endl;
    }
}
void WriteFiles()//Функция записи
{
    if (U.Rights & (1 << 1)) {//Проверка права на запись в файл (1 бит)
        string FName, Line;//Название файла и строка для хранения данных
        cout << "Введите название файла: ";
        cin >> FName;//Ввод названия файла
        ofstream WFile(FName, ios_base::app);//Открытие файла для дозаписи
        cout << "Введите текст для записи: ";
        cin >> Line;//Ввод текста
        WFile << Line;//Запись текста
        WFile.close();//Закрытие файла
        Book("Была произведена запись в файл " + FName);
    } else {
        cout << "Ошибка: У пользователя <" << U.Login << "> отсутвуют права на запись!" << endl;
    }
}
void CreatFiles()//Функция создания файлов
{
    if (U.Rights & (1 << 2)) {//Проверка права на создание файлов (2 бит)
        string FName;//Название файла
        cout << "Введите название файла: ";
        cin >> FName;
        ofstream NewFile(FName);//Создание пустого файла
        NewFile.close();//Закрытие созданного файла
        cout << "Файл " << FName << " успешно создан!" << endl;
        Book("Файл " + FName + " успешно создан!");
    } else {
        cout << "Ошибка: У пользователя <" << U.Login << "> отстутствуют права на создание файлов" << endl;
    }
}
void DeleteFiles()
{
    if (U.Rights & (1 << 3)) {//Проверка права на удаление файлов (3 бит)
        cout << "Введите название файла: ";
        string FName;//Строка для названия файла
        cin >> FName;//Ввод названия файла
        remove(FName.c_str());//Удаление файла
        cout << "Файл " << FName << " успешно удален!";
        Book("Файл " + FName + " успешно удален!");
    } else {
        cout << "Ошибка: У пользователя <" << U.Login << "> отсутствуют права на удаление файла" << endl;
    }
}
void UserActions()
{
    while (true) {
        cout << "Выберите действие (нажмите на нужную цифру, соответствующую действию): " << endl;
        cout << "\t\t1.Чтение файлов\n\t\t2.Запись в файл\n\t\t3.Создание файла\n\t\t4.Удаление файла\n\t\t5.Выход\n";
        char choice = cin.get();
        if (choice == '1')
            ReadFiles();
        else if (choice == '2')
            WriteFiles();
        else if (choice == '3')
            CreatFiles();
        else if (choice == '4')
            DeleteFiles();
        else if (choice == '5')
            break;
        else
            cout << "Ошибка: Выбрано несуществующее действие!" << endl;
    }
}
void CreatUser()
{
    string Login, Password;
    int Rights = 0;
    cout << "Введите логин нового пользователя: ";
    getline(cin, Login); // Ввод логина с использованием getline
    cout << "Введите пароль нового пользователя: ";
    getline(cin, Password); // Ввод пароля с использованием getline
    ofstream Data("/home/user/Pract_leto_Kotunov/pract1/" + Login + ".txt");
    Data << Password << endl;
    Data << "0";
    Data.close();
    cout << "Пользователь <" << Login << "> успешно создан!" << endl;
    Book("Создан новый пользователь: " + Login);
}
void DeleteUser()
{
    string Login;
    cout << "Введите логин пользователя: ";
    getline(cin, Login); // Ввод логина с использованием getline
    string path = "/home/user/Pract_leto_Kotunov/pract1/" + Login + ".txt";
    ifstream Data(path);
    if (Data.is_open()) {
        Data.close();
        remove(path.c_str());
        cout << "Пользователь <" << Login << "> успешно удален!" << endl;
        Book("Удален пользователь: " + Login);
    } else
        cout << "Ошибка: Пользователя <" << Login << "> не существует!" << endl;
}
void Rights()
{
    string User, Password;
    int Rights_ = 0;
    char a = 0;
    cout << "Введите логин пользователя: ";
    getline(cin, User); // Ввод логина с использованием getline
    string path = "/home/user/Pract_leto_Kotunov/pract1/" + User + ".txt";
    ifstream FileUser(path);
    if (FileUser.is_open()) {
metka1:
        cout << "Введите право на чтение (1 - есть, 0 - нет): " << endl;
        a = cin.get();
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка буфера
        if (a == '1')
            Rights_ |= (1 << 0);
        else if (a == '0')
            Rights_ &= ~(1 << 0);
        else {
            cout << "Ошибка: Выбрано неверное действие!" << endl;
            goto metka1;
        }
metka2:
        cout << "Введите право на запись (1 - есть, 0 - нет): " << endl;
        a = cin.get();
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка буфера
        if (a == '1')
            Rights_ |= (1 << 1);
        else if (a == '0')
            Rights_ &= ~(1 << 1);
        else {
            cout << "Ошибка: Выбрано неверное действие!" << endl;
            goto metka2;
        }
metka3:
        cout << "Введите право на создание (1 - есть, 0 - нет): " << endl;
        a = cin.get();
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка буфера
        if (a == '1')
            Rights_ |= (1 << 2);
        else if (a == '0')
            Rights_ &= ~(1 << 2);
        else {
            cout << "Ошибка: Выбрано неверное действие!" << endl;
            goto metka3;
        }
metka4:
        cout << "Введите право на удаление (1 - есть, 0 - нет): " << endl;
        a = cin.get();
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка буфера
        if (a == '1')
            Rights_ |= (1 << 3);
        else if (a == '0')
            Rights_ &= ~(1 << 3);
        else {
            cout << "Ошибка: Выбрано неверное действие!" << endl;
            goto metka4;
        }
        FileUser >> Password;
        FileUser.close();
        remove(path.c_str());
        ofstream NewFileUser(path);
        NewFileUser << Password << endl;
        NewFileUser << Rights_;
        NewFileUser.close();
        cout << "Права пользователю <" << User << "> успешно переназначены" << endl;
        Book("Назначены новые права пользователю: " + User);
    }
}
void AdminAction()
{
    while (true) {
        cout << "Выберите действие (нажмите на нужную цифру, соответствующую действию): " << endl;
        cout << "\t\t1.Создание пользователя\n\t\t2.Удаление пользователя\n\t\t3.Назначение полномочий\n\t\t4.Выход\n";
        char choice = cin.get();
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка буфера
        if (choice == '1')
            CreatUser();
        else if (choice == '2')
            DeleteUser();
        else if (choice == '3')
            Rights();
        else if (choice == '4')
            break;
        else
            cout << "Ошибка: Выбрано несуществующее действие!" << endl;
    }
}