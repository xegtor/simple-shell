# simple-shell
## Contributions
- Swapnil: the creation of the history feature, implementation of the bonus part, as well as the handling of the pipelining features built into the simple shell.
- Rohak: handling the input using the terminal and implementing all of the `execvp` commands and the `./` command.
## Features of the program
- can accept input from either the terminal or from a bash script.
- executes the following commands :
    - `ls`: lists all the contents of a directory; all other options are the same as the unix terminal.
    - `echo`: repeats the string of text typed in after it.
    - `WC`: returns word, line, etc., counts the same as the Unix terminal.
    - `rmdir`: deletes a directory with options mimicking those of the Unix terminal.
    - `mkdir`: makes a directory at the specified path with options mimicking those of the Unix terminal.
    - `rm`: removes specified files with options mimicking those of the Unix terminal.
    - `mv`: moves a file from one dir to another with options mimicking the Unix terminal's.
    - `cp`: copy files from one dir to another with options mimicking the Unix terminal's.
    - `pwd`: returns the path of the current working directory.
    - `Uniq`: returns the unique lines in a specified file with options mimicking the Unix terminal's.
    - `grep`: find patterns in files with options mimicking the Unix terminal's.
    - `sort`: sorts the contents of a file with options mimicking the Unix terminal's.
    - `cat`: prints the contents of a file with options mimicking the Unix terminal's.
    - `./`: runs an executable file with options mimicking the Unix terminal's.
    - `history`: prints a list of all the commands the user has typed in until that point.
    - `pipe(|)`: 
- Exiting the simple shell displays all the relevant information about the commands and the child processes they were executed.
- can run the command in the background while the rest of the simple shell keeps going.
## Limitation
- `No Environment Variable Management`: Managing environment variables to customize the shell's behaviour or store data is not supported. This feature is common in Unix shells but not present here. We could not implement this because we do not have system-level access.
- `No Custom Shell Built-Ins`: Custom shell built-ins like `cd`, `exit`, `alias`, and `export` are not implemented. These built-ins are essential for core-shell functionality but cannot be easily added using the exec family of functions.
- `No Input/Output Redirection`: The shell lacks support for input/output redirection (<, >, >>). These features enable manipulating input/output streams, which are complex to implement with exec.
- `No Job Control`: Job control, which allows managing background and foreground processes, suspending, resuming, and killing processes, is not implemented.
- `Security Considerations`: The shell may have security vulnerabilities, such as limited input validation, which can pose risks if handling untrusted user inputs. Robust security mechanisms are necessary in production-grade shells.
## [GitHub Repository](https://github.com/xegtor/simple-shell)
