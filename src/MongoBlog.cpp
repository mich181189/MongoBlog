///////////////////////////////////////////////////////////////////////
//   								     //
//   MongBlog - Superfast simple blogging platform                   //
//   (c) 2010 Michael Cullen http://cullen-online.com                //
//   Released under the BSD licence as define in the                 //
//   accompanying LICENCE file. Please read it :-)                   //
//                                                                   //
///////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <cstdlib> //for getenv()
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTTPRedirectHeader.h>
#include "Template.hpp"

using std::cout;
using std::endl;
using std::ifstream;
using namespace cgicc;

int main() {
    Cgicc cgi;
    cout << HTTPHTMLHeader();
    Template templ("hahaha");
    templ.set_page_title("MongoBlog");
    templ.render_head();
    templ.render_post("title","body");
    templ.render_post("title","body");
    templ.render_footer();
    return 0;
}
