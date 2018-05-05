#! /usr/bin/awk -f

BEGIN { suppression=0; md5sum = "md5sum" }
  # If the line begins with '{', it's the start of a supression; so set the var and initialise things
  /^{/  {
           suppression=1;  i=0; next 
        }
  # If the line begins with '}' its the end of a suppression
  /^}/  {
          if (suppression)
           { suppression=0;
             close(md5sum, "to")  # We've finished sending data to md5sum, so close that part of the pipe
             ProcessInput()       # Do the slightly-complicated stuff in functions
             delete supparray     # We don't want subsequent suppressions to append to it!
           }
     }

 # Otherwise, it's a normal line. If we're inside a supression, store it, and pipe it to md5sum. Otherwise it's cruft, so ignore it
     { if (suppression)
         { 
            supparray[++i] = $0
            print |& md5sum
         }
     }
 
 
 function ProcessInput()
 {
    # Pipe the result from md5sum, then close it     
    md5sum |& getline result
    close(md5sum)
    # gawk can't cope with enormous ints like $result would be, so stringify it first by prefixing a definite string
    resultstring = "prefix"result
 
    if (! (resultstring in chksum_array) )
      { chksum_array[resultstring] = 0;  # This checksum hasn't been seen before, so add it to the array
        OutputSuppression()              # and output the contents of the suppression
      }
 }
 
 function OutputSuppression()
 {
  # A suppression is surrounded by '{' and '}'. Its data was stored line by line in the array  
  print "{"  
  for (n=1; n <= i; ++n)
    { print supparray[n] }
  print "}" 
 }
