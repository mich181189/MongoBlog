///////////////////////////////////////////////////////////////////////
//   								     //
//   MongBlog - Superfast simple blogging platform                   //
//   Blog Configurator                                               //
//   (c) 2010 Michael Cullen http://cullen-online.com                //
//   Released under the BSD licence as define in the                 //
//   accompanying LICENCE file. Please read it :-)                   //
//                                                                   //
///////////////////////////////////////////////////////////////////////
#include <string>
#include <sstream>
#include <iostream>
#undef VERSION //needed because otherwise mongo conflicts
#include <mongo/client/dbclient.h>
#include "md5.h"

using namespace mongo;
using namespace std;
int main() {
    std::string hostname;
    std::string database;
    std::string namespacestr;
    std::string uname;
    std::string password;
    std::string auname;
    std::string apassword;
    char authc;
    bool useauth = false;
    bool makeauth = false;
    char buffer[255];
    cout << "MongoBlog Configurator" << endl << endl;
    cout << "Please enter the database server (default: localhost): ";
    cin.clear();
    cin.get(buffer,255);
    hostname.assign(buffer);
    if(hostname.length() == 0)
        hostname = "localhost";
    cin.clear();
    do {
        cout << "What is the database? (don't forget, with MongoDB it doesn't need to exist already): ";
        cin >> database;
    } while(database.length() == 0);
    do {
        cout << "Does this server use authentication? (Y/N) ";
        cin.clear();
        cin.getline(buffer,255);
    } while((buffer[0] != 'y') && (buffer[0] != 'Y') && (buffer[0]!= 'n') && (buffer[0] != 'N'));
    switch(buffer[0]) {
        case 'y':
        case 'Y':
            useauth = true;
            break;
        case 'n':
        case 'N':
            useauth = false;
            break;
    }
    if(useauth) {
      cout << "Do you want me to create the user on the database? (say yes if the database does not yet exist) (Default: No): ";
      do {
          cin.clear();
          cin.getline(buffer,255);
          if(buffer[0] == '\0')
          buffer[0] = 'n';
      } while((buffer[0] != 'y') && (buffer[0] != 'Y') && (buffer[0] != 'n') && (buffer[0] != 'N'));
      switch(buffer[0]) {
          case 'y':
          case 'Y':
              makeauth = true;
              break;
          case 'n':
          case 'N':
              makeauth = false;
              break;
      }
          cout << "What is the username? :";
          cin.clear();
          cin >> uname;
          useauth = true;
          cout << "What is the password?: ";
          cin.clear();
          cin >> password;
          if(makeauth) {
          cout << "What is the admin username? :";
          cin.clear();
          cin >> auname;
          useauth = true;
          cout<< "What is the admin password?: ";
          cin.clear();
          cin >> apassword;
      }
    }
    fstream file;
    file.open("blog.cnf",ios_base::out);
    if(file.fail()) {
        cerr << "error opening file" <<endl;
        return 1;
    }
    file << "hostname:" << hostname << endl;
    file << "database:" << database << endl;
    file << "uname:" << uname << endl;
    file << "password:" << password << endl;
    file << "useauth:" << useauth << endl;
    file.close();
    //now to actually do this:
    mongo::DBClientConnection con;
    try {
        con.connect(hostname);
    }
    catch(DBException &e ) {
        cout << "This probably means the database server is down. Please come back later." << endl;
        exit(0);
    }
    string error;
    if(useauth) {
        if(makeauth) {
            con.auth("admin",auname,apassword,error,true);
            stringstream hashbase;
            hashbase << uname << ":mongo:" << password;
            BSONObj p = BSON("user" << uname << "pwd" << MD5(hashbase.str()).hexdigest());
            stringstream namespacess;
            namespacess << database << ".system.users";
            con.insert(namespacess.str(),p);
            
        }
    }
    
    string un;
    string pw;
    cin.clear();
    cout << "Do you want me to create the admin user on the blog? (say yes if this is a new install): ";
      do {
          cin.clear();
          cin.getline(buffer,255);
      } while((buffer[0] != 'y') && (buffer[0] != 'Y') && (buffer[0] != 'n') && (buffer[0] != 'N'));
      switch(buffer[0]) {
          case 'y':
          case 'Y':
              cout << "Username: ";
              cin >> un;
              cout << "Password: ";
              cin >> pw;
              stringstream namespacess;
              namespacess << database << ".users";
              string error;
              con.auth(database,uname,password,error,true);
              cout << error;
              con.dropCollection(namespacess.str());
              con.insert(namespacess.str(),BSON("UserName" << un << "Password" << MD5(pw).hexdigest() << "is_admin" << 1));
              con.ensureIndex(namespacess.str(),BSON("UserName" << 1),true);
              break;
      }
    return 0;
}