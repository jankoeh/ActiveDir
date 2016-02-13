#include "fileworker.h"

#include <algorithm>
#include <iostream>


void thread_helper(FileWorker *fw){
    fw->process_files();
}

FileWorker::FileWorker()
{

}

FileWorker::FileWorker(std::string indir, std::string outdir, std::string command):
    indir(indir+"/"), outdir(outdir+"/"), command(command)
{
    thread_is_running = false;
    worker_thread = NULL;
}

void FileWorker::process_files(){
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
            std::cout << "Could not determine file extension: "<<filename<<std::endl;
            continue;
        }
        std::string basename = filename.substr(0, dotpos);
        std::string extension = filename.substr(dotpos);
        std::transform(extension.begin(), extension.end(),
                       extension.begin(), ::tolower);
        if(valid_extensions.find(extension)==valid_extensions.end()){
            std::cout <<"Invalid extension: "<<extension<<std::endl;
            continue;
        }
        //do work
        std::string cmd = get_cmd(indir+filename,outdir+filename);
        std::cout <<cmd<<std::endl;
        system(cmd.c_str());
    }
    accessing_list.unlock();
    thread_is_running = false;
}

void FileWorker::start_thread(){
    if (!thread_is_running){
        delete worker_thread; //not neccessary because of detach?
        worker_thread = new std::thread(thread_helper, this);
        worker_thread->detach();
    }
}

void FileWorker::add_file(std::string filename){
    accessing_list.lock();
    files.push_back(filename);
    accessing_list.unlock();

}

void FileWorker::set_valid_extension(std::string extension){
    std::transform(extension.begin(), extension.end(),
                   extension.begin(), ::tolower);
    valid_extensions.insert(extension);
}

std::string FileWorker::get_cmd(std::string infile, std::string outfile){
    std::string cmd = command;
    while(cmd.find("F_IN") != std::string::npos){
        int pos = cmd.find("F_IN");
        cmd.replace(pos, 4, infile);
    }
    while(cmd.find("F_OUT") != std::string::npos){
        int pos = cmd.find("F_OUT");
        cmd.replace(pos, 5, outfile);
    }
    return cmd;
}
