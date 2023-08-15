# 📂 OSP Assignment 1
Built by Ryan Comerford (s3945787)

## 🔴 Single-Threaded ("copier")
``` bash
./copier <infile> <outfile>
```

- infile: the input file for the copier.
- outfile: the file to output the copy to.

## 🔵 Multi-Threaded ("mtcopier")
``` bash
./mtcopier <numthreads> <infile> <outfile> [-d]
```

- numthreads: number of threads to use (25-100 inclusive).
- infile: the input file for the copier.
- outfile: the file to output the copy to.
- [OPTIONAL] -d: enable debug console outputs.