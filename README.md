# ray-tracing
## Build
```sh
$ mkdir build
$ cd build 
$ make
```

## Run
```sh
$ ./ray-tracing -s 6 -spp 8 -b 8 -o outputFile
```

| Command  | Verbose  | Definiton |
| :--- | :--- | :---  |
| -s  | --scene | Scene selection |
| -b  | --bounce | Max path length |
| -spp  | --sample | Samples per pixel |
| -o  | --output | Name of the output file |



```sh
$ ./ray-tracing 
```

| Command  | Default Value |
| :--- | :---  |
| -s  | 6 |
| -b  | 8 |
| -spp  | 1 |
| -o  | output  |

