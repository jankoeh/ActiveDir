Active Dir
==========

Watches a directory for new files and perform arbitrary operations on those files.

Usage Example:
--------------

* resize images from directory *in* to 20% of their size using imagemagick. `active_dir -s ./in  ./out "convert F_IN -resize 50% F_OUT`. 
* If the *in* directory should be kept clean, ` && rm F_IN" .jpg .png` could be appended.
* an arbirary number of shell commands can be given:  `active_dir -s in50  out "convert F_IN -resize 50% F_OUT" jpg    -s in20  out "convert F_IN -resize 20% F_OUT" jpg` 
* Check `resizer.sh` for a usage example. 

Build Instructions:
-------------------

```bash
git clone https://github.com/jankoeh/ActiveDir.git
cd ActiveDir
mkdir build
cd build
cmake ..
make
```

Known Problems:
---------------

* Files which are moved into the directory are ignored. I guess this is caused by the use of `IN_CLOSE_WRITE` events. Either fix this or declare ia a feature.


