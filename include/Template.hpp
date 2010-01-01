///////////////////////////////////////////////////////////////////////
//   								     //
//   MongBlog - Superfast simple blogging platform                   //
//   Template Library                                                //
//   (c) 2010 Michael Cullen http://cullen-online.com                //
//   Released under the BSD licence as define in the                 //
//   accompanying LICENCE file. Please read it :-)                   //
//                                                                   //
///////////////////////////////////////////////////////////////////////
#ifndef TEMPLATE_HPP
#define TEMPLATE_HPP
#include <string>
using std::string;

class Template {
public:
    Template(string name);
    ~Template();
    void render_head();
    void render_post(string title,string body);
    void render_footer();
    void set_page_title(string title);
private:
    string header;
    string post;
    string footer;
    string page_title;
    //replace one specific chunk
    string substitute(string input,string needle,string replacement);
    //do all standard replacements
    string substitute(string input);
};
#endif