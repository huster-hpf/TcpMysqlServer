#include <iostream>
#include <stdlib.h>
#include <src/tcpmysql/MysqlServer.h>

MysqlServer::MysqlServer()
{
  mysqlconn_=mysql_init(NULL);
  if(!mysql_real_connect(mysqlconn_,IP,USR,PSW,DB,PORT,NULL,0))
  {
    std::cout<<"Failed to connect to database. ERROR: "<<mysql_error(mysqlconn_)<<std::endl;
    exit(1);
  }
}

MysqlServer::~MysqlServer()
{
  mysql_close(mysqlconn_);
}

string MysqlServer::process(char *sql_)
{
  string res; 
  mysql_query(mysqlconn_,sql_);
  MYSQL_RES *result=mysql_store_result(mysqlconn_);
  if(result==NULL)
  {
     res+="error "+std::to_string(mysql_errno(mysqlconn_))+": "+static_cast<string>(mysql_error(mysqlconn_))+"\n";
  }
  else
  {
    int num_rows=static_cast<int>(mysql_num_rows(result));
    int num_fields=static_cast<int>(mysql_num_fields(result));
    res+="number of rows:"+std::to_string(num_rows)+"\n";
    res+="number of fields"+std::to_string(num_fields)+"\n";
    MYSQL_ROW row;
    while(num_rows--)
    {
      row=mysql_fetch_row(result);
      for(int i=0;i<num_fields;++i)
      {
        res+=static_cast<string>(row[i])+"\t";
      }
        res+="\n";
    }
    mysql_free_result(result);
  }
  return res;
}
