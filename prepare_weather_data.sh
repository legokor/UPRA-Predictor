cat "$1" | tr -s ' ' | cut --delimiter=' ' --fields=2-4,8,9 | tail -n +4 > weatherdata.txt
