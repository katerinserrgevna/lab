#include <iostream>
#include <windows.h>
#include "7_4.h"
#include <string>
#include <locale>
#include <fstream>
#include <vector>

using namespace std;

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    ifstream fin("Text.txt");


    Red_Black<string, vector<contract>> employees;
    string str;
    size_t num;

    //Парсер из файла
    employees.file_employee(fin);

    while (true)
    {
        cout << "Выберите команду:" << endl <<
            "1. Стоимость всех договоров" << endl <<
            "2. Список договоров" << endl <<
            "3. Самый продолжительный договор" << endl <<
            "4. Самый дорогой договор" << endl <<
            "5. Удаление сотрудника" << endl <<
            "6. Напечатать дерево сотрудников" << endl <<
            "7. Выход" << endl;
        cin >> num;

        switch (num)
        {
        case 1:
            cout << "Введите ФИО сотрудника: ";
            cin.clear();
            cin.ignore();
            getline(cin, str, '\n');

            if (!employees.Find(str))
            {
                cout << "Этого сотрудника нет в списке!" << endl;
            }
            else employees.sum(str);
            break;
        case 2:
            cout << "Введите ФИО сотрудника: ";
            cin.clear();
            cin.ignore();
            getline(cin, str, '\n');

            if (!employees.Find(str))
            {
                cout << "Этого сотрудника нет в списке!" << endl;
            }
            else employees.list(str);
            break;
        case 3:
            cout << "Введите ФИО сотрудника: ";
            cin.clear();
            cin.ignore();
            getline(cin, str, '\n');

            if (!employees.Find(str))
            {
                cout << "Этого сотрудника нет в списке!" << endl;
            }
            else employees.longest(str);
            break;
        case 4:
            cout << "Введите ФИО сотрудника: ";
            cin.clear();
            cin.ignore();
            getline(cin, str, '\n');

            if (!employees.Find(str))
            {
                cout << "Этого сотрудника нет в списке!" << endl;
            }
            else
                employees.expensivest(str);
            break;
        case 5:
            cout << "Введите ФИО сотрудника: ";
            cin.clear();
            cin.ignore();
            getline(cin, str, '\n');

            if (employees.Find(str))
            {
                employees.Delete(str);
            }
            else cout << "Сотрудника не было найдено" << endl;

            break;
        case 6:
            cout << endl << endl << "____________________________________________________________" << endl << endl;
            employees.Print();
            cout << "____________________________________________________________" << endl << endl;
            break;
        case 7:
            fin.close();
            return 0;
        }
    }

}