#!/bin/bash

error_code=$1
error_message=""

case $error_code in
    0) error_message="Success";;
    1) error_message="Wrong number of arguments";;
    2) error_message="Source file not found";;
    3) error_message="File open error: $error_code";;
    4) error_message="File read error: $error_code";;
    5) error_message="File write error: $error_code";;
    6) error_message="File close error: $error_code";;
    7) error_message="Destination file not found";;
    *) error_message="Unknown error: $error_code";;
esac

# Print the error message
echo "$error_message"
