#include <iostream>
#include "fileworker.h"
#include "dirwatcher.h"
using namespace std;

int main(int argc, char *argv[])
{

    DirWatcher dw;
    int i = 1;
    while(i<argc){
        cout <<argv[i]<<endl;
        if(strcmp (argv[i],"-h")==0){
            cout <<"Active Directory - v. 0.1"<<endl;
            cout <<"Watches directories for new files an applies a command which modiefies the given file or creates a resulting file in a target directory.";
            cout <<"Usage: "<<argv[0]<<" -s WATCH_DIR TARGET_DIR COMMAND file_ext1 file:ext2 "<<endl;
            cout <<"-h  Show this help and exit.";
            cout <<"-s  Execute Shell command. Syntax: '-s WATCH_DIR TARGET_DIR COMMAND'."<<endl;
            cout << "    COMMAND is an arbitrary shell command, input file shall be given as F_IN, desired output file ad F_OUT";
            cout <<"    Command is followed by a list of valid file extensions. If 'allowall'' or no extension is given, all files will be used.";
            return 0;
        }
        else if(strcmp (argv[i], "-s")==0){
            if(argc<i+4){
                cout <<"Wrong usage. Use option -h for help";
                return 1;
            }
            SysCmdFileWorker *fw = new SysCmdFileWorker(argv[i+1], argv[i+2], argv[i+3]);
            i+=4;
            while(i<argc){
                if(strcmp (argv[i], "-s")==0){
                    break;
                }
                fw->add_valid_extension(argv[i]);
                i++;
            }
            dw.add_FileWorker(fw);
        }
    }
    dw.watch();
    return 0;
}
