#ifndef FILEWORKER_H
#define FILEWORKER_H

#include <string>
#include <list>
#include <set>
#include <mutex>
#include <thread>


class FileWorker
{
public:
    FileWorker(std::string indir, std::string outdir);
    void process_files();
    void start_thread();
    void add_file(std::string filename);             //filename without dir
    void set_valid_extension(std::string extension); //not case sensitive
private:
    std::string indir;
    std::string outdir;
    std::list<std::string> files;
    std::set<std::string> valid_extensions;

    std::thread *worker_thread;
    bool thread_is_running;
    std::mutex accessing_list;
};

#endif // FILEWORKER_H
