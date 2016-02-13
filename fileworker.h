#ifndef FILEWORKER_H
#define FILEWORKER_H

#include <string>
#include <list>
#include <set>
#include <mutex>
#include <thread>

class AbstractFileWorker
{
public:
    AbstractFileWorker();
    AbstractFileWorker(std::string indir, std::string outdir);
    void process_files();
    void start_thread();
    void add_file(std::string filename);             //filename without dir
    void add_valid_extension(std::string extension); //not case sensitive
    std::string get_indir(){return indir;}
private:
    std::string indir;
    std::string outdir;
    virtual void process_file(std::string infile, std::string outfile)=0;
    std::list<std::string> files;
    std::set<std::string> valid_extensions;

    std::thread *worker_thread;
    bool thread_is_running;
    std::mutex accessing_list;
};

class SysCmdFileWorker:public AbstractFileWorker
{
public:
    SysCmdFileWorker(std::string indir, std::string outdir, std::string command);
private:
    std::string command;
    void process_file(std::string infile, std::string outfile);
};

#endif // FILEWORKER_H
