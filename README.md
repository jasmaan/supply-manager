# Supply Manager 

Command line inventory manager written in C. It loads shipment records from a file into a dynamic array and lets you view, add, remove, search, sort, and generate a report on them. Focus on manual memory management, structs, and sorting. 

## What it does

- Loads shipments from 'shipments.txt' into a dynamic array that grows with realloc as it fills (frees all memory on exit).
- Presents an option menu: view all, add, remove, search by type/supplier/date range, sort by amount/type/date, and generate a report.
- Sorts with qsort and custom comparator functions.
- Writes 'report.txt' with totals per type, each supplier's contribution percentage, and the top 3 most common types.
- Validates input throughout (bound checks, rejecting nonnumeric input where a number is expected).

## [HOW TO RUN]

```bash
gcc supply-manager.c -o supply-manager
./supply-manager
```

`shipments.txt` must be in the same directory as the executable.
