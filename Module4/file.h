#pragma once

int Sys_FileOpenRead(char *Path, int *Size);
int Sys_FileOpenWrite(char *Path);
void Sys_FileClose(int handle);
void Sys_FileSeek(int Handle, int Position);
int Sys_FileRead(int Handle, void *Dest, int Count);
int Sys_FileWrite(int Handle, void *Source, int Count);
