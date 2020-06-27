# Compressor
Compressing C/C++ code sources by deleting spaces/comments and renaming variables. Also generate header file, that you should add to your project.

### Building the project
Compilation line
```
cc compressor.c collections/array.c collections/common.c collections/hashtable.c -o compressor
```
or with Cmake

### Usage
```
compressor sourcefile1 sourcefile2 ...
```

### Warning
Compressor write changes directly
