# java-launcher
Simple Python script for launching java apps on Linux

**Configuration**

You need set the configuration directly on the script. For purposes of example, the default configuration is:

```
version = 7                              #Min version of required Java
mainclass = "cl.rvillablanca.Main"       #Main class
classpath = ""                           #Class path of your application (Jars and folders)
javaoptions = ["-Xms256m", "-Xmx512m"]   #Set up Java configurations
programname = "Launcher Example"         #Your program name
isjar = True                             #True if you will run a jar file
jarfile = "jar/launcher.jar"             #The jar file name if 'isjar' is True
```

**Running example**

```
./launcher

./launcher firstArg secondArg
```
