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
#include "md5.h"

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
    flush(cout);
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
            newpost.oid = getoid(record);
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

//Actually makes a post:
void StorageEngine::dopost(std::string title,std::string body) {
    time_t t;
    time(&t);
    dopost(title,body,t);
}

//returns cookie ID or empty string on failure.
string StorageEngine::login(string username,string password) {
    collection = "users";
    update_namespace();
    string outs;
    flush(cout);
    try {
        
    auto_ptr< DBClientCursor > ptr = con.query(namespacestr,QUERY("UserName" << username << "Password" << MD5(password).hexdigest()));
    
    if(ptr->more()) {
        stringstream cookiestring;
        cookiestring << time(NULL) << "<>" << username << "<>" << rand();
        outs = MD5(cookiestring.str()).hexdigest();
        collection = "cookies";
        update_namespace();
        con.insert(namespacestr,BSON("cid" << outs << "uid" << getoid(ptr->next())));
    }
    }
    catch(DBException &e ) {
        cout << "Caught " << e.what() << endl;
        flush(cout);
    }
    return outs;
}

void StorageEngine::dopost(std::string title,std::string body,time_t timestamp) {
    BSONObj post = BSON("title" << title << "body" << body << "timestamp" << (long long int)timestamp);
    collection = "posts";
    update_namespace();
    try {
        con.insert(namespacestr,post);
        con.ensureIndex(namespacestr,BSON("timestamp" << 1),true);
    } catch(DBException &e) {
        cout << "Error: " << e.what() <<endl;
    }
    
}

void StorageEngine::dopost(std::string title,std::string body,string oid) {
    collection = "posts";
    update_namespace();
    try {
        con.update(namespacestr,Query(getoid(oid)),BSON("title" << title << "body" << body));
    } catch(DBException &e) {
        cout << "Error: " << e.what() <<endl;
    }
}

string StorageEngine::getoid(BSONObj post) {
    BSONElement el;
    post.getObjectID(el);
    return el.__oid().str();
}

post StorageEngine::getpost(std::string oid) {
    post newpost;
    collection = "posts";
    update_namespace();
    auto_ptr<DBClientCursor> cursor = con.query(namespacestr,getoid(oid));
    if(cursor->more()) {
        BSONObj record = cursor->next();
        newpost.title = record.getStringField("title");
        newpost.body = record.getStringField("body");
        newpost.timestamp = record.getIntField("timestamp");
        newpost.oid = getoid(record);
    }
    else {
        newpost.title = "Post Not Found";
        stringstream pb;
        pb << "The post you requested (" << oid << ") was not found.";
        newpost.body = pb.str();
    }
    return newpost;
}

BSONObj StorageEngine::getoid(string oid) {
    OID oidobj;
    oidobj.init(oid);
    BSONObjBuilder ob;
    ob.appendOID("_id",&oidobj);
    return ob.obj();    
}

void StorageEngine::getUser(User &u,std::string cookie_id) {
    collection = "cookies";
    update_namespace();
    auto_ptr<DBClientCursor> cursor = con.query(namespacestr,QUERY("cid" << cookie_id));
    if(cursor->more()) {
        BSONObj record = cursor->next();
        string oid = record.getStringField("uid");
        collection = "users";
        update_namespace();
        auto_ptr<DBClientCursor> cursor_b = con.query(namespacestr,getoid(oid));
        if(cursor_b->more()) {
            BSONObj rec = cursor_b->next();
            u.username = rec.getStringField("UserName");
            u.is_admin = rec.getIntField("is_admin");
        }
        else {
            cout << "The user mentioned in the cookie does not exist!" << endl;
        }
    }
}
