# ray-tracing
Only scene 6 (Cornell Box) work properly.
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

| Command  | Verbose  | Definiton | Options |
| :--- | :--- | :---  | :--- |
| -i  | --integrator  | Select integrator | naive, bdpt, mlt  |
| -s  | --scene | Scene selection | 6 |
| -b  | --bounce | Max path length | Integer >= 3 |
| -spp  | --sample | Samples per pixel |  Integer >= 1|
| -o  | --output | Name of the output file | string |



```sh
$ ./ray-tracing 
```

| Command  | Default Value |
| :--- | :---  |
| -i  | bdpt  |
| -s  | 6 |
| -b  | 8 |
| -spp  | 1 |
| -o  | output  |

