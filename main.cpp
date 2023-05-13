#include <iostream>
#include <pqxx/pqxx>

void printResult(const pqxx::result& result) {
    for (const auto& row : result) {
        std::cout << "ID = " << row[0].as<int>() << '\n';
        std::cout << "Name = " << row[1].as<std::string>() << '\n';
        std::cout << "Age = " << row[2].as<int>() << '\n';
        std::cout << "Address = " << row[3].as<std::string>() << '\n';
        std::cout << "Salary = " << row[4].as<float>() << '\n';
    }
}

int main() {
    try {
        pqxx::connection C("dbname=testdb user=postgres password=cohondob hostaddr=127.0.0.1 port=5432");
        if (C.is_open()) {
            std::cout << "Opened database successfully: " << C.dbname() << '\n';
        } else {
            std::cout << "Can't open database\n";
            return 1;
        }

        const std::string createTableSQL = "CREATE TABLE COMPANY2 ("  //1. создаем таблицу
            "ID INT PRIMARY KEY NOT NULL,"
            "NAME TEXT NOT NULL,"
            "AGE INT NOT NULL,"
            "ADDRESS CHAR(50),"
            "SALARY REAL"
            ")";

        const std::string insertDataSQL = "INSERT INTO COMPANY2 (ID, NAME, AGE, ADDRESS, SALARY) " //2. вносим туда данные
            "VALUES "
            "(1, 'Paul', 32, 'California', 20000.00), "
            "(2, 'Allen', 25, 'Texas', 15000.00), "
            "(3, 'Teddy', 23, 'Norway', 20000.00), "
            "(4, 'Mark', 25, 'Rich-Mond', 65000.00)";
         
        pqxx::work W(C);
        W.exec(createTableSQL);
        W.exec(insertDataSQL);
        W.commit();

        const std::string selectDataSQL = "SELECT * FROM COMPANY2";
        pqxx::nontransaction N(C);
        pqxx::result R(N.exec(selectDataSQL));
        if (!R.empty()) {
            printResult(R); //и 3. Выводим данные если они есть
        } else {
            std::cout << "No data found\n";
        }

        std::cout << "Table created and data inserted successfully\n";

        C.disconnect();
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}