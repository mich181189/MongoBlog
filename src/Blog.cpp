///////////////////////////////////////////////////////////////////////
//   								     //
//   MongBlog - Superfast simple blogging platform                   //
//   Blog Class                                                      //
//   (c) 2010 Michael Cullen http://cullen-online.com                //
//   Released under the BSD licence as define in the                 //
//   accompanying LICENCE file. Please read it :-)                   //
//                                                                   //
///////////////////////////////////////////////////////////////////////

#include "Blog.hpp"
#include <vector>
#include <iostream>
#include <sstream>
using std::string;
using std::fstream;
using std::ios_base;
using std::stringstream;
using std::vector;
using std::cout;
using std::endl;
using namespace cgicc;

//constructor that uses defaults without loading a config file
Blog::Blog() {
    templatename = "hahaha";
    pagetitle = "MongoBlog";
    dbserver = "";
    dbauth = false;
    database = "MongoBlog";
    
    init();
}

Blog::~Blog() {
    if(templ)
        delete templ;
    if(storage)
        delete storage;
}

//constructor that uses a config file
Blog::Blog(std::string config) {
    fstream file;
    string line;
    file.open(config.c_str(),ios_base::in);
    while(file >> line) {
        int pos = line.find(':',0);
        string name = line.substr(0,pos);
        string val = line.substr(pos+1);
        if(!name.compare("hostname"))
        {
            dbserver = val;
        }
        else if(!name.compare("database"))
        {
            database = val;
        }
        else if(!name.compare("uname"))
        {
            dbuser = val;
        }
        else if(!name.compare("password"))
        {
            dbpassword = val;
        }
        else if(!name.compare("useauth"))
        {
            stringstream thisval(val);
            thisval >> dbauth;
        }
    }
    file.close();
    cout << "Read:" <<endl << "hostname: " << dbserver << " database: " << database <<endl << "uname: " << dbuser << " useauth: " << dbauth <<endl;
    init();
}

void Blog::init() {
    cout << HTTPHTMLHeader();
    //storage is after header so any error messages get outputted
    //if there is no servername, and no auth, use this.
    storage = new StorageEngine(dbserver,database,dbuser,dbpassword);
    templ = new Template(templatename);
    templ->set_page_title(pagetitle);
    
}

void Blog::homepage() {
    templ->render_head();
    vector<post> posts = storage->getposts();
    if(posts.size() == 0) {
        templ->render_post("Post not found","There are no posts here.");
    }
    else {
        vector<post>::iterator pit;
        for(pit=posts.begin();pit!= posts.end();pit++) {
            templ->render_post(pit->title,pit->body);
        }
    }
    templ->render_footer();
}