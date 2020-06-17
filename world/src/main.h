#ifndef __main_h__
#define __main_h__

#include <memory>
#include "mysql.h"
#include "pmutex.h"
#include "vector2.h"

extern std::array<std::unique_ptr<mcon>, 2> connections;
extern sqlquery dbaccounts;
extern sqlquery dbcharacters;
extern sqlquery dbfriendlist;
extern sqlquery dbitems;
extern sqlquery dbmails;
extern sqlquery dbslots;
extern sqlquery dbskilllist2;
extern sqlquery dbguilds;
extern sqlquery dbguilds_g;
extern sqlquery dbcharacters_g;
extern sqlquery dbguildsiege;
extern pmutex dbguildsiege_mutex;

extern std::string mysqlhost;
extern std::string mysqluser;
extern std::string mysqlpasswd;
extern std::string mysqldb;
extern int mysqlport;

class cluster;
class clustercont;
extern clustercont *clusters;

class charserver;
extern charserver *cserver;
class tplayer;
extern std::map<int, tplayer*> globaldbidplayers;
extern std::map<std::string, tplayer*> globalnameplayers;
extern pmutex globalplayersmutex;
extern int servernumber;

#endif
