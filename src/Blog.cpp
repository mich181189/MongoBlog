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

using std::string;
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
    
    //for testing only!
    dbuser = "blogtest";
    dbpassword="bt";
    dbauth = true;
    //end testing
    
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
    init();
}

void Blog::init() {
    cout << HTTPHTMLHeader();
    //storage is after header so any error messages get outputted
    //if there is no servername, and no auth, use this.
    if((dbserver.length() == 0) && !dbauth)
        storage = new StorageEngine();
    //else if there is auth but no server specified
    else if(dbserver.length() == 0)
        storage = new StorageEngine("localhost",dbuser,dbpassword);
    //else if there is a servername AND password:
    else
        storage = new StorageEngine(dbserver,dbuser,dbpassword);
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