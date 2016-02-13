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
    FileWorker();
    FileWorker(std::string indir, std::string outdir, std::string command);
    void process_files();
    void start_thread();
    void add_file(std::string filename);             //filename without dir
    void set_valid_extension(std::string extension); //not case sensitive
    std::string get_indir(){return indir;}
private:
    std::string indir;
    std::string outdir;
    std::string get_cmd(std::string infile, std::string outfile);
    virtual void process_file(std::string infile, std::string outfile);
    std::list<std::string> files;
    std::set<std::string> valid_extensions;

    std::thread *worker_thread;
    bool thread_is_running;
    std::mutex accessing_list;
};

class FWSysCmd:public FileWorker
{
    FWSysCmd(std::string indir, std::string outdir, std::string command);
    void process_file(std::string infile, std::string outfile);
private:
    std::string command;
};

#endif // FILEWORKER_H
