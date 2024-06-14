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

header_file="${new_file_name}.hpp"
source_file="${new_file_name}.cpp"


source_comments0="
// #------------------------------#
// |          includes            |
// #------------------------------#
// c/c++ includes

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
// c/c++ includes

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

function create_parent_dirs() {
    local file_path="$1"
    local dir_path

    # Extract the directory path from the file path
    dir_path=$(dirname "$file_path")

    # Create the directories
    mkdir -p "$dir_path"
}


function create_header () {
    #echo "func()"
    guard="__${current_folder^^}_${new_file_name^^}_HPP__"
    guard=$(echo "$guard" | sed 's/-/_/g')
    guard=$(echo "$guard" | sed 's/\//_/g')
    #echo "$guard"
    file_path="${1}/${header_file}"
    create_parent_dirs "$file_path"
    printf  "#ifndef ${guard}\n#define ${guard}" > $file_path
    printf  "${header_comments}\n" >> $file_path
    printf  "#endif\n" >> $file_path

}

function create_source () {
    file_path="${1}/${source_file}"
    create_parent_dirs "$file_path"
    printf  "${source_comments0}" > $file_path
    printf  "#include \"${header_file}\"" >> $file_path
    printf  "${source_comments1}" >> $file_path

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

