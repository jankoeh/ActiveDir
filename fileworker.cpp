#include "fileworker.h"

#include <algorithm>
#include <iostream>


void thread_helper(AbstractFileWorker *fw){
    fw->process_files();
}

AbstractFileWorker::AbstractFileWorker()
{
    thread_is_running = false;
    worker_thread = NULL;
}

AbstractFileWorker::AbstractFileWorker(std::string indir, std::string outdir):
    indir(indir+"/"), outdir(outdir+"/")
{
    thread_is_running = false;
    worker_thread = NULL;
}

void AbstractFileWorker::process_files(){
    thread_is_running = true;
    while(1){
        accessing_list.lock();
        if(files.empty()){
            break;
        }
        std::string filename = files.front();
        files.pop_front();
        accessing_list.unlock();
        int dotpos = filename.rfind(".");
        if (dotpos < 0){
            dotpos = filename.length();
        }
        std::string extension = filename.substr(dotpos);
        std::transform(extension.begin(), extension.end(),
                       extension.begin(), ::tolower);
        if(valid_extensions.find(extension)==valid_extensions.end() &&
           valid_extensions.find(".allowall")==valid_extensions.end() &&
           valid_extensions.size()!=0){
            std::cout <<"Invalid extension: "<<extension<<" for "<<filename<<std::endl;
            continue;
        }
        //do work
        process_file(indir+filename,outdir+filename);
    }
    accessing_list.unlock();
    thread_is_running = false;
}

void AbstractFileWorker::start_thread(){
    if (!thread_is_running){
        delete worker_thread; //not neccessary because of detach?
        worker_thread = new std::thread(thread_helper, this);
        worker_thread->detach();
    }
}

void AbstractFileWorker::add_file(std::string filename){
    accessing_list.lock();
    files.push_back(filename);
    accessing_list.unlock();

}

void AbstractFileWorker::add_valid_extension(std::string extension){
    if(extension[0] != '.'){
        extension = "."+extension;
    }
    std::transform(extension.begin(), extension.end(),
                   extension.begin(), ::tolower);
    valid_extensions.insert(extension);
}


SysCmdFileWorker::SysCmdFileWorker(std::string indir, std::string outdir, std::string command):
    command(command),AbstractFileWorker(indir, outdir){;}

void SysCmdFileWorker::process_file(std::string infile, std::string outfile){
    std::string cmd = command;
    while(cmd.find("F_IN") != std::string::npos){
        int pos = cmd.find("F_IN");
        cmd.replace(pos, 4, infile);
    }
    while(cmd.find("F_OUT") != std::string::npos){
        int pos = cmd.find("F_OUT");
        cmd.replace(pos, 5, outfile);
    }
    std::cout <<cmd<<std::endl;
    system(cmd.c_str());
}
