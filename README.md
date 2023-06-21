# simple_cpu

## Description
During the laboratory work, it was necessary to implement its own assembly language, which is compiled into a binary file, and then executed on a virtual processor. To demonstrate that memory accesses are quite expensive in memory, operations with them were specially slowed down using the sleep system call

Descriptions of the language commands can be found in the files [cmd_definitions.h](src/cmd_definitions.h) and [cmd_constants.h](src/cmd_constants.h)
## Usage
``` $ chmod u+x launch.sh ```
``` $ ./launch.sh ```
``` $ cd bin ```

``` $ ./pseudo_make.exe [asm_programm_file_name] ```
``` $ ./asm.exe [asm_programm_file_name] ```
``` $ ./cpu.exe [asm_bin_file_name] ```
``` $ ./disasm.exe [asm_bin_file_name] ```

## Examples
As examples of executable programs, programs were written fact.txt (calculating the factorial of the entered number), quadratic.txt (calculating the root of a quadratic equation given by the coefficients entered) and circle.txt (drawing a circle on virtual graphics memory).

