///////////////////////////////////////////////////////////////////////
//   								     //
//   MongBlog - Superfast simple blogging platform                   //
//   Storage Engine Base                                             //
//   (c) 2010 Michael Cullen http://cullen-online.com                //
//   Released under the BSD licence as define in the                 //
//   accompanying LICENCE file. Please read it :-)                   //
//                                                                   //
///////////////////////////////////////////////////////////////////////

#include "StorageEngine.hpp"
#include <cstdlib> //for exit()
#include <string>
#include <sstream>
#include <iostream>
using std::string;
using std::cout;
using std::endl;
using namespace mongo;

//connects to localhost
//username and password default to null, as defined in header.
StorageEngine::StorageEngine(string username,string pass) {
    hostname = "localhost";
    database = "MongoBlog";
    collection = "posts";
    connect(username,pass);
}

//connects to name
StorageEngine::StorageEngine(string host,string db,string username,string pass) {
    hostname = host;
    database = db;
    collection = "posts";
    connect(username, pass);
}

//Selects the database to use
void StorageEngine::use(string db) {
    database = db;
}

//updates namespace string - called after db or collection change.
void StorageEngine::update_namespace() {
    stringstream ss;
    ss << database << "." << collection;
    namespacestr = ss.str();
}

void StorageEngine::StorageEngine::connect(string username,string pass) {
    try {
        con.connect(hostname);
    }
    catch(DBException &e ) {
        cout << "This probably means the database server is down. Please come back later." << endl;
        exit(0);
    }
    //only try and auth if a username is supplied.
    if(username.length() != 0) {
        string error;
        if(!con.auth(database,username,pass,error,true)) {
            cout << "Error authenticating to database " << database << " as " << username << " : " << error <<endl;
            exit(0);
        }
        
    }
}

std::vector<post> StorageEngine::getposts(int number) {
    std::vector<post> posts;
    collection = "posts";
    update_namespace();
    //if it's empty, return nothing.
    try {
        if(con.count(namespacestr) == 0)
            return posts;
        Query q("{ }");
        auto_ptr<DBClientCursor> cursor = con.query(namespacestr,q.sort("timestamp",-1));
        while(cursor->more()) {
            BSONObj record = cursor->next();
            post newpost;
            newpost.title = record.getStringField("title");
            newpost.body = record.getStringField("body");
            newpost.timestamp = record.getIntField("timestamp");
            posts.push_back(newpost);
        }
    } catch( DBException &e ) {
        cout << "caught " << e.what() << endl;
        exit(0);
    }
    

    return posts;
}

StorageEngine::~StorageEngine() {
    
}