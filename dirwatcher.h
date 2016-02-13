#ifndef DIRWATCHER_H
#define DIRWATCHER_H

#include <map>
#include <utility>
#include <string>
#include <sys/inotify.h>
#include "fileworker.h"

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

typedef std::pair<std::string,std::string> str_pair;

class DirWatcher
{
public:
    DirWatcher();
    ~DirWatcher();
    void add_dir(std::string indir, std::string outdir);
    void add_FileWorker(FileWorker *fw);
    void watch();
private:
    int fd;
    std::map<int,FileWorker*> wd_fw;
};

#endif // DIRWATCHER_H
