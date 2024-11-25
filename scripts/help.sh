#!/bin/bash

# This script displays help information for the Makefile.
# Usage: ./help.sh Makefile

# colors
color_off='\033[0m'
target_color='\033[36m'
variable_color='\033[93m'
grey='\033[90m'

help() {
    echo "Usage:"
    printf "  make %b[target]%b %b[variables]%b\n\n" "$target_color" "$color_off" "$variable_color" "$color_off"
    
    _help_targets "$1"
    
    #_help_variables "$1"
    
    #_help_examples
}

_help_targets() {
    local pattern
    pattern='^[a-zA-Z0-9._-]+:.*?##.*$'
    
    echo "Target(s):"
    grep -E "$pattern" "$1" | sort | 
        while read -r line; do
            target=${line%%:*}
            description=${line#*## }
            printf "  %b%-14s%b%s\n" "$target_color" "$target" "$color_off" "$description"
        done
    echo ""
}

_help_variables() {
    local pattern
    pattern='^[a-zA-Z0-9_-]+ [:?!+]?=.*?##.*$'
    
    echo "Variable(s):"
    grep -E "$pattern" "$1" | sort | 
        while read -r line; do
            variable=${line%% *}
            default=${line#*= }
            default=${default%%##*}
            description=${line##*## }
            printf "  %b%-30s%b%s %b(default: %s)%b\n" "$variable_color" "$variable" "$color_off" "$description" "$grey" "$default" "$color_off"
        done
    echo ""
}

# Function to display examples
_help_examples() {
    echo "Example(s):"
    echo "  make build-image"
}


help "$1"

# Return exit code indicating success
exit 0