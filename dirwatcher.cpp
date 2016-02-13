#include "dirwatcher.h"
#include <unistd.h>


DirWatcher::DirWatcher()
{
    fd = inotify_init1(IN_NONBLOCK);
    //TODO: error check
}
DirWatcher::~DirWatcher()
{
    for(auto &entry : wd_fw){
        delete entry.second;
        wd_fw.erase(entry.first);
    }
}


void DirWatcher::add_FileWorker(FileWorker *fw)
{
    int wd = inotify_add_watch( fd, fw->get_indir().c_str(), IN_CLOSE_WRITE  );
    wd_fw[wd] = fw;
}

void DirWatcher::watch()
{
    fd_set watch_set;
    FD_ZERO( &watch_set );
    FD_SET( fd, &watch_set );
    char buffer[EVENT_BUF_LEN];
    while(1){
        select( fd+1, &watch_set, NULL, NULL, NULL );
        int  length = read( fd, buffer, EVENT_BUF_LEN );
        //TODO: error check
        int i = 0;
        while ( i < length ) {
            struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
            if ( event->len && event->mask & IN_CLOSE_WRITE ) {
                wd_fw[event->wd]->add_file(std::string(event->name));
                wd_fw[event->wd]->start_thread();
            }
            i += EVENT_SIZE + event->len;
        }
    }
}
