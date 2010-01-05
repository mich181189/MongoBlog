///////////////////////////////////////////////////////////////////////
//   								     //
//   MongBlog - Superfast simple blogging platform                   //
//   Blog Class                                                      //
//   (c) 2010 Michael Cullen http://cullen-online.com                //
//   Released under the BSD licence as define in the                 //
//   accompanying LICENCE file. Please read it :-)                   //
//                                                                   //
///////////////////////////////////////////////////////////////////////

#ifndef BLOG_H
#define BLOG_H
#include <string>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTTPRedirectHeader.h>
#include <cgicc/CgiEnvironment.h>
#include "Template.hpp"
#include "StorageEngine.hpp"

class key_val {
public:
    std::string key;
    std::string value;
};

class Blog {
public:
    Blog();
    Blog(std::string config);
    ~Blog();
    void homepage();
    void run();
    void showPost(string objid);
private:
    cgicc::Cgicc cgi;
    void init();
    StorageEngine *storage;
    Template *templ;
    std::string templatename;
    std::string pagetitle;
    std::string dbserver;
    std::string dbuser;
    std::string database;
    std::string dbpassword;
    bool dbauth;
};
#endif