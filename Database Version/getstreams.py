#! /usr/bin/python3
import sys,os,subprocess


# THIS SCRIPT IS USED TO GET THE STREAMS AND RETURNS A FORM IN HTML.
# This is used by getstreams.php to display the streams.


username = sys.argv[1]
direct_output = subprocess.check_output("./dbFunctions -getStreams \"" + username + "\" ", shell=True).decode("utf-8")
print(direct_output)