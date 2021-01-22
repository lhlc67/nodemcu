/*
 * @Author: lhlc67
 * @Date: 2021-01-10 20:47:24
 * @LastEditTime: 2021-01-22 20:31:46
 * @LastEditors: Please set LastEditors
 * @Description: 连接数据库工具类
 * @FilePath: \VScode\SensorDemo\MYSQL\SqlUtils.cpp
 */
#include "SqlUtils.h"

/**
 * @description: SqlUtils的构造函数，需要的参数为Client类或者它的子类，
 *               工具类会通过Client创建MySQL_Connection类和MySQL_Cursor类，集成连接和查询操作
 * @param {Client* client}
 * @return {*}
 */
SqlUtils ::SqlUtils(Client& client)
{
    // 创建一个MySQL_Connection对象
    SqlConnect = new MySQL_Connection(&client);
    // 创建一个查询对象
    SqlQuery = new MySQL_Cursor(SqlConnect);
}

/**
 * @description: 连接数据库
 * @param {void}
 * @return {bool}
 */
bool SqlUtils::MysqlConnect()
{
    /*
    Serial.println(ConnMessage.server);
    Serial.println(ConnMessage.port);
    Serial.println((char*)ConnMessage.user.c_str());
    Serial.println((char*)ConnMessage.password.c_str());
    */
    return SqlConnect->connect(ConnMessage.server,
        ConnMessage.port,
        (char*)ConnMessage.user.c_str(),
        (char*)ConnMessage.password.c_str());
}

/**
 * @description: 插入到数据库中
 * @param {TableMessage* Tabmessage}    数据表信息结构体
 * @return {bool}   返回是否插入成功
 */
bool SqlUtils::Insert(TableMessage* Tabmessage)
{
    char buff[256];
    String sql = "INSERT INTO %s.%s(equipment_id, product_id, work_id, count) VALUES('%s','%s','%s','%d')";
    // 拼接SQL语句
    //    if (Tabmessage->date == "")
    //        return false;
    // 使用占位符合成SQL语句，注意要保证buff开辟了足够大的空间, 不然会造成内存泄漏
    sprintf(buff, sql.c_str(),
        ConnMessage.database.c_str(),
        ConnMessage.tablename.c_str(),
        Tabmessage->equipment_id.c_str(),
        Tabmessage->product_id.c_str(),
        Tabmessage->work_id.c_str(),
        Tabmessage->count);
    Serial.println(buff);
    // 设置字符编码，防止出现中文乱码
    SqlQuery->execute("SET character_set_client = UTF8;");
    SqlQuery->execute("SET character_set_connection = UTF8;");
    SqlQuery->execute("SET character_set_results = UTF8;");
    SqlQuery->execute("SET character_set_server = UTF8;");
    return SqlQuery->execute(buff);
}

// More Fuctions
