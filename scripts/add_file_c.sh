#!/bin/bash

if [[ $1 == "--help" ]]; then
    echo "  adding source and header file TODO"
    exit 0
fi

if [ $# -lt 1 ]; then
    echo "too little args \n please provide file name"
    exit
fi

# echo "$new_file_name is new file"
include_folders_names=("include" "inc")
source_folders_names=("source" "src")

cwd=$(pwd)
# dirs=($(echo $cwd | tr "," "/"))
IFS='/ ' read -r -a dirs <<< "$cwd"
current_folder=${dirs[-1]}
# echo "cwd: ${current_folder}"
if [[ " ${include_folders_names[*]} " =~ " ${current_folder} " ]]; then
    # echo "we are in header go back"
    cd ..
fi
if [[ " ${source_folders_names[*]} " =~ " ${current_folder} " ]]; then
    # echo "we are in source go back"
    cd ..
fi

cwd=$(pwd)
# dirs=($(echo $cwd | tr "," "/"))
IFS='/ ' read -r -a dirs <<< "$cwd"
current_folder=${dirs[-1]}

new_file_name=$1

header_file="${new_file_name}.h"
source_file="${new_file_name}.c"


source_comments0="
// #------------------------------#
// |          includes            |
// #------------------------------#
// c includes

// my includes
"
source_comments1="

// #------------------------------#
// |           macros             |
// #------------------------------#

// #------------------------------#
// | global variables definitions |
// #------------------------------#

// #------------------------------#
// | static variables definitions |
// #------------------------------#

// #------------------------------#
// | static functions declarations|
// #------------------------------#

// #------------------------------#
// | global function definitions  |
// #------------------------------#

// #------------------------------#
// | static functions definitions |
// #------------------------------#
"
header_comments="
// #-------------------------------#
// |           includes            |
// #-------------------------------#
// c includes

// my includes

// #-------------------------------#
// |            macros             |
// #-------------------------------#

// #-------------------------------#
// | global types declarations     |
// #-------------------------------#

// #-------------------------------#
// | global variables declarations |
// #-------------------------------#

// #-------------------------------#
// | global function declarations  |
// #-------------------------------#

// #-------------------------------#
// |  global function definitions  |
// #-------------------------------#
"


function create_header () {
    #echo "func()"
    guard="__${current_folder^^}_${new_file_name^^}_H__"
    guard=$(echo "$guard" | sed 's/-/_/g')
    #echo "$guard"

    printf  "#ifndef ${guard}\n#define ${guard}" > "${1}/${header_file}"
    printf  "${header_comments}\n" >> "${1}/${header_file}"
    printf  "#endif\n" >> "${1}/${header_file}"

}

function create_source () {
    printf  "${source_comments0}" > "${1}/${source_file}"
    printf  "#include \"${header_file}\"" >> "${1}/${source_file}"
    printf  "${source_comments1}" >> "${1}/${source_file}"

}


# assume that we are on top of tree and have src and inc folder in cwd

files=$(ls)
# echo "cwd files = $files"
readarray -t files_arr <<<"$files"
# files_arr=($(echo $files | tr "," "\n"))

for file in ${files_arr[@]};
do
    # debug
    # echo "file: $file"
    if [[ " ${include_folders_names[*]} " =~ " ${file} " ]]; then
        # echo "inc file: $file"
        create_header  $file
    fi
    if [[ " ${source_folders_names[*]} " =~ " ${file} " ]]; then
        # echo "src file: $file"
        create_source $file
    fi
done;



#cwd=$(pwd)

