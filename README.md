Active Dir
==========

Watches a directory for new files and perform arbitrary operations on those files.

TODO
----
* Extend to use arbirary command 
* implement class dirwatcher
* make sure operation is performed after (not during) file copy. E.g. via
```C++
boost::filesystem::fstream fileStream(filePath, std::ios_base::in | std::ios_base::binary);

if(fileStream.is_open())
{
    //We could open the file, so file is good
    //So, file is not getting copied.
}
else
{
    //Wait, the file is currently getting copied. 
}
``` 
