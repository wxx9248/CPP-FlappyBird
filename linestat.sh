git log --author="wxx9248" --pretty=tformat: --numstat | awk '{ add += $1; subs += $2; loc += $1 - $2 } END { printf "added lines %s, removed lines: %s, total lines: %s\n", add, subs, loc }' -
sleep 5
