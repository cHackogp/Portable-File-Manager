#include<fstream>
#include <iostream>
#include<unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<ctype.h>
#include <vector>
#include<dirent.h>
#include<limits>

#define MAX_FILE_NAME 100 // maximum file size
using namespace std;

void call_clear() // function to clear the console by printing the a lot of newlines
{
    for(int i = 0 ; i <50 ;i++)
        cout<<endl;
}
string get_working_path() // function to return the workingpath or the current directory the user is in , by default it is the directory in which the .exe file is stored
{
   char temp[100];
   return ( getcwd(temp, 100) ? string( temp ) : string("") ); // getcwd() takes in a characted array and returns the currently working directory
}


void command_split(string command , vector<string> &result) // function to split the command entered by the user using space as a delimitter and returns a vector of operation and parameters
{
    char* final_command =  new char[command.length()+1];
    strcpy(final_command, command.c_str());
    char* token;
    char* rest = final_command; 
     while ((token = strtok_r(rest, " ", &rest))) //strtok_r function to split the command string
        result.push_back(token);
}



void ls(vector<string> &list) // ls function returns a vector which contains the directory contents like files and folders.
{
	struct dirent *de;  // Pointer for directory entry 
  
    // opendir() returns a pointer of DIR type.  
    DIR *dr = opendir("."); 
  
    if (dr == NULL)  // opendir returns NULL if couldn't open directory 
    { 
        printf("Could not open current directory" ); 
    } 
  
    // for readdir():- to read the contents of the directory
    while ((de = readdir(dr)) != NULL) 
            {
            		list.push_back(de->d_name); // pushes the name of the directory in the vector
            } 
 
    closedir(dr);  // function to close the directory pointer.
}

void call_ls() // function to display the contents of the directory by calling the ls function
{
    vector<string> list; // vector to hold the name of files and folders
    ls(list); // function call to return the list of files and folders present in the directory

    if(list.size()!=0) // condtion to check if the directory is empty
        {
            for(int i = 0 ; i < list.size();i++) // parsing through the entire vector to print the name of file and folders present in the directory
            {
                if(list[i]!=".." && list[i]!=".")
                cout<<">>  "<<list[i]<<endl;
            }
        }
    else
        cout<<"\n   Empty Directory";

    cout<<endl;
}


bool call_cd(vector<string> result) // function to change directories
{

    char* final_parameter; // pointer to hold the name of the disired directory

    if(result.size()==2) // condition to check if the result vector has 2 instances that is the operation and the parameter
    {
        final_parameter =  new char[result[1].length()+1]; // initializing the final_parameter to hold the new directory name
        strcpy(final_parameter, result[1].c_str());
        if(chdir(final_parameter)==-1) // if the chdir() {command to change directories} is not successful
            cout<<"\n  Error opening Directory!";
    }
    else
    {
        cout<<"\n   Invalid Command";
        cout<<"\n   enter help for command list";
    }

    cout<<endl;
}

//Creates a file from the given parameter name (the first parameter will be the create call)
bool create(vector<string> parameters){
	//validate parameters
    if(parameters.size()<2) {
            cout<<"\n  Too few parameters";
            cout<<"\n   enter help for command list";
            return false;
	}

	//get the file name at origName
    char* origName = new char[parameters[1].length()+1];
    strcpy(origName,parameters[1].c_str());

	//Open the file (since none exist, one will be created by standard libraries
    ofstream outfile(origName);

	//free the file pointer
    outfile.close();

	//return that the file has successfully been created
    return true;
}

//deletes the file specified by the parameters given
bool delete_file(vector<string> parameters) {
    //validate parameters
    if(parameters.size()<2) {
            cout<<"\n  Too few parameters";
            cout<<"\n   enter help for command list";
            return false;
    }

	//get file name at origName
    char* origName = new char[parameters[1].length()+1];
    strcpy(origName,parameters[1].c_str());

	//attempt to open the file in read mode
    FILE* fp = fopen(origName, "r");

	//check if the file exists
    if (fp == NULL) {
        cout << "The file does not exists" << endl;;
        return false;
    }
    fclose(fp);		//free pointer

	//delete the file after confirmed it exists
    remove(origName);
    return true;
}

void help() // function to provide the users with a reference on how to use the functions
{
    cout<<"\n----------------Help--------------";
    cout<<"\nCommands";

    cout<<"\ncd - to navigate through directories\n";

    cout<<"\nls - to display content in a directory\n";
    
    cout<<"\ntexted - to access text editor and manipulate files in the current directory\n";
    
    cout<<"\nexit - to exit the program\n";

    cout<<"\nCreate filename";
    cout<<"\ncd text.txt\n"; 

    cout<<"\ndelete filename";
    cout<<"\ndelete text.txt\n"; 

    cout<<"\nrename orignal_fileman new_fileman";
    cout<<"\nrename text.txt hey.txt\n"; 

    cout<<"\ncopy orignal_fileman new_fileman";
    cout<<"\ncopy text.txt hey.txt\n"; 

    cout<<"\nMove orginal filname";
    cout<<"\nNavigate to the directory";
    cout<<"\nEnter 'here' to move the file";

    cout<<endl;
}

//copies the first file given to the second location given
bool copy(vector<string> parameters) {
    //validate parameters
    if(parameters.size()<3) {
            cout<<"\n  Too few parameters";
            cout<<"\n   enter help for command list";
            return false;
     }

	//get file names
    char* origName = new char[parameters[1].length()+1];
    strcpy(origName,parameters[1].c_str());
    char* newName = new char[parameters[2].length()+1];
    strcpy(newName,parameters[2].c_str());

	//attempt to open source file to confirm that it exists
    FILE* fp = fopen(origName, "r");
    if (fp == NULL)  {
        cout << "Unable to open source file" << endl;
        return false;
    }

    //open the new file (creating if does not exist)
    std::ofstream outfile(newName);

	//write to the new file from the contents of the first file (in intervals up to what buffer can hold)
    char buffer[100000];
    while (fgets(buffer, 100000-1, fp) != NULL){
        outfile << buffer;
    }

	//end the line of the file and free the destination file pointer
    outfile << std::endl;      
    outfile.close();

	//free the source file pointer
    fclose(fp);
    return true;
}

//changes the name of the file given by the parameters by creating a file under the new name and destroying the original
bool rename(vector<string> parameters) { // vector containing the old and the new filename
    if(parameters.size()<3)   { // condtion to check the number of parameters
            cout<<"\n  Too few parameters";
            cout<<"\n   enter help for command list";
            return false;
    }

	//get the file names
    char* origName = new char[parameters[1].length()+1]; // char pointer to hold the orignal filename
    strcpy(origName,parameters[1].c_str());
    char* newName = new char[parameters[2].length()+1]; // char pointer to hold the new filename
    strcpy(newName,parameters[2].c_str());

    vector<string> v;
    if (copy(parameters))               // makes a copy of the of the orignal file with a new file name
    {
        v.push_back(parameters[0]); // creating vector with the the old filename to be deleted
        v.push_back(parameters[1]);
        if (delete_file(v)) { // deleting the old file
            return true;
        }
    }

    return false;
}


void change_dir(string old_dir)  // to go back to the orignal directory for the move function
{
    char* final_parameter =  new char[old_dir.length()+1];
    strcpy(final_parameter, old_dir.c_str());
    chdir(final_parameter);
}

bool move(vector<string> parameters)
{
    if(parameters.size()<2) // condition to check the numeber of arguments sent to the function since it takes the operation and the filename
    {
        cout<<"\n  Too few parameters";
        cout<<"\n   enter help for command list";
        return false;
    }
    //open the file that has to be moved
    char* origName = new char[parameters[1].length()+1];  // char pointer to hold the filename to be moved
    strcpy(origName,parameters[1].c_str()); 
    FILE* fp = fopen(origName, "r"); // file pointer to the file
    if (fp == NULL)  // condtion to check if the file exists
    {
        cout << "Unable to open source file" << endl;
        return false;
    }


    string old_dir= get_working_path(); // function call to get the current working directory to bring the user back once the move process is done

    cout<<"\nNavigate to the directory :-";
    string nav; // string to hold the navigating command
    while(1)
    {
        cout<<get_working_path()<<">";
        getline(cin,nav); // getting the navigation
        vector<string> result;
        command_split(nav,result); // splitiing the user command
        string operation = result[0];

        if(operation=="cd") // command to change directories
        {
            call_cd(result);
        }
        else if(operation=="ls") // command to display the content in the directory
        {
            call_ls();
        }

        else if(operation=="here") // if the operation entered is here 
        {
            //open a file here and copy the contents from the orignal
            string newfile = "newfile.txt"; // name of the newfile
            char* newName = new char[newfile.length()+1];
            strcpy(newName,newfile.c_str());
            std::ofstream outfile(newName); // opening a new file in the navigated directory

            char buffer[1000]; // to hold the buffer from the first file

            while (fgets(buffer, 999, fp) != NULL) // copying the data from the orignal file to the newfile
            {
                outfile << buffer;
            }

            outfile << std::endl;       //add to file
            outfile.close();
            rename( newName, origName ); // renmaing the newfile with the old file


            change_dir(old_dir); // changing back to the old working directory to delete the orignal file

            //deleting the orignal file
            delete_file(parameters);
            // renaming the new file the orignal file


            fclose(fp); // closing the file pointer
            

            return true;
            

        }
        else if(operation=="cancel")
        {
            return false;
        }
        else
            cout<<"\nTRY again";
    }

    
    return true;
}


//Boolean function that returns true if the passed string contains .txt at the end
bool command_split(string command)
{
    char* final_command = new char[command.length() + 1];
    strcpy(final_command, command.c_str()); //Copying contents of the string to character pointer
    char* token;
    char* rest = final_command;
    vector<string> result;
    while ((token = strtok_r(rest, ".", &rest))) //while loop that goes through the string until it finds a '.'
        result.push_back(token);
    if (result.size() > 1 && result[1] == "txt") //Checking if the rest of the string is txt
        return true;
    return false;
}

//Function that creates a new text file or appends an existing one
void openAndAppend() {
    fstream txtfile;
    string fileName;
    string content;
    cout << "\nEnter text file to open:"; //Getting filename input from user
    cin >> fileName;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); //clear buffer before
    char* final_filename = new char[fileName.length() + 1];
    strcpy(final_filename, fileName.c_str());
    //Checks if user entered filename.txt or just filename
    if (command_split(fileName) == true) //User entered filename.txt, opens if it is a new file/appends if file exists
        txtfile.open(final_filename, ios::out | ios::app);
    else { //User entered just filename
        fileName.append(".txt"); //appending the filename with .txt, opens if it is a new file/appends if file exists
        final_filename = new char[fileName.length() + 1];
        strcpy(final_filename, fileName.c_str());
        txtfile.open(final_filename, ios::out | ios::app);
    }
    //Getting content to write to file
    cout << "\nEnter content:\n";
    getline(cin, content);
    txtfile << content; //writes to file
    txtfile.close(); //Closes file
}

//Funtion to insert text in a specific postion to an existing file 
void insertText() {
    FILE* fp;
    int count = 0;
    string filename;
    char c;
    int seek;
    string content, postContent;

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); //clear buffer before

    //Asking user which text file to open
    printf("\nEnter file name: ");
    getline(cin,filename);
    if (command_split(filename) == false) //if filename entered does not contain ".txt"
        filename.append(".txt"); //appeds filename with ".txt"

    char* final_filename = new char[filename.length() + 1];
    strcpy(final_filename, filename.c_str());

    fp = fopen(final_filename, "r");

    //If file does not exist
    if (fp == NULL) { 
        printf("\nCould not open file %s\n", final_filename);
        return;
    }
    //for loop to count the number of characters in file
    for (c = getc(fp); c != EOF; c = getc(fp))
        count = count + 1; //stores character count in count 
    fclose(fp);

    printf("\nEnter number of characters to seek in file %s (0 - %d):", final_filename, count);//Asking which position to insert text
    cin >> seek;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); //clear buffer before
    if (seek <= count) { //check if user entered an appropriate seek location
        fstream txtfile;
        txtfile.open(final_filename); //Opens file
        txtfile.seekg(seek, ios::beg); //moves pointer that reads from the file 'seek' distance from the begining of the file
        while (!txtfile.eof())
            getline(txtfile, postContent); //Stores the remaining content to the right of the file pointer into postContent 
        txtfile.seekg(seek, ios::beg); //sets pointer back to seek distance from the begining of the file
        //Gets text to be inserted from the user and adds it to file in the location entered by the user
        cout << "\nEnter content to add:";
        getline(cin, content);
        txtfile << content;
        txtfile << postContent; //Append the rest of the file after inserting new text 
        txtfile.close();
        cout << "Success!\n";
    }
    else { //if user entered seek distance greater than contents of the file
        cout << "\nFailed (Enter appropriate number to seek)\n";
    }

}

//Function that removes all contents of a text file
void removeText() {
    fstream txtfile;
    string fileName;
    cout << "\nEnter file to remove all text from:"; //User enters filename to be cleared
    cin >> fileName;
    char* final_filename = new char[fileName.length() + 1];
    strcpy(final_filename, fileName.c_str());
    if (command_split(fileName) == true) //If filename contains ".txt"
        txtfile.open(fileName, ios::out | ios::trunc); //open file and truncate/clear
    else { //else, if filename does not contain ".txt", append ".txt" to filename
        fileName.append(".txt");
        txtfile.open(fileName, ios::out | ios::trunc); //open file and truncate/clear
    }
    txtfile.close();
    cout << "Success!\n";
}

//Functoin to read contents of a file
void viewTextFile() {
    ifstream txtfile;
    string fileName, content;
    int lines, linesCounter = 0;
    //User enters filename to open
    cout << "\nEnter text file to read:";
    cin >> fileName;
    char* final_filename = new char[fileName.length() + 1];
    strcpy(final_filename, fileName.c_str());
    //If statement to check if the filename contains .txt, and opens the file appropriately 
    if (command_split(fileName) == true)
        txtfile.open(final_filename);
    else {
        fileName.append(".txt");
        final_filename = new char[fileName.length() + 1];
        strcpy(final_filename, fileName.c_str());
        txtfile.open(final_filename);
    }
    if (!txtfile) { //If file does not exist
        cout << "Unable to open file" << fileName;
        return;
    }
    //User enters number of lines to be displayed at once
    cout << "\nEnter number of lines to be displayed at once:";
    cin >> lines;
    txtfile.seekg(0, ios::beg);
    // //while (!txtfile.eof()) {
	// while(getline(txtfile, content) && content.empty) {
    //    ;
    // }

    cout << "\nPress Enter to read next page...\n";
    
    for (string::size_type i = 0; i < content.size(); i++) { //For loop that goes till the last character of the file
        //if linesCounter is equal to number of lines to be displayed at once,
        //wait for enter key from user, and reset linesCounter to 0
        if (linesCounter == lines) { 
            getchar();
            linesCounter = 0;
        }
        cout << content[i]; //displays content of a file character by character
        if (i == 0) //if it is the first character being displayed, continue loop, 
            continue; //we do this to prevent newline after the first character, and increment to linesCounter
        else if (i % 30 == 0) { //we set a line is equal to 30 characters
            cout << endl; // newline every 30 characters
            linesCounter++; //line counter increments by one
        }
    }
}

// menu for the text editor
void call_texted()
{
    char loopChoice;
    int choice;

    //do-while MENU
    do {
        cout << "\nMENU:\n1:Open and write/append a file\n2:Insert text in a specific position\n3:Remove all text in a file\n4:Display contents of a file\nEnter choice:";
        cin >> choice;
        switch (choice) {
        case 1:
            openAndAppend(); //Calls function to open and write/append a file
            break;

        case 2:
            insertText(); //Calls funtion to insert text in a specific position
            break;

        case 3:
            removeText(); //Calls function to remove all text in a file
            break;

        case 4:
            viewTextFile(); //Calls function to display contents of a file
            break;

        default:
            break;
        }

        cout << "\nDo you wish to continue? ( [Y]es/[N]o )";
        cin >> loopChoice;
        if (loopChoice == 'n' || loopChoice == 'N') //breaks out of loop if user enter 'n', or 'N'
            break;
    } while (1);

    cout<<endl;
}







bool check_valid_command(string command) // function to check if the command is valid and calling the respective functions 
{
    vector<string> result;
    command_split(command,result); // splitting the string to get the operation

    if(result.size()>0)
    {
        string operation = result[0];

        if(operation=="cd")
        {
            call_cd(result);
        }

        else if (operation=="ls")
        {
            call_ls();
        }

        else if (operation=="texted")
        {
            call_texted();
        }

        else if (operation=="help")
        {
            help();
        }

        else if (operation=="create")
        {
            create(result);
        }

        else if (operation=="copy")
        {
            copy(result);
        }

        else if (operation=="delete")
        {
            delete_file(result);
        }

        else if (operation=="rename")
        {
            rename(result);
        }

        else if(operation=="move")
        {
            move(result);
        }
        else if(operation=="clear")
        {
            call_clear();
        }

        else
        {
            cout<<"\n   Invalid Command";
            cout<<"\n   enter help for command list";
            return false;
        }
    }

    return true;

}

int main()
{
    string command; // string to hold the user command

    while(1)
    {
        
        cout<<get_working_path()<<">";
        getline(cin,command);
        if(command=="exit")
            break;
        else
            check_valid_command(command); // function call to check if the entered command is valid
    }



    return 0;
}