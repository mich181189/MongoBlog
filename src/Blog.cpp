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
#include <boost/algorithm/string.hpp>
using std::string;
using std::fstream;
using std::ios_base;
using std::stringstream;
using std::vector;
using std::cout;
using std::endl;
using namespace cgicc;
using namespace boost;
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
    templatename = "hahaha";
    pagetitle = "MongoBlog";
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
            stringstream titlelink;
            if(pit->oid.length() > 0) {
                titlelink << "<a href=\"post/" << pit->oid << "/\" >" << pit->title << "</a>";
            }
            else {
                titlelink << pit->title;
            }
            templ->render_post(titlelink.str(),pit->body);
        }
    }
    templ->render_footer();
}

void Blog::showPost(string objid) {
    post p = storage->getpost(objid);
    templ->render_head();
    templ->render_post(p.title,p.body);
    templ->render_footer();
}


//decides what page to show and shows it. This is the main entry point from main.
void Blog::run() {
    string get = cgi.getEnvironment().getQueryString();
    vector<string> args;
    split(args,get, is_any_of("&") );
    vector<string>::iterator it;
    vector<key_val> kvpairs;
    for(it=args.begin();it<args.end();it++) {
        vector<string> tmp;
        split(tmp,*it,is_any_of("="));
        key_val kv;
        kv.key = tmp[0];
        if(tmp.size() > 1)
            kv.value = tmp[1];
        kvpairs.push_back(kv);
    }
    vector<key_val>::iterator kvit;
    for(kvit = kvpairs.begin();kvit<kvpairs.end();kvit++) {
        
        if(!(kvit->key.compare("post"))) {
            showPost(kvit->value);
            return;
        }
    }
    //as a default in case none of the above are present:
    homepage();
}

