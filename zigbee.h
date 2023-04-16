/* Append CRC of command to the given command, ignoring all size limits (be careful that command is big enough) */
#define APPEND_CRC(command) strcat(command, checkSum(command))