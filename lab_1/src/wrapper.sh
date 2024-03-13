#! /usr/bin/env bash

set -o errexit
set -o nounset
set -o pipefail


readonly TEST_DIR=$(pwd)/tests
readonly PROD_DIR=$(pwd)/prod_tests

function log_info()
{
    local message=${1}
    _log "INFO" "${message}"
}

function log_error()
{
    local message=${1}
    _log "ERROR" "${message}"
}


function _log()
{
    local level=${1}
    local message=${2}
    local now=$( date +"%Y-%m-%d %H:%M:%S") 
    echo "[${level}] [${now}] ${message}"
}

function main()
{   
    if [[ "$#" -ne 2 ]]; then
        log_error "There is invalid count of arguments"
        return 1
    else
        f1="$(basename "$1")"
        f2="$(basename "$2")"

        exe_file_name="$(basename "$1" | sed 's/\(.*\)\..*/\1/')"
        bench_file_name="$(basename "$2" | sed 's/\(.*\)\..*/\1/')"
        exe_file_ext="${f1##*.}"
        bench_file_ext="${f2##*.}"
    fi


    log_info "Started"
    log_info "Compiling..."
    if ! g++ ${exe_file_name}.${exe_file_ext} -o ${exe_file_name} ; then
        log_error "Compiling error"
        return 1
    fi
    log_info "Exe-file '"$exe_file_name"' successfully created"
    log_info "Inventing features..."
    rm -rf ${TEST_DIR} ${PROD_DIR}
    mkdir ${TEST_DIR}
    mkdir ${PROD_DIR}
    log_info "Directory "$TEST_DIR"' and '"$PROD_DIR"' successfully created"
    if ! ./test.py ${TEST_DIR} ; then
        log_error "Test-file compiling error"
        return 1
    fi
    log_info "Testing..."
    for test_file in $( ls ${TEST_DIR}/*.t ) ; do 
        local tmp_out=tmp
        if ! ./${exe_file_name} < ${test_file} > ${tmp_out} ; then
            log_error "Test run error"
            return 1
        fi
        local file_line_cnt=; file_line_cnt=$(cat ${test_file} | wc -l | sed -e 's/ *//g')
        local answer_file=${test_file%.*}.a
        if ! diff -u ${tmp_out} ${answer_file} ; then
            log_error "Failed to check test ${test_file}."
            return 1
        fi
        log_info "${test_file}, lines=${file_line_cnt} OK"
    done
    log_info "Benchmarking..."
    if ! g++ ${bench_file_name}.${bench_file_ext} -o ${bench_file_name} ; then
        log_error "Compiling benchmark error"
        return 1
    fi
    log_info "Exe-file '"$bench_file_name"' successfully created"

    for test_file in $( ls ${TEST_DIR}/*.t ) ; do
        log_info "Running '${test_file}'"
        if ! ./${bench_file_name} < ${test_file} ; then
            log_error "Failed to run '${bench_file_name}' for '${test_file}'."
            return 1
        fi
    done

    log_info "Cleaning..."
    if ! rm ${bench_file_name} ${exe_file_name} ${tmp_out} ; then
        log_error "Faild to delete '${bench_file_name}' '${exe_file_name}' ${tmp_out}"
        return 1
    fi
    log_info "'${bench_file_name}', '${exe_file_name}' and '${tmp_out}' was deleted successfully"
    log_info "Ended successfully"
}

main ${@}