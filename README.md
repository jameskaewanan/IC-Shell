# IC-Shell

An individual project developing a shell in C language. For my University's Principles of Computer Systems and Architecture Course


# Milestone Version History #

* __Version 0.1.0__
    * Implementation of Milestone 1
       * Basic shell implement, shell_loop(), commandHandler()
       * "echo" command
       * "!!" command
       * "exit" command

* __Version 0.2.0__
  * Implementation of Milestone 2
    * Implemented the ability to read scripts, scriptReader()
  * Added test script for Milestone 2, test.sh

* __Version 0.2.1__
  * Fixed an issue where "exit" would not return the correct exit code

* __Version 0.3.0__
  * Implementation of Milestone 3
    * Implemented the ability to run external programs/system calls in the foreground
  * Updated test script for Milestone 2, test.sh

* __Version 0.3.1__
  * Implemented a title screen, welcome.txt
  
* __Version 0.4.0__
  * Implementation of Milestone 4
    * Implemented signal handling for SIGTSTP and SIGINT, signalHandler()
    * Implemented "echo $?" command to return exit code of previous command
  * Added global exitCode
  * Extremely buggy but atleast it's something? ¯\_(ツ)_/¯
  
* __Version 0.4.1__
  * Addressed an oversight where "!!" returns the previous command but does not repeat said command
  
* __Version 0.5.0__
  * Implementation of Milestone 5
    * Input/Output Redirection, redirectionHandler()
    * Works well but still some issues such as Input Redirection not being properly implemented
    
* __Version 0.5.1__
  * Minor clean-up of the code and small visual improvements to outputs
  * Change input prompt's colour to blue for easier comprehension
  * Fixed an issue where system calls are not properlly handled (all system calls should now work without issue)
  * Input Redirection has been fully implemented should now function accordingly
  

# Bonus Implementations #

* __A title screen__

