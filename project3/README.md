#relativistic-resonance-decays

Before running anything, make sure your path/environment variables are set correctly. To do this on Mac OSX (no idea how it works on other OS), open your terminal, and type “cd”. This will take you to your home directory. Next, use the text editor of your choice to open .cshrc or .bashrc depending on which shell you’re using (I use csh). Make sure your environments for ROOT, GSL, and our EVENT program are configured, like so:

setenv LD_LIBRARY_PATH ''
setenv DYLD_LIBRARY_PATH ''                                                                                                                                                                    

setenv BASEDIR [/Users/imoo]

setenv EVENT ${BASEDIR}[/Desktop/Y1S2/6860/project3/executable]
setenv LD_LIBRARY_PATH ${EVENT}:${LD_LIBRARY_PATH}
setenv DYLD_LIBRARY_PATH ${EVENT}:${DYLD_LIBRARY_PATH}

#ROOT                                                                                                                                                                                   
setenv ROOTSYS ${BASEDIR}/root
setenv PATH ${ROOTSYS}/bin:${PATH}
setenv LD_LIBRARY_PATH ${ROOTSYS}/lib:${LD_LIBRARY_PATH}
setenv DYLD_LIBRARY_PATH ${ROOTSYS}/lib:${DYLD_LIBRARY_PATH}

#GSL                                                                                                                                                                                    
setenv GSL [/usr/local/opt/gsl]
setenv LD_LIBRARY_PATH ${GSL}/lib:${LD_LIBRARY_PATH}
setenv DYLD_LIBRARY_PATH ${GSL}/lib:${DYLD_LIBRARY_PATH}

In the above, replace anything in square brackets, […], with the appropriate paths on your machine: wherever you keep ROOT, GSL, and this project.

Now that your environment is configured, navigate to the project3 directory and go to the “executable” subdirectory.
If you see the file “libEvent.so”, run the command “make clean” so you can build the libraries and executables from scratch.
If you don’t see that file, or you just ran “make clean”, you already have the cleaned up version, so run “make”. It will probably give you plenty of errors, so open the Makefile with your favorite text editor. You’ll at least need to change “INSTALL_DIR” to the path on your machine that points to your gsl folder, but I can’t predict any other errors because they’d likely be something different about your environment or path variables. In that case go look through your .bashrc or .cshrc again to try to debug.
This step will probably take the longest…
Once you’ve fixed any possible problems that popped up, go back to the executable subdirectory and “make” again. Then assuming that worked, you’ll now have a shared library called “libEvent.so” which our programs will use. Now, type “make rrd” to run the test file which will use that shared library, and run our tests. Once that works, you can add any files you need compiled in that Makefile but be careful because Makefiles are finicky about spaces/tabs. Make sure lines are separated by returns, and breaks in commands are separated by tabs. We also can’t have multiple Makefiles in the same directory, so make sure you add your commands to the same Makefile. Also make sure you put all of your files into the src/ subdirectory.

