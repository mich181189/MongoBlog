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
    //Set default page title
    page_title = "Mwhahaha";
    stringstream fname;
    fstream file;
    //get head file from template
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
    //get footer from template
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
     //get post file from template
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

string Template::dayofweek(int num) {
    switch(num) {
        case 0:
            return "Sunday";
        break;
        case 1:
            return "Monday";
        break;
        case 2:
            return "Tuesday";
        break;
        case 3:
            return "Wednesday";
        break;
        case 4:
            return "Thursday";
        break;
        case 5:
            return "Friday";
        break;
        case 6:
            return "Saturday";
        break;
    }
}
//returns stuff like st, th and so on
string Template::num_suffix(int num) {
    int procnum = num%10;
    switch(procnum) {
        case 1:
            return "st";
            break;
        case 2:
            return "nd";
            break;
        case 3:
            return "rd";
            break;
        default:
            return "th";
            break;
    }
}

//returns name of numbered month
string Template::monthname(int num) {
    switch(num) {
        case 0:
            return "January";
            break;
        case 1:
            return "February";
            break;
        case 2:
            return "March";
            break;
        case 3:
            return "April";
            break;
        case 4:
            return "May";
            break;
        case 5:
            return "June";
            break;
        case 6:
            return "July";
            break;
        case 7:
            return "August";
            break;
        case 8:
            return "September";
            break;
        case 9:
            return "October";
            break;
        case 10:
            return "November";
            break;
        case 11:
            return "December";
            break;
    }
}

//does the standard replacements that happen everywhere
string Template::substitute(string input) {
    string output = input;
    output = substitute(input,"[TITLE]",page_title);
    //do date
    time_t t;
    struct tm * tinfo;
    time(&t);
    tinfo = localtime(&t);
    stringstream timestr,datestr;
    datestr << dayofweek(tinfo->tm_wday) << " ";
    datestr << tinfo->tm_mday << num_suffix(tinfo->tm_mday) << " ";
    datestr << monthname(tinfo->tm_mon) << " " << tinfo->tm_year+1900;
    output = substitute(output,"[DATE]",datestr.str());
    timestr << tinfo->tm_hour << ":" << tinfo->tm_min << ":" << tinfo->tm_sec;
    output = substitute(output,"[TIME]",timestr.str());
    return output;
}

void Template::render_head() {
    cout << substitute(header);
}

void Template::render_post(string title,string body) {
    string output = post;
    output = substitute(output,"[POSTTITLE]",title);
    output = substitute(output,"[POSTBODY]",body);
    output = substitute(output);
    cout << output;
}

void Template::render_footer() {
    cout << substitute(footer);
}

void Template::set_page_title(string title) {
    page_title = title;
}