# Cshell

A mini shell implemented in C that reads user commands, parses input, and executes them.  
Processes are managed using `fork`/`exec`, and several built-in commands are supported.

## Current Features

- **Built-in commands**:
  - `cd [directory]`: change the current working directory (supports `~` for home and no value as well).
  - `exit`: exit the shell.
  - `history`: display previously executed commands.

- **Command history**:
  - Commands are saved to a `.cshell_history` file in the user's HOME directory.
  - History includes all executed commands, even those run with `;` or `&&`.

- **Sequential execution and control flow**:
  - Supports multiple commands separated by `;` (sequential execution).
  - Supports `&&` to execute the next command only if the previous one succeeds.

- **External command execution**:
  - Any command not built-in is executed via `execvp` if it exists.

- **User input sanitizing**:
  - Parses arguments with proper handling of spaces and special operators.
  - Replaces `~` with the HOME directory for all commands.

