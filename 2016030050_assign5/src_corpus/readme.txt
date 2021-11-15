------------------
LAB4 IS NOW FIXED.|
------------------
The two problems with lab4 was wrong fingerprint and no code testing.
I fixed the md5 problem (just some lines of code in md5 encryption).
Also now lab4_testing has been made and work fine
	->with "make run" you run the test that makes 50 files in folder files makes 20 file_9 modifications and tried to write on multiple files with no permission
In this process i found an other bug that has to do with fileChecking ,the problem was that isActionDenied flag was wrong.So i create a "file_check function" that handle this issue and now is resolved.
I am adding here as a comment that LAB4 issues i had have nothing to do with LAB5 implementation.
So i hope that help me with LAB4 grade as well :) .
-----------------------------------------------
----
LAB5|
----
test_aclog code get optimized to help with ransomware functions for example:
Ransomware script creates files calling:
	->LD_PRELOAD=./logger.so ./test_aclog -c $1
	create <X> number of files
 	and encrypt files with:
	->LD_PRELOAD=./logger.so ./test_aclog -e $output -s $encryptedfile
	get the output of encryption and the name of file and feed them on our C functions so thath they work fine.
I remind that we got a function that Parse the file_logging.log from lab_4 so we analyze our data in a Struct Table form:
	"struct entry * getLogsTable(char * logData,int * sizeOfArray)"
Last 20 minutes total number of creations -v <X>:
	->gets time from string and finds with "difftime function" the differnce if its less than 1200 seconds it increases the counter,and if the counter is more than <X> we print it.
Print -e :
	->Find all the files with ".encrypt" at the end and print the file name that they just encrypted using "strtok function".


-----------------------------------------------
----------------
Comments for use|
----------------
	Try to avoid running if /home/.../files/.../src_corpus/files maybe brings some errors in a particullar function(edge cases).
	Also in my PC some warnings occure during compilation that they don't cause trouble and that runs with no warnings in other PCs.
	Warnings also occure to me during encryption from bashscript but not in other PCs.