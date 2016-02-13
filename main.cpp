#include <iostream>

#include "fileworker.h"
#include "dirwatcher.h"
#include <string.h>
using namespace std;

int main(int argc, char *argv[])
{

    DirWatcher dw;
    int i = 1;
    while(i<argc){
        cout <<argv[i]<<endl;
        if(strcmp (argv[i],"-h")==0){
            cout <<"Active Directory - v. 0.1"<<endl;
            cout <<"Watches directories for new files an applies a command which may create a resulting file in a target directory.";
            cout <<"Usage: "<<argv[0]<<" -R WATCH_DIR TARGET_DIR COMMAND ";
            i++;
        }
        else if(strcmp (argv[i], "-R")==0){
            SysCmdFileWorker *fw = new SysCmdFileWorker(argv[i+1], argv[i+2], argv[i+3]);
            //Error handling
            i+=4;
            while(i<argc){
                if(strcmp (argv[i], "-R")==0)
                    break;
                fw->add_valid_extension(argv[i]);
                cout <<argv[i]<<endl;
                i++;
            }
            dw.add_FileWorker(fw);
        }
    }
    dw.watch();
    return 0;
}
