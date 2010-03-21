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

class Template {
public:
    Template(std::string name);
    ~Template();
    void render_head(std::string extra = "");
    void render_post(std::string title,std::string body);
    void render_footer();
    void set_page_title(std::string title);
    std::string substitute(std::string input,std::string needle,std::string replacement);
    std::string getFile(std::string fname);
    
private:
    std::string header;
    std::string post;
    std::string footer;
    std::string page_title;
    //replace one specific chunk
    //do all standard replacements
    std::string substitute(std::string input);
    //Date functions now
    std::string dayofweek(int num);
    std::string num_suffix(int num);
    std::string monthname(int num);
};
#endif