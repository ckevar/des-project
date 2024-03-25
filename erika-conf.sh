erika-gen ~/Apps/Erika-CLI/  $1 $2

if [ $? -eq 0 ]; then
	sed -i '1i include user.mk' $2/makefile
fi

