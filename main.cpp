#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

string usernameIn, passwordIn;

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    int i;
    for (i=0; i < argc; i++)
    {
        printf("%s = %s; ", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

static int checkuser(void *NotUsed, int argc, char **argv, char **azColName)
{
    
    if (usernameIn == argv[1] && passwordIn == argv[2])
    {
        printf("Id %s login success.\n", argv[0]);
        ofstream jsonFile;
        jsonFile.open(usernameIn + ".json");
        jsonFile << "{\"r\": 0,\"m\": \"success\"}";
        exit(1);
    }
    
    return 0;
}

int main(int argc, const char * argv[]) {
    
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char const *sql;

    rc = sqlite3_open("UserData.db", &db);

    if( rc )
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    else
    {
        fprintf(stderr, "Records created successfully\n");
    }
    
    
    sql =   "CREATE TABLE USERDATA("
            "id INT PROMARY KEY    NOT NULL, "
            "username       TEXT   NOT NULL, "
            "password       TEXT   NOT NULL);";
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Table created successfully\n");
    }
    
    
    sql =   "INSERT INTO USERDATA (id,username,password)"
            "VALUES (1, 'Amberlyn', 'passwordAmberlyn');"
            "INSERT INTO USERDATA (id,username,password)"
            "VALUES (2, 'Bob', 'passwordBob');"
            "INSERT INTO USERDATA (id,username,password)"
            "VALUES (3, 'Cindy', 'passwordCindy');"
            "INSERT INTO USERDATA (id,username,password)"
            "VALUES (4, 'D', 'passwordAllen');";
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Table created successfully\n");
    }
    
    
    sql = "SELECT * from USERDATA";
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Operation done successfully\n");
    }
    
    
    printf ("Input the username: ");
    cin >> usernameIn;
    printf ("Input the password: ");
    cin >> passwordIn;
    
    rc = sqlite3_exec(db, sql, checkuser, 0, &zErrMsg);
    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "User not found.\n");
        ofstream jsonFile;
        jsonFile.open(usernameIn + ".json");
        jsonFile << "{\"m\": 0,\"m\": \"failure\"}";
    }
    
    sqlite3_close(db);
    return 0;
}
