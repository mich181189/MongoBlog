///////////////////////////////////////////////////////////////////////
//   								     //
//   MongBlog - Superfast simple blogging platform                   //
//   Template Library                                                //
//   (c) 2010 Michael Cullen http://cullen-online.com                //
//   Released under the BSD licence as define in the                 //
//   accompanying LICENCE file. Please read it :-)                   //
//                                                                   //
///////////////////////////////////////////////////////////////////////

#include "Template.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <cstdlib>
using std::string;
using std::cout;
using std::endl;
using std::stringstream;
using std::fstream;

Template::Template(string name) {
    page_title = "Mwhahaha";
    stringstream fname;
    fstream file;
    fname << getenv("DOCUMENT_ROOT") << "/mbTemplate/" << name << "/head.txt";
    file.open(fname.str().c_str());
    if(!file) {
        cout << "Cannot open |" << fname.str().c_str() << "|" << endl << "Exiting now.";
        exit(0);
    }
    string line;
    while(file >> line) {
        header.append(line);
        header.push_back('\n');
    }
    file.close();
    fname.str("");
    fname << getenv("DOCUMENT_ROOT") << "/mbTemplate/" << name << "/footer.txt";
    file.open(fname.str().c_str());
    if(!file) {
        cout << "Cannot open |" << fname.str().c_str() << "|" << endl << "Exiting now.";
        exit(0);
    }
    while(file >> line) {
        footer.append(line);
        footer.push_back('\n');
    }
    file.close();
    
     fname.str("");
    fname << getenv("DOCUMENT_ROOT") << "/mbTemplate/" << name << "/post.txt";
    file.open(fname.str().c_str());
    if(!file) {
        cout << "Cannot open |" << fname.str().c_str() << "|" << endl << "Exiting now.";
        exit(0);
    }
    
    while(file >> line) {
        post.append(line);
        post.push_back('\n');
    }
    file.close();
}

Template::~Template() {
    
}

string Template::substitute(string input,string needle,string replacement) {
    size_t pos = 0;
    while(pos < input.length()) {
        pos = input.find(needle,pos);
        if(pos == input.npos)
            continue; //break out of loop
        //still in the loop
        input.replace(pos,needle.length(),replacement);
        pos += replacement.length();
    }
    return input;
}

string Template::substitute(string input) {
    string output = input;
    output = substitute(input,"[TITLE]",page_title);
    return output;
}

void Template::render_head() {
    cout << substitute(header);
}

void Template::render_post(string title,string body) {
    string output = post;
    output = substitute(output,"[POSTTITLE]",title);
    output = substitute(output,"[POSTBODY]",body);
    cout << output;
}

void Template::render_footer() {
    cout << substitute(footer);
}

void Template::set_page_title(string title) {
    page_title = title;
}