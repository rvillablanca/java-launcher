#!/usr/bin/env python3

#Set launcher configuration
###########################

version = 7
mainclass = "cl.rvillablanca.Main"
classpath = ""
javaoptions = ["-Xms256m", "-Xmx512m"]
programname = "Launcher Example"
isjar = True
jarfile = "jar/launcher.jar"

###########################

import subprocess
import sys
import os

def execute_and_check_version(path, searched_version):
    try:
        out = subprocess.check_output([path, "-version"], stderr = subprocess.STDOUT)
        encoding = sys.getdefaultencoding()
        out = out.splitlines()[0].decode(encoding)
        idx = out.index("version")
        out = out[idx + 7:].strip().replace("\"", "")
        out = int(out[2])
        return out >= searched_version
    except Exception:
        return False
    
def run_program(command, options, classpath, mainclass, isjar, jarfile):
    command = command.split()
    if options:
        command.extend(options)
    if isjar:
        command.append("-jar")
        command.append(jarfile)
    else:
        if classpath:
            command = command + ["-cp", classpath]
        command.append(mainclass)
    args = sys.argv[1:]
    if args:
        command = command + args
    subprocess.call(command)
    
def msg_not_found():
    print("Cannot find java, be sure to install it (\"" + programname + "\" require java version 1." + str(version) + "+)")
    
try:
    found = False
    path = "java"

    found = execute_and_check_version(path, version)
    if found:
        run_program(path, javaoptions, classpath, mainclass, isjar, jarfile)
    else:
        java_home = os.environ.get("JAVA_HOME")
        if java_home:
            path = java_home + "/jre/bin/java"
            found = execute_and_check_version(path, version)
            if found:
                run_program(path, javaoptions, classpath, mainclass, isjar, jarfile)
            else:
                path = java_home + "/bin/java"
                found = execute_and_check_version(path, version)
                if found:
                    run_program(path, javaoptions, classpath, mainclass, isjar, jarfile)
                else:
                    msg_not_found()
        else:
            msg_not_found()
except KeyboardInterrupt:
    print("Cancelado por usuario")
