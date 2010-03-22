///////////////////////////////////////////////////////////////////////
//   								     //
//   MongBlog - Superfast simple blogging platform                   //
//   Storage Engine                                                  //
//   (c) 2010 Michael Cullen http://cullen-online.com                //
//   Released under the BSD licence as define in the                 //
//   accompanying LICENCE file. Please read it :-)                   //
//                                                                   //
///////////////////////////////////////////////////////////////////////
#ifndef MMBLOGSTORAGEENGINE_H
#define MMBLOGSTORAGEENGINE_H
#include <string>
#include <vector>
#undef VERSION //needed because otherwise mongo conflicts
#include <mongo/client/dbclient.h>

class post {
public:
    std::string title;
    std::string body;
    int timestamp;
    std::string oid;
};

class User {
public:
    std::string username;
    std::string password;
    bool is_admin;
    User() {is_admin = false;}
};

class StorageEngine {
public:
    StorageEngine(std::string username="",std::string pass="");
    StorageEngine(std::string host,std::string db,std::string username,std::string pass);
    ~StorageEngine();
    void use(std::string db);
    std::vector<post> getposts(int number = 10);
    post getpost(std::string oid);
    void dopost(std::string title,std::string body);
    void dopost(std::string title,std::string body,time_t timestamp);
    void dopost(std::string title,std::string body,std::string oid); //for updates
    void getUser(User &u,std::string cookie_id);
    std::string login(std::string username,std::string password);
private:
    mongo::DBClientConnection con;
    std::string hostname;
    void connect(std::string username,std::string pass);
    std::string database;
    std::string collection;
    std::string namespacestr;
    std::string uname;
    std::string password;
    void update_namespace();
    std::string getoid(mongo::BSONObj post);
    mongo::BSONObj getoid(std::string oid);
};
#endif
