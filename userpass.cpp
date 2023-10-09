#include <iostream>
#include <string>

#include "hashtable.h"
#include "pass_server.h"

void menu()
{
   cout << "\n\n";
   cout << "l - Load From File" << endl;
   cout << "a - Add User" << endl;
   cout << "r - Remove User" << endl;
   cout << "c - Change User Password" << endl;
   cout << "f - Find User" << endl;
   cout << "d - Dump HashTable" << endl;
   cout << "s - HashTable Size" << endl;
   cout << "w - Write to Password File" << endl;
   cout << "x - Exit program" << endl;
   cout << "\nEnter choice : ";
}

int main()
{
   string filename = " ";
   string user = " ";
   string pass = " ";
   string newPass = " ";
   char letter = ' ';

   PassServer<string, string> ps;   //psspspsppspspsp

   do{
      menu();
      cin >> letter;
      cout << endl;
      //cout << "WHAT" << ps.size() << endl;

      switch(letter){
         //Load From File
         case 'l':
            cout << "Please enter a filename: ";
            cin >> filename;
            cout << endl;
	         if(ps.load(filename.c_str())) //needs to be c string
               cout << "\tSuccess loading file." << endl;
            else
               cout << "\tError loading file." << endl;
         break;

         //add user
         case 'a':
            cout << "Please enter a username: ";
            cin >> user;
            cout << endl;
            cout << "Please enter a new password: ";
            cin >> pass; 
            cout << endl;
            if(!ps.find(user)){
               ps.addUser(make_pair(user, pass));
               cout << "\tSuccess adding " << user << endl;
            }
            else
               cout << "\tFailure adding " << user << endl;
         break;

         //Remove User
         case 'r':
            cout << "Please enter a username: ";
            cin >> user;
            cout << endl;
            if(ps.removeUser(user))
               cout << "\tSuccess removing " << user << endl;
            else
               cout << "\tFailure removing " << user << endl;
         break;

         //Change User Password
         case 'c':
            cout << "Please enter a username: ";
            cin >> user;
            cout << endl;
            cout << "Please enter the current password: ";
            cin >> pass; 
            cout << endl;
            cout << "Please enter the new password: ";
            cin >> newPass; 
            cout << endl;
            if(ps.changePassword(make_pair(user, pass), newPass))
               cout << "\tSuccess changing " << user << endl;
            else
               cout << "\tFailure changing " << user << endl;
         break;

         //Find User
         case 'f':
            cout << "Please enter a username: ";
            cin >> user;
            cout << endl;
            if(ps.find(user))
               cout << "\tSuccess finding " << user << endl;
            else
               cout << "\tFailure finding " << user << endl;
         break;

         //Dump HashTable
         case 'd':
            cout << "----- Beginning the dump -----\n";
            ps.dump();
            cout << "----- End -----\n";
         break;

         //HashTable Size
         case 's':
            cout << "\tThe current size is " << ps.size() << endl;
         break;

         //Write to Password File
         case 'w':
            cout << "Please enter a filename: ";
            cin >> filename;
            cout << endl;
	         if(ps.write_to_file(filename.c_str())) //needs to be c string
               cout << "\tSuccess writing " << filename << endl;
            else
               cout << "\tError writing " << filename << endl;
         break;

         case 'x':
            cout << "\tExiting ...\n"; //x message to exit
         break;
         
         default:
            cout << "\tInvalid Selection\n"; //error check
         break;
      }
      //cout << "WHAT" << ps.size() << endl; //fix off by 1, double count
   }while(letter != 'x');  //end condition
   cout << endl;
   return 0;
}
