# java-launcher
Simple Python/Bash script and C program for launch java apps on Linux

## Configuration

You need set the configuration directly on the script. For purposes of example, the default configuration is:

**Python**

```
version = 7                              #Min version of required Java
mainclass = "cl.rvillablanca.Main"       #Main class
classpath = ""                           #Class path of your application (Jars and folders)
javaoptions = ["-Xms256m", "-Xmx512m"]   #Set up Java configurations
programname = "Launcher Example"         #Your program name
isjar = True                             #True if you will run a jar file
jarfile = "jar/launcher.jar"             #The jar file name if 'isjar' is True
```

**Bash**

```
version=7                                #Min version of required Java
mainclass="cl.rvillablanca.Main"         #Main class
classpath=""                             #Class path of your application (Jars and folders)
javaoptions=("-Xms256m" "-Xmx512m")      #Set up Java configurations
programname="Launcher Example"           #Your program name
isjar=1                                  #1 if you will run a jar file
jarfile="jar/launcher.jar"               #The jar file name if 'isjar' is True
```

Maybe you would like rename the file like `mv launcher.py myprogram` or `mv launcher.sh myprogram` and then run it with `./myprogram` or `./myprogram firstArg secondArg`

**Running example**

```
./launcher.py
./launcher.sh

./launcher.py firstArg secondArg
./launcher.sh firstArg secondArg
```
