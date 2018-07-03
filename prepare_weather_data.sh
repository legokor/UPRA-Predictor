cat "$1" | tail -n +4 | awk '{$1=$1};1'| awk '{print $2 " " $1 " " $3 " " $7 " " $8 * 0.514444444444}' > weatherdata.txt
