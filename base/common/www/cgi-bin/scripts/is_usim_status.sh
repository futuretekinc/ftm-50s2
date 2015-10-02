#!/bin/sh
retval=`/www/cgi-bin/scripts/is_registered.sh | awk 'BEGIN { retval="true" }\
{\
	if ($1 ~ /\+CREG:/)\
        {\
		count=split($2, fields, ",");\
                if (count == 2 &&  fields[2] == "3")\
                {\
			retval="false"\
                }\
	}\
} END { print retval }'`
echo $retval
