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

Blog::~Blog() {
    if(templ)
        delete templ;
    if(storage)
        delete storage;
}

void Blog::urlDeEncode(string &str) {
    str = templ->substitute(str,"+"," ");
    str = templ->substitute(str,"%27","'");
    str = templ->substitute(str,"%0A","\n");
    str = templ->substitute(str,"%0D","\r");
    str = templ->substitute(str,"%2B","+");
    str = templ->substitute(str,"%24","$");
    str = templ->substitute(str,"%26","&");
    str = templ->substitute(str,"%2C",",");
    str = templ->substitute(str,"%2F","/");
    str = templ->substitute(str,"%3A",":");
    str = templ->substitute(str,"%3B",";");
    str = templ->substitute(str,"%3D","=");
    str = templ->substitute(str,"%3F","?");
    str = templ->substitute(str,"%40","@");
    str = templ->substitute(str,"%20"," ");
    str = templ->substitute(str,"%22","\"");
    str = templ->substitute(str,"%3C","<");
    str = templ->substitute(str,"%3E",">");
    str = templ->substitute(str,"%23","#");
    str = templ->substitute(str,"%28","(");
    str = templ->substitute(str,"%29",")");
    str = templ->substitute(str,"%7B","{");
    str = templ->substitute(str,"%7D","}");
    str = templ->substitute(str,"%5C","|");
    str = templ->substitute(str,"%5E","^");
    str = templ->substitute(str,"%7E","~");
    str = templ->substitute(str,"%5B","[");
    str = templ->substitute(str,"%5D","]");
    str = templ->substitute(str,"%60","`");
    str = templ->substitute(str,"%21","!");
    str = templ->substitute(str,"%25","%");
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
    //cout << "Read:" <<endl << "hostname: " << dbserver << " database: " << database <<endl << "uname: " << dbuser << " useauth: " << dbauth <<endl;
    init();
}

void Blog::init() {
    //check for post things
    CgiEnvironment env = cgi.getEnvironment();
    storage = new StorageEngine(dbserver,database,dbuser,dbpassword);
    string post = env.getPostData();
    vector<key_val> kvpairs = parseargs(post);
    vector<key_val>::iterator it;
    bool headers_sent = false;
    for(it=kvpairs.begin();it<kvpairs.end();it++) {
        //is it a login attempt?
        if(!it->key.compare("user")) {
            flush(cout);
            login_proc(kvpairs);
            headers_sent = true;
        }
        //is it a post?
        else if(!it->key.compare("body")) {
            if(admin_cookie()) {
                post_proc(kvpairs);
            }
        }
    }
    if(!headers_sent)
        cout << HTTPHTMLHeader();
    //storage is after header so any error messages get outputted
    //if there is no servername, and no auth, use this.
    templ = new Template(templatename);
    templ->set_page_title(pagetitle);
    
}

void Blog::post_proc(vector<key_val> kvpairs) {
    string title;
    string body;
    bool update = false;
    string oid;
    vector<key_val>::iterator it;
    for(it=kvpairs.begin();it<kvpairs.end();it++) {
        if(!it->key.compare("title")) {
            title = it->value;
            urlDeEncode(title);
        }
        else if(!it->key.compare("body")) {
            body = it->value;
            urlDeEncode(body);
        }
        else if(!it->key.compare("id")) {
            oid = it->value;
            urlDeEncode(oid);
            if(oid.length() > 0)
                update = true;
        }
    }
    if(body.length() > 0) {
        if(update) {
            storage->dopost(title,body,oid);
        }
        else {
            storage->dopost(title,body);
        }
    }
}

void Blog::login_proc(vector<key_val> kvpairs) {
    string user;
    string password;
    flush(cout);
    vector<key_val>::iterator it;
    for(it=kvpairs.begin();it<kvpairs.end();it++) {
        if(!it->key.compare("user")) {
            user = it->value;
        }
        else if(!it->key.compare("pass")) {
            password = it->value;
        }
    }
    
    
    //make sure it's at least sort of worked
    if(user.length() > 0) {
        flush(cout);
        string cookie = storage->login(user,password);
        flush(cout);
        //if login didn't fail
        if(cookie.length() > 0) {
            cout << HTTPHTMLHeader().setCookie(HTTPCookie("MongoBlogUser",cookie,"","",0,"",0));
            recentcookie = cookie;
            flush(cout);
        }
        else cout << HTTPHTMLHeader();
    }
    flush(cout);
    exit(0);
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
            //if oid is set, it's a post, otherwise it's a message.
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

vector<key_val> Blog::parseargs(string in) {
    vector<string> args;
    split(args,in, is_any_of("&") );
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
    return kvpairs;
}

//decides what page to show and shows it. This is the main entry point from main.
void Blog::run() {
    string get = cgi.getEnvironment().getQueryString();
    vector<key_val> kvpairs = parseargs(get);
    vector<key_val>::iterator kvit;
    for(kvit = kvpairs.begin();kvit<kvpairs.end();kvit++) {
        
        if(!(kvit->key.compare("post"))) {
            showPost(kvit->value);
            return;
        }
        if(!(kvit->key.compare("admin"))) {
            admin();
            return;
        }
    }
    //as a default in case none of the above are present:
    homepage();
}



bool Blog::admin_cookie() {
    const_cookie_iterator it;
    CgiEnvironment env = cgi.getEnvironment();
    User u;
    if(recentcookie.length() > 0) {
        storage->getUser(u,recentcookie);
    }
    else {
        for(it=env.getCookieList().begin();it<env.getCookieList().end();it++) {
            if(!(it->getName().compare("MongoBlogUser"))) {
                storage->getUser(u,it->getValue());
                break; //get out of for loop - we've found the cookie!
            }
        }
    }
    return u.is_admin;
}

void Blog::login() {
    templ->render_head();
    stringstream out;
    out << "<form method=\"POST\" action=\"\">" << endl
        << "Username: <input name=\"user\" type=\"text\"/><br/>" << endl
        << "Password: <input name=\"pass\" type=\"password\"/><br/>" << endl
        << "<input name=\"sub\" type=\"submit\"/><br/>" << endl
        << "</form>" << endl;
    templ->render_post("Login",out.str());
    templ->render_footer();
}

void Blog::admin() {
    if(!admin_cookie()) {
        login();
        return;
    }
    vector<post> posts = storage->getposts();
    stringstream js;
    js << "<script type=\"text/javascript\" src=\"/static/jquery.js\"></script>";
    js << "<script type=\"text/javascript\">";
    vector<post>::iterator pit;
    js << " $(document).ready(function(){" << endl;
    //wow jquery looks horrible in C++...
    for(pit=posts.begin();pit!= posts.end();pit++) {
        js << "$(\"#" << pit->oid << "\").click(function() {" << endl;
        js << "$(\"#titlebox\").val(\"" << templ->substitute(pit->title,"\"","\\\"") << "\");" << endl;
        js << "$(\"#id\").val(\"" << pit->oid << "\");" << endl;
        js << "$(\"#textareabox\").val(\"" << templ->substitute(templ->substitute(templ->substitute(pit->body,"\"","\\\""),"\n","\\\n"),"\r","") << "\");" << 
endl;
        js << "});";
    }
    js <<  "});";
    js << "</script>";
    templ->render_head(js.str());
    stringstream out;
    out << "<form method=\"POST\" action=\"\">" << endl
        << "Title: <input id=\"titlebox\" name=\"title\" type=\"text\"/><br/>" << endl
        << "Post Body: <textarea id=\"textareabox\" name=\"body\"></textarea><br/>" << endl
        << "<input id=\"id\" name=\"id\" type=\"hidden\"/>" << endl
        << "<input name=\"sub\" type=\"submit\"/><br/>" << endl
        << "</form>" << endl;
    templ->render_post("Make post",out.str());
    out.str("");
    
    if(posts.size() == 0) {
        templ->render_post("<tr><td colspan=3>Post not found","There are no posts here.</td></tr>");
    }
    else {
        out << "<table border=\"0\"><tr><td>Title</td><td>Body</td><td>Actions</td></tr>" << endl;
        vector<post>::iterator pit;
        for(pit=posts.begin();pit!= posts.end();pit++) {
            if(pit->body.length() > 140)
                pit->body.resize(140);
            out << "<tr><td>" << pit->title << "</td><td>" << pit->body << "</td><td> <a href=\"#\" id=\"" << pit->oid << "\">Edit</a></td></tr>" << endl;
        }
        out << "</table>" << endl;
    templ->render_post("Manage posts",out.str());
    }
    
    templ->render_footer();
}
