#!/usr/bin/env bash

#Set launcher configuration
###########################

version=7
mainclass="cl.rvillablanca.Main"
classpath=""
javaoptions=("-Xms256m" "-Xmx512m")
programname="Launcher Example"
isjar=1
jarfile="jar/launcher.jar"

###########################

check_version() {
    out=$("$1" -version 2>&1) || return 0
    out=$(echo $out | awk -F '"' 'NR==1{print $2}')
    out=${out:2:1}
    if [ "$out" -ge "$version" ]; then
        return 1
    else
        return 0
    fi
}

run_program() {
    local command=$1
    args=("$@")
    args="${args[@]:1}"
    for opt in "${javaoptions[@]}"
    do
        command="$command ${javaoptions[$i]}" 
    done
    if [ "$isjar" -eq 1 ]; then
        command="$command -jar $jarfile"
    else
        if [ -n "$classpath" ]; then
            command="$command -cp $classpath"
        fi
        command="$command $mainclass"
    fi
    command="$command $args"
    exec $command
}

msg_not_found() {
    echo "Cannot find java, be sure to install it (\"$programname\" require java version 1.$version+)"
}

command=java
check_version $command

if [ "$?" -eq 1 ]; then
    run_program $command "$@"
else
    if [ -n "$JAVA_HOME" ]; then
        command="$JAVA_HOME/jre/bin/java"
        command2="$JAVA_HOME/bin/java"
        if [ -e "$command" ]; then
            run_program $command "$@"
        elif [ -e "$command2" ]; then
            run_program $command2 "$@"
        else
            msg_not_found
        fi
    else
        msg_not_found
    fi
fi

