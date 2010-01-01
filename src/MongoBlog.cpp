######################################################################
#   								     #
#   MongBlog - Superfast simple blogging platform                    #
#   (c) 2010 Michael Cullen http://cullen-online.com                 #
#   Released under the BSD licence as define in the                  #
#   accompanying LICENCE file. Please read it :-)                    #
#                                                                    #
######################################################################

#include <iostream>

void main() {
    //Required for a CGI:
    cout << "Content-type: text/plain" << endl << endl;
    cout << "Test CGI";
}
