#include <iostream>
#include <sys/inotify.h>
#include <unistd.h>
#include <string>
#include "fileworker.h"

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

using namespace std;



int main(int argc, char *argv[])
{
    FileWorker fw("/home/koehler/in", "/home/koehler/out");
    fw.set_valid_extension(".jpeg");
    fw.set_valid_extension(".jpg");
    fw.set_valid_extension(".png");
    char buffer[EVENT_BUF_LEN];
    /*creating the INOTIFY instance*/
    int fd = inotify_init();
    int wd = inotify_add_watch( fd, "/home/koehler/in", IN_CREATE  );
    while(1){
        int  length = read( fd, buffer, EVENT_BUF_LEN );
        int i = 0;
        while ( i < length ) {
            struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
            if ( event->len && event->mask & IN_CREATE ) {
                fw.add_file(string(event->name));
            }
            i += EVENT_SIZE + event->len;
        }
        fw.start_thread();
    }
    return 0;
}
