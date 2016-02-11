#include "fileworker.h"

#include <algorithm>
#include <iostream>

void thread_helper(FileWorker *fw){
    fw->process_files();
}

FileWorker::FileWorker(const std::string indir, const std::string outdir):indir(indir+"/"), outdir(outdir+"/")
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
        std::string new_fn = outdir+basename+"_small"+extension;
        std::string command = "convert "+indir+filename+" -resize 50% "+new_fn;
        std::cout <<command<<std::endl;
        system(command.c_str());
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


