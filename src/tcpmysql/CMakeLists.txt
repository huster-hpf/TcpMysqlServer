set(TcpMysql_SRCS
    TcpMysqlServer.cc
    MysqlServer.cc
   )
add_library(TcpMysql ${TcpMysqlServer_SRCS})
target_link_libraries(TcpMysql muduo_net)

install(TARGETS TcpMysql DESTINATION lib)

set(HEADERS
    TcpMysqlServer.h
    MysqlServer.h
    TimeWheel.h
    )
install(FILES ${HEADERS} DESTINATION include/src/tcpmysql)
 
