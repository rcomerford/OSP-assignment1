# 📂 OSP Assignment 1
Built by Ryan Comerford (s3945787)

## 🚧 Building
- Compile all using `make`.
- Compile only `copier` using `make copier`.
- Compile only `mtcopier` all using `make mtcopier`.
- Clean build files using `make clean`.

> NOTE: when writing to files within a folder, folder must already be present. Neither program will create a folder to store an output file. Instead, behavior will be: `ERROR:    Files failed to open.`

## 🔴 Single-Threaded ("copier")
``` bash
./copier <infile> <outfile>  [-d] [-t]
```

- infile: the input file for the copier.
- outfile: the file to output the copy to.
- [OPTIONAL] -d: enable debug console outputs.
- [OPTIONAL] -t: enable timing outputs.

## 🔵 Multi-Threaded ("mtcopier")
``` bash
./mtcopier <numthreads> <infile> <outfile> [-d] [-t]
```

- numthreads: number of threads to use (25-100 inclusive).
- infile: the input file for the copier.
- outfile: the file to output the copy to.
- [OPTIONAL] -d: enable debug console outputs.
- [OPTIONAL] -t: enable timing outputs.