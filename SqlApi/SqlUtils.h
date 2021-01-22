/*
 * @Author: lhlcyq
 * @Date: 2021-01-10 20:47:14
 * @LastEditTime: 2021-01-22 22:01:50
 * @LastEditors: Please set LastEditors
 * @Description: 连接数据库工具类
 * @FilePath: \VScode\SensorDemo\MYSQL\SqlUtils.h
 */
#ifndef MYSQLUTILS_H
#define MYSQLUTILS_H
// #include <Arduino.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
// 连接数据库信息结构体
struct ConnectMessage {
    IPAddress server;
    int port;
    String user;
    String password;
    String database;
    String tablename;
};
// 根据不同的数据表的数据修改
typedef struct {
    String equipment_id, product_id, work_id;
    int count;
} TableMessage;
// 数据库工具类
class SqlUtils {
public:
    SqlUtils(Client& client);

    // Data members
    ConnectMessage ConnMessage;

    bool MysqlConnect();

    void MysqlClose() { SqlConnect->close(); };

    bool Insert(TableMessage* Tabmessage);

private:
    // Data members
    MySQL_Connection* SqlConnect;

    MySQL_Cursor* SqlQuery;
};

#endif
