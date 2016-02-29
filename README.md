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
```

**Running example**
```
./launcher

./launcher firstArg secondArg
```
