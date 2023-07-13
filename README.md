# ms

Usage
```bash
# interactive mode
./minishell
# simple mode
./minishell "<cmd>"
```

Example
```bash
./minishell "/usr/bin/ls | cat"
```

# Tests


## Execution order with errors

```bash
echo bam > cant_write.txt | ls
# ls command should still execute
# Example:
$>echo bam > cant_write.txt | ls
minishell: permission denied: cant_write.txt
bin   Desktop	 Downloads  installs  Pictures	sgoinfre   Videos
code  Documents  goinfre    Music     Public	Templates
```

```bash
echo bam > cant_write.txt | cat
# cat command should still execute on stdout of echo command, not on stdin!
```

```bash
cat < cant_read.txt > cant_write.txt
# command execution should stop at first error: promt -->
# Example:
$>cat < cant_read.txt > cant_write.txt
minishell: permission denied: cant_read.txt
```
