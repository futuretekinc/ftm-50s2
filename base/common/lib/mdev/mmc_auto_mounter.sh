#!/bin/sh

destdir=/opt

my_umount()
{
	if grep -qs "^/dev/$1 " /proc/mounts ; then
		umount "${destdir}";
	fi
}

my_mount()
{
	if ! mount -t auto -o sync "/dev/$1" "${destdir}"; then
# failed to mount, clean up mountpoint
		exit 1
	fi

	if [ -d "${destdir}/package" ] && [ -e "${destdir}/package/install.sh" ] ; then
		${destdir}/package/install.sh ${destdir}
	fi
}

case "${ACTION}" in
	add|"")
		my_umount ${MDEV}
		my_mount ${MDEV}
		;;
	remove)
		my_umount ${MDEV}
		;;
esac
