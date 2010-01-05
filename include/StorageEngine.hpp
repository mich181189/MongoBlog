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
    
};

class StorageEngine {
public:
    StorageEngine(std::string username="",std::string pass="");
    StorageEngine(std::string host,std::string username="",std::string pass="");
    ~StorageEngine();
    void use(std::string db);
    std::vector<post> getposts(int number = 10);
    void sortposts(std::vector<post> &posts);
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
};
#endif